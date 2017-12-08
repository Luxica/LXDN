
#ifndef __WW_DDZ_PLUS__WWNetwork__
#define __WW_DDZ_PLUS__WWNetwork__

#include "WWNetMacros.h"
#include "WWSocket.h"
#include "network/HttpClient.h"
#include "SocketParallelCheck.h"

enum WWNetStatus
{
    NetStatueLag, //网络差
    NetStatueClose, //网络无法连接
    NetStatueNoRsp,  //服务器无响应
    NetStatueHandSharingFialed, //重连握手失败
	NetGameCut,
	NetHallCut,
};

class NetEventDelegate
{
public:
    virtual void onExceptionCaught(WWNetStatus eStatus) = 0;
    virtual WWBuffer* onRequestLogonInfo() = 0;
    virtual void reConnect(const int &iType,float delayTime = 2.f) = 0;
};

class WWNetwork : public cocos2d::Ref
{
public:
    
    WWNetwork(const int &iNetType);
    virtual ~ WWNetwork();
	enum eNetName
	{
		eLoginNet,
		eHallNet,
		eGameNet,
	};
	void setLoginSuccess(bool bValue);
	bool getLoginSuccess();

	void setNewSocketUrl(const std::string &newSocketUrl);
    
    //设置获取地址
    void setInetAddress(const WWInetAddress& oInetAddress);
    const WWInetAddress& getInetAddress() const;
    
    //发送消息
    void send(WWBuffer* pBuffer);
    void send(WWBuffer* pBuffer, int msgId);
    void sendMessageNow(WWBuffer* pBuffer);
    
    //是否连接
    bool isConnected();
    
    //断开连接，同disconnect()，兼容老项目
    void close(WWSocketStatus status = eSocketDisconnected);
    
    //发起连接
    bool connect();
    
    //并发连接
    void parallelConnect(const std::vector< WWInetAddress > &inetAddrs);
    
    //断开连接
    void disconnect(WWSocketStatus status = eSocketDisconnected);
    
    //设置回调
    void setDelegate(NetEventDelegate* pDelegate) { m_netEventDelegate = pDelegate; }
    
    //socket断开链接的方式
    CC_SYNTHESIZE(WWSocketStatus, m_eStatus, ExitStatus);
    CC_SYNTHESIZE(bool, isReconnect, Reconnect);
    
    void pushMsgFront(WWBuffer* pBuffer);
    
    void clearSendBufferBak();
	void clearSendBuffer();
    
private:
    
    void onMessageReceived(WWBuffer& oBuffer);
    void onHeartBeatMsg();
    
    void removeBakBySendId(int m_sendMsg_Id);
    
    bool runRead();
    bool runWrite();
    void runSchedule(float dt);
    void registerScheduler();
    void unregisterScheduler();
    
    void reSocketConnect(float dt);
    void reConnectFail();
    
    void onComeToBackgroundEvent(cocos2d::EventCustom *pEvent);
    void onComeToForegroundEvent(cocos2d::EventCustom *pEvent);
    void onConnectTimeout();
    void onDisconnected();
    void netException(WWSocketStatus eStatus);
    
    struct _SENDBUFFER
    {
        char* pBuffer;
        int nOffset;
        int nLength;
        int nSendId;
    };
    
    bool m_bRunSchedule;
    float m_fConnectingDuration;
    float m_fDataReciveDelayDuration;
    float m_fSoTimeout;
    std::list<_SENDBUFFER> m_lSendBuffers;
    std::list<_SENDBUFFER> m_lSendBuffers_bak;
    WWBuffer m_oReadBuffer;
    WWInetAddress m_oInetAddress;
    WWSocket m_oSocket;
    char m_pReadBuffer[SOCKET_READ_BUFFER_SIZE];
    float m_heartBeatMsgDuration;
    cocos2d::Vector<WWBuffer*> m_vecCacheBuffer;
    
    int m_sendMsg_Id;
    int m_recvMsg_Id;
    bool m_isHeartMsgSend;
    bool m_isSucces;
    
    NetEventDelegate* m_netEventDelegate;
    
    int channelID;
    int m_nSocketConnectFailCount;
    int m_recvIDFromServer;
    bool m_connectFailFlag;
    int m_connectFailedCount;
    std::chrono::steady_clock::time_point m_reconnectTimePoint;
    
    cocos2d::EventListener *m_pComeToForegroundListener;
    cocos2d::EventListener *m_pComeToBackgroundListener;
    SocketParallelCheck *m_socketParallelCheck;
    
    std::string m_strHttpUrlForNewSocketAddr;
	bool bYccConnnect_;
	int m_iNetType_;

	long long	_lastReceivedMessageTimer;
	std::thread* _heartMsgThread;
	std::mutex	_heartMutex;
	bool		_needQuit;
};

#endif /* defined(__WW_DDZ_PLUS__WWNetwork__) */
