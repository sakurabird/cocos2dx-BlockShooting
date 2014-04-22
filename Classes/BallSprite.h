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

#define BALL_REMAIN 3

class BallSprite : public cocos2d::CCSprite
{
protected:

    //x方向の速度をあらわす変数＆アクセサ
    CC_SYNTHESIZE(int, m_vx, VelocityX);
    //y方向の速度をあらわす変数＆アクセサ
    CC_SYNTHESIZE(int, m_vy, VelocityY);

    void initVelocity();

public:

    BallSprite();
    virtual ~BallSprite();

    static BallSprite* createWithBallScale(float size);

    virtual bool initWithBallScale(float scale);
};

#endif /* defined(__BrockShooting__BallSprite__) */
