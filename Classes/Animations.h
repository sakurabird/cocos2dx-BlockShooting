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

class Animation
{
public:
    // top画面
    static cocos2d::CCFiniteTimeAction* topLabelAction();

    // game画面
    static cocos2d::CCFiniteTimeAction* gameClearAction();
    static cocos2d::CCFiniteTimeAction* retryButtonAction();
    static cocos2d::CCFiniteTimeAction* getItemFallAction(cocos2d::CCSprite* block);
    static cocos2d::CCFiniteTimeAction* getItemLabelAction(cocos2d::CCSprite* bar,
                                                           cocos2d::CCObject* target,
                                                           cocos2d::SEL_CallFunc selector);
};

#endif /* defined(__BrockShooting__Animations__) */
