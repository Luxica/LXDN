#ifndef LORDNETMANGER_H_
#define LORDNETMANGER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../NetSystem/WWNetwork.h"
using namespace cocos2d::ui;
using namespace cocos2d;

class LordNetManger : public NetEventDelegate
{
public:
    LordNetManger();
    virtual ~LordNetManger();
    
    static LordNetManger* getInstance();
    
	void send(const int &iType,WWBuffer* buf);
	void sendMessageNow(const int &iType,WWBuffer* buf);

	void connect(const int &iNetType);
	void connectNet(const int &iNetType);
	bool isConnect(const int &iNetType);
	void onDisconnect(const int &iNetType);
	void setReconnect(const int &iNetType,bool isReconnect);
    
    virtual void onExceptionCaught(WWNetStatus eStatus);
	virtual void reConnect(const int &iNetType,float delayTime = 2.f);
    virtual WWBuffer* onRequestLogonInfo();
    
    bool gethadShowDisConnetLayer();
    
    CC_SYNTHESIZE_PASS_BY_REF(WWInetAddress, m_inetAddress, InetAddress);
    
	WWNetwork *getNetWorkObj(const int &iType);

	void onConnectEvent(EventCustom* event);

	void updateConnectEvent(EventCustom* event);

	void touchAgreeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
public:
	//网络模块
	WWNetwork* m_pLoginNetwork_;
	WWNetwork* m_pHallNetwork_;
	WWNetwork* m_pGameNetwork_;
private:
	std::vector< WWInetAddress > genInetAddrs(const std::vector< std::string > &ips, const std::vector< int > &ports);
    
private:
    bool    m_hadShowDisConnetLayer;//是否已经展示了网络断开的对话框
};

#endif /* LORDNETMANGER_H_ */
