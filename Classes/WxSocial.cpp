#include "cocos2d.h"
#include "network/HttpClient.h"

#include "WxSocial.h"
#include "DN/Scene/LoginScene/data/QLoginManager.h"
#include "ScriptDataManager.h"
#include "json/document.h"
#include "AdmobJni.h"
#include "GameConfig.h"
//#include "QHallManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSAdapterUtility.hpp"
#endif


USING_NS_CC;

#define WX_REFRESH_EXPIRES_IN				2592000

#define WX_OPEN_ID							"weChat_open_id"
#define WX_ACCESS_TOKEN						"weChat_access_token"
#define WX_TOKEN_ACCESS_TIME				"weChat_token_access_time"
#define WX_REFRESH_TOKEN_ACCESS_TIME		"weChat_refresh_token_access_time"
#define WX_EXPIRES_IN						"weChat_expires_in"
#define WX_REFRESH_TOKEN					"weChat_refresh_token"
#define WX_SCOPE							"weChat_scope"

WxSocial::WxSocial()
{

}

WxSocial * WxSocial::theInstance = NULL;

WxSocial* WxSocial::getInstance()
{
	if (!theInstance)
	{
		theInstance = new WxSocial;
		theInstance->initData();
	}

	return theInstance;
}


WxSocial::~WxSocial()
{

}

void WxSocial::initData()
{
	// 设置微信开发平台帐号信息
    int gameType = GAMECONFIG.m_nAppType;

	if (gameType == GAME_TYPE_KXNNL)
	{
		_weChatAppId = "wx01f39efa9d7d7953";
		_weChatAppKey = "e78eb744edc27e58c4350d66c2c9094a";
	}

}

void WxSocial::wxAuthorizeTokenCb(int err_code, const char* szCode)
{
    log("wxAuthorizeTokenCb --%d-:%s-",err_code,szCode);
	if (err_code == 0)
	{

		char url[512] = { 0 };
		sprintf(url, "https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&secret=%s&code=%s&grant_type=authorization_code", _weChatAppId.c_str(), _weChatAppKey.c_str(), szCode);

		network::HttpRequest *pRequest = new network::HttpRequest();
		pRequest->setUrl(url);
		pRequest->setRequestType(network::HttpRequest::Type::GET);
		pRequest->setResponseCallback(CC_CALLBACK_2(WxSocial::onWxRefreshTokenHttpResponse, this));
		network::HttpClient::getInstance()->sendImmediate(pRequest);
		pRequest->release();
	}
	else
	{
		if (_wxLoginCb)
		{
			_wxLoginCb(false,err_code, NULL);
		}
	}
}


void WxSocial::onWxLoginHttpResponse(cocos2d::network::HttpClient* pClient, cocos2d::network::HttpResponse* pResponse)
{
	bool bRet = pResponse->isSucceed();
	std::string data;
	if (!bRet)
	{
		_wxLoginCb(false,555, NULL);
		clearUserData();
		return;
	}

	std::vector<char>* recv = pResponse->getResponseData();
	data.assign(recv->begin(), recv->end());

	log("WeiXin Login response: %s", data.c_str());
    rapidjson::Document dcmt;
    dcmt.Parse<0>(data.c_str());
    if(dcmt.HasParseError())
	{
		_wxLoginCb(false,666,NULL);
		clearUserData();
		return;
	}
    if (dcmt.HasMember("errcode"))
    {
        _wxLoginCb(false,777, NULL);
        clearUserData();
        return;
    }
	
	WxLoginInfo *userInfo = new WxLoginInfo();
	userInfo->unionid = dcmt["unionid"].GetString();
	userInfo->nickName = dcmt["nickname"].GetString();
	userInfo->headUrl = dcmt["headimgurl"].GetString();
	userInfo->sex = dcmt["sex"].GetInt();
    
    //QHallManager::getInstance()->setHeadUrl(userInfo->headUrl);
	_wxLoginCb(true,100, userInfo);
    
    
	delete userInfo;
}

void WxSocial::onWxRefreshTokenHttpResponse(cocos2d::network::HttpClient* pClient, cocos2d::network::HttpResponse* pResponse)
{
	log("onWxRefreshTokenHttpResponse");
	bool bRet = pResponse->isSucceed();
	std::string data;
	if (!bRet)
	{
		log("onWxRefreshTokenHttpResponse Faild");
		_wxLoginCb(false,888, NULL);
		clearUserData();
		return;
	}

	std::vector<char>* recv = pResponse->getResponseData();
	data.assign(recv->begin(), recv->end());

	log("onWxRefreshTokenHttpResponse response: %s", data.c_str());
    rapidjson::Document dcmt;
    dcmt.Parse<0>(data.c_str());
    if (dcmt.HasParseError())
    {
        _wxLoginCb(false,999, NULL);
        clearUserData();
        return;
    }
    if (dcmt.HasMember("errcode"))
    {
        int abc = dcmt["errcode"].GetInt();
        _wxLoginCb(false,abc, NULL);
        clearUserData();
        return;
    }
	std::string access_token = dcmt["access_token"].GetString();
	int  expires_in = dcmt["expires_in"].GetInt();
	std::string refresh_token = dcmt["refresh_token"].GetString();
	std::string openid = dcmt["openid"].GetString();
	std::string scope = dcmt["scope"].GetString();

	UserDefault::getInstance()->setStringForKey(WX_OPEN_ID, openid);
	UserDefault::getInstance()->setStringForKey(WX_ACCESS_TOKEN, access_token);
	UserDefault::getInstance()->setStringForKey(WX_REFRESH_TOKEN, refresh_token);
	UserDefault::getInstance()->setStringForKey(WX_SCOPE, scope);
	UserDefault::getInstance()->setIntegerForKey(WX_EXPIRES_IN, expires_in);
	UserDefault::getInstance()->setIntegerForKey(WX_TOKEN_ACCESS_TIME, time(NULL));
	UserDefault::getInstance()->setIntegerForKey(WX_REFRESH_TOKEN_ACCESS_TIME, time(NULL));

	WxLoginGetUserInfo(access_token, openid);
}

void WxSocial::WxLoginGetUserInfo(std::string access_token, std::string openid)
{
	std::string url = "https://api.weixin.qq.com/sns/userinfo?access_token=" + access_token + "&openid=" + openid;

	network::HttpRequest *pRequest = new network::HttpRequest();
	pRequest->setUrl(url.c_str());
	pRequest->setRequestType(network::HttpRequest::Type::GET);
	pRequest->setResponseCallback(CC_CALLBACK_2(WxSocial::onWxLoginHttpResponse, this));
	network::HttpClient::getInstance()->sendImmediate(pRequest);
	pRequest->release();
}


void WxSocial::RefreshWxToken()
{
	char url[512] = { 0 };

	std::string refresh_token = UserDefault::getInstance()->getStringForKey(WX_REFRESH_TOKEN);
	sprintf(url, "https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=%s&grant_type=refresh_token&refresh_token=%s", _weChatAppId.c_str(), refresh_token.c_str());

	network::HttpRequest *pRequest = new network::HttpRequest();
	pRequest->setUrl(url);
	pRequest->setRequestType(network::HttpRequest::Type::GET);
	pRequest->setResponseCallback(CC_CALLBACK_2(WxSocial::onWxRefreshTokenHttpResponse, this));
	network::HttpClient::getInstance()->sendImmediate(pRequest);
	pRequest->release();
}


void WxSocial::clearUserData()
{
	UserDefault::getInstance()->deleteValueForKey(WX_OPEN_ID);
	UserDefault::getInstance()->deleteValueForKey(WX_ACCESS_TOKEN);
	UserDefault::getInstance()->deleteValueForKey(WX_REFRESH_TOKEN);
	UserDefault::getInstance()->deleteValueForKey(WX_SCOPE);
	UserDefault::getInstance()->deleteValueForKey(WX_EXPIRES_IN);
	UserDefault::getInstance()->deleteValueForKey(WX_TOKEN_ACCESS_TIME);
	UserDefault::getInstance()->deleteValueForKey(WX_REFRESH_TOKEN_ACCESS_TIME);
}


void WxSocial::doWxLogin(WxLoginCallBack wxLoginCb)
{
	_wxLoginCb = wxLoginCb;

	time_t now_t = time(NULL);
	time_t wx_token_acess_t = UserDefault::getInstance()->getIntegerForKey(WX_TOKEN_ACCESS_TIME, 0);
	time_t wx_refresh_token_acess_t = UserDefault::getInstance()->getIntegerForKey(WX_REFRESH_TOKEN_ACCESS_TIME, 0);
	int expire_in = UserDefault::getInstance()->getIntegerForKey(WX_EXPIRES_IN, 7200);

    int gameType = GAMECONFIG.m_nAppType;
    int loginType = QLoginManager::getInstance()->getLoginType();
    if (gameType == GAME_TYPE_KXNNL)
    {
        wx_token_acess_t = 0;
    }
	if (gameType == GAME_TYPE_KXNNL && loginType == 1)
    {
        wx_token_acess_t = 0;
    }
    
    if (wx_token_acess_t == 0)
    {
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		AdmobJni::callWxAuthLogin(CC_CALLBACK_2(WxSocial::wxAuthorizeTokenCb, this));
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        iOSAdapterUtility::getInstance()->sendAuthReq(CC_CALLBACK_2(WxSocial::wxAuthorizeTokenCb, this));
#endif
        
		return;

    }
	if (now_t - wx_token_acess_t > expire_in)
	{
		RefreshWxToken();

		return;
	}

	if (now_t - wx_token_acess_t < expire_in)
	{
		std::string access_token = UserDefault::getInstance()->getStringForKey(WX_ACCESS_TOKEN);
		std::string openid = UserDefault::getInstance()->getStringForKey(WX_OPEN_ID);

		WxLoginGetUserInfo(access_token, openid);
		return;
	}
}

bool WxSocial::checkHasWxUser()
{
    time_t wx_token_acess_t = UserDefault::getInstance()->getIntegerForKey(WX_TOKEN_ACCESS_TIME, 0);
    if (wx_token_acess_t == 0)
    {
        return false;
    }else
    {
        return true;
    }
}

void WxSocial::shareRoom(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	AdmobJni::callWxInviteGame(webpageUrl, title, description, callback);
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSAdapterUtility::getInstance()->callWxInviteGame(webpageUrl, title, description, callback);
#endif
}
//分享到朋友圈
void WxSocial::shareRoomTo(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	AdmobJni::callWxInviteGameTo(webpageUrl, title, description, callback);
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	iOSAdapterUtility::getInstance()->callWxInviteGameTo(webpageUrl, title, description, callback);
#endif
}

void WxSocial::shareCaptureScreen(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback)
{
	log("==================shareCaptureScreen====================");
	utils::captureScreen([=](bool succeed, const std::string& outputFile) {
		if (succeed)
		{
			log("==================shareCaptureSucceed====================");
			std::string	image_path = FileUtils::getInstance()->getWritablePath() + "_share.jpg";
			if (!FileUtils::getInstance()->isFileExist(image_path))
			{
				//MessageBox(GetGameString("ScreenShotError"), "error");
				return;
			}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			log("==================AdmobJni::callWxShare====================");
			AdmobJni::callWxShare(image_path.c_str(), webpageUrl, "", "", callback);
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
			iOSAdapterUtility::getInstance()->shareImg(image_path.c_str(), webpageUrl, "", "", callback);
#endif
//            scheduleOnce(CC_SCHEDULE_SELECTOR(WxSocial::onCaptureScreenAfterShare), 0.5f);

		}
		else
		{
			log("==================shareCaptureFailed====================");
			return;
		}
	}, "_share.jpg");

	// 设置纹理宽、高、像素质量
}

void WxSocial::onCaptureScreenAfterShare(float dt)
{
    std::string	image_path = FileUtils::getInstance()->getWritablePath() + "_share.jpg";
    if (!FileUtils::getInstance()->isFileExist(image_path))
    {
        //MessageBox(GetGameString("ScreenShotError"), "error");
        return;
    }
    
    //std::string http_upl = GAME_HTTP_DOWNLOAD_URL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    AdmobJni::callWxShare(image_path.c_str(), http_upl.c_str(), GetGameString("WXShareEnd1"), GetGameString("WXShareEnd2"));
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//    iOSAdapterUtility::getInstance()->shareImg(image_path.c_str(), http_upl.c_str(), GetGameString("WXShareEnd1"), GetGameString("WXShareEnd2"));
#endif
}
