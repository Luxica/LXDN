#include "TalkLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../Comm/request/MsgGameRequest.h"
#include "../../../../ScriptDataManager.h"
#include "../../GameScene/QGameLayer.h"
#include "GameConfig.h"

USING_NS_CC;

// on "init" you need to initialize your instance


bool TalkLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		m_pRootNode = CSLoader::createNode("chat/expressionLayer.csb");
	}
	else
	{
		m_pRootNode = CSLoader::createNode("chatLayer.csb");
	}
	addChild(m_pRootNode);
	auto layout = dynamic_cast<Layout*>(m_pRootNode->getChildByName("Panel_Bg"));
	m_layout = dynamic_cast<Layout*>(m_pRootNode->getChildByName("Panel_Talk"));
	m_layout->setVisible(true);
	if (layout)
	{
		layout->addTouchEventListener(CC_CALLBACK_2(TalkLayer::touchCloseEvent, this));
	}
	else
		m_layout->addTouchEventListener(CC_CALLBACK_2(TalkLayer::touchCloseEvent, this));

	auto listView = dynamic_cast<ListView*>(m_layout->getChildByName("ListView"));
	auto text = dynamic_cast<Text*>(m_layout->getChildByName("Text_talk"));
	if (text)
	{
		text->setVisible(false);
		char szName[32] = { 0 };
		char szText[32] = { 0 };
		for (int i = 0; i < 11; i++)
		{
			auto temp = (Text*)(text->clone());
			temp->setVisible(true);
			listView->pushBackCustomItem(temp);
			sprintf(szText, "chat_%d", i + 1);
			auto chatText = ScriptDataManager::getInterface()->getGameString(szText);
			temp->setString(chatText);
			temp->setTag(i + 1);
			temp->addTouchEventListener(CC_CALLBACK_2(TalkLayer::touchQuickTalkEvent, this));
		}
	}

	auto panel_talk = dynamic_cast<Layout*>(m_layout->getChildByName("Panel_1"));
	if (panel_talk)
	{
		panel_talk->setVisible(false);
		char szName[32] = { 0 };
		char szText[32] = { 0 };
		for (int i = 0; i < 11; i++)
		{
			auto panel_clone = (Layout*)(panel_talk->clone());
			panel_clone->setVisible(true);
			listView->pushBackCustomItem(panel_clone);
			sprintf(szText, "chat_%d", i + 1);
			auto chatText = ScriptDataManager::getInterface()->getGameString(szText);
			auto text_clone = dynamic_cast<Text*>(panel_clone->getChildByName("Text_talk"));
			if (text_clone)
			{
				text_clone->setString(chatText);
				text_clone->setTag(i + 1);
				text_clone->addTouchEventListener(CC_CALLBACK_2(TalkLayer::touchQuickTalkEvent, this));
			}
		}
	}

	m_eLayout = dynamic_cast<Layout*>(m_pRootNode->getChildByName("Panel_Express"));
	if (m_eLayout)
	{
		for (int i = 0; i < 24; i++)
		{
			char dzChar[100] = { 0 };
			sprintf(dzChar, "Image_%d", i + 1);
			auto imageview = dynamic_cast<ImageView*>(m_eLayout->getChildByName(dzChar));
			if (imageview)
			{
				imageview->setTag(i);
				m_imageView.push_back(imageview);
			}
		}
		for (int i = 0; i < m_imageView.size(); i++)
		{
			m_imageView.at(i)->addTouchEventListener(CC_CALLBACK_2(TalkLayer::touchExpressEvent, this));
		}
	}
	chatButton = dynamic_cast<Button*>(m_pRootNode->getChildByName("Button_Chat"));
	if (chatButton)
	{
		/*if (GAMECONFIG.m_nAppType == GAME_TYPE_TTDN)
		{
		chatButton->setEnabled(false);
		}*/
		chatButton->addTouchEventListener(CC_CALLBACK_2(TalkLayer::touchChatEvent, this));
	}
	expressButton = dynamic_cast<Button*>(m_pRootNode->getChildByName("Button_Express"));
	if (expressButton)
	{
		expressButton->addTouchEventListener(CC_CALLBACK_2(TalkLayer::touchButtonExpressEvent, this));
	}
    return true;
}
void TalkLayer::touchChatEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	if (m_layout->isVisible())
		return;
	/*if (GAMECONFIG.m_nAppType == GAME_TYPE_TTDN)
	{
	if (chatButton)
	{
	chatButton->setEnabled(false);
	}
	if (expressButton)
	{
	expressButton->setEnabled(true);
	}
	}*/
	m_layout->setVisible(true);
	m_eLayout->setVisible(false);
}
void TalkLayer::touchButtonExpressEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	if (m_eLayout->isVisible())
		return;
	//if (GAMECONFIG.m_nAppType == GAME_TYPE_TTDN)
	//{
	//	if (chatButton)
	//	{
	//		chatButton->setEnabled(true);
	//	}
	//	if (expressButton)
	//	{
	//		expressButton->setEnabled(false);
	//	}
	//}
	m_layout->setVisible(false);
	m_eLayout->setVisible(true);
}

void TalkLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	
	 closeSelf();
}

void TalkLayer::touchExpressEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	//send talkMsg
	auto chat  = (ImageView*)sender;
	auto left  = chat->getTag() / 16 + 2;
	auto right = chat->getTag() % 16;
	MsgGameRequest::sendChatFast(left<<4|right);
	QGameLayer * gameLayer = dynamic_cast<QGameLayer*>(m_pParent);
	gameLayer->setSpeekTime();
	closeSelf();
}

void TalkLayer::touchQuickTalkEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    //send talkMsg
	auto chat = (Text*)sender;
	auto left = chat->getTag() / 16 + 0;
	auto right = chat->getTag() % 16;
	MsgGameRequest::sendChatFast(left << 4 | right);
	QGameLayer * gameLayer = dynamic_cast<QGameLayer*>(m_pParent);
	gameLayer->setSpeekTime();
    closeSelf();
}

void TalkLayer::closeSelf()
{
    this->removeFromParentAndCleanup(true);
}

void TalkLayer::setRoomID(int roomId)
{
    m_nRoomId = roomId;
}

void TalkLayer::showDialog(Node *parent)
{
    m_pParent = parent;
    m_pParent->addChild(this,100);
}




