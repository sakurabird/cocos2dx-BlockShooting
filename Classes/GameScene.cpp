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
#include "TopScene.h"
#include "GameClearPopup.h"
#include "MyRenderer.h"
#include "Utils.h"

USING_NS_CC;
using namespace CocosDenshion;

#define TEXT_SCALE 0.3

int selectedLevel;
float item1Time = 0;
float item2Time = 0;
bool isTouched;

GameScene::GameScene() :
		m_blocks(NULL), m_balls(NULL), m_activeballs(NULL), m_item1s(
				NULL), m_item2s(NULL), m_item3s(NULL), m_item4s(NULL), m_item5s(
				NULL), m_blocksDestroyed(0), m_score(0), m_ballRemain(0) {
	srand((unsigned int) time(NULL));
	MyRenderer::showAd(2);	//astrOFF();
	CCLog("GameScene start!");
}

GameScene::~GameScene() {
	this->unschedule(schedule_selector(GameScene::updateGame));
}

CCScene* GameScene::scene() {
	CCScene * scene = NULL;
	do {
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		GameScene *layer = GameScene::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);
	} while (0);
	return scene;
}

bool GameScene::init() {
	if (!CCLayer::init()) {
		return false;
	}

	if (!CCLayerColor::initWithColor(ccc4(47, 47, 47, 255))) {
		return false;
	}

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	// Android バックキー・メニューキーイベントを取得する
	setKeypadEnabled(true);

	// BGM再生
	if (UserSettings::getMusicSetting()
			&& !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

	initForVariables();

	Utils::setGirlBackground(this);
	Utils::setGirlFilter(this);

	createBalls();

	createGameStateLabels();

	makeBar();

	makeBlock();

	showStartLabel();

	Utils::createBackButton(this, menu_selector(GameScene::onTapBackButton), 0.1, 0.95);

	setBall();

	this->schedule(schedule_selector(GameScene::updateGame));

	return true;
}

//ゲームループ
void GameScene::updateGame(float dt) {
	if (m_blocksDestroyed >= BLOCK_COLUMN * BLOCK_ROW) {
		this->win();
		return;
	}

	updateBall();

	updateBlocks();

	updateBar();

	updateItems();
}

void GameScene::showStartLabel() {
	CCLabelBMFont* startLabel = CCLabelBMFont::create("Touch to Start",
			FONT_TOUCH);
	startLabel->setPosition(
			ccp(g_visibleSize.width * 0.5, g_visibleSize.height * 0.5));
	startLabel->setTag(kTagStartLabel);
	this->addChild(startLabel);
	startLabel->runAction(Animations::topLabelAction());
}

void GameScene::initForVariables() {
	this->setBallRemain(BALL_REMAIN);

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

	item1Time = 0;
	item2Time = 0;

	isTouched = false;
}

void GameScene::createBalls() {
	m_balls = CCArray::create();
	m_balls->retain();
	m_activeballs = CCArray::create();
	m_activeballs->retain();

	for (int i = 0; i < BALL_REMAIN; i++) {
		BallSprite* ball = BallSprite::createWithBallScale(1, false);
		m_balls->addObject(ball);
	}
}

void GameScene::createGameStateLabels() {
	//背景
	float width = g_visibleSize.width / 1.5;
	float height = 100;

	float x = g_visibleSize.width - width;
	float y = g_visibleSize.height;

	CCDrawNode* node = CCDrawNode::create();
	this->addChild(node);

	ccColor4F color = ccc4f(0, 0, 0, 0.9);
	CCPoint verts[] = { ccp(x, y), ccp(x + width, y),
			ccp(x + width, y - height), ccp(x , y - height) };
	node->drawPolygon(verts, 4, color, 0, color);

	//レベル
	selectedLevel = UserSettings::getSelectedLevel();
	CCString* levelString = CCString::createWithFormat("Level %d",
			selectedLevel + 1);
	CCLabelBMFont* label1 = CCLabelBMFont::create(levelString->getCString(),
			FONT_GREEN);
	label1->setScale(TEXT_SCALE);
	label1->setAnchorPoint(CCPointZero);
	label1->setPosition(g_visibleSize.width * 0.38, g_visibleSize.height - height / 2.15);
	label1->setTag(kTagLevel);
	this->addChild(label1);

	//残りボール数
	CCLabelBMFont* label2 = CCLabelBMFont::create("Balls:", FONT_WHITE);
	label2->setScale(TEXT_SCALE);
	label2->setAnchorPoint(CCPointZero);
	label2->setPosition(label1->getPositionX() * 1.7,
			label1->getPositionY());
	label2->setTag(kTagBallRemainLabel);
	this->addChild(label2);

	//スコア
	CCLabelBMFont* label3 = CCLabelBMFont::create("Score:", FONT_WHITE);
	label3->setScale(TEXT_SCALE);
	label3->setAnchorPoint(CCPointZero);
	label3->setPosition(label1->getPositionX(),
			label2->getPositionY() - label2->getContentSize().height * 0.5);
	label3->setTag(kTagScoreLabel);
	this->addChild(label3);

	//ハイスコア
	CCLabelBMFont* label4 = CCLabelBMFont::create("HighScore:", FONT_WHITE);
	label4->setScale(TEXT_SCALE);
	label4->setAnchorPoint(CCPointZero);
	label4->setPosition(label2->getPositionX(),
			label2->getPositionY() - label2->getContentSize().height * 0.5);
	this->addChild(label4);

	CCString* highScore = CCString::createWithFormat("%d", g_LevelState[1][selectedLevel]);
	CCLabelBMFont* label5 = CCLabelBMFont::create(highScore->getCString(), FONT_ORANGE);
	label5->setScale(TEXT_SCALE);
	label5->setAnchorPoint(CCPointZero);
	label5->setPosition(
			label4->getPositionX() + label4->getContentSize().width * TEXT_SCALE
					+ 10, label4->getPositionY());
	this->addChild(label5);

	showBallRemain();
	showScore();
}

void GameScene::setBall() {
	//ボールをバー上に配置する
	if (m_balls->count() <= 0) {
		return;
	}
	BallSprite* ball = dynamic_cast<BallSprite*>(m_balls->objectAtIndex(0));
	CCNode *bar = this->getChildByTag(kTagBar);
//	CCRect rect = bar->boundingBox();
	ball->setPosition(
			ccp(bar->getPositionX(), bar->getPositionY() + bar->getContentSize().height + 10));
//    CCLOG("# setBall #bar.width: %f, height: %f",
//          bar->getContentSize().width, bar->getContentSize().height);
//    CCLOG("# setBall #bar position X: %f, Y: %f",
//          bar->getPositionX(), bar->getPositionY());
//	ball->setPosition(
//			ccp(bar->getPositionX() + bar->getContentSize().width / 2, bar->getPositionY() + bar->getContentSize().height));
	this->addChild(ball);

	m_balls->removeObjectAtIndex(0);
	m_activeballs->addObject(ball);
}

void GameScene::showBallRemain() {
	//残りボール数
	CCLabelBMFont* remain = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(
			kTagBallRemainCount));
	if (remain) {
		this->removeChild(remain);
	}

	CCLabelBMFont* label = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(
			kTagBallRemainLabel));

	CCString* ballRemain = CCString::createWithFormat("%d", getBallRemain());
	remain = CCLabelBMFont::create(ballRemain->getCString(), FONT_ORANGE);
	remain->setScale(TEXT_SCALE);
	remain->setAnchorPoint(CCPointZero);
	remain->setPosition(
			label->getPositionX() + label->getContentSize().width * TEXT_SCALE
					+ 10, label->getPositionY());
	remain->setTag(kTagBallRemainCount);
	this->addChild(remain);
}

void GameScene::showScore() {
	//スコア
	CCLabelBMFont* score = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(
			kTagScore));
	if (score) {
		score->removeFromParent();
	}

	CCLabelBMFont* label = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(
			kTagScoreLabel));

	CCString* gameScore = CCString::createWithFormat("%d", getScore());
	score = CCLabelBMFont::create(gameScore->getCString(), FONT_ORANGE);
	score->setScale(TEXT_SCALE);
	score->setAnchorPoint(CCPointZero);
	score->setPosition(
			label->getPositionX() + label->getContentSize().width * TEXT_SCALE
					+ 10, label->getPositionY());
	score->setTag(kTagScore);
	this->addChild(score);
}

void GameScene::makeBar() {
	float w = g_visibleSize.width / 4;
	float h = w / 6;
//    CCLOG("bar.size w: %f, h: %f",w, h);

	BarSprite* bar = BarSprite::createWithBarSize(w, h);

//	bar->setPosition(ccp(g_visibleSize.width / 2 - bar->getContentSize().width / 2, g_visibleSize.height * 0.13));
//    CCLOG("# makebar # bar.pos X: %f, Y: %f",bar->getPositionX(), bar->getPositionY());
	bar->setPosition(
			GHelper::convI720toCC(g_visibleSize.width / 2,
					g_visibleSize.height * 0.88));
	this->addChild(bar);
}

void GameScene::makeBlock() {
	m_blocks = new CCArray;

	float width = g_visibleSize.width / 16.0;
	float height = width * 0.3;
	float margin = (g_visibleSize.width - (width * BLOCK_COLUMN))
			/ (BLOCK_COLUMN + 1);
//    CCLOG("block.size: %f, margin: %f",width, margin);

	BlockSprite *block = NULL;

	int number = 0;
	int y = g_visibleSize.height * 0.55;

	for (int i = 0; i < BLOCK_ROW; i++) {
		int x = 0;
		for (int j = 0; j < BLOCK_COLUMN; j++) {
			block = BlockSprite::createWithBlockSize(width, height, number++);
			block->setPosition(
					GHelper::convI720toCC(x + block->getContentSize().width / 2,
							y + block->getContentSize().height / 2));

			this->addChild(block);
			m_blocks->addObject(block);

			x += block->getContentSize().width + margin;
		}
		y -= block->getContentSize().height + margin;
	}
}

void GameScene::onBallLost(CCNode* sender) {
	if (UserSettings::getSESetting())
		SimpleAudioEngine::sharedEngine()->playEffect(MP3_BALLLOST);

	BallSprite *ball = dynamic_cast<BallSprite*>(sender);
	if (ball->getIsItem3()) {
		this->removeChild(ball, true);
		m_activeballs->removeObject(ball);
		if (m_activeballs->count() == 0) {
			isTouched = false;
		}
		return;
	}

	cancelTimers();

	//奈落に落ちたボールを削除
	this->removeChild(ball, true);
	m_activeballs->removeObject(ball);

	int remain = this->getBallRemain() - 1;
	this->setBallRemain(remain);

	showBallRemain();

	if (m_activeballs->count() == 0) {
		isTouched = false;
	}

	//ゲームオーバー判定
	if (remain <= 0) {
		this->gameOver();
		return;
	}
	setBall();
}

bool GameScene::ccTouchBegan(CCTouch *touch, CCEvent *event) {
	if (getBlocksDestroyed() >= BLOCK_COLUMN * BLOCK_ROW) {
		return false;
	}

	CCNode* lavel = dynamic_cast<CCNode*>(this->getChildByTag(kTagStartLabel));
	if (lavel) {
		this->removeChild(lavel, true);
	}

//	CCLog("ccTouchBegan1");
	//現在ボールが飛んでいなければボールを飛ばす
	if (!isTouched) {
		if (UserSettings::getSESetting())
			SimpleAudioEngine::sharedEngine()->playEffect(MP3_BALLPUSH);
		isTouched = true;
		return true;
	}

	CCPoint location = touch->getLocation();
	CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
	if (!bar)
		return false;

	//バーの横幅以内がタップされた場合のみタップイベントを有効にする
	bool b = false;
	CCRect rect = bar->boundingBox();
	if (!rect.containsPoint(location)) {
		b = true;
	} else if (location.x >= rect.getMinX() && location.x <= rect.getMaxX()) {
		b = true;
	}
	return b;

}

void GameScene::ccTouchMoved(CCTouch *touch, CCEvent *event) {
	// バーを動かす
	moveBar(touch);

}

void GameScene::ccTouchEnded(CCTouch *touch, CCEvent* event) {
}

void GameScene::updateBall() {
	if (!isTouched) {
		return;
	}

	for (int i = 0; i < m_activeballs->count(); i++) {
		BallSprite* ball =
				dynamic_cast<BallSprite*>(m_activeballs->objectAtIndex(i));
		if (!ball)
			return;

		CCPoint ballPoint = ball->getPosition();
		float vx = ball->getVelocityX();
		float vy = ball->getVelocityY();

		// ボールの移動
//	    CCLOG("# updateBall1 #ball position X: %f, Y: %f",
//	          ball->getPositionX(), ball->getPositionY());
		ball->setPosition(ccp(ball->getPositionX() + vx,
				ball->getPositionY() + vy));
//	    CCLOG("# updateBall2 #ball position X: %f, Y: %f",
//	          ball->getPositionX(), ball->getPositionY());

		if (ballPoint.y < 0) {
			//ユーザーがボールを奈落に落とした
			onBallLost(ball);
			return;
		}
		//壁との衝突判定
		ball->bounceBall(g_visibleSize);
	}
}

void GameScene::moveBar(CCTouch* touch) {
	CCPoint location = touch->getLocation();
	CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
	if (!bar)
		return;
	bar->setPositionX(location.x);
}

void GameScene::updateBlocks() {
	for (int i = 0; i < m_activeballs->count(); i++) {
		BallSprite* ball =
				dynamic_cast<BallSprite*>(m_activeballs->objectAtIndex(i));
		if (!ball)
			return;

		CCRect ballRect = ball->boundingBox();

		CCObject* jt = NULL;
		CCArray* blocksToDelete = new CCArray;

		CCARRAY_FOREACH(m_blocks, jt) {
			CCSprite* block = dynamic_cast<CCSprite*>(jt);
			//            CCLOG("updateGame target.x: %f, target.y: %f, tag: %d",target->getContentSize().width, target->getContentSize().height, target->getTag());
			CCRect blockRect = block->boundingBox();

			//衝突判定
			if (ballRect.intersectsRect(blockRect)) {
				// ボールは跳ね返す
				ball->bounceBall(blockRect, kTagBlock);

				blocksToDelete->addObject(block);

				//スコア加算
				m_score += 100;
				showScore();

				//確率に従ってボーナスアイテムを生成する
				makeItem(block);
			}
		}

		// 当たったブロックを消す
		CCARRAY_FOREACH(blocksToDelete, jt) {
			CCSprite *block = dynamic_cast<CCSprite*>(jt);
			m_blocks->removeObject(block);
			this->removeChild(block, true);

			m_blocksDestroyed++;
		}
		blocksToDelete->release();
	}
}

void GameScene::updateBar() {
	for (int i = 0; i < m_activeballs->count(); i++) {
		BallSprite* ball =
				dynamic_cast<BallSprite*>(m_activeballs->objectAtIndex(i));
		if (!ball)
			return;

		CCRect ballRect = ball->boundingBox();

		BarSprite *bar = dynamic_cast<BarSprite*>(this->getChildByTag(kTagBar));
		if (!bar)
			return;
		CCRect barRect = bar->boundingBox();

		//衝突判定
		if (ballRect.intersectsRect(barRect)) {
			// ボールは跳ね返す
			ball->bounceBall(barRect, kTagBar);
		}
	}
}

void GameScene::updateItems() {
	// 落下してくるアイテムをバーで受け止めた場合の処理
	BarSprite *bar = dynamic_cast<BarSprite*>(this->getChildByTag(kTagBar));
	if (!bar)
		return;
	CCRect barRect = bar->boundingBox();

	CCArray* itemsToDelete = new CCArray;

	CCObject* jt1 = NULL;
	CCObject* jt2 = NULL;
	CCObject* jt3 = NULL;
	CCObject* jt4 = NULL;
	CCObject* jt5 = NULL;

	if (m_item1s != NULL && getItem1s()->count() > 0) {
		CCARRAY_FOREACH(m_item1s, jt1) {
			CCSprite* item = dynamic_cast<CCSprite*>(jt1);
			if (!item)
				break;
			CCRect itemRect = item->boundingBox();
			if (itemRect.intersectsRect(barRect)) {
				item->removeFromParent();
				m_item1s->removeObject(item);
				onGetItem1();
			} else if (itemRect.getMinY() < 10) {
				item->removeFromParent();
				m_item1s->removeObject(item);
			}
		}
	}

	if (m_item2s != NULL && getItem2s()->count() > 0) {
		CCARRAY_FOREACH(m_item2s, jt2) {
			CCSprite* item = dynamic_cast<CCSprite*>(jt2);
			if (!item)
				break;
			CCRect itemRect = item->boundingBox();
			if (itemRect.intersectsRect(barRect)) {
				item->removeFromParent();
				m_item2s->removeObject(item);
				onGetItem2();
			} else if (itemRect.getMinY() < 10) {
				item->removeFromParent();
				m_item2s->removeObject(item);
			}
		}
	}

	if (m_item3s != NULL && getItem3s()->count() > 0) {
		CCARRAY_FOREACH(m_item3s, jt3) {
			CCSprite* item = dynamic_cast<CCSprite*>(jt3);
			if (!item)
				break;
			CCRect itemRect = item->boundingBox();
			if (itemRect.intersectsRect(barRect)) {
				item->removeFromParent();
				m_item3s->removeObject(item);
				onGetItem3();
			} else if (itemRect.getMinY() < 10) {
				item->removeFromParent();
				m_item3s->removeObject(item);
			}
		}
	}

	if (m_item4s != NULL && getItem4s()->count() > 0) {
		CCARRAY_FOREACH(m_item4s, jt4) {
			CCSprite* item = dynamic_cast<CCSprite*>(jt4);
			if (!item)
				break;
			CCRect itemRect = item->boundingBox();
			if (itemRect.intersectsRect(barRect)) {
				item->removeFromParent();
				m_item4s->removeObject(item);
				onGetItem4();
			} else if (itemRect.getMinY() < 10) {
				item->removeFromParent();
				m_item4s->removeObject(item);
			}
		}
	}

	if (m_item5s != NULL && getItem5s()->count() > 0) {
		CCARRAY_FOREACH(m_item5s, jt5) {
			CCSprite* item = dynamic_cast<CCSprite*>(jt5);
			if (!item)
				break;
			CCRect itemRect = item->boundingBox();
			if (itemRect.intersectsRect(barRect)) {
				item->removeFromParent();
				m_item5s->removeObject(item);
				onGetItem5();
			} else if (itemRect.getMinY() < 10) {
				item->removeFromParent();
				m_item5s->removeObject(item);
			}
		}
	}
}

void GameScene::makeItem(CCSprite *block) {
	//ボーナスアイテム作成
	BlockSprite *blockSprite = dynamic_cast<BlockSprite*>(block);
	if (!blockSprite)
		return;

	CCString* fileName = NULL;
	double itemRate = 0;
	kTag tag = kTagItem1;

	switch (blockSprite->getBlockColor()) {
	case kBlockColorBlue:
		fileName = CCString::createWithFormat(PNG_P_BLUE);
		itemRate = ITEM1_RATE + (selectedLevel * 0.005);
		tag = kTagItem1;
		break;

	case kBlockColorGreen:
		fileName = CCString::createWithFormat(PNG_P_GREEN);
		itemRate = ITEM2_RATE + (selectedLevel * 0.005);
		tag = kTagItem2;
		break;

	case kBlockColorRed:
		fileName = CCString::createWithFormat(PNG_P_RED);
		itemRate = ITEM3_RATE + (selectedLevel * 0.005);
		tag = kTagItem3;
		break;

	case kBlockColorViolet:
		fileName = CCString::createWithFormat(PNG_P_VIOLET);
		itemRate = ITEM4_RATE + (selectedLevel * 0.005);
		tag = kTagItem4;
		break;

	case kBlockColorYellow:
		fileName = CCString::createWithFormat(PNG_P_YELLOW);
		itemRate = ITEM5_RATE + (selectedLevel * 0.005);
		tag = kTagItem5;
		break;

	default:
		return;
		break;
	}

	if ((double) rand() / RAND_MAX < itemRate) {
		CCSprite* item = CCSprite::createWithSpriteFrameName(fileName->getCString());
		if (item) {
//			item->autorelease();
		}else{
			CC_SAFE_DELETE(item);
			return;
		}
		item->setScale(0.2f);
		item->setPosition(
				ccp(blockSprite->getPositionX(),
						blockSprite->getPositionY() + blockSprite->getContentSize().height));
		item->setTag(tag);
		item->runAction(Animations::getItemFallAction(block));

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

		this->addChild(item);
	}
}

void GameScene::onGetItem1() {
	m_score += 500;
	CCString* str = CCString::create("speed up");
	makeItemGetLabel(str);

	//ボールの速度を早くする
	for (int i = 0; i < m_activeballs->count(); i++) {
		BallSprite* ball =
				dynamic_cast<BallSprite*>(m_activeballs->objectAtIndex(i));
		if (!ball)
			return;
		ball->addVelocity(ITEM1_VELOCITY);
	}
	item1Time += ITEM_LIFE_SECONDS;
	this->schedule(schedule_selector(GameScene::item1Timer));
}

void GameScene::onGetItem2() {
	m_score += 100;
	CCString* str = CCString::create("long bar");
	makeItemGetLabel(str);
	//バーの長さを長くする
	item2Time += ITEM_LIFE_SECONDS;
	this->schedule(schedule_selector(GameScene::item2Timer));
}

void GameScene::onGetItem3() {
	m_score += 100;
	CCString* str = CCString::create("multiple balls");
	makeItemGetLabel(str);
	//ボールを追加する
	BallSprite* ball = BallSprite::createWithBallScale(1, true);
	CCNode *bar = this->getChildByTag(kTagBar);
	ball->setPosition(
			ccp(bar->getPositionX(), bar->getPositionY()+ bar->getContentSize().height));
	this->addChild(ball);
	m_activeballs->addObject(ball);
}

void GameScene::onGetItem4() {
	m_score += 100;
	CCString* str = CCString::create("+10000");
	makeItemGetLabel(str);
	//ボーナススコアを加算する
	m_score += 10000;
}

void GameScene::onGetItem5() {
	m_score += 100;
	CCString* str = CCString::create("+1 Ball!");
	makeItemGetLabel(str);

	CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
	CCParticleMeteor* p = CCParticleMeteor::create();
	p->setTexture(CCTextureCache::sharedTextureCache()->addImage(PNG_RECT1));
	p->setDuration(0.07);
	p->setSpeed(1000);
	p->setAutoRemoveOnFinish(true);
	p->setPosition(ccp(bar->getPositionX(), bar->getPositionY()));
	// エフェクトの表示
	this->addChild(p);

	//残りボール数を加算する
	BallSprite* ball = BallSprite::createWithBallScale(1, false);
	m_balls->addObject(ball);
	m_ballRemain++;
	showBallRemain();
}

void GameScene::makeItemGetLabel(CCString *string) {
	CCLabelBMFont* label = CCLabelBMFont::create(string->getCString(),
			FONT_WHITE);
	label->setScale(0.2);
	CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
	label->setPosition(
			ccp(bar->getPositionX(), bar->getPositionY() + bar->getContentSize().height / 2));
	label->setTag(kTagItemGetLabel);
	label->runAction(
			Animations::getItemLabelAction(bar, this,
					callfunc_selector(GameScene::cleanupNode)));
	this->addChild(label);
}

void GameScene::item1Timer(float time) {
	item1Time -= time;

	if (item1Time <= 0) {
		//速度を戻す
		for (int i = 0; i < m_activeballs->count(); i++) {
			BallSprite* ball =
					dynamic_cast<BallSprite*>(m_activeballs->objectAtIndex(i));
			if (!ball)
				return;
			ball->addVelocity(ITEM1_VELOCITY * -1);
		}
		this->unschedule(schedule_selector(GameScene::item1Timer));
		item1Time = 0;
		return;
	}
}
void GameScene::item2Timer(float time) {
	item2Time -= time;

	BarSprite *bar = dynamic_cast<BarSprite*>(this->getChildByTag(kTagBar));
	if (!bar)
		return;
	if (item2Time <= 0) {
		//バーの長さを戻す
		bar->restoreScale();
		this->unschedule(schedule_selector(GameScene::item2Timer));
		item2Time = 0;
		return;
	}

	bar->setScaleLonger();
}

void GameScene::cancelTimers() {
	this->unschedule(schedule_selector(GameScene::item1Timer));
	this->unschedule(schedule_selector(GameScene::item2Timer));
	item1Time = 0;
	item2Time = 0;
}

void GameScene::win() {
	//クリア時の処理
	if (UserSettings::getSESetting())
		SimpleAudioEngine::sharedEngine()->playEffect(MP3_CLEAR);

	// 残りボール数があるときスコアを加算する
	int ballScore = m_ballRemain * 10000;
	m_score += ballScore;

	this->unschedule(schedule_selector(GameScene::updateGame));

	setResultScores();

	// 現在のレベルの一つ上をアンロック
	if (selectedLevel < 15) {
		selectedLevel++;
	}
	g_LevelState[0][selectedLevel] = 1;
	UserSettings::saveLevelState();

	//ボールとバーのオブジェクトを取り除く
	for (int i = 0; i < m_activeballs->count(); i++) {
		BallSprite* ball =
				dynamic_cast<BallSprite*>(m_activeballs->objectAtIndex(i));
		if (!ball)
			break;
		this->removeChild(ball, true);
	}

	BarSprite* bar = dynamic_cast<BarSprite*>(this->getChildByTag(kTagBar));
	this->removeChild(bar, true);

	//パーティクル表示
	CCParticleSystem* emitter = CCParticleExplosion::create();
	emitter->retain();
	this->addChild(emitter, kZOrderEmmit);

	emitter->setTexture(
			CCTextureCache::sharedTextureCache()->addImage(PNG_RECT1));
	emitter->setAutoRemoveOnFinish(true);
	emitter->setPosition(
			ccp(g_visibleSize.width / 2, g_visibleSize.height / 2));

	//クリアのラベル表示
    CCString *lb = NULL;
	if (selectedLevel >= 15) {
		lb = CCString::create("ALL CLEAR★");
	} else {
		lb = CCString::create("CLEAR!");
	}
	CCLabelBMFont* label = CCLabelBMFont::create(lb->getCString(), FONT_BIG1);
	label->setPosition(
			ccp(g_visibleSize.width / 2, g_visibleSize.height * 0.5));
	label->runAction(
			Animations::gameClearAction(this,
					callfunc_selector(GameScene::showClearPopup)));
	addChild(label, kZOrderLabel);

	//残りボール数のプラススコアラベル
	if (ballScore > 0) {
		CCString* s = CCString::createWithFormat("remain ball +%d", ballScore);
		CCLabelBMFont* lb = CCLabelBMFont::create(s->getCString(), FONT_WHITE);
		lb->setScale(0.3);
		CCNode *n = this->getChildByTag(kTagScore);
		lb->setPosition(ccp(n->getPositionX(), n->getPositionY()));
		lb->runAction(Animations::gameAddingScoreAction());
		this->addChild(lb);
	}

	showScore();
}

void GameScene::showClearPopup(CCObject* sender) {
	// ポップアップを表示して確認
	GameClearPopup* pl = GameClearPopup::create();
	this->addChild(pl);
}

void GameScene::gameOver() {
	if (UserSettings::getSESetting())
		SimpleAudioEngine::sharedEngine()->playEffect(MP3_GAMEOVER);

	// 残りボール数があるときスコアを加算する
	m_score += m_ballRemain * 10000;

	this->unschedule(schedule_selector(GameScene::updateGame));

	setResultScores();

	CCScene* scene = (CCScene*) GameOverScene::create();
	CCTransitionRotoZoom* tran = CCTransitionRotoZoom::create(3, scene);
	CCDirector::sharedDirector()->replaceScene(tran);
}

void GameScene::setResultScores() {
//	    CCLOG("# setResultScores # m_score: %d", m_score);

	UserSettings::setScore(m_score);
	if (g_LevelState[1][selectedLevel] < m_score) {
		g_LevelState[1][selectedLevel] = m_score;
		UserSettings::saveLevelState();
	}
	UserSettings::setHighScore(g_LevelState[1][selectedLevel]);
}

void GameScene::cleanupNode(CCNode *sender) {
//	sender->removeFromParentAndCleanup(true);
}

void GameScene::onTapBackButton() {
	CCScene* scene = (CCScene*) TopScene::create();
	CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
	CCDirector::sharedDirector()->replaceScene(tran);
}

void GameScene::registerWithTouchDispatcher() {
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,
			kCCMenuHandlerPriority + 1, true);
}

// Androidバックキーイベント
void GameScene::keyBackClicked() {
	CCScene* scene = (CCScene*) TopScene::create();
	CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
	CCDirector::sharedDirector()->replaceScene(tran);
}

