#
# Copyright (C) 2016 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

VENDOR_PATH := device/motorola/msm8952-common

BOARD_VENDOR := motorola-qcom

TARGET_SPECIFIC_HEADER_PATH := $(VENDOR_PATH)/include

# Platform
TARGET_BOARD_PLATFORM := msm8952
TARGET_BOARD_PLATFORM_GPU := qcom-adreno405

TARGET_CPU_CORTEX_A53 := true

# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := MSM8952
TARGET_NO_BOOTLOADER := true

# Architecture
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_VARIANT := cortex-a53
TARGET_CPU_SMP := true
ARCH_ARM_HAVE_TLS_REGISTER := true
TARGET_GLOBAL_CFLAGS += -mfpu=neon -mfloat-abi=softfp
TARGET_GLOBAL_CPPFLAGS += -mfpu=neon -mfloat-abi=softfp

# Kernel
BOARD_KERNEL_CMDLINE := console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 androidboot.hardware=qcom msm_rtb.filter=0x237 ehci-hcd.park=3 androidboot.bootdevice=7824900.sdhci lpm_levels.sleep_disabled=1 earlyprintk androidboot.selinux=permissive
BOARD_KERNEL_CMDLINE += androidboot.selinux=permissive
BOARD_KERNEL_BASE := 0x80000000
BOARD_KERNEL_PAGESIZE := 2048
BOARD_KERNEL_SEPARATED_DT := true
BOARD_MKBOOTIMG_ARGS := --ramdisk_offset 0x01000000 --tags_offset 0x00000100
TARGET_CUSTOM_DTBTOOL := dtbTool_moto
TARGET_KERNEL_ARCH := arm
TARGET_KERNEL_CONFIG := cm_athene_defconfig
TARGET_KERNEL_SOURCE := kernel/motorola/msm8952
KERNEL_TOOLCHAIN := $(ANDROID_BUILD_TOP)/prebuilts/gcc/$(HOST_OS)-x86/arm/arm-eabi-4.8/bin
TARGET_KERNEL_CROSS_COMPILE_PREFIX := arm-eabi-

# Audio
AUDIO_FEATURE_ENABLED_KPI_OPTIMIZE := true
AUDIO_FEATURE_ENABLED_NEW_SAMPLE_RATE := true
AUDIO_FEATURE_ENABLED_MULTI_VOICE_SESSIONS := true
BOARD_USES_ALSA_AUDIO := true
USE_CUSTOM_AUDIO_POLICY := 1

# RIL
BOARD_PROVIDES_RILD := true
BOARD_PROVIDES_LIBRIL := true

# Boot animation
TARGET_BOOTANIMATION_MULTITHREAD_DECODE := true

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(VENDOR_PATH)/bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_QCOM := true
BLUETOOTH_HCI_USE_MCT := true

# Camera
USE_DEVICE_SPECIFIC_CAMERA := true

# CMHW
BOARD_USES_CYANOGEN_HARDWARE := true
BOARD_HARDWARE_CLASS += \
    $(VENDOR_PATH)/cmhw \
    hardware/cyanogen/cmhw

# Display
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
TARGET_USES_C2D_COMPOSITION := true
TARGET_USES_ION := true
TARGET_USES_NEW_ION_API :=true
TARGET_CONTINUOUS_SPLASH_ENABLED := true
TARGET_FORCE_HWC_FOR_VIRTUAL_DISPLAYS := true
USE_OPENGL_RENDERER := true
SF_VSYNC_EVENT_PHASE_OFFSET_NS := 2000000
VSYNC_EVENT_PHASE_OFFSET_NS := 6000000

MAX_EGL_CACHE_KEY_SIZE := 12*1024
MAX_EGL_CACHE_SIZE := 2048*1024

HAVE_ADRENO_SOURCE:= false
OVERRIDE_RS_DRIVER:= libRSDriver_adreno.so

# Filesystem
BOARD_BOOTIMAGE_PARTITION_SIZE := 16777216	# 16384 * 1024 mmcblk0p28
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 16777216	# 16384 * 1024 mmcblk0p29
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 2684354560	# 2621440 * 1024 mmcblk0p47
BOARD_PERSISTIMAGE_PARTITION_SIZE := 33554432	# 32768 * 1024 mmcblk0p30
BOARD_USERDATAIMAGE_PARTITION_SIZE := 27258650624 # 26619776 * 1024 mmcblk0p48

# FM
TARGET_QCOM_NO_FM_FIRMWARE := true

# Init
TARGET_INIT_VENDOR_LIB := libinit_msm8952
TARGET_RECOVERY_DEVICE_MODULES := libinit_msm8952
TARGET_UNIFIED_DEVICE := true

# GPS
USE_DEVICE_SPECIFIC_GPS := true
USE_DEVICE_SPECIFIC_LOC_API := true
TARGET_NO_RPC := true

# Partitions
BOARD_FLASH_BLOCK_SIZE := 131072

# Power
TARGET_POWERHAL_VARIANT := qcom

# CPU
ENABLE_CPUSETS := true

# Properties
TARGET_SYSTEM_PROP += $(VENDOR_PATH)/system.prop

# Crypto
TARGET_HW_DISK_ENCRYPTION := true

# Qualcomm support
BOARD_USES_QCOM_HARDWARE := true
BOARD_USES_QC_TIME_SERVICES := true

# Keymaster
TARGET_PROVIDES_KEYMASTER := true
TARGET_KEYMASTER_WAIT_FOR_QSEE := true

# CNE and DPM
TARGET_LDPRELOAD := libNimsWrap.so
BOARD_USES_QCNE := true

# Sensors
USE_SENSOR_MULTI_HAL := true

# Recovery
BOARD_HAS_NO_SELECT_BUTTON := true
TARGET_RECOVERY_FSTAB := $(VENDOR_PATH)/rootdir/etc/fstab.qcom
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true

# SELinux
include device/qcom/sepolicy/sepolicy.mk
BOARD_SEPOLICY_DIRS += $(VENDOR_PATH)/sepolicy

# Wifi
BOARD_HAS_QCOM_WLAN := true
BOARD_HAS_QCOM_WLAN_SDK := true
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_qcwcn
BOARD_WLAN_DEVICE := qcwcn
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_qcwcn
WIFI_DRIVER_FW_PATH_AP := "ap"
WIFI_DRIVER_FW_PATH_STA := "sta"
WPA_SUPPLICANT_VERSION := VER_0_8_X
