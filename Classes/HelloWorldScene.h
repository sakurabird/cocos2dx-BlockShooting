#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

#define TAG_BAR 1001
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
    // ブロックの列数・行数
    int _column = 12;
    int _row = 10;

private:
    CCSize _visibleSize;
    CCPoint _origin;
	CCArray *_targets;

    // 速度
    double _vx = 10;
    double _vy = 10;

	int _blocksDestroyed;
    int _ballsRemain;
    
	void addTarget();
	void spriteMoveFinished(cocos2d::CCNode* sender);
	void gameLogic(float dt);
	void updateGame(float dt);
    void makeBar();
    void makeBlock();
    void pushBall(CCSet* touches);
    void updateBlocks();
    void updateWalls();
    void gameOver(CCSprite *sprite);
};

#endif  // __HELLOWORLD_SCENE_H__