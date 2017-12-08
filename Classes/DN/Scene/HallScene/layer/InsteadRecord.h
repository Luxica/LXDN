#ifndef _INSTEAD_RECORD_H__
#define _INSTEAD_RECORD_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class InsteadRecord : public cocos2d::Layer
{
public:

	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchKickCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    void touchRecordEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchRefreshEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchInviteEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchEnterRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchKickEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchWarnEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchHeadEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void onInviteCB(int errorCode);

	void touchDismissEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchCopyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void updateInstead();

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(InsteadRecord);

private:
	Node* m_systemExitLayer;
	ListView* m_listView;
	Layout* m_recordLayout;
	ImageView* m_imageTab;
	Button* m_buttonCur;

	Button* m_refreshButton;

	Layout* m_nEndLayout;
	Layout* m_endLayout;
	Layout* m_kickLayout;
	int m_curNum;
	int m_curRoomNum;
};

#endif // _SUSSEX_LAYER_H__
