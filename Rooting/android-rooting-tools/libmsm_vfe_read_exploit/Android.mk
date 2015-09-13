LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
  msm_vfe_read.c

LOCAL_MODULE := libmsm_vfe_read_exploit
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)
