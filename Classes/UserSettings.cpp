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
const char* highScoreKey = "key_highScore";
const char* scoreKey = "key_score";

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
    return userDefault->getIntegerForKey(levelKey, LEVEL_EASY);
}

void UserSettings::setLevelSetting(int level)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setIntegerForKey(levelKey, level);
    userDefault->flush();
}

int UserSettings::getHighScore()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    return userDefault->getIntegerForKey(highScoreKey, 0);
}

void UserSettings::setHighScore(int score)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setIntegerForKey(highScoreKey, score);
    userDefault->flush();
}


int UserSettings::getScore()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    return userDefault->getIntegerForKey(scoreKey, 0);
}

void UserSettings::setScore(int score)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setIntegerForKey(scoreKey, score);
    userDefault->flush();
}