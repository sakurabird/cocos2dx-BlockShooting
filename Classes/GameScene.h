//
//  GameScene.h
//  BrockShooting
//
//  Created by sakura on 2014/04/22.
//
//

#ifndef __BrockShooting__GameScene__
#define __BrockShooting__GameScene__


#include "cocos2d.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

// TODO CCSpriteBatchNode

class GameScene : public cocos2d::CCLayerColor
{
public:
	GameScene();
	~GameScene();

    CCSize _visibleSize;
    CCPoint _origin;

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(GameScene);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void registerWithTouchDispatcher();

    //Androidのバックキー処理
    virtual void keyBackClicked();

protected:

    //残りボール数をあらわす変数＆アクセサ
    CC_SYNTHESIZE(int, m_ballRemain, BallRemain);

private:
	CCArray *_targets;

    cocos2d::CCSprite *m_background;
    void showBackground();

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
    
    void win();

    void gameOver();
};

#endif /* defined(__BrockShooting__GameScene__) */
