#ifndef __ADDROOM_LAYER_H__
#define __ADDROOM_LAYER_H__

#include <vector>
using namespace std;

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class AddRoom : public cocos2d::Layer
{
public:
	AddRoom() {}
	~AddRoom(){}
	void touchKeyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchPasteEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void onHideWaitLayer(cocos2d::EventCustom *pEvent);
public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(AddRoom);

private:
	std::string m_RoomNum;

	vector<string> m_vecRoomNum;

	//std::string m_RoomNumShow;

	int m_RoomNumSend;

	Text* m_textNum[6];
	
};

#endif // __ADDROOM_LAYER_H__
