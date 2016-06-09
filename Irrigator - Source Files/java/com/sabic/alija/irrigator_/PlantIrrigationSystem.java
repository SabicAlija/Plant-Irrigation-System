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

import java.util.ArrayList;

/**
 * Created by Alija on 23.05.2016.
 */
public class PlantIrrigationSystem {

    /* Object Variables */
    private boolean[] mState;
    private Pump mPump;
    private ArrayList<MoistureSensor> mMoistureSensor;
    private ArrayList<Valve> mValve;

    /* Constants */
    public static final boolean DEACTIVATED = false;
    public static final boolean ACTIVATED = true;
    public static final int LINE_ONE = 0;
    public static final int LINE_TWO = 1;
    public static final int LINE_THREE = 2;
    public static final int LINE_FOUR = 3;

    /** Plant Irrigation System Constructor */
    public PlantIrrigationSystem(boolean... state) {
        this.mState = new boolean[4];
        this.mPump = new PlantIrrigationSystem.Pump(Pump.MAXIMUM, Pump.OFF);
        this.mMoistureSensor = new ArrayList<MoistureSensor>();
        this.mValve = new ArrayList<Valve>();

        /** Initialize Moisture sensor and valves */
        for(int i = 0; i < 4; i++) {
            this.mMoistureSensor.add(new MoistureSensor());
            this.mValve.add(new Valve(Valve.CLOSE));
        }
    }

    public void activateLine(int i) {
        mState[i] = true;
    }
    public void deactivateLine(int i) {
        mState[i] = false;
    }

    /** Getter */
    public boolean getState(int i) {
        return mState[i];
    }
    public Pump getPump() {
        return mPump;
    }
    public MoistureSensor getMoistureSensor(int i) {
        return mMoistureSensor.get(i);
    }
    public Valve getValve(int i) {
        return mValve.get(i);
    }


    /**
     * Inner Classes
     *
    /*/
    /* Pump Class */
    public class Pump{
        private int mState;
        private int mDutyCycle;

        /* Constants */
        public static final int OFF = 0;
        public static final int ON = 1;
        public static final int MAXIMUM = 100;
        public static final int MINIMUM = 0;

        /**
         * Pump Constructor
         * @param dutycycle     Pump Duty Cycle
         * @param state         Pump State
         */
        public Pump(int dutycycle, int state) {
            this.mDutyCycle = dutycycle;
            this.mState = state;
        }

        /** Pump Setter */
        public void setDutycycle(int dutycycle) {
            this.mDutyCycle = dutycycle;
        }
        public void setState(int state) {
            this.mState = state;
        }

        /** Pump Getter */
        public int getDutycycle() {
            return mDutyCycle;
        }
        public int getState() {
            return mState;
        }

    }
    /* Moisture Sensor Class */
    public class MoistureSensor {
        private int mCurrentValue;
        private int mThresholdLow;
        private int mThresholdHigh;

        /* Constants */
        public static final int MINIMUM = 0;
        public static final int MAXIMUM = 100;

        /** Moisture Sensor Constructor */
        public MoistureSensor() {
            this.mCurrentValue = 0;
            this.mThresholdHigh = 0;
            this.mThresholdLow = 0;
        }

        /** Moisture Sensor Setter */
        public void setCurrentValue(int currentValue) {
            this.mCurrentValue = currentValue;
        }
        public void setThresholdHigh(int thresholdHigh) {
            this.mThresholdHigh = thresholdHigh;
        }
        public void setThresholdLow(int thresholdLow) {
            this.mThresholdLow = thresholdLow;
        }

        /** Moisture Sensor Getter */
        public int getCurrentValue() {
            return mCurrentValue;
        }
        public int getThresholdHigh() {
            return mThresholdHigh;
        }
        public int getThresholdLow() {
            return mThresholdLow;
        }


    }
    /* Valves Class */
    public class Valve {
        private boolean mState;

        /* Constants */
        public static final boolean CLOSE = false;
        public static final boolean OPEN = true;

        /** Valve Constructor
         * @param state     Current Valve State
         */
        public Valve(boolean state) {
            this.mState = state;
        }


        public boolean isOpen() {
            return mState;
        }

        public void open() {
            mState = true;
        }

        public void close() {
            mState = false;
        }
    }


    /**
     * Compares two Plant Irrigation Systems
     * @param o     Plant Irrigation System to compare with
     * @return      True if equal
     */
    @Override
    public boolean equals(Object o) {

        /** Load object as Plant Irrigation System */
        PlantIrrigationSystem another = (PlantIrrigationSystem) o;

        boolean bResult = true;

        /** Compare each irrigation line state, moisture sensor and valve */
        for(int i = 0; i < 4; i++) {
            if(mState[i] != another.getState(i)) bResult = false;
            if(mMoistureSensor.get(i).getThresholdLow() !=
                    another.getMoistureSensor(i).getThresholdLow()) bResult = false;
            if(mMoistureSensor.get(i).getThresholdHigh() !=
                    another.getMoistureSensor(i).getThresholdHigh()) bResult = false;
            if(mMoistureSensor.get(i).getCurrentValue() !=
                    another.getMoistureSensor(i).getCurrentValue()) bResult = false;
            if(mValve.get(i).isOpen() != another.getValve(i).isOpen()) bResult = false;
        }

        /** Compare pump state and duty cycle */
        if(mPump.getDutycycle() != another.getPump().getDutycycle() ||
           mPump.getState() != another.getPump().getState()) bResult = false;

        return bResult;
    }

    /**
     * Create a new Plant Irrigation System with same state and parameters
     * as passed Plant Irrigation Systems
     * @param another       Plant Irrigation System to copy
     * @return              Copied Plant Irrigation System instance
     */
    public static PlantIrrigationSystem getInstance(PlantIrrigationSystem another) {

        /** Create new Plant Irrigation System object */
        PlantIrrigationSystem system = new PlantIrrigationSystem(another.getState(0),
                                                                 another.getState(1),
                                                                 another.getState(2),
                                                                 another.getState(3));
        /** Load pump duty cycle and state */
        system.getPump().setDutycycle(another.getPump().getDutycycle());
        system.getPump().setState(another.getPump().getState());

        /** Load moisture sensor settings and valve state */
        for(int i = 0; i < 4; i++) {
            system.getMoistureSensor(i).setThresholdLow(another.getMoistureSensor(i).getThresholdLow());
            system.getMoistureSensor(i).setThresholdHigh(another.getMoistureSensor(i).getThresholdHigh());
            system.getMoistureSensor(i).setCurrentValue(another.getMoistureSensor(i).getCurrentValue());

            if(another.getValve(i).isOpen())
                system.getValve(i).open();
            else
                system.getValve(i).close();
        }

        return system;
    }
}
