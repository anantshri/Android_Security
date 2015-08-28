LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	expsur.cpp \
	help.cpp

LOCAL_C_INCLUDES += external/stlport/stlport/ \
	bionic \
	bionic/libc \
	bionic/libstdc++/include \
	external/jemalloc/include \
	system/core
LOCAL_SHARED_LIBRARIES := libutils libbinder libmedia libcutils libgui libui libdl


ifeq ($(TARGET_OS),linux)
	LOCAL_CFLAGS += -DXP_UNIX
	#LOCAL_SHARED_LIBRARIES += librt
endif

LOCAL_CFLAGS += -O0

LOCAL_MODULE:= exploit

include $(BUILD_EXECUTABLE)




include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	expsur.cpp \
	help.cpp

LOCAL_C_INCLUDES += external/stlport/stlport/ \
	bionic \
	bionic/libc \
	bionic/libstdc++/include \
	external/jemalloc/include \
	system/core
LOCAL_SHARED_LIBRARIES := libutils libbinder libmedia libcutils libgui libui libdl


ifeq ($(TARGET_OS),linux)
	LOCAL_CFLAGS += -DXP_UNIX
	#LOCAL_SHARED_LIBRARIES += librt
endif

LOCAL_CFLAGS += -O0

LOCAL_MODULE:= libexpsur

include $(BUILD_SHARED_LIBRARY)
