
#ifndef _GAME_OVER_SCENE_H_
#define _GAME_OVER_SCENE_H_

#include "cocos2d.h"

class GameOverScene : public cocos2d::CCLayerGradient
{

public:
	GameOverScene();
	~GameOverScene();

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(GameOverScene);

    virtual void keyBackClicked();

private:

    void makeLabel();

	void onTapOKButton();
};

#endif // _GAME_OVER_SCENE_H_
