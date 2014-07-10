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

int gLevelState[2][16];

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
    return userDefault->getIntegerForKey(levelKey, 0);
}

void UserSettings::setLevelSetting(int level)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setIntegerForKey(levelKey, level);
    userDefault->flush();
}

// TODO
void UserSettings::loadLevelState()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
//    CCLOG("UserSettings::loadLevelState");

    char buff[256];
    for(int i = 0; i < 16; i++){
        sprintf(buff,"k_panel16_0_%02d",i);
        gLevelState[0][i] = userDefault->getIntegerForKey(buff, 0);    //load
//        CCLOG("gLevelState[0][%d]=%d", i, gLevelState[0][i]);

        sprintf(buff,"k_panel16_1_%02d",i);
        gLevelState[1][i] = userDefault->getIntegerForKey(buff, 0);    //load
//        CCLOG("state[1][%d]=%d", i, gLevelState[1][i]);
    }
    gLevelState[0][0] = 1;
}

void UserSettings::setLevelState()
{
    CCLOG("UserSettings::setLevelState");
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();

    char buff[256];
    for(int i = 0; i < 16; i++){
        sprintf(buff,"k_panel16_0_%02d",i);
        userDefault->setIntegerForKey(buff, gLevelState[0][i]);   //save
        CCLOG("gLevelState[%d]=%d", i, gLevelState[0][i]);

        sprintf(buff,"k_panel16_1_%02d",i);
        userDefault->setIntegerForKey(buff, gLevelState[1][i]);   //save
        CCLOG("gLevelState[%d]=%d", i, gLevelState[1][i]);
    }
    userDefault->flush();
}
// TODO

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