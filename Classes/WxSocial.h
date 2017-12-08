#pragma once
#include <string>
#include <functional>
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "WxSocialEntity.h"

using namespace cocos2d;
using namespace cocos2d::ui;

typedef std::function<void(bool success, int code,const WxLoginInfo* wxLoginInfo)> WxLoginCallBack;

typedef std::function<void(bool success)> WxShareCallBack;

class WxSocial :public Node
{
public:

	static WxSocial* getInstance();
	
	~WxSocial();

	void doWxLogin(WxLoginCallBack wxLoginCb);

	void shareRoom(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback);
	//分享到朋友圈
	void shareRoomTo(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback);

	void shareCaptureScreen(const char* webpageUrl, const char* title, const char* description, WxInviteGameCallBack callback);
    
    /**
     * 核实本地是否已经有微信账号登入过
     */
    bool checkHasWxUser();

private:

	WxSocial();

	void initData();

	void wxAuthorizeTokenCb(int err_code,const char* szToken);

	void onWxLoginHttpResponse(cocos2d::network::HttpClient* pClient, cocos2d::network::HttpResponse* pResponse);

	void onWxRefreshTokenHttpResponse(cocos2d::network::HttpClient* pClient, cocos2d::network::HttpResponse* pResponse);

	void WxLoginGetUserInfo(std::string access_token, std::string openid);

	void RefreshWxToken();

	void clearUserData();
    
    
    /**
     *截屏之后延时处理
     */
    void onCaptureScreenAfterShare(float dt);

private:

	static WxSocial					*theInstance;

	WxLoginCallBack					_wxLoginCb;							// 微信登录回调

	std::string						_weChatAppId;						// 微信AppId

	std::string						_weChatAppKey;						// 微信AppKey
};
