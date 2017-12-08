#ifndef __QLOGIN_SCENE_H__
#define __QLGOIN_SCENE_H__

#include "cocos2d.h"
#include "../../Comm/filter/MsgLogonFilter.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"

using namespace cocos2d::ui;
using namespace cocos2d;
class Load;
class DialogLayer;

class QLoginScene : public Scene, public MsgLogonFilter
{
public:
	QLoginScene();
	~QLoginScene();

	std::string getSceneName()	{ return "LoginScene"; }
	void onEnter();
	void onExit();
	virtual bool init();
	void sendVersionHttp();
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	CREATE_FUNC(QLoginScene);

public:
	virtual void onDataReceiver(int msgType, Ref* objectData);

	void touchExitEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchUpdateEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

private:
	int m_nHttpCounts_;

	Load* m_loading;

	DialogLayer* m_dialogLayer;
};

#endif