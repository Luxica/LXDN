
#include "cocos2d.h"
#include "ProceConfig.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "StringTool.h"
USING_NS_CC;
using namespace  rapidjson;


static const char ConfigFile[] = "res/proceConfig.json";
//static const char DataFile[] = "res/Data.json";

ProceConfig::ProceConfig()/* :
m_nTcpPort(39000),
m_sHttpHost(""),
m_sHttpVer("")*/
{

}

ProceConfig::ProceConfig(ProceConfig& theConfig)/* :
	m_nTcpPort(theConfig.m_nTcpPort),
	m_sHttpHost(theConfig.m_sHttpHost),
	m_sHttpVer(theConfig.m_sHttpVer)*/
{

}

ProceConfig& ProceConfig::operator=(const ProceConfig& theConfig)
{
	return *this;
}

void ProceConfig::SaveUserToFile(int uid, int date)
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

	////写入
	//FILE* pFile = fopen(DataFile, "wb+");
	//if (pFile == NULL)
	//{
	//	log("Open File '%s' Failed.", DataFile);
	//	return;
	//}

	//fputs(oBuffer.GetString(), pFile);
	//fclose(pFile);

}
void ProceConfig::LoadUserData(int uid)
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

	//// getFileData 如果不指定，读取根目录是 Resource 文件夹  
	//unsigned char* titlech = FileUtils::getInstance()->getFileData(DataFile, "r", &size);
	//load_str = std::string((const char*)titlech, size);

	////load_str = cocos2d::FileUtils::getInstance()->getStringFromFile("..\\myhero.json");  
	//m_dcmData.Parse<0>(load_str.c_str());
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
void ProceConfig::loadConfig()
{
	if (FileUtils::getInstance()->isFileExist(ConfigFile))
	{
		std::string jsonStr = FileUtils::getInstance()->getStringFromFile(ConfigFile);
        rapidjson::Document dcmt;
        dcmt.Parse<0>(jsonStr.c_str());

		m_bShowStartScene = dcmt["showStartScene"].GetBool();
		m_bShowNews = dcmt["showNews"].GetBool();
		m_bShowInviteCode = dcmt["showInviteCode"].GetBool();
		m_bHaveShop = dcmt["haveShop"].GetBool();

		m_fSendPoker = dcmt["send_poker"].GetDouble();
		m_fRobBanker = dcmt["rob_banker"].GetDouble();
		m_fRandBanker = dcmt["rand_banker"].GetDouble();
		m_fUserBet = dcmt["user_bet"].GetDouble();
		m_fOpenPoker = dcmt["open_poker"].GetDouble();
		m_fGameResult = dcmt["game_result"].GetDouble();
		m_fGameWait = dcmt["game_wait"].GetDouble();

		m_bAutoReady = dcmt["autoReady"].GetBool();

	}
}

ProceConfig& ProceConfig::getInstance()
{
	if (!s_pTheProceConfig)
	{
		s_pTheProceConfig = new ProceConfig;
		s_pTheProceConfig->loadConfig();
	}

	return *s_pTheProceConfig;
}



ProceConfig::~ProceConfig()
{
}

ProceConfig* ProceConfig::s_pTheProceConfig = NULL;

ProceConfig::CGarbo ProceConfig::theGarbo;

ProceConfig::CGarbo::~CGarbo()
{
	if (ProceConfig::s_pTheProceConfig)
	{
		delete ProceConfig::s_pTheProceConfig;
		ProceConfig::s_pTheProceConfig = NULL;
	}
}
