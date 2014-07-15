//
//  GameScene.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/22.
//
//

#include "GameScene.h"
#include "UserSettings.h"
#include "SimpleAudioEngine.h"
#include "CCPlaySE.h"
#include "AppMacros.h"
#include "GHelper.h"
#include "BallSprite.h"
#include "BarSprite.h"
#include "BlockSprite.h"
#include "GameOverScene.h"
#include "Animations.h"

USING_NS_CC;
using namespace CocosDenshion;

bool isBallActive;

int selectedLevel;

GameScene::GameScene()
:m_blocks(NULL)
,m_balls(NULL)
,m_background(NULL)
,m_item1s(NULL)
,m_item2s(NULL)
,m_item3s(NULL)
,m_item4s(NULL)
,m_item5s(NULL)
,m_blocksDestroyed(0)
,m_score(0)
{
    srand((unsigned int)time(NULL));
}

GameScene::~GameScene()
{
    CCLOG("~GameScene!");

	this->unschedule( schedule_selector(GameScene::updateGame) );
}

CCScene* GameScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		GameScene *layer = GameScene::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);
	} while (0);
	return scene;
}

bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    if (!CCLayerColor::initWithColor( ccc4(0,0,0,0) )) {
        return false;
    }

    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);

    // Android バックキー・メニューキーイベントを取得する
    setKeypadEnabled(true);

    // BGM再生
    if (UserSettings::getMusicSetting() &&
        !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

    initForVariables();

    showBackground();

    showFilter();

    createBalls();

    createGameStateLabels();

    makeBar();

    makeBlock();

    showStartLabel();

    makeBackButton();

    makeRetryButton();

    setBall();

	this->schedule( schedule_selector(GameScene::updateGame) );

    return true;
}

//ゲームループ
void GameScene::updateGame(float dt)
{
    if (m_blocksDestroyed >= BLOCK_COLUMN * BLOCK_ROW) {
        this->win();
        return;
    }

    //ボール移動
    updateBall();

    // ブロックの当たり判定
    updateBlocks();

    // バーの当たり判定
    updateBar();
}

void GameScene::showStartLabel()
{
    CCLabelBMFont* startLabel = CCLabelBMFont::create("Touch to Start", FONT_TOUCH);
    startLabel->setPosition(ccp(_visibleSize.width * 0.5, _visibleSize.height * 0.5));
    startLabel->setTag(kTagStartLabel);
    this->addChild(startLabel);
    startLabel->runAction(Animation::topLabelAction());
}

void GameScene::initForVariables()
{
    _visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    _origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLOG("Hello visibleSize.width: %f, height: %f",_visibleSize.width,_visibleSize.height);
    CCLOG("Hello origin.x: %f, origin.y: %f",_origin.x,_origin.y);

    this->setBallRemain(BALL_REMAIN);

    isBallActive = false;

    m_item1s = CCArray::create();
    m_item1s->retain();
    m_item2s = CCArray::create();
    m_item2s->retain();
    m_item3s = CCArray::create();
    m_item3s->retain();
    m_item4s = CCArray::create();
    m_item4s->retain();
    m_item5s = CCArray::create();
    m_item5s->retain();
}

void GameScene::createBalls()
{
//	m_balls = new CCArray;
    m_balls = CCArray::create();
    m_balls->retain();

    for (int i = 0; i < BALL_REMAIN; i++)
    {
        BallSprite* ball = BallSprite::createWithBallScale(0.7);
        //    CCSprite* projectile = CCSprite::createWithSpriteFrameName("Projectile.png");//テクスチャアトラスを使用
        m_balls->addObject(ball);
    }
}

void GameScene::createGameStateLabels()
{
    //レベル
    selectedLevel = UserSettings::getSelectedLevel();
    CCString* levelString = CCString::createWithFormat("Level %d", selectedLevel + 1);
    CCLabelBMFont* label1 = CCLabelBMFont::create(levelString->getCString(), FONT_GREEN);
    label1->setScale(0.4);
    label1->setAnchorPoint(CCPointZero);
    label1->setPosition(GHelper::convI720toCC(_visibleSize.width  * 0.6, _visibleSize.height * 0.1));
    label1->setTag(kTagLevel);
    this->addChild(label1);

    //残りボール数
    CCLabelBMFont* label2 = CCLabelBMFont::create("Balls:", FONT_WHITE);
    label2->setScale(0.4);
    label2->setAnchorPoint(CCPointZero);
    label2->setPosition(label1->getPositionX(),
                        label1->getPositionY() - label1->getContentSize().height * 0.5);
    label2->setTag(kTagBallRemainLabel);
    this->addChild(label2);

    //スコア
    CCLabelBMFont* label3 = CCLabelBMFont::create("Score:", FONT_WHITE);
    label3->setScale(0.4);
    label3->setAnchorPoint(CCPointZero);
    label3->setPosition(label2->getPositionX(),
                        label2->getPositionY() - label2->getContentSize().height * 0.5);
    label3->setTag(kTagScoreLabel);
    this->addChild(label3);

    showBallRemain();
    showScore();
}

void GameScene::setBall()
{
    if (m_balls->count() <= 0) {
        return;
    }
    BallSprite* ball = dynamic_cast<BallSprite*>(m_balls->lastObject());
    CCNode *bar = this->getChildByTag(kTagBar);
	ball->setPosition( ccp(bar->getPositionX(), bar->getPositionY()+ bar->getContentSize().height) );
	this->addChild(ball);

    m_balls->removeLastObject();
}

void GameScene::showBallRemain()
{
    //残りボール数
    CCLabelBMFont* remain = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(kTagBallRemainCount));
    if (remain) {
        this->removeChild(remain);
    }

    CCLabelBMFont* label = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(kTagBallRemainLabel));

    CCString* ballRemain = CCString::createWithFormat("%d", getBallRemain());
    remain = CCLabelBMFont::create(ballRemain->getCString(), FONT_ORANGE);
    remain->setScale(0.4);
    remain->setAnchorPoint(CCPointZero);
    remain->setPosition(label->getPositionX() + label->getContentSize().width * 0.5 + 5,
                        label->getPositionY());
    remain->setTag(kTagBallRemainCount);
    this->addChild(remain);
}

void GameScene::showScore()
{
    //スコア
    CCLabelBMFont* score = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(kTagScore));
    if (score) {
        this->removeChild(score);
    }

    CCLabelBMFont* label = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(kTagScoreLabel));

    CCString* gameScore = CCString::createWithFormat("%d", getScore());
    score = CCLabelBMFont::create(gameScore->getCString(), FONT_ORANGE);
    score->setScale(0.4);
    score->setAnchorPoint(CCPointZero);
    score->setPosition(label->getPositionX() + label->getContentSize().width * 0.5 + 5,
                       label->getPositionY());
    score->setTag(kTagScore);
    this->addChild(score);
}

void GameScene::makeBar()
{
    float w = _visibleSize.width / 4;
    float h = w / 6;
//    CCLOG("bar.w: %f, height: %f",w,h);

    BarSprite* bar = BarSprite::createWithBarSize(w, h);

    bar->setPosition(GHelper::convI720toCC(_visibleSize.width / 2, _visibleSize.height * 0.95));
    this->addChild(bar);
}


void GameScene::makeBlock()
{
	m_blocks = new CCArray;

    float width = _visibleSize.width / 16.0;
    float height = width * 0.3;
    float margin = (_visibleSize.width - (width * BLOCK_COLUMN)) / (BLOCK_COLUMN + 1);
//    CCLOG("block.size: %f, margin: %f",width, margin);

    //    CCSprite *block = NULL;
    BlockSprite *block = NULL;

    int number = 0;
    int y = _visibleSize.height * 0.6;

    for (int i = 0; i < BLOCK_ROW; i++)
    {
        int x = 0;
        for (int j = 0; j < BLOCK_COLUMN; j++)
        {
            block  = BlockSprite::createWithBlockSize(width, height, number++);
            block->setPosition(GHelper::convI720toCC(x + block->getContentSize().width / 2,
                                                     y + block->getContentSize().height / 2));

            this->addChild(block);
            m_blocks->addObject(block);

            x += block->getContentSize().width + margin;
        }
        y -= block->getContentSize().height + margin;
    }
}

void GameScene::showBackground()
{
    m_background = CCSprite::create(PNG_BG);
    if (!m_background) return;

    float h = m_background->getContentSize().height;
    float sc = _visibleSize.height / h;
    m_background->setScale(sc);

    m_background->setPosition(GHelper::convI720toCC(_visibleSize.width / 2, _visibleSize.height / 2));
    addChild(m_background, kZOrderBackground, kTagBackground);
}

void GameScene::showFilter()
{
    float cellWidth = _visibleSize.width / 4;
    float cellHeight = _visibleSize.height /4;

    float x = 0;
    float y = _visibleSize.height;

    CCDrawNode* node = CCDrawNode::create();
    this->addChild(node);

    ccColor4F color = ccc4f(0, 0, 0, 0.7);

    CCLOG("showFilter");
    int index = 0;
    for(int i = 0; i < 4; i++){
        x = 0;
        for(int j = 0; j < 4; j++){
            if (g_LevelState[0][index] != 1) {
                CCPoint verts[] = {ccp(x, y), ccp(x + cellWidth, y), ccp(x + cellWidth, y - cellHeight), ccp(x , y - cellHeight)};
                node->drawPolygon(verts, 4, color, 0, color);
            }
            x += cellWidth;
            index++;
        }
        y -= cellHeight;
    }

}

void GameScene::onBallLost(CCNode* sender)
{
    if (UserSettings::getSESetting())
        SimpleAudioEngine::sharedEngine()->playEffect(MP3_BALLLOST);

    isBallActive = false;
    
    //奈落に落ちたボールを削除
    BallSprite *ball = dynamic_cast<BallSprite*>(sender);
	this->removeChild(ball, true);

    int remain = this->getBallRemain() - 1;
    this->setBallRemain(remain);

    showBallRemain();

    //ゲームオーバー判定
    if (remain <= 0) {
        this->gameOver();
        return;
    }
    setBall();
}

bool GameScene::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    if (getBlocksDestroyed() >= BLOCK_COLUMN * BLOCK_ROW) {
        return false;
    }

    CCNode* lavel = dynamic_cast<CCNode*>(this->getChildByTag(kTagStartLabel));
    if (lavel) {
        this->removeChild(lavel, true);
    }

    //現在ボールが飛んでいなければボールを飛ばす
    if (!isBallActive) {
        if (UserSettings::getSESetting())
            SimpleAudioEngine::sharedEngine()->playEffect(MP3_BALLPUSH);
        isBallActive = true;
        return true;
    }

	CCPoint location = touch->getLocation();
    CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
    if (!bar) return false;

    //バーの横幅以内がタップされた場合のみタップイベントを有効にする
    bool b = false;
    CCRect rect = bar->boundingBox();
//    CCLOG("bar  getMaxX: %f,  getMidX: %f,  getMinX: %f",rect.getMaxX(),rect.getMidX(),rect.getMinX());
    if (!rect.containsPoint(location))
    {
        b = true;
    }else if (location.x >= rect.getMinX() && location.x <= rect.getMaxX())
    {
        b = true;
    }
    return b;

}

void GameScene::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    // バーを動かす
    moveBar(touch);

}

void GameScene::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
}

void GameScene::updateBall()
{
    if (!isBallActive) {
        return;
    }
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(kTagBall));
    if (!ball) return;

    CCPoint ballPoint = ball->getPosition();
    float vx = ball->getVelocityX();
    float vy = ball->getVelocityY();

    // ボールの移動
    ball->setPosition(ccp(ball->getPositionX() + vx, ball->getPositionY() + vy));

    if( ballPoint.y < 0 )
    {
        //ユーザーがボールを奈落に落とした
        onBallLost(ball);
        return;
    }
    //壁との衝突判定
    ball->bounceBall(_visibleSize);
}

void GameScene::moveBar(CCTouch* touch)
{
	CCPoint location = touch->getLocation();
    CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
    if (!bar) return;
    bar->setPositionX(location.x);
}

void GameScene::updateBlocks()
{
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(kTagBall));
    if (!ball) return;

    CCRect ballRect = ball->boundingBox();

    CCObject* jt = NULL;
    CCArray* blocksToDelete = new CCArray;

    CCARRAY_FOREACH(m_blocks, jt)
    {
        CCSprite* block = dynamic_cast<CCSprite*>(jt);
        //            CCLOG("updateGame target.x: %f, target.y: %f, tag: %d",target->getContentSize().width, target->getContentSize().height, target->getTag());
        CCRect blockRect = block->boundingBox();

        //衝突判定
        if (ballRect.intersectsRect(blockRect))
        {
            // ボールは跳ね返す
            ball->bounceBall(blockRect, kTagBlock);

            blocksToDelete->addObject(block);

            //スコア加算
            m_score += 100;
            showScore();

            //確率に従ってアイテムを生成する
            makeItem(block);
        }
    }

    // 当たったブロックを消す
    CCARRAY_FOREACH(blocksToDelete, jt)
    {
        CCSprite *block = dynamic_cast<CCSprite*>(jt);
        m_blocks->removeObject(block);
        this->removeChild(block, true);

        m_blocksDestroyed++;
    }
    blocksToDelete->release();
}

void GameScene::makeItem(CCSprite *block)
{
    BlockSprite *blockSprite = dynamic_cast<BlockSprite*>(block);
    if (!blockSprite) return;

    CCString* fileName = NULL;
    double itemRate = 0;
    kTag tag = kTagItem1;

    switch (blockSprite->getBlockColor()) {
        case kBlockColorBlue:
            fileName = CCString::createWithFormat(PNG_P_BLUE);
            itemRate = ITEM1_RATE;
            tag = kTagItem1;
            break;

        case kBlockColorGreen:
            fileName = CCString::createWithFormat(PNG_P_GREEN);
            itemRate = ITEM2_RATE;
            tag = kTagItem2;
            break;

        case kBlockColorRed:
            fileName = CCString::createWithFormat(PNG_P_RED);
            itemRate = ITEM3_RATE;
            tag = kTagItem3;
            break;

        case kBlockColorViolet:
            fileName = CCString::createWithFormat(PNG_P_VIOLET);
            itemRate = ITEM4_RATE;
            tag = kTagItem4;
            break;

        case kBlockColorYellow:
            fileName = CCString::createWithFormat(PNG_P_YELLOW);
            itemRate = ITEM5_RATE;
            tag = kTagItem5;
            break;

        default:
            return;
            break;
    }

    if ( (double)rand()/RAND_MAX < itemRate )
    {
        CCSprite* item = CCSprite::create(fileName->getCString());
        item->setPosition(ccp(blockSprite->getPositionX(),
                              blockSprite->getPositionY() + blockSprite->getContentSize().height));
        this->addChild(item, tag);
        item->runAction(Animation::getItemFallAction(block));

        switch (tag) {
            case kTagItem1:
                m_item1s->addObject(item);
                break;
            case kTagItem2:
                m_item2s->addObject(item);
                break;
            case kTagItem3:
                m_item3s->addObject(item);
                break;
            case kTagItem4:
                m_item4s->addObject(item);
                break;
            case kTagItem5:
                m_item5s->addObject(item);
                break;
            default:
                break;
        }
    }
}

void GameScene::updateBar()
{
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(kTagBall));
    if (!ball) return;

    CCRect ballRect = ball->boundingBox();

    BarSprite *bar = dynamic_cast<BarSprite*>(this->getChildByTag(kTagBar));
    if (!bar) return;
    CCRect barRect = bar->boundingBox();

    //衝突判定
    if (ballRect.intersectsRect(barRect))
    {
        // ボールは跳ね返す
        ball->bounceBall(barRect, kTagBar);
    }

    //　アイテムをゲットする処理
    updateItems(barRect);
}

void GameScene::updateItems(cocos2d::CCRect barRect)
{
    CCObject* jt = NULL;
    if (m_item1s != NULL && getItem1s()->count() > 0) {
        CCARRAY_FOREACH(m_item1s, jt)
        {
            CCSprite* item = dynamic_cast<CCSprite*>(jt);
            if (!item) {
                CCLOG("log1");
                break;
            }
            CCRect itemRect = item->boundingBox();
            if (itemRect.intersectsRect(barRect))
            {
                this->removeChild(item, true);
                m_item1s->removeObject(jt);
                onGetItem1();
            }
            else if (itemRect.getMinY() < 0)
            {
                this->removeChild(item, true);
                m_item1s->removeObject(jt);
            }
        }
    }

    if (m_item2s != NULL && getItem2s()->count() > 0) {
        CCARRAY_FOREACH(m_item2s, jt)
        {
            CCSprite* item = dynamic_cast<CCSprite*>(jt);
            if (!item) {
                CCLOG("log2");
                break;
            }
            CCRect itemRect = item->boundingBox();
            if (itemRect.intersectsRect(barRect))
            {
                this->removeChild(item, true);
                m_item2s->removeObject(jt);
                onGetItem2();
            }
            else if (itemRect.getMinY() < 0)
            {
                this->removeChild(item, true);
                m_item2s->removeObject(jt);
            }
        }
    }

    if (m_item3s != NULL && getItem3s()->count() > 0) {
        CCARRAY_FOREACH(m_item3s, jt)
        {
            CCSprite* item = dynamic_cast<CCSprite*>(jt);
            if (!item) {
                CCLOG("log3");
                break;
            }
            CCRect itemRect = item->boundingBox();
            if (itemRect.intersectsRect(barRect))
            {
                this->removeChild(item, true);
                m_item3s->removeObject(jt);
                onGetItem3();
            }
            else if (itemRect.getMinY() < 0)
            {
                this->removeChild(item, true);
                m_item3s->removeObject(jt);
            }
        }
    }

    if (m_item4s != NULL && getItem4s()->count() > 0) {
        CCARRAY_FOREACH(m_item4s, jt)
        {
            CCSprite* item = dynamic_cast<CCSprite*>(jt);
            if (!item) {
                CCLOG("log4");
                break;
            }
            CCRect itemRect = item->boundingBox();
            if (itemRect.intersectsRect(barRect))
            {
                this->removeChild(item, true);
                m_item4s->removeObject(jt);
                onGetItem4();
            }
            else if (itemRect.getMinY() < 0)
            {
                this->removeChild(item, true);
                m_item4s->removeObject(jt);
            }
        }
    }

    if (m_item5s != NULL && getItem5s()->count() > 0) {
        CCARRAY_FOREACH(m_item5s, jt)
        {
            CCSprite* item = dynamic_cast<CCSprite*>(jt);
            if (!item) {
                CCLOG("log5");
                break;
            }
            CCRect itemRect = item->boundingBox();
            if (itemRect.intersectsRect(barRect))
            {
                this->removeChild(item, true);
                m_item5s->removeObject(jt);
                onGetItem5();
            }
            else if (itemRect.getMinY() < 0)
            {
                this->removeChild(item, true);
                m_item5s->removeObject(jt);
            }
        }
    }

}

void GameScene::onGetItem1()
{
    CCString* str = CCString::create("speed up");
    makeItemGetLabel(str);
    //ボールの速度を早くする
}

void GameScene::onGetItem2()
{
    CCString* str = CCString::create("long bar");
    makeItemGetLabel(str);
    //バーの長さを長くする
}

void GameScene::onGetItem3()
{
    CCString* str = CCString::create("multiple balls");
    makeItemGetLabel(str);
    //ボールを追加する
//    BallSprite* ball = BallSprite::createWithBallScale(0.7);
//    CCNode *bar = this->getChildByTag(kTagBar);
//	ball->setPosition( ccp(bar->getPositionX(), bar->getPositionY()+ bar->getContentSize().height) );
//	this->addChild(ball);
}

void GameScene::onGetItem4()
{
    CCString* str = CCString::create("+5000");
    makeItemGetLabel(str);
    //ボーナススコアを加算する
    m_score += 5000;
}

void GameScene::onGetItem5()
{
    CCString* str = CCString::create("+1 Ball!");
    makeItemGetLabel(str);


    CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
    CCParticleMeteor* p = CCParticleMeteor::create();
    p->setTexture( CCTextureCache::sharedTextureCache()->addImage(PNG_P_YELLOW) );
	p->setDuration(0.07);
	p->setSpeed(1000);
    p->setAutoRemoveOnFinish(true);
	p->setPosition(ccp(bar->getPositionX(), bar->getPositionY()));
	// エフェクトの表示
	this->addChild(p);

    //残りボール数を加算する
    BallSprite* ball = BallSprite::createWithBallScale(0.7);
    m_balls->addObject(ball);
    m_ballRemain++;
    showBallRemain();
}

void GameScene::makeItemGetLabel(CCString *string)
{
    CCLabelBMFont* label = CCLabelBMFont::create(string->getCString(), FONT_WHITE);
    label->setScale(0.2);
    CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
    label->setPosition(ccp(bar->getPositionX(), bar->getPositionY() + bar->getContentSize().height / 2));
    label->setTag(kTagItemGetLabel);
    label->runAction(Animation::getItemLabelAction(bar, this,
                                                   callfunc_selector(GameScene::cleanupNode)));
    this->addChild(label);
}

void GameScene::win()
{
    //クリア時の処理
    if (UserSettings::getSESetting())
        SimpleAudioEngine::sharedEngine()->playEffect(MP3_CLEAR);

	this->unschedule( schedule_selector(GameScene::updateGame) );
    isBallActive = false;

    // 現在のレベルの一つ上をアンロック
    if (selectedLevel < 16) {
        selectedLevel++;
    }
    g_LevelState[0][selectedLevel] = 1;
    UserSettings::setLevelState();

    //ボールとバーのオブジェクトを取り除く
    BallSprite* ball = dynamic_cast<BallSprite*>(this->getChildByTag(kTagBall));
    this->removeChild(ball, true);

    BarSprite* bar = dynamic_cast<BarSprite*>(this->getChildByTag(kTagBar));
    this->removeChild(bar, true);

    //パーティクル表示
    CCParticleSystem* emitter = CCParticleExplosion::create();
    emitter->retain();
    this->addChild(emitter, kZOrderEmmit);

    emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(PNG_RECT1) );
    emitter->setAutoRemoveOnFinish(true);
    emitter->setPosition( ccp(_visibleSize.width / 2, _visibleSize.height / 2) );

    //クリアのラベル表示
    CCLabelBMFont* label = CCLabelBMFont::create("CLEAR!", FONT_BIG1);
    label->setPosition( ccp(_visibleSize.width / 2, _visibleSize.height * 0.5));
    label->runAction(Animation::gameClearAction());
    addChild(label, kZOrderLabel);


    CCNode* button = this->getChildByTag(kTagRetry);
    if (!button) return;
    button->setVisible(true);
}

void GameScene::gameOver()
{
    if (UserSettings::getSESetting())
        SimpleAudioEngine::sharedEngine()->playEffect(MP3_GAMEOVER);

	this->unschedule( schedule_selector(GameScene::updateGame) );

    UserSettings::setScore(getScore());

    CCScene* scene = (CCScene*)GameOverScene::create();
    CCTransitionRotoZoom* tran = CCTransitionRotoZoom::create(3, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

void GameScene::makeBackButton()
{
    CCMenuItemImage *item = CCMenuItemImage::create(
                                                    PNG_BACK,
                                                    PNG_BACK,
                                                    this,
                                                    menu_selector(GameScene::onTapBackButton));

    if (!item) return;
    item->setPosition(GHelper::convI720toCC(20, _visibleSize.height * 0.1));
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    if (!menu) return;
    this->addChild(menu, kZOrderTop, kTagBack);
}

void GameScene::makeRetryButton()
{
    //リトライボタンを作成する
    CCMenuItemImage *item = CCMenuItemImage::create(
                                                          PNG_REFRESH,
                                                          PNG_REFRESH,
                                                          this,
                                                          menu_selector(GameScene::onTapRetryButton));
    if (!item) return;
    item->setPosition(GHelper::convI720toCC(100, _visibleSize.height * 0.1));
    item->runAction(Animation::retryButtonAction());

    CCMenu* menu = CCMenu::create(item, NULL);
    if (!menu) return;
    menu->setPosition(CCPointZero);
    this->addChild(menu, kZOrderTop, kTagRetry);
    menu->setVisible(false);

}

//リトライボタンタップ時の処理
void GameScene::onTapRetryButton()
{
    CCScene* gameScene = (CCScene*)GameScene::create();
    CCTransitionTurnOffTiles* tran = CCTransitionTurnOffTiles::create(1, gameScene);
    CCDirector::sharedDirector()->replaceScene(tran);

    CCNode* button = this->getChildByTag(kTagRetry);
    if (!button) return;
    button->setVisible(false);
}

void GameScene::cleanupNode(CCNode *sender)
{
    sender->removeFromParentAndCleanup(true);
}

void GameScene::onTapBackButton()
{
    CCDirector::sharedDirector()->popScene();
}

void GameScene::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

// Androidバックキーイベント
void GameScene::keyBackClicked()
{
    CCDirector::sharedDirector()->popScene();
}

