#ifndef _DIALOG_LAYER_H__
#define _DIALOG_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class DialogLayer : public cocos2d::Layer
{
public:

	//关闭开关
	void touchFalseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	//确定开关
	void touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
	DialogLayer(){};
	~DialogLayer(){};
	static DialogLayer* create(int type, std::string sContent);

	virtual bool init(int type, std::string sContent);
	
	Layout* m_systemLayout;
};

#endif // _DIALOG_LAYER_H__
