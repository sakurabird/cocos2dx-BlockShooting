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

    // 画像が画面の高さにフィットして表示される設定
//    pDirector->setContentScaleFactor(768.0f / pDirector->getWinSize().height);

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
