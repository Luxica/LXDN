#include "InsteadRecord.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../data/QHallManager.h"
#include "../../../../DN/Comm/request/MsgHallRequest.h"
#include "../../../../DN/Comm/request/MsgGameRequest.h"
#include "../../../../StringTool.h"
#include "../../../../net/LordNetManger.h"
#include "../../../../NetSystem/WWNetwork.h"
#include "../../GlobalLayer/Prompt.h"
#include "../../../Scene/GlobalLayer/Load.h"
#include "../../../Scene/GlobalLayer/PopLayer.h"
#include "GameAudioEngine.h"
#include "ScriptDataManager.h"
#include "WWMacros.h"
#include "WxSocial.h"
#include "GameConfig.h"
USING_NS_CC;
using namespace cocos2d::experimental;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "WebSprite.h"
#include "iOSAdapterUtility.hpp"
#else
#include "GameNode/WebSprite.h"
#include "AdmobJni.h"
#endif


int instead_CostCard[5] = { 4, 4, 7, 10, 1 };
// on "init" you need to initialize your instance
bool InsteadRecord::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	m_imageTab = nullptr;
	m_refreshButton = nullptr;
	m_curRoomNum = 0;
	m_systemExitLayer = CSLoader::createNode("InsteadRecord.csb");
	if (m_systemExitLayer)
	{
		this->addChild(m_systemExitLayer);
		auto systemLayout = dynamic_cast<Layout*>(m_systemExitLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			m_listView = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_Record"));
			m_recordLayout = dynamic_cast<Layout*>(systemLayout->getChildByName("Panel_Record"));
			if (m_recordLayout)
			{
				m_recordLayout->setVisible(false);
			}
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchCloseEvent, this));
			}

			m_imageTab = dynamic_cast<ImageView*>(systemLayout->getChildByName("Image_Tab"));

			auto button_NEnd = dynamic_cast<Button*>(systemLayout->getChildByName("Button_NEnd"));
			button_NEnd->setTag(1);
			m_buttonCur = button_NEnd;
			button_NEnd->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchRecordEvent, this));

			auto button_End = dynamic_cast<Button*>(systemLayout->getChildByName("Button_End"));
			button_End->setTag(2);
			button_End->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchRecordEvent, this));
		}
		m_nEndLayout = dynamic_cast<Layout*>(m_systemExitLayer->getChildByName("Panel_NEnd"));
		if (m_nEndLayout)
		{
			m_listView = dynamic_cast<ListView*>(m_nEndLayout->getChildByName("ListView_Record"));
			m_recordLayout = dynamic_cast<Layout*>(m_nEndLayout->getChildByName("Panel_Record"));
			if (m_recordLayout)
			{
				m_recordLayout->setVisible(false);
			}
			auto closeButton = dynamic_cast<Button*>(m_nEndLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchCloseEvent, this));
			}

			m_refreshButton = dynamic_cast<Button*>(m_nEndLayout->getChildByName("Button_Refresh"));
			if (m_refreshButton)
			{
				m_refreshButton->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchRefreshEvent, this));
			}

			m_kickLayout = dynamic_cast<Layout*>(m_nEndLayout->getChildByName("Panel_Kick"));
			if (m_kickLayout)
			{
				m_kickLayout->setZOrder(11);
				m_kickLayout->setVisible(false);
				auto closeButton = dynamic_cast<Button*>(m_kickLayout->getChildByName("Button_Close"));
				if (closeButton)
				{
					closeButton->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchKickCloseEvent, this));
				}
			}

			auto button_End = dynamic_cast<Button*>(m_nEndLayout->getChildByName("Button_End"));
			button_End->setTag(2);
			button_End->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchRecordEvent, this));
		}
		m_endLayout = dynamic_cast<Layout*>(m_systemExitLayer->getChildByName("Panel_End"));
		if (m_endLayout)
		{
			auto closeButton = dynamic_cast<Button*>(m_endLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchCloseEvent, this));
			}

			auto button_NEnd = dynamic_cast<Button*>(m_endLayout->getChildByName("Button_NEnd"));
			button_NEnd->setTag(1);
			m_buttonCur = button_NEnd;
			button_NEnd->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchRecordEvent, this));
		}
	}
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		instead_CostCard[1] = 1;
		instead_CostCard[2] = 2;
	}
	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(InsteadRecord::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(InsteadRecord::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(InsteadRecord::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
void InsteadRecord::updateInstead()
{
    if (m_systemExitLayer == nullptr)return;

	auto gameType = QHALLMGR->getGameType();
	auto takeType = QHALLMGR->getTakeType();
        
	auto taRoomVec = QHallManager::getInstance()->m_vecTARoom_;
	auto sizeRecord = taRoomVec.size();
	if (m_nEndLayout)
	{
		auto text_Num = dynamic_cast<Text*>(m_nEndLayout->getChildByName("Text_Num"));
		if (text_Num)
		{
			text_Num->setString(StringTool::intToString(sizeRecord));
		}
	}
	char szGameType[30] = {};
	char szCountNum[32] = { 0 };
	char szCurNum[32] = {};
	char szUser[32] = { 0 };
	char szImageStr[255] = { 0 };
	char szState[255] = {};
	char szZMode[100] = {};
	char szGMode[100] = {};
	string szCover = "";
	char szRoomInfo[255] = {};
	sprintf(szGameType, GetGameString("gameType_%d"), gameType);
	for (int i = 0; i < sizeRecord; i++)
	{
		auto temp = m_recordLayout->clone();
		temp->setVisible(true);
		m_listView->pushBackCustomItem(temp);

		auto textRoomNum = dynamic_cast<Text*>(temp->getChildByName("Text_RoomNum"));
		if (textRoomNum)
			textRoomNum->setString(StringTool::intToString(taRoomVec.at(i)->getRoomNum()));
		sprintf(szState, GetGameString("gameState_%d"), taRoomVec.at(i)->getRoomState() + 1);
		auto textIfEnd= dynamic_cast<Text*>(temp->getChildByName("Text_IF_End"));
		if (textIfEnd)
			textIfEnd->setString(GetGameString(szState));
		auto image_State = dynamic_cast<ImageView*>(temp->getChildByName("Image_State"));
		if (image_State)
		{
			sprintf(szState, "InsteadRecord/state_%d.png", taRoomVec.at(i)->getRoomState() + 1);
			image_State->loadTexture(szState);
		}
		sprintf(szZMode, GetGameString("GameMode%d"), taRoomVec.at(i)->getVXRoom()->getZMode());
		switch (taRoomVec.at(i)->getVXRoom()->getMMode())
		{
		case 0:
			sprintf(szGMode, GetGameString("game_mode_%d"), taRoomVec.at(i)->getVXRoom()->getGameMode());
			break;
		case 1:
			sprintf(szGMode, GetGameString("game_add_%d"), taRoomVec.at(i)->getVXRoom()->getMMode());
			break;
		default:
			break;
		}	
        int kCardway = taRoomVec.at(i)->getVXRoom()->getCover();
		switch (kCardway)
		{
		case 1:
			szCover = "card_way1";
			break;
		case 2:
			szCover = "card_way2";
			break;
		case 5:
			szCover = "card_way3";
			break;
		default:
			break;
		}
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			sprintf(szRoomInfo, GetGameString("roomInfo"), taRoomVec.at(i)->getVXRoom()->getPlayMode(),
				GetGameString(szGMode), GetGameString(szZMode));
		}
		else
		{
			sprintf(szRoomInfo, GetGameString("roomImage"), GetGameString(szGameType), taRoomVec.at(i)->getVXRoom()->getPlayMode(),
				GetGameString(szZMode), GetGameString(szCover));
		}
		auto text_RoomInfo = dynamic_cast<Text*>(temp->getChildByName("Text_RoomInfo"));
		if (text_RoomInfo)	text_RoomInfo->setString(szRoomInfo);
		auto textTime = dynamic_cast<Text*>(temp->getChildByName("Text_Time"));
		if (textTime)	textTime->setString(taRoomVec.at(i)->getStrTime());

		auto m_userVec = taRoomVec.at(i)->getUserVec();
		auto sizeUser = m_userVec.size();
		m_curNum = GAMECONFIG.m_max_num;
		for (int index = 0; index < sizeUser; index++)
		{
			auto userDate = m_userVec.at(index);
			if (userDate->getUserId() == 0)
			{
				m_curNum--;
				continue;
			}
			sprintf(szUser, "Text_Name_%d", index + 1);
			auto textUserName = dynamic_cast<Text*>(temp->getChildByName(szUser));
			if (textUserName)
			{
				textUserName->setVisible(true);
				textUserName->setString(userDate->getNickName());
			}
			sprintf(szUser, "Text_ID_%d", index + 1);
			auto textUserID = dynamic_cast<Text*>(temp->getChildByName(szUser));
			if (textUserID)
			{
				if (takeType == 0)
				{
					sprintf(szUser, " [ID:%d]", userDate->getUserId());
				}
				else
				{
					sprintf(szUser, "ID:%d", userDate->getUserId());
				}
				textUserID->setVisible(true);
				textUserID->setString(szUser);
			}
			sprintf(szImageStr, "Panel_Head_%d", index + 1);
			//显示头像
			auto m_userHead = dynamic_cast<Layout*>(temp->getChildByName(szImageStr));
			if (!m_userHead)
				continue;
			m_userHead->setVisible(true);
			m_userHead->removeAllChildren();
			WebSprite *pPortrait = nullptr;
			pPortrait = WebSprite::create(m_userVec.at(index)->getImageName(), Size(150.0f, 150.0f));
			if (pPortrait)
			{
				pPortrait->setLocalZOrder(-1);
				m_userHead->addChild(pPortrait);
				pPortrait->setPosition(m_userHead->getContentSize().width / 2, m_userHead->getContentSize().height / 2);
			}
		}
		auto text_CurNum = dynamic_cast<Text*>(temp->getChildByName("Text_CurNum"));
		if (text_CurNum)
		{
			sprintf(szCurNum, "%d/%d", m_curNum, GAMECONFIG.m_max_num);
			text_CurNum->setString(szCurNum);
		}
		auto button_Invite = dynamic_cast<Button*>(temp->getChildByName("Button_Invite"));
		if (button_Invite)
		{
			button_Invite->setTag(i);
			button_Invite->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchInviteEvent, this));
		}

		auto button_Dismiss = dynamic_cast<Button*>(temp->getChildByName("Button_Dismiss"));
		if (button_Dismiss)
		{
			button_Dismiss->setTag(i);
			button_Dismiss->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchDismissEvent, this));
		}

		auto button_Copy = dynamic_cast<Button*>(temp->getChildByName("Button_Copy"));
		if (button_Copy)
		{
			button_Copy->setTag(i);
			button_Copy->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchCopyEvent, this));
		}

		auto button_Invite_0 = dynamic_cast<Button*>(temp->getChildByName("Button_Invite_0"));
		if (button_Invite_0)
		{
			button_Invite_0->setTag(i);
			button_Invite_0->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchInviteEvent, this));
		}

		auto button_Kick = dynamic_cast<Button*>(temp->getChildByName("Button_Kick"));
		if (button_Kick)
		{
			if (taRoomVec.at(i)->getRoomState() == 0)
			{
				button_Kick->setVisible(true);
			}
			else
			{
				button_Kick->setVisible(false);
			}
			button_Kick->setTag(i);
			button_Kick->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchKickEvent, this));
		}

		auto button_Enter = dynamic_cast<Button*>(temp->getChildByName("Button_Enter"));
		if (button_Enter)
		{
			if (taRoomVec.at(i)->getRoomState() == 0)
			{
				button_Enter->setVisible(true);
			}
			else
			{
				button_Enter->setVisible(false);
			}
			button_Enter->setTag(i);
			button_Enter->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchEnterRoomEvent, this));
		}

		auto button_Warn = dynamic_cast<Button*>(temp->getChildByName("Button_Warn"));
		if (button_Warn)
		{
			if (taRoomVec.at(i)->getRoomState() == 1)
			{
				button_Warn->setVisible(true);
			}
			else
			{
				button_Warn->setVisible(false);
			}
			button_Warn->setTag(i);
			button_Warn->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchWarnEvent, this));
		}

		auto text_CostCard = dynamic_cast<Text*>(temp->getChildByName("Text_CostCard"));
		auto text_CostNum = dynamic_cast<Text*>(temp->getChildByName("Text_CostNum"));
		if (text_CostNum)
		{
			int costNum = 0;
			switch (taRoomVec.at(i)->getVXRoom()->getPlayMode())
			{
			case 5:
				costNum = instead_CostCard[0];
				break;
			case 10:
				costNum = instead_CostCard[1];
				break;
			case 15:
				costNum = instead_CostCard[4];
				break;
			case 20:
				costNum = instead_CostCard[2];
				break;
			case 30:
				costNum = instead_CostCard[3];
				break;
			default:
				break;
			}
			text_CostNum->setString(StringTool::intToString(costNum));
		}

		switch (taRoomVec.at(i)->getRoomState())
		{
		case 0:
			button_Invite->setVisible(true);
			//button_Dismiss->setVisible(true);
			break;
		case 1:
			if (button_Invite_0)
			{
				button_Invite_0->setVisible(true);
			}
			//button_Dismiss->setTouchEnabled(false);
			//button_Dismiss->setEnabled(false);
			break;
		case 2:
			text_CostCard->setVisible(true);
			text_CostNum->setVisible(true);
			break;
		default:
			break;
		}
	}
	if (m_refreshButton)
	{
		m_refreshButton->setEnabled(true);
	}
}
//触碰事件  （单点触碰）
bool InsteadRecord::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void InsteadRecord::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void InsteadRecord::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void InsteadRecord::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

void InsteadRecord::touchKickCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	if (m_kickLayout)
	{
		for (int index = 0; index < GAMECONFIG.m_max_num; index++)
		{
			char szUser[32] = {};
			sprintf(szUser, "Text_Name_%d", index + 1);
			auto textUserName = dynamic_cast<Text*>(m_kickLayout->getChildByName(szUser));
			textUserName->setVisible(false);
			sprintf(szUser, "Text_ID_%d", index + 1);
			auto textUserID = dynamic_cast<Text*>(m_kickLayout->getChildByName(szUser));
			if (textUserID)
			{
				textUserID->setVisible(false);
			}
			char szImageStr[32] = {};
			sprintf(szImageStr, "Panel_Head_%d", index + 1);
			//显示头像
			auto m_userHead = dynamic_cast<Layout*>(m_kickLayout->getChildByName(szImageStr));
			if (!m_userHead)
				continue;
			m_userHead->setVisible(false);
			m_userHead->removeAllChildren();
		}
		m_kickLayout->setVisible(false);
	}
}

//代开记录
void InsteadRecord::touchRecordEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto curButton = (Button*)sender;
	if (m_buttonCur == curButton)
		return;
	curButton->setEnabled(false);
	m_buttonCur->setEnabled(true);
	m_buttonCur = curButton;
	
	switch (m_buttonCur->getTag())
	{
	case 1:
	{
		if (m_imageTab)
			m_imageTab->loadTexture("InsteadRecord/002.png");
		if (m_endLayout)
		{
			m_endLayout->setVisible(false);
		}
		if (m_nEndLayout)
		{
			m_nEndLayout->setVisible(true);
			m_recordLayout = dynamic_cast<Layout*>(m_nEndLayout->getChildByName("Panel_Record"));
			if (m_recordLayout)
			{
				m_recordLayout->setVisible(false);
			}
			m_listView = dynamic_cast<ListView*>(m_nEndLayout->getChildByName("ListView_Record"));
		}
	}
		
		break;
	case 2:
	{
		if (m_imageTab)
			m_imageTab->loadTexture("InsteadRecord/001.png");
		if (m_nEndLayout)
		{
			m_nEndLayout->setVisible(false);
		}
		if (m_endLayout)
		{
			m_endLayout->setVisible(true);
			m_recordLayout = dynamic_cast<Layout*>(m_endLayout->getChildByName("Panel_Record"));
			if (m_recordLayout)
			{
				m_recordLayout->setVisible(false);
			}

			m_listView = dynamic_cast<ListView*>(m_endLayout->getChildByName("ListView_Record"));
		}
	}
		
		break;
	default:
		break;
	}
	m_listView->removeAllChildrenWithCleanup(true);
	MsgHallRequest::sendGetTake(1, curButton->getTag() - 1);
}

void InsteadRecord::touchRefreshEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
	m_refreshButton->setEnabled(false);
	m_listView->removeAllChildrenWithCleanup(true);
	MsgHallRequest::sendGetTake(1, 0);
}

void InsteadRecord::touchInviteEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto curButton = (Button*)sender;
	auto index = curButton->getTag();
	auto taRoomVec = QHallManager::getInstance()->m_vecTARoom_;
	unsigned int roomNum = taRoomVec.at(index)->getRoomNum();
	//int roomId = QGameManager::getInstance()->getRoomNum();
	char szType[32] = { 0 };
	sprintf(szType, "WXInvite_%d", GAMECONFIG.m_nAppType);
	char szTitle[512] = { 0 };
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		sprintf(szTitle, GetGameString(szType), roomNum, m_curNum, GAMECONFIG.m_max_num);
	}
	else
	{
		sprintf(szTitle, GetGameString(szType), roomNum);
	}
	char szGameMode[256] = { 0 };
	int mode = taRoomVec.at(index)->getVXRoom()->getZMode();
	sprintf(szGameMode, "GameMode%d", mode);
	char szWfMode[256] = { 0 };
	int gameMode = taRoomVec.at(index)->getVXRoom()->getGameMode();
	int mMode = taRoomVec.at(index)->getVXRoom()->getMMode();
	switch (mMode)
	{
	case 0:
		sprintf(szWfMode, "game_mode_%d", gameMode);
		break;
	case 1:
		sprintf(szWfMode, "game_add_%d", mMode);
		break;
	default:
		break;
	}
	int round = taRoomVec.at(index)->getVXRoom() ->getPlayMode();
	char szCost[32] = { 0 };
	int costType = taRoomVec.at(index)->getVXRoom() ->getPayMode();
	sprintf(szCost, "room_cost%d", costType + 1);
	string szCardway = "";
	int kCardWay = taRoomVec.at(index)->getVXRoom()->getCover();
	switch (kCardWay)
	{
	case 1:
		szCardway = "card_way1";
		break;
	case 2:
		szCardway = "card_way2";
		break;
	case 5:
		szCardway = "card_way3";
		break;
	default:
		break;
	}
	char szDes[512] = { 0 };
	/*if (GAMECONFIG.m_nAppType == GAME_TYPE_XXNN)
	{
		sprintf(szDes, GetGameString("WXInvite2"), GetGameString(szCardway), GetGameString(szGameMode), round, GetGameString(szCost));
	}*/
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		sprintf(szDes, GetGameString("WXInvite2"), GetGameString(szWfMode), GetGameString(szGameMode), round);
	}
	else{
		sprintf(szDes, GetGameString("WXInvite2"), roomNum, GetGameString(szGameMode), round, GetGameString(szCost));
	}

	WxSocial::getInstance()->shareRoom(GAMECONFIG.m_sHttpDownUrl.c_str(), szTitle, szDes, CC_CALLBACK_1(InsteadRecord::onInviteCB, this));
}

void InsteadRecord::touchEnterRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto curButton = (Button*)sender;
	auto index = curButton->getTag();
	auto taRoomVec = QHallManager::getInstance()->m_vecTARoom_;
	m_curRoomNum = taRoomVec.at(index)->getRoomNum();
	QHallManager::getInstance()->setRoomNum(m_curRoomNum);
	QHallManager::getInstance()->setJoinType(1);
	//this->removeFromParentAndCleanup(true);
	//WWSceneManager::getInstance()->openScene(QGameScene::create());
	//deng dai
	WWSceneManager::getInstance()->openGlobalUI(Load::create());
	MsgHallRequest::sendEnterRoom(m_curRoomNum & 0x00FFFFFF);
}

void InsteadRecord::touchKickEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	//m_kickLayout->removeAllChildrenWithCleanup(true);
	m_kickLayout->setVisible(true);
	auto curButton = (Button*)sender;
	auto index = curButton->getTag();
	auto taRoomVec = QHallManager::getInstance()->m_vecTARoom_;
	m_curRoomNum = taRoomVec.at(index)->getRoomNum();
	auto m_userVec = taRoomVec.at(index)->getUserVec();
	auto sizeUser = m_userVec.size();
	for (int index = 0; index < sizeUser; index++)
	{
		auto userDate = m_userVec.at(index);
		if (userDate->getUserId() == 0)
		{
			continue;
		}
		char szUser[32] = {};
		sprintf(szUser, "Text_Name_%d", index + 1);
		auto textUserName = dynamic_cast<Text*>(m_kickLayout->getChildByName(szUser));
		textUserName->setVisible(true);
		textUserName->setString(userDate->getNickName());
		sprintf(szUser, "Text_ID_%d", index + 1);
		auto textUserID = dynamic_cast<Text*>(m_kickLayout->getChildByName(szUser));
		if (textUserID)
		{
			sprintf(szUser, "ID:%d", userDate->getUserId());
			textUserID->setVisible(true);
			textUserID->setString(szUser);
		}
		char szImageStr[32] = {};
		sprintf(szImageStr, "Panel_Head_%d", index + 1);
		//显示头像
		auto m_userHead = dynamic_cast<Layout*>(m_kickLayout->getChildByName(szImageStr));
		if (!m_userHead)
			continue;
		m_userHead->setVisible(true);
		m_userHead->removeAllChildren();
		WebSprite *pPortrait = nullptr;
		pPortrait = WebSprite::create(m_userVec.at(index)->getImageName(), Size(150.0f, 150.0f));
		pPortrait->setLocalZOrder(-1);
		m_userHead->addChild(pPortrait);
		pPortrait->setPosition(m_userHead->getContentSize().width / 2, m_userHead->getContentSize().height / 2);
		//点击头像
		m_userHead->setTag(index);
		m_userHead->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchHeadEvent, this));
	}
}

void InsteadRecord::touchWarnEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	//m_kickLayout->removeAllChildrenWithCleanup(true);
	auto curButton = (Button*)sender;
	auto index = curButton->getTag();
	auto taRoomVec = QHallManager::getInstance()->m_vecTARoom_;
	m_curRoomNum = taRoomVec.at(index)->getRoomNum();
	QHALLMGR->setWarnRoomNum(m_curRoomNum);
	QHALLMGR->setJoinType(5);
	WWSceneManager::getInstance()->openGlobalUI(Load::create());
	int gameType = 1;//创建房游戏类型填（牛牛1，扎金花2）
	MsgHallRequest::sendEnterRoom(gameType << 24 & 0xFF000000);
	curButton->setEnabled(false);
	Action* action = Sequence::create(DelayTime::create(5.0f), CallFunc::create(
		[=]{
		LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
		if (curButton)
		{
			curButton->setEnabled(true);
		}
	}), nullptr);
	action->setTag(0);
	if (getActionByTag(0) == nullptr)
	{
		curButton->runAction(action);
	}
}

void InsteadRecord::touchHeadEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto curLayout = (Layout*)sender;
	auto index = curLayout->getTag();
	QHALLMGR->setKickRoomNum(m_curRoomNum);
	QHALLMGR->setKickSeatID(index);
	QHALLMGR->setJoinType(4);
	WWSceneManager::getInstance()->openGlobalUI(Load::create());
	int gameType = 1;//创建房游戏类型填（牛牛1，扎金花2）
	MsgHallRequest::sendEnterRoom(gameType << 24 & 0xFF000000);
}

void InsteadRecord::onInviteCB(int errorCode)
{
	return;
}

void InsteadRecord::touchDismissEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto curButton = (Button*)sender;
	auto index = curButton->getTag();
	Prompt* prompt = Prompt::create(2, GetGameString("exit_room"));
	auto trueButton = dynamic_cast<Button*>(prompt->m_systemLayout->getChildByName("Button_True"));
	if (trueButton)
	{
		trueButton->setTag(index);
		trueButton->addTouchEventListener(CC_CALLBACK_2(InsteadRecord::touchTrueEvent, this));
	}
	WWSceneManager::getInstance()->openGlobalUI(prompt);
}

void InsteadRecord::touchCopyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto curButton = (Button*)sender;
	auto index = curButton->getTag();
	auto taRoomVec = QHallManager::getInstance()->m_vecTARoom_;
	m_curRoomNum = taRoomVec.at(index)->getRoomNum();

	char str[50] = {};
	const char *roomNum = StringTool::intToString(m_curRoomNum).c_str();
	strcpy(str, roomNum);
	int code = 2;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	code = AdmobJni::copyToClipboard(str);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	code = iOSAdapterUtility::getInstance()->copyToClipboard(str);
#endif
	if (code == -1)//失败	
	{
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("Copy_Fail"), 30, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
	}
	if (code == 0)//成功
	{
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("Copy_Success"), 30, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
	}
}

void InsteadRecord::touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto curButton = (Button*)sender;
	auto index = curButton->getTag();
	auto taRoomVec = QHallManager::getInstance()->m_vecTARoom_;
	unsigned int roomNum = taRoomVec.at(index)->getRoomNum();
	QHALLMGR->setDisRoomNum(roomNum);
	QHALLMGR->setJoinType(3);
	WWSceneManager::getInstance()->openGlobalUI(Load::create());
	int gameType = 1;//创建房游戏类型填（牛牛1，扎金花2）
	MsgHallRequest::sendEnterRoom(gameType << 24 & 0xFF000000);
	WWSceneManager::getInstance()->closeGlobalUIByName("prompt");
}
