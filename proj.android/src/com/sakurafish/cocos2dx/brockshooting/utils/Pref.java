
package com.sakurafish.cocos2dx.brockshooting.utils;

import android.content.Context;
import android.content.SharedPreferences;

public class Pref {

    private static final String PREF_NAME = "app";

    public static SharedPreferences getSharedPreferences(Context con) {
        return con.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
    }

    public static void setPref(Context con, String key, String value) {
        SharedPreferences pref = con.getSharedPreferences(PREF_NAME,
                Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = pref.edit();
        editor.putString(key, value);
        editor.commit();
    }

    public static String getPrefString(Context con, String key) {
        SharedPreferences pref = con.getSharedPreferences(PREF_NAME,
                Context.MODE_PRIVATE);
        return pref.getString(key, null);
    }

    public static void setPref(Context con, String key, boolean value) {
        SharedPreferences pref = con.getSharedPreferences(PREF_NAME,
                Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = pref.edit();
        editor.putBoolean(key, value);
        editor.commit();
    }

    public static boolean getPrefBool(Context con, String key,
            boolean defaultBool) {
        SharedPreferences pref = con.getSharedPreferences(PREF_NAME,
                Context.MODE_PRIVATE);
        return pref.getBoolean(key, defaultBool);
    }

    public static void setPref(Context con, String key, int value) {
        SharedPreferences pref = con.getSharedPreferences(PREF_NAME,
                Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = pref.edit();
        editor.putInt(key, value);
        editor.commit();
    }

    public static int getPrefInt(Context con, String key) {
        SharedPreferences pref = con.getSharedPreferences(PREF_NAME,
                Context.MODE_PRIVATE);
        return pref.getInt(key, 0);
    }

    public static long getPrefLong(Context con, String key, long defaultValue) {
        SharedPreferences pref = con.getSharedPreferences(PREF_NAME,
                Context.MODE_PRIVATE);
        return pref.getLong(key, defaultValue);
    }

    public static void setPrefLong(Context con, String key, long value) {
        SharedPreferences pref = con.getSharedPreferences(PREF_NAME,
                Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = pref.edit();
        editor.putLong(key, value);
        editor.commit();
    }

    public static SharedPreferences.Editor getEditor(Context con) {
        SharedPreferences pref = con.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
        return pref.edit();
    }

    private Pref() {
    }
}
