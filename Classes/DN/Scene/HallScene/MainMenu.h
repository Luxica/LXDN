#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

USING_NS_CC;

class SetLayer;
class SussexLayer;
class SortScene;
class InsteadRecord;
class DialogLayer;
class FeedBack;
class InviteCode;

class MainMenu : public cocos2d::Layer
{
public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void update(float dt);
    // implement the "static create()" method manually
	CREATE_FUNC(MainMenu);

public:
	void updateCanShare(float dt);
	void touchCSEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchHeadEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchBackEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchRechargeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchSortEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchSociatyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchInsteadEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchNewsEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchMarkEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchSetEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchIntroEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchExtendEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchShareEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchCopyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchInviteEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchInviteCodeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchCreateRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchBackRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchDiamondRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchAddRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchZJHEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchRebateEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchProxyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchAddCardEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchLoginShopEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchFeedBackEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchExpandEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void resumeScrollTextPosition();

	void initListener();

	void updateNotice(EventCustom* event);
	void updateOnlineNum(unsigned int num);
	void updateRecord(EventCustom* event);
	void updateRank(EventCustom* event);
	void updateInstead(EventCustom* event);

	void updateTakeOn(EventCustom* event);
	void updateTakeOff(EventCustom* event);

	void updateRoomKick(EventCustom* event);

	void updateFeedBack(EventCustom* event);

    void onInviteCB(int errorCode);
	void setHallRoomCard();
	void setInviteShow();
	void setHint(bool isShow);
    void onShowInviteDlg();

	void showShop();

	void touchLinkButtonEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	
	void updateAction();
    
    void showVIP();

	ImageView* getHintLayer(){ return m_hint; };

	void updateWife(float dt);				//更新wife信号和电池电量

	void selectedMenuEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void LocationCallBack(double latitude, double Longitude, const char* sAddress);
private:

	Text* m_userName;
	Text* m_userCardNum;

	int m_stringLenth;
	std::vector<std::string> m_noticeVec;
	bool m_bIsAction;
	Layout* m_scrollScrn;
	Text* m_TextScroll;
    
    ImageView* m_image_VIP;

	SortScene* m_Rank;			//排行版
	SussexLayer* m_Record;		//战绩
	InsteadRecord* m_Instead;	//代开记录
	FeedBack* m_FeedBack;		//反馈

	DialogLayer* m_shopingLayer;

	ImageView* m_hint;
	bool m_IsUpdate;

	Text* m_textNum;					//当前在线人数

	ImageView* m_Wife;					//wife信号
	LoadingBar* m_Power;				//电池电量

	Button* m_inviteButton;				// 邀请按钮

	Layout* m_menuLayer;
	CheckBox* menueCheck;
};

#endif // __MAINMENU_SCENE_H__
