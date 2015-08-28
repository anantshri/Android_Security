mm
adb push $ANDROID_BUILD_TOP/out/target/product/hammerhead/symbols/system/bin/bbshell /data/local/tmp/bbshell
adb push busybox_unstripped /data/local/tmp/busybox_unstripped
if [ $# == 0 ]; then
#    adb shell su -c setenforce 0
    adb shell  /data/local/tmp/bbshell
else
    #adb shell su -c setenforce 0
    adb shell su -c gdbserver :1333 /data/local/tmp/bbshell tt &
    adb forward tcp:5039 tcp:5039
    adb forward tcp:1333 tcp:1333
    gdbclient bbshell :1333
fi
