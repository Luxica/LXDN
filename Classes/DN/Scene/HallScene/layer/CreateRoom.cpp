#include "CreateRoom.h"
#include "cocostudio/CocoStudio.h"
#include "../../GameScene/QGameScene.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Scene/HallScene/data/QHallManager.h"
#include "../../../Comm/request/MsgHallRequest.h"
#include "../../../Scene/GlobalLayer/Load.h"
#include "../../QDNEvent.h"
#include "../../../Scene/GlobalLayer/PopLayer.h"
#include "../../GlobalLayer/Prompt.h"
#include "ScriptDataManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "GameConfig.h"
USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance

bool CreateRoom::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	m_isForbid = false;
	m_isMMode = true;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto systemExitLayer = CSLoader::createNode("createRoomLayer.csb");
	this->addChild(systemExitLayer);
	layout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));

	auto closeButton = dynamic_cast<Button*>(layout->getChildByName("Button_Close"));
	if (closeButton)
	{
		closeButton->addTouchEventListener(CC_CALLBACK_2(CreateRoom::touchCloseEvent, this));
	}

	auto trueButton = dynamic_cast<Button*>(layout->getChildByName("Button_True"));
	if (trueButton)
	{
		trueButton->setTag(0);
		trueButton->addTouchEventListener(CC_CALLBACK_2(CreateRoom::touchTrueEvent, this));
	}

	//自己开房按钮
	auto button_True_Self = dynamic_cast<Button*>(layout->getChildByName("Button_True_Self"));
	if (button_True_Self)
	{
		//button_True_Self->setEnabled(true);
		button_True_Self->setTag(0);
		button_True_Self->addTouchEventListener(CC_CALLBACK_2(CreateRoom::touchTrueEvent, this));
	}

	//代开房按钮
	auto button_True_Instead = dynamic_cast<Button*>(layout->getChildByName("Button_True_Instead"));
	if (button_True_Instead)
	{
		button_True_Instead->setTag(2);
		button_True_Instead->addTouchEventListener(CC_CALLBACK_2(CreateRoom::touchTrueEvent, this));
	}

	//创建局数
	auto checkBox_JS_10 = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_JS_10"));
	if (checkBox_JS_10)
	{
		checkBox_JS_10->setSelected(true);
		checkBox_JS_10->setTouchEnabled(false);
		checkBox_JS_10->setTag(10);
		checkBox_JS_10->addEventListener(CC_CALLBACK_2(CreateRoom::selectedGameNumEvent, this));
		m_pGameNum = checkBox_JS_10;
	}

	auto checkBox_JS_16 = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_JS_16"));
	if (checkBox_JS_16)
	{
		checkBox_JS_16->setSelected(false);
		checkBox_JS_16->setTag(16);
		checkBox_JS_16->addEventListener(CC_CALLBACK_2(CreateRoom::selectedGameNumEvent, this));
	}
	
	//坐庄模式
	//房主坐庄
	auto checkBox_ZZ_FZZZ = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_ZZ_FZZZ"));
	if (checkBox_ZZ_FZZZ)
	{
		checkBox_ZZ_FZZZ->setSelected(true);
		checkBox_ZZ_FZZZ->setTouchEnabled(false);
		checkBox_ZZ_FZZZ->setTag(2);
		checkBox_ZZ_FZZZ->addEventListener(CC_CALLBACK_2(CreateRoom::selectedGameModelEvent, this));
		m_pGameMode = checkBox_ZZ_FZZZ;
	}

	//轮流坐庄
	auto checkBox_ZZ_LLZZ = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_ZZ_LLZZ"));
	if (checkBox_ZZ_LLZZ)
	{
		checkBox_ZZ_LLZZ->setSelected(false);
		checkBox_ZZ_LLZZ->setTag(1);
		checkBox_ZZ_LLZZ->addEventListener(CC_CALLBACK_2(CreateRoom::selectedGameModelEvent, this));
	}

	//翻四张抢庄
	auto checkBox_ZZ_FSZQZ = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_ZZ_FSZQZ"));
	if (checkBox_ZZ_FSZQZ)
	{
		checkBox_ZZ_FSZQZ->setTag(9);
		checkBox_ZZ_FSZQZ->addEventListener(CC_CALLBACK_2(CreateRoom::selectedGameModelEvent, this));
	}
	
	//经典抢庄
	auto checkBox_ZZ_JDQZ = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_ZZ_JDQZ"));
	if (checkBox_ZZ_JDQZ)
	{
		checkBox_ZZ_JDQZ->setSelected(false);
		checkBox_ZZ_JDQZ->setTag(0);
		checkBox_ZZ_JDQZ->addEventListener(CC_CALLBACK_2(CreateRoom::selectedGameModelEvent, this));
	}

	//房卡支付方式（AA or 房主）
	//房主支付
	auto checkBox_FF_Owner = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_FF_Owner"));
	if (checkBox_FF_Owner)
	{
		checkBox_FF_Owner->setSelected(true);
		checkBox_FF_Owner->setTouchEnabled(false);
		m_pGamePay = checkBox_FF_Owner;
		m_costType = 1;
		checkBox_FF_Owner->setTag(1);
		checkBox_FF_Owner->addEventListener(CC_CALLBACK_2(CreateRoom::selectedGamePayEvent, this));
	}

	//AA支付
	auto checkBox_FF_AA = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_FF_AA"));
	if (checkBox_FF_AA)
	{
		checkBox_FF_AA->setSelected(false);
		checkBox_FF_AA->setTag(0);
		checkBox_FF_AA->addEventListener(CC_CALLBACK_2(CreateRoom::selectedGamePayEvent, this));
	}

	//全副牌还是半副牌
	m_pHalf = nullptr;
	//有花牌
	auto checkBox_WF_YHP = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_WF_YHP"));
	if (checkBox_WF_YHP)
	{
		checkBox_WF_YHP->setSelected(true);
		checkBox_WF_YHP->setTouchEnabled(false);
		checkBox_WF_YHP->setTag(0);
		checkBox_WF_YHP->addEventListener(CC_CALLBACK_2(CreateRoom::selectedHalfEvent, this));
		m_pHalf = checkBox_WF_YHP;
	}
	//无花牌
	auto checkBox_WF_WHP = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_WF_WHP"));
	if (checkBox_WF_WHP)
	{
		checkBox_WF_WHP->setSelected(false);
		checkBox_WF_WHP->setTag(1);
		checkBox_WF_WHP->addEventListener(CC_CALLBACK_2(CreateRoom::selectedHalfEvent, this));
	}

	//无牛番
	auto checkBox_WF_WNF = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_WF_WNF"));
	if (checkBox_WF_WNF)
	{
		checkBox_WF_WNF->addEventListener(CC_CALLBACK_2(CreateRoom::selectedMModeEvent, this));
	}

	//是否禁止中途加入
	auto checkBox_WF_JZJR = dynamic_cast<CheckBox*>(layout->getChildByName("CheckBox_WF_JZJR"));
	if (checkBox_WF_JZJR)
	{
		checkBox_WF_JZJR->addEventListener(CC_CALLBACK_2(CreateRoom::selectedForbidEvent, this));
	}

    m_roomCardText = dynamic_cast<Text*>(layout->getChildByName("Text_12"));
    onShowCostRoomCard();

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CreateRoom::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CreateRoom::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CreateRoom::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto pListener = EventListenerCustom::create(EVENT_ENTEROOM_FILURE, CC_CALLBACK_1(CreateRoom::onHideWaitLayer, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

    return true;
}

void CreateRoom::onHideWaitLayer(EventCustom *pEvent)
{
	log("111111111");
	//把等待界面去掉
	//WWSceneManager::getInstance()->closeGlobalUIByName("load");
	//WWSceneManager::getInstance()->closeGlobalUI(Load::create());
}

bool CreateRoom::isCreat()
{
	if (QHallManager::getInstance()->getCostMode() == 0)
	{
		switch (QHallManager::getInstance()->getPlayMode())
		{
		case 5:
			m_costCard = 1;
			break;
		case 10:
			m_costCard = 10;
			break;
		case 15:
			m_costCard = 1;
			break;
		case 16:
			m_costCard = 10;
			break;
		case 20:
			m_costCard = 2;
			break;
		case 30:
		{
			m_costCard = 3;
		}
			break;
		default:
			break;
		}
	}
	if (QHallManager::getInstance()->getCostMode() == 1)
	{
		switch (QHallManager::getInstance()->getPlayMode())
		{
		case 5:
			m_costCard = GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL ? 3 : 4;
			break;
		case 10:
		{
			if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
			{
				m_costCard = 50;
			}
		}
			break;
		case 15:
		{
			m_costCard = GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL ? 3 : 4;
		}
			break;
		case 16:
			m_costCard = 50;
			break;
		case 20:
		{
			if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
			{
				m_costCard = 2;
			}
		}
			break;
		case 30:
		{
			m_costCard = GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL ? 6 : 10;
		}
			break;
		default:
			break;
		}
	}
	int userRoomCard = QHallManager::getInstance()->getRoomCard();
	log("----%d-----", userRoomCard);
	if (userRoomCard >= m_costCard)
	{
		log("=---------sdfsdfasd");
		//QHallManager::getInstance()->setRoomCard(userRoomCard-m_costCard);
		return true;
	}
	else
	{
		//房卡不足的弹窗
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("roomCardLack"), 30, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
		return false;
	}
}

bool CreateRoom::isInstead()
{
	if (QHallManager::getInstance()->getCostMode() == 1)
	{
		switch (QHallManager::getInstance()->getPlayMode())
		{
		case 5:
			m_costCard = GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL ? 3 : 4;
			break;
		case 10:
		{
			if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
			{
				m_costCard = 1;
			}
		}
			break;
		case 15:
		{
			m_costCard = GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL ? 3 : 4;
		}
			break;
		case 20:
		{
			m_costCard = GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL ? 6 : 7;
			if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
			{
				m_costCard = 2;
			}
		}
			break;
		case 30:
		{
			m_costCard = GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL ? 6 : 10;
		}
			break;
		default:
			break;
		}
	}
	int userRoomCard = QHallManager::getInstance()->getRoomCard();
	QHALLMGR->setCostNum(m_costCard);
	log("----%d-----", userRoomCard);
	if (userRoomCard >= m_costCard)
	{
		log("=---------sdfsdfasd");
		//QHallManager::getInstance()->setRoomCard(userRoomCard-m_costCard);
		return true;
	}
	else
	{
		//房卡不足的弹窗
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("roomCardLack"), 30, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
		return false;
	}
}

//触碰事件  （单点触碰）
bool CreateRoom::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void CreateRoom::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void CreateRoom::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void CreateRoom::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED://按钮按下在按钮上抬起

		GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
		this->removeFromParentAndCleanup(true);
	
		break;
	}
}
void CreateRoom::touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	auto curButton = (Button*)sender;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);

	if (m_pGameNum)
	{
		QHALLMGR->setPlayMode(m_pGameNum->getTag());
	}
	QHALLMGR->setGameMode(0);
	QHALLMGR->setKcardWay(5);
	if (m_pGameMode)
	{
		QHALLMGR->setZMode(m_pGameMode->getTag());
		if (m_pGameMode->getTag() == 9)
		{
			QHALLMGR->setZMode(0);
			QHALLMGR->setKcardWay(1);
		}
	}
	QHALLMGR->setCostMode(m_pGamePay->getTag());
	QHALLMGR->setMMode(m_isMMode);
	QHALLMGR->setMinCard(0);
	if (m_pHalf)
	{
		QHALLMGR->setHalf(m_pHalf->getTag());
	}
	QHALLMGR->setForbid(m_isForbid);

	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		QHallManager::getInstance()->setMaxBet(30);
	}
	if (curButton->getTag() == 2)
	{
		QHallManager::getInstance()->setCostMode(1);

		if (isInstead())
		{
			QHallManager::getInstance()->setJoinType(curButton->getTag());
			WWSceneManager::getInstance()->openGlobalUI(Load::create());

			int gameType = 1;//创建房游戏类型填（牛牛1，扎金花2）
			MsgHallRequest::sendEnterRoom(gameType << 24 & 0xFF000000);
		}
	}
	else
	{
		if (isCreat())
		{
			QHallManager::getInstance()->setJoinType(curButton->getTag());
			WWSceneManager::getInstance()->openGlobalUI(Load::create());

			int gameType = 1;//创建房游戏类型填（牛牛1，扎金花2）
			MsgHallRequest::sendEnterRoom(gameType << 24 & 0xFF000000);
		}
	}
}

void CreateRoom::selectedGameNumEvent(Ref* pSender, CheckBox::EventType type)
{
	bool isSelect = false;
	if (type != CheckBox::EventType::SELECTED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto checkBox = (CheckBox*)pSender;
	if (m_pGameNum == checkBox)
		return;
	checkBox->setSelected(true);
	checkBox->setTouchEnabled(false);
	m_pGameNum->setSelected(false);
	m_pGameNum->setTouchEnabled(true);
	m_pGameNum = checkBox;
	onShowCostRoomCard();
}


/**
 * 显示扣除房卡
 */
void CreateRoom::onShowCostRoomCard()
{
	int costNum = 0;
	switch (m_costType)
	{
	case 0:
		costNum = 10;
		break;
	case 1:
		costNum = 50;
		break;
	default:
		break;
	}
	char szCost[100] = {};
	sprintf(szCost, GetGameString("costNum"), costNum);

	auto text_Cost_10 = dynamic_cast<Text*>(layout->getChildByName("Text_Cost_10"));
	if (text_Cost_10)
	{
		text_Cost_10->setString(szCost);
	}
	auto text_Cost_16 = dynamic_cast<Text*>(layout->getChildByName("Text_Cost_16"));
	if (text_Cost_16)
	{
		text_Cost_16->setString(szCost);
	}
}

void CreateRoom::selectedGameModelEvent(Ref* pSender, CheckBox::EventType type)
{
	bool isSelect = false;
	if (type != CheckBox::EventType::SELECTED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto checkBox = (CheckBox*)pSender;
	if (m_pGameMode == checkBox)
		return;
	checkBox->setSelected(true);
	checkBox->setTouchEnabled(false);
	m_pGameMode->setSelected(false);
	m_pGameMode->setTouchEnabled(true);
	m_pGameMode = checkBox;
	onShowCostRoomCard();
}

void CreateRoom::selectedGamePayEvent(Ref* pSender, CheckBox::EventType type)
{
	bool isSelect = false;
	if (type != CheckBox::EventType::SELECTED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto checkBox = (CheckBox*)pSender;
	if (m_pGamePay == checkBox)
		return;
	checkBox->setSelected(true);
	checkBox->setTouchEnabled(false);
	m_pGamePay->setSelected(false);
	m_pGamePay->setTouchEnabled(true);
	m_pGamePay = checkBox;
	m_costType = m_pGamePay->getTag();
	onShowCostRoomCard();
}

void CreateRoom::selectedKcardWayEvent(Ref* pSender, CheckBox::EventType type)
{
	bool isSelect = false;
	if (type != CheckBox::EventType::SELECTED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto checkBox = (CheckBox*)pSender;
}
void CreateRoom::selectedMaxBetEvent(Ref* pSender, CheckBox::EventType type)
{
	bool isSelect = false;
	if (type != CheckBox::EventType::SELECTED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto checkBox = (CheckBox*)pSender;

}

void CreateRoom::selectedHalfEvent(Ref* pSender, CheckBox::EventType type)
{
	bool isSelect = false;
	if (type != CheckBox::EventType::SELECTED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto checkBox = (CheckBox*)pSender;
	if (m_pHalf == checkBox)
		return;
	checkBox->setSelected(true);
	checkBox->setTouchEnabled(false);
	m_pHalf->setSelected(false);
	m_pHalf->setTouchEnabled(true);
	m_pHalf = checkBox;
}

void CreateRoom::selectedMModeEvent(Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		m_isMMode = false;
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		m_isMMode = true;
		break;
	default:
		break;
	}
}

void CreateRoom::selectedForbidEvent(Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		m_isForbid = true;
		break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		m_isForbid = false;
		break;
	default:
		break;
	}
}

void CreateRoom::selectedPlayTypeEvent(Ref* pSender, CheckBox::EventType type)
{
	bool isSelect = false;
	if (type != CheckBox::EventType::SELECTED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	auto checkBox = (CheckBox*)pSender;
}




