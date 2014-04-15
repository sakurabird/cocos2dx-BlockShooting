#include "AppDelegate.h"
#include "AppMacros.h"
#include "TopScene.h"
#include "GHelper.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {

    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView *pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    //デザインサイズの設定
    CCLOG("applicationDidFinishLaunching1");
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionFixedWidth);

    //検証用
//    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
//    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
//
//    CCLOG("AppDelegate designResolutionSize.width: %f, height: %f",designResolutionSize.width,designResolutionSize.height);
//    CCLOG("AppDelegate visibleSize.width: %f, height: %f",visibleSize.width,visibleSize.height);
//    CCLOG("AppDelegate origin.x: %f, origin.y: %f",origin.x,origin.y);

    CCSize flameSize = pEGLView->getFrameSize();

    std::vector<std::string> searchPaths;

    if (flameSize.height > sdResource.size.height) {
        searchPaths.push_back(hdResource.directory);
        pDirector->setContentScaleFactor(MIN(hdResource.size.height / designResolutionSize.height,
                                             hdResource.size.width / designResolutionSize.width));
    }else{
        searchPaths.push_back(sdResource.directory);
        pDirector->setContentScaleFactor(MIN(sdResource.size.height / designResolutionSize.height,
                                             sdResource.size.width / designResolutionSize.width));
    }

    //リソースディレクトリを指定
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);


    //テクスチャアトラスを使用する あとでやる
//    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
//    frameCache->addSpriteFramesWithFile("simpleGame.plist");


    // turn on display FPS
    pDirector->setDisplayStats(true);

    pDirector->setAnimationInterval(1.0 / 60);

    CCScene *pScene = TopScene::scene();

    //椋梨さんの作られたGHelper
    GHelper::init();
    
    // run
    pDirector->runWithScene(pScene);
    
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
