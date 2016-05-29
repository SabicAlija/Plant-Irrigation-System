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
