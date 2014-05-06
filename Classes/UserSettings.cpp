//
//  UserSettings.cpp
//  BrockShooting
//
//  Created by sakura on 2014/05/07.
//
//

#include "UserSettings.h"

const char* musicKey = "music";
const char* soundEffectKey = "soundeffect";

bool UserSettings::getMusicSetting()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    return userDefault->getBoolForKey(musicKey, true);
}

bool UserSettings::getSESetting()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    return userDefault->getBoolForKey(soundEffectKey, true);
}

void UserSettings::setMusicSetting(bool onoff)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setBoolForKey(musicKey, onoff);
    userDefault->flush();
}

void UserSettings::setSESetting(bool onoff)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setBoolForKey(soundEffectKey, onoff);
    userDefault->flush();
}
