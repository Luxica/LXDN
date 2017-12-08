#ifndef __INVITECODE_LAYER_H__
#define __INVITECODE_LAYER_H__

#include <vector>
using namespace std;

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class InviteCode : public cocos2d::Layer
{
public:
	InviteCode() {}
	~InviteCode(){}
	void touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void onFreshBand();
	//关闭开关
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(InviteCode);

private:
    TextField*      m_inviteCodeTF;

	Layout*			systemLayout;
};

#endif // __InviteCode_LAYER_H__
