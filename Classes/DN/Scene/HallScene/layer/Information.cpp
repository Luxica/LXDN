#include "Information.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../data/QHallManager.h"
#include "../../../../StringTool.h"
#include "GameAudioEngine.h"
#include "ScriptDataManager.h"
#include "WWMacros.h"
#include "WxSocial.h"
#include "GameConfig.h"
#include "../../GameScene/data/QGameManager.h"
#include "../../../Comm/request/MsgGameRequest.h"
USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "WebSprite.h"
#else
#include "GameNode/WebSprite.h"
#endif

Information* Information::create(QUserData* userDate)
{
	Information *pRet = new(std::nothrow) Information();
	if (pRet && pRet->init(userDate))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
bool Information::init(QUserData* userDate)
{
	if (!Layer::init())
	{
		return false;
	}
	auto systemLayer = CSLoader::createNode("InformationLayer.csb");
	if (systemLayer)
	{
		this->addChild(systemLayer);
		auto bg_layout = dynamic_cast<Layout*>(systemLayer->getChildByName("Panel_Bg"));
		if (bg_layout)
		{
			bg_layout->setOpacity(0);
			bg_layout->addTouchEventListener(CC_CALLBACK_2(Information::touchCloseEvent, this));
		}
		auto systemLayout = dynamic_cast<Layout*>(systemLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			auto panel_head = dynamic_cast<Layout*>(systemLayout->getChildByName("Panel_head"));
			if (panel_head)
			{
				panel_head->removeAllChildren();
				auto pPortrait = WebSprite::create(userDate->getStrImage(), Size(100.0f, 100.0f));
				pPortrait->setLocalZOrder(-1);
				panel_head->addChild(pPortrait);
				pPortrait->setPosition(panel_head->getContentSize().width / 2, panel_head->getContentSize().height / 2);
			}
			auto image_Sex = dynamic_cast<ImageView*>(systemLayout->getChildByName("Image_Sex"));
			if (image_Sex)
			{
				switch (userDate->getSex())
				{
				case 1:
					image_Sex->loadTexture("res/Information/m.png");
					break;
				case 2:
					image_Sex->loadTexture("res/Information/w.png");
					break;
				default:
					break;
				}
			}
			auto text_nick = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Nick"));
			if (text_nick)
			{
				text_nick->setString(userDate->getNickName());
			}
			auto text_ID = dynamic_cast<Text*>(systemLayout->getChildByName("Text_ID"));
			if (text_ID)
			{
				text_ID->setString(StringTool::intToString(userDate->getUserId()));
			}
			//位置
			auto text_Path = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Path"));
			if (text_Path)
			{
				char szPath[500] = {};
				sprintf(szPath, GetGameString("Path"), userDate->getStrLocation().c_str());
				text_Path->setString(szPath);
			}

			auto text_Win = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Win"));
			if (text_Win)
			{
				text_Win->setString(StringTool::intToString(userDate->getWinCount()));
			}

			auto text_Lose = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Lose"));
			if (text_Lose)
			{
				text_Lose->setString(StringTool::intToString(userDate->getLoseCount()));
			}

			auto text_Winrate = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Winrate"));
			if (text_Winrate)
			{
				float winrate = 0.0f;
				if (userDate->getWinCount() == 0 && userDate->getLoseCount() == 0)
				{
					winrate = 0.0f;
				}
				else
				{
					winrate = (float)(userDate->getWinCount()) / (float)((userDate->getWinCount() + userDate->getLoseCount())) * 100;
				}
				char szName[50] = {};
				sprintf(szName, "%.1f%%", winrate);
				text_Winrate->setString(szName);
			}

			auto card_Text = dynamic_cast<Text*>(systemLayout->getChildByName("Text_1_1"));
			if (card_Text)
			{
				if (userDate->getUserId() == QHALLMGR->getUserId())
				{
					card_Text->setVisible(true);
				}
				else
					card_Text->setVisible(false);
			}
			auto text_card = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Card"));
			if (text_card)
			{
				if (userDate->getUserId() == QHALLMGR->getUserId())
				{
					text_card->setString(StringTool::intToString(QHallManager::getInstance()->getRoomCard()));
				}
				else
					text_card->setVisible(false);
			}
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(Information::touchCloseEvent, this));
			}
			auto express_layout = dynamic_cast<Layout*>(systemLayout->getChildByName("Panel_Express"));
			if (express_layout)
			{
				for (int i = 0; i < 5; i++)
				{
					char dzChar[100] = { 0 };
					sprintf(dzChar, "Image_%d", i);
					auto imageview = dynamic_cast<ImageView*>(express_layout->getChildByName(dzChar));
					if (imageview)
					{
						imageview->setTag(i);
						imageview->addTouchEventListener(CC_CALLBACK_2(Information::touchExpressEvent, this, userDate));
						m_imageView.push_back(imageview);
					}
				}
				if (userDate->getSeatId() == QGameManager::getInstance()->getSeatId())
				{
					express_layout->setVisible(false);
				}
			}
		}
	}
	this->setScale(0.0f);
	auto scal = ScaleTo::create(0.2f, 1.0f);
	this->runAction(scal);
	return true;
}
bool Information::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto systemLayer = CSLoader::createNode("InformationLayer.csb");
	if (systemLayer)
	{
		this->addChild(systemLayer);
		auto systemLayout = dynamic_cast<Layout*>(systemLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			auto panel_head = dynamic_cast<Layout*>(systemLayout->getChildByName("Panel_head"));
			if (panel_head)
			{
				panel_head->removeAllChildren();
				auto pPortrait = WebSprite::create(QHallManager::getInstance()->getHeadUrl(), Size(100.0f, 100.0f));
				pPortrait->setLocalZOrder(-1);
				panel_head->addChild(pPortrait);
				pPortrait->setPosition(panel_head->getContentSize().width / 2, panel_head->getContentSize().height / 2);
			}
			auto image_Sex = dynamic_cast<ImageView*>(systemLayout->getChildByName("Image_Sex"));
			if (image_Sex)
			{
				switch (QHallManager::getInstance()->getSex())
				{
				case 1:
					image_Sex->loadTexture("res/Information/m.png");
					break;
				case 2:
					image_Sex->loadTexture("res/Information/w.png");
					break;
				default:
					break;
				}
			}
			auto text_nick = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Nick"));
			if (text_nick)
			{
				text_nick->setString(QHallManager::getInstance()->getNickName());
			}
			auto text_ID = dynamic_cast<Text*>(systemLayout->getChildByName("Text_ID"));
			if (text_ID)
			{
				text_ID->setString(StringTool::intToString(QHallManager::getInstance()->getUserId()));
			}
			auto text_card = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Card"));
			if (text_card)
			{
				text_card->setString(StringTool::intToString(QHallManager::getInstance()->getRoomCard()));
			}
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(Information::touchCloseEvent, this));
			}
		}
	}
	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Information::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Information::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Information::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

//触碰事件  （单点触碰）
bool Information::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void Information::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void Information::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void Information::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

void Information::touchExpressEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type, QUserData* userDate)
{
	if (type != Widget::TouchEventType::ENDED || userDate == nullptr)
		return;
	auto chat = (ImageView*)sender;
	auto left = userDate->getSeatId() + 4;
	auto right = chat->getTag();
	MsgGameRequest::sendChatFast(left << 4 | right);
	this->removeFromParentAndCleanup(true);
}

