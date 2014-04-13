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
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);

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


    //テクスチャアトラスを使用する
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

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
