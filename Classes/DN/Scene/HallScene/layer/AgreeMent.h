#ifndef _AGREEMENT_LAYER_H__
#define _AGREEMENT_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class Agreement : public cocos2d::Layer
{
public:

	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(Agreement);
	
private:
	Button*		m_buttonCur;
	ListView*	m_listNews;
	ListView*	m_listContact;
	ListView*	m_listInform;
};

#endif // _NEWS_LAYER_H__
