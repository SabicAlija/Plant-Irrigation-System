/*
 * Plant_Irrigation_Server.c
 *
 *  Created on: 27.05.2016
 *      Author: Alija
 */
#include "Plant_Irrigation_Server.h"

extern Mailbox_Handle 	PlantIrrigationMailboxRx;		/**< Receive Mailbox for Control Requests */
extern Mailbox_Handle 	PlantIrrigationMailboxTx;		/**< Transmit Mailbox for Control Response and Echo Requests */

/**
 *    ======== Plant_Irrigation_Server.c ========
 *    Contains non-BSD sockets code (NDK Network Open Hook)
 */


/**
 *  ======== PlantIrrigationServerOpenHook ========
 *  NDK network open hook used to initialize IPv4
 */
void PlantIrrigationServerOpenHook()
{
	Task_Handle 	PlantIrrigationServerTask;
	Task_Params 	taskParams;
	Error_Block 	eb;

	Error_init(&eb);

	/**
	 * Create the Task that handles incoming Control Request
	 * for Plant Irrigation System.
	 */
	Task_Params_init(&taskParams);
	taskParams.stackSize = PLANTIRRIGATIONSERVERSTACK;
	taskParams.priority = 1;
	taskParams.arg0 = PLANTPORT;

	PlantIrrigationServerTask = Task_create((Task_FuncPtr)PlantIrrigationServer, &taskParams, &eb);
	if (PlantIrrigationServerTask == NULL) {
		System_printf("PlantIrrigationServerOpenHook: Task creation failed.\n");
	}

	System_flush();
}


/**
 *  ======== PlantIrrigationServer ========
 *  Creates new Task to handle incoming control request.
 */
Void PlantIrrigationServer(UArg arg0, UArg arg1) {

	int					server;
	int					status;
	int					clientfd;

	int					optval;
	int					optlen = sizeof(optval);

	/** Socket Addresses */
	struct sockaddr_in 	localAddr;
	struct sockaddr_in	clientAddr;
	socklen_t			addrlen = sizeof(clientAddr);

	/** Plant Worker - Task */
	Task_Handle			PlantWorkerTask;
	Task_Params			taskParams;
	Error_Block			eb;


	/** Create socket for server - client communciation */
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == -1) {
		System_printf("Error: socket creation failed.\n");
		goto shutdown;
	}

	/** Reset localAddr and load parameter */
	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY); //Host to Network Long
	localAddr.sin_port = htons(arg0); //Host to Network Short

	/** Bind server on port for control request.. */
	System_printf("..binding TCP-Port %d\n", arg0);
	status = bind(server, (struct sockaddr *)&localAddr, sizeof(localAddr));
	if (status == -1) {
		System_printf("Server - Error: bind failed.\n");
		goto shutdown;
	}

	/** ..to listen on Port and limit amount of parallel requests. */
	status = listen(server, NUMPLANTWORKERS);
	if (status == -1) {
		System_printf("Server - Error: listen failed.\n");
		goto shutdown;
	}

	/** Set socket options */
	optval = 1;

	//if (setsockopt(server, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
	if (setsockopt(server, SOL_SOCKET, TCP_NODELAY, &optval, optlen) < 0) {
		System_printf("Server - Error: setsockopt failed.");
		goto shutdown;
	}


	/** Accept incoming control request and pass handling to PlantWorkerTasks. */
	while ((clientfd =
			accept(server, (struct sockaddr *)&clientAddr, &addrlen)) != -1) {


		System_printf("PlantIrrigationServer: Creating thread clientfd = %d\n", clientfd);

		Error_init(&eb);

		/** Create PlantWorkerTask to handle incoming control request */
		Task_Params_init(&taskParams);
		taskParams.arg0 = (UArg)clientfd;
		taskParams.stackSize = 2048;
		PlantWorkerTask = Task_create((Task_FuncPtr)PlantWorker, &taskParams, &eb);
		if (PlantWorkerTask == NULL) {
			System_printf("Server - Error: PlantWorker creation failed.\n");

			/** Close accepted client socket */
			close(clientfd);
		}

		/** addrlen is a result-parameter and must be reseted. */
		addrlen = sizeof(clientAddr);

	}

shutdown:
	if (server > 0) {
		close(server);
	}

}



/**
 *  ======== PlantWorker ========
 *  PlantWorker Task handles incomding control request.
 *  Multiple Values can be instantiated at a time, but
 *  amount is limited.
 */
Void PlantWorker(UArg arg0, UArg arg1) {

	int 	clientfd = (int)arg0;
	int 	bytesRcvd;
	char 	buffer[PLANTPACKETSIZE];
	MsgObj  msg;


	System_printf("PlantWorker: start clientfd = 0x%x\n", clientfd);

	/**
	 * socket not flushed before send buffer is full, therefore socket
	 * needs to be closed to allow working communication
	 */
	/**
	while ((bytesRcvd = recv(clientfd, buffer, PLANTPACKETSIZE, 0)) > 0) {

		bytesSent = send(clientfd, buffer, bytesRcvd, 0);

		if (bytesSent < 0 || bytesSent != bytesRcvd) {
		System_printf("PlantWorker - Error: send failed.\n");
			break;
		}
		break;

	} */

	/** Load incoming control or echo request into buffer */
	bytesRcvd = recv(clientfd, buffer, PLANTPACKETSIZE, 0);

	/** Load message object for sending */
	msg.clientfd = clientfd;
	msg.length = bytesRcvd;
	memcpy(msg.buffer, buffer, sizeof(buffer));

	/** Send message to PlantIrrigationSystem Task */
	Mailbox_post(PlantIrrigationMailboxRx, &msg, BIOS_WAIT_FOREVER);

	/** Pend on incoming echo or control response messages */
	Mailbox_pend(PlantIrrigationMailboxTx, &msg, BIOS_WAIT_FOREVER);

	/** Send buffer from message to client */
	send(clientfd, msg.buffer, msg.length, 0);

	System_printf("PlantWorker: stop clientfd = 0x%x\n", clientfd);
	System_flush();

	close(msg.clientfd);
}


