#include "TopScene.h"
#include "GameScene.h"
#include "GHelper.h"

USING_NS_CC;


TopScene::~TopScene()
{
}

TopScene::TopScene()
{
}

CCScene* TopScene::scene()
{
    CCScene * scene = NULL;
    do
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        TopScene *layer = TopScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool TopScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    if (!CCLayerColor::initWithColor( ccc4(0,0,0,0) )) {
        return false;
    }

    makeLabel();

    return  true;
}

void TopScene::makeLabel()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCLabelBMFont* title = CCLabelBMFont::create("ブロックくずし", "title.fnt");
    title->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.8));
    addChild(title);

    CCLabelTTF* startLabel1 = CCLabelTTF::create("ゲームスタート!", "Arial", 45.0);
    startLabel1->setColor(ccc3(255,192,203));
    CCMenuItemLabel* item1 = CCMenuItemLabel::create(startLabel1, this, menu_selector(TopScene::tapStartButton));

    CCLabelTTF* startLabel2 = CCLabelTTF::create("Quit", "Arial", 45.0);
    startLabel1->setColor(ccc3(255,192,203));
    CCMenuItemLabel* item2 = CCMenuItemLabel::create(startLabel2, this, menu_selector(TopScene::menuCloseCallback));

    CCMenu* menu = CCMenu::create( item1, item2, NULL );
    menu->alignItemsVerticallyWithPadding(30.0);

    // elastic effect
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    int i=0;
    CCNode* child;
    CCArray * pArray = menu->getChildren();
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(pArray, pObject)
    {
        if(pObject == NULL)
            break;

        child = (CCNode*)pObject;

        CCPoint dstPoint = child->getPosition();
        int offset = (int) (s.width/2 + 50);
        if( i % 2 == 0)
            offset = -offset;

        child->setPosition( ccp( dstPoint.x + offset, dstPoint.y) );
        child->runAction(
                         CCEaseElasticOut::create(CCMoveBy::create(2, ccp(dstPoint.x - offset,0)), 0.35f)
                         );
        i++;
    }

    addChild( menu );
}

void TopScene::tapStartButton()
{
    CCScene* scene = (CCScene*)GameScene::create();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void TopScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");

#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
