﻿#ifndef _REBATE_LAYER_H__
#define _REBATE_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class RebateLayer : public cocos2d::Layer
{
public:

	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchCopyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(RebateLayer);
	
private:

	ListView* m_listView;
	Layout *m_rebateLayout;
	std::string m_selfWxNum;
};

#endif // _NEWS_LAYER_H__
