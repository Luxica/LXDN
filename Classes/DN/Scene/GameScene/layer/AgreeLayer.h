#ifndef _AGREE_LAYER_H__
#define _AGREE_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "../data/QGameBaseData.h"
#include "../data/QGameManager.h"

using namespace cocos2d::ui;

class AgreeLayer : public cocos2d::Layer
{
public:

	//关闭开关
	void touchFalseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	//确定开关
	void touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void UpdateAgreeState(QUserData* player[], int index);

	void setAgreeState(QUserData* player, int index, int agree);

	void resetTime(float dt);
public:
	AgreeLayer(){};
	~AgreeLayer(){};
	static AgreeLayer* create(std::string sContent);

	virtual bool init(std::string sContent);

	Layout* m_systemLayout;

	ListView* listView;
	Layout* m_tempLayout;

	int m_exitTime;						//关闭退出弹窗倒计时

};

#endif // _AGREE_LAYER_H__
