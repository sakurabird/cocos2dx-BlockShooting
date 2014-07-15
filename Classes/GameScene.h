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
    CC_SYNTHESIZE(int, m_ballRemain, BallRemain);

    CC_SYNTHESIZE(int, m_blocksDestroyed, BlocksDestroyed);

    CC_SYNTHESIZE(int, m_score, Score);

    CC_SYNTHESIZE(cocos2d::CCSprite*, m_background, Background);

    CC_SYNTHESIZE(cocos2d::CCArray*, m_blocks, Blocks);

    CC_SYNTHESIZE(cocos2d::CCArray*, m_balls, Balls);

    CC_SYNTHESIZE(cocos2d::CCArray*, m_item1s, Item1s);
    CC_SYNTHESIZE(cocos2d::CCArray*, m_item2s, Item2s);
    CC_SYNTHESIZE(cocos2d::CCArray*, m_item3s, Item3s);
    CC_SYNTHESIZE(cocos2d::CCArray*, m_item4s, Item4s);
    CC_SYNTHESIZE(cocos2d::CCArray*, m_item5s, Item5s);

    void releaseObject();

    void initForVariables();

    void updateGame(float dt);

    void createBalls();

    void createGameStateLabels();

    void showBackground();

    void showFilter();

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

    void updateItems();

    void onBallLost(cocos2d::CCNode* sender);
    
    void win();

    void gameOver();

    void makeBackButton();

    void onTapBackButton();

    void makeRetryButton();

    void onTapRetryButton();

    void makeItem(cocos2d::CCSprite* block);

    void makeItemGetLabel(cocos2d::CCString* string);

    void cleanupNode(CCNode* sender);

    void onGetItem1();
    void onGetItem2();
    void onGetItem3();
    void onGetItem4();
    void onGetItem5();

    void item1Timer(float time);
    void item2Timer(float time);
    void item3Timer(float time);
};

#endif /* defined(__BrockShooting__GameScene__) */
