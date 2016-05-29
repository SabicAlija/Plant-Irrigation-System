package com.sabic.alija.irrigator_;

import android.app.Fragment;
import android.os.Bundle;
import android.util.MutableBoolean;
import android.util.MutableInt;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SeekBar;

/**
 * Created by Alija on 26.05.2016.
 */
public class ControlSettingsFragment extends Fragment {

    private PlantIrrigationSystem mSystem;
    private MutableInt mActiveLine;
    private MutableBoolean mSeekBarLoaded;

    /**
     * onCreateView callback function.
     * @param inflater
     * @param container
     * @param savedInstanceState
     * @return View object of control fragment settings layout
    /*/
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.control_fragment_settings, container, false);
    }

    /**
     * Setter
    /*/
    /**
     * Set active irrigation line
     * @param activeLine
    /*/
    public void setActiveLine(MutableInt activeLine) {
        this.mActiveLine = activeLine;
    }

    /**
     * Set Plant Irrigation System object
     * @param system
    /*/
    public void setSystem(PlantIrrigationSystem system) {
        this.mSystem = system;
    }

    /**
     * onResume callback function loading current value of moisture sensor
     * into seekbar components
    /*/
    @Override
    public void onResume() {
        super.onResume();

        /** Load ui components */
        SeekBar mSeekCurrent = (SeekBar) getActivity().findViewById(R.id.control_settings_current_seekbar);
        SeekBar mSeekLow = (SeekBar) getActivity().findViewById(R.id.control_settings_low_seekbar);
        SeekBar mSeekHigh = (SeekBar) getActivity().findViewById(R.id.control_settings_high_seekbar);

        int currentValue, low, high;

        /** Check if seekbar of current value is loaded */
        if (mSeekCurrent != null) {

            /** Load current value from moisture sensor */
            currentValue = (mSystem.getMoistureSensor(mActiveLine.value).getCurrentValue() * 100) / 255;

            /** Set moisture sensor value as current progress */
            mSeekCurrent.setProgress(currentValue);
        }

        /** Check if seekbar of dry threshold is loaded */
        if (mSeekLow != null) {

            /** Load current dry threshold from moisture sensor */
            low = (mSystem.getMoistureSensor(mActiveLine.value).getThresholdLow() * 100) / 255;

            /** Set moisture sensor dry threshold as current progress */
            mSeekLow.setProgress(low);
        }

        /** Check if seekbar of wet threshold is loaded */
        if (mSeekHigh != null) {

            /** Load current wet threshold from moisture sensor */
            high = (mSystem.getMoistureSensor(mActiveLine.value).getThresholdHigh() * 100) / 255;

            /** Set moisture sensor wet threshold as current progress */
            mSeekHigh.setProgress(high);

        }
    }

}
