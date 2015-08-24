LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
  ping_unhash.c \
  exploit.c

LOCAL_MODULE := libping_unhash_exploit
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)
