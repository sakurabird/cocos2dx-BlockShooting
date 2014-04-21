//
//  BallSprite.h
//  BrockShooting
//
//  Created by sakura on 2014/04/17.
//
//

#ifndef __BrockShooting__BallSprite__
#define __BrockShooting__BallSprite__


#include "cocos2d.h"
#include "Config.h"

#define PNG_BALL "ball.png"

#define TAG_BALL 1002

#define BALL_REMAIN 5

class BallSprite : public cocos2d::CCSprite
{
protected:

public:

    BallSprite();
    virtual ~BallSprite();

    static BallSprite* createWithBallSize(float size);

    virtual bool initWithBallSize(float size);
};

#endif /* defined(__BrockShooting__BallSprite__) */
