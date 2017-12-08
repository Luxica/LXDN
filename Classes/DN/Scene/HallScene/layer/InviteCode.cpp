#include "InviteCode.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Scene/GameScene/QGameScene.h"
#include "../../../Scene/HallScene/data/QHallManager.h"
#include "../../../../StringTool.h"
#include "../../../Comm/request/MsgHallRequest.h"
#include "../../QDNEvent.h"
#include "../../../Scene/GlobalLayer/Load.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include <stdlib.h>

USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool InviteCode::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

   	auto systemExitLayer = CSLoader::createNode("InviteCodeLayer.csb");
	this->addChild(systemExitLayer);
	systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel_5"));

	auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_False"));
	if (closeButton)	closeButton->addTouchEventListener(CC_CALLBACK_2(InviteCode::touchCloseEvent, this));
    auto Button_Close = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
	if (Button_Close)	Button_Close->addTouchEventListener(CC_CALLBACK_2(InviteCode::touchCloseEvent, this));
    auto trueButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_True"));
	if (trueButton)		trueButton->addTouchEventListener(CC_CALLBACK_2(InviteCode::touchTrueEvent, this));
    
    m_inviteCodeTF = dynamic_cast<TextField*>(systemLayout->getChildByName("TextField_Code"));
	this->onFreshBand();
	this->setName("inveteCode");
    return true;
}
void InviteCode::onFreshBand()
{
	int userType = QHALLMGR->getUserType();
	if (userType == 2)
	{
		auto trueButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_True"));
		if (trueButton) trueButton->setEnabled(false);
		m_inviteCodeTF->setVisible(false);
		auto textBz = dynamic_cast<Text*>(systemLayout->getChildByName("Text_BZ"));
		if (textBz) textBz->setVisible(true);
	}
}
void InviteCode::touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)//按钮按下在按钮上抬起
        return;
    std::string codeString = m_inviteCodeTF->getString();

    MsgHallRequest::sendBindCode(codeString.c_str());
	//this->onFreshBand();
}


//关闭
void InviteCode::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)//按钮按下在按钮上抬起
		return;

	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}
