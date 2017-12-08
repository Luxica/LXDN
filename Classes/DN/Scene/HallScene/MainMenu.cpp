#include "MainMenu.h"
#include "cocostudio/CocoStudio.h"
#include "../../../GUISystem/WWSceneManager.h"
#include "layer/BackLayer.h"
#include "layer/AddRoom.h"
#include "layer/CreateRoom.h"
#include "layer/NewsLayer.h"
#include "layer/Circular.h"
#include "layer/PlayIntroLayer.h"
#include "layer/SetLayer.h"
#include "layer/ShopLayer.h"
#include "layer/SortScene.h"
#include "layer/Sociaty.h"
#include "layer/SussexLayer.h"
#include "layer/RebateLayer.h"
#include "layer/ProxyLayer.h"
#include "layer/InviteCode.h"
#include "layer/Information.h"
#include "layer/InsteadRecord.h"
#include "layer/FeedBack.h"
#include "../GlobalLayer/DialogLayer.h"
#include "../GlobalLayer/Prompt.h"
#include "../../Scene/GlobalLayer/Load.h"
#include "../../../StringTool.h"
#include "../../Scene/HallScene/data/QHallManager.h"
#include "../QDNEvent.h"
#include "../../../DN/Comm/request/MsgHallRequest.h"
#include "SimpleAudioEngine.h"
#include "WxSocial.h"
#include "ScriptDataManager.h"
#include "GameConfig.h"
#include "ProceConfig.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "../../../net/LordNetManger.h"
#include "../GlobalLayer/DialogLayer.h"
#include "layer/ShareLayer.h"
#include "../../../DN/GlobalDefines.h"
#include "../../Scene/GlobalLayer/PopLayer.h"
#include "../GameScene/QGameScene.h"
#include "ScriptDataManager.h"
#include "../../Comm/request/MsgGameRequest.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSAdapterUtility.hpp"
#endif
#include "AdmobJni.h"
#include "../LoginScene/layer/LoginLoad.h"
#include "../../Scene/GameScene/data/QGameManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "WebSprite.h"
#else
#include "GameNode/WebSprite.h"
#endif

using namespace std;
using namespace cocos2d::ui;

using namespace cocos2d;
using namespace cocostudio::timeline;

// on "init" you need to initialize your instance
bool MainMenu::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_IsUpdate = true;
	m_bIsAction = false;
	m_hint = nullptr;
	m_menuLayer = nullptr;
	menueCheck = nullptr;

	m_FeedBack = nullptr;
	auto mainLayer = CSLoader::createNode("mainLayer.csb");
	addChild(mainLayer);

	m_image_VIP = dynamic_cast<ImageView*>(mainLayer->getChildByName("Image_VIP"));
	if (m_image_VIP)
	{
		showVIP();
	}

	//显示头像
	auto m_userHead = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_head"));
	if (m_userHead)
	{
		m_userHead->removeAllChildren();
		auto pPortrait = WebSprite::create(QHallManager::getInstance()->getHeadUrl(), Size(148.0f, 158.0f));
		if (pPortrait)
		{
			pPortrait->setLocalZOrder(-1);
			m_userHead->addChild(pPortrait);
			pPortrait->setPosition(m_userHead->getContentSize().width / 2, m_userHead->getContentSize().height / 2);
		}
	}
	//显示昵称
	auto name_layout = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Name"));
	if (name_layout)
	{
		m_userName = dynamic_cast<Text*>(name_layout->getChildByName("playerName"));
		m_userName->setString(QHallManager::getInstance()->getNickName());
	}
	//显示UID
	auto m_userUID = dynamic_cast<Text*>(mainLayer->getChildByName("Text_UID"));
	if (m_userUID)
	{
		char uId[32] = { 0 };
		sprintf(uId, GetGameString("show_UID"), QHallManager::getInstance()->getUserId());
		m_userUID->setString(uId);
	}

	//显示房卡数
	m_userCardNum = dynamic_cast<Text*>(mainLayer->getChildByName("cardNum"));
	if (m_userCardNum)
	{
		m_userCardNum->setString(StringTool::intToString(QHallManager::getInstance()->getRoomCard()));
	}
	auto headButton = dynamic_cast<Button*>(mainLayer->getChildByName("Button_head"));
	if (headButton)
	{
		headButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchHeadEvent, this));
	}

	//返回按钮
	auto backButton = dynamic_cast<Button*>(mainLayer->getChildByName("backButton"));
	if (backButton)
	{
		backButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchBackEvent, this));
	}
	//冲卡按钮
	auto rechargeButton = dynamic_cast<Button*>(mainLayer->getChildByName("rechargeButton"));
	if (rechargeButton)
	{
		rechargeButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchRechargeEvent, this));
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			auto animation = Animation::create();
			for (int i = 1; i < 4; i++)
			{
				char szName[100] = { 0 };
				sprintf(szName, "MainLayer/LightEffect_2/2-%d.png", i);
				animation->addSpriteFrameWithFile(szName);
			}
			// 控制帧动画的每帧间的播放间隔
			animation->setDelayPerUnit(0.5f);
			// 是否在播放后重置成第一张图片
			animation->setRestoreOriginalFrame(false);
			auto action_AddCard = Animate::create(animation);

			auto sprite = Sprite::create();
			sprite->setTexture("MainLayer\LightEffect_2/2-1.png");
			sprite->setPosition(rechargeButton->getPosition());
			addChild(sprite);
			sprite->runAction(RepeatForever::create(action_AddCard));
		}
	}
	//商城按钮
	auto shopButton = dynamic_cast<Button*>(mainLayer->getChildByName("shopButton"));
	if (shopButton)
	{
		shopButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchRechargeEvent, this));
	}
	//排行榜按钮
	auto sortButton = dynamic_cast<Button*>(mainLayer->getChildByName("sortButton"));
	if (sortButton)
	{
		sortButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchSortEvent, this));
	}
	//公会按钮
	auto sociatyButton = dynamic_cast<Button*>(mainLayer->getChildByName("sociatyButton"));
	if (sociatyButton)
	{
		sociatyButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchSociatyEvent, this));
	}
	//代开房按钮
	auto insteadButton = dynamic_cast<Button*>(mainLayer->getChildByName("insteadButton"));
	if (insteadButton)
	{
		insteadButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchInsteadEvent, this));
	}
	//新闻按钮
	auto newsButton = dynamic_cast<Button*>(mainLayer->getChildByName("newsButton"));
	if (newsButton)
	{
		newsButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchNewsEvent, this));
	}
	//战绩按钮
	auto sussexButton = dynamic_cast<Button*>(mainLayer->getChildByName("markButton"));
	if (sussexButton)
	{
		sussexButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchMarkEvent, this));
	}
	//设置按钮
	auto setButton = dynamic_cast<Button*>(mainLayer->getChildByName("setButton"));
	if (setButton)
	{
		setButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchSetEvent, this));
	}
	//玩法按钮
	auto introButton = dynamic_cast<Button*>(mainLayer->getChildByName("playIntroButton"));
	if (introButton)
	{
		introButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchIntroEvent, this));
	}
	//客服按钮
	auto customerButton = dynamic_cast<Button*>(mainLayer->getChildByName("kfButton"));
	if (customerButton)
	{
		customerButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchCSEvent, this));
	}
	auto extendButton = dynamic_cast<Button*>(mainLayer->getChildByName("extendButton"));
	if (extendButton)
	{
		extendButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchExtendEvent, this));
	}
	//分享按钮
	auto shareButton = dynamic_cast<Button*>(mainLayer->getChildByName("shareButton"));
	if (shareButton)
	{
		shareButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchShareEvent, this));
	}
	menueCheck = dynamic_cast<CheckBox*>(mainLayer->getChildByName("CheckBox_Menu"));
	if (menueCheck)
	{
		menueCheck->addTouchEventListener(CC_CALLBACK_2(MainMenu::selectedMenuEvent, this));
	}
	m_menuLayer = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_Menue"));
	if (m_menuLayer)
	{
		m_menuLayer->setVisible(false);
		auto introButton = dynamic_cast<Button*>(m_menuLayer->getChildByName("playIntroButton"));
		if (introButton)
		{
			introButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchIntroEvent, this));
		}
		auto setButton = dynamic_cast<Button*>(m_menuLayer->getChildByName("setButton"));
		if (setButton)
		{
			setButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchSetEvent, this));
		}
		auto backButton = dynamic_cast<Button*>(m_menuLayer->getChildByName("backButton"));
		if (backButton)
		{
			backButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchBackEvent, this));
		}
	}
	//邀请按钮
	m_inviteButton = dynamic_cast<Button*>(mainLayer->getChildByName("inviteButton"));
	if (m_inviteButton)
	{
		auto userType = QHALLMGR->getUserType();
		if (userType == 0)
		{
			m_inviteButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchInviteCodeEvent, this));
		}
		else
		{
			m_inviteButton->setVisible(false);
		}
	}
	//创建房间按钮
	auto createRoomButton = dynamic_cast<Button*>(mainLayer->getChildByName("creatRoomButton"));
	if (createRoomButton)
	{
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			auto animation = Animation::create();
			for (int i = 1; i < 9; i++)
			{
				char szName[100] = { 0 };
				sprintf(szName, "MainLayer/LightEffect_1/1-%d.png", i);
				animation->addSpriteFrameWithFile(szName);
			}
			// 控制帧动画的每帧间的播放间隔
			animation->setDelayPerUnit(0.2f);
			// 是否在播放后重置成第一张图片
			animation->setRestoreOriginalFrame(false);
			auto action_Create = Animate::create(animation);

			auto sprite = Sprite::create();
			sprite->setTexture("MainLayer/LightEffect_1/1-1.png");
			sprite->setPosition(createRoomButton->getPosition());
			addChild(sprite);
			sprite->runAction(RepeatForever::create(action_Create));
		}
		createRoomButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchCreateRoomEvent, this));
	}

	//加入房间按钮
	auto addRoomButton = dynamic_cast<Button*>(mainLayer->getChildByName("addRoomButton"));
	if (addRoomButton)
	{
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			auto animation = Animation::create();
			for (int i = 1; i < 9; i++)
			{
				char szName[100] = { 0 };
				sprintf(szName, "MainLayer/LightEffect_1/1-%d.png", i);
				animation->addSpriteFrameWithFile(szName);
			}
			// 控制帧动画的每帧间的播放间隔
			animation->setDelayPerUnit(0.2f);
			// 是否在播放后重置成第一张图片
			animation->setRestoreOriginalFrame(false);
			auto action_Add = Animate::create(animation);

			auto sprite = Sprite::create();
			sprite->setTexture("MainLayer\LightEffect_1/1-1.png");
			sprite->setPosition(addRoomButton->getPosition());
			addChild(sprite);
			sprite->runAction(RepeatForever::create(action_Add));
		}
		addRoomButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchAddRoomEvent, this));
	}

	//扎金花链接按钮
	auto zjhButton = dynamic_cast<Button*>(mainLayer->getChildByName("zjhButton"));
	if (zjhButton)
	{
		zjhButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchZJHEvent, this));
	}
	//钻石房按钮
	auto diamondRoomButton = dynamic_cast<Button*>(mainLayer->getChildByName("diamondRoomButton"));
	if (diamondRoomButton)
	{
		diamondRoomButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchDiamondRoomEvent, this));
		diamondRoomButton->setTag(1);
	}
	//彩钻房按钮
	auto colorDiamondRoomButton = dynamic_cast<Button*>(mainLayer->getChildByName("colorDiamondRoomButton"));
	if (colorDiamondRoomButton)
	{
		colorDiamondRoomButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchDiamondRoomEvent, this));
		colorDiamondRoomButton->setTag(2);
	}
	//返利按钮
	auto rebateButton = dynamic_cast<Button*>(mainLayer->getChildByName("rebateButton"));
	if (rebateButton)
	{
		rebateButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchRebateEvent, this));
	}
	//代理按钮
	auto proxyButton = dynamic_cast<Button*>(mainLayer->getChildByName("proxyButton"));
	if (proxyButton)
	{
		proxyButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchProxyEvent, this));
	}
	//麻将链接按钮
	auto mjButton = dynamic_cast<Button*>(mainLayer->getChildByName("MJButton"));
	if (mjButton)
	{
		mjButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchLinkButtonEvent, this));
		mjButton->setTag(1);
	}
	//扑克链接按钮
	auto pkButton = dynamic_cast<Button*>(mainLayer->getChildByName("PKButton"));
	if (pkButton)
	{
		pkButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchLinkButtonEvent, this));
		pkButton->setTag(2);
	}
	//跑马灯
	m_scrollScrn = dynamic_cast<Layout*>(mainLayer->getChildByName("Panel_ScrollScrn"));
	if (m_scrollScrn)
	{
		m_TextScroll = dynamic_cast<Text*>(m_scrollScrn->getChildByName("Text_Scrn"));
		MsgHallRequest::sendGetNotice();
	}
	//充值按钮
	auto addButton = dynamic_cast<Button*>(mainLayer->getChildByName("Button_Add"));
	if (addButton)
	{
		addButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchAddCardEvent, this));
	}
	//反馈按钮
	auto feedbackBtn = dynamic_cast<Button*>(mainLayer->getChildByName("feedbackButton"));
	if (feedbackBtn)
	{
		feedbackBtn->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchFeedBackEvent, this));
	}

	auto expandBtn = dynamic_cast<Button*>(mainLayer->getChildByName("expandButton"));
	if (expandBtn)
	{
		expandBtn->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchExpandEvent, this));
	}

	//分享提示
	m_hint = dynamic_cast<ImageView*>(mainLayer->getChildByName("Image_Hint"));
	if (m_hint)
	{
		MsgHallRequest::sendCanShare();
	}

	this->initListener();

    auto userDefault = UserDefault::getInstance();
    float music = userDefault->getFloatForKey(MUSIC_VALUE_TEXT, 0.5);
	float sound = userDefault->getFloatForKey(SOUND_VALUE_TEXT, 0.5);
    
	int userType = QHALLMGR->getUserType();
	//bool isShow = false;
	log("isFirst %d", QHALLMGR->getIsFirst());
	//判断登陆是否弹出通知框和邀请码界面
	if (QHALLMGR->getIsFirst())
	{
		if (PROCECONFIG.m_bShowNews)
		{
			this->addChild(NewsLayer::create());
		}
		if (PROCECONFIG.m_bShowInviteCode && userType == 0)
		{
			onShowInviteDlg();
		}
	}
	//当前在线人数
	m_textNum = dynamic_cast<Text*>(mainLayer->getChildByName("Text_Num"));
	updateOnlineNum(QHALLMGR->getCurNum());

	m_Wife = dynamic_cast<ImageView*>(mainLayer->getChildByName("Image_Wife"));
	m_Power = dynamic_cast<LoadingBar*>(mainLayer->getChildByName("LoadingBar_Power"));
	
    
    // 播放背景音乐
	GameAudioEngine::getInstance()->setSwitchOfMusic(true);
	GameAudioEngine::getInstance()->setSwitcjofEffect(true);

	GameAudioEngine::getInstance()->setBackgroundMusicVolume(music);
	GameAudioEngine::getInstance()->setEffectsVolume(sound);
   
    GameAudioEngine::getInstance()->playBackgroundMusic(GAME_HALL_BACKGROUND_MUSIC, true);

	this->scheduleUpdate();
	updateWife(0);
	schedule(CC_SCHEDULE_SELECTOR(MainMenu::updateWife), 5.0f);
	
	auto pListener = EventListenerCustom::create("EVENT_GOTO_ROOM", [](EventCustom* event) {
		std::string roomId = QGameManager::getInstance()->getGotoRoomId();
		int roomNumSend = StringTool::stringToint(roomId);
		if (roomId.size() > 0 && roomNumSend > 0) {
			WWSceneManager::getInstance()->openGlobalUI(Load::create());

			QHallManager::getInstance()->setRoomNum(roomNumSend);
			QHallManager::getInstance()->setJoinType(1);

			MsgHallRequest::sendEnterRoom(roomNumSend & 0x00FFFFFF);

			QGameManager::getInstance()->setGotoRoomId("0");
		}
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	std::string roomId = QGameManager::getInstance()->getGotoRoomId();
	int roomNumSend = StringTool::stringToint(roomId);
	if (roomId.size() > 0 && roomNumSend > 0) {
		WWSceneManager::getInstance()->openGlobalUI(Load::create());

		QHallManager::getInstance()->setRoomNum(roomNumSend);
		QHallManager::getInstance()->setJoinType(1);

		MsgHallRequest::sendEnterRoom(roomNumSend & 0x00FFFFFF);

		QGameManager::getInstance()->setGotoRoomId("0");
	} else { 
		QHallManager::getInstance()->setIsFirst(false);
	}

	//定位
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	iOSAdapterUtility::getInstance()->onStartLocation(CC_CALLBACK_3(MainMenu::LocationCallBack, this));
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	AdmobJni::onStartLocation(CC_CALLBACK_3(MainMenu::LocationCallBack, this));
#endif
	return true;
}

void MainMenu::initListener()
{
	auto pListener = EventListenerCustom::create(EVENT_RECORD, CC_CALLBACK_1(MainMenu::updateRecord, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_RANK, CC_CALLBACK_1(MainMenu::updateRank, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_NOTICE, CC_CALLBACK_1(MainMenu::updateNotice, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_INSTEAD, CC_CALLBACK_1(MainMenu::updateInstead, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_TAKE_ON, CC_CALLBACK_1(MainMenu::updateTakeOn, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_TAKE_OFF, CC_CALLBACK_1(MainMenu::updateTakeOff, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_ROOM_KICK, CC_CALLBACK_1(MainMenu::updateRoomKick, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	pListener = EventListenerCustom::create(EVENT_FEEDBACK, CC_CALLBACK_1(MainMenu::updateFeedBack, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

}

void MainMenu::setHallRoomCard()
{
	if (m_userCardNum)
	{
		m_userCardNum->setString(StringTool::intToString(QHallManager::getInstance()->getRoomCard()));
	}
}

void MainMenu::setInviteShow()
{
	if (m_inviteButton)
	{
		m_inviteButton->setVisible(false);
	}
}

void MainMenu::setHint(bool isShow)
{
	if (m_hint)
	{
		m_hint->setVisible(isShow);
	}
}
void MainMenu::update(float dt)
{
	if (m_bIsAction) {
		auto iBegin = m_noticeVec.begin();
		if (iBegin != m_noticeVec.end()) {
			iBegin = m_noticeVec.erase(iBegin);
		}
		MsgHallRequest::sendGetNotice();
		m_bIsAction = false;
	}
	
}
void MainMenu::updateCanShare(float dt)
{
	m_IsUpdate = false;
	MsgHallRequest::sendCanShare();
	
	struct tm *ptr;
	time_t t;
	time(&t);
	ptr = localtime(&t);
	int m = ptr->tm_min;
	int h = ptr->tm_hour;
	if (h != 0 ||h!= 12 || m >= 1)
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(MainMenu::updateCanShare));
		m_IsUpdate = true;
	}
}
void MainMenu::updateAction()
{
	if (m_TextScroll)
	{
		auto action = MoveTo::create(20.0f,
			Vec2(m_scrollScrn->getPositionX() - m_scrollScrn->getContentSize().width / 2 - m_stringLenth, 0));
		auto action1 = Sequence::create(Place::create(m_TextScroll->getPosition()), action, CallFunc::create(
			[&]{
			m_bIsAction = true;
		}), nullptr);
		m_TextScroll->runAction(RepeatForever::create(action1));
	}
}

void MainMenu::showVIP()
{
    if (m_image_VIP)
    {
        switch (QHALLMGR->getVIP())
        {
            case 0:
				m_image_VIP->setVisible(false);
				break;
            case 1:
				m_image_VIP->loadTexture("MainLayer/Diamond_1.png");
				break;
            case 2:
				m_image_VIP->loadTexture("MainLayer/Diamond_2.png");
				break;
            default:
				break;
        }
    }
}

void MainMenu::updateWife(float dt)
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
	log("net level : %d", level);

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

void MainMenu::updateNotice(EventCustom* event)
{
	if (m_noticeVec.empty())
		m_noticeVec = QHallManager::getInstance()->getNoticeVec();
	if (m_TextScroll&&!m_noticeVec.empty())
	{
		auto iBegin = m_noticeVec.begin();
		char noticStr[256] = { 0 };
		sprintf(noticStr, GetGameString("game_public"), (*iBegin).c_str());
		m_TextScroll->setString(noticStr);
		m_TextScroll->stopAllActions();
		m_stringLenth = m_TextScroll->getContentSize().width;
		updateAction();
	}
}

void MainMenu::updateOnlineNum(unsigned int num)
{
	if (m_textNum)
	{
		char numArr[255] = { 0 };
		sprintf(numArr, GetGameString("curNum"), QHALLMGR->getCurNum());
		m_textNum->setString(numArr);
	}
}

void MainMenu::updateRank(EventCustom* event)
{
	if (m_Rank)
	{
		m_Rank->updateRank();
	}
}

void MainMenu::updateInstead(EventCustom* event)
{
	if (m_Instead)
	{
		m_Instead->updateInstead();
	}
}

void MainMenu::updateTakeOn(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long iRoomNum = reinterpret_cast<long>(event->getUserData());
#else
	int iRoomNum = reinterpret_cast<int>(event->getUserData());
#endif
	if (iRoomNum)
	{
		log("InsteadSucces:---RoomNum----%d", iRoomNum);
		QHALLMGR->setInsteadRoomNum(iRoomNum);
		this->removeChildByName("createRoom", true);
		WWSceneManager::getInstance()->closeGlobalUIByName("prompt");
		auto cPlayMode = QHALLMGR->getPlayMode();
		auto cMMode = QHALLMGR->getMMode();
		auto cZMode = QHALLMGR->getZMode();
		char tsName[100] = {};
		char mMode[50] = {};
		char zMode[50] = {};
		sprintf(mMode, "game_add_%d", cMMode);
		sprintf(zMode, "GameMode%d", cZMode);
		sprintf(tsName, GetGameString("insteadRoomInfo"), iRoomNum, cPlayMode, GetGameString(mMode), GetGameString(zMode));
		auto promptLayer = Prompt::create(1, tsName);
		auto viewButton = dynamic_cast<Button*>(promptLayer->m_systemLayout->getChildByName("Button_View"));
		if (viewButton)
		{
			viewButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchInsteadEvent, this));
		}
		auto copyButton = dynamic_cast<Button*>(promptLayer->m_systemLayout->getChildByName("Button_Copy"));
		if (copyButton)
		{
			copyButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchCopyEvent, this));
		}
		auto inviteButton = dynamic_cast<Button*>(promptLayer->m_systemLayout->getChildByName("Button_Invite"));
		if (inviteButton)
		{
			inviteButton->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchInviteEvent, this));
		}
		WWSceneManager::getInstance()->openGlobalUI(promptLayer);
		LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
	}
	else
	{
		log("InsteadFail:---RoomNum----%d", iRoomNum);
		WWSceneManager::getInstance()->closeGlobalUIByName("prompt");
		auto promptLayer = Prompt::create(2, GetGameString("insteadFail"));
		WWSceneManager::getInstance()->openGlobalUI(promptLayer);
		LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
	}
}

void MainMenu::updateTakeOff(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long errorCode = reinterpret_cast<long>(event->getUserData());
#else
	int errorCode = reinterpret_cast<int>(event->getUserData());
#endif
	log("errorCode:----------%d", errorCode);
	LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
	if (m_Instead)
	{
		m_Instead->removeFromParentAndCleanup(true);
		m_Instead = nullptr;
	}
	MsgHallRequest::sendGetTake(1, 0);
	if (!m_Instead)
	{
		m_Instead = InsteadRecord::create();
		if (m_Instead)
		{
			this->addChild(m_Instead);
		}
	}
}

void MainMenu::updateRoomKick(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long errorCode = reinterpret_cast<long>(event->getUserData());
#else
	int errorCode = reinterpret_cast<int>(event->getUserData());
#endif
	log("errorCode:----------%d", errorCode);
	LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
	m_Instead->removeFromParentAndCleanup(true);
	m_Instead = nullptr;
	MsgHallRequest::sendGetTake(1, 0);
	if (!m_Instead)
	{
		m_Instead = InsteadRecord::create();
		if (m_Instead)
		{
			this->addChild(m_Instead);
		}
	}
}

void MainMenu::updateFeedBack(EventCustom* event)
{
	#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	long errorCode = reinterpret_cast<long>(event->getUserData());
#else
	int errorCode = reinterpret_cast<int>(event->getUserData());
#endif
	log("errorCode:----------%d", errorCode);
	if (m_FeedBack)
	{
		m_FeedBack->updateFeedBack(errorCode);
	}
}

void MainMenu::updateRecord(EventCustom* event)
{
	if (m_Record)
	{
		m_Record->updateRecord();
	}
}

void MainMenu::touchHeadEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->addChild(Information::create());
}

void MainMenu::touchBackEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	if (m_menuLayer)
	{
		m_menuLayer->setVisible(false);
		menueCheck->setSelected(false);
	}
	WWSceneManager::getInstance()->openGlobalUI(DialogLayer::create(2,GetGameString("exit_login")));
}

void MainMenu::touchRechargeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	int userType = QHALLMGR->getUserType();
	if (PROCECONFIG.m_bHaveShop)
	{
		if (userType == 0)
		{
			auto inviteCodeLayer = InviteCode::create();
			if (inviteCodeLayer)
			{
				inviteCodeLayer->setName("inviteCode");
				this->addChild(inviteCodeLayer);
			}
		}
		else if (userType == 2)
		{
			this->addChild(ShopLayer::create());
		}
	}
	else
	{
		m_shopingLayer = DialogLayer::create(1, GetGameString("buy_card"));
		if (m_shopingLayer)
		{
			this->addChild(m_shopingLayer);
			auto text = dynamic_cast<Text*>(m_shopingLayer->m_systemLayout->getChildByName("Text"));
			if (text)
			{
				text->ignoreContentAdaptWithSize(false);
				text->setSize(Size(300, 90));
				text->setFontSize(30.0f);
			}
			auto button = dynamic_cast<Button*>(m_shopingLayer->m_systemLayout->getChildByName("Button_Ture_0"));
			if (button)
			{
				button->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchLoginShopEvent, this));
			}
		}
		
	}

}

void MainMenu::touchSortEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	m_Rank = SortScene::create();
	if (m_Rank)
	{
		this->addChild(m_Rank);
	}
	MsgHallRequest::sendGetRank(0);

}

void MainMenu::touchSociatyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->addChild(Sociaty::create());
}

void MainMenu::touchInsteadEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WWSceneManager::getInstance()->closeGlobalUIByName("prompt");
	auto createLayer = this->getChildByName("createRoom");
	if (createLayer)
	{
		createLayer->removeFromParentAndCleanup(true);
	}
	m_Instead = InsteadRecord::create();
	if (m_Instead)
	{
		this->addChild(m_Instead);
	}
	MsgHallRequest::sendGetTake(1, 0);
}

void MainMenu::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WWSceneManager::getInstance()->closeGlobalUIByName("prompt");
	this->removeChildByName("createRoom", true);
}

void MainMenu::touchNewsEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->addChild(NewsLayer::create());

}
void MainMenu::touchMarkEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	m_Record = SussexLayer::create();
	if (m_Record)
	{
		this->addChild(m_Record);
	}
	MsgHallRequest::sendGetRecord(1);

}
void MainMenu::touchSetEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);

	auto setLayer = SetLayer::create();
	this->addChild(setLayer);

	if (m_menuLayer)
	{
		m_menuLayer->setVisible(false);
		menueCheck->setSelected(false);
	}
}
void MainMenu::touchIntroEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->addChild(PlayIntroLayer::create());
	if (m_menuLayer)
	{
		m_menuLayer->setVisible(false);
		menueCheck->setSelected(false);
	}
}
void MainMenu::touchCSEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->addChild(Circular::create());

}

void MainMenu::touchExtendEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	m_shopingLayer = DialogLayer::create(1, GetGameString("extend"));
	if (m_shopingLayer)
	{
		this->addChild(m_shopingLayer);
		auto text = dynamic_cast<Text*>(m_shopingLayer->m_systemLayout->getChildByName("Text"));
		if (text)
		{
			text->ignoreContentAdaptWithSize(false);
			text->setSize(Size(350, 70));
			text->setFontSize(20.0f);
		}
		auto button = dynamic_cast<Button*>(m_shopingLayer->m_systemLayout->getChildByName("Button_Ture_0"));
		if (button)
		{
			button->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchLoginShopEvent, this));
		}
	}
}

void MainMenu::touchShareEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);

	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		auto layer = ShareLayer::create();
		if (layer)
		{
			this->addChild(layer);
			layer->setUID(QHALLMGR->getUserId());
		}
		return;
	}
	cocos2d::log("MainMenu::touchShareEvent");
	WxSocial::getInstance()->shareRoom(GAMECONFIG.m_sHttpDownUrl.c_str(), GAMECONFIG.m_sAppName.c_str(), GetGameString("WXInvite3"), CC_CALLBACK_1(MainMenu::onInviteCB, this));
}

void MainMenu::touchCopyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto insteadRoomNum = QHALLMGR->getInsteadRoomNum();
	char str[50] = {};
	const char *roomNum = StringTool::intToString(insteadRoomNum).c_str();
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
		Director::getInstance()->getRunningScene()->addChild(popup, 1);
	}
	if (code == 0)//成功
	{
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("Copy_Success"), 30, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup, 1);
	}
}

void MainMenu::touchInviteEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
}

void MainMenu::touchInviteCodeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);

	auto inviteCodeLayer = InviteCode::create();
	if (inviteCodeLayer)
	{
		inviteCodeLayer->setName("inviteCode");
		this->addChild(inviteCodeLayer);
	}
	
}

void MainMenu::touchRebateEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->addChild(RebateLayer::create());

}

void MainMenu::touchProxyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->addChild(ProxyLayer::create());

}

void MainMenu::touchAddCardEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	m_shopingLayer = DialogLayer::create(1, GetGameString("buy_card"));
	if (m_shopingLayer)
	{
		this->addChild(m_shopingLayer);
		auto text = dynamic_cast<Text*>(m_shopingLayer->m_systemLayout->getChildByName("Text"));
		if (text)
		{
			text->ignoreContentAdaptWithSize(false);
			text->setSize(Size(350, 70));
			text->setFontSize(20.0f);
		}
		auto button = dynamic_cast<Button*>(m_shopingLayer->m_systemLayout->getChildByName("Button_Ture_0"));
		button->addTouchEventListener(CC_CALLBACK_2(MainMenu::touchLoginShopEvent, this));
	}
}

void MainMenu::touchLoginShopEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	if (m_shopingLayer)
	{
		m_shopingLayer->removeFromParentAndCleanup(true);
	}
}

void MainMenu::touchFeedBackEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	m_FeedBack = FeedBack::create();
	if (m_FeedBack)
	{
		this->addChild(m_FeedBack);
	}
}

void MainMenu::touchExpandEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	PopLayer* popup = PopLayer::create();
	popup->setContentText(GetGameString("exPand"), 40, 800, 500);
	Director::getInstance()->getRunningScene()->addChild(popup);
}

void MainMenu::onInviteCB(int errorCode)
{
    scheduleOnce([=](float dt)
     {
         if (errorCode == 0)
         {
             //邀请已经发送给好友
         }
         else
         {
             //您取消了邀请好友
         }
     }, 0.5f, "onInviteCB");
}

void MainMenu::touchCreateRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	QHALLMGR->setRoomType(QHALLMGR->R_NOMAL);
	auto layer = CreateRoom::create();
	layer->setName("createRoom");
	this->addChild(layer);
}

void MainMenu::touchBackRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);

	MsgHallRequest::sendAskRoom();
}

void MainMenu::touchDiamondRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto* pSender = (Button*)sender;
	if (pSender->getTag() == 1)
	{
		if (QHALLMGR->getRoomCard() < K_DIAMONDS_VIP_CARD_NUM)
		{
			//提示房卡不够
			PopLayer* popup = PopLayer::create();
			if (popup)
			{
				popup->setContentText(GetGameString("diamondRoomFail"), 30, 800, 500);
				Director::getInstance()->getRunningScene()->addChild(popup);
			}
		}
		else
		{
			QHALLMGR->setRoomType(QHALLMGR->R_DIAMOND);
			this->addChild(CreateRoom::create());
		}
	}
	else if (pSender->getTag() == 2)
	{
		if (QHALLMGR->getRoomCard() < K_COLOR_DIAMOND_VIP_CARD_NUM)
		{
			//提示房卡不够
			PopLayer* popup = PopLayer::create();
			if (popup)
			{
				popup->setContentText(GetGameString("colorDiamondRoomFail"), 30, 800, 500);
				Director::getInstance()->getRunningScene()->addChild(popup);
			}
		}
		else
		{
			QHALLMGR->setRoomType(QHALLMGR->R_COLORDIAMOND);
			this->addChild(CreateRoom::create());
		}
	}
}

void MainMenu::touchAddRoomEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->addChild(AddRoom::create());

}

void MainMenu::touchZJHEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	string webpageUrl = GetGameString("zjh_url");
	CCApplication::getInstance()->openURL(webpageUrl.c_str());
}

void MainMenu::onShowInviteDlg()
{
    this->addChild(InviteCode::create());
}

void MainMenu::showShop()
{
	this->removeChildByName("inviteCode");
	auto shop = ShopLayer::create();
	this->addChild(shop);
}

void MainMenu::touchLinkButtonEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto button=(Button*)sender;
	string webpageUrl = "";
	switch (button->getTag())
	{
	case 1:
	{
		webpageUrl = "http://jsmj.27qm.com";
	}
		break;
	case 2:
		webpageUrl = "http://jspk.27qm.com";
		break;
	default:
		break;
	}
	CCApplication::getInstance()->openURL(webpageUrl.c_str());
}

void MainMenu::selectedMenuEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto checkBox = (CheckBox*)sender;
	m_menuLayer->setVisible(checkBox->isSelected());
}

void MainMenu::LocationCallBack(double latitude, double Longitude, const char* sAddress)
{
	log("LocationCallBack --%lf--%lf--%s--", latitude, latitude, sAddress);
	if (sAddress == nullptr && sAddress == QHALLMGR->getCurAddress())
		return;
	QHALLMGR->setCurAddress(sAddress);
	MsgHallRequest::sendSetInfo(QHallManager::getInstance()->getNickName().c_str(), QHallManager::getInstance()->getHeadUrl().c_str(),
		QHallManager::getInstance()->getSex(), QHallManager::getInstance()->getCurAddress().c_str());
}

