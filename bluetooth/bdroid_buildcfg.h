/*
 * Copyright (C) 2015 The CyanogenMod Project
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

#ifndef _BDROID_BUILDCFG_H
#define _BDROID_BUILDCFG_H

#include <cutils/properties.h>
#include <string.h>

inline const char* BtmGetDefaultName()
{
	char device[PROPERTY_VALUE_MAX];
	property_get("ro.boot.device", device, "");

	if (!strcmp("surnia", device)) {
		return "Moto E";
	} else if (!strcmp("osprey", device)) {
		return "Moto G";
	} else if (!strcmp("lux", device)) {
		return "Moto X Play";
	}

	return "Motorola";
}

#define BTM_DEF_LOCAL_NAME BtmGetDefaultName()
#define BTA_DISABLE_DELAY 1000 /* in milliseconds */
#define BLUETOOTH_QCOM_SW TRUE
#define BLUETOOTH_QCOM_LE_INTL_SCAN TRUE
#define BTC_INCLUDED TRUE
#endif
