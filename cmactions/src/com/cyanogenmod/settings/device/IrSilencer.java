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
import android.telephony.PhoneStateListener;
import android.telecom.TelecomManager;
import android.telephony.TelephonyManager;
import android.util.Log;

import static com.cyanogenmod.settings.device.IrGestureManager.*;
import static android.telephony.TelephonyManager.*;

public class IrSilencer extends PhoneStateListener implements SensorEventListener, UpdatedStateNotifier {
    private static final String TAG = "CMActions-IRSilencer";

    private static final int IR_GESTURES_FOR_RINGING = (1 << IR_GESTURE_SWIPE);
    private static final int SILENCE_DELAY_MS = 500;

    private final TelecomManager mTelecomManager;
    private final TelephonyManager mTelephonyManager;
    private final CMActionsSettings mCMActionsSettings;
    private final SensorHelper mSensorHelper;
    private final Sensor mSensor;
    private final IrGestureVote mIrGestureVote;

    private boolean mIsRinging;
    private long mRingStartedMs;

    public IrSilencer(CMActionsSettings cmActionsSettings, Context context,
                SensorHelper sensorHelper, IrGestureManager irGestureManager) {
        mTelecomManager = (TelecomManager) context.getSystemService(Context.TELECOM_SERVICE);
        mTelephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);

        mCMActionsSettings = cmActionsSettings;
        mSensorHelper = sensorHelper;
        mSensor = sensorHelper.getIrGestureSensor();
        mIrGestureVote = new IrGestureVote(irGestureManager);
        mIrGestureVote.voteForSensors(0);
    }

    @Override
    public void updateState() {
        if (mCMActionsSettings.isIrSilencerEnabled()) {
            mTelephonyManager.listen(this, LISTEN_CALL_STATE);
        } else {
            mTelephonyManager.listen(this, 0);
        }
    }

    @Override
    public synchronized void onSensorChanged(SensorEvent event) {
        int gesture = (int) event.values[1];

        if (gesture == IR_GESTURE_SWIPE && mIsRinging) {
            Log.d(TAG, "event: [" + event.values.length + "]: " + event.values[0] + ", " +
                event.values[1] + ", " + event.values[2]);
            long now = System.currentTimeMillis();
            if (now - mRingStartedMs >= SILENCE_DELAY_MS) {
                Log.d(TAG, "Silencing ringer");
                mTelecomManager.silenceRinger();
            } else {
                Log.d(TAG, "Ignoring silence gesture: " + now + " is too close to " +
                        mRingStartedMs + ", delay=" + SILENCE_DELAY_MS);
            }
        }
    }

    @Override
    public synchronized void onCallStateChanged(int state, String incomingNumber) {
        if (state == CALL_STATE_RINGING && !mIsRinging) {
            Log.d(TAG, "Ringing started");
            mSensorHelper.registerListener(mSensor, this);
            mIrGestureVote.voteForSensors(IR_GESTURES_FOR_RINGING);
            mIsRinging = true;
            mRingStartedMs = System.currentTimeMillis();
        } else if (state != CALL_STATE_RINGING && mIsRinging) {
            Log.d(TAG, "Ringing stopped");
            mSensorHelper.unregisterListener(this);
            mIrGestureVote.voteForSensors(0);
            mIsRinging = false;
        }
    }

    @Override
    public void onAccuracyChanged(Sensor mSensor, int accuracy) {
    }
}
