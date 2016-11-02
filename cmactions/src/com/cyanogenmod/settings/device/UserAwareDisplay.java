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

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.os.Handler;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;

import android.app.KeyguardManager;

import android.util.Log;

import static com.cyanogenmod.settings.device.IrGestureManager.*;

public class UserAwareDisplay implements ScreenStateNotifier {
    private static final String TAG = "CMActions-UAD";

    private static final int DELAYED_OFF_MS = 3000;
    private static final int KEYGUARD_POLL_MS = 1000;

    private static final int IR_GESTURES_FOR_SCREEN_ON = (1 << IR_GESTURE_OBJECT_DETECTED) |
            (1 << IR_GESTURE_GESTURE_OBJECT_NOT_DETECTED);
    private static final int IR_GESTURES_FOR_SCREEN_OFF = 0;

    private final CMActionsSettings mCMActionsSettings;
    private final SensorHelper mSensorHelper;
    private final IrGestureVote mIrGestureVote;
    private final PowerManager mPowerManager;
    private final KeyguardManager mKeyguardManager;
    private final Sensor mIrGestureSensor;
    private final Sensor mStowSensor;
    private final WakeLock mWakeLock;
    private final WakeLock mDelayedOffWakeLock;
    private Handler mHandler;

    private boolean mEnabled;
    private boolean mScreenIsLocked;
    private boolean mObjectIsDetected;
    private boolean mIsStowed;

    public UserAwareDisplay(CMActionsSettings cmActionsSettings, SensorHelper sensorHelper,
                IrGestureManager irGestureManager, Context context) {
        mCMActionsSettings = cmActionsSettings;
        mSensorHelper = sensorHelper;
        mIrGestureVote = new IrGestureVote(irGestureManager);
        mPowerManager = (PowerManager) context.getSystemService(Context.POWER_SERVICE);

        mKeyguardManager = (KeyguardManager) context.getSystemService(Context.KEYGUARD_SERVICE);

        mWakeLock = mPowerManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK, TAG);
        mDelayedOffWakeLock = mPowerManager.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, TAG);
        mHandler = new Handler();

        mIrGestureSensor = sensorHelper.getIrGestureSensor();
        mStowSensor = sensorHelper.getStowSensor();
        mIrGestureVote.voteForSensors(0);
    }

    @Override
    public void screenTurnedOn() {
        if (mCMActionsSettings.isUserAwareDisplayEnabled()) {
            if (mKeyguardManager.inKeyguardRestrictedInputMode()) {
                scheduleKeyguardPoll();
            } else {
                enableSensors();
            }
        } else {
            // Option was potentially disabled while the screen is on, make
            // sure everything is turned off if it was enabled.
            screenTurnedOff();
        }
    }

    @Override
    public void screenTurnedOff() {
        disableKeyguardPolling();
        disableSensors();
        disableScreenLock();
    }

    private void scheduleKeyguardPoll() {
        mHandler.postDelayed(mCheckKeyguard, KEYGUARD_POLL_MS);
    }

    private void disableKeyguardPolling() {
        mHandler.removeCallbacks(mCheckKeyguard);
    }

    private void enableSensors() {
        if (! mEnabled) {
            Log.d(TAG, "Enabling");

            mEnabled = true;
            mObjectIsDetected = false;
            mIsStowed = false;

            mSensorHelper.registerListener(mIrGestureSensor, mIrGestureListener);
            mSensorHelper.registerListener(mStowSensor, mStowListener);
            mIrGestureVote.voteForSensors(IR_GESTURES_FOR_SCREEN_ON);
        }
    }

    private void disableSensors() {
        if (mEnabled) {
            Log.d(TAG, "Disabling");
            mSensorHelper.unregisterListener(mStowListener);
            mSensorHelper.unregisterListener(mIrGestureListener);
            mIrGestureVote.voteForSensors(IR_GESTURES_FOR_SCREEN_OFF);
            mEnabled = false;
        }
    }

    private synchronized void setIsStowed(boolean isStowed) {
        Log.d(TAG, "Stowed: " + isStowed);
        mIsStowed = isStowed;
        updateScreenLock();
    }

    private synchronized void setObjectIsDetected(boolean objectIsDetected) {
        Log.d(TAG, "IR object is detected: " + objectIsDetected);
        mObjectIsDetected = objectIsDetected;
        updateScreenLock();
    }

    private synchronized void updateScreenLock() {
        boolean isLocked = mObjectIsDetected && ! mIsStowed;

        if (isLocked) {
            enableScreenLock();
        } else {
            disableScreenLock();
        }
    }

    private synchronized void enableScreenLock() {
        if (! mScreenIsLocked) {
            Log.d(TAG, "Acquiring screen wakelock");
            mScreenIsLocked = true;
            mWakeLock.acquire();
        }
    }

    private synchronized void disableScreenLock() {
        if (mScreenIsLocked) {
            mScreenIsLocked = false;
            mDelayedOffWakeLock.acquire(DELAYED_OFF_MS);
            mWakeLock.release();
            Log.d(TAG, "Released screen wakelock");
        }
    }

    private SensorEventListener mIrGestureListener = new SensorEventListener() {
        @Override
        public void onSensorChanged(SensorEvent event) {
            int gesture = (int) event.values[1];

            if (gesture == IR_GESTURE_OBJECT_DETECTED) {
                setObjectIsDetected(true);
            } else if (gesture == IR_GESTURE_GESTURE_OBJECT_NOT_DETECTED) {
                setObjectIsDetected(false);
            }
        }

        @Override
        public void onAccuracyChanged(Sensor mSensor, int accuracy) {
        }
    };

    private SensorEventListener mStowListener = new SensorEventListener() {
        @Override
        public void onSensorChanged(SensorEvent event) {
            setIsStowed(event.values[0] != 0);
        }

        @Override
        public void onAccuracyChanged(Sensor mSensor, int accuracy) {
        }
    };

    private Runnable mCheckKeyguard = new Runnable() {
        @Override
        public void run() {
            if (! mKeyguardManager.inKeyguardRestrictedInputMode()) {
                enableSensors();
            } else {
                scheduleKeyguardPoll();
            }
        }
    };
}
