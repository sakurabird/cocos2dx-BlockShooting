//
//  GirlImageScene.h
//  BrockShooting
//
//  Created by sakura on 2014/10/07.
//
//

#ifndef __BrockShooting__GirlImageScene__
#define __BrockShooting__GirlImageScene__


#include "cocos2d.h"

class GirlImageScene : public cocos2d::CCLayer
{
public:
	GirlImageScene();
	~GirlImageScene();

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(GirlImageScene);

    //Androidのバックキー処理
    virtual void keyBackClicked();

private:
    void onTapBackButton();
};

#endif /* defined(__BrockShooting__GirlImageScene__) */
