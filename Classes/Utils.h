//
//  Utils.h
//  BrockShooting
//
//  Created by sakura on 2014/05/10.
//
//

#ifndef __BrockShooting__Utils__
#define __BrockShooting__Utils__


#include "cocos2d.h"

class Utils
{
public:
    static void setGirlBackground(cocos2d::CCLayer* target);
    static void setGirlFilter(cocos2d::CCLayer* target);

    static void createBackButton(cocos2d::CCLayer* target, cocos2d::SEL_MenuHandler selector,
    		float rate_x, float rate_y);

    static void quit();
};


#endif /* defined(__BrockShooting__Utils__) */
