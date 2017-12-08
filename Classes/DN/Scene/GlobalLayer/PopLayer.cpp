#include "PopLayer.h"
#include "../../../GameConfig.h"

PopLayer::PopLayer()
{
	m_contentPadding = 0;
	m_contentPaddingTop = 0;
	m_ltContentText = nullptr;

}

//释放
PopLayer::~PopLayer()
{

}

//初始化
bool PopLayer::init()
{
	if (!CCLayer::init()){
		return false;
	}

	this->setContentSize(Size(0, 0));

	Size winSize = Director::sharedDirector()->getWinSize();
	Point center = ccp(winSize.width / 2, winSize.height / 2);
	Size contentSize;
	//初始化需要的Menu
	//m_ltContentText = LabelTTF::create("", "Arial", 24);
	//m_ltContentText->setColor(Color3B::BLACK);
	//m_ltContentText->setPosition(center);
	//m_ltContentText->setDimensions(CCSizeMake(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop));
	//m_ltContentText->setHorizontalAlignment(kCCTextAlignmentLeft);
	//this->addChild(m_ltContentText, 10);

	return true;
}

//给文本变量赋值
void PopLayer::setContentText(const std::string& text, int fontsize, int padding, int paddingTop)
{
	if (!m_ltContentText)
	{
		m_ltContentText = LabelTTF::create(text, "Arial", fontsize);
	}
	
	m_ltContentText->setColor(Color3B::BLACK);
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		m_ltContentText->setColor(Color3B::YELLOW);
		m_ltContentText->enableShadow(Size(2, -2), 50, 0, true);
	}
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;

	Size winSize = Director::sharedDirector()->getWinSize();
	Point center = ccp(winSize.width / 2, winSize.height / 2);
	Size contentSize;
	m_ltContentText->setDimensions(CCSizeMake(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop));
	m_ltContentText->setHorizontalAlignment(kCCTextAlignmentLeft);

}

//销毁弹出框，传递参数node给下层
void PopLayer::stopCallBack()
{
	this->removeFromParentAndCleanup(true);
}

//全部参数都设定好后，在运行时动态加载
void PopLayer::onEnter()
{
	Layer::onEnter();

	Size winSize = Director::sharedDirector()->getWinSize();
	Point center = ccp(winSize.width / 2, winSize.height / 2);
	Size contentSize;
	//弹出效果
	if (m_ltContentText) {
		m_ltContentText->setPosition(center);
		m_ltContentText->setDimensions(CCSizeMake(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop));
		m_ltContentText->setHorizontalAlignment(kCCTextAlignmentLeft);
		this->addChild(m_ltContentText);

		Sequence *popupActions = Sequence::create(
			ScaleTo::create(0.1, 1.5),
			MoveBy::create(0.8, Vec2(0, 50.0)),
			//CallFunc::create(std::bind(&PopLayer::stopCallBack, this)),
			RemoveSelf::create(),
			NULL);
		m_ltContentText->runAction(popupActions);
	}
}

//退出
void PopLayer::onExit()
{
	Layer::onExit();
}