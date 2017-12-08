#include "ControllerMgr.h"
#include "cocos2d.h"
#include "GlobalController.h"
using namespace cocos2d;

ControllerMgr *ControllerMgr::_instance = nullptr;

ControllerMgr *ControllerMgr::getInstance()
{
	if (!_instance) {
		_instance = new ControllerMgr();
		if (!_instance || !_instance->init()) {
			CC_SAFE_DELETE(_instance);
			_instance = nullptr;
		}
	}

	return _instance;
}

void ControllerMgr::destroyInstance()
{
	if (_instance)
	{
		CC_SAFE_DELETE(_instance);
		_instance = nullptr;
	}
}

ControllerMgr::ControllerMgr()
{

}

ControllerMgr::~ControllerMgr()
{

}

bool ControllerMgr::init()
{
	GlobalController::create();
	return true;
}