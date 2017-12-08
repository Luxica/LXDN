#ifndef _SUSSEX_LAYER_H__
#define _SUSSEX_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class SussexLayer : public cocos2d::Layer
{
public:

	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    void touchShareEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void updateRecord();
    
    void onInviteCB(int errorCode);

	//触碰事件  （单点触碰）
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(SussexLayer);

private:
	
	ListView* m_listView;
	Layout *m_recordLayout;
    Node*  m_systemExitLayer;

};

#endif // _SUSSEX_LAYER_H__
