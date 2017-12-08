#ifndef __QHALL_SCENE_H__
#define __QHALL_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocos2d;

class QHallScene : public Scene
{
public:
	QHallScene();
	~QHallScene();

	std::string getSceneName()	{ return "QHallScene"; }
	void onEnter();
	void onExit();
	virtual bool init();
    void setHallRoomCard();
	void setInviteShow();
	void showShop();
    void showVIP();
	void setHint(bool isShow);
	void updateOnlineNum(unsigned int num);

	CREATE_FUNC(QHallScene);
};

#endif
