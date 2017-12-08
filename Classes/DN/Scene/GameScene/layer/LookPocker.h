#ifndef __LOOKPOCKER_LAYER_H__
#define __LOOKPOCKER_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class PocketSprite;
class QCardData;

class LookPocker : public cocos2d::Layer
{
public:
	LookPocker();
	~LookPocker();
	virtual bool init();
	CREATE_FUNC(LookPocker);

	void onExit();
	void setParentLayer(Node* parent){ m_parentLayer = parent; };
public:
	void touchPocketEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

public:
	void destroyLayer();
	void refreshData(QCardData *pCard);

	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void ShowPocker();
private:
	Node* m_parentLayer;
	Node* m_pRootNode;

	ImageView* m_panelPos[5];

	vector<Sprite*> m_vecSprite;
	vector<Sprite*> m_vecTouchSprite;

	vector<PocketSprite*> m_vecPocket;
	EventListenerTouchOneByOne* listener;

	Vec2 m_beginPoint;

	bool isControl;
	int cuoindex;	//搓牌到第几帧
	bool isShow;	//是否开牌

	int touchIndex;
};
#endif // __LOOKPOCKER_LAYER_H__
