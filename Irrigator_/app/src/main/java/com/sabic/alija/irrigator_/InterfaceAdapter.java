/**
 *  Created on: 06.06.2016
 *  Author: Alija Sabic
 *  E-Mail: sabic.alija@gmail.com
 *  
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 * 
 * Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 */
package com.sabic.alija.irrigator_;

import android.content.Context;
import android.os.AsyncTask;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;

/**
 * Created by Alija on 20.05.2016.
 */
public class InterfaceAdapter extends BaseAdapter {

    /**
     * Variables
     *
    /*/
    /* List of NetworkInterface Items of InterfaceAdapter */
    private List<InterfaceContainer> mNets;
    /* Layout Inflater */
    private final LayoutInflater mInflater;
    /* InterfaceAdapter */
    private final InterfaceAdapter mAdapter;

    /**
     * Container Classes
     *
    /*/
    static class ViewHolder {
        TextView display_name;
        TextView name;
        TextView inet_address;
    }
    static class InterfaceContainer {
        int mPosition;
        NetworkInterface mInterface;

        public InterfaceContainer (int position, NetworkInterface net) {
            this.mPosition = position;
            this.mInterface = net;
        }
    }

    /**
     * Constructor
     *
    /*/
    public InterfaceAdapter(final Context context) {

        /** Setup list adapter */
        mAdapter = this;
        mNets = new ArrayList<InterfaceContainer>();
        mInflater = LayoutInflater.from(context);

        /** Load list of interfaces */
        updateList();

    }



    /**
     * Update List Handler
     *
    /*/
    public void updateList() {

        /** Get all network interfaces */
        new getNetworkInterfacesTask().execute();

    }


    /**
     * AsyncTask Handlers
     *
    /*/
    /* Get Network Interfaces */
    private class getNetworkInterfacesTask
            extends AsyncTask<Void, InterfaceContainer, Void> {

        /**
         * doInBackground function loading network interfaces
         * asynchronously in a separate thread (sequential).
         * After valid network interface is detected result is
         * published to onProgressUpdate. Final result is not
         * in use.
         * @param params
         * @return
         */
        @Override
        protected Void doInBackground(Void... params) {

            mNets = new ArrayList<InterfaceContainer>();
            Enumeration<NetworkInterface> nets;

            try {

                /** Get List of Network Interfaces */
                nets = NetworkInterface.getNetworkInterfaces();

                int i = 0;

                /** Step through all network interfaces in enumeration */
                for(NetworkInterface net : Collections.list(nets)) {

                    /** Check if interface contains any IP addresses and if
                     * it is not a loopback address.
                     */
                    if(Collections.list(net.getInetAddresses()).size() > 0 &&
                            !net.isLoopback()) {

                        /** Publish progress to onProgressUpdate */
                        publishProgress(new InterfaceContainer(i, net));
                    }

                    i++;
                }
            } catch(SocketException e) {
                e.printStackTrace();
            }
            return null;
        }

        /**
         * @brief OnProgressUpdate function adding network interfaces
         * and notifying adapter to reload list.
         * @param netContainer
         */
        @Override
        protected void onProgressUpdate(InterfaceContainer... netContainer) {
            super.onProgressUpdate(netContainer);

            /** Add found network interfaces */
            mNets.add(netContainer[0]);

            /** Notify adapter */
            mAdapter.notifyDataSetChanged();
        }
    }

    /**
     * BaseAdapter Interface
    /*/
    @Override
    public int getCount() {
        return mNets.size();
    }
    @Override
    public Object getItem(int position) {
        return mNets.get(position);
    }
    @Override
    public long getItemId(int position) {
        return position;
    }
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        ViewHolder holder;
        Context context = parent.getContext();
        InterfaceContainer net = (InterfaceContainer) getItem(position);
        Enumeration<InetAddress> inetAddresses = net.mInterface.getInetAddresses();

        /** Load container */
        if(convertView == null) {
            convertView = mInflater.inflate(R.layout.interface_list_item, parent, false);

            holder = new ViewHolder();
            holder.display_name = (TextView)
                    convertView.findViewById(R.id.interface_item_display_name);
            holder.name = (TextView)
                    convertView.findViewById(R.id.interface_item_name);
            holder.inet_address = (TextView)
                    convertView.findViewById(R.id.interface_item_inet_address);

            convertView.setTag(holder);
        } else {
          holder = (ViewHolder) convertView.getTag();
        }

        /** Load text into container item */
        holder.display_name.setText("Display Name: \t\t\t\t" + net.mInterface.getDisplayName());
        holder.name.setText("Name: \t\t\t\t\t\t\t\t\t\t\t\t\t" + net.mInterface.getName());
        if(net.mInterface.getInterfaceAddresses().size() > 1)
            holder.inet_address.setText("Internet Addresses:\n\n");
        else
            holder.inet_address.setText("Internet Address:\n\n");

        /** Add every internet address */
        for(InetAddress inetAddress : Collections.list(inetAddresses)) {
            holder.inet_address.append(inetAddress.toString() + "\n");
        }

        return convertView;
    }

}

