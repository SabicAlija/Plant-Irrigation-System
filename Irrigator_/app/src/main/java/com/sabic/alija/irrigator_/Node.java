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
