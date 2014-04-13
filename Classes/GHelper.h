//
//  GHelper.h
//  HelloCpp
//
//  Created by 椋梨 健太郎 on 2013/07/09.
//
//

#ifndef HelloCpp_GHelper_h
#define HelloCpp_GHelper_h

#include "cocos2d.h"


class GHelper {

public:
    static void                 init();
    static void                 addFrameCache(const char *file_name);
    static void                 removeFrameCashe(const char *file_name);

	static cocos2d::CCSprite*	createGSprite720_1280(int ix, int iy, const char *file_name);
    static cocos2d::CCSprite*	createGSprite720_1280LT(int ix, int iy, const char *file_name);

    static cocos2d::CCSprite*	createGSprite1280_720(int ix, int iy, const char *file_name);

    static cocos2d::CCSprite*	createGSprite640_960(int ix, int iy, const char *file_name);

    static cocos2d::CCPoint     convI720toCC(int ix, int iy);
    static cocos2d::CCPoint     convI1280toCC(int ix, int iy);

    static cocos2d::CCRect      CCRectMake720_1080(float x, float y, float w, float h);
    static cocos2d::CCPoint     CCPointMake720_1080(float x, float y);

    static double               getSecond();    //起動時からの経過時間
    static double               getMSec();      //起動時からの経過時間
    static long                 getNowSec();    //絶対時間、単位は秒のみ
    static long                 getNowMSec();
    static int 					getSecondINT();	//起動時からの経過時間、戻り値はint

    static void                 transitionFade(float duration, cocos2d::CCScene *scene, const cocos2d::ccColor3B& color);
    static void                 transitionFade(cocos2d::CCScene *scene);

    static cocos2d::CCAnimate*  createAnimate(int loops, float delay_per_unit, ...);

    static void                 changeSpriteFrame(cocos2d::CCSprite *sprite, const char *frame_name);

public:
    static cocos2d::CCSpriteFrameCache* cache;
    static cocos2d::CCSize visibleSize;
    static cocos2d::CCPoint origin;
    static cocos2d::cc_timeval  beginTime;
    static long beginSec;

};


#endif