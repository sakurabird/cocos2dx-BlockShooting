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

class LevelSelectScene : public cocos2d::CCLayerColor
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

	void onLevelSelected(int level);
    void onTapBackButton();
    void tapEasyButton();
    void tapNormalButton();
    void tapHardButton();
};


#endif /* defined(__BrockShooting__LevelSelectScene__) */
