
package com.sakurafish.cocos2dx.brockshooting;

import android.app.Application;
import android.content.Context;

public class MyApplication extends Application {

    private static MyApplication application;

    public MyApplication() {
        super();
        application = this;
    }

    public static MyApplication getInstance() {
        return application;
    }

    public static Context getContext() {
        return application.getApplicationContext();
    }
}
