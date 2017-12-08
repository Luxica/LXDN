
#include "WWConfigManager.h"
WWConfigManager* WWConfigManager::m_pInstance = nullptr;
WWConfigManager::WWConfigManager() :
		m_Dictionary(NULL)
{

}

WWConfigManager::~WWConfigManager() {
	CC_SAFE_RELEASE_NULL(m_Dictionary);
}

void WWConfigManager::loadXML(const char *pFileName) {
	CC_SAFE_RELEASE(m_Dictionary);
	m_Dictionary = __Dictionary::createWithContentsOfFile(pFileName);
	CC_SAFE_RETAIN(m_Dictionary);
}

const __String* WWConfigManager::getValue(const char *pKey) {
	if(m_Dictionary == NULL||!pKey ){
		return __String::create("");
	}
	__String *c_Value = (__String*)m_Dictionary->objectForKey(pKey);
	if(c_Value) return c_Value;
	return __String::create("");
}