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
import android.content.Intent;
import android.view.View;
import android.widget.AdapterView;

/**
 * Created by Alija on 23.05.2016.
 */
public class Node {

    /**
     * OnItemClickListener for Items of Node List on NodeListActivity
     *
    /*/
    public class OnItem implements AdapterView.OnItemClickListener{

        private Context mContext;
        private Context mListView;
        private NodeAdapter mNodeAdapter;
        private int mNetworkInterfaceIndex;

        /**
         * OnItem callback constructor
         * @param context
         * @param listView
         * @param nodeAdapter
         * @param networkInterfaceIndex
         */
        public OnItem(Context context, Context listView, NodeAdapter nodeAdapter, int networkInterfaceIndex) {
            this.mContext = context;
            this.mListView = listView;
            this.mNodeAdapter = nodeAdapter;
            this.mNetworkInterfaceIndex = networkInterfaceIndex;
        }

        /**
         * OnItemClick callback function handling clicks
         * on node list items.
         * @param parent
         * @param view
         * @param position
         * @param id
         */
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

            mNodeAdapter.mDecodeThreadPool.shutdownNow();

            Intent intent;
            String[] parseStr;
            NodeListActivity.NodeIntentContainer nodeIntentContainer;

            /** Get selected node */
            NodeAdapter.NodeListContainer node =
                    (NodeAdapter.NodeListContainer) mNodeAdapter.getItem(position);

            /** Parse IP Text */
            parseStr = node.ip.split("\t");

            /** Prepare Node Intent Container */
            nodeIntentContainer = new
                    NodeListActivity.NodeIntentContainer(parseStr[parseStr.length-1], mNetworkInterfaceIndex);

            /** Load Node Intent Container */
            intent = new Intent(mContext, ControlActivity.class);
            intent.putExtra("Node", position);
            intent.putExtra("Node", nodeIntentContainer);

            /** Start control activity */
            mListView.startActivity(intent);

        }
    }

    /**
     * OnClickListener for Refresh FAB on NodeListActivity
     *
     */
    public class OnRefresh implements View.OnClickListener {

        private NodeAdapter mNodeAdapter;

        /**
         * OnRefresh callback function constructor
         * @param adapter
         */
        public OnRefresh (NodeAdapter adapter) {
            this.mNodeAdapter = adapter;
        }

        /**
         * onClick callback function handling clicks on
         * Refresh FloatingActionButton.
         * @param v
         */
        @Override
        public void onClick(View v) {

            /** Load list of nodes */
            mNodeAdapter.updateList();
        }
    }


}
