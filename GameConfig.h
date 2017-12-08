#pragma once
#include "json/document.h"
#include <string>
using namespace std;

#define GAME_TYPE_KXNNL 1	//开心牛牛乐

class GameConfig
{

public:

	class CGarbo
	{
	public:
		~CGarbo();
	};

	static GameConfig&			getInstance();			// 获取单例

	~GameConfig();

	void SaveUserToFile(int uid, int date);
	void LoadUserData(int uid);

private:
	GameConfig();

	GameConfig(GameConfig&);
	GameConfig& operator = (const GameConfig&);

	void loadConfig();

public:
	bool			m_bNetTest;
	bool			m_bAutoTest;
	string			m_sTcpAddr;						// 游戏地址
	int				m_nTcpPort;						// 大厅服务器端口

	string			m_sHttpHost;						// HTTP服务地址
	string			m_sHttpVer;							// HTTP协议版本
	string			m_sHttpApk;							// apk下载
	string			m_sHttpIpa;							//ipa下载
    string			m_sNewsUrl;							// 公告下载地址
    string			m_sProxy1Url;						// 代理须知下载地址
    string			m_sProxy2Url;						// 代理加盟地址
    string          m_sHttpDownUrl;                     // 微信分享下载地址

	string			m_sMagicWindowMLink;	

    string          m_sAppName;                         //app名称
    int             m_nAppType;                         //1 热血牛牛 2 天天 

	string			m_socket_ip;
	string			m_domain_key;
	int				m_socket_ip_ports;
	string			m_play_type_id;
	string			m_game_version;
	string			m_user_type;
	string			m_user_sex;
	string			m_is_show_debug_info;

	float			m_actionTime;
	float			m_scheduleTime;
	int				m_max_num;
	rapidjson::Document m_dcmData;
    
	string			m_uid;
	int             m_date;
	
private:
	static GameConfig* s_pTheGameConfig;				// 单例

	static CGarbo theGarbo;
};
#define GAMECONFIG  GameConfig::getInstance()
