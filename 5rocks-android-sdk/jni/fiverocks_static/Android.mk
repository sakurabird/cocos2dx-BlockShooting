LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE            := fiverocks_static
LOCAL_SRC_FILES         := $(TARGET_ARCH_ABI)/libfiverocks.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_EXPORT_CFLAGS     := -DFIVEROCKS_STATIC=1
include $(PREBUILT_STATIC_LIBRARY)
