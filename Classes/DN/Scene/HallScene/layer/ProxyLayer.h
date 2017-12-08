#ifndef _PROXY_LAYER_H__
#define _PROXY_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class ProxyLayer : public cocos2d::Layer
{
public:
	void touchProxyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchAgreementEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(ProxyLayer);
	
private:
	Button*		m_buttonCur;
	ListView*	m_listProxy_1;
	ListView*	m_listProxy_2;
	ListView*	m_listProxy_3;
	CheckBox*   checkBox_Agreement;
	Button*		button_Agreement;
    
    int         m_startDownIndex;

	std::map<std::string, std::string> m_dataDic;
};

#endif // _NEWS_LAYER_H__
