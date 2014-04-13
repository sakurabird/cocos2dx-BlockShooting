
#ifndef _GAME_OVER_SCENE_H_
#define _GAME_OVER_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;


class GameOverLayer : public cocos2d::CCLayerColor
{
public:
    GameOverLayer():_label(NULL),_label2(NULL) {};
    virtual ~GameOverLayer();
    bool init();
    CREATE_FUNC(GameOverLayer);

    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label2, Label2);

    void setResult(int score);

protected:

    void gameOverDone();

	// a selector callback
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);
};



class GameOverScene : public cocos2d::CCScene
{
public:
    GameOverScene():_layer(NULL) {};
    ~GameOverScene();
    bool init();
    CREATE_FUNC(GameOverScene);
  
    CC_SYNTHESIZE_READONLY(GameOverLayer*, _layer, Layer);
};

#endif // _GAME_OVER_SCENE_H_
