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
#include <stdio.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

static void num_sims(void);
static void target_ram(void);

void vendor_load_properties()
{
    INFO("Starting custom ini");

    std::string platform;
    std::string device_boot;
    std::string sku;
    std::string radio;
    std::string device;
    std::string carrier;

    platform = property_get("ro.board.platform");
    if (platform != ANDROID_TARGET)
        return;

    device_boot = property_get("ro.boot.device");
    property_set("ro.hw.device", device_boot.c_str());

    sku = property_get("ro.boot.hardware.sku");

    carrier = property_get("ro.boot.carrier");

    radio = property_get("ro.boot.radio");
    property_set("ro.hw.radio", radio.c_str());

    /* Common for all models */
    property_set("ro.build.product", "athene");
    target_ram();
    num_sims();

    if (device_boot == "athene_13mp") {
        /* Moto G4 (XT162x) */
        property_set("ro.product.device", "athene");
        property_set("ro.build.description", "athene-user 7.0 NPJ25.93-14 16 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene/athene:7.0/NPJ25.93-14/16:user/release-keys");
        property_set("ro.product.model", "Moto G4");
        property_set("ro.telephony.default_network", "10");
    } else {
        /* Moto G4 Plus (XT164x) */
        property_set("ro.product.device", "athene_f");
        property_set("ro.build.description", "athene_f-user 7.0 NPJ25.93-14 16 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene_f/athene_f:7.0/NPJ25.93-14/16:user/release-keys");
        property_set("ro.product.model", "Moto G4 Plus");
        property_set("ro.telephony.default_network", "10,10");
    }

    if (sku == "XT1625" || sku == "XT1644") {
        property_set("persist.radio.is_wps_enabled", "true");
        property_set("ro.radio.imei.sv", "4");
    }

    if (sku == "XT1621" || sku == "XT1622" || sku == "XT1640" || sku == "XT1642" || sku == "XT1643") {
        if (radio == "India") {
            property_set("ro.radio.imei.sv", "6");
            property_set("persist.radio.is_wps_enabled", "true");
        } else {
            property_set("ro.radio.imei.sv", "3");
        }
    }

    if (sku == "XT1626" || sku == "XT1641") {
        property_set("ro.radio.imei.sv", "2");
        property_set("persist.radio.is_wps_enabled", "true");
        property_set("persist.radio.pb.max.match", "10");
    }
}

/* Target-Specific Dalvik Heap & HWUI Configuration */
static void target_ram(void) {
    std::string ram;

    ram = property_get("ro.boot.ram");

    if (ram == "2GB") {
        property_set("dalvik.vm.heapstartsize", "16m");
        property_set("dalvik.vm.heapgrowthlimit", "192m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "2m");
        property_set("dalvik.vm.heapmaxfree", "8m");

        property_set("ro.hwui.texture_cache_size", "72");
        property_set("ro.hwui.layer_cache_size", "48");
        property_set("ro.hwui.r_buffer_cache_size", "8");
        property_set("ro.hwui.path_cache_size", "32");
        property_set("ro.hwui.gradient_cache_size", "1");
        property_set("ro.hwui.drop_shadow_cache_size", "6");
        property_set("ro.hwui.texture_cache_flushrate", "0.4");
        property_set("ro.hwui.text_small_cache_width", "1024");
        property_set("ro.hwui.text_small_cache_height", "1024");
        property_set("ro.hwui.text_large_cache_width", "2048");
        property_set("ro.hwui.text_large_cache_height", "1024");
    } else {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "288m");
        property_set("dalvik.vm.heapsize", "768m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "512k");
        property_set("dalvik.vm.heapmaxfree", "8m");

        property_set("ro.hwui.texture_cache_size", "72");
        property_set("ro.hwui.layer_cache_size", "48");
        property_set("ro.hwui.r_buffer_cache_size", "8");
        property_set("ro.hwui.path_cache_size", "32");
        property_set("ro.hwui.gradient_cache_size", "1");
        property_set("ro.hwui.drop_shadow_cache_size", "6");
        property_set("ro.hwui.texture_cache_flushrate", "0.4");
        property_set("ro.hwui.text_small_cache_width", "1024");
        property_set("ro.hwui.text_small_cache_height", "1024");
        property_set("ro.hwui.text_large_cache_width", "2048");
        property_set("ro.hwui.text_large_cache_height", "1024");
    }
}

static void num_sims(void) {
    std::string dualsim;

    dualsim = property_get("ro.boot.dualsim");

    property_set("ro.hw.dualsim", dualsim.c_str());

    if (dualsim == "true") {
        property_set("persist.radio.multisim.config", "dsds");
    } else {
        property_set("persist.radio.multisim.config", "");
    }
}
