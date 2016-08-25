/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

#define ISMATCH(a,b)    (!strncmp(a,b,PROP_VALUE_MAX))

void num_sims() {
    char dualsim[PROP_VALUE_MAX];
    property_get("ro.boot.dualsim", dualsim);
	property_set("ro.hw.dualsim", dualsim);

    if (ISMATCH(dualsim, "true")) {
        property_set("persist.radio.multisim.config", "dsds");
	} else {
        property_set("persist.radio.multisim.config", "");
	}
}

void vendor_load_properties()
{
    char platform[PROP_VALUE_MAX];
    char device_boot[PROP_VALUE_MAX];
    char sku[PROP_VALUE_MAX];
    char carrier[PROP_VALUE_MAX];
    char device[PROP_VALUE_MAX];
    char devicename[PROP_VALUE_MAX];
	char radio[PROP_VALUE_MAX];
    int rc;

    rc = property_get("ro.board.platform", platform);
    if (!rc || !ISMATCH(platform, ANDROID_TARGET))
    return;

    property_get("ro.boot.device", device_boot);
	property_set("ro.hw.device", device_boot);
	
    property_get("ro.boot.hardware.sku", sku);
    property_get("ro.boot.carrier", carrier);
	
	property_get("ro.boot.radio", radio);
    property_set("ro.hw.radio", radio);
	
    /* Common for all models */
    property_set("ro.build.product", "athene");
    num_sims();

    if (ISMATCH(device_boot, "athene_13mp")) {
        /* Moto G4 (XT162x) */
        property_set("ro.product.device", "athene");
        property_set("ro.build.description", "athene-user 6.0.1 MPJ24.139-23.4 4 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene/athene:6.0.1/MPJ24.139-23.4/4:user/release-keys");
        property_set("ro.hw.fps", "false");
        property_set("ro.product.model", "Moto G4");	
    } else {
        /* Moto G4 Plus (XT164x) */
        property_set("ro.product.device", "athene_f");
        property_set("ro.build.description", "athene_f-user 6.0.1 MPJ24.139-23.4 4 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene_f/athene_f:6.0.1/MPJ24.139-23.4/4:user/release-keys");
        property_set("ro.hw.fps", "true");
        property_set("ro.product.model", "Moto G4 Plus");
    }
	
	if (ISMATCH(sku, "XT1625") || ISMATCH(sku, "XT1644")) {
		property_set("persist.radio.is_wps_enabled", "true");
		property_set("ro.radio.imei.sv", "4");
	}
	
	if (ISMATCH(sku, "XT1621") || ISMATCH(sku, "XT1622") || ISMATCH(sku, "XT1640") || ISMATCH(sku, "XT1642") || ISMATCH(sku, "XT1643")) {
		property_set("ro.radio.imei.sv", "3");
	}
	
	if (ISMATCH(sku, "XT1626") || ISMATCH(sku, "XT1641")) {
		property_set("ro.radio.imei.sv", "2");
		property_set("persist.radio.is_wps_enabled", "true");
		property_set("persist.radio.pb.max.match", "10");
	}
	
    property_get("ro.product.device", device);
    strlcpy(devicename, device, sizeof(devicename));
    INFO("Found sku id: %s setting build properties for %s device\n", sku, devicename);
}
