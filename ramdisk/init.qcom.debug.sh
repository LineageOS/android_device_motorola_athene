#!/system/bin/sh
# Copyright (c) 2014-2015, The Linux Foundation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of The Linux Foundation nor
#       the names of its contributors may be used to endorse or promote
#       products derived from this software without specific prior written
#       permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# function to enable ftrace events to CoreSight STM
target=`getprop ro.board.platform`
HERE=/system/etc
source $HERE/init.qcom.debug-8976.sh
debug_8976=`getprop persist.debug.8976.config`

if [ -f /sys/devices/soc0/soc_id ]; then
    soc_id=`cat /sys/devices/soc0/soc_id`
else
    soc_id=`cat /sys/devices/system/soc/soc0/id`
fi

enable_stm_events()
{
    # bail out if its perf config
    if [ ! -d /sys/module/msm_rtb ]
    then
        return
    fi
    # bail out if coresight isn't present
    if [ ! -d /sys/bus/coresight ]
    then
        return
    fi
    # bail out if ftrace events aren't present
    if [ ! -d /sys/kernel/debug/tracing/events ]
    then
        return
    fi

    echo 1 > /sys/bus/coresight/devices/coresight-tmc-etr/curr_sink
    echo 1 > /sys/bus/coresight/devices/coresight-stm/enable
    echo 1 > /sys/kernel/debug/tracing/tracing_on
    # schedular
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_cpu_hotplug/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_cpu_load/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_enq_deq_task/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_load_balance/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_migrate_task/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_switch/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_task_load/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_update_history/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_update_task_ravg/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_wakeup/enable
    echo 1 > /sys/kernel/debug/tracing/events/sched/sched_wakeup_new/enable
    # sound
    echo 1 > /sys/kernel/debug/tracing/events/asoc/snd_soc_reg_read/enable
    echo 1 > /sys/kernel/debug/tracing/events/asoc/snd_soc_reg_write/enable
    # mdp
    echo 1 > /sys/kernel/debug/tracing/events/mdss/mdp_video_underrun_done/enable
    # video
    echo 1 > /sys/kernel/debug/tracing/events/msm_vidc/enable
    # clock
    echo 1 > /sys/kernel/debug/tracing/events/power/clock_set_rate/enable
    # regulator
    echo 1 > /sys/kernel/debug/tracing/events/regulator/enable
    # power
    echo 1 > /sys/kernel/debug/tracing/events/msm_low_power/enable
    #thermal
    echo 1 > /sys/kernel/debug/tracing/events/thermal/thermal_pre_core_offline/enable
    echo 1 > /sys/kernel/debug/tracing/events/thermal/thermal_post_core_offline/enable
    echo 1 > /sys/kernel/debug/tracing/events/thermal/thermal_pre_core_online/enable
    echo 1 > /sys/kernel/debug/tracing/events/thermal/thermal_post_core_online/enable
    echo 1 > /sys/kernel/debug/tracing/events/thermal/thermal_pre_frequency_mit/enable
    echo 1 > /sys/kernel/debug/tracing/events/thermal/thermal_post_frequency_mit/enable
}

# Function MSM8996 DCC configuration
enable_msm8996_dcc_config()
{
    DCC_PATH="/sys/bus/platform/devices/4b3000.dcc"
    if [ ! -d $DCC_PATH ]; then
        echo "DCC don't exist on this build."
        return
    fi

    echo  0 > $DCC_PATH/enable
    echo cap > $DCC_PATH/func_type
    echo sram > $DCC_PATH/data_sink
    echo  1 > $DCC_PATH/config_reset

    #SPM Registers
    # CPU0
    echo  0x998000C > $DCC_PATH/config
    echo  0x9980030 > $DCC_PATH/config
    echo  0x998003C > $DCC_PATH/config
    # CPU1
    echo  0x999000C > $DCC_PATH/config
    echo  0x9990030 > $DCC_PATH/config
    echo  0x999003C > $DCC_PATH/config
    # CPU2
    echo  0x99B000C > $DCC_PATH/config
    echo  0x99B0030 > $DCC_PATH/config
    echo  0x99B003C > $DCC_PATH/config
    # CPU3
    echo  0x99C000C > $DCC_PATH/config
    echo  0x99C0030 > $DCC_PATH/config
    echo  0x99C003C > $DCC_PATH/config
    # PWRL2
    echo  0x99A000C > $DCC_PATH/config
    echo  0x99A0030 > $DCC_PATH/config
    echo  0x99A003C > $DCC_PATH/config
    # PERFL2
    echo  0x99D000C > $DCC_PATH/config
    echo  0x99D0030 > $DCC_PATH/config
    echo  0x99D003C > $DCC_PATH/config
    # L3
    echo  0x9A0000C > $DCC_PATH/config
    echo  0x9A00030 > $DCC_PATH/config
    echo  0x9A0003C > $DCC_PATH/config
    # CBF
    echo  0x9A1000C > $DCC_PATH/config
    echo  0x9A10030 > $DCC_PATH/config
    echo  0x9A1003C > $DCC_PATH/config
    # PWR L2 HW-FLUSH
    echo  0x99A1060 > $DCC_PATH/config
    # PERF L2 HW-FLUSH
    echo  0x99D1060 > $DCC_PATH/config
    # APCS_APC0_SLEEP_EN_VOTE
    echo  0x99A2030 > $DCC_PATH/config
    # APCS_APCC_SW_EN_VOTE
    echo  0x99E0020 > $DCC_PATH/config

    echo  1 > $DCC_PATH/enable
}

# Function DCC configuration
enable_dcc_config()
{
    case "$target" in
        "msm8996")
            echo "Enabling DCC config for 8996."
            enable_msm8996_dcc_config
            ;;
    esac
}

if [ $target == "msm8952" ] && $debug_8976; then
case "$soc_id" in
      "266" | "274" | "277" | "278")
      echo "Enabling Debug config for 8976."
      enable_msm8976_debug
      ;;
esac
else
coresight_config=`getprop persist.debug.coresight.config`
enable_dcc_config
case "$coresight_config" in
    "stm-events")
        echo "Enabling STM events."
        enable_stm_events
        ;;
    *)
        echo "Skipping coresight configuration."
        exit
        ;;
esac
fi
