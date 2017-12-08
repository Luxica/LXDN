#ifndef WW_CONFIGMANAGER_H_
#define WW_CONFIGMANAGER_H_
#include "cocos2d.h"
using namespace cocos2d;

/**
 * 配置文件管理（socket地址、sp等配置的读取）
 */
class WWConfigManager: public Ref {
public:
	WWConfigManager(void);
	virtual ~WWConfigManager(void);

	static WWConfigManager* getInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new WWConfigManager();
		}
		return m_pInstance;
	}


	/*
	 * 载入xml文件
	 */
	void loadXML(const char *pFileName);
	/**
	 * 获取相应的参数值（键在WWConfig.h）
	 */
	const cocos2d::__String* getValue(const char *pKey);
private:
	static WWConfigManager* m_pInstance;
	cocos2d::__Dictionary* m_Dictionary;
};

#endif /* CONFIGMANAGER_H_ */
