#ifndef _INFORMATION_LAYER_H__
#define _INFORMATION_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../../GameScene/data/QGameBaseData.h"

using namespace cocos2d::ui;

class Information : public cocos2d::Layer
{
public:

	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchExpressEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type, QUserData* userDate);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
 
	static Information* create(QUserData* userDate);
	bool init(QUserData* userDate);

    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(Information);

private:

	std::vector<ImageView*> m_imageView;

};

#endif // _SUSSEX_LAYER_H__
