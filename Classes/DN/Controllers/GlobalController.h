#ifndef __GlobalController_H__
#define __GlobalController_H__

#include "cocos2d.h"
using namespace cocos2d;

class GlobalController : public Ref
{
public:
	CREATE_FUNC(GlobalController);
	GlobalController();
	~GlobalController();

private:
	bool init();
	void onShowWait(EventCustom *pEvent);
	void onHideWait(EventCustom *pEvent);
};

#endif