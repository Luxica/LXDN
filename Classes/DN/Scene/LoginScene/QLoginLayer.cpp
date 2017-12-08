#include "QLoginLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../GUISystem/WWSceneManager.h"
#include "../../Scene/HallScene/QHallScene.h"
#include "../../../net/LordNetManger.h"
#include "../LoginScene/layer/LoginLoad.h"
#include "../../../StringTool.h"
#include "../../Scene/LoginScene/data/QLoginManager.h"
#include "../../Scene/GlobalLayer/PopLayer.h"
#include "../../Scene/HallScene/data/QHallManager.h"
#include "../../../GameAudioEngine.h"
#include "GameConfig.h"
USING_NS_CC;

using namespace cocostudio::timeline;

QLoginLayer::QLoginLayer()
{
	//log("Enter...QLoginScene");
}

QLoginLayer::~QLoginLayer()
{
	//log("exit....QLoginScene");
}

bool QLoginLayer::init()
{
	QHallManager::getInstance()->setIsFirst(true);
	auto rootNode = CSLoader::createNode("LoginScene.csb");
	if (rootNode)
	{
		addChild(rootNode);
		auto ID_PW = CSLoader::createNode("LoginIDPWLayer.csb");
		if (ID_PW)
		{
			addChild(ID_PW);
            ID_PW->setVisible(GAMECONFIG.m_bNetTest);
			m_userID = dynamic_cast<TextField*>(ID_PW->getChildByName("TextField_ID"));
			m_userPW = dynamic_cast<TextField*>(ID_PW->getChildByName("TextField_PW"));
		}

		auto layer = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_1"));
		if (layer)
		{		
			//关闭按钮
			closeButton = dynamic_cast<Button*>(layer->getChildByName("Button_Close"));
			if (closeButton)
			{
				//事件回调   按钮做什么事
				closeButton->addTouchEventListener(CC_CALLBACK_2(QLoginLayer::touchCloseEvent, this));
			}

			loginButton = dynamic_cast<Button*>(layer->getChildByName("Button_Login"));
			if (loginButton)
			{
				if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
				{
					auto animation = Animation::create();
					for (int i = 1; i < 8; i++)
					{
						char szName[100] = { 0 };
						sprintf(szName, "Login/2-%d.png", i);
						animation->addSpriteFrameWithFile(szName);
					}
					// 控制帧动画的每帧间的播放间隔
					animation->setDelayPerUnit(0.2f);
					// 是否在播放后重置成第一张图片
					animation->setRestoreOriginalFrame(false);
					auto action_Create = Animate::create(animation);

					auto sprite = Sprite::create();
					sprite->setTexture("Login/2-1.png");
					sprite->setPosition(loginButton->getPosition());
					addChild(sprite);
					sprite->runAction(RepeatForever::create(action_Create));
				}
				loginButton->addTouchEventListener(CC_CALLBACK_2(QLoginLayer::touchLoginEvent, this));
			}

			m_agreement = dynamic_cast<Text*>(layer->getChildByName("Text_Agreement"));
			m_checkBox = dynamic_cast<CheckBox*>(layer->getChildByName("CheckBox_Xy"));
		}
	}
	return true;
}


void QLoginLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WWSceneManager::getInstance()->exit();
}

void QLoginLayer::touchLoginEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	PLAYEFFECT;
	loginButton->setTouchEnabled(false);
	if (m_checkBox->getSelectedState())
	{
        WWSceneManager::getInstance()->openGlobalUI(LoginLoad::create());
		if (m_userID->getString().length() > 0 && m_userPW->getString().length() > 0)
		{
			//setButtonVisibleNo();
			QHallManager::getInstance()->setAccount(m_userID->getString());
			QHallManager::getInstance()->setPassword(m_userPW->getString());
			LordNetManger::getInstance()->connectNet(WWNetwork::eHallNet);
		}
		else
		{
			//微信登入
			WxSocial::getInstance()->doWxLogin(CC_CALLBACK_3(QLoginLayer::onWxAuthCb, this));
		}
	}
	loginButton->setTouchEnabled(true);
}

void QLoginLayer::onWxAuthCb(bool success, int code,const WxLoginInfo* wxLoginInfo)
{
	/*char sztt[512] = { 0 };
	sprintf(sztt, "e%dabc%d", code, success);
	m_userID->setString(sztt);*/

	if (success)
    {
       //微信登入成功
        std::string account  = wxLoginInfo->unionid.c_str();
        std::string url  = wxLoginInfo->headUrl.c_str();
        
        QHallManager::getInstance()->setAccount(wxLoginInfo->unionid);
        QHallManager::getInstance()->setPassword("123");
        QHallManager::getInstance()->setNickName(wxLoginInfo->nickName);
        QHallManager::getInstance()->setHeadUrl(wxLoginInfo->headUrl);
        QHallManager::getInstance()->setSex(wxLoginInfo->sex);
        LordNetManger::getInstance()->connectNet(WWNetwork::eHallNet);
    }
    else
    {
        WWSceneManager::getInstance()->closeGlobalUIByName("loginload");
		/*char sztt[512] = { 0 };
		sprintf(sztt, "e%dabc", code);
		m_userID->setString(sztt);*/
		return;
    }
}

