LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := coreloader
LOCAL_SRC_FILES := coreloader.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
