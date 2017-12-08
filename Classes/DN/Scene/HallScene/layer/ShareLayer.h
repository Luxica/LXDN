#ifndef _SHARE_LAYER_H__
#define _SHARE_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class ShareLayer : public cocos2d::Layer
{
public:
	void touchToFriendEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//关闭开关
	void touchToCoterieEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void onInviteCB(int errorCode);
    void onInviteAB(int errorCode);
    
	void setUID(int uid);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(ShareLayer);

private:

	int m_isShare;
	Layout* systemLayout;
	
};

#endif // _NEWS_LAYER_H__
