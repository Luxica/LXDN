#include "Speek.h"
#include "cocostudio/CocoStudio.h"
#include "ScriptDataManager.h"
#include "../../../../GameConfig.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace cocos2d;

SpeekLayer* SpeekLayer::create(int type,int chatId)
{
	SpeekLayer*pRet = new SpeekLayer();
	if (pRet && pRet->init(type,chatId))
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
bool SpeekLayer::init()
{

    if ( !Layer::init() )
    {
        return false;
    }
	this->setType();
 	return true;
}
bool SpeekLayer::init(int type, int chatId)
{
	if (!Layer::init())
	{
		return false;
	}
	this->setType(type,chatId);
	return true;
}
void SpeekLayer::DestroySpesk()
{
	this->removeFromParentAndCleanup(true);
}
Animate* SpeekLayer::SpeakAction()
{
	auto animation = Animation::create();
	for (int i = 1; i < 5; i++)
	{
		char szName[100] = { 0 };
		sprintf(szName, "res/speak/%d.png", i);
		animation->addSpriteFrameWithFile(szName);
	}
	// 控制帧动画的每帧间的播放间隔
	animation->setDelayPerUnit(0.2f);
	// 是否在播放后重置成第一张图片
	animation->setRestoreOriginalFrame(true);
	auto action_Speak = Animate::create(animation);

	return action_Speak;
}

void SpeekLayer::setType(int type,int chatId)
{
	Text *text = Text::create();
	text->setAnchorPoint(Vec2(0.0f, 0.5f));
	text->setContentSize(Size(600, 200));
	text->setFontSize(44.0f);
	if (GAMECONFIG.m_nAppType == 2)
	{
		text->setColor(Color3B::BLACK);
	}
	char chatText[32] = { 0 };
	ImageView *bg = ImageView::create("res/speak/speekuang.png");
	bg->setAnchorPoint(Vec2(0.0f, 0.5f));
	bg->setScale9Enabled(true);
	if (GAMECONFIG.m_nAppType == 4)
	{
		bg->setOpacity(GLubyte(180));
	}
    if (type == -1)
    {
        auto speakSprite = Sprite::create("res/speak/1.png");
        this->addChild(speakSprite);       
        speakSprite->runAction(RepeatForever::create(SpeakAction()));
          
    }
	else if (type == 1)
    {
		sprintf(chatText, "chat_%d", chatId);
		text->setString(GetGameString(chatText));
		text->setTextHorizontalAlignment(TextHAlignment::LEFT);
        Size textsize = text->getVirtualRenderer()->getContentSize();
        bg->setContentSize(Size(textsize.width + 15 ,textsize.height + 8));
		text->setPosition(Vec2(15.0f, 0.0f));
		this->addChild(bg);
		this->addChild(text);
	}
	else
	{
		sprintf(chatText, "chat_%d", chatId);
		text->setString(GetGameString(chatText));
		text->setTextHorizontalAlignment(TextHAlignment::RIGHT);
		Size textsize = text->getVirtualRenderer()->getContentSize();	
		bg->setContentSize(Size(textsize.width + 15, textsize.height + 8));
		bg->setFlipX(true);
		text->setPosition(Vec2(0 - (bg->getContentSize().width)+10.0f, 0.0f));
		this->addChild(bg);
		this->addChild(text);
	}

}




