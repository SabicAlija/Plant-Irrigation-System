package com.sabic.alija.irrigator_;

import android.util.MutableInt;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;

/**
 * Created by Alija on 24.05.2016.
 */
public class PlantIrrigationSystemClient {

    public static InetAddress SERVER_IP;
    public static final int SERVER_PORT = 36363;
    private OnPlantIrrigationMessageReceived mMessageListener;
    private BufferedOutputStream mOut;
    private BufferedInputStream mIn;
    private boolean mRun = false;
    private PlantIrrigationMessage mMessage;
    private MutableInt mActiveLine;
    private Socket mSocket = null;

    /**
     * Plant Irrigation System Client Constructor
     * @param listener  Message Received callback function
     * @param server    Plant Irrigation Server
     */
    public PlantIrrigationSystemClient (OnPlantIrrigationMessageReceived listener,
                                        InetAddress server) {
        this.mMessageListener = listener;
        this.SERVER_IP = server;
    }

    /**
     * Getter
    /*/
    public MutableInt getActiveLine() {
        return mActiveLine;
    }

    /**
     * Send Plant Irrigation Message to connected server.
     * @param message   Message containing command and Plant Irrigation System Intent
     */
    public void send(PlantIrrigationMessage message) {

        try {

            /** Create socket with server ip and port for plant irrigation
             * command (36363)
            /*/
            mSocket = new Socket(SERVER_IP, SERVER_PORT);

            /** Create buffered output stream */
            mOut = new BufferedOutputStream(new DataOutputStream(mSocket.getOutputStream()));

            /** Create buffered input stream */
            mIn = new BufferedInputStream(new DataInputStream(mSocket.getInputStream()));

            /** Check if output stream creation was successful */
            if (mOut != null) {

                /** Write currently selected line */
                mOut.write(message.getActiveLine());

                /** Write command counter */
                mOut.write(message.getCmdCount());

                /** Write all commands */
                for (Integer i : message.getCmd()) {
                    mOut.write(i.intValue());
                }

                /** Finally, write Plant Irrigation System Intent */
                writeSystem(mOut, message.getSystem());

                /** Flush data output stream buffer */
                mOut.flush();

            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Receive Plant Irrigation Message to connected server.
     * @param in    Buffered Input Stream
     * @return      Received Plant Irrigation System Message
     */
    public PlantIrrigationMessage receive(BufferedInputStream in) {

        PlantIrrigationMessage message = null;
        ArrayList<Integer> cmd = new ArrayList<Integer>();
        int i;

        /** Load command counter to check read value */
        int cmdCount = -1;

        try {

            /** Check if buffered data input stream is created */
            if(in != null) {

                /** Read data from input buffer */
                cmdCount = in.read();

                /** Check if active line value was received and if message listener callback
                 * function is available */
                if (cmdCount >= 0 && mMessageListener != null) {

                    /** Load command counter */
                    cmdCount = in.read();

                    /** Read each command */
                    for (i = 0; i < cmdCount; i++) {
                        cmd.add(new Integer(in.read()));
                    }

                    /** Create new Plant Irrigation System object used for storing received
                     * Plant Irrigation System
                    /*/
                    PlantIrrigationSystem txSystem = new PlantIrrigationSystem(false,
                                                                                false,
                                                                                false,
                                                                                false);

                    /** Read result of Plant Irrigation System intent */
                    readSystem(mIn, txSystem);

                    /** Create and load new Plant Irrigation Message */
                    mMessage = new PlantIrrigationMessage(0, cmd, txSystem);

                    /** Pass message to message received callback function */
                    mMessageListener.plantIrrigationMessageReceived(mMessage);

                    /** Store message for later handling */
                    message = mMessage;

                    /** Empty receive buffer */
                    while (in.read() != -1) ;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        finally {
            try {

                /** Check if message was read and if socket exists */
                if ((cmdCount > 0) && (mSocket != null)) {

                    /** Close socket */
                    mSocket.close();
                    mSocket = null;
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return message;
    }

    /**
     * Write Plant Irrigation System Intent into client message
     * @param out           Buffer output stream
     * @param system        Plant Irrigation System Intent
     * @throws IOException
     */
    public void writeSystem(BufferedOutputStream out, PlantIrrigationSystem system) throws IOException {

        /** Start with state of automatic irrigation for each line */
        for(int i = 0; i < 4; i++) {
            if(system.getState(i)) out.write(0x01);
            else out.write(0x00);
        }

        /** Write pump duty cycle and state */
        out.write(system.getPump().getDutycycle());
        out.write(system.getPump().getState());

        /** Write each moisture sensor result and configuration parameter */
        for(int i = 0; i < 4; i++) {
            out.write(system.getMoistureSensor(i).getCurrentValue());
            out.write(system.getMoistureSensor(i).getThresholdLow());
            out.write(system.getMoistureSensor(i).getThresholdHigh());
        }

        /** Write each valve state */
        for(int i = 0; i < 4; i++) {
            if(system.getValve(i).isOpen()) out.write(0x01);
            else out.write(0x00);
        }

    }

    /**
     * Read Plant Irrigation System Intent from server message
     * @param in
     * @param system
     * @throws IOException
     */
    public void readSystem(BufferedInputStream in, PlantIrrigationSystem system) throws IOException {

        /** Read state of automatic irrigation for each line */
        for(int i = 0; i < 4; i++) {
            if(in.read() != 0x00) system.activateLine(i);
            else system.deactivateLine(i);
        }

        /** Read duty cycle and state */
        system.getPump().setDutycycle(in.read());
        system.getPump().setState(in.read());

        /** Read each moisture sensor result and configuration */
        for(int i = 0; i < 4; i++) {
            system.getMoistureSensor(i).setCurrentValue(in.read());
            system.getMoistureSensor(i).setThresholdLow(in.read());
            system.getMoistureSensor(i).setThresholdHigh(in.read());
        }

        /** Read state of each valve */
        for(int i = 0; i < 4; i++) {
            if(in.read() != 0x00) system.getValve(i).open();
            else system.getValve(i).close();
        }
    }

    /** Stop Client */
    public void stop() {
        mRun = false;
    }

    /**
     * Initialize Client Server communication
     * @param system        Plant Irrigation System
     * @throws IOException
     */
    public void init(PlantIrrigationSystem system) throws IOException {

        /** Update client status */
        mRun = true;

        /** Poll for incoming messages */
        while(mRun) {
            mMessage = receive(mIn);
            mMessage = null;
        }
    }

    /** Message Receive callback interface */
    public interface OnPlantIrrigationMessageReceived {
        public void plantIrrigationMessageReceived(PlantIrrigationMessage message);
    }
}
