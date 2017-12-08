#ifndef _DISMISS_LAYER_H__
#define _DISMISS_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "../data/QGameBaseData.h"
#include "../data/QGameManager.h"
class QGameLayer;

using namespace cocos2d::ui;

class DismissLayer : public cocos2d::Layer
{
public:

	//关闭开关
	void touchFalseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	//确定开关
	void touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchAgreeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchRefuseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void UpdateAgreeState(QUserData* player[], int index);

	void UpdateDismissResult(QUserData* player[], int cPass);

	void setAgreeState(QUserData* player, int index, int agree);

	void resetTime(float dt);

	void setSelfParent(QGameLayer* parent){ m_parent = parent; }
public:
	DismissLayer(){};
	~DismissLayer(){};
	static DismissLayer* create(int curNum);

	virtual bool init(int curNum);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:

	Layout* m_dismissLayout_1;
	Layout* m_dismissLayout_2;
	Layout* m_tsLayout;
	Layout* m_failLayout;

	ListView* listView;
	Layout* m_tempLayout;

	int m_exitTime;						//关闭退出弹窗倒计时

	QGameLayer* m_parent;

};

#endif // _AGREE_LAYER_H__
