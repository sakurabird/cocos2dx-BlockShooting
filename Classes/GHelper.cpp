//
//  GHelper.cpp
//  HelloCpp
//
//  Created by 椋梨 健太郎 on 2013/07/09.
//
//

#include "GHelper.h"
USING_NS_CC;

//static変数
CCSpriteFrameCache* GHelper::cache;
CCSize              GHelper::visibleSize;
CCPoint             GHelper::origin;
cc_timeval          GHelper::beginTime;
long                GHelper::beginSec;


//初期化
void GHelper::init(){
    cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    visibleSize = cocos2d::CCDirector::sharedDirector()->getVisibleSize();
    origin = cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();

    CCTime::gettimeofdayCocos2d(&beginTime, NULL);

    beginSec = beginTime.tv_sec;
    CCLOG("GHelper::init sec =  %ld ", beginSec);
}

// テクスチャーの追加
void GHelper::addFrameCache(const char *file_name){
    cache->addSpriteFramesWithFile(file_name);
}

void GHelper::removeFrameCashe(const char *file_name)
{
    cache->removeSpriteFrameByName(file_name);
}

//720x1280 横
CCRect GHelper::CCRectMake720_1080(float x, float y, float w, float h){

    CCPoint point = CCPointMake720_1080(x,y);
    return CCRectMake(point.x, point.y-h, w, h);
}


CCPoint GHelper::CCPointMake720_1080(float x, float y){
    float x1,x2, y1,y2;

    x1 = x-720.0f/2.0f;
    y1 = y-1280.0f/2.0f;

    x2 = x1+visibleSize.width/2.0f;
    y2 = visibleSize.height/2.0 -y1;

    return CCPointMake(x2, y2);
}


CCPoint GHelper::convI720toCC(int ix, int iy){
    float x1,x2, y1,y2;

    x1 = ix-720.0f/2.0f;
    y1 = iy-1280.0f/2.0f;

    x2 = x1+visibleSize.width/2.0f;
    y2 = visibleSize.height/2.0 -y1;

    return ccp(x2, y2);
}

CCSprite* GHelper::createGSprite720_1280(int ix, int iy, const char *file_name){
    float x1,x2, y1,y2;

    //720x1280を中心座標へ
    x1 = ix-720.0f/2.0f;
    y1 = iy-1280.0f/2.0f;

    x2 = x1+visibleSize.width/2.0f;
    y2 = visibleSize.height/2.0 -y1;

    /***
     CCLOG("GHelper::createGSprite720_1280 file_name = %s ", file_name);
     CCLOG("GHelper::createGSprite720_1280 ix = %d iy = %d ", ix, iy);
     CCLOG("GHelper::createGSprite720_1280 x1 = %f y1 = %f ", x1, y1);
     CCLOG("GHelper::createGSprite720_1280 x2 = %f y2 = %f ", x2, y2);
     ***/

    CCSprite* sprite = cocos2d::CCSprite::createWithSpriteFrameName(file_name);    //createWithSpriteFrameNameを使用

    sprite->setPosition( ccp(x2, y2) );
    return sprite;
}

CCSprite* GHelper::createGSprite720_1280LT(int ix, int iy, const char *file_name){
    float x1,x2, y1,y2;


    CCSprite* sprite = cocos2d::CCSprite::createWithSpriteFrameName(file_name);    //createWithSpriteFrameNameを使用

    //720x1280を中心座標へ
    x1 = (ix+sprite->getContentSize().width/2.0f)  -720.0f/2.0f;    //ixは左上から中心座標に
    y1 = (iy+sprite->getContentSize().height/2.0f) -1280.0f/2.0f;   //iyは左上から中心座標に

    x2 = x1+visibleSize.width/2.0f;
    y2 = visibleSize.height/2.0 -y1;

    /***
     CCLOG("GHelper::createGSprite720_1280LT file_name = %s ", file_name);
     CCLOG("GHelper::createGSprite720_1280LT ix = %d iy = %d ", ix, iy);
     CCLOG("GHelper::createGSprite720_1280LT x1 = %f y1 = %f ", x1, y1);
     CCLOG("GHelper::createGSprite720_1280LT x2 = %f y2 = %f ", x2, y2);
     ***/



    sprite->setPosition( ccp(x2, y2) );
    return sprite;
}


//1280x720 横
CCPoint GHelper::convI1280toCC(int ix, int iy){
    float x1,x2, y1,y2;

    x1 = ix-1280.0f/2.0f;
    y1 = iy-720.0f/2.0f;

    x2 = x1+visibleSize.width/2.0f;
    y2 = visibleSize.height/2.0 -y1;

    return ccp(x2, y2);
}

CCSprite* GHelper::createGSprite1280_720(int ix, int iy, const char *file_name){
    float x1,x2, y1,y2;

    //中心座標へ
    x1 = ix-1280.0f/2.0f;
    y1 = iy-720.0f/2.0f;

    x2 = x1+visibleSize.width/2.0f;
    y2 = visibleSize.height/2.0 -y1;

    /***
     CCLOG("GHelper::createGSprite720_1280 file_name = %s ", file_name);
     CCLOG("GHelper::createGSprite720_1280 ix = %d iy = %d ", ix, iy);
     CCLOG("GHelper::createGSprite720_1280 x1 = %f y1 = %f ", x1, y1);
     CCLOG("GHelper::createGSprite720_1280 x2 = %f y2 = %f ", x2, y2);
     ***/

    CCSprite* sprite = cocos2d::CCSprite::createWithSpriteFrameName(file_name);    //createWithSpriteFrameNameを使用

    sprite->setPosition( ccp(x2, y2) );
    return sprite;
}

//


CCSprite* GHelper::createGSprite640_960(int ix, int iy, const char *file_name){
    float x1,x2, y1,y2;

    //640x960を中心座標へ
    x1 = ix-640.0f/2.0f;
    y1 = iy-960.0f/2.0f;

    x2 = x1+visibleSize.width/2.0f;
    y2 = visibleSize.height/2.0 -y1;

    /***
     CCLOG("GHelper::createGSprite640_960 file_name = %s ", file_name);
     CCLOG("GHelper::createGSprite640_960 ix = %d iy = %d ", ix, iy);
     CCLOG("GHelper::createGSprite640_960 x1 = %f y1 = %f ", x1, y1);
     CCLOG("GHelper::createGSprite640_960 x2 = %f y2 = %f ", x2, y2);
     ***/

    CCSprite* sprite = cocos2d::CCSprite::createWithSpriteFrameName(file_name);    //createWithSpriteFrameNameを使用

    sprite->setPosition( ccp(x2, y2) );
    return sprite;
}

//現在の時刻（秒）を取得
long GHelper::getNowSec()
{
    cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return tv.tv_sec;
}

//現在の時刻（秒）を取得
long GHelper::getNowMSec()
{
    cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return tv.tv_sec + tv.tv_usec;
}

//現在の時間（秒）を取得 経過時間
double GHelper::getSecond()
{
    cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return (tv.tv_sec - beginTime.tv_sec) + (tv.tv_usec - beginTime.tv_usec) * 1e-6;
}

//現在の時間（秒）を取得 経過時間
int GHelper::getSecondINT()
{
    cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);

    int ret = tv.tv_sec - beginTime.tv_sec;
    return ret;
}

//現在の時間（ミリ秒）を取得　経過時間
double GHelper::getMSec()
{
    cc_timeval tv;
    CCTime::gettimeofdayCocos2d(&tv, NULL);
    return ((tv.tv_sec - beginTime.tv_sec) + (tv.tv_usec - beginTime.tv_usec) * 1e-6) * 1000;
}

//フェードによるシーン切替
void GHelper::transitionFade(float duration, CCScene *scene, const ccColor3B &color)
{
    CCTransitionScene* transition = CCTransitionFade::create(duration, scene, color);
    CCDirector::sharedDirector()->replaceScene( transition );
}

void GHelper::transitionFade(CCScene *scene)
{
    transitionFade(0.5f, scene, ccBLACK);
}

//CCAnimateの生成　               (ループ数, 時間, 画像名...可変個数, 最後はNULL)
CCAnimate* GHelper::createAnimate(int loops, float delay_per_unit, ...)
{
    va_list args;
    va_start(args, delay_per_unit);
    char* frameName = va_arg(args , char*);
    CCAnimation* animation = CCAnimation::create();
    while (frameName != NULL) {
        CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        animation->addSpriteFrame(pFrame);
        frameName = va_arg(args , char*);
	}
	va_end(args);
    animation->setLoops(loops);
    animation->setDelayPerUnit(delay_per_unit);
    animation->setRestoreOriginalFrame(true);
    return CCAnimate::create(animation);
}

//スプライトの画像を変更       (スプライト, 画像名)
void GHelper::changeSpriteFrame(cocos2d::CCSprite *sprite, const char *frame_name)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frame_name);
    sprite->setTexture(pFrame->getTexture());
    sprite->setTextureRect(pFrame->getRect());
}

