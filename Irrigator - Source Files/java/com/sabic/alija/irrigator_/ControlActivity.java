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

import android.content.Intent;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.PorterDuff;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Vibrator;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.util.MutableInt;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ImageView;

import java.io.IOException;
import java.net.InetAddress;
import java.util.ArrayList;

public class ControlActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private int mNetworkInterfaceIndex;
    private InetAddress mServerAddress;
    private String mServerAddressStr;
    private MutableInt mActiveLine;

    private PlantIrrigationSystem mPlantIrrigationSystem;
    private PlantIrrigationSystem mPlantIrrigationSystemIntent;


    /* Control Class Instance */
    private Control mControl;

    private PlantIrrigationSystemClient mPlantIrrigationSystemClient;

    /**
     * AppCompatActivity Interface
     *
    /*/

    /**
     * OnCreate hook function loading UI components
     * @param savedInstanceState
    /*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Toolbar toolbar;
        DrawerLayout drawer;
        ActionBarDrawerToggle toggle;
        NavigationView navigationView;
        Intent intent;
        NodeListActivity.NodeIntentContainer nodeIntentContainer;

        /** Load layout */
        setContentView(R.layout.control_main);

        /** Load class variables */;
        mControl = new Control();

        /** Load Intent Data */
        intent = getIntent();
        nodeIntentContainer = intent.getParcelableExtra("Node");

        /** Load ui components */
        toolbar = (Toolbar) findViewById(R.id.toolbar);
        drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        navigationView = (NavigationView) findViewById(R.id.nav_view);

        /** Set listener to notify when a menu item is clicked */
        navigationView.setNavigationItemSelectedListener(this);

        /** Setup activity toolbar */
        setSupportActionBar(toolbar);
        toggle = new ActionBarDrawerToggle(this, drawer, toolbar,
                R.string.navigation_drawer_open,
                R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();


        /** Load data from intent container */
        mServerAddressStr = nodeIntentContainer.getIp();
        mNetworkInterfaceIndex = nodeIntentContainer.getNetworkInterfaceIndex();

        /** Load Server Address */
        new loadServerAddressTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);

        /** Create Plant Irrigation System Object */
        mPlantIrrigationSystem = new PlantIrrigationSystem(PlantIrrigationSystem.DEACTIVATED,
                                                           PlantIrrigationSystem.DEACTIVATED,
                                                           PlantIrrigationSystem.DEACTIVATED,
                                                           PlantIrrigationSystem.DEACTIVATED);

        /** Create Plant Irrigation System Intent Object */
        mPlantIrrigationSystemIntent = PlantIrrigationSystem.getInstance(mPlantIrrigationSystem);
    }


    /**
     * onBackPressed hook function handling back button actions
    /*/
    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    /**
     * onCreateOptionsMenu hook function loading option menu
     * @param menu
     * @return
    /*/
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }


    /**
     * onOptionsItemSelected hook function on option menu item selected
     * @param item
     * @return
    /*/
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }


    /**
     * NavigationView.OnNavigationItemSelectedListener Interface
     *
    /*/
    /**
     * onNavigationItemSelected hook function on navigation item selected
     * @param item
     * @return
    /*/
    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {

        int id = item.getItemId();
        Intent intent = null;

        /** Check which navigation item is selected */
        switch(id) {
            case R.id.nav_plant1:
                mActiveLine = new MutableInt(PlantIrrigationSystem.LINE_ONE);
                break;

            case R.id.nav_plant2:
                mActiveLine = new MutableInt(PlantIrrigationSystem.LINE_TWO);
                break;

            case R.id.nav_plant3:
                mActiveLine = new MutableInt(PlantIrrigationSystem.LINE_THREE);
                break;

            case R.id.nav_plant4:
                mActiveLine = new MutableInt(PlantIrrigationSystem.LINE_FOUR);
                break;

            case R.id.nav_interface:
                intent = new Intent(getApplicationContext(), InterfaceListActivity.class);
                break;

            case R.id.nav_node:
                intent = new Intent(getApplicationContext(), NodeListActivity.class);
                intent.putExtra("Interface",mNetworkInterfaceIndex);
                break;

            default:
                break;
        }

        /** Create and load Control Fragment */
        mControl.new CntrFragment()
                .load(ControlActivity.this,
                        mActiveLine,
                        mPlantIrrigationSystemClient,
                        mPlantIrrigationSystem,
                        mPlantIrrigationSystemIntent);

        /** Start Interface/Node ListActivity if intent is loaded */
        if(intent != null)
            startActivity(intent);

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);

        return true;
    }

    /**
     * AsyncTask Handlers
    /*/
    /** Load Server Address */
    public class loadServerAddressTask
            extends AsyncTask<Void, Void, Void> {

        /**
         * doInBackground function loading server address
         * asynchronously in a separate thread (sequential).
         * @param params
         * @return Result is stored in global variable
        /*/
        @Override
        protected Void doInBackground(Void... params) {

            try {
                /** Load Server Address */
                mServerAddress = InetAddress.getByName(mServerAddressStr);
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }

        /**
         * @brief onPostExecute function connecting smartphone
         * to Plant Irrigation System asynchronously (sequential).
         * @param aVoid
        /*/
        @Override
        protected void onPostExecute(Void aVoid) {
            super.onPostExecute(aVoid);

            /** Connect to Plant Irrigation System */
            new connectPlantIrrigationSystemTask().execute();
        }
    }

    /** Connect to Plant Irrigation System */
    public class connectPlantIrrigationSystemTask
        extends AsyncTask<Void, PlantIrrigationMessage, Void> {

        /**
         * doInBackground function attaching message receive
         * listener which post results to OnProgressUpdate function.
         * @param params
         * @return
        /*/
        @Override
        protected Void doInBackground(Void... params) {

            /** Create Plant Irrigation System TCP Client */
            mPlantIrrigationSystemClient = new PlantIrrigationSystemClient(

                    /** Attach plantIrrigationMessageReceived Listener and server address */
                    new PlantIrrigationSystemClient.OnPlantIrrigationMessageReceived() {

                        /**
                         * Override plantIrrigationMessageReceived to pass received
                         * messages to onProgressUpdate
                         * @param message
                        /*/
                        @Override
                        public void plantIrrigationMessageReceived(PlantIrrigationMessage message) {
                            publishProgress(message);
                        }
                    }, mServerAddress);

            try {

                /** Initialize Plant Irrigation System client */
                mPlantIrrigationSystemClient.init(mPlantIrrigationSystem);
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onProgressUpdate(PlantIrrigationMessage... message) {
            super.onProgressUpdate(message);

            /** Handle incoming messages*/
            /** Check if control request was successful */
            if(processMessage(message[0])) {
                //TODO: Implement Update SUCCESSFULL
                Log.d("Control","Successfull");

            /** Echo Request if control request if Plant Irrigation System
             * state is invalid.
            /*/
            } else {
                //TODO: Implement Update UNSUCESSFULL / REQUEST Update
                Log.d("Control","Unsuccessfull");

                /** Load echo command */
                final ArrayList<Integer> mCmd = new ArrayList<Integer>() {{
                    add(PlantIrrigationMessage.PLANT_ECHO);
                }};

                /** Send echo command */
                if(mPlantIrrigationSystemClient != null) {

                    /** Run in an asynchronous thread */
                    Thread t = new Thread(new Runnable() {

                        /**
                         * @brief Send echo command to Plant Irrigation Server.
                        /*/
                        @Override
                        public void run() {
                            mPlantIrrigationSystemClient.send(new PlantIrrigationMessage(0, mCmd, mPlantIrrigationSystem));
                        }
                    });

                    /** Trigger thread */
                    t.start();
                }
            }

        }
    }

    /**
     * Process incoming messages (responses) from Plant
     * Irrigation Server.
     * @param message
     * @return
     */
    public boolean processMessage(PlantIrrigationMessage message) {

        boolean bResult = false;

        /** Create color filter for inactive images/buttons. */
        ColorMatrix mMatrix = new ColorMatrix();
        mMatrix.setSaturation(0);
        ColorMatrixColorFilter mColorFilter = new ColorMatrixColorFilter(mMatrix);

        /** Load ui components */
        final ImageView mImage = (ImageView) findViewById(R.id.control_image_view);
        final FloatingActionButton mFabValve =
                (FloatingActionButton) findViewById(R.id.fabValve);
        final FloatingActionButton mFabSettings =
                (FloatingActionButton) findViewById(R.id.fabSettings);
        final FloatingActionButton mFabPump =
                (FloatingActionButton) findViewById(R.id.fabPump);

        /** Load vibrator server */
        Vibrator mVibrator = (Vibrator) getApplication().getSystemService(VIBRATOR_SERVICE);
        long[] pattern = {0, 100, 0};


        /** Check if first command is response of echo command */
        if((message.getCmd().get(0)) == PlantIrrigationMessage.PLANT_ECHO) {
            Log.d("Control","Echo");

            /** Update Plant Irrigation System */
            for(int i = 0; i < 4; i++) {

                /** Update Irrigation Line State */
                if(message.getSystem().getState(i)) {
                    mPlantIrrigationSystem.activateLine(i);
                } else {
                    mPlantIrrigationSystem.deactivateLine(i);
                }

                /** Update Moisture Sensor Settings */
                mPlantIrrigationSystem.getMoistureSensor(i)
                        .setCurrentValue(message.getSystem()
                                .getMoistureSensor(i).getCurrentValue());
                mPlantIrrigationSystem.getMoistureSensor(i)
                        .setThresholdLow(message.getSystem()
                                .getMoistureSensor(i).getThresholdLow());
                mPlantIrrigationSystem.getMoistureSensor(i)
                        .setThresholdHigh(message.getSystem()
                                .getMoistureSensor(i).getThresholdHigh());

                /** Update Valve State */
                if(message.getSystem().getValve(i).isOpen()) {
                    mPlantIrrigationSystem.getValve(i).open();
                } else {
                    mPlantIrrigationSystem.getValve(i).close();
                }

            }

            /** Update Pump duty cycle and state */
            mPlantIrrigationSystem.getPump()
                    .setDutycycle(message.getSystem().getPump().getDutycycle());
            mPlantIrrigationSystem.getPump()
                    .setState(message.getSystem().getPump().getState());

            bResult = true;
        }

        /** Check if indent state equals received state of Plant Irrigation
         * System
         */
        else if(message.getSystem().equals(mPlantIrrigationSystemIntent)) {

            /** Check if current state stored equals received state of
             * Plant Irrigation System
              */
            if(!message.getSystem().equals(mPlantIrrigationSystem)) {

                /** Iterate over received updates/command responses */
                for(int i = 0; i < message.getCmdCount(); i++) {

                    /** Handle received update/command response individually */
                    switch(message.getCmd().get(i)) {

                        case PlantIrrigationMessage.PLANT_ECHO:
                            // TODO: Implement Echo
                            break;

                        case PlantIrrigationMessage.PLANT_AUTOMAT_ON:
                            /** Check if automatic irrigation was activated */
                            if(message.getSystem().getState(mActiveLine.value))
                                mPlantIrrigationSystem.activateLine(mActiveLine.value);

                            /** Clear color filter from plant image */
                            mImage.setColorFilter(null);
                            Log.d("Control:Aut_ON: Line", Integer.toString(mActiveLine.value + 1));
                            break;

                        case PlantIrrigationMessage.PLANT_AUTOMAT_OFF:
                            /** Check if automatich irrigation was deactivated */
                            if(!message.getSystem().getState(mActiveLine.value))
                                mPlantIrrigationSystem.deactivateLine(mActiveLine.value);

                            /** Set color filter for plant image */
                            mImage.setColorFilter(mColorFilter);
                            Log.d("Control:Aut_OFF: Line", Integer.toString(mActiveLine.value + 1));
                            break;

                        case PlantIrrigationMessage.PLANT_VALVE_OPEN:
                        case PlantIrrigationMessage.PLANT_VALVE_CLOSE:

                            /** Check if valve was opened */
                            if(message.getSystem().getValve(mActiveLine.value).isOpen()) {

                                /** Open valve of active line */
                                mPlantIrrigationSystem.getValve(mActiveLine.value).open();

                                /** Load valve open image for valve FAB */
                                ((ImageView) mFabValve)
                                        .setImageResource(R.drawable.ic_fab_valve_open);
                                /** Set color of valve FAB to black */
                                mFabValve
                                        .setBackgroundTintList(ColorStateList.valueOf(Color.BLACK));

                                /** Activate pump FAB */
                                mFabPump.setEnabled(true);
                                /** Set color of pump FAB to blue */
                                ((ImageView) mFabPump)
                                        .getBackground()
                                        .setColorFilter(Color.parseColor("#FF005599"),
                                                PorterDuff.Mode.MULTIPLY);

                            /** Check if valve was closed */
                            } else {

                                /** Close valve of active line */
                                mPlantIrrigationSystem.getValve(mActiveLine.value).close();

                                /** Load valve close image for valve FAB */
                                ((ImageView) mFabValve)
                                        .setImageResource(R.drawable.ic_fab_valve_close);
                                /** Set color of valve FAB to light grey */
                                ((ImageView) mFabPump).getBackground()
                                        .setColorFilter(Color.parseColor("#FF444444"),
                                                PorterDuff.Mode.MULTIPLY);

                                /** Deactivate pump FAB */
                                mFabPump.setEnabled(false);
                                /** Set color of pump FAB to dark gray */
                                mFabValve.setBackgroundTintList(ColorStateList.valueOf(Color.GRAY));
                            }
                            break;

                        case PlantIrrigationMessage.PLANT_PUMP_ON:

                            /** Start vibrating */
                            mVibrator.vibrate(pattern,0);

                            /** Turn on pump */
                            mPlantIrrigationSystem.getPump()
                                    .setState(PlantIrrigationSystem.Pump.ON);
                            break;

                        case PlantIrrigationMessage.PLANT_PUMP_OFF:

                            /** Stop vibrating */
                            mVibrator.cancel();

                            /** Turn off pump */
                            mPlantIrrigationSystem.getPump()
                                    .setState(PlantIrrigationSystem.Pump.OFF);
                            break;

                        case PlantIrrigationMessage.PLANT_PUMP_DUTY_CYCLE:

                            /** Set pump duty cycle */
                            mPlantIrrigationSystem.getPump()
                                    .setDutycycle(message.getSystem()
                                            .getPump().getDutycycle());
                            break;

                        case PlantIrrigationMessage.PLANT_MOISTURE_SENSOR:

                            /** Update moisture sensor values and configuration */
                            mPlantIrrigationSystem.getMoistureSensor(mActiveLine.value)
                                    .setCurrentValue(message.getSystem()
                                            .getMoistureSensor(mActiveLine.value)
                                                    .getCurrentValue());
                            mPlantIrrigationSystem.getMoistureSensor(mActiveLine.value)
                                    .setThresholdLow(message.getSystem()
                                            .getMoistureSensor(mActiveLine.value)
                                            .getThresholdLow());
                            mPlantIrrigationSystem.getMoistureSensor(mActiveLine.value)
                                    .setThresholdHigh(message.getSystem()
                                            .getMoistureSensor(mActiveLine.value)
                                            .getThresholdHigh());
                            break;

                        default:
                            break;
                    }
                }

                /** Check if Plant Irrigation System object from command response equals
                 * current Plant Irrigation System object state
                 */
                if(message.getSystem().equals(mPlantIrrigationSystem))
                    bResult = true;
            }
        }

        return bResult;
    }
}