//
//  BarSprite.h
//  BrockShooting
//
//  Created by sakura on 2014/04/21.
//
//

#ifndef __BrockShooting__BarSprite__
#define __BrockShooting__BarSprite__

#include "cocos2d.h"
#include "Config.h"

class BarSprite : public cocos2d::CCSprite
{
protected:

public:

    BarSprite();
    virtual ~BarSprite();

    static BarSprite* createWithBarSize(float width, float height);

    virtual bool initWithBarSize(float width, float height);

    void setScaleLonger();

    void restoreScale();
};


#endif /* defined(__BrockShooting__BarSprite__) */
