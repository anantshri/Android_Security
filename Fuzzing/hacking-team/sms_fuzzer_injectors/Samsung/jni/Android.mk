# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)
CPP_CORE := $(abspath $(call my-dir)/../../CPPCore)


include $(CLEAR_VARS)

LOCAL_MODULE    := client
LOCAL_SRC_FILES := client.c
LOCAL_ARM_MODE := arm
LDFLAGS := -lutil
LOCAL_CFLAGS := -g
LOCAL_C_INCLUDES += headers
include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)

LOCAL_MODULE    := libinjector
LOCAL_SRC_FILES := injector.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -g
LOCAL_LDLIBS := libsec-ril.so
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := libinjector_rfs
LOCAL_SRC_FILES := injector_rfs.c
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -g
LOCAL_LDLIBS := libsec-ril.so
include $(BUILD_SHARED_LIBRARY)

