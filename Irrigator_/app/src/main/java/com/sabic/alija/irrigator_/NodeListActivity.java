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

import android.app.ListActivity;
import android.os.Bundle;
import android.os.Parcel;
import android.os.Parcelable;
import android.support.design.widget.FloatingActionButton;
import android.util.Log;

/**
 * Created by Alija on 21.05.2016.
 */
public class NodeListActivity extends ListActivity {

    /**
     * Variables
     *
    /*/
    /* NodeAdapter */
    private NodeAdapter mNodeAdapter;
    /* Selected NetworkInterface */
    private int mNetworkInterfaceIndex;
    /* Node Class Instance */
    private Node mNode;

    /**
     * Container Classes
    /*/
    public static class NodeIntentContainer implements Parcelable {
        private String mIp;
        private int mNetworkInterfaceIndex;

        /**
         * NodeIntentContainer constructor class
         * @param ip
         * @param netIdx
        /*/
        public NodeIntentContainer(String ip, int netIdx) {
            this.mIp = ip;
            this.mNetworkInterfaceIndex = netIdx;
        }

        /**
         * Getter
        /*/
        public String getIp() {
            return mIp;
        }

        public int getNetworkInterfaceIndex() {
            return mNetworkInterfaceIndex;
        }

        /**
         * Parelable Interface
        /*/
        @Override
        public int describeContents() {
            return 0;
        }

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            dest.writeString(mIp);
            dest.writeInt(mNetworkInterfaceIndex);
        }

        public static final Parcelable.Creator<NodeIntentContainer> CREATOR =
                new Parcelable.Creator<NodeIntentContainer>() {
            @Override
            public NodeIntentContainer createFromParcel(Parcel source) {
                return new NodeIntentContainer(source);
            }

            @Override
            public NodeIntentContainer[] newArray(int size) {
                return new NodeIntentContainer[size];
            }
        };

        private NodeIntentContainer(Parcel source) {
            mIp = source.readString();
            mNetworkInterfaceIndex = source.readInt();
        }

    }

    /**
     * ListActivity Interface
     *
    /*/
    /**
     * onCreate hook function loading intent data,
     * configuring adapter and loading user interface.
     * @param savedInstanceState
    /*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        int position;
        Bundle bundle;

        /** Get NetworkInterface Position of selected interface from
         * Bundle */
        bundle = getIntent().getExtras();
        if(bundle != null) {
            position = bundle.getInt("Interface");
            Log.d("Node List", String.valueOf(position));
        } else {
            position = Integer.getInteger((String) savedInstanceState.get("Interface"));
            Log.d("Node List", String.valueOf(position));
        }

        /* Load class variables */
        mNetworkInterfaceIndex = position;
        mNodeAdapter = new NodeAdapter(this, position);
        mNode = new Node();

        /* Configure Adapter */
        setListAdapter(mNodeAdapter);

        /* Load layout */
        setContentView(R.layout.node_list);
    }

    /**
     * onStart hook function loading listeners for ui node components
     * and list adapter
    /*/
    @Override
    protected void onStart() {
        super.onStart();

        /* Set FAB Click Handler */
        FloatingActionButton fabNodeRefresh = (FloatingActionButton)
                findViewById(R.id.fabNodeRefresh);
        fabNodeRefresh.setOnClickListener(mNode.new OnRefresh(mNodeAdapter));

        /* List Item Click Handler */
        getListView().setOnItemClickListener(mNode.new OnItem(getApplicationContext(),
                this, mNodeAdapter, mNetworkInterfaceIndex));
    }
}
