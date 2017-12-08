 #include "QGameLayer.h"
#include "cocostudio/CocoStudio.h"

#include "../../../GUISystem/WWSceneManager.h"
#include "../../../net/LordNetManger.h"
#include "../../../ScriptDataManager.h"
#include "../../Comm/request/MsgGameRequest.h"
#include "../QDNEvent.h"
#include "../HallScene/data/QHallManager.h"
#include "../HallScene/QHallScene.h"
#include "../HallScene/layer/SetLayer.h"
#include "../HallScene/layer/Information.h"
#include "../GlobalLayer/DialogLayer.h"
#include "../GlobalLayer/PopupLayer.h"
#include "../GlobalLayer/PopLayer.h"
#include "layer/TalkLayer.h"
#include "layer/GameOver.h"
#include "layer/Speek.h"
#include "layer/GameVoiceRecordLayer.h"
#include "data/QGameManager.h"
#include "layer/AgreeLayer.h"
#include "layer/DismissLayer.h"
#include "layer/LookPocker.h"

#include "WxSocial.h"
#include "GameConfig.h"
#include "ProceConfig.h"
#include "GameAudioEngine.h"
#include "StringTool.h"
#include "WWMacros.h"

#include <SimpleAudioEngine.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "WebSprite.h"
#else
#include "GameNode/WebSprite.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "AdmobJni.h"
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSAdapterUtility.hpp"
#endif


#include <time.h> 
using namespace std;

int QGameLayer::COUNT = 0;
int QGameLayer::sendIndex = 0;
int QGameLayer::indexCount = 0;

QGameLayer::QGameLayer()
{
	m_gameState = 0;
	m_countNum = nullptr;
	m_readyLayout = nullptr;
	m_grapLayout = nullptr;
	m_chipLayout = nullptr;
	memset(m_coutPockNum, NULL, sizeof(m_coutPockNum));
	memset(m_player, NULL, sizeof(m_player));
	m_showResult = nullptr;
	m_PocketPos = nullptr;

	m_systemTime = nullptr;
	m_exitRoom = nullptr;
	m_agreeMent = nullptr;
	m_dismiss = nullptr;
	m_lookPocker = nullptr;
	m_gameOverlayer = nullptr;
	m_running = nullptr;
	m_WarnLayer = nullptr;
	m_voiceLayer = nullptr;
	m_lightLayout = nullptr;
	m_ligthDealer = nullptr;
	m_popupLayer = nullptr;

	m_showBg = nullptr;

	_charVoicePlay = false;

	m_ctorWin = false;

	m_ZBet = nullptr;

	m_curNum = 0;

	m_btnQuery = nullptr;
	m_btnContinue = nullptr;

	for (int index = 0; index < 6;index++)
	{
		m_scoreSum[index] = 0;
	}
    m_bCanSendSpeek = true;
	m_ligthDealer = Sprite::create("Light/light1.png");
	m_ligthDealer->setVisible(false);
	addChild(m_ligthDealer, 1);
}
QGameLayer::~QGameLayer()
{
	for (int i = 0; i < _queueCharVoice.size(); i++) {
		QGameVoiceData *data =  _queueCharVoice.front();
		_queueCharVoice.pop();
		CC_SAFE_DELETE(data);
	}
	for (int i = 0; i < GAMECONFIG.m_max_num; i++)
	{
		if (m_player[i])
		{
			CC_SAFE_DELETE(m_player[i]);
		}
	}
	m_vecLight.clear();
    QGameManager::getInstance()->clearData();
}
bool QGameLayer::init()
{
	auto pListener = EventListenerCustom::create("ON_NETWORK_DISCONNECTED", [&](EventCustom* event){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        long iNetType = reinterpret_cast<long>(event->getUserData());
#else
        int iNetType = reinterpret_cast<int>(event->getUserData());
#endif
		if (iNetType == WWNetwork::eGameNet) {
			QHALLMGR->setBackType(1);
		}
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	TextureCache::getInstance()->addImage("GameScene/20.png");

	m_bIsStart = false;
	m_pIsSendPocke = false;
	m_bIsGameBack = false;
	isGameOver = false;
	CoutSum = 0;
	m_isPlay = -1;
	auto mainLayer = CSLoader::createNode("GameScene.csb");
	addChild(mainLayer);

	auto g_TMode = QHALLMGR->getPlayMode();		//局数的模式
	auto g_BMode = QHALLMGR->getZMode();		//坐庄的模式
	auto g_PMode = QHALLMGR->getCostMode();		//付卡的模式
	auto g_GMode = QHALLMGR->getGameMode();		//游戏的模式（0经典,1坎顺,2银牛,3顺金）
	auto g_MMode = QHALLMGR->getMMode();		// 倍数的模式(0普通倍数,1超级倍数)
	auto g_Cover = QHALLMGR->getKcardWay();		// 盖住几张牌
	auto g_MaxBet = QHALLMGR->getMaxBet();		// 最大下注/下注模式
	auto g_MinCard = QHALLMGR->getMinCard();	// 最少房卡(大于等于此值才可以开房/加房)
	auto g_Half = QHALLMGR->getHalf();			// 半幅扑克(0完整的,1半幅的)
	auto g_Forbid = QHALLMGR->getForbid();		// 开局后禁止加入
	log("TMode:%d, BMode:%d, PMode:%d, GMode:%d, MMode:%d, Cover:%d, MaxBet:%d, MinCard:%d, Half:%d, Forbid:%d",
		g_TMode, g_BMode, g_PMode, g_GMode, g_MMode, g_Cover, g_MaxBet, g_MinCard, g_Half, g_Forbid);

	auto imageview_WF = dynamic_cast<ImageView*>(mainLayer->getChildByName("Image_WF"));
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL &&imageview_WF)
	{
		switch (g_MMode)
		{
		case 0:
			imageview_WF->loadTexture("res/GameScene/bg_2.png");
			break;
		case 1:
			imageview_WF->loadTexture("res/GameScene/bg_1.png");
			break;
		default:
			break;
		}
	}

	//GM隐蔽功能
	auto checkBox_MS = dynamic_cast<CheckBox*>(mainLayer->getChildByName("CheckBox_MS"));
	if (checkBox_MS)
	{
		checkBox_MS->setVisible(false);
	}

	auto checkBox_GM_Win = dynamic_cast<CheckBox*>(mainLayer->getChildByName("CheckBox_GM_Win"));
	auto text_GM_Win = dynamic_cast<Text*>(mainLayer->getChildByName("Text_GM_Win"));
	if (QHALLMGR->getUserType() == 1)
	{
		if (checkBox_GM_Win)
		{
			checkBox_GM_Win->setVisible(true);
			checkBox_GM_Win->addEventListener(CC_CALLBACK_2(QGameLayer::selectedCtorWinEvent, this));
		}
		if (text_GM_Win)
		{
			text_GM_Win->setVisible(true);
		}
	}

	m_PocketPos = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_ShowPocket0"));
	CC_ASSERT(m_PocketPos);
	for (int i = 1; i < 6; i++)
	{
		char szName[20] = { 0 };
		sprintf(szName, "Image_%d", i);
		auto pocket = dynamic_cast<ImageView*>(m_PocketPos->getChildByName(szName));
		if (pocket)
		{
			m_pocketUser.pushBack(pocket);
		}
	}
	//显示房间号
	auto roomText = dynamic_cast<Text*>(mainLayer->getChildByName("Text_RoomNum"));
	char dzRoom[100] = { 0 };
	sprintf(dzRoom, GetGameString("show_roomNum"), QGameManager::getInstance()->getRoomNum());
	if (roomText)
	{
		roomText->setString(dzRoom);
	}
	//显示局数
	m_gameNum = 0;
	m_countNum = dynamic_cast<Text*>(mainLayer->getChildByName("Text_CountNum"));
	sprintf(dzRoom, GetGameString("show_gameNum"), m_gameNum, g_TMode);
	if (m_countNum)
	{
		m_countNum->setString(dzRoom);
	}

	//显示时间
	m_systemTime = dynamic_cast<Text*>(mainLayer->getChildByName("Text_Time"));
	if (m_systemTime)
	{
		m_systemTime->setString(this->gettimecurrent());
	}

	//显示坐庄模式
	auto msText = dynamic_cast<Text*>(mainLayer->getChildByName("Text_MS"));
	if (msText)
	{
		char szName[50] = {};
		sprintf(szName, "GameMode%d", g_BMode);
		msText->setString(GetGameString(szName));
	}

	m_ZBet = dynamic_cast<ImageView*>(mainLayer->getChildByName("Image_ZBet"));
	if (m_ZBet)
	{
		m_ZBet->setVisible(false);
	}

	//显示付费方式
	auto costText = dynamic_cast<Text*>(mainLayer->getChildByName("Text_CostMode"));
	if (costText)
	{
		switch (QHallManager::getInstance()->getCostMode())
		{
		case 0:
		{
			if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
			{
				costText->setString(GetGameString("game_mode_0"));
			}
			else
			{
				costText->setString(GetGameString("room_cost1"));
			}
		}
			break;
		case 1:
			if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
			{
				costText->setString(GetGameString("game_mode_1"));
			}
			else
			{
				costText->setString(GetGameString("room_cost2"));
			}
			break;
		default:
			break;
		}
	}

	//显示玩法模式
	auto wfText = dynamic_cast<Text*>(mainLayer->getChildByName("Text_WF"));
	if (wfText)
	{
		char szName[30] = {};
		if (g_MMode == 0)
			sprintf(szName, "game_mode_%d", g_GMode);
		else
			sprintf(szName, "game_add_%d", g_MMode);
		wfText->setString(GetGameString(szName));

	}

	//显示扣牌方式
	auto cardText = dynamic_cast<Text*>(mainLayer->getChildByName("Text_CardWay"));
	if (cardText)
	{
		switch (QHallManager::getInstance()->getKcardWay())
		{
		case 1:
			cardText->setString(GetGameString("card_way1"));
			break;
		case 2:
			cardText->setString(GetGameString("card_way2"));
			break;
		case 5:
			cardText->setString(GetGameString("card_way3"));
			break;
		default:
			break;
		}
	}

	//显示最大下注
	auto maxbetText = dynamic_cast<Text*>(mainLayer->getChildByName("Text_MaxBet"));
	if (maxbetText)
	{
		char szName[32] = {};
		sprintf(szName, GetGameString("max_bet"), g_MaxBet);

		maxbetText->setString(szName);
	}

	//显示游戏信息
	auto gameInfoText = dynamic_cast<Text*>(mainLayer->getChildByName("Text_GameInfo"));
	if (gameInfoText)
	{
		char hnName[50] = {};
		char nfName[50] = {};
		char qzName[50] = {};
		char infoName[200] = {};
		sprintf(hnName, "Half_%d", g_Half);
		sprintf(nfName, "NF_%d", g_MMode);
		sprintf(qzName, "GameMode%d", g_BMode);
		if (g_BMode == 0 && g_Cover == 1)
		{
			sprintf(qzName, "GameMode%d", 9);
		}
		sprintf(infoName, GetGameString("gameInfo"), GetGameString(hnName), GetGameString(nfName), GetGameString(qzName));
		gameInfoText->setString(infoName);
	}

	//隐藏解散，暂离，设置按钮
	auto checkBox_Show = dynamic_cast<CheckBox*>(mainLayer->getChildByName("CheckBox_Show"));
	if (checkBox_Show)
	{
		checkBox_Show->addEventListener(CC_CALLBACK_2(QGameLayer::selectedShowMenuEvent, this));
	}	
	

	//解散按钮
	m_dissolveButton = dynamic_cast<Button*>(mainLayer->getChildByName("Button_Dissolve"));
	if (m_dissolveButton)
	{
		m_dissolveButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchDismissEvent, this));
	}

	//提示牛的倍数按钮
	auto gzBtn = dynamic_cast<Button*>(mainLayer->getChildByName("Button_GZ"));
	if (gzBtn)
	{
		gzBtn->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchGZBtnEvent, this));
	}
    
    auto speekButton = dynamic_cast<Button*>(mainLayer->getChildByName("Button_Speek"));
    if (speekButton)
    {
		speekButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchSpeekEvent, this));
	}


	//设置按钮
	auto headButton = dynamic_cast<Button*>(mainLayer->getChildByName("Button_head"));
	if (headButton)
	{
		headButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchHeadEvent, this));
	}
	//聊天按钮
	auto talkButton = dynamic_cast<Button*>(mainLayer->getChildByName("Button_Talk"));
	if (talkButton)
	{
		talkButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchTalkEvent, this));
	}

	//查询小结战绩按钮
	m_btnQuery = dynamic_cast<Button*>(mainLayer->getChildByName("Button_Query"));
	if (m_btnQuery)
	{
		m_btnQuery->setVisible(false);
		m_btnQuery->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchQueryEvent, this));
	}
	//继续游戏按钮
	m_btnContinue = dynamic_cast<Button*>(mainLayer->getChildByName("Button_Continue"));
	if (m_btnContinue)
	{
		m_btnContinue->setVisible(false);
		m_btnContinue->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchContinueEvent, this));
	}

	m_startNext = dynamic_cast<ImageView*>(mainLayer->getChildByName("Image_StartNext"));
	if (m_startNext)
	{
		m_startNext->setVisible(false);
	}

	//楚天下注托管
	chipCheckBox = nullptr;
	m_checkChip = 0;
	
	//倒计时层
	m_RoomClock = RoomClock::create();
	auto clockPanle = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_clock"));
	if (clockPanle)
	{
		clockPanle->addChild(m_RoomClock);
		m_RoomClock->CancelTick();
	}

	//初始化图层
	char szName[100] = { 0 };
	char szName_1[100] = {};
	int index = GAMECONFIG.m_max_num;
	for (int i = 0; i < GAMECONFIG.m_max_num; i++)
	{
		sprintf(szName, "Panel_%d", i);
		//玩家头像
		auto layout = dynamic_cast<Layout*>(mainLayer->getChildByName(szName));
		if (layout)
			m_playerPanelVec.push_back(layout);
		//玩家牌层
		sprintf(szName_1, "Panel_ShowPocket%d", i);
		auto pokerLayout = dynamic_cast<Layout*>(mainLayer->getChildByName(szName_1));
		if (pokerLayout)
			m_palyerPockerVec.push_back(pokerLayout);
		m_scoreSum[i] = 0;
		//语音
		sprintf(szName, "Panel_Speak_%d", i);
		m_speakPos[i] = dynamic_cast<Layout*>(mainLayer->getChildByName(szName));
		CC_ASSERT(m_speakPos[i]);
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			sprintf(szName, "Panel_HeadPos_%d", i);
			m_HeadPos[i] = dynamic_cast<Layout*>(mainLayer->getChildByName(szName));
			CC_ASSERT(m_HeadPos[i]);
		}
		m_loseRoom[i] = nullptr;
	}

	//-----------------------准备--------------------------------------------
	m_readyLayout = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Ready"));
	CC_ASSERT(m_readyLayout);
	//准备按钮
	auto readyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Ready"));
	if (readyButton)
		readyButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchReadyEvent, this));
    auto inviteButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Friend"));
	if (inviteButton)
		inviteButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchInviteFriendEvent, this));
	auto copyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Copy"));
	if (copyButton)
		copyButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchCopyRoomNumEvent, this));

	m_running = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Running"));
	m_WarnLayer = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Warn"));

	//-----------------------抢庄----------------------------------
	m_lightLayout = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Light"));
	m_grapLayout = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Grap"));
	if (m_grapLayout)
	{
		if (g_BMode == 5)
		{
			for (int i = 0; i < 4; i++) {
				std::stringstream stream;
				stream << "Button_Grap_" << i;
				auto grapButton = dynamic_cast<Button*>(m_grapLayout->getChildByName(stream.str()));
				if (grapButton)
				{
					grapButton->setVisible(true);
					grapButton->setTag(i+1);
					grapButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchGrapEvent, this));
				}
			}
			auto grapNButton_0 = dynamic_cast<Button*>(m_grapLayout->getChildByName("Button_GrapN_0"));
			if (grapNButton_0)
			{
				grapNButton_0->setVisible(true);
				grapNButton_0->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchGrapNoEvent, this));
			}
		}
		if (g_BMode == 0)
		{
			auto grapNButton = dynamic_cast<Button*>(m_grapLayout->getChildByName("Button_GrapN"));
			if (grapNButton)
			{
				grapNButton->setVisible(true);
				grapNButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchGrapNoEvent, this));
			}
			auto grapYButton = dynamic_cast<Button*>(m_grapLayout->getChildByName("Button_Grap"));
			if (grapYButton)
			{
				grapYButton->setVisible(true);
				grapYButton->setTag(1);
				grapYButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchGrapEvent, this));
			}
		}
	}
	//-----------------------下注-----------------------------------
	m_chipLayout = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Chip"));
	if (m_chipLayout)
	{
		auto twoButton = dynamic_cast<Button*>(m_chipLayout->getChildByName("Button_Two"));
		if (twoButton)m_chipButtonVec.push_back(twoButton);
		twoButton = dynamic_cast<Button*>(m_chipLayout->getChildByName("Button_Three"));
		if (twoButton)m_chipButtonVec.push_back(twoButton);
		twoButton = dynamic_cast<Button*>(m_chipLayout->getChildByName("Button_Four"));
		if (twoButton)m_chipButtonVec.push_back(twoButton);
		twoButton = dynamic_cast<Button*>(m_chipLayout->getChildByName("Button_Five"));
		if (twoButton)m_chipButtonVec.push_back(twoButton);
		twoButton = dynamic_cast<Button*>(m_chipLayout->getChildByName("Button_Ten"));
		if (twoButton)m_chipButtonVec.push_back(twoButton);
	}
	char dzButtonMath[32] = {0};
	for (int index = 0; index < m_chipButtonVec.size(); index++)
	{
		if (m_chipButtonVec.at(index)){
			if (g_MaxBet == 3)
			{
				if (index == 0 || index == 4)
				{
					m_chipButtonVec.at(index)->setVisible(false);
				}
				else
				{
					sprintf(dzButtonMath, "res/Grap/%d-1.png", index);
					m_chipButtonVec.at(index)->loadTextureNormal(dzButtonMath);
					sprintf(dzButtonMath, "res/Grap/%d-2.png", index);
					m_chipButtonVec.at(index)->loadTexturePressed(dzButtonMath);
				}
			}
			if (g_MaxBet == 10)
			{
				if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
				{
					sprintf(dzButtonMath, "res/Grap/%d-1.png", 2 * (index + 1));
					m_chipButtonVec.at(index)->loadTextureNormal(dzButtonMath);
					sprintf(dzButtonMath, "res/Grap/%d-2.png", 2 * (index + 1));
					m_chipButtonVec.at(index)->loadTexturePressed(dzButtonMath);
				}
			}
			m_chipButtonVec.at(index)->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchChipEvent, this));
		}
	}
	//-----------------------显示玩家第五张牌并给15s时间计算有牛还是无牛----------------------- 
	m_countLayout = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Count"));

	for (int index = 0; index < 3; index++)
	{
		m_coutPockNum[index] = nullptr;
	}

	//-----------------------结算当前局数---------------------------
	m_showResult = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_ShowNN"));
	CC_ASSERT(m_showResult);
	m_result = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Result"));
	if (m_result)
	{
		auto listView = dynamic_cast<ListView*>(m_result->getChildByName("ListView"));
		m_listView_1 = dynamic_cast<ListView*>(listView->getChildByName("ListView_1"));
		m_listView_2 = dynamic_cast<ListView*>(listView->getChildByName("ListView_2"));
		m_resultPanel = dynamic_cast<Layout*>(m_result->getChildByName("Panel_0"));
		m_resultPanel->setVisible(false);
		auto button_Continue = dynamic_cast<Button*>(m_result->getChildByName("Button_Continue"));
		if (button_Continue)
		{
			button_Continue->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchContinueEvent, this));
		}
		auto button_Share = dynamic_cast<Button*>(m_result->getChildByName("Button_Share"));
		if (button_Share)
		{
			button_Share->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchShareEvent, this));
		}
	}
    m_ZID = -1;
	this->updateHead();
	this->initListener();

	m_Wife = dynamic_cast<ImageView*>(mainLayer->getChildByName("Image_Wife"));
	m_Power = dynamic_cast<LoadingBar*>(mainLayer->getChildByName("LoadingBar_Power"));
	if (m_Wife&&m_Power)
	{
		updateWife(0);
		schedule(CC_SCHEDULE_SELECTOR(QGameLayer::updateWife), 5.0f);
	}
	scheduleUpdate();
	//播放音乐
	auto userDefault = UserDefault::getInstance();
	float music = userDefault->getFloatForKey(MUSIC_VALUE_TEXT, 0.5);
	float sound = userDefault->getFloatForKey(SOUND_VALUE_TEXT, 0.5);

	GameAudioEngine::getInstance()->stopBackgroundMusic(GAME_HALL_BACKGROUND_MUSIC);
	GameAudioEngine::getInstance()->playBackgroundMusic(GAME_GAME_BACKGROUND_MUSIC, true);
	GameAudioEngine::getInstance()->setBackgroundMusicVolume(music);
	GameAudioEngine::getInstance()->setEffectsVolume(sound);
	return true;
}
void QGameLayer::initListener()
{
	auto pListener = EventListenerCustom::create(EVENT_JOINROOM, CC_CALLBACK_1(QGameLayer::updateEvent, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_GAMESTATUES, CC_CALLBACK_1(QGameLayer::updateGameStaues, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_GAMEREADY, CC_CALLBACK_1(QGameLayer::updateReady, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_ROB, CC_CALLBACK_1(QGameLayer::updateRobZhuang, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_GETBET, CC_CALLBACK_1(QGameLayer::updateGetBet, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_OPENCARD, CC_CALLBACK_1(QGameLayer::updateOpenCard, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_ROOMCANCEL, CC_CALLBACK_1(QGameLayer::updateRoomCancel, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_VOTECANCEL, CC_CALLBACK_1(QGameLayer::updateVoteCancel, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_GAMEBACK, CC_CALLBACK_1(QGameLayer::updateGameBack, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_GETZHUANG, CC_CALLBACK_1(QGameLayer::updateGetZhuang, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_GETCARD, CC_CALLBACK_1(QGameLayer::updateGetPocker, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_RESULT, CC_CALLBACK_1(QGameLayer::updateResult, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_ROOMOVER, CC_CALLBACK_1(QGameLayer::updateRoomOver, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_GAMEOVER, CC_CALLBACK_1(QGameLayer::updateGameOver, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_CHAR_FAST, CC_CALLBACK_1(QGameLayer::updateCharFast, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
    
    pListener = EventListenerCustom::create(EVENT_CHAR_VOICE, CC_CALLBACK_1(QGameLayer::updateCharVoice, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_EXITROOM, CC_CALLBACK_1(QGameLayer::updateExitRoom, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_LOSEROOM, CC_CALLBACK_1(QGameLayer::updateLoseRoom, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_BACKROOM, CC_CALLBACK_1(QGameLayer::updateBackRoom, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_ROOM_WARN, CC_CALLBACK_1(QGameLayer::updateRoomWarn, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_BACK_HALL, CC_CALLBACK_1(QGameLayer::updateBackHall, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_SETTLE, CC_CALLBACK_1(QGameLayer::updateSellte, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

}

void QGameLayer::selectedCtorWinEvent(Ref* pSender, CheckBox::EventType type)
{

	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto checkBox = (CheckBox*)pSender;
	m_ctorWin = checkBox->getSelectedState();
	if (m_ctorWin)
	{
		MsgGameRequest::sendCtrlWin((unsigned char)(m_gameNum+1), QGameManager::getInstance()->getSeatId());
	}
	else
	{
		MsgGameRequest::sendCtrlWin((unsigned char)(m_gameNum+1), (unsigned char)(-1));
	}
}

void QGameLayer::selectedShowMenuEvent(Ref* pSender, CheckBox::EventType type)
{
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto checkBox = (CheckBox*)pSender;
	auto isSelected = checkBox->isSelected();
}

//掉线回来
void QGameLayer::updateBackRoom(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long iSeatId = reinterpret_cast<long>(event->getUserData());
#else
	int iSeatId = reinterpret_cast<int>(event->getUserData());
#endif
	auto viewIndex = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
	if (m_loseRoom[viewIndex])
	{
		m_loseRoom[viewIndex]->removeFromParentAndCleanup(true);
		m_loseRoom[viewIndex] = nullptr;
	}
}
//掉线
void QGameLayer::updateLoseRoom(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long iSeatId = reinterpret_cast<long>(event->getUserData());
#else
	int iSeatId = reinterpret_cast<int>(event->getUserData());
#endif
	auto viewIndex = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
	if (m_loseRoom[viewIndex] == nullptr)
	{
		m_loseRoom[viewIndex] = Sprite::create("res/GameScene/diaoXian.png");
		auto imageHead = dynamic_cast<Layout*>(m_playerPanelVec.at(viewIndex)->getChildByName("Panel_head"));
		if (imageHead)
		{
			m_loseRoom[viewIndex]->setPosition(imageHead->getPosition());
			m_playerPanelVec.at(viewIndex)->addChild(m_loseRoom[viewIndex]);
		}
	}
}
//退出房间
void QGameLayer::updateExitRoom(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long iSeatId = reinterpret_cast<long>(event->getUserData());
#else
	int iSeatId = reinterpret_cast<int>(event->getUserData());
#endif

	int index = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
	if (m_player[index])CC_SAFE_DELETE(m_player[index]);
	m_playerPanelVec.at(index)->setVisible(false);
	QGameManager::getInstance()->m_vecUser_.clear();
	for (int i = 0; i < 5; i++)
	{
		if (m_player[i] != nullptr)
			QGameManager::getInstance()->m_vecUser_.pushBack(m_player[i]);
	}
	char szName[32] = { 0 };
	sprintf(szName, "Sprite_Ready_%d", index);
	auto readySprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName(szName));
	if (readySprite&&readySprite->isVisible())
	{
		readySprite->setVisible(false);
	}
	int countNum = 0;
	for (int i = 0; i < 8; i++)
	{
		if (m_player[i] != nullptr)
		{
			countNum++;
		}
	}
	if (countNum < 2 && QGameManager::getInstance()->getSeatId() == 0 && m_gameNum == 0)//如果是当前视图的人
	{
		auto startButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Start"));
		if (startButton)
		{
			startButton->setVisible(false);
			startButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchStartGameEvent, this));
		}
	}
}

//快捷聊天
void QGameLayer::updateCharFast(EventCustom* event)
{
	std::string sInfo = (char*)event->getUserData();
	int iSeatId = 0;//显示谁抢庄
	int chatId = 0;//显示抢庄还是不抢庄
	std::stringstream is(sInfo);
	is >> iSeatId >> chatId;
	
	int index = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
	auto type = chatId >> 4;
	auto viewId = chatId & 0x0f;
	if (type == 0 || type == 1)
	{
		viewId = type * 16 + viewId;
		SpeekLayer*speaklayer = nullptr;
		if (index == 1 || index == 2)
		{
			speaklayer = SpeekLayer::create(2, viewId);
			if (speaklayer)
			{
				speaklayer->setAnchorPoint(Vec2(0.0f, 0.0f));
				speaklayer->setPosition(m_speakPos[index]->getPositionX() + 40, m_speakPos[index]->getPositionY() - 10);
				this->addChild(speaklayer);
			}
		}
		else
		{
			speaklayer = SpeekLayer::create(1, viewId);
			if (speaklayer)
			{
				speaklayer->setAnchorPoint(Vec2(0.0f, 0.0f));
				speaklayer->setPosition(m_speakPos[index]->getPositionX() - 50, m_speakPos[index]->getPositionY() - 10);
				this->addChild(speaklayer);
			}
		}

		char chat[100] = { 0 };
		//快捷聊天音效添加
		if (m_player[index]->getSex() == 2)
		{
			sprintf(chat, "sound/chat_female_%d.wav", viewId);
		}
		else
		{
			sprintf(chat, "sound/chat_male_%d.wav", viewId);
		}
		GameAudioEngine::getInstance()->playEffect(chat);

		float voiceLenght = 3000.0f / 1000.0f;
		DelayTime *pDelayTime = DelayTime::create(voiceLenght);
		CallFunc *pHideFunc = CallFunc::create([=]
		{
			// 隐藏播放
			if (speaklayer)
			{
				speaklayer->DestroySpesk();
			}
		});
		Sequence *pAction = Sequence::create(pDelayTime, pHideFunc, NULL);
		this->runAction(pAction);
	}
	else if (type == 2 || type == 3)
	{
		viewId = (type - 2) * 16 + viewId;

		char dzChar[32] = { 0 };
		sprintf(dzChar, "res/Expression/%d.png", viewId+1);
		auto exSprite = Sprite::create(dzChar);
		if (exSprite)
		{
			exSprite->setPosition(m_HeadPos[index]->getPosition());
			exSprite->setScale(0.4f);
			this->addChild(exSprite);
			auto scaleAction = ScaleTo::create(0.5f, 1.0f);
			exSprite->runAction(scaleAction);
			float voiceLenght = 3000.0f / 1000.0f;
			DelayTime *pDelayTime = DelayTime::create(voiceLenght);
			CallFunc *pHideFunc = CallFunc::create([=]
			{
				// 隐藏播放
				exSprite->removeFromParentAndCleanup(true);
			});
			Sequence *pAction = Sequence::create(pDelayTime, pHideFunc, NULL);
			this->runAction(pAction);
		}
	}
	else
	{
		auto viewSeat = type - 4;
		auto otherIndex = (viewSeat - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
		string dzChar = "";
		switch (viewId)
		{
		case 0:
			dzChar = "bomb";
			break;
		case 1:
			dzChar = "egg";
			break;
		case 2:
			dzChar = "flower";
			break;
		case 3:
			dzChar = "zan";
			break;
		case 4:
			dzChar = "shoe";
			break;
		default:
			break;
		}

		ExpressionAction(m_HeadPos[index]->getPosition(), m_HeadPos[otherIndex]->getPosition(), dzChar);
	}
}

void QGameLayer::ExpressionAction(const Vec2& pos1, const Vec2& pos2, string fileName)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vector<SpriteFrame*> spriteFrameVec;
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	char dzChar[256] = { 0 };
	char dzCharTo[256] = { 0 };
	sprintf(dzChar, "res/Expression/%s.plist", fileName.c_str());
	sprintf(dzCharTo, "res/Expression/%s.png", fileName.c_str());

	// 这里使用plist文件  
	spriteFrameCache->addSpriteFramesWithFile(dzChar, dzCharTo);

	char path[256] = { 0 };
	for (int i = 1; i <= 9; ++i)
	{
		sprintf(path, "%send%d.png", fileName.c_str(), i);
		log("path = %s", path);
		SpriteFrame *pSpriteFrame = spriteFrameCache->getSpriteFrameByName(path);
		spriteFrameVec.pushBack(pSpriteFrame);
	}

	auto animation = Animation::createWithSpriteFrames(spriteFrameVec, 0.1);
	animation->setDelayPerUnit(2.0f / 20.0f);//必须设置否则不会动态播放  
	animation->setRestoreOriginalFrame(false);//是否回到第一帧  
	animation->setLoops(1);//重复次数 （-1:无限循环）  
	AnimationCache::getInstance()->addAnimation(animation, "dogs");

	auto sprite = Sprite::create();
	char dzCharTwo[256] = { 0 };
	sprintf(dzCharTwo, "%stemp.png", fileName.c_str());
	sprite->setSpriteFrame(spriteFrameCache->getSpriteFrameByName(dzCharTwo));
	sprite->setPosition(pos1);
	addChild(sprite, 1, 1);

	MoveTo* moveTo = MoveTo::create(0.5f, pos2);
	FiniteTimeAction * animate = Animate::create(animation);
	//auto actionMove = Spawn::create(moveTo, animate, nullptr);
	sprite->runAction(Sequence::create(moveTo, animate, RemoveSelf::create(), nullptr));
}

void QGameLayer::updateWife(float dt)
{
    string stateStr = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    stateStr = AdmobJni::getNetconType();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    stateStr = iOSAdapterUtility::getInstance()->getNetconType();
#endif
    
    log("NET STATE : %s", stateStr.c_str());
    
    int level = 0;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	level = AdmobJni::getWifiLevel();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	level = iOSAdapterUtility::getInstance()->getWifiLevel();
#endif

	if (m_Wife)
	{
		char name[255] = "";
		sprintf(name, "GameScene/signal%d.png", level);
		m_Wife->loadTexture(name);
	}
	log("wifi level : %d", level);

	int batteryLevel = 0;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	batteryLevel = AdmobJni::getBatteryLevel();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	batteryLevel = iOSAdapterUtility::getInstance()->getBatteryLevel();
#endif
	if (m_Power)
	{
		m_Power->setPercent(batteryLevel);
	}
	log("battery level : %d", batteryLevel);
}

//语音聊天
void QGameLayer::updateCharVoice(EventCustom* event)
{
    QGameVoiceData* voiceData = static_cast<QGameVoiceData*>(event->getUserData());
	if (voiceData && _charVoicePlay == false) {
		playCharVoice(voiceData);
	} else {
		_queueCharVoice.push(voiceData);
	}
}

void QGameLayer::playCharVoice(QGameVoiceData *voiceData) {
	if (voiceData == nullptr) {
		return;
	}
	_charVoicePlay = true;
	int iSeatId = voiceData->getSeatId();//位置
	int playTime = voiceData->getPlayTime(); //播放时间
	std::string voice = voiceData->getVoice();//显示抢庄还是不抢庄

	CC_SAFE_DELETE(voiceData);

	// 显示播放
	int index = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
	auto speaklayer = SpeekLayer::create();
	if (!speaklayer)
		return;
	speaklayer->setPosition(m_speakPos[index]->getPosition());
	this->addChild(speaklayer);

	GameAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();

	float voiceLenght = playTime / 1000.0f + 0.2f;
	DelayTime *pDelayTime = DelayTime::create(voiceLenght);
	CallFunc *pHideFunc = CallFunc::create([=] {
		// 隐藏播放
		speaklayer->DestroySpesk();
	});
	CallFunc *pHideFunc1 = CallFunc::create([=] {
		// 隐藏播放
		_charVoicePlay = false;
		if (_queueCharVoice.size() > 0) {
			QGameVoiceData *voiceData = _queueCharVoice.front();
			_queueCharVoice.pop();
			playCharVoice(voiceData);
		} else {
			GameAudioEngine::getInstance()->resumeBackgroundMusic();
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
		}
	});
	Sequence *pAction = Sequence::create(pDelayTime, pHideFunc, DelayTime::create(1.0f), pHideFunc1, NULL);
	runAction(pAction);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	AdmobJni::playRecord(voice.c_str(), voice.length());
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	iOSAdapterUtility::getInstance()->playRecord(voice.c_str(), 10);
#endif
}

void QGameLayer::updateRoomWarn(EventCustom* event)
{
	if (m_WarnLayer)
	{
		m_WarnLayer->runAction(Sequence::create(Show::create(), DelayTime::create(5.0f), Hide::create(), nullptr));
	}
}

void QGameLayer::updateBackHall(EventCustom* event)
{
	WWSceneManager::getInstance()->closeAllUI();
	LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
	if (Director::getInstance()->getRunningScene()->getName() != "QHallScene")
	{
		WWSceneManager::getInstance()->openScene(QHallScene::create());
	}
}

void QGameLayer::updateSellte(EventCustom* event)
{
	auto sellteData = QGameManager::getInstance()->m_vecSellteData;

	showAllPocket(sellteData);
}

//通知退出房间消息---大结
void QGameLayer::updateRoomOver(EventCustom* event)
{
	if (m_exitRoom)
	{
		m_exitRoom->removeFromParentAndCleanup(true);
	}
	if (this->isScheduled(schedule_selector(QGameLayer::lightCallBack)))
	{
		unschedule(schedule_selector(QGameLayer::lightCallBack));
	}

	m_gameOverlayer = GameOverLayer::create();
	this->addChild(m_gameOverlayer);
	for (int i = 0; i < m_playerPanelVec.size(); i++)
	{
		if (m_playerPanelVec.at(i))
			m_playerPanelVec.at(i)->setVisible(false);
	}
	if (m_gameOverlayer)
	{
		m_gameOverlayer->onRefreshData(m_player);
		if (isGameOver)
			m_gameOverlayer->setVisible(false);
	}
	unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::hideExitRoom));
}
//同意或不同意解散房间
void QGameLayer::updateGameOver(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long cPass = reinterpret_cast<long>(event->getUserData());
#else
	int cPass = reinterpret_cast<int>(event->getUserData());
#endif

	if (cPass == 0)
	{
		if (m_dismiss)
		{
			m_dismiss->UpdateDismissResult(m_player, cPass);
		}
		//不是等待状态就不弹出结算界面
		if (QGameManager::getInstance()->getGameStatues() != GS_GAME_WAIT)
			return;
		//别人准备了  也不弹出结算界面
		if (m_readyLayout->isVisible() == false)
		{
			//m_result->setVisible(true);
			return;
		}
		char dzReady[32] = { 0 };
		for (int index = 0; index < GAMECONFIG.m_max_num; index++)
		{
			if (m_player[index] == nullptr)
				continue;
			if (m_player[index]->getSeatId() == QGameManager::getInstance()->getSeatId())
			{
				sprintf(dzReady, "Sprite_Ready_%d", index);
				auto readySprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName(dzReady));
				if (readySprite->isVisible())
					m_result->setVisible(false);
				//else
				//	m_result->setVisible(true);
			}
		}
	}
	else
	{
		if (m_dismiss)
		{
			m_dismiss->removeFromParentAndCleanup(true);
			m_dismiss = nullptr;
		}
		this->unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::updateSendCard));
	}
	if (!m_bIsStart)
	{
		log("-------进入大厅------");
		WWSceneManager::getInstance()->closeAllUI();
		LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
		if (Director::getInstance()->getRunningScene()->getName() != "QHallScene")
		{
			WWSceneManager::getInstance()->openScene(QHallScene::create());
		}
		sendIndex = 0;
		indexCount = 0;
	}
}
//退房表决消息
void QGameLayer::updateVoteCancel(EventCustom* event)
{
	std::string sInfo = (char*)event->getUserData();
	int iSeatId = 0;
	int iAgree = 0;
	std::stringstream is(sInfo);
	is >> iSeatId >> iAgree;

	int index = ((int)iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
	if (m_dismiss)
	{
		m_dismiss->setAgreeState(m_player[index], index, iAgree);
	}
}
void QGameLayer::touchAgreeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	MsgGameRequest::sendVoteCancel(1);
	unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::resetTime));
    unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::hideExitRoom));
	if (m_exitRoom)
	{
		m_exitRoom->removeFromParentAndCleanup(true);
		m_exitRoom = nullptr;
	}
}
void QGameLayer::touchDisAgreeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	MsgGameRequest::sendVoteCancel(0);
	unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::resetTime));
    unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::hideExitRoom));
	if (m_exitRoom)
	{
		m_exitRoom->removeFromParentAndCleanup(true);
		m_exitRoom = nullptr;
	}
}

//发起退房
void QGameLayer::updateRoomCancel(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long iSeatId = reinterpret_cast<long>(event->getUserData());
#else
	int iSeatId = reinterpret_cast<int>(event->getUserData());
#endif
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		int index = ((int)iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
		if (m_dismiss == nullptr)
		{
			m_dismiss = DismissLayer::create(m_gameNum);
			if (m_dismiss)
			{
				this->addChild(m_dismiss, 1);
				m_dismiss->setSelfParent(this);
			}
		}
		m_dismiss->UpdateAgreeState(m_player, index);
	}
	else
	{
		if (iSeatId == QGameManager::getInstance()->getSeatId())
			return;
		if (m_exitRoom == nullptr)
		{
			m_exitRoom = DialogLayer::create(2, GetGameString("exit_room"));
			this->addChild(m_exitRoom, 1);
		}
		auto agreeExit = dynamic_cast<Button*>(m_exitRoom->m_systemLayout->getChildByName("Button_Ture"));
		if (agreeExit) agreeExit->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchAgreeEvent, this));
		auto falseButton = dynamic_cast<Button*>(m_exitRoom->m_systemLayout->getChildByName("Button_False"));
		if (falseButton) falseButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchDisAgreeEvent, this));
		//隔断时间请求退房弹窗消失
		m_exitTime = 10.0f;
		/*if (GAMECONFIG.m_nAppType == GAME_TYPE_XRNN)
		{
		m_exitTime = 60.0f;
		if (m_exitRoom)
		{
		char szCountNum[4] = { 0 };
		sprintf(szCountNum, "%02d", m_exitTime);
		auto timeImage = dynamic_cast<ImageView*>(m_exitRoom->m_systemLayout->getChildByName("Image_TimeBg"));
		if (timeImage) timeImage->setVisible(true);
		auto timeText = dynamic_cast<Text*>(m_exitRoom->m_systemLayout->getChildByName("Text_Time"));
		if(timeText)
		{
		timeText->setVisible(true);
		timeText->setString(szCountNum);
		}
		unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::resetTime));
		this->schedule(CC_SCHEDULE_SELECTOR(QGameLayer::resetTime), 1.0f);
		auto text_Warn = dynamic_cast<Text*>(m_exitRoom->m_systemLayout->getChildByName("Text_Z"));
		if (text_Warn)
		{
		text_Warn->setVisible(true);
		text_Warn->setString(GetGameString("exit_warning"));
		}
		}
		}*/
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			if (this->isScheduled(CC_SCHEDULE_SELECTOR(QGameLayer::hideExitRoom)))
			{
				unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::hideExitRoom));
			}
			if (!this->isScheduled(CC_SCHEDULE_SELECTOR(QGameLayer::hideExitRoom)))
			{
				this->scheduleOnce(CC_SCHEDULE_SELECTOR(QGameLayer::hideExitRoom), m_exitTime);
			}
		}
	}
}
void QGameLayer::resetTime(float dt)
{
	if (m_exitTime == 0)
	{
		unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::resetTime));
		return;
	}
	else
	{
		m_exitTime--;
		char szCountNum[4] = { 0 };
		sprintf(szCountNum, "%02d", m_exitTime);
		auto timeText = dynamic_cast<Text*>(m_exitRoom->m_systemLayout->getChildByName("Text_Time"));
		if (timeText)
		{
			timeText->setString(szCountNum);
		}
	}
}

void QGameLayer::hideExitRoom(float dt)
{
    // 弹窗消失
    if (m_exitRoom)
    {
		MsgGameRequest::sendVoteCancel(0);
        m_exitRoom->removeFromParentAndCleanup(true);
        m_exitRoom = nullptr;
    }
}
                           
//开牌消息
void QGameLayer::updateOpenCard(EventCustom* event)
{
	QCardData *pCard = static_cast<QCardData*>(event->getUserData());
	if (pCard)
		showOtherHand(pCard);
}
//获取下注消息
void QGameLayer::updateGetBet(EventCustom* event)
{
	//if (QGameManager::getInstance()->getReady() != 0)
	//{
	std::string sInfo = (char*)event->getUserData();
	int iSeatId = 0;
	int cBet = 0;
	std::stringstream is(sInfo);
	is >> iSeatId >> cBet;

	int iCount = GAMECONFIG.m_max_num;
	int index = ((int)iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + iCount) % iCount;
	if (m_player[index])
		m_player[index]->setUserBet(cBet);

	BetChip(cBet, m_HeadPos, index);
	auto chip = dynamic_cast<Text*>(m_playerPanelVec.at(index)->getChildByName("Text_Chip"));
	if (chip)
	{
		chip->setString(StringTool::intToString(cBet));
	}
	//}
}
//获取扑克
void QGameLayer::updateGetPocker(EventCustom* event)
{
	showMyHand();
	auto c_Cover = QHALLMGR->getKcardWay();
	for (int i = 0; i < m_pocketUser.size() - 1; i++)
	{
		auto pocketType = (int)(m_pocketVec.at(i) & 0xF);
		auto pocketNum = (int)(m_pocketVec.at(i) >> 4);
		char szName[20] = { 0 };
		sprintf(szName, "Pocke/%d-%d.png", pocketNum, pocketType);
		m_pocketUser.at(i)->loadTexture(szName);
		m_pocketUser.at(i)->setPositionX(m_PocketPos->getContentSize().width * (2 * i + 1) / 10);
		m_pocketUser.at(i)->setVisible(false);
	}
	switch (c_Cover)
	{
	case 1:
		m_pocketUser.at(4)->loadTexture("Pocke/Back.png");
		break;
	case 2:
		m_pocketUser.at(3)->loadTexture("Pocke/Back.png");
		m_pocketUser.at(4)->loadTexture("Pocke/Back.png");
		break;
	case 5:
		m_pocketUser.at(0)->loadTexture("Pocke/Back.png");
		m_pocketUser.at(1)->loadTexture("Pocke/Back.png");
		m_pocketUser.at(2)->loadTexture("Pocke/Back.png");
		m_pocketUser.at(3)->loadTexture("Pocke/Back.png");
		m_pocketUser.at(4)->loadTexture("Pocke/Back.png");
		break;
	default:
		break;
	}

	float time = GAMECONFIG.m_scheduleTime;
	schedule(CC_SCHEDULE_SELECTOR(QGameLayer::updateSendCard), time);
}
//排序
vector<unsigned char> QGameLayer::SortPokerVec(std::vector<unsigned char>& vecCard)
{
	vector<unsigned char> tmpVec;
	
	if (vecCard.size() != 5)
		return tmpVec;
	bool isBreak = false;
	unsigned char card1 = 0;
	unsigned char card2 = 0;
	unsigned char card3 = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = i + 1; j < 5; j++)
		{
			for (int k = j + 1; k < 5; k++)
			{
				int card11 = vecCard[i] >> 4;
				int card22 = vecCard[j] >> 4;
				int card33 = vecCard[k] >> 4;
				int u08Value = GetValue(card11) + GetValue(card22) + GetValue(card33);
				if (u08Value % 10 == 0)
				{
					card1 = vecCard[i];
					card2 = vecCard[j];
					card3 = vecCard[k];
					tmpVec.push_back(vecCard[k]);
					tmpVec.push_back(vecCard[j]);
					tmpVec.push_back(vecCard[i]);
					isBreak = true;
					break;
				}
			}
			if (isBreak)
				break;
		}
		if (isBreak)
			break;
	}
	sort(tmpVec.begin(), tmpVec.end(), greater<int>());
	for (int i = 0; i < 5; i++)
	{
		unsigned char value = vecCard[i];
		if (card1 != value && card2 != value && card3 != value)
		{
			tmpVec.push_back(value);
		}
	}
	return tmpVec;
}
//获取他人的手牌
void QGameLayer::showOtherHand(QCardData *pCard)
{
	//if (QGameManager::getInstance()->getReady() != 0)
	//{
		int iSeatId = pCard->getSeatId();
		int iCount = GAMECONFIG.m_max_num;
		int index = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + iCount) % iCount;

		auto cardVec = pCard->getPokerVec();
		bool isCow = pCard->getCow() > 0 && pCard->getCow() <= 10;
		if (isCow)
		{
			cardVec = SortPokerVec(cardVec);
		}
		if (m_player[index] == nullptr)
			return;

		if (m_player[index]->getSeatId() == QGameManager::getInstance()->getSeatId())//如果是当前视图的人
		{
			//PromptCow();
			cardVec = SortPokerVec(m_pocketVec);
			char szPath[100] = { 0 };
			char szName[100] = { 0 };
			for (int i = 0; i < cardVec.size(); i++)
			{
				auto pockerType = (PocketSprite::PocketType)(cardVec.at(i) & 0xF);
				auto pockerNum = (PocketSprite::PocketNum)(cardVec.at(i) >> 4);
				sprintf(szPath, "Pocke/%d-%d.png", (int)pockerNum, (int)pockerType);
				sprintf(szName, "Image_%d", i + 1);
				auto sprite = dynamic_cast<ImageView*>(m_palyerPockerVec.at(index)->getChildByName(szName));
				if (sprite)
				{
					sprite->loadTexture(szPath);
					sprite->setVisible(true);
					if (isCow)
					{
						if (i <= 2)
						{
							sprite->setPositionX(m_palyerPockerVec.at(index)->getContentSize().width * 3 / 10 + (i - 1) * 120);
						}
						else
						{
							sprite->setPositionX(m_palyerPockerVec.at(index)->getContentSize().width * 9 / 10 + (i - 4) * 120);
						}
					}
					else
					{
						sprite->setPositionX(m_palyerPockerVec.at(index)->getContentSize().width * (2 * i + 1) / 10);
					}
				}

			}
			this->ShowNiu(pCard->getCow(), index);
			m_countLayout->setVisible(false);
			return;
		}

		char szPath[100] = { 0 };
		char szName[100] = { 0 };
		for (int i = 0; i < cardVec.size(); i++)
		{
			m_spritePockerVec[index - 1].push_back(cardVec.at(i));
			auto pockerType = (PocketSprite::PocketType)(cardVec.at(i) & 0xF);
			auto pockerNum = (PocketSprite::PocketNum)(cardVec.at(i) >> 4);
			sprintf(szPath, "Pocke/%d-%d.png", (int)pockerNum, (int)pockerType);
			sprintf(szName, "Sprite_%d", i + 1);
			auto sprite = dynamic_cast<Sprite*>(m_palyerPockerVec.at(index)->getChildByName(szName));
			if (sprite)
			{
				sprite->setTexture(szPath);
				sprite->setScale(0.5f);
				sprite->setVisible(true);
				if (isCow)
				{
					if (i <= 2)
					{
						sprite->setPositionX(m_palyerPockerVec.at(index)->getContentSize().width * 1 / 3 + (i - 1) * 30);
					}
					else
					{
						sprite->setPositionX(m_palyerPockerVec.at(index)->getContentSize().width * 5 / 6 + (i - 4) * 30);
					}
				}
				else
				{
					sprite->setPositionX(m_palyerPockerVec.at(index)->getContentSize().width * (i + 1) / 6);
				}
			}

		}
		this->ShowNiu(pCard->getCow(), index);
	//}
}

void QGameLayer::showOtherBackPoker()
{
	auto size_Num = m_pocketVec.size();
	for (int index = 0; index < GAMECONFIG.m_max_num; index++)
	{
		if (m_player[index] == nullptr || index == 0)
		{
			for (int j = 0; j < size_Num; j++)
			{
				m_pocketUser.at(j)->setVisible(true);
			}
			continue;
		}
		else
		{
			m_palyerPockerVec.at(index)->setVisible(true);
			for (int j = 0; j < 5; j++)
			{
				char szName[255] = {};
				sprintf(szName, "Sprite_%d", j + 1);
				auto sprite_pocket = dynamic_cast<Sprite*>(m_palyerPockerVec.at(index)->getChildByName(szName));
				if (sprite_pocket)
				{
					sprite_pocket->setVisible(true);
				}
			}
		}
	}
}

void QGameLayer::showMyPoker()
{
	for (int i = 0; i < m_pocketUser.size(); i++)
	{
		auto pocketType = (int)(m_pocketVec.at(i) & 0xF);
		auto pocketNum = (int)(m_pocketVec.at(i) >> 4);
		char szName[20] = { 0 };
		sprintf(szName, "Pocke/%d-%d.png", pocketNum, pocketType);
		m_pocketUser.at(i)->loadTexture(szName);
		m_pocketUser.at(i)->setVisible(true);
	}
}

void QGameLayer::showAllPocket(Vector<QCardData*> & vecSellte)
{
	for (int i = 0; i < vecSellte.size(); i++)
	{
		auto iSeatId = vecSellte.at(i)->getSeatId();
		int iCount = GAMECONFIG.m_max_num;
		int index = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + iCount) % iCount;

		auto cardVec = vecSellte.at(i)->getPokerVec();
		bool isCow = vecSellte.at(i)->getCow() > 0 && vecSellte.at(i)->getCow() <= 10;
		if (isCow)
		{
			cardVec = SortPokerVec(cardVec);
		}
		this->runAction(Sequence::create(DelayTime::create(i*0.8f), CallFunc::create([=]
		{
			ShowNiu(vecSellte.at(i)->getCow(), index);
		}), NULL));

		char szPath[100] = { 0 };
		char szName[100] = { 0 };
		if (m_player[index]->getSeatId() == QGameManager::getInstance()->getSeatId())//如果是当前视图的人
		{
			//this->ShowNiu(pCard->getCow(), index);
			for (int j = 0; j < cardVec.size(); j++)
			{
				auto pockerType = (PocketSprite::PocketType)(cardVec.at(j) & 0xF);
				auto pockerNum = (PocketSprite::PocketNum)(cardVec.at(j) >> 4);
				sprintf(szPath, "Pocke/%d-%d.png", (int)pockerNum, (int)pockerType);

			}
			continue;
		}

		for (int j = 0; j < cardVec.size(); j++)
		{
			m_spritePockerVec[index - 1].push_back(cardVec.at(j));
			auto pockerType = (PocketSprite::PocketType)(cardVec.at(j) & 0xF);
			auto pockerNum = (PocketSprite::PocketNum)(cardVec.at(j) >> 4);
			sprintf(szPath, "Pocke/%d-%d.png", (int)pockerNum, (int)pockerType);
			sprintf(szName, "Sprite_%d", j + 1);
			auto sprite = dynamic_cast<Sprite*>(m_palyerPockerVec.at(index)->getChildByName(szName));
			if (sprite)
			{
				sprite->setTexture(szPath);
				sprite->setVisible(true);
				if (isCow&&j <= 2)
				{
					sprite->setPositionY(m_palyerPockerVec.at(index)->getContentSize().height / 2 + 10);
				}
				else
				{
					sprite->setPositionY(m_palyerPockerVec.at(index)->getContentSize().height / 2);
				}
			}
		}
	}
}

//获取自己的手牌
void QGameLayer::showMyHand()
{
	if (m_pocketVec.size() != 0)
	{
		m_pocketVec.clear();
	}
	auto cardDate = QGameManager::getInstance()->getpCardData();
	if (!cardDate)
		return;
	auto cardVec = cardDate->getPokerVec();
	for (int i = 0; i < cardVec.size(); i++)
	{
		m_pocketVec.push_back(cardVec.at(i));
	}
	char szName[100] = { 0 };
    m_palyerPockerVec.at(0)->setVisible(true);
	for (int i = 0; i < m_pocketVec.size(); i++)
	{
		m_pocketUser.at(i)->setVisible(false);
		m_pocketUser.at(i)->setPositionX(m_PocketPos->getContentSize().width * (2 * i + 1) / 10);
		m_pocketUser.at(i)->setTag(i);
		m_pocketUser.at(i)->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchPocketEvent, this));
	}

}

void QGameLayer::showMyPaiAgain()
{
	this->unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::updateSendCard));
	indexCount = 0;
	sendIndex = 0;
	int size_Num = m_pocketVec.size();
	m_gameState = QGameManager::getInstance()->getGameStatues();
	if (m_gameState >= GS_OPEN_POKER)
	{
		size_Num = m_pocketVec.size();
	}
	for (int i = 0; i < size_Num; i++)
    {
		m_pocketUser.at(i)->setVisible(true);
    }

	for (int index = 0; index < GAMECONFIG.m_max_num; index++)
	{
		if (m_player[index] == nullptr || index == 0)
		{
			for (int j = 0; j < size_Num; j++)
			{
				m_pocketUser.at(j)->setVisible(true);
			}
			continue;
		}
		else
		{
			m_palyerPockerVec.at(index)->setVisible(true);
			for (int j = 0; j < size_Num; j++)
			{
				char szName[255] = {};
				sprintf(szName, "Sprite_%d", j + 1);
				auto sprite_pocket = dynamic_cast<Sprite*>(m_palyerPockerVec.at(index)->getChildByName(szName));
				if (sprite_pocket)
				{
					sprite_pocket->setVisible(true);
				}
			}
		}
	}
	
}
//获取庄消息
void QGameLayer::updateGetZhuang(EventCustom* event)
{
	std::string sInfo = (char*)event->getUserData();
	int iSeatId = 0;
	int cMulti = 0;
	std::stringstream is(sInfo);
	is >> iSeatId >> cMulti;

	int iCount = GAMECONFIG.m_max_num;
	int index = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + iCount) % iCount;
	m_ZID = index;
	char szName[30] = {};
	sprintf(szName, "res/Grap/grapY_%d.png", cMulti);
	m_grapLayout->setVisible(false);
    
}
//抢庄消息
void QGameLayer::updateRobZhuang(EventCustom* event)
{
	std::string sInfo = (char*)event->getUserData();
	int iSeatId = 0;//显示谁抢庄
	int isGrap = 0;//显示抢庄还是不抢庄
	std::stringstream is(sInfo);
	is >> iSeatId >> isGrap;
	//抢庄完成显示
	int iCount = GAMECONFIG.m_max_num;
	int index = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + iCount) % iCount;
	if (m_player[index] == nullptr)
		return;
	char szName[100] = { 0 };
	if (isGrap == 0)
	{
		//sprintf(szName, "Sprite_NGrap_%d", index);
		//auto grapNSprite = dynamic_cast<Sprite*>(m_grapLayout->getChildByName(szName));
		//if (grapNSprite)
		//{
		//	grapNSprite->setVisible(true);
		//	grapNSprite->setTexture("res/Grap/grapN.png");
		//}
	}
	else
	{
		sprintf(szName, "Sprite_NGrap_%d", index);
		auto grapNSprite = dynamic_cast<Sprite*>(m_grapLayout->getChildByName(szName));
		if (grapNSprite)
		{
			grapNSprite->setVisible(true);
			if (QHALLMGR->getZMode()==5)
			{
				sprintf(szName, "res/Grap/grapY_%d.png", isGrap);
				grapNSprite->setTexture(szName);
			}
			else
			{
				grapNSprite->setTexture("res/Grap/grapY.png");
			}
			auto scaleTo = ScaleTo::create(0.2f, 0.25f);
			auto scaleBack = ScaleTo::create(0.2f, 0.5f);
			grapNSprite->runAction(Repeat::create(Sequence::create(scaleTo, scaleBack, nullptr), 3));
			sprintf(szName, "Sprite_Light_%d", index);
			auto lootSprite = dynamic_cast<Sprite*>(m_lightLayout->getChildByName(szName));
			if (lootSprite)m_vecLight.push_back(lootSprite);
		}
	}
    int nqiang = isGrap == 0 ? 0 : 1;
    char chat[100] = { 0 };
    if (m_player[index]->getSex() == 2)
    {
        sprintf(chat, "sound/female_bank%d.mp3", nqiang);
    }
    else
    {
        sprintf(chat, "sound/male_bank%d.mp3", nqiang);
    }
    GameAudioEngine::getInstance()->playEffect(chat);
}
//准备消息
void QGameLayer::updateReady(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long iSeatId = reinterpret_cast<long>(event->getUserData());
#else
	int iSeatId = reinterpret_cast<int>(event->getUserData());
#endif
	//谁准备好了
	if (QGameManager::getInstance()->getSeatId() == iSeatId)
	{
		m_result->setVisible(false);
		//this->ResetLayer();
		m_readyLayout->setVisible(true);
		if (m_gameNum > 0)
		{
			auto friendButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Friend"));
			if (friendButton) friendButton->setVisible(false);
			auto copyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Copy"));
			if (copyButton) copyButton->setVisible(false);
			auto statSprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName("Sprite_Starting"));
			if (statSprite) statSprite->setVisible(true);
		}
		auto readyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Ready"));
		if (readyButton) readyButton->setVisible(false);
		QGameManager::getInstance()->setReady(1);
	}
	int index = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
	if (m_player[index] == nullptr)
		return;
	char szName[100] = { 0 };
	sprintf(szName, "Sprite_Ready_%d", index);
	auto readySprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName(szName));
	if (readySprite) readySprite->setVisible(true);

	if (QHallManager::getInstance()->getZMode() == 2)
	{
		showZhuang();
	}
	int countNum = 0;
	for (int i = 0; i < 8; i++)
	{
		if (m_player[i] != nullptr)
		{
			countNum++;
		}
	}
	if (countNum >= 2 && QGameManager::getInstance()->getSeatId() == 0 && m_gameNum == 0)//如果是当前视图的人
	{
		auto startButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Start"));
		if (startButton)
		{
			startButton->setVisible(true);
			startButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchStartGameEvent, this));
		}
	}
}
void QGameLayer::reStartTime(float dt)
{
	if (m_reStartTime == 0)
	{
		unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::reStartTime));
		return;
	}
	m_reStartTime--;
	//char szCountNum[4] = { 0 };
	//sprintf(szCountNum, "%02d", m_reStartTime);
	//auto timeText = dynamic_cast<Text*>(m_result->getChildByName("Text_Time"));
 //   if(timeText)
 //   {
 //       timeText->setString(szCountNum);
 //   }
}
//状态
void QGameLayer::updateGameStaues(EventCustom* event)
{
	m_gameState = QGameManager::getInstance()->getGameStatues();
	switch (m_gameState)
	{
	case QGameLayer::GS_GAME_WAIT:
		QGameManager::getInstance()->m_vecSellteData.clear();
		if (QGameManager::getInstance()->getReady() != 0)
		{
			if (m_bIsGameBack)
			{
				m_bIsStart = true;
				m_result->setVisible(false);
				m_readyLayout->setVisible(true);
				auto friendButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Friend"));
				if (friendButton) friendButton->setVisible(false);
				auto copyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Copy"));
				if (copyButton) copyButton->setVisible(false);
			}
			else
			{
				if (m_isPlay == 1)
					GameAudioEngine::getInstance()->playEffect("sound/gamewin.mp3");
				if (m_isPlay == 0)
					GameAudioEngine::getInstance()->playEffect("sound/gamelost.mp3");
				//m_result->setVisible(true);
				if (m_btnContinue)
				{
					m_btnContinue->setVisible(true);
				}
				if (m_btnQuery)
				{
					m_btnQuery->setVisible(true);
				}
			}
			showUserScore();
		}
		else
		{
			if (m_running)
			{
				m_running->setVisible(false);
			}
			m_readyLayout->setVisible(true);
			auto friendButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Friend"));
			if (friendButton) friendButton->setVisible(false);
			auto copyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Copy"));
			if (copyButton) copyButton->setVisible(false);
		}
		if (PROCECONFIG.m_bAutoReady)
		{
			m_reStartTime = PROCECONFIG.m_fGameWait;
			m_RoomClock->CancelTick();
			m_RoomClock->Tick(m_reStartTime, false);
			if (m_gameNum == 0)
			{
				m_startNext->setVisible(false);
			}
			else
			{
				m_startNext->setVisible(true);
			}
			//char szCountNum[4] = { 0 };
			//sprintf(szCountNum, "%02d", m_reStartTime);
			//auto timeText = dynamic_cast<Text*>(m_result->getChildByName("Text_Time"));
			//if (timeText)
			//{
			//	timeText->setString(szCountNum);
			//}
			this->schedule(CC_SCHEDULE_SELECTOR(QGameLayer::reStartTime), 1.0f);
			this->scheduleOnce(CC_SCHEDULE_SELECTOR(QGameLayer::startNext), m_reStartTime);
		}
		break;
	case QGameLayer::GS_SEND_POKER:
	{
		m_RoomClock->CancelTick();
		if (m_startNext)
		{
			m_startNext->setVisible(false);
		}
		if (QGameManager::getInstance()->getReady() != 0)
		{
			unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::startNext));

			if (chipCheckBox&&chipCheckBox->isVisible() == false)
			{
				chipCheckBox->setVisible(true);
			}
			if (chipCheckBox&&QHALLMGR->getZMode() == 3)
			{
				chipCheckBox->setVisible(false);
			}
			if (QHallManager::getInstance()->getZMode() == 2)
			{
				showZhuang();
			}
			m_bIsGameBack = false;
			m_pIsSendPocke = true;

			//m_RoomClock->Tick(PROCECONFIG.m_fSendPoker, false);

			//发牌
			for (int i = 0; i < GAMECONFIG.m_max_num; i++)
			{
				char szName[100] = { 0 };
				sprintf(szName, "Sprite_Ready_%d", i);
				auto readySprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName(szName));
				if (readySprite) readySprite->setVisible(false);
			}
			m_readyLayout->setVisible(false);
			auto readyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Ready"));
			if (readyButton) readyButton->setVisible(false);
			auto friendButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Friend"));
			if (friendButton) friendButton->setVisible(false);
			auto startButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Start"));
			if (startButton) startButton->setVisible(false);	
			auto copyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Copy"));
			if (copyButton) copyButton->setVisible(false);
			m_gameNum += 1;
			char dzRoom[32] = { 0 };
			sprintf(dzRoom, GetGameString("show_gameNum"), m_gameNum, QHallManager::getInstance()->getPlayMode());
			m_countNum->setString(dzRoom);
		}
		else
		{
			showOtherBackPoker();
		}
	}
		break;
	case QGameLayer::GS_LOOT_DEAL:
	{
		//抢庄状态
		m_RoomClock->CancelTick();
		m_RoomClock->Tick(PROCECONFIG.m_fRobBanker, false);
		if (QGameManager::getInstance()->getReady() != 0)
		{
			m_grapLayout->setVisible(true);//显示
		}
		else
		{
			m_grapLayout->setVisible(false);
		}
		showOtherBackPoker();
	}
		break;
	case QGameLayer::GS_RAND_DEAL:
	{
		if (QGameManager::getInstance()->getReady() != 0)
		{
			m_RoomClock->CancelTick();
			m_RoomClock->Tick(PROCECONFIG.m_fRandBanker, false);

			//随机庄家状态
			m_grapLayout->setVisible(false);
			m_lightLayout->setVisible(true);
			if (QHallManager::getInstance()->getZMode() == 0 || QHallManager::getInstance()->getZMode() == 5)
			{
				//添加随机动画
				if (m_vecLight.size() > 0)
				{
					m_vecLight[0]->setVisible(true);
					if (m_vecLight.size() >= 2)
					{
						schedule(schedule_selector(QGameLayer::lightCallBack), 0.2);
					}
				}
				else
				{
					//都不抢
					char szName[128] = { 0 };
					for (int i = 0; i < GAMECONFIG.m_max_num; i++)
					{
						if (m_player[i] == nullptr)
							continue;
						sprintf(szName, "Sprite_NGrap_%d", i);
						auto grapNSprite = dynamic_cast<Sprite*>(m_grapLayout->getChildByName(szName));
						if (grapNSprite)
						{
							grapNSprite->setVisible(true);
							grapNSprite->setTexture("res/Grap/grapY.png");
						}
						sprintf(szName, "Sprite_Light_%d", i);
						auto lootSprite = dynamic_cast<Sprite*>(m_lightLayout->getChildByName(szName));
						if (lootSprite) m_vecLight.push_back(lootSprite);
						sprintf(szName, "Sprite_NGrap_%d", i);
					}
					m_vecLight[0]->setVisible(true);
					schedule(schedule_selector(QGameLayer::lightCallBack), 0.2);
				}
			}
		}
		else
		{
			showOtherBackPoker();
		}
	}
		break;
	case QGameLayer::GS_PLAYER_BET:
	{	
		//showMyPaiAgain();
		m_RoomClock->CancelTick();
		m_RoomClock->Tick(PROCECONFIG.m_fUserBet, false);
		if (QGameManager::getInstance()->getReady() != 0)
		{
			if (QHallManager::getInstance()->getZMode() == 0 || QHallManager::getInstance()->getZMode() == 5)
			{
				char szName[100] = { 0 };
				sprintf(szName, "Sprite_Light_%d", m_ZID);
				auto sprite_z = dynamic_cast<Sprite*>(m_lightLayout->getChildByName(szName));
				if (sprite_z)
				{
					m_ligthDealer->setVisible(true);
					m_ligthDealer->setPosition(sprite_z->getPosition());
					m_ligthDealer->runAction(Sequence::createWithTwoActions(lightBlink(), CallFunc::create([&](){
						m_ligthDealer->setVisible(false);
					})));
				}
			}
			showZhuang();
			unschedule(schedule_selector(QGameLayer::lightCallBack));
			int iCount = m_vecLight.size();
			if (iCount > 0)
			{
				for (int i = 0; i < iCount; i++)
				{
					m_vecLight[i]->setVisible(false);
				}
				m_vecLight.clear();
			}
			m_grapLayout->setVisible(false);
			m_lightLayout->setVisible(false);

			//闲家下注
			if (QHallManager::getInstance()->getZMode() == 1)
				m_readyLayout->setVisible(false);
			if (QHallManager::getInstance()->getZMode() == 3)
			{
				m_chipLayout->setVisible(false);
			}
			else
				m_chipLayout->setVisible(true);
			auto imageChip = dynamic_cast<ImageView*>(m_chipLayout->getChildByName("Image_Chip"));
			if (imageChip &&  GAMECONFIG.m_nAppType != GAME_TYPE_KXNNL)
			{
				imageChip->loadTexture("res/Grap/xiazhu.png");
			}

			//托管下注
			if (chipCheckBox)
			{
				if (chipCheckBox->isSelected())
				{
					if (m_checkChip != 0)
					{
						m_chipLayout->setVisible(false);
						MsgGameRequest::sendBet(m_checkChip);
					}
				}
				else
				{
					m_checkChip = 0;
				}
			}
			if (m_ZID == 0)
			{
				if (imageChip && GAMECONFIG.m_nAppType != GAME_TYPE_KXNNL)
				{
					imageChip->loadTexture("res/Grap/mskx.png");
				}
				for (int i = 0; i < m_chipButtonVec.size(); i++)
				{
					m_chipButtonVec.at(i)->setVisible(false);
				}
			}
		}
		else
		{
			m_chipLayout->setVisible(false);
			showOtherBackPoker();
		}
	}
		break;
	case QGameLayer::GS_OPEN_POKER:
	{
		if (QGameManager::getInstance()->getReady() != 0)
		{
			m_RoomClock->CancelTick();		
			m_RoomClock->Tick(PROCECONFIG.m_fOpenPoker, false);
			//显示计算
			m_chipLayout->setVisible(false);
			m_countLayout->setVisible(true);
			//翻牌
			auto fanButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_Fan"));
			if (fanButton)
			{
				fanButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchCountEvent, this));
			}
			//自动算牌
			auto zdspButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_ZDSN"));
			if (zdspButton)
			{
				zdspButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchFanCardEvent, this));
			}
			//显示桌面上的牌 背面牌
			for (int index = 0; index < GAMECONFIG.m_max_num; index++)
			{
				if (m_player[index] == nullptr || index == 0)
					continue;
				else
				{
					m_palyerPockerVec.at(index)->setVisible(true);
				}
			}
			for (int i = 0; i < m_imageview.size(); i++)
			{
				auto temp = m_imageview.at(i);
				if (temp)
				{
					temp->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchPocketEvent, this));
					temp->setTouchEnabled(true);
					temp->setTag(100 + i);
					temp->setVisible(true);
					temp->removeBackGroundImage();
				}
			}
		}
		else
		{
			m_countLayout->setVisible(false);
		}
	}
		break;
	case QGameLayer::GS_GAME_RET:
	{
		if (QGameManager::getInstance()->getReady() != 0)
		{
			m_RoomClock->CancelTick();
			auto wuButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_WN"));
			if (wuButton)
			{
				wuButton->setVisible(false);
			}
			auto openButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_Open"));
			if (openButton)
			{
				openButton->setVisible(false);
			}
			if (!m_imageview.empty())
			{
				for (int i = 0; i < m_imageview.size(); i++)
				{
					m_imageview.at(i)->setTouchEnabled(false);
				}
			}
			MsgGameRequest::sendOpenPoker();
			if (m_gameNum == QHALLMGR->getPlayMode())
			{
				isGameOver = true;
			}
		}
	}
		break;
	default:
		break;
	}
}
void QGameLayer::startNext(float dt)
{
	unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::startNext));
	//重置UI
	m_result->setVisible(false);
	this->ResetLayer();
	if (m_gameNum == QHALLMGR->getPlayMode())
	{
		if(m_gameOverlayer) 
			m_gameOverlayer->setVisible(true);
		return;
	}

	MsgGameRequest::sendReady();
	
}
//结算---小结窗口
void QGameLayer::updateResult(EventCustom* event)
{
	auto vectResult = QGameManager::getInstance()->m_vecResult_;
	for (int i = 0; i < vectResult.size(); i++)
	{
		auto resultData = vectResult.at(i);
		auto score = resultData->getScore();
		auto seatId = resultData->getSeatId();
		if (score > 0)
		{
			ChipToWinner(seatId);
			if (seatId == QGameManager::getInstance()->getSeatId())
			{
				PlayWinAction();
			}
		}
		int index = (seatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
		auto panel_result = dynamic_cast<Layout*>(m_playerPanelVec.at(index)->getChildByName("Panel_Result"));
		if (panel_result)
		{
			auto bg = dynamic_cast<ImageView*>(panel_result->getChildByName("Image_Bg"));
			if (bg)
			{
				if (score >= 0)
				{
					bg->loadTexture("GameScene/BgWin.png");
				}
				else
				{
					bg->loadTexture("GameScene/BgLose.png");
				}
			}
			auto addScore = dynamic_cast<Text*>(panel_result->getChildByName("Text_AddScore"));
			if (addScore)
			{
				if (score >= 0)
				{
					addScore->setColor(Color3B::YELLOW);
				}
				else
				{
					addScore->setColor(Color3B::BLUE);
				}
				addScore->setString(StringTool::intToString(score));
			}
			panel_result->setVisible(true);
		}

	}


	m_bIsStart = true;
	auto text_Time = dynamic_cast<Text*>(m_result->getChildByName("Text_Time"));
	if (text_Time)
	{
		text_Time->setString(this->gettimecurrent());
	}
	auto text_CurNum = dynamic_cast<Text*>(m_result->getChildByName("Text_CurNum"));
	if (text_CurNum)
	{
		char dzRoom[100] = {};
		sprintf(dzRoom, GetGameString("show_gameNum"), m_gameNum, QHallManager::getInstance()->getPlayMode());
		if (text_CurNum)
		{
			text_CurNum->setString(dzRoom);
		}
	}
	m_listView_1->removeAllItems();
	m_listView_2->removeAllItems();
	for (int i = 0; i < vectResult.size(); i++)
	{
		auto resultData = vectResult.at(i);
		auto seatId = resultData->getSeatId();
		int index = (seatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
		auto curPlayer = m_player[index];
		if (curPlayer != nullptr)
		{
			auto temp = (Layout*)m_resultPanel->clone();
			temp->setVisible(true);
			if (i < 4)
			{
				m_listView_1->pushBackCustomItem(temp);
			}
			else
			{
				m_listView_2->pushBackCustomItem(temp);
			}
			auto panel_head = dynamic_cast<Layout*>(temp->getChildByName("Panel_Head"));
			if (panel_head)
			{
				panel_head->removeAllChildren();
				auto pPortrait = WebSprite::create(curPlayer->getStrImage(), Size(100.0f, 100.0f));
				pPortrait->setLocalZOrder(-1);
				panel_head->addChild(pPortrait);
				pPortrait->setPosition(panel_head->getContentSize().width / 2, panel_head->getContentSize().height / 2);
			}
			auto panelName = dynamic_cast<Layout*>(temp->getChildByName("Panel_Name"));
			auto textName = dynamic_cast<Text*>(panelName->getChildByName("Text_Name"));
			textName->setString(curPlayer->getNickName());
			auto textID = dynamic_cast<Text*>(temp->getChildByName("Text_ID"));
			if (textID)
			{
				textID->setString(StringTool::intToString(curPlayer->getUserId()));
			}
			auto score= dynamic_cast<Text*>(temp->getChildByName("Text_Score"));
			if (score)
			{
				score->setString(StringTool::intToString(m_scoreSum[index]));
			}

			auto score_Add = dynamic_cast<Text*>(temp->getChildByName("Text_Score_Add"));
			if (score_Add)
			{
				score_Add->setString(StringTool::intToString(resultData->getScore()));
			}
			auto image_Win = dynamic_cast<ImageView*>(temp->getChildByName("Image_Win"));
			if (image_Win)
			{
				if (resultData->getScore() > 0)
				{
					image_Win->setVisible(true);
				}
			}
			if (m_ZID >= 0 && m_player[m_ZID]->getSeatId() == seatId)
			{
				auto imageZhuang = dynamic_cast<ImageView*>(temp->getChildByName("Image_Zhuang"));
				if (imageZhuang)	imageZhuang->setVisible(true);
			}
			m_scoreSum[index] += (int)resultData->getScore();
			auto pokeVec = m_pocketVec;
			if (index != 0)
				pokeVec = m_spritePockerVec[index - 1];
			char dzPos[32] = { 0 };
			for (int j = 0; j < pokeVec.size(); j++)
			{
				auto pocke = pokeVec.at(j);
				auto pockerType = (PocketSprite::PocketType)(pokeVec.at(j) & 0xF);
				auto pockerNum = (PocketSprite::PocketNum)(pokeVec.at(j) >> 4);
				sprintf(dzPos, "Pocke/%d-%d.png", (int)pockerNum, (int)pockerType);
				PocketSprite* pockeNew = PocketSprite::create(dzPos, pockerType, pockerNum);
				if (pockeNew)
				{
					temp->addChild(pockeNew, 1);
					sprintf(dzPos, "Panel_PK_%d", j);
					auto panel = dynamic_cast <Layout*> (temp->getChildByName(dzPos));
					if (panel)
					{
						pockeNew->setScale(0.5f);
						pockeNew->setPosition(panel->getPosition());
					}
				}
			}
			auto imageNN = dynamic_cast<ImageView*>(temp->getChildByName("Image_ShowNN"));
			if (imageNN)
			{
				char szName[50] = {};
				sprintf(szName, "Niu/N-%d.png", curPlayer->getUserCow());
				imageNN->loadTexture(szName);
			}

			auto textNiu = dynamic_cast<Text*>(temp->getChildByName("Text_Niu"));
			if (textNiu)
			{
				char szName[50] = {};
				sprintf(szName, "Niu_%d", curPlayer->getUserCow());
				textNiu->setString(GetGameString(szName));
			}
			
			if (QGameManager::getInstance()->getSeatId() == resultData->getSeatId() && (int)resultData->getScore() < 0)
			{
				m_isPlay = 0;
				auto resultBg = dynamic_cast<Sprite*>(m_result->getChildByName("Sprite_Bg"));
				if (resultBg)
				{
					resultBg->setTexture("res/GameLose/lose.png");
					auto animation = Animation::create();
					for (int i = 1; i < 11; i++)
					{
						char szName[100] = { 0 };
						sprintf(szName, "res/GameLose/lose/%d.png", i);
						animation->addSpriteFrameWithFile(szName);
					}
					// 控制帧动画的每帧间的播放间隔
					animation->setDelayPerUnit(0.2f);
					// 是否在播放后重置成第一张图片
					animation->setRestoreOriginalFrame(false);
					auto action_Lose = Animate::create(animation);
					resultBg->runAction(action_Lose);
				}
			}
			if (QGameManager::getInstance()->getSeatId() == resultData->getSeatId() && (int)resultData->getScore() > 0)
			{
				m_isPlay = 1;
				auto resultBg = dynamic_cast<Sprite*>(m_result->getChildByName("Sprite_Bg"));
				if (resultBg)
				{
					resultBg->setTexture("res/GameLose/win.png");
					auto animation = Animation::create();
					for (int i = 1; i < 11; i++)
					{
						char szName[100] = { 0 };
						sprintf(szName, "res/GameLose/win/%d.png", i);
						animation->addSpriteFrameWithFile(szName);
					}
					// 控制帧动画的每帧间的播放间隔
					animation->setDelayPerUnit(0.2f);
					// 是否在播放后重置成第一张图片
					animation->setRestoreOriginalFrame(false);
					auto action_Win = Animate::create(animation);
					resultBg->runAction(action_Win);
				}

			}

		}
	}
	
	auto shareButton = dynamic_cast<Button*>(m_result->getChildByName("Button_Share"));
	if (shareButton)
		shareButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchShareEvent, this));
	auto continueButton = dynamic_cast<Button*>(m_result->getChildByName("Button_Continue"));
	if (continueButton)
		continueButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchContinueEvent, this));
}
void QGameLayer::goldMove(Vector<QRetData*> vecResult)
{
	if (QHALLMGR->getZMode() == 3) m_ZID = 0;
	for (int i = 0; i < vecResult.size(); i++)
	{
		auto resultData = vecResult.at(i);
		auto seatId = resultData->getSeatId();
		int index = (seatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
		if (index == 0)
		{
			auto score = resultData->getScore();
			auto imageResult = dynamic_cast<ImageView*>(m_showResult->getChildByName("Image_Result"));
			if (imageResult && score >= 0)
			{
				imageResult->setVisible(true);
				imageResult->loadTexture("res/GameScene/win.png");
			}
			else if (imageResult && score < 0)
			{
				imageResult->setVisible(true);
				imageResult->loadTexture("res/GameScene/lose.png");
			}
		}
		if (m_ZID == index)
		{
			auto zData = resultData;
			for (int j = 0; j < vecResult.size(); j++)
			{
				auto otherData = vecResult.at(j);
				auto otherSeatId = otherData->getSeatId();
				int indexView = (otherSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
				if (m_ZID == indexView)
					continue;
				GameAudioEngine::getInstance()->playEffect("sound/gold_fly.mp3");
				if (zData->getScore()>otherData->getScore())
				{
					for (int m = 0; m < 6; m++)
					{
						auto gold = Sprite::create("res/GameScene/gold.png");
						//gold->setScale(0.2f);
						float x = m_HeadPos[indexView]->getPositionX();
						float y = m_HeadPos[indexView]->getPositionY();
						float rand_x = CCRANDOM_0_1()*(x + 50 - (x - 50) + 1) + (x - 50);  //产生一个从start到end间的随机数 
						float rand_y = CCRANDOM_0_1()*(y + 50 - (y - 50) + 1) + (y - 50);  //产生一个从start到end间的随机数
						gold->setPosition(Vec2(rand_x, rand_y));
						this->addChild(gold);
						m_goldVec[indexView].push_back(gold);
					}
					auto goldmove = MoveTo::create(0.5f, m_HeadPos[m_ZID]->getPosition());
					for (int n = 0; n < m_goldVec[indexView].size(); n++)
					{
						m_goldVec[indexView].at(n)->runAction(Sequence::create(goldmove->clone(), RemoveSelf::create() , nullptr));
					}
					m_goldVec[indexView].clear();
				}
				else
				{
					for (int m = 0; m < 6; m++)//固定创建6个金币
					{
						auto gold = Sprite::create("res/GameScene/gold.png");
						//gold->setScale(0.2f);
						float x = m_HeadPos[m_ZID]->getPositionX();
						float y = m_HeadPos[m_ZID]->getPositionY();
						float rand_x = CCRANDOM_0_1()*(x + 50 - (x - 50) + 1) + (x - 50);  //产生一个从start到end间的随机数 
						float rand_y = CCRANDOM_0_1()*(y + 50 - (y - 50) + 1) + (y - 50);  //产生一个从start到end间的随机数
						gold->setPosition(Vec2(rand_x, rand_y));
						this->addChild(gold);
						m_goldVec[m_ZID].push_back(gold);
					}

					auto goldmove = MoveTo::create(0.5f, m_HeadPos[indexView]->getPosition());
					for (int n = 0; n < m_goldVec[m_ZID].size(); n++)
					{
						m_goldVec[m_ZID].at(n)->runAction(Sequence::create(goldmove->clone(), RemoveSelf::create(), nullptr));
					}
					m_goldVec[m_ZID].clear();
				}
			}
		}
	}
}
//显示玩家积分
void QGameLayer::showUserScore()
{
	for (int index = 0; index < GAMECONFIG.m_max_num; index++)
	{
		if (m_player[index] == nullptr)
			continue;
		auto scoreText = dynamic_cast<Text*>(m_playerPanelVec.at(index)->getChildByName("Text_Score"));
		if (scoreText) scoreText->setString(StringTool::intToString(m_scoreSum[index]));
	}
}
//退出
void QGameLayer::touchExitRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	if (m_gameNum == QHALLMGR->getPlayMode())
	{
		if (m_gameOverlayer) 
			m_gameOverlayer->setVisible(true);
		return;
	}
	m_result->setVisible(false);
	MsgGameRequest::sendRoomCancel();

}
//继续游戏
void QGameLayer::touchContinueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::reStartTime));
	unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::startNext));
    if(QGameManager::getInstance()->getGameStatues()>GS_GAME_WAIT)
    {
        return;
    }
	//重置UI
	m_result->setVisible(false);
	this->ResetLayer();
	if (m_gameNum == QHALLMGR->getPlayMode())
	{
		if (m_gameOverlayer)
			m_gameOverlayer->setVisible(true);
		return;
	}
	if (m_ctorWin && m_gameNum < QHALLMGR->getPlayMode())
	{
		MsgGameRequest::sendCtrlWin((unsigned char)(m_gameNum + 1), QGameManager::getInstance()->getSeatId());
	}
	MsgGameRequest::sendReady();
}

void QGameLayer::touchQueryEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	if (m_result)
	{
		m_result->setVisible(true);
	}
}

void QGameLayer::touchShareEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WxSocial::getInstance()->shareCaptureScreen(GAMECONFIG.m_sHttpDownUrl.c_str(), "", "", CC_CALLBACK_1(QGameLayer::onInviteCB, this));
}

void QGameLayer::touchFanCardEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	showMyPoker();
}

//重置UI
void QGameLayer::ResetLayer() 
{
	log("=============ResetLayer================");
	unschedule(schedule_selector(QGameLayer::lightCallBack));
	m_ligthDealer->stopAllActions();
	m_ligthDealer->setVisible(false);
	log("QGameLayer::ResetLayer");

	for (int i = 0; i < GAMECONFIG.m_max_num; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "Sprite_Ready_%d", i);
		auto readySprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName(szName));
		if (readySprite) readySprite->setVisible(false);
		if (m_player[i] != nullptr)
		{
			auto zhuangSprite = dynamic_cast<Sprite*>(m_playerPanelVec.at(i)->getChildByName("Sprite_Chip"));
			if (zhuangSprite) zhuangSprite->setVisible(false);
			auto text_Chip = dynamic_cast<Text*>(m_playerPanelVec.at(i)->getChildByName("Text_Chip"));
			if (text_Chip)
			{
				text_Chip->setString("0");
			}
			auto panel_result = dynamic_cast<Layout*>(m_playerPanelVec.at(i)->getChildByName("Panel_Result"));
			if (panel_result)
			{
				panel_result->setVisible(false);
			}
			m_player[i]->setUserCow(-1);
			m_player[i]->setUserBet(0);
		}
	}
	if (m_vecLight.size() != 0)
	{
		m_vecLight.clear();
	}
	if (m_ZBet)
	{
		m_ZBet->setVisible(false);
		m_ZBet->loadTexture("res/Grap/grapY_1.png");
	}
	for (int index = 0; index < GAMECONFIG.m_max_num; index++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "Sprite_NGrap_%d", index);
		auto grapNSprite = dynamic_cast<Sprite*>(m_grapLayout->getChildByName(szName));
		grapNSprite->setVisible(false);
	}
	//关闭继续游戏和查询结果按钮
	if (m_btnQuery)
	{
		m_btnQuery->setVisible(false);
	}
	if (m_btnContinue)
	{
		m_btnContinue->setVisible(false);
	}
	if (m_vecChip.size())
	{
		for (int i = 0; i < m_vecChip.size(); i++)
		{
			m_vecChip.at(i)->removeFromParentAndCleanup(true);
		}
		m_vecChip.clear();
	}

	//关闭牛层
	m_showResult->setVisible(false);
	for (int i = 0; i < m_spriteNiu.size(); i++)
	{
		m_spriteNiu.at(i)->setVisible(false);
	}
	auto imageResult = dynamic_cast<ImageView*>(m_showResult->getChildByName("Image_Result"));
	if (imageResult)
	{
		imageResult->setVisible(false);
	}
	for (int i = 0; i < m_niuBg.size(); i++)
	{
		m_niuBg.at(i)->setVisible(false);
		m_niuBg.at(i)->setScale(0.5f);
	}
	//显示抢庄 和下注按钮
	if (QHALLMGR->getZMode() == 5)
	{
		setGrapButton(true,false);
	}
	if (QHALLMGR->getZMode() == 0)
	{
		setGrapButton(false, true);
	}
	for (int i = 0; i < m_chipButtonVec.size(); i++)
	{
		if (QHALLMGR->getMaxBet() == 3)
		{
			if (i == 0 || i == 4)
			{
				m_chipButtonVec.at(i)->setVisible(false);
			}
			else{
		m_chipButtonVec.at(i)->setVisible(true);
	}
		}
		else{
			m_chipButtonVec.at(i)->setVisible(true);
		}
	}
	for (int i = 0; i < m_pocketUser.size(); i++)
	{
		m_pocketUser.at(i)->loadTexture("Pocke/Back.png");
		m_pocketUser.at(i)->setVisible(false);
	}
	//关闭牌层
	for (int index = 0; index < m_palyerPockerVec.size(); index++)
	{
		if (index != 0 && m_palyerPockerVec.at(index)->isVisible())
		{
			char szName[32] = { 0 };
			for (int i = 1; i < 6; i++)
			{
				sprintf(szName, "Sprite_%d", i);
				auto sprite = dynamic_cast<Sprite*>(m_palyerPockerVec.at(index)->getChildByName(szName));
				if (sprite)
				{
					sprite->setTexture("res/Pocke/Back.png");
					sprite->setPositionX(m_palyerPockerVec.at(index)->getContentSize().width * i / 6);
					sprite->setVisible(false);
				}
			}
			m_palyerPockerVec.at(index)->setVisible(true);
		}
	}
	m_pocketVec.clear();
	for (int i = 0; i < GAMECONFIG.m_max_num - 1; i++)
	{
		if (m_spritePockerVec[i].size() > 0)
		{
			for (int j = 0; j < m_spritePockerVec[i].size(); j++)
			{
				m_spritePockerVec[i].clear();
			}
		}
	}
	COUNT = 0;

	for (int i = 0; i < 3; i++)
	{
		m_coutPockNum[i] = nullptr;
	}
	if (QHallManager::getInstance()->getZMode() != 2)
	{
		m_ZID = -1;
	}
	CoutSum = 0;
}

//重置UI
void QGameLayer::ResetLayerComeBack()
{
	log("===============ResetLayerComeBack==============");
	this->unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::updateSendCard));
	indexCount = 0;
	sendIndex = 0;
	int iCount = m_vecLight.size();
	if (iCount > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			m_vecLight[i]->setVisible(false);
		}
		m_vecLight.clear();
	}
	log("QGameLayer::ResetLayer");
	auto readyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Ready"));
	if (readyButton) readyButton->setVisible(true);
	auto statSprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName("Sprite_Starting"));
	if (statSprite) statSprite->setVisible(false);
	this->ResetLayer();
	m_showResult->setVisible(false);
	m_countLayout->setVisible(false);
	m_lightLayout->setVisible(true);
	m_RoomClock->setVisible(false);
	m_result->setVisible(false);
	m_chipLayout->setVisible(false);
	m_grapLayout->setVisible(false);
	if (m_dismiss)
	{
		m_dismiss->removeFromParentAndCleanup(true);
		m_dismiss = nullptr;
	}
}

//更新头像
void QGameLayer::updateHead()
{
	int iCount = GAMECONFIG.m_max_num;
	QUserData *qUser = nullptr;
	int index = 0;
	for (int i = 0; i < QGameManager::getInstance()->m_vecUser_.size(); i++)
	{
		qUser = QGameManager::getInstance()->m_vecUser_.at(i);
		index = ((int)(qUser->getSeatId()) - (int)(QGameManager::getInstance()->getSeatId()) + iCount) % iCount;
		if (m_player[index] == nullptr)
		{
			m_player[index] = new QUserData();
		}
		if (m_curNum < qUser->getSeatId() + 1)
		{
			m_curNum = qUser->getSeatId() + 1;
		}
		m_player[index]->setUserData(qUser);

		m_playerPanelVec.at(index)->setVisible(true);
		//显示头像
		auto m_userHead = dynamic_cast<Layout*>(m_playerPanelVec.at(index)->getChildByName("Panel_head"));
		if (!m_userHead)
			return;
		m_userHead->removeAllChildren();
		m_verUserHead.push_back(m_userHead);

        WebSprite *pPortrait =  nullptr;
        pPortrait = WebSprite::create(qUser->getStrImage(), Size(148.0f, 158.0f));
		pPortrait->setLocalZOrder(-1);
		m_userHead->addChild(pPortrait);
		pPortrait->setPosition(m_userHead->getContentSize().width / 2, m_userHead->getContentSize().height / 2);
		
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			//点击头像
			m_userHead->setTouchEnabled(true);
			m_userHead->setTag(index);
			m_userHead->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchHeadEvent, this));
		}
		//显示昵称
		auto name_layout = dynamic_cast<Layout*>(m_playerPanelVec.at(index)->getChildByName("Panel_Name"));
		if (name_layout)
		{
			auto textName = dynamic_cast<Text*>(name_layout->getChildByName("Text_name"));
			if (textName) textName->setString(qUser->getNickName());
		}
		auto textScore = dynamic_cast<Text*>(m_playerPanelVec.at(index)->getChildByName("Text_Score"));
		if (textScore) textScore->setString(StringTool::intToString(m_scoreSum[index]));
	}
	auto textUID = dynamic_cast<Text*>(m_playerPanelVec.at(0)->getChildByName("Text_UID"));
	if (textUID) textUID->setString(StringTool::intToString(QGameManager::getInstance()->getUserId()));

}
//加入房间
void QGameLayer::updateEvent(EventCustom* event)
{
	this->updateHead();
}

//下注
void QGameLayer::touchChipEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto button = (Button*)sender;
	if (button->getName() == "Button_Two")
	{
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			switch (QHALLMGR->getMaxBet())
			{
			case 3:
				button->setVisible(false);
				break;
			case 5:
				MsgGameRequest::sendBet(1);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 1;
				break;
			case 10:
				MsgGameRequest::sendBet(2);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 2;
				break;
			case 30:
				MsgGameRequest::sendBet(5);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 5;
				break;
			default:
				break;
			}
		}
		else
		{
			MsgGameRequest::sendBet(2);
			if (chipCheckBox&&chipCheckBox->isSelected())
			{
				m_checkChip = 2;
			}
		}
			
	}
	else if (button->getName() == "Button_Three")
	{
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			switch (QHALLMGR->getMaxBet())
			{
			case 3:
				MsgGameRequest::sendBet(1);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 1;
				break;
			case 5:
				MsgGameRequest::sendBet(2);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 2;
				break;
			case 10:
				MsgGameRequest::sendBet(4);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 4;
				break;
			case 30:
				MsgGameRequest::sendBet(10);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 10;
				break;
			default:
				break;
			}
		}
		else
		{
			MsgGameRequest::sendBet(3);
			if (chipCheckBox&&chipCheckBox->isSelected())
			{
				m_checkChip = 3;
			}
		}
	}
	else if (button->getName() == "Button_Four")
	{
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			switch (QHALLMGR->getMaxBet())
			{
			case 3:
				MsgGameRequest::sendBet(2);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 2;
				break;
			case 5:
				MsgGameRequest::sendBet(3);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 3;
				break;
			case 10:
				MsgGameRequest::sendBet(6);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 6;
				break;
			case 30:
				MsgGameRequest::sendBet(20);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 20;
				break;
			default:
				break;
			}
		}
		else
		{
			MsgGameRequest::sendBet(4);
			if (chipCheckBox&&chipCheckBox->isSelected())
			{
				m_checkChip = 4;
			}
		}
	}
	else if (button->getName() == "Button_Five")
	{
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			switch (QHALLMGR->getMaxBet())
			{
			case 3:
				MsgGameRequest::sendBet(3);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 3;
				break;
			case 5:
				MsgGameRequest::sendBet(4);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 4;
				break;
			case 10:
				MsgGameRequest::sendBet(8);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 8;
				break;
			case 30:
				MsgGameRequest::sendBet(25);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 25;
				break;
			default:
				break;
			}
		}
		else
		{
			MsgGameRequest::sendBet(5);
			if (chipCheckBox&&chipCheckBox->isSelected())
			{
				m_checkChip = 5;
			}
		}
	}
	else if (button->getName() == "Button_Ten")
	{
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			switch (QHALLMGR->getMaxBet())
			{
			case 3:
				button->setVisible(false);
				break;
			case 5:
				MsgGameRequest::sendBet(5);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 5;
				break;
			case 10:
				MsgGameRequest::sendBet(10);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 10;
				break;
			case 30:
				MsgGameRequest::sendBet(30);
				if (chipCheckBox&&chipCheckBox->isSelected())
					m_checkChip = 30;
				break;
			default:
				break;
			}
		}
		else
		{
			MsgGameRequest::sendBet(10);
			if (chipCheckBox&&chipCheckBox->isSelected())
			{
				m_checkChip = 10;
			}
		}
	}
	else
	{
		return;
	}

	m_chipLayout->setVisible(false);

}
//抢庄  与  不抢庄
void QGameLayer::touchGrapEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED || sender == nullptr)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	
	setGrapButton(false);
	Node *node =  dynamic_cast<Node *>(sender);

	MsgGameRequest::sendRobZhuang(node->getTag());
}
void QGameLayer::touchGrapNoEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);

	setGrapButton(false);
	MsgGameRequest::sendRobZhuang(0);
}
void QGameLayer::setGrapButton(bool isFKQ,bool isJDQ)
{
	for (int i = 0; i < 4; i++) {
		std::stringstream stream;
		stream << "Button_Grap_" << i;
		auto grapButton = dynamic_cast<Button*>(m_grapLayout->getChildByName(stream.str()));
		if (grapButton)
			grapButton->setVisible(isFKQ);
	}
	auto grapNButton_0 = dynamic_cast<Button*>(m_grapLayout->getChildByName("Button_GrapN_0"));
	auto grapYButton = dynamic_cast<Button*>(m_grapLayout->getChildByName("Button_Grap"));
	auto grapNButton = dynamic_cast<Button*>(m_grapLayout->getChildByName("Button_GrapN"));
	auto grapText = dynamic_cast<ImageView*>(m_grapLayout->getChildByName("Image_GrapText"));
	if (grapNButton_0) grapNButton_0->setVisible(isFKQ);
	if (grapYButton) grapYButton->setVisible(isJDQ);
	if (grapNButton) grapNButton->setVisible(isJDQ);
	if (grapText) grapText->setVisible(isFKQ || isJDQ);
}
//准备
void QGameLayer::touchReadyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto readyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Ready"));
	if (readyButton) readyButton->setVisible(false);
	auto statSprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName("Sprite_Starting"));
	if (statSprite) statSprite->setVisible(true);
	MsgGameRequest::sendReady();
}

void QGameLayer::touchStartGameEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	auto curButton = (Button*)sender;
	if (curButton)
	{
		curButton->setVisible(false);
	}
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	MsgGameRequest::sendStartGame(QGameManager::getInstance()->getUserId());
}

//快捷聊天
void QGameLayer::touchTalkEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
    bool isCan = isCanSpeek();
    if (isCan)
    {
        auto  talk = TalkLayer::create();
        talk->showDialog(this);
    }
}

//邀请好友
void QGameLayer::touchInviteFriendEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
    int roomId = QGameManager::getInstance()->getRoomNum();
	char szType[32] = { 0 };
	sprintf(szType, "WXInvite_%d", GAMECONFIG.m_nAppType);
	char szTitle[512] = { 0 };
	sprintf(szTitle, GetGameString(szType), roomId);
	
	int tMode = QHALLMGR->getPlayMode();	//局数
	int bMode = QHALLMGR->getZMode();		//坐庄模式
	int pMode = QHALLMGR->getCostMode();	//付卡模式
	int mMode = QHALLMGR->getMMode();		//倍数模式
	int cover = QHALLMGR->getKcardWay();	//盖住几张牌
	int half = QHALLMGR->getHalf();			//半副扑克
	int forbid = QHALLMGR->getForbid();		//开局后禁止加入

	if (cover == 1)
	{
		bMode = 9;
	}
    char szBMode[100] = {0};
	sprintf(szBMode, "GameMode%d", bMode);
	char szPMode[100] = { 0 };
	sprintf(szPMode, "room_cost%d", pMode + 1);
	char szMMode[100] = {};
	sprintf(szMMode, "NF_%d", mMode);
	char szHalf[100] = {};
	sprintf(szHalf, "Half_%d", half);
	char szForbid[100] = {};
	sprintf(szForbid, "Forbid_%d", forbid);

    char szDes[512] ={0};  
	sprintf(szDes, GetGameString("WXInvite2"), GAMECONFIG.m_max_num, (GAMECONFIG.m_max_num - m_curNum), GetGameString(szBMode),
		tMode, GetGameString(szPMode), GetGameString(szHalf), GetGameString(szMMode), GetGameString(szForbid));
	
	WxSocial::getInstance()->shareRoom(GAMECONFIG.m_sHttpDownUrl.c_str(), szTitle, szDes, CC_CALLBACK_1(QGameLayer::onInviteCB, this));	
}

void QGameLayer::touchCopyRoomNumEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	char str[50] = {};
	const char *roomNum = StringTool::intToString(QGameManager::getInstance()->getRoomNum()).c_str();
	strcpy(str,roomNum);
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

void QGameLayer::onInviteCB(int errorCode)
{
	return;
}

//暂离
void QGameLayer::touchExitEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	MsgGameRequest::sendRoomCancel();
}
//设置
void QGameLayer::touchSetEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto layer = SetLayer::create();
	this->addChild(layer);
}

//语音
void QGameLayer::touchSpeekEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    static bool isCreate = false;

    if (type == Widget::TouchEventType::BEGAN)
    {
        bool isCan = isCanSpeek();
        if (isCan)
        {
			if (!m_voiceLayer)
			{
				m_voiceLayer = GameVoiceRecordLayer::create();
				m_voiceLayer->setName("voice");
				m_voiceLayer->showDialog(this);
			}
            isCreate = true;
        }
    }
    
    if (type == Widget::TouchEventType::CANCELED)
    {
		if (m_voiceLayer && isCreate)
        {
			m_voiceLayer->cancelRecord();
			m_voiceLayer = nullptr;
            isCreate = false;
        }
    }
    
    if (type == Widget::TouchEventType::ENDED)
    {
		if (m_voiceLayer && isCreate)
        {
			m_voiceLayer->stopRecord();
			m_voiceLayer = nullptr;
            isCreate = false;
        }
    }
}

//解散------
void QGameLayer::touchDismissEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	//m_dissolveButton->setTouchEnabled(false);
	m_dismiss = DismissLayer::create(m_gameNum);
	if (m_dismiss)
	{
		this->addChild(m_dismiss, 1);
		m_dismiss->setSelfParent(this);
	}
}

void QGameLayer::touchGZBtnEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	string filePath = "";
	switch (QHALLMGR->getMMode())
	{
	case 0:
		filePath = "System_Exit/WNF.png";
		break;
	case 1:
		filePath = "System_Exit/YNF.png";
		break;
	default:
		break;
	}

	if (m_popupLayer == nullptr) 
	{
		m_popupLayer = PopupLayer::create(filePath);
		this->addChild(m_popupLayer, 1);
	}
	auto bg_layout = dynamic_cast<Layout*>(m_popupLayer->m_bgLayout);
	if (bg_layout)
	{
		bg_layout->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchCloseGzEvent, this));
	}
	auto closeBtn = dynamic_cast<Button*>(m_popupLayer->m_systemLayout->getChildByName("Button_Close"));
	if (closeBtn) closeBtn->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchCloseGzEvent, this));
}

void QGameLayer::touchCloseGzEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	if (m_popupLayer)
	{
		m_popupLayer->removeFromParentAndCleanup(true);
		m_popupLayer = nullptr;
	}
}

void QGameLayer::touchHeadEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto head = (Layout*)sender;
	int tag = head->getTag();
	if (m_HeadPos[tag] == nullptr)
		return;
	auto layer = Information::create(m_player[tag]);
	layer->setAnchorPoint(Vec2(0.0f, 0.0f));
	layer->setPosition(m_HeadPos[tag]->getPosition());
	auto moveAction = MoveTo::create(0.2f, Vec2(0.0f, 0.0f));
	layer->runAction(moveAction);
	this->addChild(layer);
}

//自己计算   15s后自动结算
void QGameLayer::touchPocketEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto button = (Button*)sender;
	int tag = button->getTag();

	int index = tag % 100;
	if (!m_pocketVec.at(index)) return;
	//if (m_pocketVec.at(index)->getIsUp() == false)
	//{
	//	if (COUNT == 3)
	//	{
	//		return;
	//	}
	//	m_pocketVec.at(index)->setIsUp(true);
	//	for (int i = 0; i < 3; i++)
	//	{
	//		if (m_coutPockNum[i] == nullptr)
	//		{
	//			m_coutPockNum[i] = m_pocketVec.at(index);
	//			m_textVec.at(i)->setVisible(true);
	//			if ((int)(m_coutPockNum[i]->getPNum()) >= 10)
	//			{
	//				m_textVec.at(i)->setString("10");
	//			}
	//			else
	//			{
	//				m_textVec.at(i)->setString(StringTool::intToString((int)(m_coutPockNum[i]->getPNum())));
	//			}
	//			break;
	//		}
	//	}
	//	COUNT++;
	//	if (COUNT == 3)
	//	{
	//		int sum = 0;
	//		int a = 0;
	//		for (int i = 0; i < 3; i++)
	//		{
	//			a = (int)(m_coutPockNum[i]->getPNum());
	//			if (a >= 10)
	//			{
	//				a = 10;
	//			}
	//			sum += a;
	//		}
	//		m_textVec.at(3)->setVisible(true);
	//		m_textVec.at(3)->setString(StringTool::intToString(sum));
	//		CoutSum = sum;
	//	}
	//}
	//else
	//{
	//	m_pocketVec.at(index)->setIsUp(false);
	//	for (int i = 0; i < 3; i++)
	//	{
	//		if (m_coutPockNum[i] == nullptr)
	//			continue;
	//		if (m_coutPockNum[i]->getPNum() == m_pocketVec.at(index)->getPNum()&&
	//			m_coutPockNum[i]->getPType() == m_pocketVec.at(index)->getPType())
	//		{
	//			m_coutPockNum[i] = nullptr;
	//			m_textVec.at(i)->setVisible(false);
	//			m_textVec.at(3)->setVisible(false);
	//		}
	//	}
	//	COUNT--;
	//	CoutSum = 0;
	//}
}
//点击结算(有牛无牛 按钮事件)
void QGameLayer::touchCountEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	int cow = (int)(QGameManager::getInstance()->getpCardData()->getCow());
	int size_Text = 15;
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		size_Text = 30;
	}
	auto button = (Button*)sender;
	if (button->getName() == "Button_HN")
	{
		//如果是炸弹或五花牛或五小牛  点击有牛 直接开牌
		if (this->isHaveCattle() && QGameManager::getInstance()->getpCardData()->getCow() > 10)
		{
			MsgGameRequest::sendOpenPoker();
			m_countLayout->setVisible(false);
		}
		else
		{

			if (COUNT != 3)
			{
				//请选择三张牌
				auto loginFail = ScriptDataManager::getInterface()->getGameString("check_three");
				//弹窗
				PopLayer* popup = PopLayer::create();
				popup->setContentText(loginFail.c_str(), size_Text, 800, 500);
				Director::getInstance()->getRunningScene()->addChild(popup);
			}
			else
			{
				if (CoutSum % 10 == 0)
				{
					MsgGameRequest::sendOpenPoker();
					m_countLayout->setVisible(false);
				}
				else
				{
					if (this->isHaveCattle())
					{
						//再思考一下，请重新选牌。。。
						auto loginFail = ScriptDataManager::getInterface()->getGameString("check_CX");
						//弹窗
						PopLayer* popup = PopLayer::create();
						popup->setContentText(loginFail.c_str(), size_Text, 800, 500);
						Director::getInstance()->getRunningScene()->addChild(popup);
					}
					else
					{
						//你的牛呢？？
						auto loginFail = ScriptDataManager::getInterface()->getGameString("check_WN");
						//弹窗
						PopLayer* popup = PopLayer::create();
						popup->setContentText(loginFail.c_str(), size_Text, 800, 500);
						Director::getInstance()->getRunningScene()->addChild(popup);
					}
				}
			}
		}
	}
	else if (button->getName() == "Button_WN")
	{
		if (cow > 0)
		{
			//再思考一下，你有牛哦。。。
			auto loginFail = ScriptDataManager::getInterface()->getGameString("check_HN");
			//弹窗
			PopLayer* popup = PopLayer::create();
			popup->setContentText(loginFail.c_str(), size_Text, 800, 500);
			Director::getInstance()->getRunningScene()->addChild(popup);
		}
		else
		{
			MsgGameRequest::sendOpenPoker();
			m_countLayout->setVisible(false);
		}
	}
	else if (button->getName() == "Button_Open")
	{
		if (cow <= 0)
		{
			PromptCow();
		}
		else
		{
			MsgGameRequest::sendOpenPoker();
			m_countLayout->setVisible(false);
		}
	}
	else if (button->getName() == "Button_Fan")
	{
		MsgGameRequest::sendOpenPoker();
		m_countLayout->setVisible(false);
	}
}
//结算点击提示按钮
void QGameLayer::touchPromptEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	PromptCow();
}
//提示牛几
void QGameLayer::PromptCow()
{
	int size_Text = 15;
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		size_Text = 18;
	}
	int cow = (int)(QGameManager::getInstance()->getpCardData()->getCow());
	////如果是炸弹或五花牛或五小牛  直接弹框提示
	//if (cow > 10 || !this->isHaveCattle())
	//{
	//	char proChar[100] = { 0 };
	//	sprintf(proChar, "COW_%d", cow);
	//	//弹窗
	//	PopLayer* popup = PopLayer::create();
	//	popup->setContentText(GetGameString(proChar), size_Text, 800, 500);
	//	Director::getInstance()->getRunningScene()->addChild(popup);
	//	return;
	//}
	if (this->isHaveCattle())
	{
		if (!m_imageview.empty())
		{
			for (int i = 0; i < m_imageview.size(); i++)
			{
				m_imageview.at(i)->setTouchEnabled(false);
			}
		}
		//for (int i = 0; i < m_pocketVec.size(); i++)
		//{
		//	if (m_pocketVec[i]->getIsUp())
		//	{
		//		m_pocketVec[i]->setIsUp(false);
		//	}
		//}
		bool isBreak = false;
		for (int i = 0; i < 5; i++)
		{
			for (int j = i + 1; j < 5; j++)
			{
				for (int k = j + 1; k < 5; k++)
				{
					int card11 = m_pocketVec.at(i) >> 4;
					int card22 = m_pocketVec.at(j) >> 4;
					int card33 = m_pocketVec.at(k) >> 4;
					int u08Value = GetValue(card11) + GetValue(card22) + GetValue(card33);
					if (u08Value % 10 == 0)
					{
						m_pocketUser.at(i)->setPositionY(m_PocketPos->getContentSize().height / 2 + 10);
						m_pocketUser.at(j)->setPositionY(m_PocketPos->getContentSize().height / 2 + 10);
						m_pocketUser.at(k)->setPositionY(m_PocketPos->getContentSize().height / 2 + 10);
						COUNT = 3;
						CoutSum = u08Value;
						isBreak = true;
						break;
					}
				}
				if (isBreak)
					break;
			}
			if (isBreak)
				break;
		}
	}
}
//自动结算
void QGameLayer::ShowNiu(int nn, int id)
{
	m_showResult->setVisible(true);
	if (m_player[id])
		m_player[id]->setUserCow(nn);
	for (int i = 0; i < GAMECONFIG.m_max_num; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "Sprite_%d", i);
		auto sprite = dynamic_cast<Sprite*>(m_showResult->getChildByName(szName));
		m_spriteNiu.push_back(sprite);
		sprintf(szName, "Sprite_BG_%d", i);
		auto bg = dynamic_cast<Sprite*>(m_showResult->getChildByName(szName));
		if (bg)
		{
			m_niuBg.push_back(bg);
		}
	}
	char chat[100] = { 0 };
	if (m_player[id]->getSex() == 2)
	{
		sprintf(chat, "sound/res_female_%d.mp3", nn);
	}
	else
	{
		sprintf(chat, "sound/res_male_%d.mp3", nn);
	}
	GameAudioEngine::getInstance()->playEffect(chat);

	//显示没牛0   有牛1
	char nzCow[32] = { 0 };
	if (QHALLMGR->getMMode() == 0)
	{
		sprintf(nzCow, "res/Niu/wnf/N-%d.png", nn);
	}
	else{
		sprintf(nzCow, "res/Niu/ynf/N-%d.png", nn);
	}
	m_spriteNiu.at(id)->setTexture(nzCow);
	m_spriteNiu.at(id)->setVisible(true);
	m_spriteNiu.at(id)->setScale(1.5f);
	m_niuBg.at(id)->setTexture("Niu/wnf/bg.png");
	m_niuBg.at(id)->setVisible(true);
	m_niuBg.at(id)->setScale(1.5f);
	ScaleTo* scaleNiu = nullptr;
	if (m_player[id]->getSeatId() == QGameManager::getInstance()->getSeatId())
	{
		scaleNiu = ScaleTo::create(0.5f, 1.0f);
	}
	else
	{
		scaleNiu = ScaleTo::create(0.5f, 0.8f);
	}

	ScaleTo* scaleBg = nullptr;
	if (m_player[id]->getSeatId() == QGameManager::getInstance()->getSeatId())
	{
		scaleBg = ScaleTo::create(0.5f, 1.0f);
	}
	else
	{
		scaleBg = ScaleTo::create(0.5f, 0.8f);
	}
	m_spriteNiu.at(id)->runAction(EaseIn::create(scaleNiu, 2));
	m_niuBg.at(id)->runAction(EaseIn::create(scaleBg, 2));
}
//游戏状态机
void QGameLayer::update(float delta)
{
	m_systemTime->setString(this->gettimecurrent());
}
bool QGameLayer::isHaveCattle()
{
	if (m_pocketVec.size() != 5)
		return false;
    std::vector<int> t_vPocketVal;
    t_vPocketVal.clear();
	for (int i = 0; i < 5; i++)
	{
		int value = m_pocketVec.at(i) >> 4;
		t_vPocketVal.push_back(value);
	}
    int result = GetCow(t_vPocketVal);
	return result > 0;

}
//时间
string QGameLayer::gettimecurrent()
{
	time_t t;
	time(&t);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%X", localtime((&t)));
	string timeStr = tmp;
	return timeStr;
}
//*改的*
void QGameLayer::updateSendCard(float dt)
{
	float time = GAMECONFIG.m_actionTime;
	int index_Card = m_pocketVec.size();
	if (indexCount >= index_Card)
		indexCount = 0;
	if (sendIndex >= index_Card) sendIndex = 0;
	auto size_Win = Director::getInstance()->getVisibleSize();
	char card_name[100] = {};
	sprintf(card_name, "Image_%d", indexCount + 1);
	auto pocket_1 = dynamic_cast<ImageView*>(m_PocketPos->getChildByName(card_name));
	if (!pocket_1)
		return;
	auto pocket_Size = pocket_1->getContentSize();
	auto card_1 = Sprite::create("Pocke/Back.png");

	card_1->setPosition(size_Win.width / 2.0, size_Win.height / 2.0);
	card_1->setScale(0.5f);
	this->addChild(card_1);
	auto moveTo = MoveTo::create(time, Vec2(m_PocketPos->getPositionX() - m_PocketPos->getContentSize().width/2, m_PocketPos->getPositionY()));
	auto scaleTo = ScaleTo::create(time, 0.5f);
	auto rotateBy = RotateBy::create(time, 360.0f);
	auto spawn_Card = Spawn::create(moveTo, scaleTo, rotateBy, nullptr);
	card_1->runAction(Sequence::create(spawn_Card, CallFunc::create(
		[&]{
		GameAudioEngine::getInstance()->playEffect("sound/sendCard.mp3");
		if (sendIndex < m_pocketVec.size()) {

			m_pocketUser.at(sendIndex)->setVisible(true);
			sendIndex++;

			for (int index = 0; index < GAMECONFIG.m_max_num; index++)
			{
				if (m_player[index] == nullptr || index == 0)
					continue;
				else
				{
					m_palyerPockerVec.at(index)->setVisible(true);
					char szName[255] = {};
					sprintf(szName, "Sprite_%d", sendIndex);
					auto sprite_pocket = dynamic_cast<Sprite*>(m_palyerPockerVec.at(index)->getChildByName(szName));
					sprite_pocket->setPositionX(m_palyerPockerVec.at(index)->getContentSize().width * (sendIndex) / 6);
					sprite_pocket->setVisible(true);
				}
			}
			
		}   
	}), RemoveSelf::create(),nullptr));

	indexCount++;
	if (indexCount == index_Card)
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(QGameLayer::updateSendCard));
	}
}

void QGameLayer::lightCallBack(float dt)
{
	int iCount = m_vecLight.size();
	if (iCount > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			if (m_vecLight[i]->isVisible())
			{
				m_vecLight[i]->setVisible(false);
				int iTar = (i + 1) % iCount;
				m_vecLight[iTar]->setVisible(true);
				GameAudioEngine::getInstance()->playEffect("sound/button.mp3");
				return;
			}
		}
	}
}
Animate* QGameLayer::lightBlink()
{
	GameAudioEngine::getInstance()->playEffect("sound/zhuang.mp3");
	auto animation = Animation::create();
	for (int i = 1; i < 7; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "Light/light%d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	// 控制帧动画的每帧间的播放间隔
	animation->setDelayPerUnit(0.2f);
	// 是否在播放后重置成第一张图片
	animation->setRestoreOriginalFrame(true);
	auto action_Blink = Animate::create(animation);
	return action_Blink;
}
//显示庄家
void QGameLayer::showZhuang()
{
	if (m_ZID >= 0 && m_ZID < GAMECONFIG.m_max_num)
	{
		if (m_player[m_ZID] == nullptr)
			return;
		auto zhuangSprite = dynamic_cast<Sprite*>(m_playerPanelVec.at(m_ZID)->getChildByName("Sprite_Chip"));
		if (zhuangSprite)
		{
			zhuangSprite->setTexture("res/GameScene/zhuang.png");
			zhuangSprite->setVisible(true);
		}
		if (QHALLMGR->getZMode() == 5 && m_ZBet)
		{
			m_ZBet->setVisible(true);
		}
	}
}
//断线重连回来
void QGameLayer::updateGameBack(EventCustom* event)
{
	ResetLayerComeBack();
	QGameBackData* gameBackData = static_cast<QGameBackData*>(event->getUserData());
	QCardData* cardData = QGameManager::getInstance()->getpCardData();
	auto vecCard = cardData->getPokerVec();
	auto userVec = gameBackData->getUserBackVec();
	int cMulti = gameBackData->getMulti();
	m_gameNum = gameBackData->getPlayNum();

	int countNum = 0;
	for (int i = 0; i < 8; i++)
	{
		if (m_player[i] != nullptr)
		{
			countNum++;
		}
	}
	if (countNum >= 2 && QGameManager::getInstance()->getSeatId() == 0 && m_gameNum == 0)//如果是当前视图的人
	{
		auto startButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Start"));
		if (startButton)
		{
			startButton->setVisible(true);
			startButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchStartGameEvent, this));
		}
	}
	char dzRoom[32] = { 0 };
	sprintf(dzRoom, GetGameString("show_gameNum"), m_gameNum, QHallManager::getInstance()->getPlayMode());
	if (m_countNum)
		m_countNum->setString(dzRoom);
	if (m_gameNum >= 1 && QGameManager::getInstance()->getReady() != 0)
	{
		m_pIsSendPocke = true;
		if (chipCheckBox)	
			chipCheckBox->setVisible(true);
	}
	if (m_gameNum>=2)
		m_bIsStart = true;
	if (QGameManager::getInstance()->getReady() == 0 && gameBackData->getGameStatues() > QGameLayer::GS_GAME_WAIT)
	{
		if (m_running)
		{
			m_running->setVisible(true);
		}
	}
	if (gameBackData->getGameStatues() >= QGameLayer::GS_PLAYER_BET)
	{
		char szName[30] = {};
		sprintf(szName, "res/Grap/grapY_%d.png", cMulti);
		if (m_ZBet && QHALLMGR->getZMode() == 5)
		{
			m_ZBet->loadTexture(szName);
			m_ZBet->setVisible(true);
		}
	}

	//积分
	for (int i = 0; i < userVec.size(); i++)
	{ 
		auto userData = userVec.at(i);
		int index = ((int)userData->getSeatId() - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
		m_scoreSum[index] = userData->getScore();
		auto scoreText = dynamic_cast<Text*>(m_playerPanelVec.at(index)->getChildByName("Text_Score"));
		if(scoreText) scoreText->setString(StringTool::intToString(m_scoreSum[index]));
		//掉线回来记录玩家的下注
		m_player[index]->setUserBet(userData->getBetCount());
		//显示掉线玩家
		if (userData->getConnect() == 0 && m_loseRoom[index] == nullptr)
		{
			m_loseRoom[index] = Sprite::create("res/GameScene/diaoXian.png");
			auto imageHead = dynamic_cast<Layout*>(m_playerPanelVec.at(index)->getChildByName("Panel_head"));
			if (imageHead)
			{
				m_verUserHead.push_back(imageHead);
				m_loseRoom[index]->setPosition(imageHead->getPosition());
				m_playerPanelVec.at(index)->addChild(m_loseRoom[index]);
			}
		}
	}
	if (m_gameNum > 0)
	{
		for (int i = 0; i < m_verUserHead.size(); i++)
		{
			m_verUserHead[i]->setTouchEnabled(true);
		}
	}
	//标记庄家
	m_ZID = gameBackData->getBanker() == 255 ? -1 : (gameBackData->getBanker() - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
	//播放音乐
	auto userDefault = UserDefault::getInstance();
	float music = userDefault->getFloatForKey(MUSIC_VALUE_TEXT, 0.5);
	float sound = userDefault->getFloatForKey(SOUND_VALUE_TEXT, 0.5);
	GameAudioEngine::getInstance()->playBackgroundMusic(GAME_GAME_BACKGROUND_MUSIC, true);
	GameAudioEngine::getInstance()->setBackgroundMusicVolume(music);
	GameAudioEngine::getInstance()->setEffectsVolume(sound);
	//牌的
	if (vecCard.size() > 0)
	{
		showMyHand();
		int size_Num = m_pocketVec.size();
		for (int i = 0; i < size_Num; i++)
		{
			if (m_pocketUser.at(i))
			{
				auto pocketType = (int)(m_pocketVec.at(i) & 0xF);
				auto pocketNum = (int)(m_pocketVec.at(i) >> 4);
				char szName[20] = { 0 };
				sprintf(szName, "Pocke/%d-%d.png", pocketNum, pocketType);
				m_pocketUser.at(i)->loadTexture(szName);
				m_pocketUser.at(i)->setVisible(true);
			}
			auto c_Cover = QHALLMGR->getKcardWay();
			switch (c_Cover)
			{
			case 1:
				m_pocketUser.at(4)->loadTexture("Pocke/Back.png");
				break;
			case 2:
				m_pocketUser.at(3)->loadTexture("Pocke/Back.png");
				m_pocketUser.at(4)->loadTexture("Pocke/Back.png");
				break;
			case 5:
				m_pocketUser.at(0)->loadTexture("Pocke/Back.png");
				m_pocketUser.at(1)->loadTexture("Pocke/Back.png");
				m_pocketUser.at(2)->loadTexture("Pocke/Back.png");
				m_pocketUser.at(3)->loadTexture("Pocke/Back.png");
				m_pocketUser.at(4)->loadTexture("Pocke/Back.png");
				break;
			default:
				break;
			}
		}

		//显示其他玩家的背面牌
		for (int index = 0; index < GAMECONFIG.m_max_num; index++)
		{
			if (m_player[index] == nullptr)
				continue;
			else
			{
				m_palyerPockerVec.at(index)->setVisible(true);
			}
		}
		
	}
	if (gameBackData->getGameStatues() == QGameLayer::GS_OPEN_POKER)
	{
		//开牌状态下有玩家开牌显示玩家的牌
		if (QGameManager::getInstance()->getReady() != 0)
		{
			for (int i = 0; i < userVec.size(); i++)
			{
				auto pCard = userVec.at(i)->getpCardData();
				if (pCard->getSeatId() != cardData->getSeatId() && pCard->getPokerVec().size() > 0)
				{
					showOtherHand(pCard);
				}
			}
		}
	}
	//显示庄家
	if (gameBackData->getGameStatues() >= QGameLayer::GS_RAND_DEAL)
	{
		if (m_grapLayout)
			m_grapLayout->setVisible(false);
		if (m_lightLayout)
			m_lightLayout->setVisible(false);
		showZhuang();
	}
	
	//准备按钮
	if (gameBackData->getGameStatues() > QGameLayer::GS_GAME_WAIT)
	{
		if (m_readyLayout)
			m_readyLayout->setVisible(false);
	}
		
	if (gameBackData->getGameStatues() >= QGameLayer::GS_PLAYER_BET)
	{
		if (QGameManager::getInstance()->getSeatId() == gameBackData->getBanker())
		{
			if (m_chipLayout)
			{
				m_chipLayout->setVisible(true);
			}
			auto imageChip = dynamic_cast<ImageView*>(m_chipLayout->getChildByName("Image_Chip"));
			if (imageChip && GAMECONFIG.m_nAppType != GAME_TYPE_KXNNL)
			{
				imageChip->loadTexture("res/Grap/mskx.png");
			}
			for (int i = 0; i < m_chipButtonVec.size(); i++)
			{
				m_chipButtonVec.at(i)->setVisible(false);
			}
		}
		int iNum = userVec.size();
		QUserBackData *qData = nullptr;
		for (int i = 0; i < iNum; i++)
		{
			qData = userVec.at(i);
			if (qData->getBetCount() > 0)
			{
				int iSeatId = qData->getSeatId();
				int index = (iSeatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
				auto chip = dynamic_cast<Text*>(m_playerPanelVec.at(index)->getChildByName("Text_Chip"));
				if (chip)
				{
					chip->setString(StringTool::intToString(qData->getBetCount()));
				}
			}
			else
			{
				if (qData->getSeatId() == QGameManager::getInstance()->getSeatId())
				{
					if (m_chipLayout)
						m_chipLayout->setVisible(true);
				}		
			}
		}

	}

	switch (gameBackData->getGameStatues())
	{
	case QGameLayer::GS_GAME_WAIT:
	{
		if (m_pIsSendPocke)
			m_bIsStart = true;
		//你自己是否准备
		m_result->setVisible(false);
		int iNum = userVec.size();
		QUserBackData *qData = nullptr;
		for (int i = 0; i < iNum; i++)
		{
			qData = userVec.at(i);
			if (qData->getSeatId() == QGameManager::getInstance()->getSeatId())
			{
				if (qData->getReady() == 1)
				{
					if (m_readyLayout)
						m_readyLayout->setVisible(true);
					auto statSprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName("Sprite_Starting"));
					if (statSprite) statSprite->setVisible(true);
					char dzReady[32] = {0};
					int index = (qData->getSeatId() - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
					sprintf(dzReady,"Sprite_Ready_%d",index);
					auto reayHead = dynamic_cast<Sprite*>(m_readyLayout->getChildByName(dzReady));
					if (reayHead) reayHead->setVisible(true);
					auto readyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Ready"));
					if(readyButton) readyButton->setVisible(false);
					if (m_gameNum > 0)
					{
						auto friendButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Friend"));
						if (friendButton) friendButton->setVisible(false);
						auto copyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Copy"));
						if (copyButton) copyButton->setVisible(false);
					}
				}
				else
				{
					if (m_readyLayout)
						m_readyLayout->setVisible(true);
					if (m_gameNum > 0)
					{
						auto friendButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Friend"));
						if (friendButton) friendButton->setVisible(false);
						auto copyButton = dynamic_cast<Button*>(m_readyLayout->getChildByName("Button_Copy"));
						if (copyButton) copyButton->setVisible(false);
					}
				}
			}
			else
			{
				if (qData->getReady() == 1)
				{
					int iCount = GAMECONFIG.m_max_num;
					int index = ((int)qData->getSeatId() - (int)(QGameManager::getInstance()->getSeatId()) + iCount) % iCount;
					if (m_player[index] == nullptr)
						return;
					char szName[100] = { 0 };
					sprintf(szName, "Sprite_Ready_%d", index);
					auto readySprite = dynamic_cast<Sprite*>(m_readyLayout->getChildByName(szName));
					if (readySprite) readySprite->setVisible(true);
				}
			}
		}
		break;
	}
	case QGameLayer::GS_SEND_POKER:
	{
		m_pIsSendPocke = true;
		break;
	}
	case QGameLayer::GS_LOOT_DEAL:
	{
		if (QGameManager::getInstance()->getReady() == 0)
		{
			m_grapLayout->setVisible(false);
			showOtherBackPoker();
		}
		else{
			m_grapLayout->setVisible(true);
		}
		break;
	}
	case QGameLayer::GS_RAND_DEAL:
	{
		if (m_grapLayout)
			m_grapLayout->setVisible(false);
		break;
	}
	case QGameLayer::GS_PLAYER_BET:
	{
        showMyHand();
		showMyPaiAgain();
		if (QHallManager::getInstance()->getZMode() == 1)
		{
			if (m_readyLayout)
				m_readyLayout->setVisible(false);
		}
		if (QHallManager::getInstance()->getZMode() == 3)
		{
			m_chipLayout->setVisible(false);
		}
		else
			m_chipLayout->setVisible(true);
		if (QGameManager::getInstance()->getReady() == 0)
		{
			if (m_chipLayout)
				m_chipLayout->setVisible(false);
		}
		break;
	}
	case QGameLayer::GS_OPEN_POKER:
	{
		showMyPaiAgain();
		if (m_chipLayout)
			m_chipLayout->setVisible(false);
		if (m_countLayout)
		{
			m_countLayout->setVisible(true);
			auto fanButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_Fan"));
			if (fanButton)
			{
				fanButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchCountEvent, this));
			}
			//自动算牌
			auto zdspButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_ZDSN"));
			if (zdspButton)
			{
				zdspButton->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchFanCardEvent, this));
			}
		}
		if (QGameManager::getInstance()->getReady() == 0)
		{
			if (m_countLayout)
				m_countLayout->setVisible(false);
		}

		for (int i = 0; i < m_imageview.size(); i++)
		{
			m_imageview.at(i)->addTouchEventListener(CC_CALLBACK_2(QGameLayer::touchPocketEvent, this));
			m_imageview.at(i)->setTouchEnabled(true);
			m_imageview.at(i)->setTag(100 + i);
			m_imageview.at(i)->setVisible(true);
			m_imageview.at(i)->removeBackGroundImage();
		}
		break;
	}
	case QGameLayer::GS_GAME_RET:
	{
		if (m_countLayout)
			m_countLayout->setVisible(false);
		if (!m_imageview.empty())
		{
			for (int i = 0; i < m_imageview.size(); i++)
			{
				m_imageview.at(i)->setTouchEnabled(false);
			}
		}
		MsgGameRequest::sendOpenPoker();
		this->ResetLayer();
		m_bIsGameBack = true;
		break;
	}
	default:
		break;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (QHALLMGR->getBackType() == 1)
	{	//弹窗
		PopLayer* popup = PopLayer::create();
		if (popup)
		{
			popup->setContentText(GetGameString("HYHLAI"), 30, 800, 500);
			popup->setPosition(0, -visibleSize.height / 5);
			this->addChild(popup, 100);
		}

		QHALLMGR->setBackType(0);
	}
}
void QGameLayer::touchCuoCardEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	m_lookPocker = LookPocker::create();
	this->addChild(m_lookPocker, 1);
	m_lookPocker->setParentLayer(this);
}
int QGameLayer::GetValue(int u08Card)
{
    int u08Value = u08Card;
    if ( u08Value > 10 )
        u08Value = 10;
    return u08Value;
}
int QGameLayer::GetValue(vector<int>& vecCard)
{
    if ( vecCard.size()!=5 )
        return -1;
    
    for ( int i=0;i<5;i++ )
    {
        for ( int j=i+1;j<5;j++ )
        {
            for ( int k=j+1;k<5;k++ )
            {
                int u08Value = GetValue(vecCard[i])+GetValue(vecCard[j])+GetValue(vecCard[k]);
                if ( u08Value%10 == 0 )
                {
                    int u08Ret = GetValue(vecCard[0])+GetValue(vecCard[1])+GetValue(vecCard[2])
                    +GetValue(vecCard[3])+GetValue(vecCard[4]);
                    return u08Ret%10;
                }
            }
        }
    }
    return -1;
}

int QGameLayer::GetCow(vector<int>& vecCard)
{
    int result = PC_NULL ;
    
    if ( vecCard.size()!=5 )
        return 0;
    
    if ( IsZhaDan(vecCard) )
        return PC_ZHADAN;
    else if ( IsWuXiao(vecCard) )
        return PC_WUXIAO;
    else if ( IsWuHua(vecCard) )
        return PC_WUHUA;
    else
    {
        int u08Temp = GetValue(vecCard);
        if ( u08Temp==0 )
            return PC_COW;
        else if ( u08Temp== -1 )
            return PC_NULL;
        else
            return u08Temp;
    }
    return result;
}

bool QGameLayer::IsZhaDan(vector<int>& vecCard)
{
    if ( vecCard.size()!=5 )
        return false;
    
    int arrTemp[14] = {0};
    for ( int i=0;i<5;i++ )
    {
        int u08Temp = vecCard[i];
        if ( u08Temp>13 )
            return false;
        arrTemp[u08Temp]++;
    }
    for ( int i=1;i<=14;i++ )
    {
        if ( arrTemp[i]==4 )
            return true;
    }
    return false;
}

bool QGameLayer::IsWuXiao(vector<int>& vecCard)
{
    if ( vecCard.size()!=5 )
        return false;
    
    int u08Temp = GetValue(vecCard[0])+GetValue(vecCard[1])+GetValue(vecCard[2])+GetValue(vecCard[3])+GetValue(vecCard[4]);
    if ( u08Temp<=10 )
        return true;
    return false;
}

bool QGameLayer::IsWuHua(vector<int>& vecCard)
{
    if ( vecCard.size()!=5 )
        return false;
    
    for ( int i=0;i<5;i++ )
    {
        int u08Temp = vecCard[i];
        if ( u08Temp <= 10 )
            return false;
    }
    return true;
}

void QGameLayer::setSpeekTime()
{
    m_bCanSendSpeek = false;
    gettimeofday(&m_tvSpeekTime, NULL);
}

bool QGameLayer::isCanSpeek()
{
    if (m_bCanSendSpeek) return true;
    else
    {
        struct timeval	tvEnd;
        gettimeofday(&tvEnd, NULL);
        int gapTime = tvEnd.tv_sec * 1000L + tvEnd.tv_usec / 1000L - m_tvSpeekTime.tv_sec * 1000L - m_tvSpeekTime.tv_usec / 1000L;
        //5秒写在配置里面
        if (gapTime >= 5000)
        {
            m_bCanSendSpeek = true;
            return true;
        }
        auto notSpeek = ScriptDataManager::getInterface()->getGameString("notSpeek");
        //弹窗
        PopLayer* popup = PopLayer::create();
        popup->setContentText(notSpeek.c_str(), 30, 800, 500);
        Director::getInstance()->getRunningScene()->addChild(popup);
        //提示时间太短
        return false;
    }
}

void QGameLayer::showLastCard()
{
	//if (m_pocketVec.size() != 0)
	//{
	//	if (QHallManager::getInstance()->getKcardWay() == 5)
	//	{
	//		for (int i = 0; i < m_pocketVec.size(); i++)
	//		{
	//			m_pocketVec.at(i)->setisBack(false);
	//		}
	//	}
	//	else if (QHallManager::getInstance()->getKcardWay() == 2)
	//	{
	//		m_pocketVec.at(3)->setisBack(false);
	//		m_pocketVec.at(4)->setisBack(false);
	//	}
	//	else
	//		m_pocketVec.at(4)->setisBack(false);
	//}
	if (this->isHaveCattle())
	{
		PromptCow();
	}
	MsgGameRequest::sendOpenPoker();
	m_countLayout->setVisible(false);
	auto cuoButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_Cuo"));
	if (cuoButton) cuoButton->setVisible(false);
	auto fanButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_Fan"));
	if (fanButton) fanButton->setVisible(false);
	auto openButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_Open"));
	if (openButton) openButton->setVisible(true);
	auto tsButton = dynamic_cast<Button*>(m_countLayout->getChildByName("Button_TS"));
	if (tsButton) tsButton->setVisible(true);
	auto imageBg = dynamic_cast<ImageView*>(m_countLayout->getChildByName("Image_Bg"));
	if (imageBg) imageBg->setVisible(true);
}

void QGameLayer::BetChip(int score, Layout* headPos[], int index)
{
	auto chipNum = score / 5;
	for (int i = 0; i < chipNum; i++)
	{
		auto betSprite = Sprite::create("GameScene/chip.png");
		betSprite->setScale(1.4f);
		betSprite->setPosition(headPos[index]->getPosition());
		betSprite->runAction(ChipMove());
		m_vecChip.push_back(betSprite);
		this->addChild(betSprite);	
	}
}

cocos2d::Action* QGameLayer::ChipMove()
{
	Size size = Director::getInstance()->getVisibleSize();
	float x = size.width / 2;
	float y = size.height * 1 / 2 + 20;
	float moveTime = 0.3f;
	float rand_x = CCRANDOM_0_1()*(x + 60 - (x - 60) + 1) + (x - 60);  //产生一个从start到end间的随机数 
	float rand_y = CCRANDOM_0_1()*(y + 35 - (y - 35) + 1) + (y - 35);  //产生一个从start到end间的随机数 
	MoveTo* moveTo = MoveTo::create(moveTime, Vec2(rand_x, rand_y));
	return moveTo;
}

void QGameLayer::ChipToWinner(int seatId)
{
	GameAudioEngine::getInstance()->playEffect("sound/g_addchip.mp3");
	auto iBegin = m_vecChip.begin();
	auto iEnd = m_vecChip.end();
	int index = (seatId - (int)(QGameManager::getInstance()->getSeatId()) + GAMECONFIG.m_max_num) % GAMECONFIG.m_max_num;
	while (iBegin != iEnd)
	{
		auto moveTo = MoveTo::create(0.5f, Vec2(m_HeadPos[index]->getPositionX(), m_HeadPos[index]->getPositionY()));
		(*iBegin)->runAction(Sequence::create(moveTo, RemoveSelf::create(), nullptr));
		iBegin++;
	}
	m_vecChip.clear();
}

void QGameLayer::PlayWinAction()
{
	auto sizeWin = Director::getInstance()->getWinSize();
	auto animation = Animation::create();
	for (int i = 1; i < 7; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "GameLose/win/1-%d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	// 控制帧动画的每帧间的播放间隔
	animation->setDelayPerUnit(0.2f);
	// 是否在播放后重置成第一张图片
	animation->setRestoreOriginalFrame(false);
	auto action_Win = Animate::create(animation);

	auto sprite = Sprite::create();
	sprite->setTexture("GameLose/win/1-1.png");
	sprite->setPosition(Vec2(sizeWin.width / 2, sizeWin.height / 2));
	addChild(sprite);
	sprite->runAction(Sequence::create(action_Win, RemoveSelf::create(), NULL));
}

void QGameLayer::CloseDismissLayer()
{
	if (m_dismiss)
	{
		m_dismiss->removeFromParentAndCleanup(true);
		m_dismiss = nullptr;
	}
}
