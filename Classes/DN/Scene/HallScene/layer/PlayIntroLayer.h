#ifndef __PLAYINTRO_LAYER_H__
#define __PLAYINTRO_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class PlayIntroLayer : public cocos2d::Layer
{
public:

	void touchPlayEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(PlayIntroLayer);
	
private:
	ImageView*    m_newsImage;
	Button*       m_playType;
	ListView*         listView_Intro;
	ListView*         listView_Type;
};

#endif // __PLAYINTRO_LAYER_H__
