#pragma once

#include <string>
#include <functional>

struct WxLoginInfo
{
	std::string openid;
	std::string unionid;
	std::string nickName;
	std::string headUrl;
	int sex;
	std::string province;
	std::string city;
	std::string country;

	WxLoginInfo()
	{
		unionid = "";
		nickName = "";
		headUrl = "";
		province = "";
		city = "";
		country = "";
		sex = 0;
	}
};

typedef std::function<void(int errCode, const char* szToken)> WxAuthorizeCallBack;

typedef std::function<void(int errCode)> WxInviteGameCallBack;

typedef std::function<void(int errCod)> WxShareImgCallBack;
