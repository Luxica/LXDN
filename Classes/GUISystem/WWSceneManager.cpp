
#include "WWSceneManager.h"

int NotificationZoder=INT_MAX-1;

WWSceneManager* WWSceneManager::m_Instance = nullptr;

WWSceneManager::WWSceneManager() :
m_pSence(nullptr)
{

}

WWSceneManager::~WWSceneManager()
{
	CC_SAFE_RELEASE_NULL(m_pSence);
}

WWSceneManager* WWSceneManager::getInstance()
{
	if (nullptr == m_Instance)
	{
		m_Instance = new WWSceneManager();
		if (m_Instance && m_Instance->init())
		{
			m_Instance->autorelease();
			CC_SAFE_RETAIN(m_Instance);
		}
		else
		{
			CC_SAFE_DELETE(m_Instance);
			m_Instance = nullptr;
		}
	}
	return m_Instance;
}

void WWSceneManager::destoryInstance()
{
	CC_SAFE_RELEASE_NULL(m_Instance);
}

bool WWSceneManager::hasUIwithName(const std::string &ui_name)
{
	if (!m_pSence) {
		return false;
	}

	return (nullptr != m_pSence->getChildByName(ui_name));
}

bool WWSceneManager::hasUIwithTag(int tag)
{
	if (!m_pSence) {
		return false;
	}

	return (nullptr != m_pSence->getChildByTag(tag));
}

void WWSceneManager::exit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("WP操作系统，请自行退出","退出提醒 !!!");
	return;
#endif
    
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	JniUtils::exitGame();
//#endif
//    
	WWSceneManager::destoryInstance();
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	::exit(0);
#endif

}

bool WWSceneManager::init()
{
	bool bRet = false;
	do
	{
		m_pSence = Scene::create();
		CC_BREAK_IF(!m_pSence);
		CC_SAFE_RETAIN(m_pSence);

		bRet = true;
	} while (0);
	return bRet;
}

void WWSceneManager::openScene(Scene* pScene)
{
	CCAssert(pScene != nullptr, "openScene args pScene is NULL");
	Scene* pCurScene = Director::getInstance()->getRunningScene();
	if (nullptr == pCurScene)
	{
        
		//WW_LOG_ERROR("the first add scene");
		//加入伪第二渲染树（用于显示一直存在的节点）
		if (m_pSence)
		{
			//WW_LOG_ERROR(" m_pSence the first add scene");
			pScene->addChild(m_pSence,NotificationZoder);
		}
		Director::getInstance()->runWithScene(pScene);
	}
	else
	{
		if (pCurScene->getName() == pScene->getName()) {
			return;
		}
		if (m_pSence)
		{
			if (nullptr != m_pSence->getParent())
			{
				m_pSence->removeFromParentAndCleanup(false);
			}
			pScene->addChild(m_pSence,NotificationZoder);
		}
		//WW_LOG_INFO("replace scene,Description= %s", pScene->getDescription().c_str());
		Director::getInstance()->replaceScene(pScene);
	}
}

Scene*  WWSceneManager::getGlobalScene()
{
    return m_pSence;
}

void WWSceneManager::openGlobalUI(Node* pNode/*, int tag*/)
{
	CCAssert(pNode != nullptr, "openGlobalUI args pNode is NULL");
    CC_ASSERT(m_pSence && m_pSence->getParent());
	do
	{
		//判断第二渲染树根节点是否为空，为空则跳出
		CC_BREAK_IF(!m_pSence);
		CCLOG("openGlobalUI !m_pSence");
		//判断加入的节点是否为空，为空则跳出
		CC_BREAK_IF(!pNode);
		CCLOG("openGlobalUI !pNode");
		//判断加入的的节点的父节点是否为空，不为空则跳出
		CC_BREAK_IF(pNode->getParent());
		CCLOG("openGlobalUI pNode->getParent()");
		//判断加入的的节点的父节点是否已经存在，有则跳出
		CC_BREAK_IF(m_pSence->getChildren().contains(pNode));
        
        CCLOG("WWSceneManager::openGlobalUI 检测通过");
		m_pSence->addChild(pNode);
	} while (0);
}

void WWSceneManager::closeGlobalUI(Node* pNode)
{
	CCAssert(pNode != nullptr, "closeGlobalUI args pNode is NULL");
	do
	{
		CC_BREAK_IF(!m_pSence);
		CC_BREAK_IF(!pNode);
		m_pSence->removeChild(pNode);
	} while (0);

}

void WWSceneManager::closeGlobalUIByName(const std::string &name)
{
	m_pSence->removeChildByName(name);
}

void WWSceneManager::closeGlobalUIByTag(int tag)
{
	m_pSence->removeChildByTag(tag);
}

Node *WWSceneManager::getGlobalUIByName(const std::string &name)
{
	if (!hasUIwithName(name)) {
		return nullptr;
	}

	return m_pSence->getChildByName(name);
}

Node *WWSceneManager::getGlobalUIByTag(int tag) 
{
	if (!hasUIwithTag(tag)) {
		return nullptr;
	}

	return m_pSence->getChildByTag(tag);
}
											
void WWSceneManager::closeAllUI()
{
	log("WWSceneManager::closeAlllUI");
	do
	{
		CC_BREAK_IF(!m_pSence);
        
		//清空第二渲染树根节点
		m_pSence->removeAllChildren(); 
	} while (0);
}

Size WWSceneManager::getWinSize()
{
	return Director::getInstance()->getWinSize();
}

//返回弹框数量
ssize_t WWSceneManager::getNumOfGlobalUI()
{
    if (m_pSence)
    {
        Vector<Node *> vNode = m_pSence->getChildren();

        return vNode.size() - 1;
    }
    
    return 0;
}

//返回最后一个添加的弹框
Node * WWSceneManager::getLastNode()
{
    if (m_pSence)
    {
        Vector<Node *> vNode = m_pSence->getChildren();
        
        ssize_t size = vNode.size();
        if (size > 0)
        {
            return vNode.at(size - 1);
        }
    }
    return nullptr;
}
