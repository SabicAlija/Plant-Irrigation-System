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

import android.app.ListActivity;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;

/**
 * Created by Alija on 20.05.2016.
 */
public class InterfaceListActivity extends ListActivity {
    /**
     * Variables
     *
    /*/
    /* InterfaceAdapter */
    private InterfaceAdapter mInterfaceAdapter;
    /* Interface Class Instance */
    private Interface mInterface;

    /**
     * ListActivity Interface
    /*/
    /**
     * onCreate hook function loading and configuring
     * adapter and loading user interface.
     * @param savedInstanceState
    /*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        /* Load class variables */
        mInterfaceAdapter = new InterfaceAdapter(this);
        mInterface = new Interface();

        /* Configure Adapter */
        setListAdapter(mInterfaceAdapter);

        /* Load layout */
        setContentView(R.layout.interface_list);

    }

    /**
     * onStart hook function loading listeners for ui
     * interface components and list adapter
    /*/
    @Override
    protected void onStart() {
        super.onStart();

        /* Set FAB Click Handler */
        FloatingActionButton fabRefresh = (FloatingActionButton) findViewById(R.id.fabInterfaceRefresh);
        fabRefresh.setOnClickListener( mInterface.new OnRefresh(mInterfaceAdapter));

        /* List Item Click Handler */
        getListView().setOnItemClickListener( mInterface.new OnItem(getApplicationContext(), this, mInterfaceAdapter));
    }

}
