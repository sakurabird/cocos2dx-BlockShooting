#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

#define TAG_PLAYER 1001
#define TAG_BALL 1002

using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayerColor
{
public:
	HelloWorld();
	~HelloWorld();

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(HelloWorld);

	void registerWithTouchDispatcher();

	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);


protected:

    

private:
    CCSize _visibleSize;
    CCPoint _origin;
	CCArray *_targets;
	CCArray *_projectiles;
    
	int _projectilesDestroyed;
    int _projectilesRemain;
	void addTarget();
	void spriteMoveFinished(cocos2d::CCNode* sender);
	void gameLogic(float dt);
	void updateGame(float dt);
    void makeBlock();
    void makePlayer();
    void pushBall(CCSet* touches);
    void gameOver(CCSprite *sprite);
};

#endif  // __HELLOWORLD_SCENE_H__