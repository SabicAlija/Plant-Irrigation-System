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
