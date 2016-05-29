package com.sabic.alija.irrigator_;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

/**
 * Created by Alija on 22.05.2016.
 */
public class ControlFragment extends Fragment {

    /**
     * onCreateView callback function.
     * @param inflater
     * @param container
     * @param savedInstanceState
     * @return View object of control fragment layout
     */
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.control_fragment_layout, container, false);
    }
}

