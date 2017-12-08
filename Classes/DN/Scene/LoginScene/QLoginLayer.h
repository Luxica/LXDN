#ifndef __QLOGIN_LAYER_H__
#define __QLOGIN_LAYER_H__

#include "cocos2d.h"
#include "../../Comm/filter/MsgLogonFilter.h"

#include "ui/CocosGUI.h"
#include "WxSocial.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class QLoginLayer : public Layer
{
public:
	QLoginLayer();
	~QLoginLayer();

	virtual bool init();
	CREATE_FUNC(QLoginLayer);

public:

	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchLoginEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//void setButtonVisibleIs();
	//void setButtonVisibleNo();
private:
    void onWxAuthCb(bool success,int code ,const WxLoginInfo* wxLoginInfo);
private:

	Button* closeButton;
	Button*	loginButton;
	CheckBox* m_checkBox;
	Text* m_agreement;

	TextField* m_userID;

	TextField* m_userPW;
    
};

#endif
