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

