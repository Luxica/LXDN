
#include "ScriptDataManager.h"
#include "json/document.h"

ScriptDataManager::ScriptDataManager()
{

}

ScriptDataManager::~ScriptDataManager()
{
}

ScriptDataManager* ScriptDataManager::getInterface()
{
	static ScriptDataManager s_pInterface;
	return &s_pInterface;
}

void ScriptDataManager::init()
{
    std::string path = "";
    std::string data = "";
    path = FileUtils::getInstance()->fullPathForFilename("res/Script/GameTipStringInfo.json");
    data = FileUtils::getInstance()->getStringFromFile(path);
    
    log("load string :%s",path.c_str());
    
    rapidjson::Document dcmt;
    dcmt.Parse<0>(data.c_str());
    if (dcmt.HasParseError()) {
        CCLOG("GetParseError %s\n", dcmt.GetParseError());
    }
    
    for (rapidjson::Value::ConstMemberIterator itr = dcmt.MemberBegin(); itr != dcmt.MemberEnd(); itr++) {
        //chs->retain();
		_stringDataDic[itr->name.GetString()] = itr->value.GetString();
    }
}

std::string ScriptDataManager::getGameString(const std::string &key)
{
	auto iter = _stringDataDic.find(key);
	if (iter != _stringDataDic.end()) {
		return iter->second;
	}
	log("get error string for %s", key.c_str());
	return key;
}
