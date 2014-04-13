//
//  AppMacros.h
//  BrockShooting
//
//  Created by sakura on 2014/04/13.
//
//

#ifndef BrockShooting_AppMacros_h
#define BrockShooting_AppMacros_h

#include "cocos2d.h"

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
} Resource;

static Resource sdResource = { cocos2d::CCSizeMake(384, 512), "s"};
static Resource hdResource = { cocos2d::CCSizeMake(768, 1025), "h"};

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(768, 1024);

#define LEVEL_FONT_SIZE (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / hdResource.size.width * 48)

#endif
