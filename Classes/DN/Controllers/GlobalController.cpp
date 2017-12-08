#include "GlobalController.h"
#include "GlobalEvent.h"

GlobalController::GlobalController()
{

}

GlobalController::~GlobalController()
{

}

bool GlobalController::init()
{
	auto eventListener = Director::getInstance()->getEventDispatcher()->addCustomEventListener(SHOW_WAIT,
		CC_CALLBACK_1(GlobalController::onShowWait, this));
	eventListener = Director::getInstance()->getEventDispatcher()->addCustomEventListener(HIDE_WAIT,
		CC_CALLBACK_1(GlobalController::onHideWait, this));

	return true;
}

void GlobalController::onShowWait(EventCustom *pEvent)
{
	log("111111111");
}

void GlobalController::onHideWait(EventCustom *pEvent)
{

}