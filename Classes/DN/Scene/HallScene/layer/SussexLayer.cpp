#include "SussexLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../data/QHallManager.h"
#include "../../../../StringTool.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "WxSocial.h"
#include "GameConfig.h"
#include "../../../../ScriptDataManager.h"
USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool SussexLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	m_systemExitLayer = CSLoader::createNode("GameSussex/SussexLayer.csb");
	if (m_systemExitLayer)
	{
		this->addChild(m_systemExitLayer);
		auto systemLayout = dynamic_cast<Layout*>(m_systemExitLayer->getChildByName("Panel"));
		m_listView = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_Sussex"));
		m_recordLayout = dynamic_cast<Layout*>(systemLayout->getChildByName("Panel_Sussex"));
		m_recordLayout->setVisible(false);
		auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
		if (closeButton)
		{
			closeButton->addTouchEventListener(CC_CALLBACK_2(SussexLayer::touchCloseEvent, this));
		}
		auto button_True = dynamic_cast<Button*>(systemLayout->getChildByName("Button_True"));
		if (button_True)
		{
			button_True->addTouchEventListener(CC_CALLBACK_2(SussexLayer::touchCloseEvent, this));
		}
        
        auto Button_Xuan = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Xuan"));
		if (Button_Xuan) Button_Xuan->addTouchEventListener(CC_CALLBACK_2(SussexLayer::touchShareEvent, this));
	}
	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SussexLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SussexLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SussexLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
void SussexLayer::updateRecord()
{
    if (m_systemExitLayer == nullptr)return;
        
	auto recordDateVec = QHallManager::getInstance()->m_vecRecord_;
	auto sizeRecord = recordDateVec.size();
	char szCountNum[32] = { 0 };
	char szUser[32] = { 0 };
	for (int i = 0; i < sizeRecord; i++)
	{
		if (!m_recordLayout)
			return;
		auto temp = m_recordLayout->clone();
		if (!temp)
			return;
		temp->setVisible(true);
		m_listView->pushBackCustomItem(temp);

		auto sortAtlaslabel = dynamic_cast<TextAtlas*>(temp->getChildByName("AtlasLabel_Record"));
		if (sortAtlaslabel)
		{
			sprintf(szCountNum, "%2d", i + 1);
			sortAtlaslabel->setVisible(true);
			sortAtlaslabel->setString(szCountNum);
		}
		auto win = dynamic_cast<ImageView*>(temp->getChildByName("Image_shengfu"));
		auto textRoomNum = dynamic_cast<Text*>(temp->getChildByName("Text_RoomNum"));
		textRoomNum->setString(StringTool::intToString(recordDateVec.at(i)->getRoomNum()));
		auto textTime = dynamic_cast<Text*>(temp->getChildByName("Text_Time"));
		textTime->setString(recordDateVec.at(i)->getRecordTime());

		auto gameMMode = recordDateVec.at(i)->getVXRoom()->getMMode();
		char cInfo[32] = { 0 };
		auto gameZMode = recordDateVec.at(i)->getVXRoom()->getZMode();
		char zMode[32] = { 0 };
		sprintf(zMode, "GameMode%d", gameZMode);
		if (gameMMode == 0)
		{
			sprintf(cInfo, GetGameString("Record"), GetGameString("game_mode_3"), GetGameString(zMode));
		}
		if (gameMMode == 1)
		{
			sprintf(cInfo, GetGameString("Record"), GetGameString("game_add_1"), GetGameString(zMode));
		}
		auto test_Info = dynamic_cast<Text*>(temp->getChildByName("Text_Info"));
		if (test_Info)
			test_Info->setString(cInfo);
		auto m_userVec = recordDateVec.at(i)->getScoreVec();
		auto sizeUser = m_userVec.size();
		for (int index = 0; index < sizeUser; index++)
		{
			auto userDate = m_userVec.at(index);
			sprintf(szUser, "Text_Name_%d", index);
			if (userDate->getUserId() == QHALLMGR->getUserId())
			{
				if (win)
				{
					if (userDate->getScore() >= 0)
					{
						win->loadTexture("res/GameSussex/7.png");
					}
					else
					{
						win->loadTexture("res/GameSussex/8.png");
					}
				}
			}
			auto textUserName = dynamic_cast<Text*>(temp->getChildByName(szUser));
			textUserName->setVisible(true);
			textUserName->setString(userDate->getNickName());
			sprintf(szUser, "Text_Data_%d", index);
			auto textUserScore = dynamic_cast<Text*>(temp->getChildByName(szUser));
			textUserScore->setVisible(true);
			sprintf(szCountNum, ":%d", userDate->getScore());
			textUserScore->setString(szCountNum);
		}
	}
	
}
//触碰事件  （单点触碰）
bool SussexLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void SussexLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void SussexLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void SussexLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

//分享
void SussexLayer::touchShareEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
    WxSocial::getInstance()->shareCaptureScreen(GAMECONFIG.m_sHttpDownUrl.c_str(), "", "", CC_CALLBACK_1(SussexLayer::onInviteCB, this));
}

void SussexLayer::onInviteCB(int errorCode)
{
	return;
}



