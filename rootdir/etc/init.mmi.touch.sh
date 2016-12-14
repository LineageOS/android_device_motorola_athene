#!/system/bin/sh

PATH=/sbin:/system/sbin:/system/bin:/system/xbin
export PATH

while getopts ds op;
do
	case $op in
		d)  dbg_on=1;;
		s)  dump_statistics=1;;
	esac
done
shift $(($OPTIND-1))

scriptname=${0##*/}

debug()
{
	[ $dbg_on ] && echo "Debug: $*"
}

notice()
{
	echo "$*"
	echo "$scriptname: $*" > /dev/kmsg
}

error_and_leave()
{
	local err_msg
	local err_code=$1
	case $err_code in
		1)  err_msg="Error: No response from touch IC";;
		2)  err_msg="Error: Cannot read property $2";;
		3)  err_msg="Error: No matching firmware file found";;
		4)  err_msg="Error: Touch IC is in bootloader mode";;
		5)  err_msg="Error: Touch provides no reflash interface";;
		6)  err_msg="Error: Touch driver is not running";;
	esac
	notice "$err_msg"
	exit $err_code
}

touch_class_path=/sys/class/touchscreen
touch_product_string=$(ls $touch_class_path)
touch_status_prop=hw.touch.status

[ -z "$touch_product_string" ] && error_and_leave 6

touch_vendor=$(cat $touch_class_path/$touch_product_string/vendor)
debug "touch vendor [$touch_vendor]"
touch_path=/sys$(cat $touch_class_path/$touch_product_string/path)
debug "sysfs touch path: $touch_path"
panel_path=/sys/devices/virtual/graphics/fb0
debug "sysfs panel path: $panel_path"

if [ $dump_statistics ]; then
	debug "dumping touch statistics"
	cat $touch_path/ic_ver
	if [ -f $touch_path/stats ]; then
		cat $touch_path/stats
	fi
	return 0
fi

[ -f $touch_path/doreflash ] || error_and_leave 5
[ -f $touch_path/poweron ] || error_and_leave 5

selinux=$(getprop ro.boot.selinux 2> /dev/null)

if [ "$selinux" == "permissive" ]; then
	debug "loosen permissions to touch report sysfs entries"
	touch_report_files="reporting query stats"
	for entry in $touch_report_files; do
		chmod 0666 $touch_path/$entry
		debug "change permissions of $touch_path/$entry"
	done
	for entry in $(ls $touch_path/f54/ 2>/dev/null); do
		chmod 0666 $touch_path/f54/$entry
		debug "change permissions of $touch_path/f54/$entry"
	done
	unset touch_report_files
fi

# Set permissions to enable factory touch tests
chown root:mot_tcmd $touch_path/drv_irq
chown root:mot_tcmd $touch_path/hw_irqstat
chown root:mot_tcmd $touch_path/reset
# Set permissions to allow Bug2Go access to touch statistics
chown root:log $touch_path/stats

debug "wait until driver reports <ready to flash>..."
while true; do
	readiness=$(cat $touch_path/poweron)
	if [ "$readiness" == "1" ]; then
		debug "ready to flash!!!"
		break;
	fi
	sleep 1
	debug "not ready; keep waiting..."
done
unset readiness

device_property=ro.hw.device
hwrev_property=ro.hw.revision
firmware_path=/system/etc/firmware

let dec_cfg_id_boot=0; dec_cfg_id_latest=0;

read_touch_property()
{
	property=""
	debug "retrieving property: [$touch_path/$1]"
	property=$(cat $touch_path/$1 2> /dev/null)
	debug "touch property [$1] is: [$property]"
	[ -z "$property" ] && return 1
	return 0
}

read_panel_property()
{
	property=""
	debug "retrieving panel property: [$panel_path/$1]"
	property=$(cat $panel_path/$1 2> /dev/null)
	debug "panel property [$1] is: [$property]"
	[ -z "$property" ] && return 1
	return 0
}

find_latest_config_id()
{
	local fw_mask=$1
	local skip_fields=$2
	local dec max z str_hex i

	str_cfg_id_latest=""

	debug "scanning dir for files matching [$fw_mask]"
	let dec=0; max=0;
	for file in $(ls $fw_mask 2>/dev/null);
	do
		z=$file
		i=0
		while [ ! $i -eq $skip_fields ];
		do
			z=${z#*-}
			i=$((i+1))
		done

		str_hex=${z%%-*};

		let dec=0x$str_hex
		if [ $dec -gt $max ];
		then
			let max=$dec; dec_cfg_id_latest=$dec;
			str_cfg_id_latest=$str_hex
		fi
	done

	[ -z "$str_cfg_id_latest" ] && return 1
	return 0
}

read_touch_property flashprog || error_and_leave 1
bl_mode=$property
debug "bl mode: $bl_mode"

read_touch_property productinfo || error_and_leave 1
touch_product_id=$property
if [ -z "$touch_product_id" ] || [ "$touch_product_id" == "0" ];
then
	debug "touch ic reports invalid product id"
	error_and_leave 3
fi
debug "touch product id: $touch_product_id"

read_touch_property buildid || error_and_leave 1
str_cfg_id_boot=${property#*-}
let dec_cfg_id_boot=0x$str_cfg_id_boot
debug "touch config id: $str_cfg_id_boot"

build_id_boot=${property%-*}
debug "touch build id: $build_id_boot"

typeset -l product_id
product_id=$(getprop $device_property 2> /dev/null)
[ -z "$product_id" ] && error_and_leave 2 $device_property
product_id=${product_id%-*}
product_id=${product_id%_*}
debug "product id: $product_id"

hwrev_id=$(getprop $hwrev_property 2> /dev/null)
[ -z "$hwrev_id" ] && notice "hw revision undefined"
debug "hw revision: $hwrev_id"

read_panel_property "panel_supplier"
supplier=$property
if [ -z "$supplier" ];
then
	debug "driver does not report panel supplier"
fi
debug "panel supplier: $supplier"

cd $firmware_path

find_best_match()
{
	local hw_mask=$1
	local panel_supplier=$2
	local skip_fields fw_mask

	while [ ! -z "$hw_mask" ]; do
		if [ "$hw_mask" == "-" ]; then
			hw_mask=""
		fi

		if [ ! -z $panel_supplier ];
		then
			skip_fields=3
			fw_mask="$touch_vendor-$panel_supplier-$touch_product_id-*-$product_id$hw_mask.*"
		else
			skip_fields=2
			fw_mask="$touch_vendor-$touch_product_id-*-$product_id$hw_mask.*"
		fi

		find_latest_config_id "$fw_mask" "$skip_fields" && break

		hw_mask=${hw_mask%?}
	done

	[ -z "$str_cfg_id_latest" ] && return 1

	if [ -z $panel_supplier ]; then
		firmware_file=$(ls $touch_vendor-$touch_product_id-$str_cfg_id_latest-*-$product_id$hw_mask.*)
	else
		firmware_file=$(ls $touch_vendor-$panel_supplier-$touch_product_id-$str_cfg_id_latest-*-$product_id$hw_mask.*)
	fi
	notice "Firmware file for upgrade $firmware_file"

	return 0
}

hw_mask="-$hwrev_id"
debug "hw_mask=$hw_mask"

match_not_found=1
if [ ! -z "$supplier" ];
then
	debug "search for best hw revision match with supplier"
	find_best_match "-$hwrev_id" "$supplier"
	match_not_found=$?
fi

if [ "$match_not_found" -ne "0" ];
then
	debug "search for best hw revision match without supplier"
	find_best_match "-$hwrev_id" || error_and_leave 3
fi

recovery=0
if [ "$bl_mode" == "1" ] || [ "$build_id_boot" == "0" ];
then
	recovery=1
	notice "Initiating touch firmware recovery"
	notice "  bl mode = $bl_mode"
	notice "  build id = $build_id_boot"
fi

if [ $dec_cfg_id_boot -ne $dec_cfg_id_latest ] || [ "$recovery" == "1" ];
then
	debug "forcing firmware upgrade"
	echo 1 > $touch_path/forcereflash
	debug "sending reflash command"
	echo $firmware_file > $touch_path/doreflash
	read_touch_property flashprog || error_and_leave 1
	bl_mode=$property

	[ "$bl_mode" == "1" ] && error_and_leave 4

	read_touch_property buildid || error_and_leave 1
	str_cfg_id_new=${property#*-}
	build_id_new=${property%-*}
	debug "firmware config ids: expected $str_cfg_id_latest, current $str_cfg_id_new"

	notice "Touch firmware config id at boot time $str_cfg_id_boot"
	notice "Touch firmware config id in the file $str_cfg_id_latest"
	notice "Touch firmware config id currently programmed $str_cfg_id_new"

	if [ "$str_cfg_id_latest" != "$str_cfg_id_new" ]; then
		notice "Touch firmware upgrade failed!!!"
		# build_id_new=1; uncomment this line to test
		if [ "$build_id_new" == "1" ]; then
			notice "Touch needs to go through recovery!!!"
			reboot_cnt=$(getprop $touch_status_prop 2>/dev/null)
			[ -z "$reboot_cnt" ] && reboot_cnt=0
			debug "current reboot counter [$reboot_cnt]"
			setprop $touch_status_prop "dead"
			notice "property [$touch_status_prop] set to [`getprop $touch_status_prop`]"
			if [ $((reboot_cnt)) -lt 2 ]; then
				notice "Touch is not working; rebooting..."
				debug "sleep 3s to allow touch-dead-sh service to run"
				sleep 3
				reboot
			else
				notice "Although touch is not working, no more reboots"
			fi
		fi
	else
		if [ -f $touch_path/f54/force_update ]; then
			notice "forcing F54 registers update"
			echo 1 > $touch_path/f54/force_update
		fi
		debug "set [$touch_status_prop] to [ready]"
		setprop $touch_status_prop "ready"
		notice "property [$touch_status_prop] set to [`getprop $touch_status_prop`]"
	fi
else
	notice "Touch firmware is up to date"
fi

unset device_property hwrev_property supplier
unset str_cfg_id_boot str_cfg_id_latest str_cfg_id_new
unset dec_cfg_id_boot dec_cfg_id_latest match_not_found
unset hwrev_id product_id touch_product_id scriptname
unset synaptics_link firmware_path touch_path
unset bl_mode dbg_on hw_mask firmware_file property

return 0
