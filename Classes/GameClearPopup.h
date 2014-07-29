//
//  GameClearPopup.h
//  BrockShooting
//
//  Created by sakura on 2014/07/24.
//
//

#ifndef __BrockShooting__GameClearPopup__
#define __BrockShooting__GameClearPopup__

#include "cocos2d.h"

class GameClearPopup: public cocos2d::CCLayer{
    
public:
    GameClearPopup();
    ~GameClearPopup();

    virtual bool init();
    CREATE_FUNC(GameClearPopup);
//
//    virtual void registerWithTouchDispatcher(void);
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

//    virtual void onEnter();
//    static GameClearPopup
//    * create(const char* backgroundImage);

    virtual void keyBackClicked();

private:

    void makeLabels();

    void onTapNextStage();

	void onTapAgain();

	void onTapMenu();

	void onTapQuit();


//    CCObject* m_callbackListener;
//    cocos2d::SEL_CallFuncN m_callback;
//
//    CC_SYNTHESIZE_RETAIN(cocos2d::CCMenu*, m__pMenu, MenuButton);
//    CC_SYNTHESIZE_RETAIN(cocos2d::CCSprite*, m__sfBackGround, SpriteBackGround);
//    CC_SYNTHESIZE_RETAIN(cocos2d::CCLabelTTF*, m__ltTitle, LabelTitle);
//    CC_SYNTHESIZE_RETAIN(cocos2d::CCLabelTTF*, m__ltContentText, LabelContentText);

};


#endif /* defined(__BrockShooting__GameClearPopup__) */
