//
//  SettingScene.h
//  BrockShooting
//
//  Created by sakura on 2014/04/27.
//
//

#ifndef __BrockShooting__SettingScene__
#define __BrockShooting__SettingScene__

#include "cocos2d.h"

USING_NS_CC;

class SettingScene : public cocos2d::CCLayerColor
{
public:
	SettingScene();
	~SettingScene();
    
	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(SettingScene);


private:

    void makeLabel();

	void tapReturnButton();
};


#endif /* defined(__BrockShooting__SettingScene__) */
