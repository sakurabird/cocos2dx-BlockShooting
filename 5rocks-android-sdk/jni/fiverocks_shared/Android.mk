LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE            := fiverocks_shared
LOCAL_SRC_FILES         := $(TARGET_ARCH_ABI)/libfiverocks.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_EXPORT_CFLAGS     := -DFIVEROCKS_SHARED=1
include $(PREBUILT_SHARED_LIBRARY)
