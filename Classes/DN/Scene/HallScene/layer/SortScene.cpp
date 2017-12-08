#include "SortScene.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Scene/HallScene/MainMenu.h"
#include "../../../Scene/GlobalLayer/Load.h"
#include "../../../../DN/Comm/request/MsgHallRequest.h"
#include "../../../../StringTool.h"
#include "../../../../ScriptDataManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "WebSprite.h"
#else
#include "GameNode/WebSprite.h"
#endif

//#include "MainMenuScene\MainMenu.h"
//#include "Macro.h"


USING_NS_CC;

using namespace cocostudio::timeline;

Scene* SortScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = SortScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SortScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	rootNode = CSLoader::createNode("RankingScene.csb");
	if (rootNode)
	{
		addChild(rootNode);
		listView = dynamic_cast<ListView*>(rootNode->getChildByName("ListView_Today"));
		listView->setScrollBarOpacity(GLubyte(0));
        //m_signleTempLayout = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_1"));
        //m_signleTempLayout->setVisible(false);   
        if (listView)
		{
			listView->setScrollBarOpacity(GLubyte(0));
			vecPanel = Vector<Node*>(listView->getChildren());
            //listView->removeAllItems();       
		}

		today_Layout = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Today"));
		tomorrow_Layout = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Tomorrow"));
        
        auto name_panel = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Name"));
        m_selfNameText = dynamic_cast<Text*>(name_panel->getChildByName("Text_Name"));
        m_selfHeadLayout = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_head"));
        m_selfWinText = dynamic_cast<Text*>(rootNode->getChildByName("Text_Shengfu"));
        m_selfScoreText = dynamic_cast<Text*>(rootNode->getChildByName("Text_Score"));
        m_selfSortText = dynamic_cast<Text *>(rootNode->getChildByName("Text_Sort"));
		if (tomorrow_Layout)
		{
			tomorrow_Layout->setVisible(false);
		}

		tsWeek_Layout = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Tswk"));
		if (tsWeek_Layout)
		{
            tsWeek_Layout->setVisible(false);
		}

		lastWeek_Layout = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_LastWeek"));
		if (lastWeek_Layout)
		{
			lastWeek_Layout->setVisible(false);
		}

		//关闭按钮
		auto backButton = dynamic_cast<Button*>(rootNode->getChildByName("Button_Back"));
		if (backButton)
		{
			//事件回调   按钮做什么事
			backButton->addTouchEventListener(CC_CALLBACK_2(SortScene::touchBackEvent, this));
		}

		auto todayButton = dynamic_cast<Button*>(rootNode->getChildByName("Button_today"));
		if (todayButton)
		{
            todayButton->setTag(0);
			todayButton->addTouchEventListener(CC_CALLBACK_2(SortScene::touchRankListEvent, this));
		}

		auto tomorrowButton = dynamic_cast<Button*>(rootNode->getChildByName("Button_tomorrow"));
		if (tomorrowButton)
		{
            tomorrowButton->setTag(1);
			tomorrowButton->addTouchEventListener(CC_CALLBACK_2(SortScene::touchRankListEvent, this));
		}

		auto tsWeekButton = dynamic_cast<Button*>(rootNode->getChildByName("Button_TswK"));
		if (tsWeekButton)
		{
            tsWeekButton->setTag(2);
			tsWeekButton->addTouchEventListener(CC_CALLBACK_2(SortScene::touchRankListEvent, this));
		}

		auto lastWeekButton = dynamic_cast<Button*>(rootNode->getChildByName("Button_LastWeek"));
		if (lastWeekButton)
		{
            lastWeekButton->setTag(3);
			lastWeekButton->addTouchEventListener(CC_CALLBACK_2(SortScene::touchRankListEvent, this));
		}
		
	}

	//开起转圈圈界面
	//WWSceneManager::getInstance()->openGlobalUI(Load::create());

    return true;
}

void SortScene::updateRank()
{
    if (rootNode == nullptr) return;
    
	auto rankDate = QHallManager::getInstance()->getRankData();
	auto rankUserVec = rankDate->getRankUserVec();
	char szCountNum[128] = { 0 };
	int sizeVec = rankUserVec.size();
    m_RankCounts = sizeVec;
	
    listView->setVisible(true);
    
    auto  myselfData = rankUserVec.at(sizeVec - 1);
    int selfId = myselfData->getUserId();
    bool isIn = false;
    m_startDownIndex = 0;
    WebSprite* firstWeb = nullptr;
    bool isDown = false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    isDown = false;
#else
    isDown = false;
#endif
    
    char szTemp [32] = {0};
	for (int i = 0; i < 20; i++)
	{
        sprintf(szTemp,"Panel_%d",i + 1);
        auto temp = dynamic_cast<Layout*>(listView->getChildByName(szTemp));
        if (sizeVec-1 >= i )
        {
            temp->setVisible(true);
            auto singleData = rankUserVec.at(i);
            auto userHead = dynamic_cast<Layout*>(temp->getChildByName("Panel_head"));
            userHead->removeAllChildren();
            
            auto pPortrait = WebSprite::create(singleData->getImageName(), Size(148.0f, 158.0f),isDown);
            pPortrait->setLocalZOrder(-1);
            userHead->addChild(pPortrait);
            pPortrait->setTag(100);
            pPortrait->setName(singleData->getNickName());
            pPortrait->setPosition(userHead->getContentSize().width / 2, userHead->getContentSize().height / 2);
            if (i == 0 && isDown == false )
            {
               firstWeb = pPortrait;
            }
            
            auto name_panel = dynamic_cast<Layout*>(temp->getChildByName("Panel_Name"));
            auto textName = dynamic_cast<Text*>(name_panel->getChildByName("Text_Name"));
            textName->setString(singleData->getNickName());

            auto winner = dynamic_cast<Text*>(temp->getChildByName("Text_Shengfu"));
            sprintf(szCountNum, GetGameString("rank_winNum"), singleData->getWinCount(), singleData->getLoseCount());
            winner->setString(szCountNum);
            auto textScore = dynamic_cast<Text*>(temp->getChildByName("Text_Score"));
            textScore->setString(StringTool::intToString(singleData->getScore()));
            auto sortAtlaslabel = dynamic_cast<TextAtlas*>(temp->getChildByName("AtlasLabel_Sort"));
            auto sortImage = static_cast<ImageView*>(temp->getChildByName("Image_Sort"));
            if (i <= 2)
            {
                sortImage->setVisible(true);
                sprintf(szCountNum, "res/ranking/king/%d.png", i + 1);
                sortImage->loadTexture(szCountNum);
                sortAtlaslabel->setVisible(false);
            }
            else
            {	
                sprintf(szCountNum, "%2d", i+1);
                sortAtlaslabel->setVisible(true);
                sortAtlaslabel->setString(szCountNum);
                sortImage->setVisible(false);
            }
            int friendId = singleData->getUserId();
            if (selfId == friendId)
            {
                //已经入围
                isIn = true;
            }
        }else {
            temp->setVisible(false);
        }
	}
    
    if (firstWeb != nullptr)
    {
        firstWeb->startUpdate([&](){
            m_startDownIndex ++ ;
            updateRankUrlIcon();
        });
    }
	//显示头像
	m_selfHeadLayout->removeAllChildren();

	auto pPortrait = WebSprite::create(myselfData->getImageName(), Size(148.0f, 158.0f));
	pPortrait->setLocalZOrder(-1);
	m_selfHeadLayout->addChild(pPortrait);
	pPortrait->setPosition(m_selfHeadLayout->getContentSize().width / 2, m_selfHeadLayout->getContentSize().height / 2);

	m_selfNameText->setString(myselfData->getNickName());

    sprintf(szCountNum,"%d",myselfData->getScore());
	m_selfScoreText->setString(szCountNum);
    sprintf(szCountNum, GetGameString("rank_winNum"), myselfData->getWinCount(), myselfData->getLoseCount());
    m_selfWinText->setString(szCountNum);
    m_selfSortText->setString(isIn ?GetGameString("inRank"):GetGameString("notRank"));
}

void SortScene::updateRankUrlIcon()
{
    if (listView != nullptr && listView->getItems().size() > m_startDownIndex)
    {
        Layout *item = dynamic_cast<Layout*>(listView->getItem(m_startDownIndex));
        Layout *userHead = dynamic_cast<Layout*>(item->getChildByName("Panel_head"));
        auto webIcon = dynamic_cast<WebSprite*>(userHead->getChildByTag(100));
        if (webIcon != nullptr)
        {
            webIcon->startUpdate([&](){
                m_startDownIndex ++ ;
                updateRankUrlIcon();
            });
        }
    }
}
    
void SortScene::touchRankListEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED)
        return;
    
    int tag = ((Button*)sender)->getTag();
    
    GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
    today_Layout->setVisible(tag == 0);
    tomorrow_Layout->setVisible(tag == 1);
    tsWeek_Layout->setVisible(tag == 2);
    lastWeek_Layout->setVisible(tag == 3);
    MsgHallRequest::sendGetRank(tag);
}

void SortScene::touchBackEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
    this->removeFromParentAndCleanup(true);
    rootNode = nullptr;
}
