#ifndef __LOAD__
#define __LOAD__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class Load : public cocos2d::Layer
{
public:
	Load(){};
	~Load(){};
	virtual bool init();

	CREATE_FUNC(Load);

public:
	//void stopCallBack();
	
};

#endif // _DIALOG_LAYER_H__
