#pragma once

#include <string>
#include "WxSocialEntity.h"
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>


#endif

using namespace cocos2d;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

struct JNICM {
	JNIEnv* env;
	jclass 	classID;
	jmethodID methodID;

	bool vaild()
	{
		return env != nullptr;
	}

};

class AdmobJni
{
public:
    
	static std::string getLogFilePath();

	static std::string getPhoneUUID();
    
    /**
     * 获取手机信号
     */
    static std::string getNetconType();

	static int getBatteryValue();

	static bool isWXInstall();

    /**
     *  第三方支付
     * @ type 类型 1 微信 2 支付宝 3 百度 4 充值卡
     */
    static void thirdPay(int type,int index,int userId,std::function<void(int code)> callback);
    
    static void thirdPay11(int type,int index,int userId,std::function<void(int code)> callback);
    
	static void callWxAuthLogin(WxAuthorizeCallBack callback);

	//定位
	static void onStartLocation(std::function<void(double x, double y, const char* sAddress)> callback);
	static std::string onStopLocation();

	static void callWxInviteGame(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback);
	//分享到朋友圈
	static void callWxInviteGameTo(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback);

	static void callWxShare(const char* imgPath, const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback);

	static void exitGame();

	static void startRecord();

	static void cancelRecord();

	static std::string stopRecord();

	static int getVoiceDb();

	static void playRecord(const char* data, int size);

	static WxAuthorizeCallBack theAuthorizeCB;

	static WxInviteGameCallBack theInviteGameCB;
    
    static std::function<void(int code)> m_thirdPayCallBack;

	static std::function<void(double x, double y, const char* sAddress)> m_locaTionCallBack;
    
	static std::string getAppVersion();

	// wifi信号强度
	static int getWifiLevel();
	// 电池电量
	static int getBatteryLevel();

	static bool isConnectionAvailable();

	//复制到剪切板
	static int copyToClipboard(const char* paras);

	//粘贴
	static std::string pasteFromClipboard();

private:

	static JNICM getStaticMethod(const char *classname, const char *methodname, const char *sig);

	static JNICM getObjectMethod(jobject jobj, const char *methodname, const char *sig);

};

#ifdef __cplusplus
extern "C"
{
	JNIEXPORT void JNICALL Java_com_yskj_dngame_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);
    
    JNIEXPORT void JNICALL Java_com_yskj_dngame_GameActivity_thirdPayResultCallBack(
                                                                                          JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_dngame_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);
    
    JNIEXPORT void JNICALL Java_com_yskj_ttdngame_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                          JNIEnv *, jclass, jint, jstring);
    
    JNIEXPORT void JNICALL Java_com_yskj_ttdngame_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                           JNIEnv *, jclass, jint);
    
    JNIEXPORT void JNICALL Java_com_yskj_ttyx_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                        JNIEnv *, jclass, jint, jstring);
    
    JNIEXPORT void JNICALL Java_com_yskj_ttyx_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                         JNIEnv *, jclass, jint);
    JNIEXPORT void JNICALL Java_com_yskj_lmdn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                        JNIEnv *, jclass, jint, jstring);
    
    JNIEXPORT void JNICALL Java_com_yskj_lmdn_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                         JNIEnv *, jclass, jint);
    
    JNIEXPORT void JNICALL Java_com_yskj_xrnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                          JNIEnv *, jclass, jint, jstring);
    
    JNIEXPORT void JNICALL Java_com_yskj_xrnn_GameActivity_thirdPayResultCallBack(
                                                                                    JNIEnv *, jclass, jint);
    
    JNIEXPORT void JNICALL Java_com_yskj_xrnn_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                           JNIEnv *, jclass, jint);

    JNIEXPORT void JNICALL Java_com_yskj_ctnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
                                                                                        JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_ctnn_GameActivity_thirdPayResultCallBack(
                                                                                    JNIEnv *, jclass, jint);
    
    JNIEXPORT void JNICALL Java_com_yskj_ctnn_wxapi_WXEntryActivity_InviteToGameCallBack(
                                                                                         JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_dddn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_dddn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_dddn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);
    
	JNIEXPORT void JNICALL Java_com_yskj_dznn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_dznn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_dznn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_hldn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_hldn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_hldn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_htnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_htnn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_htnn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_kxdn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_kxdn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_kxdn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_xlnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_xlnn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_xlnn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_xxnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_xxnn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_xxnn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_jjnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_jjnn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_jjnn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_fsnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_fsnn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_fsnn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_dsnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_dsnn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_dsnn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);


	JNIEXPORT void JNICALL Java_com_yskj_htnn_wxapi_GameActivity_gotoRoomCallBack (
		JNIEnv *, jclass, jstring);


	JNIEXPORT void JNICALL Java_com_yskj_hsnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_hsnn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_wcnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_wcnn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_wcnn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_csnn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_csnn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_csnn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_nzt_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_nzt_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_nzt_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_fkdn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_fkdn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_fkdn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_ddpk_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_ddpk_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_ddpk_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_njz_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_njz_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_njz_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_hynn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_hynn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_hynn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_ttdn_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_ttdn_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_ttdn_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_kxnnl_wxapi_WXEntryActivity_WxAuthLoginCallBack(
		JNIEnv *, jclass, jint, jstring);

	JNIEXPORT void JNICALL Java_com_yskj_kxnnl_GameActivity_thirdPayResultCallBack(
		JNIEnv *, jclass, jint);

	JNIEXPORT void JNICALL Java_com_yskj_kxnnl_wxapi_WXEntryActivity_InviteToGameCallBack(
		JNIEnv *, jclass, jint);

	//GPS定位回调
	JNIEXPORT void JNICALL Java_com_yskj_kxnnl_GameActivity_LocationCallBack
		(JNIEnv *, jclass, jdouble, jdouble, jstring);
}
#endif


#endif
