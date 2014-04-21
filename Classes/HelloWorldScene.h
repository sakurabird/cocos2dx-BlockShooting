#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

// TODO CCSpriteBatchNode


#define BLOCK_COLUMN 15
#define BLOCK_ROW 13

class HelloWorld : public cocos2d::CCLayerColor
{
public:
	HelloWorld();
	~HelloWorld();

    CCSize _visibleSize;
    CCPoint _origin;

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(HelloWorld);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void registerWithTouchDispatcher();

protected:

    //残りボール数をあらわす変数＆アクセサ
    CC_SYNTHESIZE(int, m_ballRemain, BallRemain);

private:
	CCArray *_targets;

	int _blocksDestroyed;

    void initForVariables();

	void addTarget();

    void updateGame(float dt);

    void makeBar();

    void makeBlock();

    void pushBall(CCTouch *touch);

    void moveBar(CCTouch* touch);

    void updateBlocks();

    void updateWalls();

    void updateBar();

    void onBallLost(cocos2d::CCNode* sender);

    void gameOver();
};

#endif  // __HELLOWORLD_SCENE_H__