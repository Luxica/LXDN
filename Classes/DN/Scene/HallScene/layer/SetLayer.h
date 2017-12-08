#ifndef __SET_LAYER_H__
#define __SET_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class SetLayer : public cocos2d::Layer
{
public:

	void touchMusicEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchSoundEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void musicSliderCallBack(Ref *pSender, Slider::EventType type);
	void soundSliderCallBack(Ref *pSender, Slider::EventType type);

	void touchCancelEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//显示
	void showMyself();
	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchExitEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(SetLayer);

};

#endif // __SET_LAYER_H__
