#ifndef __FEED_BACK_H__
#define __FEED_BACK_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class FeedBack : public cocos2d::Layer
{
public:
    
	void touchBtnEvent(Ref* pSender, CheckBox::EventType type);

	void touchSubmitEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void updateFeedBack(int errorCode);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(FeedBack);
	
private:
	CheckBox*	m_CurSelect;
	Layout*	m_kfLayer;
	Layout*	m_fkLayer;
	TextField* m_textPhone;			//手机号
	TextField* m_textOpinion;		//意见
};

#endif // _NEWS_LAYER_H__
