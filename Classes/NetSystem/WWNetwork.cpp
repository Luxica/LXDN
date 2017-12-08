
#include "WWNetwork.h"
#include "../EventSystem/WWMsgManager.h"
#include "../net/LordNetConstants.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iOSAdapterUtility.hpp"
#include "bugly/CrashReport.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AdmobJni.h"
#include "bugly/CrashReport.h"
#endif

USING_NS_CC;
using namespace cocos2d::network;

static const int CONNECT_FAIL_CHANGE = 8;
#define HTTP_URL_FOR_NEW_SOCKET_ADDR "http://gs.wawagame.cn:5320/handler/requestAddress"

static long long getCurrentTime() {
	struct timeval now;
	gettimeofday(&now, nullptr);
	return now.tv_sec * 1000 + now.tv_usec / 1000;
}

void WWNetwork::setInetAddress(const WWInetAddress& oInetAddress)
{
    m_oInetAddress = oInetAddress;
}

const WWInetAddress& WWNetwork::getInetAddress() const
{
    return m_oInetAddress;
}

void WWNetwork::send(WWBuffer* pBuffer)
{
    if (pBuffer->empty())
    {
        return;
    }
	int u_len = pBuffer->length();
    
#if USING_PACKAGE_HEAD_LENGTH
    
    int u_move_len = sizeof(int);
    if (u_len >= 0xFFFF)
    {
        u_move_len += sizeof(int);
        pBuffer->moveRight(u_move_len);
        pBuffer->moveWriterIndexToFront();
        pBuffer->writeShort((short)0xFFFF);
        pBuffer->writeInt(u_len);
    }
    else
    {
        pBuffer->moveRight(u_move_len);
        pBuffer->moveWriterIndexToFront();
        pBuffer->writeInt(u_len);
    }
    
#endif
    
    pBuffer->moveReaderIndexToFront();
    char* pData = pBuffer->readWholeData();
    int nLength = (int)pBuffer->length();
    pBuffer->moveReaderIndexToFront();
    
    _SENDBUFFER tBuf;
    tBuf.pBuffer = pData;
    tBuf.nLength = nLength;
    tBuf.nOffset = 0;
    tBuf.nSendId = 0;
    
    m_lSendBuffers.push_back(tBuf);
}

void WWNetwork::sendMessageNow(WWBuffer* pBuffer)
{
    if (pBuffer->empty())
        return;
#if USING_PACKAGE_HEAD_LENGTH
    //获取buffer 长度 u_len
    int u_len = pBuffer->length();
    CCLOG("send length=%d", u_len);
    
    int u_move_len = sizeof(int);
    //如果长度超过0xFFFF
    if (u_len >= 0xFFFF)
    {
        //右移长度加一个int所占长度
        u_move_len += sizeof(int);
        //右移
        pBuffer->moveRight(u_move_len);
        //将写入索引移到最前端
        pBuffer->moveWriterIndexToFront();
        //写入short 0xFFFF
        pBuffer->writeShort((short) 0xFFFF);
        //写入真实长度
        pBuffer->writeInt(u_len);
    }
    else
    {
        //右移
        pBuffer->moveRight(u_move_len);
        //将写入索引移到最前端
        pBuffer->moveWriterIndexToFront();
        //写入真实长度
        pBuffer->writeInt(u_len);
    }
    
#endif
    
    pBuffer->moveReaderIndexToFront();
    char* pData = pBuffer->readWholeData();
    int nLength = (int) pBuffer->length();
    pBuffer->moveReaderIndexToFront();

	if (pBuffer)
		CC_SAFE_DELETE(pBuffer);
    
    _SENDBUFFER tBuffer;
    tBuffer.pBuffer = pData;
    tBuffer.nLength = nLength;
    tBuffer.nOffset = 0;
    tBuffer.nSendId = 0+1;
    
    int nRet = m_oSocket.ccWrite(tBuffer.pBuffer + tBuffer.nOffset, tBuffer.nLength - tBuffer.nOffset);
}

bool WWNetwork::isConnected()
{
    return m_eStatus == eSocketConnected;
}

bool WWNetwork::connect()
{
    if (m_eStatus != eSocketConnected && m_eStatus != eSocketConnecting)
    {
        m_oSocket.setInetAddress(m_oInetAddress);
        if (m_oSocket.ccConnect())
        {
            registerScheduler();
            m_eStatus = eSocketConnecting;
            return true;
        }
        else
        {
            m_oSocket.ccClose();
            m_eStatus = eSocketConnectFailed;
            
            netException(eSocketConnectFailed);
        }
    }
    return false;
}

void WWNetwork::clearSendBufferBak()
{
    CCLOG("clear send buffer bak");
    // 清空发送队列的缓存
    while (!m_lSendBuffers_bak.empty())
    {
        CC_SAFE_DELETE_ARRAY(m_lSendBuffers_bak.front().pBuffer);
        m_lSendBuffers_bak.pop_front();
    }
}

void WWNetwork::clearSendBuffer()
{
	while (!m_lSendBuffers.empty())
	{
		CC_SAFE_DELETE_ARRAY(m_lSendBuffers.front().pBuffer);
		m_lSendBuffers.pop_front();
	}
}

void WWNetwork::parallelConnect(const std::vector< WWInetAddress > &inetAddrs)
{
    if (m_socketParallelCheck->isParalleling())
    {
        m_socketParallelCheck->end();
    }
    
    m_socketParallelCheck->setSocketInetAddrs(inetAddrs);
    m_socketParallelCheck->start();
}

void WWNetwork::disconnect(WWSocketStatus status)
{
    if (status == eSocketHandshakingFailed)
    {
        m_eStatus = status;
        
        onDisconnected();
        
        return;
    }
    
    if (m_eStatus == eSocketConnected)
    {
        unregisterScheduler();
        m_oSocket.ccDisconnect();
        m_eStatus = status;
        m_fDataReciveDelayDuration = 0.0f;
        m_fConnectingDuration = 0.0f;
        if (m_eStatus == eSocketDisconnected)
        {
			clearSendBufferBak();
        }
        
        onDisconnected();
    }
}

void WWNetwork::removeBakBySendId(int m_sendMsg_Id)
{
    std::list<_SENDBUFFER>::iterator it;
    for (it = m_lSendBuffers_bak.begin(); it != m_lSendBuffers_bak.end();)
    {
        if (it->nSendId == m_sendMsg_Id)
        {
            //删除 char* 内存
            CC_SAFE_DELETE_ARRAY(it->pBuffer);
            //重新给迭代器it赋值为删除后的迭代器
            it = m_lSendBuffers_bak.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void WWNetwork::close(WWSocketStatus status)
{
    disconnect(status);
}

void WWNetwork::runSchedule(float dt)
{
    switch (m_eStatus)
    {
        case eSocketConnecting:
        {
            switch (m_oSocket.ccIsConnected())
            {
                case eSocketConnected:
                {
                    m_eStatus = eSocketConnected;
					if (bYccConnnect_)
					{
						bYccConnnect_ = false;
					}
					EventCustom event("CONNECTRE_SUCCESS");
					event.setUserData((void*)m_iNetType_);
					Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

					_lastReceivedMessageTimer = getCurrentTime();
					m_nSocketConnectFailCount = 0;
					m_connectFailedCount = CONNECT_FAIL_CHANGE;
                }
                    break;
                case eSocketConnectFailed:
                {
                    unregisterScheduler();
                    m_oSocket.ccClose();
                    m_eStatus = eSocketConnectFailed;
                    
                    netException(eSocketConnectFailed);
                }
                    break;
                case eSocketConnecting:
                {
                    if (m_fConnectingDuration > m_fSoTimeout)
                    {
                        unregisterScheduler();
                        m_oSocket.ccDisconnect();
                        m_eStatus = eSocketConnectTimeout;
                        
                        onConnectTimeout();
                        
                        m_fConnectingDuration = 0.0f;
                    }
                    else
                    {
                        m_fConnectingDuration += dt;
                    }
                }
                    break;
                default:
                    break;
            }
        }
            break;
		case eSocketConnected:
		{
			if (m_iNetType_ >= eHallNet)
			{
				long long current = getCurrentTime();
				if (current - _lastReceivedMessageTimer >= 15 * 1000) {
					unregisterScheduler();
					m_oSocket.ccClose();
					m_eStatus = eSocketNoReceiveInTime;
					netException(m_eStatus);
					break;
				}

				if (m_fDataReciveDelayDuration > RECIVE_DATA_DELAY_TIME) {
					CCLOG("m_fDataReciveDelayDuration > RECIVE_DATA_DELAY_TIME");
					////接受数据间隔时间超过30,断开连接回调断线方法
					unregisterScheduler();
					m_oSocket.ccDisconnect();
					m_eStatus = eSocketNoReceiveInTime;

					onDisconnected();

					m_fDataReciveDelayDuration = 0.0f;
					break;
				}

			}
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//			if(!AdmobJni::isConnectionAvailable()) {
//				disconnect(eSocketNoReceiveInTime);
//				return;
//			}
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//            if(!iOSAdapterUtility::getInstance()->isConnectionAvailable()) {
//                disconnect(eSocketNoReceiveInTime);
//                return;
//            }
//#endif
			if (m_oSocket.ccIsReadable()) {
				m_fDataReciveDelayDuration = 0.0f;
			}
			else {
				m_fDataReciveDelayDuration += dt;
			}

			//把缓存的消息分帧发送
			if (m_vecCacheBuffer.size() > 0) {
				auto pData = m_vecCacheBuffer.front();
				onMessageReceived(*pData);
				m_vecCacheBuffer.eraseObject(pData);
				break;
			}

#if HANDLE_ON_SINGLE_FRAME
			while (m_oSocket.ccIsReadable())
#else
			if (m_oSocket.ccIsReadable())
#endif
				//读取
			{
				if (this->runRead()) {

					return;
				}
			}

			if (m_iNetType_ >= eHallNet)
			{
				if (m_heartBeatMsgDuration >= HEART_BEAT_DELAY_TIME)
				{
					//收到心跳已发送心跳状态置为true,表示上次发送心跳还没收到回应调用网络延迟方法
					onHeartBeatMsg();
				}
				else
				{
					m_heartBeatMsgDuration += dt;
				}
			}
			//modify ycc 10-30
			/*if (m_iNetType_ >= eHallNet) {
				_needQuit = false;
				if (_heartMsgThread == nullptr) {
					_heartMsgThread = new std::thread([&]() { 
						while (!_needQuit) {
							_heartMutex.lock();
							if (isConnected()) {
								long long current = getCurrentTime();
								if (current - _lastReceivedMessageTimer >= 30 * 1000) {
									unregisterScheduler();
									m_oSocket.ccClose();
									m_eStatus = eSocketNoReceiveInTime;
									netException(m_eStatus);
								}
								else {
									log("network(%d) send Heart msg ..............", m_iNetType_);
									onHeartBeatMsg();
								}
							}
							_heartMutex.unlock();
							std::this_thread::sleep_for(std::chrono::milliseconds(5000));
						}
					});
				}
				m_isHeartMsgSend = true;
			}*/
            
#if HANDLE_ON_SINGLE_FRAME
            while (m_oSocket.ccIsWritable() && !m_lSendBuffers.empty())
#else
                if (m_oSocket.ccIsWritable() && !m_lSendBuffers.empty())
#endif
                    //发送
                {
                    if (this->runWrite())
                    {
                        return;
                    }
                    //else
                    //{
                    //    m_heartBeatMsgDuration = 0.0f;
                    //}
                }
        }
            break;
        default:
            break;
    }
}

bool WWNetwork::runRead()
{
    int nRet = m_oSocket.ccRead(m_pReadBuffer, SOCKET_READ_BUFFER_SIZE);
    
    if (nRet == eSocketIoError || nRet == eSocketIoClosed)
    {
		CCLOG("runRead.......error");
  //      unregisterScheduler();
  //      m_oSocket.ccClose();
  //      m_eStatus = eSocketIoClosed;
		//m_eStatus == eSocketDisconnected;
		////bYccConnnect_ = true;
  //      onDisconnected();
		disconnect(eSocketNoReceiveInTime);
		//if (m_iNetType_ == eGameNet)
		//{
		//	if (m_netEventDelegate)
		//		m_netEventDelegate->onExceptionCaught(NetGameCut);
		//}
		//else if (m_iNetType_ == eHallNet)
		//{
		//	if (m_netEventDelegate)
		//		m_netEventDelegate->onExceptionCaught(NetHallCut);
		//}
		
        return true;
    }
    else
    {
        CCLOG("WWSOCKET READ LEN %d", nRet);
		_lastReceivedMessageTimer = getCurrentTime();

        m_oReadBuffer.writeData(m_pReadBuffer, nRet);
        
#if USING_PACKAGE_HEAD_LENGTH
        //是否能读取一个short
        while (m_oReadBuffer.isReadable(sizeof(unsigned int)))
        {
            //将读取索引移到最前端
            m_oReadBuffer.moveReaderIndexToFront();
            //读取一个short 获取消息体长度 n_msg_len
            int n_msg_len = m_oReadBuffer.readUInt();
            //消息体所占长度n_head_len
            int n_head_len = (int) (sizeof(unsigned int));
            if (n_msg_len <= 0)
            {
                CCLOGERROR("invalidate msg length");
                m_oReadBuffer.moveLeft(n_head_len);
            }
            
            if (n_msg_len >= 0xFFFF)
            {
                //消息体所占长度增加一个int 字节数
                if (!m_oReadBuffer.isReadable(sizeof(int)))
                {
                    break;
                }
                n_head_len += (int) (sizeof(int));
                n_msg_len = m_oReadBuffer.readInt();
                
                if (n_msg_len <= 0)
                {
                    CCLOGERROR("invalidate msg length");
                    m_oReadBuffer.moveLeft(n_head_len);
                }
            }
            if (!m_oReadBuffer.isReadable(sizeof(int)))
            {
                break;
            }
            
            //当前缓冲区buffer长度
            int n_content_len = (int) m_oReadBuffer.length();
            
            //判读是否读取完整一条消息
            if (n_content_len - n_head_len >= n_msg_len)
            {
                //左移覆盖消息头所占长度
                m_oReadBuffer.moveLeft(n_head_len);
                //获取msg 长度消息体
                WWBuffer* pData = m_oReadBuffer.readData(n_msg_len);
                //左移 移除读取后的消息体
                m_oReadBuffer.moveLeft(n_msg_len);
                //读取索引移到buffer首段
                m_oReadBuffer.moveReaderIndexToFront();
                //写入索引移到buffer末端
                m_oReadBuffer.moveWriterIndexToBack();
                
                //收到完整消息体，回调接收消息函数
                //onMessageReceived(*pData);
                m_vecCacheBuffer.pushBack(pData);
            }
            else
            {
                break;
            }
        }
#else
        WWBuffer* pData = (WWBuffer*) m_oReadBuffer.copy();
        pData->autorelease();
        m_oReadBuffer.clear();
        
        onMessageReceived(*pData);
#endif
    }
    return false;
}

bool WWNetwork::runWrite()
{
    _SENDBUFFER& tBuffer = m_lSendBuffers.front();
    
    int nRet = m_oSocket.ccWrite(tBuffer.pBuffer + tBuffer.nOffset, tBuffer.nLength - tBuffer.nOffset);
    CCLOG("WWSOCKET WRITE LEN= %d", nRet);
    if (nRet == eSocketIoError || nRet == eSocketIoClosed)
    {
		log("runWrite.......error");
        unregisterScheduler();
        m_oSocket.ccClose();
        m_eStatus = eSocketIoClosed;
        
        disconnect(eSocketIoError);
        
        return true;
    }
    else if (nRet == tBuffer.nLength - tBuffer.nOffset)
    {
        //CC_SAFE_DELETE_ARRAY(tBuffer.pBuffer);
        //偏移索引归零 后加入待确认备份列表
        
        tBuffer.nOffset = 0;
        //如果发送的id为-1(心跳) -2 握手 不加入备份
        if (tBuffer.nSendId != -1 && tBuffer.nSendId != -2)
        {
            //m_lSendBuffers_bak.push_back(tBuffer);	modify ycc
        }
        else
        {
            CC_SAFE_DELETE_ARRAY(tBuffer.pBuffer);
            tBuffer.pBuffer = nullptr;
        }
        
        m_lSendBuffers.pop_front();
    }
    else
    {
        tBuffer.nOffset += nRet;
    }
    return false;
}

void WWNetwork::registerScheduler()
{
    CCLOG("WWNetDelegate::registerScheduler%d", m_bRunSchedule);
    if (m_bRunSchedule)
    {
        return;
    }
    
	auto scheduler = Director::getInstance()->getScheduler();
	if (!scheduler->isScheduled(schedule_selector(WWNetwork::runSchedule), this))
	{
		scheduler->schedule(schedule_selector(WWNetwork::runSchedule), this, 0, false);
	}
    m_bRunSchedule = true;
    CCLOG("WWNetDelegate::registerScheduler end %d", m_bRunSchedule);
}

void WWNetwork::unregisterScheduler()
{
    CCLOG("WWNetDelegate::unregisterScheduler %d", m_bRunSchedule);
    
    if (!m_bRunSchedule) return;
    
	auto scheduler = Director::getInstance()->getScheduler();
	if (scheduler->isScheduled(schedule_selector(WWNetwork::runSchedule), this))
	{
		scheduler->unschedule(schedule_selector(WWNetwork::runSchedule), this);
	}


    m_bRunSchedule = false;
    
    CCLOG("WWNetDelegate::unregisterScheduler end %d", m_bRunSchedule);
    
}

void WWNetwork::pushMsgFront(WWBuffer *pBuffer)
{
    if (!pBuffer || pBuffer->empty()) return;
    
#if USING_PACKAGE_HEAD_LENGTH
    //获取buffer 长度 u_len
    int u_len = pBuffer->length();
    CCLOG("send length=%d", u_len);
    //u_len-=2*sizeof(char);
    //buffer需要右移的长度 u_move_len
    
    int u_move_len = sizeof(int);
    //如果长度超过0xFFFF
    if (u_len >= 0xFFFF)
    {
        //右移长度加一个int所占长度
        u_move_len += sizeof(int);
        //右移
        pBuffer->moveRight(u_move_len);
        //将写入索引移到最前端
        pBuffer->moveWriterIndexToFront();
        //写入short 0xFFFF
        pBuffer->writeShort((short) 0xFFFF);
        //写入真实长度
        pBuffer->writeInt(u_len);
    }
    else
    {
        //右移
        pBuffer->moveRight(u_move_len);
        //将写入索引移到最前端
        pBuffer->moveWriterIndexToFront();
        //写入真实长度
        pBuffer->writeShort((short) u_len);
    }
    
#endif
    
    pBuffer->moveReaderIndexToFront();
    char* pData = pBuffer->readWholeData();
    int nLength = (int) pBuffer->length();
    pBuffer->moveReaderIndexToFront();
    
    _SENDBUFFER tBuf;
    tBuf.pBuffer = pData;
    tBuf.nLength = nLength;
    tBuf.nOffset = 0;
    tBuf.nSendId = m_sendMsg_Id;
    
    m_lSendBuffers.push_front(tBuf);
}

void WWNetwork::onMessageReceived(WWBuffer& oBuffer)
{
	//CCLOG("WWNetwork onMessageReceived");
    
    int msgType = oBuffer.readInt();
	CCLOG("network(%d) msgType = %d ------------------------------------------ ", m_iNetType_, msgType);
    
    switch (msgType)
    {
		case 0x010101: // 忽略心跳返回消息0x7f7f7f
            //收到心跳已发送心跳状态置为false
            m_isHeartMsgSend = false;
			m_isSucces = true;
            CCLOG("heart back");
            break;
        default:
            //重置读取的索引位置为标记位置
            oBuffer.resetReaderIndex();

            //消息分发中心分发
            WWMsgManager::getInstance()->onMessage(&oBuffer);
            break;
    }
}

void WWNetwork::reSocketConnect(float dt)
{
    CCLOG("WWNetwork::reSocketConnect %d ", m_nSocketConnectFailCount);
    if(m_isSucces){
        return;
    }
    
    //请求连接
    if (isConnected())
    {
        close();
    }
    m_nSocketConnectFailCount++;
    connect();
    CCLOG("WWNetwork::reSocketConnect count(%d)", m_nSocketConnectFailCount);
}

void WWNetwork::onHeartBeatMsg()
{
	//发送心跳信息
	WWBuffer* pBuffer = new WWBuffer();
	pBuffer->writeInt(0);
	if (pBuffer->empty() || !isConnected()) return;

	//获取buffer 长度 u_len
	int u_len = pBuffer->length();

#if USING_PACKAGE_HEAD_LENGTH

	//buffer需要右移的长度 u_move_len
	int u_move_len = sizeof(int);
	//如果长度超过0xFFFF
	if (u_len >= 0xFFFF)
	{
		//右移长度加一个int所占长度
		u_move_len += sizeof(int);
		//右移
		pBuffer->moveRight(u_move_len);
		//将写入索引移到最前端
		pBuffer->moveWriterIndexToFront();
		//写入short 0xFFFF
		pBuffer->writeShort((short)0xFFFF);
		//写入真实长度
		pBuffer->writeInt(u_len);
	}
	else
	{
		//右移
		pBuffer->moveRight(u_move_len);
		//将写入索引移到最前端
		pBuffer->moveWriterIndexToFront();
		//写入真实长度
		pBuffer->writeInt((int)u_len);
	}

#endif
	pBuffer->moveReaderIndexToFront();
	char* pData = pBuffer->readWholeData();
	int nLength = (int)pBuffer->length();
	pBuffer->moveReaderIndexToFront();

	int nRet = m_oSocket.ccWrite(pData, nLength);
	CCLOG("WWSOCKET WRITE LEN= %d", nRet);
	CC_SAFE_DELETE(pBuffer);
    if (nRet == eSocketIoError || nRet == eSocketIoClosed)
    {
        log("runWrite.......error");
        unregisterScheduler();
        m_oSocket.ccClose();
        m_eStatus = eSocketIoClosed;
        
        onDisconnected();
    }
	log("send NetManager %d onHeartBeatMsg", m_iNetType_);
}

void WWNetwork::onConnectTimeout()
{
    log("WWNetwork  onConnectTimeout");
    onDisconnected();
    //连接失败后，重试
    if (eSocketConnectTimeout == m_eStatus)
    {
        //连接失败后，延迟5s重试
        Director::getInstance()->getScheduler()->schedule(schedule_selector(WWNetwork::reSocketConnect), this, 0.0f, 0, 0.0f, false);
    }
}

void WWNetwork::onDisconnected()
{
	m_isSucces = false;

	unregisterScheduler();
	
	if (m_eStatus != eSocketDisconnected) {
		//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CrashReport::log(CrashReport::Info, "WWNetwork::onDisconnected", "---1---m_iNetType_=%d---", m_iNetType_);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		CrashReport::log(CrashReport::Info, "WWNetwork::onDisconnected", "---1---m_iNetType_=%d---", m_iNetType_);
#endif

		EventCustom event("ON_NETWORK_DISCONNECTED");
		event.setUserData((void*)m_iNetType_);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CrashReport::log(CrashReport::Info, "WWNetwork::onDisconnected", "---2---m_iNetType_=%d---", m_iNetType_);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		CrashReport::log(CrashReport::Info, "WWNetwork::onDisconnected", "---2---m_iNetType_=%d---", m_iNetType_);
#endif
	}

	log("WWNetwork  onDisconnected error %d,netid %d", this->m_eStatus, this->m_iNetType_);
    if(eSocketHandshakingFailed == m_eStatus)
    {
        if (m_netEventDelegate)
        {
            m_netEventDelegate->onExceptionCaught(NetStatueHandSharingFialed);
        }
        return;
    }
    if (eSocketDisconnected != m_eStatus && eSocketDisconnectedForBg != m_eStatus)
    {
        //连接失败后，延迟5s重试
        //如果超过重试次数，则使用http请求新的socket地址
		if (m_nSocketConnectFailCount < SOCKET_CONNECT_FAIL_COUNT)
		{
			if (!Director::getInstance()->getScheduler()->isScheduled(schedule_selector(WWNetwork::reSocketConnect),this))
			{
				Director::getInstance()->getScheduler()->schedule(schedule_selector(WWNetwork::reSocketConnect), this, 0.0f, 0, 0.0f, false);
			}
		}
        else
        {
            //使用http请求获取新的socket地址
            //m_nSocketConnectFailCount = 0;
			reConnectFail();
        }
    }
    else
    {
        isReconnect = false;
        m_nSocketConnectFailCount = 0;
		clearSendBuffer();
    }
}

void WWNetwork::netException(WWSocketStatus eStatus)
{
    if (eSocketConnectFailed == eStatus)
    {
        CC_ASSERT(m_netEventDelegate);
	    if (m_connectFailedCount < CONNECT_FAIL_CHANGE - 2)
        {
            m_netEventDelegate->onExceptionCaught(NetStatueClose);
            m_connectFailFlag = true;
            m_reconnectTimePoint = std::chrono::steady_clock::now();
        }
        else
        {
            if (m_connectFailedCount > 0)
            {
                if (m_connectFailedCount == CONNECT_FAIL_CHANGE)
                {
                    m_netEventDelegate->reConnect(m_iNetType_,0.f);
                } else {
					m_netEventDelegate->reConnect(m_iNetType_);
                }
                m_connectFailedCount--;
                log("onExceptionCaught:reConnect[%d]", m_connectFailedCount);
                return ;
            }
            
            m_connectFailedCount = CONNECT_FAIL_CHANGE;
            log("onExceptionCaught: m_connectFailedCount[%d]", m_connectFailedCount);
            m_netEventDelegate->onExceptionCaught(NetStatueClose);
            m_connectFailFlag = true;
            m_reconnectTimePoint = std::chrono::steady_clock::now();
        }
//        return ;
    }
    log("WWNetwork  onExceptionCaught %d", (int)eStatus);
    onDisconnected();
}

void WWNetwork::reConnectFail()
{
    log("reconnect fail.");
	EventCustom event(CONNECT_FAIL);
	event.setUserData((void*)m_iNetType_);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    m_nSocketConnectFailCount = 0;
    
  //  if (eSocketNoReceiveInTime == m_eStatus)
  //  {
  //      if (m_netEventDelegate)
  //      {
  //          m_netEventDelegate->onExceptionCaught(NetStatueNoRsp);
  //          return ;
  //      }
  //  }
  //  
  //  if (eSocketDisconnected != m_eStatus || eSocketDisconnectedForBg != m_eStatus)
  //  {
  //      m_eStatus = eSocketConnectFailed;
  //      onDisconnected();
		////连接失败 提醒 YCC
  //  }
}

WWNetwork::WWNetwork(const int &iNetType)
	:m_bRunSchedule(false), 
	m_fSoTimeout(SOCKET_SOTIMEOUT), 
	m_fConnectingDuration(0.00000f), 
	m_heartBeatMsgDuration(0.00000f), 
	m_fDataReciveDelayDuration(0.00000f), 
	m_recvMsg_Id(0), 
	m_sendMsg_Id(0), 
	m_isHeartMsgSend(false), 
	m_isSucces(false), 
	m_eStatus(eSocketIoClosed), 
	m_netEventDelegate(nullptr), 
	isReconnect(false), 
	channelID(-1), 
	m_nSocketConnectFailCount(0), 
	m_recvIDFromServer(0), 
	m_connectFailFlag(false), 
	m_pComeToBackgroundListener(nullptr), 
	m_pComeToForegroundListener(nullptr), 
	m_socketParallelCheck(nullptr),
	m_strHttpUrlForNewSocketAddr(HTTP_URL_FOR_NEW_SOCKET_ADDR),
	_heartMsgThread(nullptr),
	_needQuit(true),
	_lastReceivedMessageTimer(0)
{
	m_iNetType_ = iNetType;
	memset(m_pReadBuffer, 0, sizeof(m_pReadBuffer));
	bYccConnnect_ = false;
    m_connectFailedCount = CONNECT_FAIL_CHANGE;
    m_socketParallelCheck = new SocketParallelCheck();
    
    m_socketParallelCheck->setCheckCallback([this](SocketParallelCheck::Statue statue, const WWInetAddress &inetAddr) {
        if (SocketParallelCheck::Statue::SUCCESS == statue) {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, inetAddr]() {
                CCLOG("WWNetwork-testConnect: success, ip:%s, port:%d", inetAddr.getIp(), inetAddr.getPort());
                this->setInetAddress(inetAddr);
                this->connect();
            });
        } else if (SocketParallelCheck::Statue::FAILURE == statue) {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, inetAddr]() {
                CCLOG("WWNetwork-testConnect: failure.");
                // TODO:暂时没有找到网络连接不上时，怎样直接通知UI界面，所以只能用无法连接的地址信息进行连接
                this->setInetAddress(inetAddr);
                this->connect();
            });
        } else {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, inetAddr]() {
                CCLOG("WWNetwork-testConnect: timeout");
                // TODO:暂时没有找到网络连接不上时，怎样直接通知UI界面，所以只能用无法连接的地址信息进行连接
                this->setInetAddress(inetAddr);
                this->connect();
            });
        }
    });
    
	if (m_iNetType_ != eLoginNet)
	{
		auto evtDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
		m_pComeToBackgroundListener = evtDispatcher->addCustomEventListener(EVENT_COME_TO_BACKGROUND,
			CC_CALLBACK_1(WWNetwork::onComeToBackgroundEvent, this));
		m_pComeToForegroundListener = evtDispatcher->addCustomEventListener(EVENT_COME_TO_FOREGROUND,
			CC_CALLBACK_1(WWNetwork::onComeToForegroundEvent, this));
	}
}

WWNetwork:: ~ WWNetwork()
{
	unregisterScheduler();
    m_oSocket.ccClose();

	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);

	clearSendBuffer();
	clearSendBufferBak();
    
    auto evtDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    evtDispatcher->removeEventListener(m_pComeToForegroundListener);
    evtDispatcher->removeEventListener(m_pComeToBackgroundListener);

	CC_SAFE_DELETE(m_socketParallelCheck);

	_needQuit = true;
	if (_heartMsgThread) {
		if (_heartMsgThread->joinable()) {
			_heartMsgThread->join();
		}
		CC_SAFE_DELETE(_heartMsgThread);
	}
}

void WWNetwork::setNewSocketUrl(const std::string &newSocketUrl)
{
	if (!newSocketUrl.empty())
		return;
	m_strHttpUrlForNewSocketAddr = newSocketUrl;
	cocos2d::log("<WWNetwork | setNewSocketUrl - %s", m_strHttpUrlForNewSocketAddr.c_str());
}

void WWNetwork::onComeToBackgroundEvent(cocos2d::EventCustom *pEvent)
{
	if (m_connectFailFlag)
	{
		return;
	}
	m_connectFailFlag = true;
	m_reconnectTimePoint = std::chrono::steady_clock::now();
}

void WWNetwork::onComeToForegroundEvent(cocos2d::EventCustom *pEvent)
{
	// TODO
}
