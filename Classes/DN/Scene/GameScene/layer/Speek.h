#ifndef __SPEEK_LAYER_H__
#define __SPEEK_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

class SpeekLayer: public cocos2d::Layer
{
public:
	static SpeekLayer* create(int type,int chatId);

    virtual bool init();

	bool init(int type, int chatId);

	void setType(int type = -1, int chatId=0);
	CREATE_FUNC(SpeekLayer);
public:

	cocos2d::Animate* SpeakAction();

	void DestroySpesk();
private:

};

#endif // __SPEEK_LAYER_H__
