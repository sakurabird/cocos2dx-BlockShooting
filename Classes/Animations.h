//
//  Animations.h
//  BrockShooting
//
//  Created by sakura on 2014/05/07.
//
//

#ifndef __BrockShooting__Animations__
#define __BrockShooting__Animations__


#include "cocos2d.h"

class Animations
{
public:
    // top画面
    static cocos2d::CCFiniteTimeAction* topMiniAnimation();
    static cocos2d::CCFiniteTimeAction* topLabelAction();

    // game画面
    static cocos2d::CCFiniteTimeAction* gameClearAction(cocos2d::CCObject* target,
                                                        cocos2d::SEL_CallFunc selector);
    static cocos2d::CCFiniteTimeAction* gameAddingScoreAction();
    static cocos2d::CCFiniteTimeAction* retryButtonAction();
    static cocos2d::CCFiniteTimeAction* getItemFallAction(cocos2d::CCSprite* block);
    static cocos2d::CCFiniteTimeAction* getItemLabelAction(cocos2d::CCNode* bar,
                                                           cocos2d::CCObject* target,
                                                           cocos2d::SEL_CallFunc selector);
    static cocos2d::CCFiniteTimeAction* winMiniAnimation();
    static cocos2d::CCFiniteTimeAction* gameoverMiniAnimation();
};

#endif /* defined(__BrockShooting__Animations__) */
