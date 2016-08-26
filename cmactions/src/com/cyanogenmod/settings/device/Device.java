/*
 * Copyright (c) 2016 The CyanogenMod Project
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

import android.os.SystemProperties;

public class Device {
    private static final String TAG = "CMActions-Device";

    public static boolean isLux() {
        return checkDevice("lux");
    }

    public static boolean isMerlin() {
        return checkDevice("merlin");
    }

    public static boolean isOsprey() {
        return checkDevice("osprey");
    }

    public static boolean isSurnia() {
        return checkDevice("surnia");
    }

    private static boolean checkDevice(String codename) {
        String str = SystemProperties.get("ro.product.device");
        if (str != null) {
            return str.startsWith(codename);
        }
        return false;
    }
}
