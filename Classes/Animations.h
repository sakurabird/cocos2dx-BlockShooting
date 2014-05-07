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

USING_NS_CC;

class Animation
{
public:
    // top画面
    static CCFiniteTimeAction* topLavelAction();

    // game画面
    static CCFiniteTimeAction* gameClearAction();


//    static CCFiniteTimeAction* gameOverAnimation(CCObject* target, SEL_CallFunc selector);
};

#endif /* defined(__BrockShooting__Animations__) */
