
package com.sakurafish.cocos2dx.brockshooting;

import android.util.Log;

public class CallFromCPP {
    static final String TAG = "CallFromCPP";

    public static void showAd(int n) {
        Log.v(TAG, "showAd " + n);
        Global.activity.showAd(n);
    }
}
