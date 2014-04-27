//
//  CCPlaySE.h
//  効果音を再生するアクション
//  nyangame
//
//  Created by sakura on 2014/04/20.
//
//

#ifndef __nyangame__CCPlaySE__
#define __nyangame__CCPlaySE__

#include "cocos2d.h"

class CCPlaySE : public cocos2d::CCActionInstant
{
public:
    CCPlaySE(std::string sound);
    virtual ~CCPlaySE(){}

    virtual void update(float time);
    virtual CCFiniteTimeAction* reverse(void);
    virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);

public:
    static CCPlaySE* create(std::string sound);

protected:
    std::string m_sound;
};

#endif /* defined(__nyangame__CCPlaySE__) */
