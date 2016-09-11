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

/* Target-Specific Dalvik Heap & HWUI Configuration */
void target_ram() {
    std::string ram = property_get("ro.boot.ram");

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
    } else if (ram == "3GB") {
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
    } else if (ram == "4GB") {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "384m");
        property_set("dalvik.vm.heapsize", "1024m");
        property_set("dalvik.vm.heaptargetutilization", "0.25");
        property_set("dalvik.vm.heapminfree", "4m");
        property_set("dalvik.vm.heapmaxfree", "16m");

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

void num_sims() {
    std::string dualsim = property_get("ro.boot.dualsim");
    property_set("ro.hw.dualsim", dualsim.c_str());

    if (dualsim == "true") {
            property_set("persist.radio.multisim.config", "dsds");
	} else {
            property_set("persist.radio.multisim.config", "");
	}
}

void vendor_load_properties()
{
    INFO("Starting custom ini");

    std::string platform, device_boot, sku, radio, device, carrier;

    platform = property_get("ro.board.platform");
    if (platform == ANDROID_TARGET) {
        INFO("ANDROID_TARGET");
        return;
    }

    INFO("Get device");
    device_boot = property_get("ro.boot.device");
    property_set("ro.hw.device", device_boot.c_str());

    INFO("Get sku");
    sku = property_get("ro.boot.hardware.sku");

    INFO("Get carrier");
    carrier = property_get("ro.boot.carrier");

    INFO("Get radio");
    radio = property_get("ro.boot.radio");
    property_set("ro.hw.radio", radio.c_str());

    /* Common for all models */
    property_set("ro.build.product", "athene");
    INFO("Set RAM");
    target_ram();
    INFO("Set SIM");
    num_sims();

    if (device_boot == "athene_13mp") {
        /* Moto G4 (XT162x) */
        property_set("ro.product.device", "athene");
        property_set("ro.build.description", "athene-user 6.0.1 MPJ24.139-23.4 4 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene/athene:6.0.1/MPJ24.139-23.4/4:user/release-keys");
        property_set("ro.product.model", "Moto G⁴");	
        property_set("ro.telephony.default_network", "10");
    } else {
        /* Moto G4 Plus (XT164x) */
        property_set("ro.product.device", "athene_f");
        property_set("ro.build.description", "athene_f-user 6.0.1 MPJ24.139-23.4 4 release-keys");
        property_set("ro.build.fingerprint", "motorola/athene_f/athene_f:6.0.1/MPJ24.139-23.4/4:user/release-keys");
        property_set("ro.product.model", "Moto G⁴ Plus");
        property_set("ro.telephony.default_network", "10,10");
    }

	if (sku == "XT1625" || sku == "XT1644") {
		property_set("persist.radio.is_wps_enabled", "true");
		property_set("ro.radio.imei.sv", "4");
	}

	if (sku == "XT1621" || sku == "XT1622" || sku == "XT1640" || sku == "XT1642" || sku == "XT1643") {
		property_set("ro.radio.imei.sv", "3");
	}

	if (sku == "XT1626" || sku == "XT1641") {
		property_set("ro.radio.imei.sv", "2");
		property_set("persist.radio.is_wps_enabled", "true");
		property_set("persist.radio.pb.max.match", "10");
	}

    device = property_get("ro.product.device");
    INFO("Found sku id: %s setting build properties for %s device\n", sku.c_str() , device.c_str());
}
