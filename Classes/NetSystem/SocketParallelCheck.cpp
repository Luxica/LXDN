#include "SocketParallelCheck.h"
#include "WWSocket.h"

USING_NS_CC;

/*
 * SocketParallelCheck.h 实现文件
 */

#define DEFAULT_TIME_OUT 10.f

SocketParallelCheck::SocketParallelCheck()
	: m_isParalleling(false)
	, m_threadExitTag(false)
	, m_totalSchedulerTime(DEFAULT_TIME_OUT)
	, m_timeout(0.f)
{
	_scheduler = Director::getInstance()->getScheduler();
}

SocketParallelCheck::~SocketParallelCheck()
{
	end();
}

void SocketParallelCheck::setCheckCallback(const FN_CheckCallback &callback)
{
	m_callback = callback;
}

void SocketParallelCheck::setTimeout(float fTimeout) 
{
	m_totalSchedulerTime = fTimeout;
	if (m_totalSchedulerTime <= 0.f) {
		m_totalSchedulerTime = DEFAULT_TIME_OUT;
	}
}

bool SocketParallelCheck::setSocketInetAddrs(const std::vector< WWInetAddress > &inetAddrs)
{
	if (isParalleling()) {
		CCLOGINFO("SocketParallelCheck-setSocketInetAddr: parallel is running.");
		return false;
	}

	m_inetAddrs = inetAddrs;
	return true;
}

bool SocketParallelCheck::start()
{
	if (isParalleling()) {
		CCLOGINFO("SocketParallelCheck-start: is running.");
		return false;
	}

	if (m_inetAddrs.empty()) {
		CCLOG("SocketParallelCheck-start: inetAddress is empty.");
		return false;
	}

	m_isParalleling = true;
	m_threadExitTag = false;

	generateSocketDatas();

	CCLOGINFO("SocketParallelCheck-start: start threads(%d)", m_socketDatas.size());
	for (auto &item : m_socketDatas) {
		m_parallels.push_back(std::thread(&SocketParallelCheck::checkThreadFunc, this, item));
		m_parallels.back().detach();
	}
	
	CCLOGINFO("SocketParallelCheck-start: start scheduler to validate check result.");
	m_timeout = 0.f;
	_scheduler->scheduleUpdate(this, 0, false);
	return true;
}

bool SocketParallelCheck::end()
{
	if (!isParalleling()) {
		CCLOGINFO("SocketParalleCheck-end: not running.");
		return false;
	}

	CCLOGINFO("SocketParallelCheck-end: stop scheduler.");
	_scheduler->unscheduleUpdate(this);

	m_isParalleling = false;
	m_threadExitTag = true;
	CCLOGINFO("SocketParallelCheck-end: stop and release threads");
	m_parallels.clear();

	destroySocketDatas();	
	m_inetAddrs.clear();

	return true;
}

bool SocketParallelCheck::isParalleling() const 
{
	return m_isParalleling;
}

void SocketParallelCheck::update(float deltaTime) 
{
	m_timeout += deltaTime;
	bool checkSuccessMark = false;
	int checkFailCount = 0;
	WWInetAddress inetAddr = (*m_socketDatas.begin())->_inetAddr;
	for (auto &item : m_socketDatas) {
		std::lock_guard< std::mutex > lck(item->_mutex);
		if (CheckStatusType::FAILURE == item->_checkSuccessed) {
			checkFailCount++;
		} else if (CheckStatusType::SUCCESS == item->_checkSuccessed) {
			checkSuccessMark = true;
			inetAddr = item->_inetAddr;
			break;
		}
	}

	bool isEnded = true;
	Statue resultStatue = Statue::TIMEOUT;
	do {
		if (checkSuccessMark) {
			CCLOGINFO("SocketParallelCheck-update: check success.");
			resultStatue = Statue::SUCCESS;
			break;
		}

		if (checkFailCount == m_socketDatas.size()) {
			CCLOGINFO("SocketParallelCheck-update: check failure.");
			resultStatue = Statue::FAILURE;
			break;
		}

		if (m_timeout >= m_totalSchedulerTime) {
			CCLOGINFO("SocketParallelCheck-update: timeout(%.2f - %.2f)", m_timeout, m_totalSchedulerTime);
			resultStatue = Statue::TIMEOUT;
			break;
		}

		isEnded = false;
	} while (false);

	if (isEnded) {
		if (m_callback) {
			m_callback(resultStatue, inetAddr);
		} else {
			dispatchNetEventWithInetAddr(resultStatue, inetAddr);
		}
		end();
		return ;
	}
	

	CCLOGINFO("SocketParallelCheck-update: check is running.");
}

#define CHANGE_CHECK_STATE(pData, stateType) \
{ \
	std::lock_guard< std::mutex > lck(pData->_mutex); \
	pData->_checkSuccessed = stateType; \
	pData->release(); \
}

void SocketParallelCheck::checkThreadFunc(SocketData *pData)
{
	// 检测是否退出
	if (m_threadExitTag) {
		CCLOGINFO("SocketParallelCheck-checkThreadFunc: [entry]thread exit tag is true.");
		CHANGE_CHECK_STATE(pData, CheckStatusType::FAILURE);
		return ;
	}

	// socket连接操作
	WWSocket _socket;
	_socket.setInetAddress(pData->_inetAddr);
	auto connected = _socket.ccConnect();
	if (!connected) {
		CCLOGINFO("SocketParallelCheck-checkThreadFunc: [connect]socket connect failure(%s, %d)", pData->_inetAddr.getIp(), pData->_inetAddr.getPort());
		CHANGE_CHECK_STATE(pData, CheckStatusType::FAILURE);		
		_socket.ccClose();
		return ;
	}

	while (true) {
		if (m_threadExitTag) {
			CCLOGINFO("SocketParallelCheck-checkThreadFunc: [check] thread exit tag is true.");
			CHANGE_CHECK_STATE(pData, CheckStatusType::FAILURE);
			break;
		}

		auto connectStatus = _socket.ccIsConnected();
		if (eSocketConnected == connectStatus) {
			CCLOGINFO("SocketParallelCheck-checkThreadFunc: [check] socket connect success(%s, %d)", pData->_inetAddr.getIp(), pData->_inetAddr.getPort());
			CHANGE_CHECK_STATE(pData, CheckStatusType::SUCCESS);
			_socket.ccClose();
			break;
		} else if (eSocketConnectFailed == connectStatus) {
			CCLOGINFO("SocketParallelCheck-checkThreadFunc: [check] socket connect failure(%s, %d)", pData->_inetAddr.getIp(), pData->_inetAddr.getPort());
			CHANGE_CHECK_STATE(pData, CheckStatusType::FAILURE);
			_socket.ccClose();
			break;
		}

		// sleep for 1 millisecond to let other thread to execute
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void SocketParallelCheck::generateSocketDatas()
{
	destroySocketDatas();

	for (auto &item : m_inetAddrs) {
		SocketData *pData = new SocketData(item);
		pData->retain();
		m_socketDatas.push_back(pData);
	}
}

void SocketParallelCheck::destroySocketDatas()
{
	for (auto &item : m_socketDatas) {
		std::lock_guard< std::mutex > lck(item->_mutex);
		//item->release();
	}
	m_socketDatas.clear();
	
// 	for (auto iter = m_socketDatas.begin(); iter != m_socketDatas.end();) {
// 		{
// 		std::lock_guard< std::mutex > lck((*iter)->_mutex);
// 		(*iter)->release();}
// 		iter = m_socketDatas.erase(iter);
// 	}
// 
// 	m_socketDatas.clear();
}

void SocketParallelCheck::dispatchNetEventWithInetAddr(Statue val, const WWInetAddress &inetAddr) 
{
	_scheduler->performFunctionInCocosThread([this, val, &inetAddr]() {
		std::tuple< Statue, const WWInetAddress *> data = std::make_tuple(val, &inetAddr);
		cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_LORD_PARALLEL_NET_RESULT, &data);
	});
}