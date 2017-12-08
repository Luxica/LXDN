#pragma once
#include "json/document.h"
#include <string>
using namespace std;

class ProceConfig
{

public:

	class CGarbo
	{
	public:
		~CGarbo();
	};

	static ProceConfig&			getInstance();			// 获取单例

	~ProceConfig();

	void SaveUserToFile(int uid, int date);
	void LoadUserData(int uid);

private:
	ProceConfig();

	ProceConfig(ProceConfig&);
	ProceConfig& operator = (const ProceConfig&);

	void loadConfig();

public:
	bool				m_bShowStartScene;				//是否显示开始界面
	bool				m_bShowNews;					//首次登陆大厅是否显示公告界面
	bool				m_bShowInviteCode;				// 首次登陆大厅是否显示邀请码界面

	bool				m_bHaveShop;					//是否有商城

	float				m_fSendPoker;					// 发牌时间
	float				m_fRobBanker;					// 抢庄时间
	float				m_fRandBanker;					// 随机庄时间
	float				m_fUserBet;						// 下注时间
	float				m_fOpenPoker;					// 开牌时间
	float				m_fGameResult;					// 小结时间
	float				m_fGameWait;					// 等待时间

	bool				m_bAutoReady;					//是否自动发送准备

	rapidjson::Document m_dcmData;
    
	string			m_uid;
	int             m_date;
	
private:
	static ProceConfig* s_pTheProceConfig;				// 单例

	static CGarbo theGarbo;
};
#define PROCECONFIG  ProceConfig::getInstance()
