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

import static com.cyanogenmod.settings.device.IrGestureManager.*;

public class IrGestureSensor implements ScreenStateNotifier, SensorEventListener {
    private static final String TAG = "CMActions-IRGestureSensor";

    private static final int IR_GESTURES_FOR_SCREEN_OFF = (1 << IR_GESTURE_SWIPE) | (1 << IR_GESTURE_APPROACH);

    private final CMActionsSettings mCMActionsSettings;
    private final SensorHelper mSensorHelper;
    private final SensorAction mSensorAction;
    private final IrGestureVote mIrGestureVote;
    private final Sensor mSensor;

    private boolean mEnabled;

    public IrGestureSensor(CMActionsSettings cmActionsSettings, SensorHelper sensorHelper,
                SensorAction action, IrGestureManager irGestureManager) {
        mCMActionsSettings = cmActionsSettings;
        mSensorHelper = sensorHelper;
        mSensorAction = action;
        mIrGestureVote = new IrGestureVote(irGestureManager);

        mSensor = sensorHelper.getIrGestureSensor();
        mIrGestureVote.voteForSensors(0);
    }

    @Override
    public void screenTurnedOn() {
        if (mEnabled) {
            Log.d(TAG, "Disabling");
            mSensorHelper.unregisterListener(this);
            mIrGestureVote.voteForSensors(0);
            mEnabled = false;
        }
    }

    @Override
    public void screenTurnedOff() {
        if (mCMActionsSettings.isIrWakeupEnabled() && !mEnabled) {
            Log.d(TAG, "Enabling");
            mSensorHelper.registerListener(mSensor, this);
            mIrGestureVote.voteForSensors(IR_GESTURES_FOR_SCREEN_OFF);
            mEnabled = true;
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        int gesture = (int) event.values[1];

        if (gesture == IR_GESTURE_SWIPE || gesture == IR_GESTURE_APPROACH) {
            Log.d(TAG, "event: [" + event.values.length + "]: " + event.values[0] + ", " +
                event.values[1] + ", " + event.values[2]);
            mSensorAction.action();
        }
    }

    @Override
    public void onAccuracyChanged(Sensor mSensor, int accuracy) {
    }
}
