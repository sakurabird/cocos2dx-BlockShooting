//
//  LevelSelectScene.h
//  BrockShooting
//
//  Created by sakura on 2014/05/10.
//
//

#ifndef __BrockShooting__LevelSelectScene__
#define __BrockShooting__LevelSelectScene__

#include "cocos2d.h"

class LevelSelectScene : public cocos2d::CCLayerGradient
{
public:
	LevelSelectScene();
	~LevelSelectScene();

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(LevelSelectScene);

    //Androidのバックキー処理
    virtual void keyBackClicked();


private:

    void makeLabel();

    void onTapBackButton();

    void onTapLevel(CCObject *sender);
};


#endif /* defined(__BrockShooting__LevelSelectScene__) */
