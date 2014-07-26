//
//  GameClearPopup.cpp
//  BrockShooting
//
//  Created by sakura on 2014/07/24.
//
//

#include "GameClearPopup.h"
#include "Config.h"
#include "TopScene.h"
#include "GameScene.h"
#include "Utils.h"

USING_NS_CC;

#define kAnimationTime 0.4f

GameClearPopup::GameClearPopup():
m__pMenu(NULL)
//, m_contentPadding(0)
//, m_contentPaddingTop(0)
, m_callbackListener(NULL)
, m_callback(NULL)
, m__sfBackGround(NULL)
//, m__ltContentText(NULL)
//, m__ltTitle(NULL)
{
}

GameClearPopup::~GameClearPopup(){
    CC_SAFE_RELEASE(m__pMenu);
    CC_SAFE_RELEASE(m__sfBackGround);
//    CC_SAFE_RELEASE(m__ltContentText);
//    CC_SAFE_RELEASE(m__ltTitle);
}


//GameClearPopup* GameClearPopup::create(const char *backgroundImage){
//    GameClearPopup* ml = GameClearPopup::create();
//    ml->setSpriteBackGround(CCSprite::create(backgroundImage));
////    ml->setPosition(CCPointZero);
//    return ml;
//}

bool GameClearPopup::init()
{
    if (!CCLayer::init()) {
        return false;
    }

//    setSpriteBackGround(CCSprite::create(PNG_POPUP_BG2));
//    setContentSize(CCSizeMake(g_visibleSize.width * 0.7, 400));
//    setPosition(ccp(g_visibleSize.width / 2, g_visibleSize.height / 2));
//    this->addChild(getSpriteBackGround(), 0, 0);

//    CCMenu* menu = CCMenu::create();
//    menu->setPosition(CCPointZero);
//    setMenuButton(menu);

    makeLabels();

    setKeypadEnabled(true);

    return  true;
}

void GameClearPopup::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

void GameClearPopup::makeLabels()
{
    CCLabelBMFont* title = CCLabelBMFont::create("You Win!", FONT_BLUE);
    title->setScale(0.6);
    title->setPosition( ccp(g_visibleSize.width / 2, g_visibleSize.height * 0.8));
    this->addChild(title);

    //Next Stage?
    CCLabelBMFont* disc1 = CCLabelBMFont::create("Next Stage?", FONT_GREEN);
    disc1->setScale(0.5);
    disc1->setAnchorPoint(CCPointZero);
//    disc1->setPosition(ccp(g_visibleSize.width * 0.2, g_visibleSize.height * 0.65));
//    this->addChild(disc1);
    CCMenuItemLabel* item1 = CCMenuItemLabel::create(disc1, this, menu_selector(GameClearPopup::onTapNextStage));

    //Again?
    CCLabelBMFont* disc2 = CCLabelBMFont::create("Again?", FONT_WHITE);
    disc2->setScale(0.5);
    disc2->setAnchorPoint(CCPointZero);
//    disc2->setPosition(ccp(g_visibleSize.width * 0.2, g_visibleSize.height * 0.65));
//    this->addChild(disc2);
    CCMenuItemLabel* item2 = CCMenuItemLabel::create(disc2, this, menu_selector(GameClearPopup::onTapAgain));

    //Menu
    CCLabelBMFont* disc3 = CCLabelBMFont::create("Menu", FONT_WHITE);
    disc3->setScale(0.5);
    disc3->setAnchorPoint(CCPointZero);
    //    disc2->setPosition(ccp(g_visibleSize.width * 0.2, g_visibleSize.height * 0.65));
//    this->addChild(disc3);
    CCMenuItemLabel* item3 = CCMenuItemLabel::create(disc3, this, menu_selector(GameClearPopup::onTapMenu));

    //Quit
    CCLabelBMFont* disc4 = CCLabelBMFont::create("Quit", FONT_WHITE);
    disc4->setScale(0.5);
    disc4->setAnchorPoint(CCPointZero);
    //    disc2->setPosition(ccp(g_visibleSize.width * 0.2, g_visibleSize.height * 0.65));
//    this->addChild(disc4);
    CCMenuItemLabel* item4 = CCMenuItemLabel::create(disc4, this, menu_selector(GameClearPopup::onTapQuit));

    CCMenu* menu = CCMenu::create( item1, item2, item3, item4, NULL );
    menu->alignItemsVerticallyWithPadding(5.0);
    this->addChild(menu);
}

void GameClearPopup::onTapNextStage(){
    CCScene* scene = (CCScene*)GameScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
    this->removeFromParent();
}

void GameClearPopup::onTapAgain(){
    CCScene* scene = (CCScene*)GameScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
    this->removeFromParent();
}

void GameClearPopup::onTapMenu()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
    this->removeFromParent();
}

void GameClearPopup::onTapQuit()
{
    Utils::quit();
    this->removeFromParent();
}

void GameClearPopup::keyBackClicked()
{
    this->removeFromParent();
}

bool GameClearPopup::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    //
    CCLog("GameClearPopup touch");
    return true;
}

//void GameClearPopup::setContentText(const char *text, int fontsize, int padding, int paddingTop){
//    CCLabelTTF* ltf = CCLabelTTF::create(text, "", fontsize);
//    ltf->setColor(ccc3(0, 0, 0));
//    setLabelContentText(ltf);
//    m_contentPadding = padding;
//    m_contentPaddingTop = paddingTop;
//}

//void GameClearPopup::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun){
//    m_callbackListener = target;
//    m_callback = callfun;
//}


//bool GameClearPopup::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag){
//    CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage, this, menu_selector(GameClearPopup::buttonCallback));
//    menuImage->setScale(2.0f);
//    menuImage->setTag(tag);
//    menuImage->setPosition(CCPointZero);
//
//    // 添加文字说明并设置位置
//    CCSize imenu = menuImage->getContentSize();
//    CCLabelTTF* ttf = CCLabelTTF::create(title, "", 20);
//    ttf->setColor(ccc3(0, 0, 0));
//    ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
//    menuImage->addChild(ttf);
//
//    getMenuButton()->addChild(menuImage);
//    return true;
//}

//void GameClearPopup::buttonCallback(cocos2d::CCObject *pSender){
//    CCNode* node = dynamic_cast<CCNode*>(pSender);
//    //    CCLog("touch tag: %d", node->getTag());
//    if (m_callback && m_callbackListener){
//        (m_callbackListener->*m_callback)(node);
//    }
//    this->removeFromParent();
//}

void GameClearPopup::onEnter(){
    CCLayer::onEnter();
    setSpriteBackGround(CCSprite::create(PNG_POPUP_BG2));
    getSpriteBackGround()->setPosition(ccp(g_visibleSize.width / 2, g_visibleSize.height / 2));
    setContentSize(CCSizeMake(g_visibleSize.width * 0.7, 400));
//    setPosition(ccp(g_visibleSize.width / 2, g_visibleSize.height / 2));
    this->addChild(getSpriteBackGround(), 0, 0);
}
//void GameClearPopup::onEnter(){
//    CCLayer::onEnter();
//
//    CCSize contentSize;
//    getSpriteBackGround()->setPosition(ccp(g_visibleSize.width / 2, g_visibleSize.height / 2));
//    this->addChild(getSpriteBackGround(), 0, 0);
//    contentSize = getSpriteBackGround()->getTexture()->getContentSize();

//    this->addChild(getMenuButton());
//    float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount() + 1);
//
//    CCArray* array = getMenuButton()->getChildren();
//    CCObject* pObj = NULL;
//    int i = 0;
//    CCARRAY_FOREACH(array, pObj){
//        CCNode* node = dynamic_cast<CCNode*>(pObj);
//        node->setPosition(ccp( winSize.width / 2 - contentSize.width / 2 + btnWidth * (i + 1), winSize.height / 2 - contentSize.height / 3));
//        i++;
//    }
//
//
//    // 显示对话框标题
//    if (getLabelTitle()){
//        getLabelTitle()->setPosition(ccpAdd(pCenter, ccp(0, contentSize.height / 2 - 35.0f)));
//        this->addChild(getLabelTitle());
//    }
//
//    // 显示文本内容
//    if (getLabelContentText()){
//        CCLabelTTF* ltf = getLabelContentText();
//        ltf->setPosition(ccp(winSize.width / 2, winSize.height / 2));
//        ltf->setDimensions(CCSizeMake(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop));
//        ltf->setHorizontalAlignment(kCCTextAlignmentLeft);
//        this->addChild(ltf);
//    }
//
//    // 弹出效果
//    CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
//                                              CCScaleTo::create(0.06, 1.05),
//                                              CCScaleTo::create(0.08, 0.95),
//                                              CCScaleTo::create(0.08, 1.0), NULL);
//    this->runAction(popupLayer);
//}
