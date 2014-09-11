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

USING_NS_CC;
using namespace CocosDenshion;

#define TEXT_SCALE 0.3

int selectedLevel;
float item1Time = 0;
float item2Time = 0;
bool isTouched;

GameScene::GameScene() :
		m_blocks(NULL), m_balls(NULL), m_activeballs(NULL), m_background(NULL), m_item1s(
				NULL), m_item2s(NULL), m_item3s(NULL), m_item4s(NULL), m_item5s(
				NULL), m_blocksDestroyed(0), m_score(0), m_ballRemain(0) {
	srand((unsigned int) time(NULL));
	CCLog("sssssssssssstttttttttaaaaaaaaaaaaarrrrrrrrrttttttttttttt");
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

	showBackground();

	showFilter();

	createBalls();

	createGameStateLabels();

	makeBar();

	makeBlock();

	showStartLabel();

	makeBackButton();

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
		BallSprite* ball = BallSprite::createWithBallScale(0.7);
		//    CCSprite* projectile = CCSprite::createWithSpriteFrameName("Projectile.png");//テクスチャアトラスを使用
		m_balls->addObject(ball);
	}
}

void GameScene::createGameStateLabels() {
	//背景
	float width = g_visibleSize.width / 3.5;
	float height = 120;

	float x = g_visibleSize.width - width;
	float y = g_visibleSize.height - 120;

	CCDrawNode* node = CCDrawNode::create();
	this->addChild(node);

	ccColor4F color = ccc4f(0, 0, 0, 0.7);
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
	label1->setPosition(
			GHelper::convI720toCC(g_visibleSize.width * 0.7,
					g_visibleSize.height * 0.09));
	label1->setTag(kTagLevel);
	this->addChild(label1);

	//残りボール数
	CCLabelBMFont* label2 = CCLabelBMFont::create("Balls:", FONT_WHITE);
	label2->setScale(TEXT_SCALE);
	label2->setAnchorPoint(CCPointZero);
	label2->setPosition(label1->getPositionX(),
			label1->getPositionY() - label1->getContentSize().height * 0.5);
	label2->setTag(kTagBallRemainLabel);
	this->addChild(label2);

	//スコア
	CCLabelBMFont* label3 = CCLabelBMFont::create("Score:", FONT_WHITE);
	label3->setScale(TEXT_SCALE);
	label3->setAnchorPoint(CCPointZero);
	label3->setPosition(label2->getPositionX(),
			label2->getPositionY() - label2->getContentSize().height * 0.5);
	label3->setTag(kTagScoreLabel);
	this->addChild(label3);

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
	ball->setPosition(
			ccp(bar->getPositionX(), bar->getPositionY()+ bar->getContentSize().height));
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

	BarSprite* bar = BarSprite::createWithBarSize(w, h);

	bar->setPosition(
			GHelper::convI720toCC(g_visibleSize.width / 2,
					g_visibleSize.height * 0.9));
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

void GameScene::showBackground() {
	m_background = CCSprite::create(PNG_BG);
	if (!m_background)
		return;

	float h = m_background->getContentSize().height;
	float sc = g_visibleSize.height / h;
	m_background->setScale(sc);

	m_background->setPosition(
			GHelper::convI720toCC(g_visibleSize.width / 2,
					g_visibleSize.height / 2));
	addChild(m_background, kZOrderBackground, kTagBackground);
}

void GameScene::showFilter() {
	float cellWidth = g_visibleSize.width / 4;
	float cellHeight = g_visibleSize.height / 4;

	float x = 0;
	float y = g_visibleSize.height;

	CCDrawNode* node = CCDrawNode::create();
	this->addChild(node);

	ccColor4F color = ccc4f(0, 0, 0, 0.7);

	int index = 0;
	for (int i = 0; i < 4; i++) {
		x = 0;
		for (int j = 0; j < 4; j++) {
			if (g_LevelState[0][index] != 1) {
				CCPoint verts[] = { ccp(x, y), ccp(x + cellWidth, y),
						ccp(x + cellWidth, y - cellHeight),
						ccp(x , y - cellHeight) };
				node->drawPolygon(verts, 4, color, 0, color);
			}
			x += cellWidth;
			index++;
		}
		y -= cellHeight;
	}
}

void GameScene::onBallLost(CCNode* sender) {
	if (UserSettings::getSESetting())
		SimpleAudioEngine::sharedEngine()->playEffect(MP3_BALLLOST);

	cancelTimers();

	//奈落に落ちたボールを削除
	BallSprite *ball = dynamic_cast<BallSprite*>(sender);
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
		ball->setPosition(ccp(ball->getPositionX() + vx,
				ball->getPositionY() + vy));

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

//void GameScene::updateItems() {
//	// 落下してくるアイテムをバーで受け止めた場合の処理
//	BarSprite *bar = dynamic_cast<BarSprite*>(this->getChildByTag(kTagBar));
//	if (!bar)
//		return;
//	CCRect barRect = bar->boundingBox();
//
//	CCArray* itemsToDelete = new CCArray;
//
//	CCObject* jt = NULL;
//	CCLog("＊updateItems");
//	if (m_item1s != NULL && getItem1s()->count() > 0) {
//		CCLog("ーーupdateItems1-1");
//		CCARRAY_FOREACH(m_item1s, jt) {
//			CCLog("ーーupdateItems1-2");
//			CCSprite* item = dynamic_cast<CCSprite*>(jt);
//			if (!item)
//				break;
//			CCLog("ーーtag:%d",item->getTag());
//			CCRect itemRect = item->boundingBox();
//			if (itemRect.intersectsRect(barRect)) {
//				itemsToDelete->addObject(item);
//				CCLog("ーーupdateItems1-3");
//				onGetItem1();
//			} else if (itemRect.getMinY() < 10) {
//				CCLog("ーーupdateItems1-4");
//				itemsToDelete->addObject(item);
//			}
//		}
//		CCLog("ーーupdateItems1-5");
//	}
//
//	jt = NULL;
//	if (m_item2s != NULL && getItem2s()->count() > 0) {
//		CCLog("ーーupdateItems2-1");
//		CCARRAY_FOREACH(m_item2s, jt) {
//			CCLog("ーーupdateItems2-2");
//			CCSprite* item = dynamic_cast<CCSprite*>(jt);
//			if (!item)
//				break;
//			CCLog("ーーtag:%d",item->getTag());
//			CCRect itemRect = item->boundingBox();
//			if (itemRect.intersectsRect(barRect)) {
//				itemsToDelete->addObject(item);
//				CCLog("ーーupdateItems2-3");
//				onGetItem2();
//			} else if (itemRect.getMinY() < 10) {
//				CCLog("ーーupdateItems2-4");
//				itemsToDelete->addObject(item);
//			}
//		}
//		CCLog("ーーupdateItems2-5");
//	}
//
//	jt = NULL;
//	if (m_item3s != NULL && getItem3s()->count() > 0) {
//		CCLog("ーーupdateItems3-1");
//		CCARRAY_FOREACH(m_item3s, jt) {
//			CCLog("ーーupdateItems3-2");
//			CCSprite* item = dynamic_cast<CCSprite*>(jt);
//			if (!item)
//				break;
//			CCLog("ーーtag:%d",item->getTag());
//			CCRect itemRect = item->boundingBox();
//			if (itemRect.intersectsRect(barRect)) {
//				itemsToDelete->addObject(item);
//				CCLog("ーーupdateItems3-3");
//				onGetItem3();
//			} else if (itemRect.getMinY() < 10) {
//				CCLog("ーーupdateItems3-4");
//				itemsToDelete->addObject(item);
//			}
//		}
//		CCLog("ーーupdateItems3-5");
//	}
//
//	jt = NULL;
//	if (m_item4s != NULL && getItem4s()->count() > 0) {
//		CCLog("ーーupdateItems4-1");
//		CCARRAY_FOREACH(m_item4s, jt) {
//			CCLog("ーーupdateItems4-2");
//			CCSprite* item = dynamic_cast<CCSprite*>(jt);
//			if (!item)
//				break;
//			CCLog("ーーtag:%d",item->getTag());
//			CCRect itemRect = item->boundingBox();
//			if (itemRect.intersectsRect(barRect)) {
//				itemsToDelete->addObject(item);
//				CCLog("ーーupdateItems4-3");
//				onGetItem4();
//			} else if (itemRect.getMinY() < 10) {
//				CCLog("ーーupdateItems4-4");
//				itemsToDelete->addObject(item);
//			}
//		}
//		CCLog("ーーupdateItems4-5");
//	}
//
//	jt = NULL;
//	if (m_item5s != NULL && getItem5s()->count() > 0) {
//		CCLog("ーーupdateItems5-1");
//		CCARRAY_FOREACH(m_item5s, jt) {
//			CCLog("ーーupdateItems5-2");
//			CCSprite* item = dynamic_cast<CCSprite*>(jt);
//			if (!item)
//				break;
//			CCLog("ーーtag:%d",item->getTag());
//			CCRect itemRect = item->boundingBox();
//			if (itemRect.intersectsRect(barRect)) {
//				itemsToDelete->addObject(item);
//				CCLog("ーーupdateItems5-3");
//				onGetItem5();
//			} else if (itemRect.getMinY() < 10) {
//				CCLog("ーーupdateItems5-4");
//				itemsToDelete->addObject(item);
//			}
//		}
//		CCLog("ーーupdateItems5-5");
//	}
//
//	// バーでキャッチした、又は奈落に落ちたアイテムを消す
//	CCLog("@@@itemsToDelete count: %d",itemsToDelete->count());
//	jt = NULL;
//	CCARRAY_FOREACH(itemsToDelete, jt) {
//		CCNode *item = dynamic_cast<CCNode*>(jt);
//		if (!item) {
//			continue;
//		}
//
//		CCLog("@@@remove@@@");
////		CCLog("@@@remove2@@@");
//
//		switch (item->getTag()) {
//		case kTagItem1:
//			CCLog("@@@kTagItem1 tag:%d",item->getTag());
//			CCLog("@@@indexOfObject():%d",m_item1s->indexOfObject(item));
//			m_item1s->removeObject(item);
//			CCLog("@@@kTagItem1-out");
//			break;
//		case kTagItem2:
//			CCLog("@@@kTagItem2 tag:%d",item->getTag());
//			CCLog("@@@indexOfObject():%d",m_item2s->indexOfObject(item));
//			m_item2s->removeObject(item);
//			CCLog("@@@kTagItem2-out");
//			break;
//		case kTagItem3:
//			CCLog("@@@kTagItem3 tag:%d",item->getTag());
//			CCLog("@@@indexOfObject():%d",m_item3s->indexOfObject(item));
//			m_item3s->removeObject(item);
//			CCLog("@@@kTagItem3-out");
//			break;
//		case kTagItem4:
//			CCLog("@@@kTagItem4 tag:%d",item->getTag());
//			CCLog("@@@indexOfObject():%d",m_item4s->indexOfObject(item));
//			m_item4s->removeObject(item);
//			CCLog("@@@kTagItem4-out");
//			break;
//		case kTagItem5:
//			CCLog("@@@kTagItem5 tag:%d",item->getTag());
//			CCLog("@@@indexOfObject():%d",m_item5s->indexOfObject(item));
//			m_item5s->removeObject(item);
//			CCLog("@@@kTagItem5-out");
//			break;
//		default:
//			CCLog("@@@tag nasi %d",item->getTag());
//			return;
//			break;
//		}
//		int tag = item->getTag();
//		CCLog("removeFromParent tag=%d",tag);
//		item->removeFromParent();
////		itemsToDelete->removeObject(item);
//	}
//
//	CCLog("@@@updateItem-out");
////	CCLog("@@@release1@@@");
////	if (itemsToDelete->count() > 0) {
////		itemsToDelete->release();
////		CCLog("@@@release2@@@");
////	}
//}

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

	if ((double) rand() / RAND_MAX < itemRate) {
		CCSprite* item = CCSprite::create(fileName->getCString());
		if (item) {
			item->autorelease();
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
	m_score += 100;
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
	BallSprite* ball = BallSprite::createWithBallScale(0.7);
	CCNode *bar = this->getChildByTag(kTagBar);
	ball->setPosition(
			ccp(bar->getPositionX(), bar->getPositionY()+ bar->getContentSize().height));
	this->addChild(ball);
	m_activeballs->addObject(ball);
	m_ballRemain++;
}

void GameScene::onGetItem4() {
	m_score += 100;
	CCString* str = CCString::create("+5000");
	makeItemGetLabel(str);
	//ボーナススコアを加算する
	m_score += 5000;
}

void GameScene::onGetItem5() {
	m_score += 100;
	CCString* str = CCString::create("+1 Ball!");
	makeItemGetLabel(str);

	CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
	CCParticleMeteor* p = CCParticleMeteor::create();
	p->setTexture(CCTextureCache::sharedTextureCache()->addImage(PNG_P_YELLOW));
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

	this->unschedule(schedule_selector(GameScene::updateGame));

	setResultScores();

	// 現在のレベルの一つ上をアンロック
	if (selectedLevel < 16) {
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
	CCLabelBMFont* label = CCLabelBMFont::create("CLEAR!", FONT_BIG1);
	label->setPosition(
			ccp(g_visibleSize.width / 2, g_visibleSize.height * 0.5));
	label->runAction(
			Animations::gameClearAction(this,
					callfunc_selector(GameScene::showClearPopup)));
	addChild(label, kZOrderLabel);
}

void GameScene::showClearPopup(CCObject* sender) {
	// ポップアップを表示して確認
	GameClearPopup* pl = GameClearPopup::create();
	this->addChild(pl);
}

void GameScene::gameOver() {
	if (UserSettings::getSESetting())
		SimpleAudioEngine::sharedEngine()->playEffect(MP3_GAMEOVER);

	this->unschedule(schedule_selector(GameScene::updateGame));

	setResultScores();

	CCScene* scene = (CCScene*) GameOverScene::create();
	CCTransitionRotoZoom* tran = CCTransitionRotoZoom::create(3, scene);
	CCDirector::sharedDirector()->replaceScene(tran);
}

void GameScene::setResultScores() {
	UserSettings::setScore(getScore());
	if (g_LevelState[1][selectedLevel] < getScore()) {
		g_LevelState[1][selectedLevel] = getScore();
		UserSettings::saveLevelState();
	}
	UserSettings::setHighScore(g_LevelState[1][selectedLevel]);
}

void GameScene::makeBackButton() {
	CCMenuItemImage *item = CCMenuItemImage::create(PNG_BACK, PNG_BACK, this,
			menu_selector(GameScene::onTapBackButton));

	if (!item)
		return;
	item->setPosition(
			GHelper::convI720toCC(g_visibleSize.width * 0.08,
					g_visibleSize.height * 0.1));
	CCMenu* menu = CCMenu::create(item, NULL);
	menu->setPosition(CCPointZero);
	if (!menu)
		return;
	this->addChild(menu, kZOrderTop, kTagBack);
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

