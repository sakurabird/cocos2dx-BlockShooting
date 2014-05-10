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

// TODO CCSpriteBatchNode

class GameScene : public cocos2d::CCLayerColor
{
public:
	GameScene();
	~GameScene();

    cocos2d::CCSize _visibleSize;
    cocos2d::CCPoint _origin;

	virtual bool init();

	static cocos2d::CCScene* scene();

	CREATE_FUNC(GameScene);

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();

    //Androidのバックキー処理
    virtual void keyBackClicked();

protected:


private:
    //残りボール数をあらわす変数＆アクセサ
    CC_SYNTHESIZE(int, m_ballRemain, BallRemain);

    CC_SYNTHESIZE(int, m_score, Score);

    CC_SYNTHESIZE(cocos2d::CCArray*, m_balls, Balls);

    cocos2d::CCArray *m_blocks;

    cocos2d::CCSprite *m_background;

	int m_blocksDestroyed;


    void releaseObject();

    void initForVariables();

    void updateGame(float dt);

    void createBalls();

    void showBackground();

    void showStartLabel();

    void showBallRemain();

    void showScore();

    void makeBar();

    void makeBlock();

    void setBall();

    void moveBar(cocos2d::CCTouch* touch);

    void updateBlocks();

    void updateBall();

    void updateBar();
    
    void onBallLost(cocos2d::CCNode* sender);
    
    void win();

    void gameOver();

    void makeBackButton();

    void onTapBackButton();

    void makeRetryButton();

    void onTapRetryButton();
};

#endif /* defined(__BrockShooting__GameScene__) */
