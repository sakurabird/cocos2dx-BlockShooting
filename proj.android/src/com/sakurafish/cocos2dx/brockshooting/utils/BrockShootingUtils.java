/*
 * Copyright 2014 sakura <neko3genki@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * 
 */

package com.sakurafish.cocos2dx.brockshooting.utils;

import android.text.TextUtils;

import com.sakurafish.cocos2dx.brockshooting.MyApplication;

import java.util.UUID;

/**
 * @author sakura
 */
public class BrockShootingUtils {

    /**
     * uuid取得：preferenceになければ取得
     */
    public static String getUUID() {
        String uuid = Pref.getPrefString(MyApplication.getContext(), "uuid");
        if (TextUtils.isEmpty(uuid)) {
            uuid = UUID.randomUUID().toString();
            Pref.setPref(MyApplication.getContext(), "uuid", uuid);
        }
        Utils.logDebug(uuid);
        return uuid;
    }

    private BrockShootingUtils() {
    }
}
