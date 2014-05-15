//
//  TopScene.h
//  BrocjShooting
//
//  Created by sakura on 2014/02/19.
//
//

#ifndef __BrockShooting__TopScene__
#define __BrockShooting__TopScene__


#include "cocos2d.h"

class TopScene : public cocos2d::CCLayerColor
{
public:
	TopScene();
	~TopScene();

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(TopScene);

	virtual void menuCloseCallback(cocos2d::CCObject* pSender);

    //Androidのバックキー処理
    virtual void keyBackClicked();


private:

    void makeLabel();

	void onTapStartButton();

    void onTapSettingButton();

    void onTapHelpButton();
};

#endif /* defined(__BrockShooting__TopScene__) */
