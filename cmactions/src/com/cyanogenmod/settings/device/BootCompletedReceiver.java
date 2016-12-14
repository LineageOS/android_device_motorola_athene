/*
 * Copyright (c) 2015 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.cyanogenmod.settings.device;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import com.cyanogenmod.settings.device.ServiceWrapper.LocalBinder;

import org.cyanogenmod.internal.util.FileUtils;

public class BootCompletedReceiver extends BroadcastReceiver {
    static final String TAG = "CMActions";
    private ServiceWrapper mServiceWrapper;

    @Override
    public void onReceive(final Context context, Intent intent) {
        Log.i(TAG, "Booting");

        // Restore nodes to saved preference values
        for (String pref : Constants.sButtonPrefKeys) {
             String value = Constants.isPreferenceEnabled(context, pref) ? "1" : "0";
             String node = Constants.sBooleanNodePreferenceMap.get(pref);

             if (!FileUtils.writeLine(node, value)) {
                 Log.w(TAG, "Write to node " + node +
                       " failed while restoring saved preference values");
             }
        }

        context.startService(new Intent(context, ServiceWrapper.class));
    }

    private ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName className, IBinder service) {
            LocalBinder binder = (LocalBinder) service;
            mServiceWrapper = binder.getService();
            mServiceWrapper.start();
        }

        @Override
        public void onServiceDisconnected(ComponentName className) {
            mServiceWrapper = null;
        }
    };
}
