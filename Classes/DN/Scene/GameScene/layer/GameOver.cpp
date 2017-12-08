#include "GameOver.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../../net/LordNetManger.h"
#include "../../HallScene/QHallScene.h"
#include "ScriptDataManager.h"
#include "../../HallScene/data/QHallManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "WebSprite.h"
#else
#include "GameNode/WebSprite.h"
#endif
#include "../data/QGameBaseData.h"
#include "../data/QGameManager.h"
#include "StringTool.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "GameConfig.h"
#include "WxSocial.h"
using namespace std;

USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool GameOverLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto mainNode = CSLoader::createNode("GameOverLayer.csb");
	this->addChild(mainNode, 0, 1);
	layout = dynamic_cast<Layout*>(mainNode->getChildByName("Panel"));
	auto text_RoomNum = dynamic_cast<Text*>(layout->getChildByName("Text_RoomNum"));
	char dzRoom[32] = { 0 };
	sprintf(dzRoom, GetGameString("show_roomNum"), QGameManager::getInstance()->getRoomNum());
	if (text_RoomNum)
	{
		text_RoomNum->setString(dzRoom);
	}
	auto text_WF = dynamic_cast<Text*>(layout->getChildByName("Text_WF"));
	if (text_WF)
	{
		switch (QHALLMGR->getMMode())
		{
		case 0:
			text_WF->setString(GetGameString("game_mode_3"));
			break;
		case 1:
			text_WF->setString(GetGameString("game_add_1"));
			break;
		default:
			break;
		}
	}
	auto text_ZMode = dynamic_cast<Text*>(layout->getChildByName("Text_ZMode"));
	if (text_ZMode)
	{
		switch (QHallManager::getInstance()->getZMode())
		{
		case 0:
			text_ZMode->setString(GetGameString("BM_LOOT"));
			break;
		case 1:
			text_ZMode->setString(GetGameString("BM_TURN"));
			break;
		case 2:
			text_ZMode->setString(GetGameString("BM_SURE"));
			break;
		case 3:
			text_ZMode->setString(GetGameString("BM_NO"));
			break;
		case 4:
			text_ZMode->setString(GetGameString("BM_KING"));
			break;
		case 5:
			text_ZMode->setString(GetGameString("BM_TIMES"));
			break;
		case 6:
			text_ZMode->setString(GetGameString("BM_NULL"));
			break;
		default:
			break;
		}
	}

	auto text_PlayMode = dynamic_cast<Text*>(layout->getChildByName("Text_PlayMode"));
	if (text_PlayMode)
	{
		switch (QHallManager::getInstance()->getPlayMode())
		{
		case 5:
			text_PlayMode->setString(GetGameString("playMode_1"));
			break;
		case 10:
			text_PlayMode->setString(GetGameString("playMode_2"));
			break;
		case 15:
			text_PlayMode->setString(GetGameString("playMode_5"));
			break;
		case 20:
			text_PlayMode->setString(GetGameString("playMode_3"));
			break;
		case 30:
			text_PlayMode->setString(GetGameString("playMode_4"));
			break;
		default:
			break;
		}
	}
	auto text_CardWay = dynamic_cast<Text*>(layout->getChildByName("Text_CardWay"));
	if (text_CardWay)
	{
		switch (QHallManager::getInstance()->getKcardWay())
		{
		case 1:
			text_CardWay->setString(GetGameString("card_way1"));
			break;
		case 5:
			text_CardWay->setString(GetGameString("card_way3"));
			break;
		case 2:
			text_CardWay->setString(GetGameString("card_way2"));
			break;
		default:
			break;
		}
	}
	auto text_MaxBet = dynamic_cast<Text*>(layout->getChildByName("Text_MaxBet"));
	if (text_MaxBet)
	{
		text_MaxBet->setString(StringTool::intToString(QHALLMGR->getMaxBet()));
	}
	auto text_Time = dynamic_cast<Text*>(layout->getChildByName("Text_Time"));
	if (text_Time)
	{
		text_Time->setString(QGameManager::getInstance()->getStrTime());
	}
	auto text_Info = dynamic_cast<Text*>(layout->getChildByName("Text_Info"));
	if (text_Info)
	{
		char szRoomInfo[50] = {};
		char szGameMode[50] = {};
		int gameMode = QHALLMGR->getGameMode();
		int mMode = QHALLMGR->getMMode();
		switch (mMode)
		{
		case 0:
			sprintf(szGameMode, "game_mode_%d", gameMode);
			break;
		case 1:
			sprintf(szGameMode, "game_add_%d", mMode);
			break;
		default:
			break;
		}
		sprintf(szRoomInfo, GetGameString("roomOverInfo"), QGameManager::getInstance()->getRoomNum(), GetGameString(szGameMode), QHALLMGR->getPlayMode());
		text_Info->setString(szRoomInfo);
	}
	auto listView = dynamic_cast<ListView*>(layout->getChildByName("ListView"));
	m_listView1 = dynamic_cast<ListView*>(listView->getChildByName("ListView_1"));
	m_listView2 = dynamic_cast<ListView*>(listView->getChildByName("ListView_2"));
	m_listView3 = dynamic_cast<ListView*>(listView->getChildByName("ListView_3"));
	m_tempLayout = dynamic_cast<Layout*>(layout->getChildByName("Panel_0"));
	m_tempLayout->setVisible(false);

	auto replay = dynamic_cast<Button*>(layout->getChildByName("Button_Replay"));
	replay->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::touchReplayEvent, this));
    
	auto share = dynamic_cast<Button*>(layout->getChildByName("Button_Share"));
    share->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::touchShareEvent, this));
    

    return true;
}

void GameOverLayer::onRefreshData(QUserData* m_player[])
{
	m_listView1->removeAllItems();
	m_listView2->removeAllItems();
	m_listView3->removeAllItems();
	char szName[100] = { 0 };
	auto vecOver = QGameManager::getInstance()->m_vecOver_;
	int max = vecOver.at(0)->getScore();
	int min = vecOver.at(0)->getScore();
	for (int i = 0; i < vecOver.size() - 1; i++)
	{
		int seatId = vecOver.at(i)->getSeatId();
		int index = (seatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
		auto player = m_player[index];
		auto userData = vecOver.at(i);
		if (userData != nullptr&&player != nullptr)
		{
			if (max < vecOver.at(i+1)->getScore())
			{
				max = vecOver.at(i+1)->getScore();
			}
			if (min > vecOver.at(i + 1)->getScore())
			{
				min = vecOver.at(i + 1)->getScore();
			}
		}
	}
	int iNum = 0;//显示个数
	for (int i = 0; i < vecOver.size(); i++)
	{
		int seatId = vecOver.at(i)->getSeatId();
		int index = (seatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
		auto player = m_player[index];
		auto userData = vecOver.at(i);
		if (userData != nullptr&&player != nullptr)
		{
			Layout* single = nullptr;
			single = (Layout*)m_tempLayout->clone();
			if (iNum < 3)
			{
				m_listView1->pushBackCustomItem(single);
			}
			else if (iNum >= 3 && iNum < 6)
			{
				m_listView2->pushBackCustomItem(single);
			}
			else
			{
				m_listView3->pushBackCustomItem(single);
			}
			if (!single) return;
			single->setVisible(true);
			auto m_userHead = dynamic_cast<Layout*>(single->getChildByName("Panel_head"));
			if (m_userHead)
			{
				m_userHead->removeAllChildren();
				auto pPortrait = WebSprite::create(player->getStrImage(), Size(100.0f, 100.0f));
				if (pPortrait)
				{
					pPortrait->setLocalZOrder(-1);
					m_userHead->addChild(pPortrait);
					pPortrait->setPosition(m_userHead->getContentSize().width / 2, m_userHead->getContentSize().height / 2);
				}
			}

			auto panelName = dynamic_cast<Layout*>(single->getChildByName("Panel_Name"));
			auto textName = dynamic_cast<Text*>(panelName->getChildByName("Text_Name"));
			if (textName)
			{
				textName->setString(player->getNickName());
			}
			auto textUid = dynamic_cast<Text*>(single->getChildByName("Text_UID"));
			if (textUid)
			{
				char dzUID[32] = { 0 };
				sprintf(dzUID, GetGameString("show_UID"), player->getUserId());
				textUid->setString(dzUID);
			}
			auto allKill = dynamic_cast<Text*>(single->getChildByName("Text_AllKillTimes"));
			if (allKill)
			{
				allKill->setString(StringTool::intToString(userData->getKillCount()));
			}
			auto haveNiu = dynamic_cast<Text*>(single->getChildByName("Text_HaveNiuTimes"));
			if (haveNiu)
			{
				haveNiu->setString(StringTool::intToString(userData->getCowCount()));
			}
			auto victory = dynamic_cast<Text*>(single->getChildByName("Text_VictoryTimes"));
			if (victory)
			{
				victory->setString(StringTool::intToString(userData->getWinCount()));
			}
			auto winner = dynamic_cast<ImageView*>(single->getChildByName("Image_Win"));
			if (winner)
			{
				if (userData->getScore() > 0)
				{
					winner->setVisible(true);
				}
			}
			auto totalScore = dynamic_cast<Text*>(single->getChildByName("Text_TotalScoreNum"));
			if (totalScore)
			{
				totalScore->setString(StringTool::intToString(userData->getScore()));
			}
			auto imageFZ = dynamic_cast<ImageView*>(single->getChildByName("Image_FZ"));
			if (imageFZ)
			{
				if (seatId == 0) imageFZ->setVisible(true);
			}
			auto spriteBg = dynamic_cast<ImageView*>(single->getChildByName("Image_Bg"));
			if (spriteBg)
			{
				auto imageView_Score = dynamic_cast<ImageView*>(single->getChildByName("Image_Score"));
				if (userData->getScore() < 0)
				{
					if (spriteBg)
					{
						spriteBg->loadTexture("res/GameOver/3.png");
					}
					if (imageView_Score)
					{
						imageView_Score->loadTexture("res/GameOver/score_lose.png");
					}
				}
				else
				{
					if (spriteBg)
					{
						spriteBg->loadTexture("res/GameOver/2.png");
					}
					if (imageView_Score)
					{
						imageView_Score->loadTexture("res/GameOver/score_win.png");
					}
				}
			}
			for (int j = 0; j < 20; j++)
			{
				char niuName[50] = {};
				sprintf(niuName, "Text_N%d_0", j);
				auto textNiu = dynamic_cast<Text*>(single->getChildByName(niuName));
				if (textNiu)
				{
					textNiu->setString(StringTool::intToString(userData->getXiVec().at(j)));
				}
			}
			iNum++;
		}
		auto imageView_SF = dynamic_cast<ImageView*>(layout->getChildByName("Image_SF"));
		if (imageView_SF && QGameManager::getInstance()->getSeatId() == userData->getSeatId() && (int)userData->getScore() < 0)
		{
			imageView_SF->loadTexture("res/GameOver/10.png");
		}
		if (imageView_SF && QGameManager::getInstance()->getSeatId() == userData->getSeatId() && (int)userData->getScore() >= 0)
		{
			imageView_SF->loadTexture("res/GameOver/9.png");
		}
	}
}

void GameOverLayer::touchShareEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
    WxSocial::getInstance()->shareCaptureScreen(GAMECONFIG.m_sHttpDownUrl.c_str(), "", "", CC_CALLBACK_1(GameOverLayer::onInviteCB, this));
}
//开始新的一局游戏
void GameOverLayer::touchReplayEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WWSceneManager::getInstance()->closeAllUI();
	LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
	if (Director::getInstance()->getRunningScene()->getName() != "QHallScene")
		WWSceneManager::getInstance()->openScene(QHallScene::create());
}

void GameOverLayer::onInviteCB(int errorCode)
{

}



