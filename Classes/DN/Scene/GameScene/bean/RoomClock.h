#ifndef _ROOMCLOCK_H_
#define _ROOMCLOCK_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace cocos2d::ui;
class RoomClock : public Node
{
public:
	CREATE_FUNC(RoomClock);
	RoomClock();
	~RoomClock();
	virtual bool init();
	void Tick(int timeCount,bool showTitle = true);
	void CancelTick();
	virtual void onExit() override;
private:
	void beginCount(int timeCount);
	void timeTick(float dt);
	void showCountNum();
private:
	Node* m_pRootNode;
	TextAtlas* m_pAtlRoomClockNum;
	ImageView* m_pImagViewBg;
	int m_showingTime;
};

#endif //_ROOMCLOCK_H_