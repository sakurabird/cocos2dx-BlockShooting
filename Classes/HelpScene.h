//
//  HelpScene.h
//  BrockShooting
//
//  Created by sakura on 2014/05/15.
//
//

#ifndef __BrockShooting__HelpScene__
#define __BrockShooting__HelpScene__


#include "cocos2d.h"

class HelpScene : public cocos2d::CCLayerColor
{
public:
	HelpScene();
	~HelpScene();

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(HelpScene);

    cocos2d::CCSize _visibleSize;

    //Androidのバックキー処理
    virtual void keyBackClicked();

private:

    void makeLabels();

    void makeBackButton();

    void onTapBackButton();
};

#endif /* defined(__BrockShooting__HelpScene__) */