LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	media.cpp \
	help.cpp \
	shellcode.cpp \
	runsc.cpp \
    asm.S \
	../bbshell/bbshell.cpp

LOCAL_C_INCLUDES += external/stlport/stlport/ \
	bionic \
	bionic/libc \
	bionic/libstdc++/include \
	external/jemalloc/include \
	system/core
LOCAL_SHARED_LIBRARIES := libutils libbinder libmedia libcutils libgui libui libdl


LOCAL_STATIC_LIBRARIES := \
        libstlport_static 
ifeq ($(TARGET_OS),linux)
	LOCAL_CFLAGS += -DXP_UNIX
	#LOCAL_SHARED_LIBRARIES += librt
endif

#LOCAL_CFLAGS += -O0

LOCAL_MODULE:= exploitmedia

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
	LOCAL_SRC_FILES := sotest.cpp \
					../bbshell/bbshell.cpp
	LOCAL_MODULE := testso
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

