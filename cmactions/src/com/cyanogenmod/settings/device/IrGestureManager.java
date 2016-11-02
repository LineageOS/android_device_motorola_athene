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

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.util.Log;

public class IrGestureManager {
    private static final String TAG = "CMActions-IRGestureManager";

    // Something occludes the sensor
    public static final int IR_GESTURE_OBJECT_DETECTED             = 1;
    // No occlusion
    public static final int IR_GESTURE_GESTURE_OBJECT_NOT_DETECTED = 2;
    // Swiping above the phone (send doze)
    public static final int IR_GESTURE_SWIPE                       = 3;
    // Hand wave in front of the phone (send doze)
    public static final int IR_GESTURE_APPROACH                    = 4;
    // Gestures not tracked
    public static final int IR_GESTURE_COVER                       = 5;
    public static final int IR_GESTURE_DEPART                      = 6;
    public static final int IR_GESTURE_HOVER                       = 7;
    public static final int IR_GESTURE_HOVER_PULSE                 = 8;
    public static final int IR_GESTURE_PROXIMITY_NONE              = 9;
    public static final int IR_GESTURE_HOVER_FIST                  = 10;
    public static final int IR_GESTURE_LAST                        = IR_GESTURE_HOVER_FIST;

    private int mVotes[] = new int[IR_GESTURE_LAST+1];
    private int mGestures;

    static
    {
       System.load("/system/lib64/libjni_CMActions.so");
    }

    public IrGestureManager() {
        nativeSetIrDisabled(true);
    }

    public synchronized void updateState(int oldFlags, int newFlags) {
        voteDelta(oldFlags, -1);
        int gestures = voteDelta(newFlags, +1);

        updateSensorConfig(gestures);
    }

    private synchronized int voteDelta(int flags, int delta) {
        int gestures = 0;
        for (int i = 0; i <= IR_GESTURE_LAST; i++) {
            if ((flags & (1<<i)) != 0) {
                mVotes[i] += delta;
            }
            if (mVotes[i] > 0) {
                gestures |= (1 << i);
            }
        }

        return gestures;
    }

    private synchronized void updateSensorConfig(int gestures) {
        if (mGestures != gestures) {
            boolean irDisabled = (gestures == 0);
            mGestures = gestures;

            if (!nativeSetIrDisabled(irDisabled)) {
                Log.e(TAG, "Failed setting IR disabled " + irDisabled);
            }
            if (! nativeSetIrWakeConfig(mGestures)) {
                Log.e(TAG, "Failed setting IR gestures " + mGestures);
            }
        }
    }

    private final native boolean nativeSetIrDisabled(boolean disabled);
    private final native boolean nativeSetIrWakeConfig(int wakeConfig);
}
