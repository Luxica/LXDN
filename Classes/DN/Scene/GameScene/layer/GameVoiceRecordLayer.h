#pragma once

#include "network/HttpClient.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
using namespace cocos2d::ui;
USING_NS_CC;
class GameVoiceRecordLayer : public cocos2d::Layer
{
public:
	GameVoiceRecordLayer();

	~GameVoiceRecordLayer();

	CREATE_FUNC(GameVoiceRecordLayer)

	virtual bool init();

	virtual void onExit();
    void showDialog(Node *parent);
	void stopRecord();

	void cancelRecord();

private:

	void updateVoiceDb(float dt);
public:

	int				m_nRoomID;

private:

	struct timeval	m_tvBegin;

	int				m_recordTime;

	Node*			m_pRootNode;

	LoadingBar*		m_pLdpVoice;

    Node*           m_pParent;
};
