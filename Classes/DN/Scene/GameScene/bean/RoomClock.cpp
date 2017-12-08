#include <math.h>
#include "RoomClock.h"
RoomClock::RoomClock()
{
}
RoomClock::~RoomClock()
{
}
bool RoomClock::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_pRootNode = CSLoader::createNode("RoomClock.csb");
	addChild(m_pRootNode);
	auto Image_1 = dynamic_cast<ImageView*>  (m_pRootNode->getChildByName("Image_1"));
	m_pRootNode->setPosition(Image_1->getContentSize().width/2, Image_1->getContentSize().height/2);
	m_pAtlRoomClockNum = static_cast<TextAtlas *> (m_pRootNode->getChildByName("AtlasLabel_count"));
	m_pImagViewBg = dynamic_cast<ImageView*>(m_pRootNode->getChildByName("Image_1_0"));
	return true;
}
void RoomClock::Tick(int timeCount, bool showTitle)
{
	unschedule(CC_SCHEDULE_SELECTOR(RoomClock::timeTick)); 
	setVisible(true);
	m_pImagViewBg->setVisible(showTitle);
	beginCount(timeCount);
}

void RoomClock::CancelTick()
{
	setVisible(false);
}
void RoomClock::onExit()
{
	Node::onExit();
}
void RoomClock::beginCount(int timeCount)
{
	m_showingTime = timeCount;
	schedule(CC_SCHEDULE_SELECTOR(RoomClock::timeTick), 1.0f);
	showCountNum();
}
void RoomClock::timeTick(float dt)
{
	if (m_showingTime == 0)
	{
		unschedule(CC_SCHEDULE_SELECTOR(RoomClock::timeTick)); 
		return;
	}
		m_showingTime--;
	showCountNum();
}
void RoomClock::showCountNum()
{
	char szCountNum[4] = { 0 };
	sprintf(szCountNum, "%02d", m_showingTime);
	m_pAtlRoomClockNum->setVisible(true);
	m_pAtlRoomClockNum->setString(szCountNum);
}