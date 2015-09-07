#!/bin/bash
#####
#####
# Stuff that still needs to be integrated
# manual usage of application
# network data monitoring
# automated passive scan on network data
# adb backup after usage for some time. - Done with a nifty touch event to autostart the backup
# automated analysis of data folder content 
# mobilizer and grep script results should be combined into one place prefereably in a python script.
# repurpose intent fuzzer to work on all aspects.
# identify permission map
# identify if extra permissions are requested.
# cobradroid integration
# inotify integration
# introspy integration
# https://github.com/AndroidTamer/jd-core-java

# Screenshot video tour embedded 
# genymotion vm should have arm translation - One VM prepared nexus4_with_gapps
# extract data from snapshot.
# restore genymotion to a snapshot position after a app analysis
# additionally playstore configured and google account running on it. but this only after f-droid analysis is done
# take memory dump to analyze in memory strings
# identify list of activities and run each activity and take a screenshot.
# all activity names
# aapt dump badging dropbox.apk  | grep "activity" | awk '{print $2}' | sed s/name=//g | sed s/\'//g
# 	screenshot, run main activity, screenshot, run activities screenshot
# draft set added lets see how it fares with the real life
# in extracted content : see if there is a plaintext password type column in xml or do a sqlite dump and do a grep on it to see if text like password or secret is found.
# in adb backup do a backup extraction.
# look for sensitive information in the extracted backup
# Android virus check @ virustotal via sha1 sum


if [[ "$OSTYPE" == "darwin"* ]]
then
	readlink=greadlink
else
	readlink=readlink
fi
apk_path=`$readlink -f $1`
apk=$(basename $apk_path)
echo $apk_path
CWD=`pwd`
Base_assessment_dir='/Users/anant/Work/Mobile_projects/Android/APK_assessment/'
new_dir=$(date +"%Y%m%d")
Assessment_dir=$Base_assessment_dir$new_dir
mkdir -p $Assessment_dir
pkg_name=`aapt dump badging $apk_path | grep package | awk '{print $2}' | sed s/name=//g | sed s/\'//g`
cd $Assessment_dir
mkdir $Assessment_dir/results
cp $apk_path $Assessment_dir/$apk
echo "decompile each apk"
apk2java $apk
cd $apk"_src"
echo "Place unziped content inside unzip just in case its needed"

echo "Doing a Jadx Decompilation just in case"
jadx -d jadx classes.dex
echo "running static analysis : Droidscan"
droidscan.sh $Assessment_dir/$apk"_src"
echo "running static analysis : Androwarn"
androwarn -i $Assessment_dir/$apk -v 3 -r html -n  -d | tee $Assessment_dir/results/"androwarn_"$apk".txt"
cd $Assessment_dir
#$vmstat=`VBoxManage showvminfo "Galaxy_note_4.1.1" | grep "State" | grep "running"`
#echo $vmstat
# if [ $vmstat -eq 0 ]
# then
echo "Starting Drozer service"
adb wait-for-devices
echo "start drozer agent"
adb shell am startservice -n com.mwr.dz/.services.ServerService -c com.mwr.dz.START_EMBEDDED
adb forward tcp:31415 tcp:31415
echo "install apk to genymotion"
adb install $apk
echo "monitor logcat for time being"
adb logcat > $Assessment_dir/results/"logcat_dump_"$apk".txt" &
#read -p "Press enter to continue after you have done some processing work on it."
echo "running monkey for some action"
adb shell monkey -p $pkg_name -v 1000
adb shell am kill $pkg_name
echo "running drozer checks"
drozer_check.sh $pkg_name | tee  $Assessment_dir/results/"drozer_checks_"$apk".txt"
echo "creating an adb backup"
mkdir -p $Assessment_dir/$apk"_src"/adbbackup/
cd $Assessment_dir/$apk"_src"/adbbackup/
adb backup -apk -shared -f $Assessment_dir/$apk"_src"/adbbackup/backup.ab $pkg_name &
#adb nexus4 shell input swipe 400 1100 300 300 100
# these two touch events need to be customized for specific scenario
adb shell input swipe 360 890 640 940 100
adb shell input tap 465 1180 
fg
echo "Adb Backup done"
echo "making a data dump"
mkdir -p $Assessment_dir/$apk"_src"/data_folder_content/
cd $Assessment_dir/$apk"_src"/data_folder_content/
adb pull /data/data/$pkg_name/ ./ 
cd $Assessment_dir
echo "attempting activity screenshoting"
mkdir -p $Assessment_dir/$apk"_src"/activity_screenshots/
apk_activity_list=`aapt dump badging $apk  | grep "activity" | awk '{print $2}' | sed s/name=//g | sed s/\'//g`
while read -r x;
do
	echo $x
	am start -n $pkg_name/.$x
	adb shell screencap -p | perl -pe 's/\x0D\x0A/\x0A/g' > $Assessment_dir/$apk"_src"/activity_screenshots/$x_$pkg_name_screen.png
done<<< "$apk_activity_list"
echo "done "
echo "Uninstalling apk"
adb uninstall $pkg_name
killall adb