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


class UserSettings
{
public:

    static bool getMusicSetting();
    static void setMusicSetting(bool onoff);

    static bool getSESetting();
    static void setSESetting(bool onoff);

    static void loadLevelState();
    static void setLevelState();

    static int getLevelSetting();
    static void setLevelSetting(int level);

    static int getHighScore();
    static void setHighScore(int score);

    static int getScore();
    static void setScore(int score);
};

#endif /* defined(__BrockShooting__UserSettings__) */
