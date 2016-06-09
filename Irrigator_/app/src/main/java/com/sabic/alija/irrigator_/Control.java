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

import android.app.Activity;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.util.Log;
import android.util.MutableInt;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import java.util.ArrayList;

import static com.sabic.alija.irrigator_.R.drawable;
import static com.sabic.alija.irrigator_.R.id;

/**
 * Created by Alija on 23.05.2016.
 */
public class Control {

    /** Control Fragment Class */
    public class CntrFragment {
        private TextView mText;
        private LinearLayout.LayoutParams mParams;
        private FragmentManager mFragmentManager;
        private FragmentTransaction mFragmentTransaction;
        private ControlFragment mControlFragment;
        private View mView;

        /**
         * Load Control Fragment
         * @param a Test
         * @param activeLine
         * @param client
         * @param system
         * @param systemIntent
         */
        public void load(Activity a, MutableInt activeLine,
                         PlantIrrigationSystemClient client,
                         PlantIrrigationSystem system,
                         PlantIrrigationSystem systemIntent) {

            mFragmentManager = a.getFragmentManager();
            mFragmentTransaction = mFragmentManager.beginTransaction();
            mControlFragment = new ControlFragment();

            /** Load TextView and configure height */
            mText = (TextView) a.findViewById(id.control_text_view);
            mParams = (LinearLayout.LayoutParams) mText.getLayoutParams();
            mParams.height = 40;
            mText.setLayoutParams(mParams);

            /** Initiate fragment replace transaction */
            mFragmentTransaction.replace(id.control_frame, mControlFragment);
            mFragmentTransaction.addToBackStack(null);
            mFragmentTransaction.commit();

            /** Load control frame (containing control fragment layout) */
            mView = a.findViewById(id.control_frame);

            /** Attach OnGlobalLayoutListener to ViewTreeObserver to update
             * ui interface components based on current state of Plant Irrigation
             * System.
             */
            mView.getViewTreeObserver()
                    .addOnGlobalLayoutListener(new ControlViewListener(
                            a, activeLine, client, system, systemIntent));

        }

    }

    /** Control Settings Fragment Class */
    public class CntrSettingsFragment {

        private FragmentManager mFragmentManager;
        private FragmentTransaction mFragmentTransaction;
        private ControlSettingsFragment mControlSettingsFragment;

        /**
         * Load Control Settings Fragment
         * @param a
         * @param activeLine
         * @param systemIntent
         */
        public void load(Activity a, MutableInt activeLine, PlantIrrigationSystem systemIntent) {
            mFragmentManager = a.getFragmentManager();
            mFragmentTransaction = mFragmentManager.beginTransaction();
            mControlSettingsFragment = new ControlSettingsFragment();
            mControlSettingsFragment.setActiveLine(activeLine);
            mControlSettingsFragment.setSystem(systemIntent);

            /** Initiate fragment replace transaction */
            mFragmentTransaction.replace(id.control_frame, mControlSettingsFragment);
            mFragmentTransaction.addToBackStack(null);
            mFragmentTransaction.commit();

            /** OnGlobalLayoutListener attached during Control Fragment setup contains
             * handler function (listeners) for settings fragment components which are
             * attached after seekbar components are available.
             */
        }


    }

    /** OnGlobalLayoutListener: ControlViewListener Class */
    public class ControlViewListener implements ViewTreeObserver.OnGlobalLayoutListener {

        private Activity mActivity;
        private View mLinearLayout;
        private TextView mText;
        private ImageView mImage;
        private FloatingActionButton mFabValve;
        private FloatingActionButton mFabSettings;
        private FloatingActionButton mFabPump;
        private MutableInt mActiveLine;

        private SeekBar mSeekCurrent;
        private SeekBar mSeekLow;
        private SeekBar mSeekHigh;

        private ColorMatrix mMatrix;
        private ColorMatrixColorFilter mColorFilter;

        private PlantIrrigationSystemClient mClient;
        private PlantIrrigationSystem mSystem;
        private PlantIrrigationSystem mSystemIntent;

        /** OnGlobalLayoutListener: ControlViewListener Constructor */
        public ControlViewListener (Activity a, MutableInt activeLine,
                                    PlantIrrigationSystemClient client,
                                    PlantIrrigationSystem system,
                                    PlantIrrigationSystem systemIntent) {

            /** Load object references */
            this.mActivity = a;
            this.mActiveLine = activeLine;
            this.mClient = client;
            this.mSystem = system;
            this.mSystemIntent = systemIntent;
            this.mText = (TextView) a.findViewById(id.control_text_view);
            this.mLinearLayout = a.findViewById(id.ll_control_main);

        }

        /**
         * onGlobalLayout callback: called on changes/activities on
         * referenced layout
         */
        @Override
        public void onGlobalLayout() {

            /** Load control frame ui components */
            mImage = (ImageView) mActivity.findViewById(id.control_image_view);
            mFabValve = (FloatingActionButton) mActivity.findViewById(id.fabValve);
            mFabSettings = (FloatingActionButton) mActivity.findViewById(id.fabSettings);
            mFabPump = (FloatingActionButton) mActivity.findViewById(id.fabPump);

            /** Load control settings frame ui components */
            mSeekCurrent = (SeekBar) mActivity.findViewById(id.control_settings_current_seekbar);
            mSeekLow = (SeekBar) mActivity.findViewById(id.control_settings_low_seekbar);
            mSeekHigh = (SeekBar) mActivity.findViewById(id.control_settings_high_seekbar);

            /** Check if plant image is loaded */
            if(mImage != null) {

                /** Handle every line individually */
                switch(mActiveLine.value) {

                    case PlantIrrigationSystem.LINE_ONE:
                        mImage.setImageResource(drawable.ic_menu_sunflower);
                        mText.setText("#1");
                        break;

                    case PlantIrrigationSystem.LINE_TWO:
                        mImage.setImageResource(drawable.ic_menu_cactus);
                        mText.setText("#2");
                        break;

                    case PlantIrrigationSystem.LINE_THREE:
                        mImage.setImageResource(drawable.ic_menu_tulip);
                        mText.setText("#3");
                        break;

                    case PlantIrrigationSystem.LINE_FOUR:
                        mImage.setImageResource(drawable.ic_menu_flower);
                        mText.setText("#4");
                        break;

                    default:
                        break;
                }

                /** Attach onClickListener to Image */
                mImage.setOnClickListener(new OnControlImage(mActiveLine, mClient, mSystemIntent));

                /** Create color filter (for deactive irrigation automats) */
                mMatrix = new ColorMatrix();
                mMatrix.setSaturation(0);
                mColorFilter = new ColorMatrixColorFilter(mMatrix);

                /** Remove color from plant image if line is active */
                if(mSystem.getState(mActiveLine.value))
                    mImage.setColorFilter(null);
                else
                    mImage.setColorFilter(mColorFilter);

            }

            /** Check if settings FAB is loaded */
            if(mFabSettings != null) {

                /** Attach onClickListener to settings FAB */
                mFabSettings.setOnClickListener(new OnSettings(mActivity,
                                                mActiveLine,
                                                mSystemIntent));
            }

            /** Check if pump FAB is loaded */
            if(mFabPump != null) {

                /** Check valve state of active line and enable
                 * pump FAB if open.
                 */
                if(mSystem.getValve(mActiveLine.value).isOpen())
                    mFabPump.setEnabled(true);
                else
                    mFabPump.setEnabled(false);


                /** Attach onClickListener to pump FAB */
                mFabPump.setOnTouchListener(new OnPumpTouch(mLinearLayout,
                                                mActiveLine,
                                                mClient,
                                                mSystemIntent));

                /** Check if valve FAB is loaded after loading pump FAB */
                if(mFabValve != null) {

                    /** Attach onClickListener to valve FAB */
                    mFabValve.setOnClickListener(new OnValve(mActiveLine,
                                                mClient,
                                                mSystemIntent));

                    /** Paint valve and pump FAB depending on valve
                     * state of active line
                     */
                    if(mSystem.getValve(mActiveLine.value).isOpen()) {
                        mFabPump.setBackgroundTintList(ColorStateList.valueOf(Color.rgb(00,55,99)));
                        mFabValve.setBackgroundTintList(ColorStateList.valueOf(Color.BLACK));
                        ((ImageView) mFabValve).setImageResource(drawable.ic_fab_valve_open);
                    } else {
                        mFabPump.setBackgroundTintList(ColorStateList.valueOf(Color.DKGRAY));
                        mFabValve.setBackgroundTintList(ColorStateList.valueOf(Color.GRAY));
                        ((ImageView) mFabValve).setImageResource(drawable.ic_fab_valve_close);
                    }
                }

            }

            /** Check if seekbar for current value is loaded */
            if(mSeekCurrent != null) {

                /** Attach onClickListener to seekbar for current value */
                mSeekCurrent.setOnSeekBarChangeListener(new SeekBarCurrentListener(mActivity,
                                                mActiveLine,
                                                mClient,
                                                mSystemIntent));
            }

            /** Check if seekbar for dry threshold is loaded */
            if(mSeekLow != null) {

                /** Attach onClickListener to seekbar for dry threshold */
                mSeekLow.setOnSeekBarChangeListener(new SeekBarLowThresholdListener(mActivity,
                                                mActiveLine,
                                                mClient,
                                                mSystemIntent));

            }

            /** Check if seekbar for wet threshold is loaded */
            if(mSeekHigh != null) {

                /** Attach onClickListener to seekbar for wet threshold */
                mSeekHigh.setOnSeekBarChangeListener(new SeekBarHighThresholdListener(mActivity,
                                                mActiveLine,
                                                mClient,
                                                mSystemIntent));
            }
        }
    }

    /** OnSeekBarChangeListener: SeekBarCurrentListener Class */
    public class SeekBarCurrentListener implements SeekBar.OnSeekBarChangeListener {

        private Activity mActivity;
        private TextView mTextValue;
        private MutableInt mActiveLine;
        private PlantIrrigationSystemClient mClient;
        private PlantIrrigationSystem mSystemIntent;
        private int mValue;
        private ArrayList<Integer> mCmd;

        /**
         * OnSeekBarChangeListener: SeekBarCurrentListener Constructor
         * @param a             Current activity
         * @param activeLine    Active irrigation line
         * @param client        Plant Irrigation System client
         * @param systemIntent  Plant Irrigation System intent
         */
        public SeekBarCurrentListener (Activity a, MutableInt activeLine, PlantIrrigationSystemClient client, PlantIrrigationSystem systemIntent) {
            this.mActivity = a;
            this.mTextValue = (TextView) mActivity.findViewById(R.id.control_settings_current_value);
            this.mActiveLine = activeLine;
            this.mClient = client;
            this.mSystemIntent = systemIntent;
        }

        /**
         * onProgressChanged callback function updating displayed value
         * @param seekBar  Current Value SeekBar
         * @param progress Progress of Current Value SeekBar
         * @param fromUser
         */
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            /** Normalize progress value */
            mValue = (progress * 255) / 100;

            /** Update TextView */
            mTextValue.setText(Integer.toString(mValue));
        }

        /**
         * onStartTrackingTouch callback function not used.
         * @param seekBar   Current Value SeekBar
         */
        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        /**
         * onStopTrackingTouch callback function sending
         * current value to Plant Irrigation System
         * @param seekBar   Current Value SeekBar
         */
        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

            mCmd = new ArrayList<Integer>();

            /** Add moisture sensor command */
            mCmd.add(PlantIrrigationMessage.PLANT_MOISTURE_SENSOR);

            /** Normalize and load current value into intent */
            mSystemIntent.getMoistureSensor(mActiveLine.value).setCurrentValue((seekBar.getProgress() * 255) / 100);

            /** Create thread for asynchronous execution */
            Thread t = new Thread(new Runnable() {
                @Override
                public void run() {

                    /** Send update of current value to Plant Irrigation System */
                    mClient.send(new PlantIrrigationMessage(mActiveLine.value, mCmd, mSystemIntent));
                }
            });

            /** Start asynchronous thread */
            t.start();
        }
    }

    /** OnSeekBarChangeListener: SeekBarLowThresholdListener Class */
    public class SeekBarLowThresholdListener implements SeekBar.OnSeekBarChangeListener {

        private Activity mActivity;
        private TextView mTextValue;
        private MutableInt mActiveLine;
        private PlantIrrigationSystemClient mClient;
        private PlantIrrigationSystem mSystemIntent;
        private int mValue;
        private ArrayList<Integer> mCmd;

        /** OnSeekBarChangeListener: SeekBarLowThresholdListener Constructor */
        public SeekBarLowThresholdListener (Activity a, MutableInt activeLine, PlantIrrigationSystemClient client, PlantIrrigationSystem systemIntent) {
            this.mActivity = a;
            this.mTextValue = (TextView) mActivity.findViewById(id.control_settings_low_value);
            this.mActiveLine = activeLine;
            this.mClient = client;
            this.mSystemIntent = systemIntent;
        }

        /**
         * onProgressChanged callback function updating displayed value
         * @param seekBar   Dry Threshold SeekBar
         * @param progress  Progress of Dry Threshold SeekBar
         * @param fromUser
         */
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            /** Normalize progress value */
            mValue = (progress * 255) / 100;

            /** Update TextView */
            mTextValue.setText(Integer.toString(mValue));
        }

        /**
         * onStartTrackingTouch callback function not used.
         * @param seekBar   Dry Threshold SeekBar
         */
        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        /**
         * onStopTrackingTouch callback function sending
         * dry threshold to Plant Irrigation System
         * @param seekBar   Dry Threshold SeekBar
         */
        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

            mCmd = new ArrayList<Integer>();

            /** Add moisture sensor command */
            mCmd.add(PlantIrrigationMessage.PLANT_MOISTURE_SENSOR);

            /** Normalize and load dry threshold into intent */
            mSystemIntent.getMoistureSensor(mActiveLine.value).setThresholdLow((seekBar.getProgress() * 255) / 100);

            /** Create thread for asynchronous execution */
            Thread t = new Thread(new Runnable() {
                @Override
                public void run() {

                    /** Send update of dry threshold to Plant Irrigation System */
                    mClient.send(new PlantIrrigationMessage(mActiveLine.value, mCmd, mSystemIntent));
                }
            });

            /** Start asynchronous thread */
            t.start();
        }
    }

    /** OnSeekBarChangeListener: SeekBarHighThresholdListener Class */
    public class SeekBarHighThresholdListener implements SeekBar.OnSeekBarChangeListener {

        private Activity mActivity;
        private TextView mTextValue;
        private MutableInt mActiveLine;
        private PlantIrrigationSystemClient mClient;
        private PlantIrrigationSystem mSystemIntent;
        private int mValue;
        private ArrayList<Integer> mCmd;

        /** OnSeekBarChangeListener: SeekBarHighThresholdListener Class */
        public SeekBarHighThresholdListener (Activity a, MutableInt activeLine,
                                             PlantIrrigationSystemClient client,
                                             PlantIrrigationSystem systemIntent) {
            this.mActivity = a;
            this.mTextValue = (TextView) mActivity.findViewById(id.control_settings_high_value);
            this.mActiveLine = activeLine;
            this.mClient = client;
            this.mSystemIntent = systemIntent;
        }

        /**
         * onProgressChanged callback function updating displayed value
         * @param seekBar   Wet Threshold SeekBar
         * @param progress  Progress of Wet Threshold SeekBar
         * @param fromUser
         */
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            /** Normalize progress value */
            mValue = (progress * 255) / 100;

            /** Update TextView */
            mTextValue.setText(Integer.toString(mValue));
        }

        /**
         * onStartTrackingTouch callback function not used.
         * @param seekBar   Wet Threshold SeekBar
         */
        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        /**
         * onStopTrackingTouch callback function sending
         * wet threshold to Plant Irrigation System
         * @param seekBar   Wet Threshold SeekBar
         */
        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

            mCmd = new ArrayList<Integer>();

            /** Add moisture sensor command */
            mCmd.add(PlantIrrigationMessage.PLANT_MOISTURE_SENSOR);

            /** Normalize and load wet threshold into intent */
            mSystemIntent.getMoistureSensor(mActiveLine.value)
                    .setThresholdHigh((seekBar.getProgress() * 255) / 100);

            /** Create thread for asynchronous execution */
            Thread t = new Thread(new Runnable() {
                @Override
                public void run() {

                    /** Send update of wet threshold to Plant Irrigation System */
                    mClient.send(new PlantIrrigationMessage(mActiveLine.value,
                                            mCmd,
                                            mSystemIntent));
                }
            });

            /** Start asynchronous thread */
            t.start();
        }
    }

    /** OnClickListener: OnSetting Class */
    public class OnSettings implements View.OnClickListener {

        private Activity mActivity;
        private MutableInt mActiveLine;
        private PlantIrrigationSystem mSystemIntent;

        /** OnClickListener: OnSetting Constructor */
        public OnSettings(Activity a, MutableInt activeLine,
                          PlantIrrigationSystem systemIntent) {
            this.mActivity = a;
            this.mActiveLine = activeLine;
            this.mSystemIntent = systemIntent;
        }

        /**
         * OnClick callback function loading settings fragment
         * @param v View object referencing settings FAB
         */
        @Override
        public void onClick(View v) {
            Log.d("Settings FAB","pressed");

            /** Load Control Settings Fragment */
            new CntrSettingsFragment().load(mActivity, mActiveLine, mSystemIntent);
        }
    }

    /** OnClickListener: OnValve Class */
    public class OnValve implements View.OnClickListener {

        private PlantIrrigationSystemClient mClient;
        private PlantIrrigationSystem mSystemIntent;
        private ArrayList<Integer> mCmd;
        private MutableInt mActiveLine;

        /** OnClickListener: OnValve Constructor */
        public OnValve(MutableInt activeLine, PlantIrrigationSystemClient client,
                       PlantIrrigationSystem systemIntent) {
            this.mActiveLine = activeLine;
            this.mSystemIntent = systemIntent;
            this.mClient = client;
        }

        /**
         * onClick callback function sending valve command to Plant Irrigation
         * System
         * @param v
         */
        @Override
        public void onClick(View v) {
            Log.d("Valve FAB","pressed");

            mCmd = new ArrayList<Integer>();

            /** Check current valve state */
            if(mSystemIntent.getValve(mActiveLine.value).isOpen()) {

                /** Load valve close command if valve is opened currently */
                mCmd.add(new Integer(PlantIrrigationMessage.PLANT_VALVE_CLOSE));
                mSystemIntent.getValve(mActiveLine.value).close();
            } else {

                /** Load valve open command if valve is closed currently */
                mCmd.add(new Integer(PlantIrrigationMessage.PLANT_VALVE_OPEN));
                mSystemIntent.getValve(mActiveLine.value).open();
            }

            /** Create thread for asynchronous execution */
            Thread t = new Thread(new Runnable() {
                @Override
                public void run() {

                    /** Send valve command to Plant Irrigation System */
                    mClient.send(new PlantIrrigationMessage(mActiveLine.value, mCmd, mSystemIntent));
                }
            });

            /** Start asynchronous thread */
            t.start();
        }
    }

    /** OnTouchListener: OnPump Touch Class */
    public class OnPumpTouch implements View.OnTouchListener {

        private View mView;
        private long mTimeDown;
        private ArrayList<Integer> mCmd;
        private PlantIrrigationSystemClient mClient;
        private PlantIrrigationSystem mSystemIntent;
        private boolean mPumpActive;
        private MutableInt mActiveLine;

        /** OnTouchListener: OnPump Touch Constructor */
        public OnPumpTouch (View view, MutableInt activeLine, PlantIrrigationSystemClient client, PlantIrrigationSystem systemIntent) {
            this.mView = view;
            this.mActiveLine = activeLine;
            this.mClient = client;
            this.mSystemIntent = systemIntent;
            this.mPumpActive = false;
        }

        /**
         * onTouch callback function handling press activities
         * on pump FAB.
         * @param v         View object referencing pump FAB
         * @param event     Pump FAB motion event
         * @return
         */
        @Override
        public boolean onTouch(View v, MotionEvent event) {

            mCmd = new ArrayList<Integer>();

            /** Handle pump FAB action individually */
            switch(event.getAction()) {

                case MotionEvent.ACTION_DOWN:

                    /** Check if pump on was not pressed before */
                    if(!mPumpActive) {

                        /** Prevent motion event ACTION_DOWN from
                         * repeated execution
                         */
                        mPumpActive = true;
                        Log.d("Pump FAB", "pressed");

                        /** Store current time of motion event */
                        this.mTimeDown = System.currentTimeMillis();

                        /** Add pump on command */
                        mCmd.add(new Integer(PlantIrrigationMessage.PLANT_PUMP_ON));

                        /** Load pump state into intent */
                        mSystemIntent.getPump().setState(PlantIrrigationSystem.Pump.ON);
                        if (mClient != null) {

                            /** Create thread for asynchronous execution */
                            Thread t = new Thread(new Runnable() {
                                @Override
                                public void run() {

                                    /** Send pump on command to Plant Irrigation System */
                                    mClient.send(new PlantIrrigationMessage(mActiveLine.value, mCmd, mSystemIntent));
                                }
                            });

                            /** Start asynchronous thread */
                            t.start();
                        }
                    }
                    break;
                case MotionEvent.ACTION_UP:

                    /** Check if pump on was pressed before */
                    if(mPumpActive) {

                        /** Release pump active lock */
                        mPumpActive = false;
                        Log.d("Pump FAB", "released");

                        /** Calculate button pressed / pump running time and check
                         * if button was pressed more than 400 ms */
                        if (System.currentTimeMillis() - mTimeDown < 400) {

                            if (mView != null) {

                                /** Display pump handling instruction on user interface */
                                Snackbar.make(mView,
                                        "Press and hold to start watering..",
                                        Snackbar.LENGTH_LONG)
                                        .setDuration(Snackbar.LENGTH_SHORT)
                                        .setAction("Action", null).show();
                            }
                        }

                        /** Add pump off command */
                        mCmd.add(new Integer(PlantIrrigationMessage.PLANT_PUMP_OFF));

                        /** Load pump state into intent */
                        mSystemIntent.getPump().setState(PlantIrrigationSystem.Pump.OFF);
                        if(mClient != null) {

                            /** Create thread for asynchronous execution */
                            Thread t = new Thread(new Runnable() {
                                @Override
                                public void run() {

                                    /** Send pump on command to Plant Irrigation System */
                                    mClient.send(new PlantIrrigationMessage(mActiveLine.value, mCmd, mSystemIntent));
                                }
                            });

                            /** Start asynchronous thread */
                            t.start();
                        }
                    }
                    break;

                default:
                    break;

            }
            return false;
        }
    }

    /** OnClickListener: OnControlImage Class */
    public class OnControlImage implements View.OnClickListener {

        private PlantIrrigationSystemClient mClient;
        private PlantIrrigationSystem mSystemIntent;
        private ArrayList<Integer> mCmd;
        private MutableInt mActiveLine;

        /** OnClickListener: OnControlImage Constructor */
        public OnControlImage (MutableInt activeLine, PlantIrrigationSystemClient client, PlantIrrigationSystem systemIntent) {
            this.mActiveLine = activeLine;
            this.mClient = client;
            this.mSystemIntent = systemIntent;
        }

        /**
         * onClick callback function sending state of automatic irrigation to
         * Plant Irrigation System
         * @param v
         */
        @Override
        public void onClick(View v) {

            Log.d("Image","pressed");
            mCmd = new ArrayList<Integer>();

            /** Check current status of automatic irrigation */
            if(mSystemIntent.getState(mActiveLine.value)) {

                /** Load automatic irrigation off command if state is activated currently */
                mCmd.add(new Integer(PlantIrrigationMessage.PLANT_AUTOMAT_OFF));
                mSystemIntent.deactivateLine(mActiveLine.value);
            } else {

                /** Load automatic irrigation off command if state is deactivated currently */
                mCmd.add(new Integer(PlantIrrigationMessage.PLANT_AUTOMAT_ON));
                mSystemIntent.activateLine(mActiveLine.value);
            }

            /** Create thread for asynchronous execution */
            Thread t = new Thread(new Runnable() {
                @Override
                public void run() {

                    /** Send valve command to Plant Irrigation System */
                    mClient.send(new PlantIrrigationMessage(mActiveLine.value, mCmd , mSystemIntent));
                }
            });

            /** Start asynchronous thread */
            t.start();
        }
    }
}