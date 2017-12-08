#ifndef __LOGIN_LOAD_H__
#define __LOGIN_LOAD_H__

#include "cocos2d.h"
//#include "../../Comm/filter/MsgLogonFilter.h"

#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class LoginLoad : public Layer
{
public:
	LoginLoad();
	~LoginLoad();

	virtual bool init();
	CREATE_FUNC(LoginLoad);

public:
	void stopCallBack();


};

#endif