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

/* Target-Specific Dalvik Heap Configuration */
void target_2gb() {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "192m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "512k");
        property_set("dalvik.vm.heapmaxfree", "8m");
}

void target_3gb() {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "96m");
        property_set("dalvik.vm.heapsize", "256m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "2m");
        property_set("dalvik.vm.heapmaxfree", "8m");
}

void msim() {
        property_set("persist.radio.multisim.config", "dsds");
}

void ssim() {
        property_set("persist.radio.multisim.config", "");
}

void vendor_load_properties()
{
    char platform[PROP_VALUE_MAX];
    char radio[PROP_VALUE_MAX];
    char sku[PROP_VALUE_MAX];
    char carrier[PROP_VALUE_MAX];
    char device[PROP_VALUE_MAX];
    char devicename[PROP_VALUE_MAX];
    int rc;

    rc = property_get("ro.board.platform", platform);
    if (!rc || !ISMATCH(platform, ANDROID_TARGET))
    return;

    property_get("ro.boot.radio", radio);
    property_get("ro.boot.hardware.sku", sku);
    property_get("ro.boot.carrier", carrier);

    /* Common for all models */
    property_set("ro.build.product", "athene");
    property_set("ro.product.model", "Moto G (4)");
    property_set("ro.mot.build.customerid", "retail");
    property_set("persist.radio.mot_ecc_custid", "common");
    property_set("persist.radio.mot_ecc_enabled", "1");
    property_set("persist.radio.force_get_pref", "1");
    property_set("ro.telephony.default_network", "10,0");

    if (ISMATCH(sku, "XT1622")) {
        /* XT1622 */
        target_2gb();
        msim();
        property_set("ro.product.device", "athene");
        property_set("ro.build.description", "athene-user 6.0.1 MPJ24.139-23.4 4 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene/athene:6.0.1/MPJ24.139-23.4/4:user/release-keys");
    } else if (ISMATCH(sku, "XT1625")) {
        /* XT1625 */
        target_2gb();
        ssim();
        property_set("ro.product.device", "athene");
        property_set("ro.build.description", "athene-user 6.0.1 MPJ24.139-48 48 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene/athene:6.0.1/MPJ24.139-48/48:user/release-keys");
    } else if (ISMATCH(sku, "XT1642")) {
        /* XT1642 */
        target_2gb();
        msim();
        property_set("ro.product.device", "athene_f");
        property_set("ro.build.description", "athene_f-user 6.0.1 MPJ24.139-23.4 4 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene_f/athene_f:6.0.1/MPJ24.139-23.4/4:user/release-keys");
    } else if (ISMATCH(sku, "XT1643")) {
        /* XT1643 */
        target_3gb();
        msim();
        property_set("ro.product.device", "athene_f");
        property_set("ro.build.description", "athene_f-user 6.0.1 MPJ24.139-23.1 1 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene_f/athene_f:6.0.1/MPJ24.139-23.1/1:user/release-keys");
}

    property_get("ro.product.device", device);
    strlcpy(devicename, device, sizeof(devicename));
    INFO("Found sku id: %s setting build properties for %s device\n", sku, devicename);
}
