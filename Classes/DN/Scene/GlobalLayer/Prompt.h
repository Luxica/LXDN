#ifndef _PROMPT_LAYER_H__
#define _PROMPT_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class Prompt : public cocos2d::Layer
{
public:

	//确定开关
	void touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
	Prompt(){};
	~Prompt(){};
	static Prompt* create(int type, std::string sContent);

	virtual bool init(int type, std::string sContent);
	
	Layout* m_systemLayout;
};

#endif // _DIALOG_LAYER_H__
