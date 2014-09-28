LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/Animations.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BallSprite.cpp \
                   ../../Classes/BarSprite.cpp \
                   ../../Classes/BlockSprite.cpp \
                   ../../Classes/CCPlaySE.cpp \
                   ../../Classes/GameClearPopup.cpp \
                   ../../Classes/GameOverScene.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/GHelper.cpp \
                   ../../Classes/HelpScene.cpp \
                   ../../Classes/LevelMenuItemSprite.cpp \
                   ../../Classes/LevelSelectScene.cpp \
                   ../../Classes/PopupLayer.cpp \
                   ../../Classes/SettingScene.cpp \
                   ../../Classes/TopScene.cpp \
                   ../../Classes/UserSettings.cpp \
                   ../../Classes/MyRenderer.cpp \
                   ../../Classes/Utils.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
