/**
 * Created on: 06.06.2016
 * Author: Alija Sabic
 * E-Mail: sabic.alija@gmail.com
 *  
 * Copyright (C) 2016  Alija Sabic
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
package com.sabic.alija.irrigator_;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.io.IOException;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;


/**
 * Created by Alija on 21.05.2016.
 */
public class NodeAdapter extends BaseAdapter{

    /**
     * Variables
     *
    /*/
    /* List of NodeListContainer Items of NodeAdapter */
    private List<NodeListContainer> mNodeList;
    /* Layout Inflater */
    private final LayoutInflater mInflater;
    /* NodeAdapter */
    private final NodeAdapter mAdapter;


    /**
     * Network Interface Parameter
     *
    /*/
    /* Network Interface */
    private NetworkInterface mNet;
    /* Internet Address */
    private InetAddress mInet;
    /* Subnet Mask */
    private int mSubnet;
    /* Ping Response Time */
    private final int mPingResponseTime = 700;


    /**
     * Thread Execution Variables
     *
    /*/
    /* Gets the number of available cores (not always the same as the maximum number of cores) */
    private static int NUMBER_OF_CORES = Runtime.getRuntime().availableProcessors();
    /* Sets the amount of time an idle thread waits before terminating */
    private static final int KEEP_ALIVE_TIME = 1;
    //private static final int KEEP_ALIVE_TIME = 550;
    /* Sets the Time Unit to seconds */
    private static final TimeUnit KEEP_ALIVE_TIME_UNIT = TimeUnit.SECONDS;
    //private static final TimeUnit KEEP_ALIVE_TIME_UNIT = TimeUnit.MILLISECONDS;


    /**
     * Thread Execution
     *
    /*/
    /* A queue of Runnables */
    private final BlockingQueue<Runnable> mDecodeWorkQueue;


    /* Thread Pool */
    public final ThreadPoolExecutor mDecodeThreadPool;
    /* Thread Pool (Cached) */
    private ExecutorService mCachedPool;
    private ExecutorService mFixedPool;


    /**
     * Container Classes
     *
    /*/
    public class NetworkContainer {
        int listIndex;

        /**
         * Network Container Constructor
         * @param listIndex
        /*/
        public NetworkContainer(int listIndex) {
            this.listIndex = listIndex;
        }
    }
    public class NodeContainer {
        InetAddress net;
        int subnet;

        /**
         * Node Container Constructor
         * @param net
         * @param subnet
        /*/
        public NodeContainer(InetAddress net, int subnet) {
            this.net = net;
            this.subnet = subnet;
        }

    }
    public class NodeListContainer {
        InetAddress net;
        String hostname;
        String ip;
    }
    public class ViewHolder {
        TextView name;
        TextView address;
    }
    public class PingContainer {
        InetAddress addr;
        int count;

        /**
         * Ping Container Constructor
         * @param addr
         * @param count
        /*/
        public PingContainer(InetAddress addr, int count) {
            this.addr = addr;
            this.count = count;
        }
    }


    /**
     * Constructor
     *
    /*/
    public NodeAdapter(final Context context, final int listIndex) {

        /** Instantiates the queue of Runnables as a LinkedBlockingQueue */
        mDecodeWorkQueue = new LinkedBlockingQueue<Runnable>();
        //mDecodeWorkQueue = new LinkedBlockingQueue<Runnable>(254);

        /** Create Thread pool(s). Used for different performance tests */
        mCachedPool = Executors.newCachedThreadPool();
        mFixedPool = Executors.newFixedThreadPool(254);

        /** Setup list adapter */
        mAdapter = this;
        mNodeList = new ArrayList<NodeListContainer> ();
        mInflater = LayoutInflater.from(context);

        /** Create thread pool */
        mDecodeThreadPool = new ThreadPoolExecutor(
                8, //NUMBER_OF_CORES,       // Initial pool size
                8, //NUMBER_OF_CORES,       // Max pool size
                KEEP_ALIVE_TIME,
                KEEP_ALIVE_TIME_UNIT,
                mDecodeWorkQueue);

        /** Load network interface and initiate ping */
        new getNetworkInterfaceTask().execute(new NetworkContainer(listIndex));
    }



    /**
     * Update List Handler
     *
    /*/
    public void updateList() {

        /** Shutdown ThreadExecutor */
        //mDecodeThreadPool.shutdownNow();

        /** Intiate ping */
        new updateNodeListTask().execute(new NodeContainer(mInet, mSubnet));
    }



    /**
     * AsyncTask Handlers
     *
    /*/
    /* Get Network Interface */
    private class getNetworkInterfaceTask
            extends AsyncTask<NetworkContainer, NetworkInterface, NodeContainer> {

        /**
         * doInBackground function loading the selected network interface
         * based on the result received from InterfaceListActivity asynchronously
         * in a separate thread (sequential).
         * @param networkContainer
         * @return Found internet address and subnet per NodeContainer
         */
        @Override
        protected NodeContainer doInBackground(NetworkContainer... networkContainer) {

            try {

                /** Get selected network interface */
                mNet = Collections.list(NetworkInterface.getNetworkInterfaces())
                        .get(networkContainer[0].listIndex);

                /** Counter for correct selection at subnet calculation */
                int i = 0;

                /** Load all available internet addresses */
                for(InetAddress inet : Collections.list(mNet.getInetAddresses())) {

                    /** Check for IPv4 addresses */
                    if(Inet4Address.class.isAssignableFrom(inet.getClass())) {

                        mInet = inet;
                        mSubnet = NetworkInterface.getByInetAddress(mInet)
                                        .getInterfaceAddresses()
                                        .get(i)
                                        .getNetworkPrefixLength();

                        break;
                    }
                    i++;
                }
            } catch (SocketException e) {
                e.printStackTrace();
            }

            return new NodeContainer(mInet, mSubnet);
        }

        /**
         * onPostExecute function initiating ping and passing
         * result per NodeContainer
         * @param nodeContainer
         */
        @Override
        protected void onPostExecute(NodeContainer nodeContainer) {
            super.onPostExecute(nodeContainer);

            /** Start updateNodeListTask */
            if(nodeContainer != null)
                new updateNodeListTask().execute(nodeContainer);
        }
    }
    /* Search responding Nodes */
    public class updateNodeListTask
            extends AsyncTask<NodeContainer, PingContainer, Void> {

        /**
         * @brief doInBackground function calculating valid
         * internet address asynchronously and passing it to separate
         * thread (sequential).
         * @param node
         * @return
         */
        @Override
        protected Void doInBackground(NodeContainer... node) {

            if(mInet != null) {

                mNodeList = new ArrayList<NodeListContainer>();

                /** Generate subnet mask */
                int subnetMask = 0xFFFFFFFF << (32 - node[0].subnet);

                /** Calculate number of clients */
                int numClients = (2 << (31 - node[0].subnet)) - 2;
                final byte[] subnetMaskByte = new byte[]{
                        (byte) (subnetMask >> 24),
                        (byte) (subnetMask >> 16),
                        (byte) (subnetMask >> 8),
                        (byte) (subnetMask)};

                /** Calculate internet address */
                final byte[] inetByte = new byte[4];

                /** Store own internet address */
                byte[] ownInetByte = mInet.getAddress();
                for (int i = 0; i < 4; i++) {
                    inetByte[i] = (byte) (ownInetByte[i] & subnetMaskByte[i]);
                    if (i == 3) inetByte[3]++;
                }


                /** Load every possible network host based on
                 * subnetmask and internet address */
                for (int i = 0; i < numClients; i++) {
                    InetAddress addr = null;
                    PingContainer ping = null;

                    try {
                        addr = InetAddress.getByAddress(inetByte);

                        /** Load PingContainer with result for net thread */
                        ping = new PingContainer(addr, i);

                        /** Publish progress to onProgressUpdate */
                        publishProgress(ping);
                    } catch (UnknownHostException e) {
                        e.printStackTrace();
                    }

                    /** Increment host counter */
                    /** TODO: Implement correct increment calculation */
                    if (i > 255) {
                        inetByte[2]++;
                    } else {
                        inetByte[3]++;
                    }

                }
            }
            return null;
        }

        /**
         * @brief onProgressUpdate function starting ping asynchronously
         * in different Thread Pool Executor setups (concurrent/sequential).
         * @param ping
         */
        @Override
        protected void onProgressUpdate(PingContainer... ping) {
            super.onProgressUpdate(ping);

            final PingContainer pingCnt = ping[0];

            /** Execute ping on initially created Thread Pool */
            new pingNodeTask().executeOnExecutor(mDecodeThreadPool, pingCnt);

            //new pingNodeTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR ,ping[0]);

            /* /
            mCachedPool.execute(new Runnable() {
                @Override
                public void run() {
                    new pingNodeTask().execute(pingCnt);
                }
            });
            /* */

            /* /
            mFixedPool.execute(new Runnable() {
                @Override
                public void run() {
                    new pingNodeTask().execute(pingCnt);
                }
            });
            /* */

            /* /
            mDecodeThreadPool.execute(new Runnable() {
               @Override
               public void run() {
                   new pingNodeTask().execute(pingCnt);
               }
            });
            /* */

            /* /
            if( pingCnt.count < 120 )
                new pingNodeTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, pingCnt);
            else if( pingCnt.count < 150 )
                new pingNodeTask().execute(pingCnt);
            else
                new pingNodeTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, pingCnt);
            /* */
        }
    }
    /* Ping Nodes */
    public class pingNodeTask
            extends AsyncTask<PingContainer, Void, NodeListContainer> {

        /**
         * @brief doInBackground function starting ping asynchronously
         * (concurrent/sequential). Adds internet hosts on positive
         * results.
         * @param ping
         * @return
         */
        @Override
        protected NodeListContainer doInBackground(PingContainer... ping) {
            String[] nodeText;
            NodeListContainer nodeListContainer = new NodeListContainer();

            Log.d("pingNodeTask start", ping[0].addr.toString().split("/")[1]);

            try {

                /** Ping host */
                if (ping[0].addr.isReachable(mPingResponseTime)) {

                    /** Load NodeListContainer with host data */
                    nodeText = ping[0].addr.toString().split("/");
                    nodeListContainer.net = ping[0].addr;
                    nodeListContainer.hostname = "Hostname \t\t" + ping[0].addr.getHostName();
                    nodeListContainer.ip = "IP Address \n\n\t\t\t\t\t\t\t\t\t" + nodeText[1];
                }
            } catch (IOException e) {
                e.printStackTrace();
            }

            Log.d("pingNodeTask response", ping[0].addr.toString().split("/")[1]);

            /** Pass result to onPostExecute */
            return nodeListContainer;
        }

        /**
         * @brief onPostExecute function handling responding nodes
         * @param nodeList
         */
        @Override
        protected void onPostExecute(NodeListContainer nodeList) {
            super.onPostExecute(nodeList);

            if(nodeList.ip != null) {

                //String compareString = new String("/").concat(nodeList.ip.toString());
                String[] ip = nodeList.ip.toString().split("\t");
                ip[ip.length-1] = "/" + ip[ip.length-1];

                /* Filter own Ip Address */
                if (!ip[ip.length-1].equals(mInet.toString())) {

                    /** Add found nodes */
                    mNodeList.add(nodeList);

                    /** Notify adapter */
                    mAdapter.notifyDataSetChanged();
                }
            }
        }
    }



    /**
     * BaseAdapter Interface
     *
    /*/
    @Override
    public int getCount() {
        return mNodeList.size();
    }
    @Override
    public Object getItem(int position) {
        return mNodeList.get(position);
    }
    @Override
    public long getItemId(int position) {
        return position;
    }
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder;

        /** Load container */
        if(convertView == null) {
            convertView = mInflater.inflate(R.layout.node_list_item, parent, false);

            holder = new ViewHolder();
            holder.name = (TextView) convertView.findViewById(R.id.node_item_name);
            holder.address = (TextView) convertView.findViewById(R.id.node_item_address);

            convertView.setTag(holder);
        } else {
            holder = (ViewHolder) convertView.getTag();
        }

        /** Load node data from adapter */
        NodeListContainer node = (NodeListContainer) getItem(position);

        /** Load data into container */
        holder.name.setText(node.hostname);
        holder.address.setText(node.ip);

        return convertView;
    }
}
