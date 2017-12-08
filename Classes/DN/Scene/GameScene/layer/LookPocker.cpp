#include "LookPocker.h"
#include "cocostudio/CocoStudio.h"
#include "../../../Comm/request/MsgGameRequest.h"
#include "../../../../ScriptDataManager.h"
#include "../../GameScene/QGameLayer.h"
#include "../data/QGameManager.h"
#include "../data/QGameBaseData.h"
#include "../../GlobalLayer/PopLayer.h"
#include "GameConfig.h"

using namespace cocos2d;
USING_NS_CC;

// on "init" you need to initialize your instance

LookPocker::LookPocker()
{
	m_parentLayer=nullptr;
	m_pRootNode = nullptr;
	touchIndex = 0;
	cuoindex = 1;
	isShow = false;
}

LookPocker::~LookPocker()
{
	onExit();
	for (int i = 0; i < m_vecPocket.size(); i++)
	{
		if (m_vecPocket.at(i))
			m_vecPocket.at(i)->removeFromParent();
	}
	m_vecPocket.clear();
}

bool LookPocker::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	m_pRootNode = CSLoader::createNode("GameScene/LookPokerLayer.csb");
	addChild(m_pRootNode);

	auto layer = dynamic_cast<Layout*>(m_pRootNode->getChildByName("Panel"));
	if (layer)
	{
		m_panelPos[0] = dynamic_cast<ImageView*>(layer->getChildByName("Image_0"));
		if (m_panelPos[0]) m_panelPos[0]->setTag(0);
		m_panelPos[1] = dynamic_cast<ImageView*>(layer->getChildByName("Image_1"));
		if (m_panelPos[1])  m_panelPos[1]->setTag(1);
		m_panelPos[2] = dynamic_cast<ImageView*>(layer->getChildByName("Image_2"));
		if (m_panelPos[2])  m_panelPos[2]->setTag(2);
		m_panelPos[3] = dynamic_cast<ImageView*>(layer->getChildByName("Image_3"));
		if (m_panelPos[3])  m_panelPos[3]->setTag(3);
		m_panelPos[4] = dynamic_cast<ImageView*>(layer->getChildByName("Image_4"));
		if (m_panelPos[4])  m_panelPos[4]->setTag(4);

		auto pockeSprite = dynamic_cast<Sprite*>(layer->getChildByName("Sprite_Touch_0"));
		if (pockeSprite) m_vecTouchSprite.push_back(pockeSprite);

		layer->addTouchEventListener(CC_CALLBACK_2(LookPocker::touchCloseEvent, this));
	}

	//触碰事件  (单点触碰)
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LookPocker::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(LookPocker::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(LookPocker::onTouchEnded, this);
	//提高监听优先级addEventListenerWithFixedPriority
	_eventDispatcher->addEventListenerWithFixedPriority(listener, -1);//值越小优先级越高

	auto closeButton = dynamic_cast<Button*>(m_pRootNode->getChildByName("Button_Close"));
	if (closeButton)
	{
		closeButton->addTouchEventListener(CC_CALLBACK_2(LookPocker::touchCloseEvent, this));
	}
	return true;
}
void LookPocker::refreshData(QCardData *pCard)
{
	m_vecPocket.clear();
	auto cardVec = pCard->getPokerVec();
	char szName[100] = { 0 };
	for (int i = 0; i < cardVec.size(); i++)
	{
		auto pockerType = (PocketSprite::PocketType)(cardVec.at(i) & 0xF);
		auto pockerNum = (PocketSprite::PocketNum)(cardVec.at(i) >> 4);
		sprintf(szName, "Pocke/%d-%d.png", (int)pockerNum, (int)pockerType);
		PocketSprite* pocket = PocketSprite::create(szName, pockerType, pockerNum);
		pocket->setPosition(m_panelPos[1]->getPosition());
		this->addChild(pocket);
		pocket->setScale(2.0f);
		m_vecPocket.push_back(pocket);
	}
	float voiceLenght = 0.0f;
	DelayTime *pDelayTime = DelayTime::create(voiceLenght);
	CallFunc *pHideFunc = CallFunc::create([=]
	{
		ShowPocker();
	});
	Sequence *pAction = Sequence::create(pDelayTime, pHideFunc, NULL);
	this->runAction(pAction);
}
//触碰事件  （单点触碰）
bool LookPocker::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	m_beginPoint = touch->getLocation();
	for (int i = 0; i < m_vecTouchSprite.size(); i++)
	{
		auto pocker = m_vecTouchSprite.at(i);
		Rect ballRect = pocker->getBoundingBox();
		if (ballRect.containsPoint(m_beginPoint))
		{
			isControl = isShow ? false : true;
		}
	}
	auto closeButton = dynamic_cast<Button*>(m_pRootNode->getChildByName("Button_Close"));
	if (closeButton)
	{
		if (closeButton->getBoundingBox().containsPoint(m_beginPoint))
			_eventDispatcher->removeEventListener(listener);
	}
	return true;
}
void LookPocker::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (isControl)
	{
		char path[100] = {0};
		auto vecDPocker = QGameManager::getInstance()->getpCardData()->getPokerVec();
		auto pockerType = (PocketSprite::PocketType)(vecDPocker.at(4) & 0xF);
		auto pockerNum = (PocketSprite::PocketNum)(vecDPocker.at(4) >> 4);
		auto pocker = m_vecTouchSprite.at(0);
		if (cuoindex > 5)
		{
			sprintf(path, "res/Pocke/move/%d-%d/%d.png", (int)pockerNum, (int)pockerType, cuoindex-2);
		}
		else
			sprintf(path, "res/Pocke/move/%d.png", cuoindex);
		pocker->setTexture(path);
		Vec2 beginPoint = touch->getLocation();
		Vec2 endPoint = touch->getPreviousLocation();
		Point offSet = beginPoint - m_beginPoint;
		log("onTouchMoved======offSet.y======%f", offSet.y);
		if (offSet.y >= 30)
		{
			m_beginPoint = beginPoint;
			cuoindex++;
			if (cuoindex >= 9)
			{
				cuoindex = 9;
			}
			if (cuoindex > 5)
			{
				sprintf(path, "res/Pocke/move/%d-%d/%d.png", (int)pockerNum, (int)pockerType, cuoindex-2);
			}
			else
				sprintf(path, "res/Pocke/move/%d.png", cuoindex);
			pocker->setTexture(path);
		}
		else if (offSet.y<=-30)
		{
			m_beginPoint = beginPoint;
			cuoindex--;
			if (cuoindex <= 1)
			{
				cuoindex = 1;
			}
			if (cuoindex > 5)
			{
				sprintf(path, "res/Pocke/move/%d-%d/%d.png", (int)pockerNum, (int)pockerType, cuoindex-2);
			}
			else
				sprintf(path, "res/Pocke/move/%d.png", cuoindex);
			pocker->setTexture(path);
		}
		
	}
}
void LookPocker::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	isControl = false;
	if (cuoindex > 3)
	{
		isShow = true;
		auto vecDPocker = QGameManager::getInstance()->getpCardData()->getPokerVec();
		auto pockerType = (PocketSprite::PocketType)(vecDPocker.at(4) & 0xF);
		auto pockerNum = (PocketSprite::PocketNum)(vecDPocker.at(4) >> 4);
		auto animation = CCAnimation::create();
		for (int i = cuoindex; i <= 9; i++)
		{
			char path[32] = { 0 };
			if (i > 5)
			{
				sprintf(path, "res/Pocke/move/%d-%d/%d.png", (int)pockerNum, (int)pockerType, i-2);
			}
			else
				sprintf(path, "res/Pocke/move/%d.png", i);
			animation->addSpriteFrameWithFile(path);
		}
		animation->setDelayPerUnit(0.5f / 10.0f);         //0.5秒内实现10帧的播放
		animation->setRestoreOriginalFrame(false);
		animation->setLoops(1);
		auto animate = Animate::create(animation);
		m_vecTouchSprite.at(0)->runAction(Sequence::create(animate, CallFunc::create(
			[=]{
			char szName[100] = { 0 };
			sprintf(szName, "Pocke/HPocker/%d-%d.png", (int)pockerNum, (int)pockerType);
			m_vecTouchSprite.at(0)->setTexture(szName);
		}), DelayTime::create(0.5f), CallFunc::create([=]{ ShowPocker(); }), nullptr));
	}
	cuoindex = 1;
}
void LookPocker::ShowPocker()
{
	_eventDispatcher->removeEventListener(listener);
	destroyLayer();
}
void LookPocker::destroyLayer()
{
	QGameLayer* layer = (QGameLayer*)m_parentLayer;
	this->removeFromParentAndCleanup(true);
	layer->setLookLayer();
	layer->showLastCard();
}

void LookPocker::onExit()
{
	Layer::onExit();
}

void LookPocker::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	if (isShow)
	{
		ShowPocker();
	}
	else
	{
		_eventDispatcher->removeEventListener(listener);
		QGameLayer* layer = (QGameLayer*)m_parentLayer;
		this->removeFromParentAndCleanup(true);
		layer->setLookLayer();
	}
}

void LookPocker::touchPocketEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	auto curButton = (ImageView*)sender;
	curButton->setTouchEnabled(false);
	int index = curButton->getTag();
	auto vecDPocker = QGameManager::getInstance()->getpCardData()->getPokerVec();
	char szName[100] = { 0 };
	if (touchIndex == 2)
	{
		touchIndex = 0;
		this->ShowPocker();
		return;
	}
	auto layer = (QGameLayer*)m_parentLayer;
	auto pockerType = (PocketSprite::PocketType)(vecDPocker.at(4) & 0xF);
	auto pockerNum = (PocketSprite::PocketNum)(vecDPocker.at(4) >> 4);
	sprintf(szName, "Pocke/%d-%d.png", (int)pockerNum, (int)pockerType);
	m_panelPos[index]->loadTexture(szName);

	touchIndex++;

}






