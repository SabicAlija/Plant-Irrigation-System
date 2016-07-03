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

import java.util.ArrayList;

/**
 * Created by Alija on 24.05.2016.
 */
public class PlantIrrigationMessage {

    private int mCmdCount;
    private ArrayList<Integer> mCmd;
    private PlantIrrigationSystem mPlantIrrigationSystemIntent;
    private int mActiveLine;

    /* Commands Constants */
    public static final int PLANT_ECHO = 0;
    public static final int PLANT_AUTOMAT_ON = 1;
    public static final int PLANT_AUTOMAT_OFF = 2;
    public static final int PLANT_VALVE_OPEN = 3;
    public static final int PLANT_VALVE_CLOSE = 4;
    public static final int PLANT_PUMP_ON = 5;
    public static final int PLANT_PUMP_OFF = 6;
    public static final int PLANT_PUMP_DUTY_CYCLE = 7;
    public static final int PLANT_MOISTURE_SENSOR = 8;

    /**
     * PlantIrrigationMessage Constructor
     * @param activeLine                    Currently active irrigation line
     * @param cmd                           Commands
     * @param plantIrrigationSystemIntent   Plant Irrigation System Intent
     */
    public PlantIrrigationMessage (int activeLine, ArrayList<Integer> cmd, PlantIrrigationSystem plantIrrigationSystemIntent) {

        mActiveLine = activeLine;
        mCmdCount = 0;
        mCmd = new ArrayList<Integer>();

        /** Load commands and number of commands */
        for(int i : cmd) {
            mCmdCount++;
            mCmd.add(new Integer(i));
        }

        /** Load Plant Irrigation System Intent */
        mPlantIrrigationSystemIntent = plantIrrigationSystemIntent;
    }

    /**
     * Message Object Getter
    /*/
    public ArrayList<Integer> getCmd() {
        return mCmd;
    }
    public int getCmdCount() {
        return mCmdCount;
    }
    public PlantIrrigationSystem getSystem() {
        return mPlantIrrigationSystemIntent;
    }
    public int getActiveLine() { return mActiveLine;}

}
