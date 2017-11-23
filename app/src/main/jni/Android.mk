LOCAL_PATH:= $(call my-dir)

######################################################
include $(CLEAR_VARS)
LOCAL_MODULE := grideye_api_lv3
LOCAL_PREBUILT_LIBS := grideye/libgrideye_api_lv3.a
include $(BUILD_MULTI_PREBUILT)
######################################################

include $(CLEAR_VARS)

LOCAL_SRC_FILES := grideye/grideye_api_lv1.c \
                    grideye/grideye_api_lv2.c \
                    grideyeLib.c

LOCAL_C_INCLUDES := grideye/include
LOCAL_STATIC_LIBRARIES := grideye_api_lv3

LOCAL_MODULE := grideyeLib
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

include $(BUILD_SHARED_LIBRARY)
