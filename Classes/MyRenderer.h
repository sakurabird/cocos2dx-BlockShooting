//
//  MyRenderer.h
//  HelloCpp
//
//  Created by 椋梨 健太郎 on 2013/07/11.
//
//

#ifndef HelloCpp_MyRenderer_h
#define HelloCpp_MyRenderer_h

#include "cocos2d.h"
USING_NS_CC;

#define callback_selector(_SELECTOR) (SEL_callbackHandler)(&_SELECTOR)
//typedef void (cocos2d::CCObject::*SEL_callbackHandler)(const char *json);
typedef void (cocos2d::CCObject::*SEL_callbackHandler)(int flag);

//63 7page
//#define MAX_PAGE  8  //9x6=54 9x7  =63 9x8=72
//#define MAX_PAGE2 12  //6x9=54 6x11 = 66 3つ出ないようにする 6x12 = 72

#define MAX_PAGE  3  //9x3=27 9x7
#define MAX_PAGE2 5  //6x5=30 

class MyRenderer
{
public:
	static int endFlag;

	static void disp_gameFeat();
	static void showAd(int n);

    static void init();
    static void init2();
    static void init_lang();
    static void end();
    static void replaceScene( cocos2d::CCScene *scene );
    
    static void saveEventNum(int n);
    static void saveCollectNum(int n);
    static void send_gga_lavel(const char* messsage);
    
    static void playEffect(const char* pszFilePath);

    static cocos2d::CCScene *pGSceneSplash;
    static cocos2d::CCScene *pGSceneMainMenu;
    
    static cocos2d::CCSpriteFrameCache* cache;

    static void regist(CCObject* listene, SEL_callbackHandler selector);
    static void activate(int flag);
    static CCObject*       m_pListener;
    static SEL_callbackHandler    m_pfnSelector;

};

#endif
