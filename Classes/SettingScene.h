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

class SettingScene : public cocos2d::CCLayerGradient
{
public:
	SettingScene();
	~SettingScene();
    
	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(SettingScene);

    //Androidのバックキー処理
    virtual void keyBackClicked();

private:

    void makeLabels();

    void onTapBackButton();

	void onTapMusicOnOFFButton();

    void onTapSEOnOFFButton();

    void onTapClearUserData();

    void popupCallback(CCNode* sender);
};


#endif /* defined(__BrockShooting__SettingScene__) */
