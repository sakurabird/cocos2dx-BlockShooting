//
//  UserSettings.cpp
//  BrockShooting
//
//  Created by sakura on 2014/05/07.
//
//

#include "UserSettings.h"
#include "Config.h"

USING_NS_CC;

const char* musicKey = "key_music";
const char* soundEffectKey = "key_soundeffect";
const char* levelKey = "key_level";

bool UserSettings::getMusicSetting()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    return userDefault->getBoolForKey(musicKey, true);
}

void UserSettings::setMusicSetting(bool onoff)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setBoolForKey(musicKey, onoff);
    userDefault->flush();
}

bool UserSettings::getSESetting()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    return userDefault->getBoolForKey(soundEffectKey, true);
}

void UserSettings::setSESetting(bool onoff)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setBoolForKey(soundEffectKey, onoff);
    userDefault->flush();
}

int UserSettings::getLevelSetting()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    return userDefault->getBoolForKey(levelKey, LEVEL_EASY);
}

void UserSettings::setLevelSetting(int level)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setIntegerForKey(levelKey, level);
    userDefault->flush();
}