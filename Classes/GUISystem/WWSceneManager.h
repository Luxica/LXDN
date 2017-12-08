#ifndef WWSCENEMANAGER_H_
#define WWSCENEMANAGER_H_

#include "cocos2d.h"
using namespace cocos2d;


class WWSceneManager : public Ref {
public:
	static WWSceneManager* getInstance();

	static void destoryInstance();

	/*
	 * 判断当前的全局UI里面是否存节点，此节点在加入之前设置过Name或是Tag值
	 */
	bool hasUIwithName(const std::string &ui_name);
	bool hasUIwithTag(int tag);

	//退出
	void exit();

	bool init();

	//打开场景
	void openScene(Scene* pScene);

	//打开全局UI
    void openGlobalUI(Node* pNode);

	//关闭全局UI
	void closeGlobalUI(Node* pNode);

	void closeGlobalUIByName(const std::string &name);
	void closeGlobalUIByTag(int tag);

	cocos2d::Node *getGlobalUIByName(const std::string &name);
	cocos2d::Node *getGlobalUIByTag(int tag);


	//关闭所有弹窗
	void closeAllUI();
    
	//窗口大小
	Size getWinSize();

  
    //返回弹框数量
    ssize_t getNumOfGlobalUI();
    
    //返回最后一个添加的弹框
    Node * getLastNode();

    Scene*  getGlobalScene();

private:
	WWSceneManager();
	virtual ~WWSceneManager();

	//第二渲染树根场景
	Scene*    m_pSence;

	//单例
	static WWSceneManager* m_Instance;
};

#endif /* WWSCENEMANAGER_H_ */
