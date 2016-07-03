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
/*/
public class Interface {

    /**
     * OnItemClickListener for Items of Interface List on InterfaceListActivity
     *
    /*/
    public class OnItem implements AdapterView.OnItemClickListener {
        private Context mContext;
        private Context mListView;
        private InterfaceAdapter mAdapter;

        /**
         * OnItem callback function constructor
         * @param context
         * @param listView
        /*/
        public OnItem(Context context, Context listView, InterfaceAdapter interfaceAdapter) {
            this.mContext = context;
            this.mListView = listView;
            this.mAdapter = interfaceAdapter;
        }

        /**
         * OnItemClick callback function handling clicks
         * on interface list items.
         * @param parent
         * @param view
         * @param position
         * @param id
        /*/
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            Intent intent = new Intent(mContext, NodeListActivity.class);

            InterfaceAdapter.InterfaceContainer netContainer =
                    (InterfaceAdapter.InterfaceContainer) mAdapter.getItem(position);

            /** Load position of selected interface for Node Activity */
            intent.putExtra("Interface",netContainer.mPosition);

            /** Start NodeListActivity */
            mListView.startActivity(intent);
        }
    }

    /**
     * OnClickListener for Refresh FAB on InterfaceListActivity
     *
    /*/
    public class OnRefresh implements View.OnClickListener {

        private InterfaceAdapter mInterfaceAdapter;

        /**
         * OnRefresh callback function constructor
         * @param adapter
        /*/
        public OnRefresh(InterfaceAdapter adapter) {
            this.mInterfaceAdapter = adapter;
        }

        /**
         * onClick callback function handling clicks on
         * Refresh FloatingActionButton.
         * @param v
        /*/
        @Override
        public void onClick(View v) {

            /** Load list of interfaces */
            mInterfaceAdapter.updateList();
        }
    }

}
