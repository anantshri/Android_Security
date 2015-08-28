LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := expsys.cpp help.cpp
LOCAL_MODULE := libexpsys
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES :=  liblog libdl libselinux libui libgui libbinder libutils libcutils libstlport
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include \
					system/core
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
	external/stlport/stlport/ \
	bionic \
	bionic/libc \
	bionic/libstdc++/include \
	external/jemalloc/include \
	system/core
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
LOCAL_CFLAGS := -Werror
LOCAL_CFLAGS += -O0
include $(BUILD_SHARED_LIBRARY)

