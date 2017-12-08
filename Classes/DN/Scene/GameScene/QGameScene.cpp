#include "QGameScene.h"
#include "../../../GUISystem/WWSceneManager.h"
#include "QGameLayer.h"
#include "../QDNEvent.h"
#include "data/QGameManager.h"
#include "StringTool.h"

QGameScene::QGameScene()
{
	m_pGameLayer_ = nullptr;
	setName("QGameScene");
	log("Enter...QGameScene");
    
    auto pListener = EventListenerCustom::create("EVENT_GOTO_ROOM", [](EventCustom* event) {
        QGameManager::getInstance()->setGotoRoomId("0");
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);
}

QGameScene::~QGameScene()
{
	log("exit....QGameScene");
}

bool QGameScene::init()
{
	m_pGameLayer_ = QGameLayer::create();
	addChild(m_pGameLayer_);

	initListenter();
	return true;
}

void QGameScene::initListenter()
{
	

	
}

