//
//  UserSettings.h
//  BrockShooting
//
//  Created by sakura on 2014/05/07.
//
//

#ifndef __BrockShooting__UserSettings__
#define __BrockShooting__UserSettings__


#include "cocos2d.h"

USING_NS_CC;

class UserSettings
{
public:

    static bool getMusicSetting();
    static void setMusicSetting(bool onoff);

    static bool getSESetting();
    static void setSESetting(bool onoff);
};

#endif /* defined(__BrockShooting__UserSettings__) */
