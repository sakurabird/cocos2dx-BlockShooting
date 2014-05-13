//
//  Animations.cpp
//  BrockShooting
//
//  Created by sakura on 2014/05/07.
//
//

#include "Animations.h"

USING_NS_CC;

CCFiniteTimeAction* Animation::topLavelAction()
{
    //Playボタンのアニメーション
    CCDelayTime* delay1 = CCDelayTime::create(2);
    CCScaleTo* scaleUp = CCScaleTo::create(0.3, 1.1);
    CCScaleTo* scaleDown = CCScaleTo::create(0.3, 1);
    CCSequence* seq = CCSequence::create(delay1, scaleUp, scaleDown, NULL);
    CCRepeatForever* repeat = CCRepeatForever::create(seq);

    return repeat;
}

CCFiniteTimeAction* Animation::gameClearAction()
{
    //Gameクリア時に表示するラベルのアニメーション
    CCDelayTime* delay1 = CCDelayTime::create(0.5);
    CCScaleTo* scaleUp1 = CCScaleTo::create(0.4, 4.5);
    CCDelayTime* delay2 = CCDelayTime::create(0.2);
    CCScaleTo* scaleDown1 = CCScaleTo::create(0.2, 2);
    CCScaleTo* scaleUp2 = CCScaleTo::create(0.1, 2.4);
    CCScaleTo* scaleDown2 = CCScaleTo::create(0.1, 2);

    CCDelayTime* delay3 = CCDelayTime::create(3.5);
    CCScaleTo* scaleDown3 = CCScaleTo::create(0.1, 0);

    return CCSequence::create(delay1, scaleUp1, delay2, scaleDown1, scaleUp2, scaleDown2, delay3, scaleDown3, NULL);
}


CCFiniteTimeAction* Animation::retryButtonAction()
{
    CCDelayTime* delay1 = CCDelayTime::create(1);
    CCScaleTo* scaleUp = CCScaleTo::create(0.1, 1.5);
    CCDelayTime* delay2 = CCDelayTime::create(0.5);
    CCScaleTo* scaleDown = CCScaleTo::create(0.3, 1);
    CCSequence* seq = CCSequence::create(delay1, scaleUp, delay2, scaleDown, NULL);
    CCRepeatForever* repeat = CCRepeatForever::create(seq);

    return repeat;
}
