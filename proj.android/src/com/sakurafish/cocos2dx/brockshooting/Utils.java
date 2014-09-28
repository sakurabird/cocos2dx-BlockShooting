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

package com.sakurafish.cocos2dx.brockshooting;

import android.util.Log;

/**
 * @author sakura
 */
public class Utils {

    /**
     * デバッグ用ログ出力：出所が分かるようにメソッドの出力位置を表示する
     * 
     * @param message 出力するメッセージ文字列
     */
    public static void logDebug(String message) {
        if (BuildConfig.DEBUG) {
            String prefLabel = new Throwable().getStackTrace()[1].toString();
            Log.d("logDebug", prefLabel + ": " + message);
        }
    }

    private Utils() {
    }
}
