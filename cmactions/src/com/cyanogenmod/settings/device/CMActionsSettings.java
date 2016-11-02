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

import android.content.ContentResolver;
import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.provider.Settings;

import android.util.Log;

public class CMActionsSettings {
    private static final String TAG = "CMActions";

    private static final int ACTION_NONE = 0;
    private static final int ACTION_LAUNCH_CAMERA = 1;
    private static final int ACTION_TORCH = 2;

    private static final String GESTURE_USER_AWARE_DISPLAY_KEY = "gesture_user_aware_display";
    private static final String GESTURE_CAMERA_ACTION_KEY = "gesture_camera_action";
    private static final String GESTURE_CHOP_CHOP_KEY = "gesture_chop_chop";
    private static final String GESTURE_FEEDBACK_INTENSITY_KEY = "gesture_feedback_intensity";
    private static final String GESTURE_PICK_UP_KEY = "gesture_pick_up";
    private static final String GESTURE_IR_WAKEUP_KEY = "gesture_ir_wake_up";
    private static final String GESTURE_IR_SILENCER_KEY = "gesture_ir_silencer";

    private final Context mContext;
    private final UpdatedStateNotifier mUpdatedStateNotifier;

    private boolean mUserAwareDisplayEnabled;
    private int mCameraGestureAction;
    private int mChopChopAction;
    private int mFeedbackIntensity;
    private boolean mIrWakeUpEnabled;
    private boolean mIrSilencerEnabled;
    private boolean mPickUpGestureEnabled;

    public CMActionsSettings(Context context, UpdatedStateNotifier updatedStateNotifier) {
        SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(context);
        loadPreferences(sharedPrefs);
        sharedPrefs.registerOnSharedPreferenceChangeListener(mPrefListener);
        mContext = context;
        mUpdatedStateNotifier = updatedStateNotifier;
    }

    public SensorAction newCameraActivationAction() {
        return new ConfigurableAction(true);
    }

    public SensorAction newChopChopAction() {
        return new ConfigurableAction(false);
    }

    public boolean isUserAwareDisplayEnabled() {
        return mUserAwareDisplayEnabled;
    }

    public boolean isCameraGestureEnabled() {
        return mCameraGestureAction != ACTION_NONE;
    }

    public boolean isChopChopGestureEnabled() {
        return mChopChopAction != ACTION_NONE;
    }

    public static boolean isDozeEnabled(ContentResolver contentResolver) {
        return (Settings.Secure.getInt(contentResolver, Settings.Secure.DOZE_ENABLED, 1) != 0);
    }

    public boolean isDozeEnabled() {
        return isDozeEnabled(mContext.getContentResolver());
    }

    public boolean isIrWakeupEnabled() {
        return isDozeEnabled() && mIrWakeUpEnabled;
    }

    public boolean isPickUpEnabled() {
        return isDozeEnabled() && mPickUpGestureEnabled;
    }

    public boolean isIrSilencerEnabled() {
        return mIrSilencerEnabled;
    }

    private void loadPreferences(SharedPreferences sharedPreferences) {
        mUserAwareDisplayEnabled = sharedPreferences.getBoolean(GESTURE_USER_AWARE_DISPLAY_KEY, false);
        mCameraGestureAction = getIntPreference(sharedPreferences, GESTURE_CAMERA_ACTION_KEY);
        mChopChopAction = getIntPreference(sharedPreferences, GESTURE_CHOP_CHOP_KEY);
        mFeedbackIntensity = getIntPreference(sharedPreferences, GESTURE_FEEDBACK_INTENSITY_KEY);
        mIrWakeUpEnabled = sharedPreferences.getBoolean(GESTURE_IR_WAKEUP_KEY, false);
        mPickUpGestureEnabled = sharedPreferences.getBoolean(GESTURE_PICK_UP_KEY, false);
        mIrSilencerEnabled = sharedPreferences.getBoolean(GESTURE_IR_SILENCER_KEY, false);
    }

    private int getIntPreference(SharedPreferences sharedPreferences, String key) {
        String value = sharedPreferences.getString(key, "0");
        return Integer.valueOf(value);
    }

    private SharedPreferences.OnSharedPreferenceChangeListener mPrefListener =
            new SharedPreferences.OnSharedPreferenceChangeListener() {
        @Override
        public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
            boolean updated = true;

            if (GESTURE_USER_AWARE_DISPLAY_KEY.equals(key)) {
                mUserAwareDisplayEnabled = sharedPreferences.getBoolean(GESTURE_USER_AWARE_DISPLAY_KEY, false);
            } else if (GESTURE_CAMERA_ACTION_KEY.equals(key)) {
                mCameraGestureAction = getIntPreference(sharedPreferences, GESTURE_CAMERA_ACTION_KEY);
            } else if (GESTURE_CHOP_CHOP_KEY.equals(key)) {
                mChopChopAction = getIntPreference(sharedPreferences, GESTURE_CHOP_CHOP_KEY);
            } else if (GESTURE_FEEDBACK_INTENSITY_KEY.equals(key)) {
                mFeedbackIntensity = getIntPreference(sharedPreferences, GESTURE_FEEDBACK_INTENSITY_KEY);
            } else if (GESTURE_IR_WAKEUP_KEY.equals(key)) {
                mIrWakeUpEnabled = sharedPreferences.getBoolean(GESTURE_IR_WAKEUP_KEY, false);
            } else if (GESTURE_PICK_UP_KEY.equals(key)) {
                mPickUpGestureEnabled = sharedPreferences.getBoolean(GESTURE_PICK_UP_KEY, false);
            } else if (GESTURE_IR_SILENCER_KEY.equals(key)) {
                mIrSilencerEnabled = sharedPreferences.getBoolean(GESTURE_IR_SILENCER_KEY, false);
            } else {
                updated = false;
            }

            if (updated) {
                mUpdatedStateNotifier.updateState();
            }
        }
    };

    private class ConfigurableAction implements SensorAction {
        private final boolean mIsCamera;

        public ConfigurableAction(boolean isCamera) {
             mIsCamera = isCamera;
        }

        @Override
        public void action() {
            if (mIsCamera) {
                action(mCameraGestureAction);
            } else {
                action(mChopChopAction);
            }
        }

        private void action(int action) {
            int vibratorPeriod = mFeedbackIntensity * 80;
            if (action == ACTION_LAUNCH_CAMERA) {
                new CameraActivationAction(mContext, vibratorPeriod).action();
            } else if (action == ACTION_TORCH) {
                new TorchAction(mContext, vibratorPeriod).action();
            }
        }
    };
}
