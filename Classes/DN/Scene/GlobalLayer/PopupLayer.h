#ifndef _POPUP_LAYER_H__
#define _POPUP_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class PopupLayer : public cocos2d::Layer
{
public:

	//关闭按钮
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
	PopupLayer(){};
	~PopupLayer(){};
	static PopupLayer* create(std::string path);

	virtual bool init(std::string path);
	Layout* m_bgLayout;
	Layout* m_systemLayout;
};

#endif // _DIALOG_LAYER_H__
