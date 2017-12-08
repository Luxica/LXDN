
#include "cocos2d.h"
#include "GameConfig.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "StringTool.h"
USING_NS_CC;
using namespace  rapidjson;


static const char ConfigFile[] = "res/config.json";
static const char DataFile[] = "res/Data.json";

GameConfig::GameConfig() :
	m_nTcpPort(39000),
	m_sHttpHost(""),
	m_sHttpVer("")
{

}

GameConfig::GameConfig(GameConfig& theConfig) :
	m_nTcpPort(theConfig.m_nTcpPort),
	m_sHttpHost(theConfig.m_sHttpHost),
	m_sHttpVer(theConfig.m_sHttpVer)
{

}

GameConfig& GameConfig::operator=(const GameConfig& theConfig)
{
	return *this;
}

void GameConfig::SaveUserToFile(int uid,int date)
{
	//rapidjson::Document oWriteDoc;
	//oWriteDoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = m_dcmData.GetAllocator();
	rapidjson::Value oArray(rapidjson::kArrayType);
	string strUID = StringTool::intToString(uid);

	m_dcmData.RemoveMember(strUID.c_str());
	m_dcmData.AddMember(rapidjson::StringRef(strUID.c_str()), date, allocator);
	//m_dcmData.AddMember("month", month, allocator);
	//m_dcmData.AddMember("day", 22, allocator);

	rapidjson::StringBuffer oBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> oWriter(oBuffer);
	m_dcmData.Accept(oWriter);

	//写入
	FILE* pFile = fopen(DataFile, "wb+");
	if (pFile == NULL)
	{
		log("Open File '%s' Failed.", DataFile);
		return;
	}

	fputs(oBuffer.GetString(), pFile);
	fclose(pFile);

}
void GameConfig::LoadUserData(int uid)
{
	//if (FileUtils::getInstance()->isFileExist(DataFile))
	//{
	//	//FileUtils::getInstance()->purgeCachedEntries();
	//	//FileUtils::getInstance()->destroyInstance();
	//	std::string jsonStr = FileUtils::getInstance()->getStringFromFile(DataFile);
	//	FileUtils::getInstance()->getWritablePath();
	//	//rapidjson::Document dcmt;
	//	m_dcmData.Parse<0>(jsonStr.c_str());
	//	if (m_dcmData.HasMember(StringTool::intToString(uid).c_str()))
	//		m_date = m_dcmData[StringTool::intToString(uid).c_str()].GetInt();
	//	else
	//		m_date = 0;

	//}

	//*** 读取 json 文件 ***  
	bool bRet = false;
	ssize_t size = 0;
	std::string load_str;

	// getFileData 如果不指定，读取根目录是 Resource 文件夹  
	unsigned char* titlech = FileUtils::getInstance()->getFileData(DataFile, "r", &size);
	load_str = std::string((const char*)titlech, size);

	//load_str = cocos2d::FileUtils::getInstance()->getStringFromFile("..\\myhero.json");  
	m_dcmData.Parse<0>(load_str.c_str());
	if (m_dcmData.HasParseError())
	{
		CCLOG("GetParseError%s\n", m_dcmData.GetParseError());
	}

	if (!m_dcmData.IsObject())
		return;
	if (m_dcmData.HasMember(StringTool::intToString(uid).c_str()))
		m_date = m_dcmData[StringTool::intToString(uid).c_str()].GetInt();
	else
		m_date = 0;
		
}
void GameConfig::loadConfig()
{
	if (FileUtils::getInstance()->isFileExist(ConfigFile))
	{
		std::string jsonStr = FileUtils::getInstance()->getStringFromFile(ConfigFile);
        rapidjson::Document dcmt;
        dcmt.Parse<0>(jsonStr.c_str());

		m_bAutoTest = dcmt["AutoTest"].GetBool();
		m_bNetTest = dcmt["NetTest"].GetBool();
		m_sTcpAddr = dcmt["TcpAddr"].GetString();
		m_nTcpPort = dcmt["TcpPort"].GetInt();
        m_sHttpHost = dcmt["HttpHost"].GetString();
		m_sHttpVer = dcmt["HttpVer"].GetString();
		m_sHttpApk = dcmt["HttpApk"].GetString();
		m_sHttpIpa = dcmt["HttpIpa"].GetString();
        m_sNewsUrl = dcmt["NewsUrl"].GetString();
        m_sProxy1Url = dcmt["Proxy1Url"].GetString();
        m_sProxy2Url = dcmt["Proxy2Url"].GetString();
        m_sHttpDownUrl = dcmt["HttpDownUrl"].GetString();
        m_sAppName = dcmt["AppName"].GetString();
        m_nAppType = dcmt["AppType"].GetInt();

		m_socket_ip = dcmt["socket_ip"].GetString();
		m_domain_key = dcmt["domain_key"].GetString();
		m_socket_ip_ports = dcmt["socket_ip_ports"].GetInt();
		m_play_type_id = dcmt["play_type_id"].GetString();
		m_game_version = dcmt["game_version"].GetString();
		m_user_type = dcmt["user_type"].GetString();
		m_user_sex = dcmt["user_sex"].GetString();
		m_is_show_debug_info = dcmt["is_show_debug_info"].GetString();
		
		m_actionTime = dcmt["actionTime"].GetDouble();
		m_scheduleTime = dcmt["scheduleTime"].GetDouble();
		m_max_num = dcmt["maxNum"].GetInt();
	}
	if (FileUtils::getInstance()->isFileExist(DataFile))
	{
		std::string jsonStr = FileUtils::getInstance()->getStringFromFile(DataFile);
		//rapidjson::Document dcmt;
		m_dcmData.Parse<0>(jsonStr.c_str());

		//m_Year = dcmt["year"].GetInt();
		//m_Month = dcmt["month"].GetInt();
		//m_Day = dcmt["day"].GetInt();
	}
}

GameConfig& GameConfig::getInstance()
{
	if (!s_pTheGameConfig)
	{
		s_pTheGameConfig = new GameConfig;
		s_pTheGameConfig->loadConfig();
	}

	return *s_pTheGameConfig;
}



GameConfig::~GameConfig()
{
}

GameConfig* GameConfig::s_pTheGameConfig = NULL;

GameConfig::CGarbo GameConfig::theGarbo;

GameConfig::CGarbo::~CGarbo()
{
	if (GameConfig::s_pTheGameConfig)
	{
		delete GameConfig::s_pTheGameConfig;
		GameConfig::s_pTheGameConfig = NULL;
	}
}
