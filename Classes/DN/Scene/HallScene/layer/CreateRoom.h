#ifndef __CREATEROOM_LAYER_H__
#define __CREATEROOM_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <list>
using namespace std;
using namespace cocos2d::ui;
class Prompt;

class CreateRoom : public cocos2d::Layer
{
private:

	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void selectedGameNumEvent(Ref* pSender, CheckBox::EventType type);
	void selectedGameModelEvent(Ref* pSender, CheckBox::EventType type);
	void selectedGamePayEvent(Ref* pSender, CheckBox::EventType type);
	void selectedKcardWayEvent(Ref* pSender, CheckBox::EventType type);
	void selectedMaxBetEvent(Ref* pSender, CheckBox::EventType type);
	void selectedHalfEvent(Ref* pSender, CheckBox::EventType type);
	void selectedMModeEvent(Ref* pSender, CheckBox::EventType type);
	void selectedForbidEvent(Ref* pSender, CheckBox::EventType type);
	void selectedPlayTypeEvent(Ref* pSender, CheckBox::EventType type);

	void onHideWaitLayer(cocos2d::EventCustom *pEvent);
    
    /**
     * 显示扣除房卡
     */
    void onShowCostRoomCard();
    
public:
	bool isCreat();

	bool isInstead();

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(CreateRoom);

private:

	CheckBox* m_pGameNum;
	CheckBox* m_pGameMode;
	CheckBox* m_pGamePay;
	CheckBox* m_pHalf;

    Text            *m_roomCardText; //扣住房卡
	int m_costCard;
    int             m_costType;   //付费类型 1 aa 2房主

	Layout* layout;

	bool m_isMMode;			//有无牛番
	bool m_isForbid;		//是否禁止中途加入
};

#endif // __CREATEROOM_LAYER_H__
