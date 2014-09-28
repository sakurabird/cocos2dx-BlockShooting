//
//  MyRenderer.cpp
//  HelloCpp
//
//  Created by 椋梨 健太郎 on 2013/07/11.
//
//
#include "MyRenderer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

#define CLASS_NAME "com/sakurafish/cocos2dx/brockshooting/CallFromCPP"

USING_NS_CC;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
	//JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_SimpleGame_nativeEnd(JNIEnv* env, jobject thiz)
	JNIEXPORT void JNICALL Java_com_terasurware_theia_theia222_nativeEnd(JNIEnv* env, jobject thiz)
	{
	    CCLOG("SimpleGame_nativeEnd");
		CCDirector::sharedDirector()->end();
	}

	JNIEXPORT void JNICALL Java_com_terasurware_theia_theia222_jinonConsumeFinished(JNIEnv* env, jobject thiz, jint flag)
	{
	    CCLOG("jinonConsumeFinished flag=%d", flag);
		//CCDirector::sharedDirector()->end();
	    MyRenderer::activate(flag);	//シーンのコールバックメソッドを実行する
	}
}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//Android
//call Java
void MyRenderer::showAd(int n){
    CCLOG("MyRenderer::showAd %d",n);
    JniMethodInfo methodInfo;

    if (JniHelper::getStaticMethodInfo(methodInfo
                                       , CLASS_NAME
                                       , "showAd"	//
                                       , "(I)V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, n);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

#else
//iPhone

void MyRenderer::showAd(int n){
    
}
#endif

//
//実体の宣言
CCObject*       MyRenderer::m_pListener;
SEL_callbackHandler    MyRenderer::m_pfnSelector;

void MyRenderer::regist(CCObject* listene, SEL_callbackHandler selector){
    CCLOG("MyRenderer::regist");

	m_pListener = listene;
	m_pfnSelector = selector;
}

//JavaからC++を呼ぶ
void MyRenderer::activate(int flag)
{
    CCLOG("MyRenderer::activate1 flag=%d",flag);

	if (m_pListener && m_pfnSelector)
	{
	    CCLOG("MyRenderer::activate2");
		(m_pListener->*m_pfnSelector)(flag);
	}
    CCLOG("MyRenderer::activate3");
}



void MyRenderer::end(){
    CCLOG("MyRenderer::end");

}

