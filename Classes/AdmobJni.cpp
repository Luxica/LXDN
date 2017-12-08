
#include "WxSocialEntity.h"
#include "../Classes/DN/Scene/HallScene/QHallScene.h"
#include "../Classes/DN/Scene/GameScene/data/QGameManager.h"

#include "AdmobJni.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include "cocos2d.h"
#include "GameConfig.h"

using namespace cocos2d;

std::string andriod_java_path1[30] = { "",
									"com/yskj/kxnnl/WxSocialUtil"
};

std::string andriod_java_path2[30] = {"",
									"com/yskj/kxnnl/GameActivity"
};

std::string andriod_java_path3[30] = {"",
									"com/yskj/kxnnl/GameJniHelper"
};

JNIEXPORT void JNICALL Java_com_yskj_dngame_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_dngame_GameActivity_thirdPayResultCallBack(
                                                                                      JNIEnv *env, jclass clazz, jint errCode)
{
    AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_dngame_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
	
}

JNIEXPORT void JNICALL Java_com_yskj_ttdngame_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                      JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
    const char* pToken = env->GetStringUTFChars(code, NULL);
    AdmobJni::theAuthorizeCB(errCode, pToken);
}


JNIEXPORT void JNICALL Java_com_yskj_ttdngame_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                       JNIEnv *env, jclass clazz, jint errCode)
{
    AdmobJni::theInviteGameCB(errCode);
}


JNIEXPORT void JNICALL Java_com_yskj_ttyx_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                      JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
    const char* pToken = env->GetStringUTFChars(code, NULL);
    AdmobJni::theAuthorizeCB(errCode, pToken);
}


JNIEXPORT void JNICALL Java_com_yskj_ttyx_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                       JNIEnv *env, jclass clazz, jint errCode)
{
    AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_lmdn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                    JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
    const char* pToken = env->GetStringUTFChars(code, NULL);
    AdmobJni::theAuthorizeCB(errCode, pToken);
}


JNIEXPORT void JNICALL Java_com_yskj_lmdn_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                     JNIEnv *env, jclass clazz, jint errCode)
{
    AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_xrnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                      JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
    const char* pToken = env->GetStringUTFChars(code, NULL);
    AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_xrnn_GameActivity_thirdPayResultCallBack(
                                                                                JNIEnv *env, jclass clazz, jint errCode)
{
    AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_xrnn_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                       JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_ctnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                    JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
    const char* pToken = env->GetStringUTFChars(code, NULL);
    AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_ctnn_GameActivity_thirdPayResultCallBack(
                                                                               JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_ctnn_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                     JNIEnv *env, jclass clazz, jint errCode)
{
    AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_dddn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_dddn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_dddn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_dznn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_dznn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_dznn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_hldn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_hldn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_hldn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_htnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_htnn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_htnn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_kxdn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_kxdn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_kxdn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_xlnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_xlnn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_xlnn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_xxnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_xxnn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_xxnn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_jjnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_jjnn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_jjnn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_fsnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_fsnn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_fsnn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}
JNIEXPORT void JNICALL Java_com_yskj_hsnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	if (errCode == 321456) {
		QGameManager::getInstance()->setGotoRoomId(pToken);

		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("EVENT_GOTO_ROOM");

		CCLog("goto room id %s", pToken);
	} else {
		AdmobJni::theAuthorizeCB(errCode, pToken);
	}
}

JNIEXPORT void JNICALL Java_com_yskj_hsnn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_dsnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{	
	const char* pToken = env->GetStringUTFChars(code, NULL);

	if (errCode == 321456) {
		QGameManager::getInstance()->setGotoRoomId(pToken);

		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("EVENT_GOTO_ROOM");

		CCLog("goto room id %s", pToken);
	} else {
		AdmobJni::theAuthorizeCB(errCode, pToken);
	}
}

JNIEXPORT void JNICALL Java_com_yskj_dsnn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_dsnn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_wcnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{	
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_wcnn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_wcnn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_csnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_csnn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_csnn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_nzt_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_nzt_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_nzt_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_fkdn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_fkdn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_fkdn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_ddpk_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_ddpk_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_ddpk_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}
JNIEXPORT void JNICALL Java_com_yskj_njz_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_njz_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_njz_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_hynn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_hynn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_hynn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_ttdn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_ttdn_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_ttdn_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_kxnnl_wxapi_WXEntryActivity_WxAuthLoginCallBack(
	JNIEnv *env, jclass clazz, jint errCode, jstring code)
{
	const char* pToken = env->GetStringUTFChars(code, NULL);
	AdmobJni::theAuthorizeCB(errCode, pToken);
}

JNIEXPORT void JNICALL Java_com_yskj_kxnnl_GameActivity_thirdPayResultCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::m_thirdPayCallBack((int)errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_kxnnl_wxapi_WXEntryActivity_InviteToGameCallBack(
	JNIEnv *env, jclass clazz, jint errCode)
{
	AdmobJni::theInviteGameCB(errCode);
}

JNIEXPORT void JNICALL Java_com_yskj_kxnnl_GameActivity_LocationCallBack(
	JNIEnv *env, jclass clazz, jdouble latitude,jdouble Longitude,jstring code)
{
	const char* address = env->GetStringUTFChars(code, NULL);
	AdmobJni::m_locaTionCallBack(latitude,Longitude,address);
}

JNICM AdmobJni::getStaticMethod(const char *classname, const char *methodname, const char *sig)
{
	JNICM tjnicm;
	tjnicm.env = nullptr;
	tjnicm.classID = nullptr;
	tjnicm.methodID = nullptr;
	JavaVM* jVM = JniHelper::getJavaVM();
	jVM->AttachCurrentThread(&tjnicm.env, nullptr);
	if (tjnicm.env)
	{
		tjnicm.classID = tjnicm.env->FindClass(classname);
		jthrowable expClass = tjnicm.env->ExceptionOccurred();
		if (expClass)
		{
			tjnicm.env->ExceptionClear();
		}

		if (tjnicm.classID)
		{
			tjnicm.methodID = tjnicm.env->GetStaticMethodID(tjnicm.classID, methodname, sig);
			jthrowable expMethod = tjnicm.env->ExceptionOccurred();
			if (expMethod)
			{
				tjnicm.env->ExceptionClear();
			}
		}
	}
	return tjnicm;
}

JNICM AdmobJni::getObjectMethod(jobject jobj, const char *methodname, const char *sig)
{
	JNICM tjnicm;
	tjnicm.env = nullptr;
	tjnicm.classID = nullptr;
	tjnicm.methodID = nullptr;
	JavaVM* jVM = JniHelper::getJavaVM();
	jVM->AttachCurrentThread(&tjnicm.env, nullptr);
	if (tjnicm.env)
	{
		tjnicm.classID = tjnicm.env->GetObjectClass(jobj);
		jthrowable expClass = tjnicm.env->ExceptionOccurred();
		if (expClass)
		{
			tjnicm.env->ExceptionClear();
		}

		if (tjnicm.classID)
		{
			tjnicm.methodID = tjnicm.env->GetStaticMethodID(tjnicm.classID, methodname, sig);
			jthrowable expMethod = tjnicm.env->ExceptionOccurred();
			if (expMethod)
			{
				tjnicm.env->ExceptionClear();
			}
		}
	}
	return tjnicm;
}

void AdmobJni::callWxAuthLogin(WxAuthorizeCallBack callback)
{
	JniMethodInfo mi;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path1[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(mi,
		path.c_str(), "authLogin", "()V");

	log("############   isHave:%d", isHave);

	if (isHave)
	{
		theAuthorizeCB = callback;
		mi.env->CallStaticVoidMethod(mi.classID, mi.methodID);
		mi.env->DeleteLocalRef(mi.classID);
	}
}

void AdmobJni::onStartLocation(std::function<void(double x, double y, const char* sAddress)> callback)
{
	JniMethodInfo mi;
	int gameType = GAMECONFIG.m_nAppType;
	std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(mi,
		path.c_str(), "locationStart", "()V");

	log("############   isHave:%d", isHave);

	if (isHave)
	{
		m_locaTionCallBack = callback;

		mi.env->CallStaticVoidMethod(mi.classID, mi.methodID);
		mi.env->DeleteLocalRef(mi.classID);
	}
}

std::string AdmobJni::onStopLocation()
{
	std::string str;
	JniMethodInfo minfo;
	int gameType = GAMECONFIG.m_nAppType;
	std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "getAddr", "()Ljava/lang/String;");
	if (isHave)
	{
		jstring js = jstring(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
		str = JniHelper::jstring2string(js);
		minfo.env->DeleteLocalRef(minfo.classID);
	}

	return str;
}

void AdmobJni::callWxInviteGame(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback)
{
	JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path1[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		path.c_str(), "shareText", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

	log("############ AdmobJni::callWxInviteGame  isHave:%d", isHave);

	if (isHave)
	{
		theInviteGameCB = callback;

		jstring jarg0 = minfo.env->NewStringUTF(webpageUrl);
		jstring jarg1 = minfo.env->NewStringUTF(title);
		jstring jarg2 = minfo.env->NewStringUTF(description);

		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jarg0, jarg1, jarg2);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}
void AdmobJni::callWxInviteGameTo(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback)
{
	JniMethodInfo minfo;
	int gameType = GAMECONFIG.m_nAppType;
	std::string path = andriod_java_path1[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		path.c_str(), "shareText1", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

	log("############ AdmobJni::callWxInviteGameTo  isHave:%d", isHave);

	if (isHave)
	{
		theInviteGameCB = callback;

		jstring jarg0 = minfo.env->NewStringUTF(webpageUrl);
		jstring jarg1 = minfo.env->NewStringUTF(title);
		jstring jarg2 = minfo.env->NewStringUTF(description);

		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jarg0, jarg1, jarg2);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}
void AdmobJni::thirdPay(int type,int index,int userId,std::function<void(int code)> callback)
{
    JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path2[gameType];
    bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                 path.c_str(), "sdkPay", "(III)V");
    
    log("############   isHave:%d", isHave);
    
    if (isHave)
    {
        m_thirdPayCallBack = callback;
        
        jint jarg0 = type;
        jint jarg1 = index;
        jint jarg2 = userId;
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jarg0, jarg1, jarg2);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void AdmobJni::thirdPay11(int type,int index,int userId,std::function<void(int code)> callback)
{
    JniMethodInfo minfo;

    
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/yskj/dngame/GameActivity","getInstance","()Ljava/lang/Object;");
    
    log("############   isHave:%d", isHave);
    
    if (isHave)
    {
        m_thirdPayCallBack = callback;
        log("getInstance方法已经找到，准备调用......");
        jobject jobj = minfo.env->CallStaticObjectMethod(minfo.classID,minfo.methodID);
        isHave = JniHelper::getMethodInfo(minfo,"com/yskj/dngame/GameActivity","sdkPay","(III)V");
        if(!isHave)
        {
            log("sdkPay此方法不存在!");
        }
        else
        {
            CCLog("sdkPay已经找到");
            //
            jint jarg0 = type;
            jint jarg1 = index;
            jint jarg2 = userId;
            minfo.env->CallVoidMethod(jobj, minfo.methodID,jarg0, jarg1, jarg2);
            minfo.env->DeleteLocalRef(minfo.classID);
            log("sdkPay函数执行完毕");
        }
    }
}

void AdmobJni::callWxShare(const char* imgPath, const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback)
{
	JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path1[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		path.c_str(), "shareImg", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

	log("############   isHave:%d", isHave);

	if (isHave)
	{
		theInviteGameCB = callback;

		jstring jarg0 = minfo.env->NewStringUTF(webpageUrl);
		jstring jarg1 = minfo.env->NewStringUTF(title);
		jstring jarg2 = minfo.env->NewStringUTF(description);
		jstring jarg3 = minfo.env->NewStringUTF(imgPath);

		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jarg0, jarg1, jarg2, jarg3);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}

void AdmobJni::exitGame()
{
	JniMethodInfo mi;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(mi,
		path.c_str(), "exitGame", "()V");

	log("############   isHave:%d", isHave);

	if (isHave)
	{
		mi.env->CallStaticVoidMethod(mi.classID, mi.methodID);
		mi.env->DeleteLocalRef(mi.classID);
	}
}


void AdmobJni::startRecord()
{
	log("### DEBUG");
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path2[gameType];
	JNICM jnicm = getStaticMethod(path.c_str(), "startRecord",
		"()V");
	log("### DEBUG");
	if (!jnicm.vaild())
		return;

	log("### DEBUG");
	jnicm.env->CallStaticVoidMethod(
		jnicm.classID,
		jnicm.methodID
		);

	log("### DEBUG");

	jnicm.env->DeleteLocalRef(jnicm.classID);
}


void AdmobJni::cancelRecord()
{
	JniMethodInfo mi;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(mi,
		path.c_str(), "cancleRecord", "()V");

	log("############   isHave:%d", isHave);

	if (isHave)
	{
		mi.env->CallStaticVoidMethod(mi.classID, mi.methodID);
		mi.env->DeleteLocalRef(mi.classID);
	}
}

std::string AdmobJni::stopRecord()
{
	std::string str;
	JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "stopRecordAndReturn", "()Ljava/lang/String;");
	if (isHave)
	{
		jstring js = jstring(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
		str = JniHelper::jstring2string(js);
		minfo.env->DeleteLocalRef(minfo.classID);
	}

	return str;
}


std::string AdmobJni::getAppVersion()
{
	std::string str;
	JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path3[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "getVersion", "()Ljava/lang/String;");
	if (isHave)
	{
		jstring js = jstring(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
		str = JniHelper::jstring2string(js);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	return str;
}


int AdmobJni::getVoiceDb()
{
	jint db;
	JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "getVoiceDb", "()I");
	if (isHave)
	{
		db = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
	}

	return db;
}


void AdmobJni::playRecord(const char* data, int size)
{
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path2[gameType];
	JNICM jnicm = getStaticMethod(path.c_str(), "playRecord",
		"([B)V");
	if (!jnicm.vaild())
	{
		log("########## DEBUG ");
		return;
	}

	jbyteArray result;

	result = jnicm.env->NewByteArray(size);

	log("########## DEBUG ");

	jnicm.env->SetByteArrayRegion(result, 0, size, (const jbyte*)data);

	log("########## DEBUG ");

	jnicm.env->CallStaticVoidMethod(
		jnicm.classID,
		jnicm.methodID,
		result);

	jnicm.env->DeleteLocalRef(result);
	jnicm.env->DeleteLocalRef(jnicm.classID);

	log("########## DEBUG ");
}

WxAuthorizeCallBack AdmobJni::theAuthorizeCB = NULL;
std::function<void(int code)> AdmobJni::m_thirdPayCallBack = NULL;
std::function<void(double x, double y, const char* sAddress)> AdmobJni::m_locaTionCallBack=NULL;

WxInviteGameCallBack AdmobJni::theInviteGameCB = NULL;

std::string AdmobJni::getLogFilePath()
{
	std::string str;
	JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path3[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "getLogFilePath", "()Ljava/lang/String;");
	if (isHave)
	{
		log("###################    getLogFilePath  true");
		jstring js = jstring(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
		str = JniHelper::jstring2string(js);
		minfo.env->DeleteLocalRef(minfo.classID);

		log("############## getLogFilePath : %s", str.c_str());
	}

	return str;
}


std::string AdmobJni::getPhoneUUID()
{
	std::string str;
	JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path3[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "getSerialNumber", "()Ljava/lang/String;");
	if (isHave)
	{
		log("###################    getSerialNumber  true");
		jstring js = jstring(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
		str = JniHelper::jstring2string(js);
		minfo.env->DeleteLocalRef(minfo.classID);

		log("############## getSerialNumber : %s", str.c_str());
	}

	return str;
}


std::string AdmobJni::getNetconType()
{
    std::string str;
    JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path3[gameType];
    bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "getPhoneNetStates", "()Ljava/lang/String;");
    if (isHave)
    {
        log("###################    getPhoneNetStates  true");
        jstring js = jstring(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
        str = JniHelper::jstring2string(js);
        minfo.env->DeleteLocalRef(minfo.classID);
        
        log("############## getPhoneNetStates : %s", str.c_str());
    }
    
    return str;
}


int AdmobJni::getBatteryValue()
{
	jint batteryValue = 100;
	JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path3[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "getBatteryLevel", "()I");
	if (isHave)
	{
		batteryValue = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
		log("############## getBatteryLevel : %d", batteryValue);
	}

	return batteryValue;
}

bool AdmobJni::isWXInstall()
{
	jboolean isInstallWx;
	JniMethodInfo minfo;
    int gameType = GAMECONFIG.m_nAppType;
    std::string path = andriod_java_path1[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "isWxInstall", "()Z");
	if (isHave)
	{
		isInstallWx = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
	}

	return isInstallWx;
}
//
int AdmobJni::copyToClipboard(const char* paras) 
{
	JniMethodInfo minfo;
	int gameType = GAMECONFIG.m_nAppType;
	std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo,  path.c_str(),"copyToClipboard", "(Ljava/lang/String;)I");
	if (isHave)
	{
		jstring jstr = minfo.env->NewStringUTF(paras);

		int code = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID, jstr);
		return code;
	}
	return -1;
}

std::string AdmobJni::pasteFromClipboard()
{
	std::string str;
	JniMethodInfo minfo;
	int gameType = GAMECONFIG.m_nAppType;
	std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "pasteFromClipboard", "()Ljava/lang/String;");
	if (isHave)
	{
		jstring js = jstring(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
		str = JniHelper::jstring2string(js);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	return str;
}

// wifi
int AdmobJni::getWifiLevel() {
	jint value = 0;
	JniMethodInfo minfo;
	int gameType = GAMECONFIG.m_nAppType;
	std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "getSingnalLevel", "()I");
	if (isHave)
	{
		value = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	return value;
}
// 电池
int AdmobJni::getBatteryLevel() {
	jint value = 0;
	JniMethodInfo minfo;
	int gameType = GAMECONFIG.m_nAppType;
	std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "getBatteryLevel", "()I");
	if (isHave) {
		value = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	return value;
}

bool AdmobJni::isConnectionAvailable() {
	jboolean isInstallWx;
	JniMethodInfo minfo;
	int gameType = GAMECONFIG.m_nAppType;
	std::string path = andriod_java_path2[gameType];
	bool isHave = JniHelper::getStaticMethodInfo(minfo, path.c_str(), "isNetworkConnected", "()Z");
	if (isHave) {
		isInstallWx = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	return isInstallWx;
}

#endif
