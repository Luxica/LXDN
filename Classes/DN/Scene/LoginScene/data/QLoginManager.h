#ifndef __QLOGIN_MANAGER_H__
#define __QLOGIN_MANAGER_H__

#include "cocos2d.h"
using namespace cocos2d;

class QLoginManager
{
private:
	QLoginManager();
	~QLoginManager();

public:
	static QLoginManager* getInstance()
	{
		static QLoginManager m_pInstance;
		return &m_pInstance;
	}

public:
	void dealGetType(Ref *obj);
	void dealGetWay(Ref *obj);
public:
	CC_SYNTHESIZE(bool, m_isTourist_, Tourist); //游客
	CC_SYNTHESIZE(std::string, m_sHallIp_, HallIp); //大厅ip
	CC_SYNTHESIZE(unsigned short, m_nHallPort_, HallPort);	//大厅port
	CC_SYNTHESIZE(std::string, m_sAccount_, Account); //账号
	CC_SYNTHESIZE(std::string, m_sPassWord_, PassWord); //密码
    CC_SYNTHESIZE(unsigned int, m_loginType, LoginType); //是否重新微信登录
};

#endif
