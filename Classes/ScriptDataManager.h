#pragma once
#include "cocos2d.h"
#include <sstream>
USING_NS_CC;

class ScriptDataManager
{
public:
	ScriptDataManager(void);
	ScriptDataManager(std::string kFileName);
	~ScriptDataManager(void);
public:
	static ScriptDataManager* getInterface();

public:
    std::string getGameString(const std::string &key);
    void init();
private:
    std::map<std::string, std::string> _stringDataDic;
    
};

#define GetGameString(key) ScriptDataManager::getInterface()->getGameString(key).c_str()
