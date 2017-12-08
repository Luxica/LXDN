#ifndef __SocketParallelCheck_H__
#define __SocketParallelCheck_H__

#include "cocos2d.h"
#include "WWInetAddress.h"

// 并发检测结果事件分发名称
#define EVENT_LORD_PARALLEL_NET_RESULT			"Event_ParallelLordNetResult"

/*
 * 游戏中用于并发检测socket通道是否可用
 */
class SocketParallelCheck : public cocos2d::Ref
{
public:
	enum class Statue {
		SUCCESS,			// 成功，并发检测匹配到可用的服务端地址信息
		FAILURE,			// 失败，并发检测没有匹配到可用的服务端地址信息
		TIMEOUT,			// 超时，并发检测在超时时间内没有匹配到可用的服务端地址信息
	};
	/*
	 * 检测结果回调原型定义
	 */
	typedef std::function< void (Statue, const WWInetAddress &) > FN_CheckCallback;

public:
    SocketParallelCheck();
    ~SocketParallelCheck();

public:
	/*
	 * 设置检测状态更新时的回调
	 * @param[in] callback -- 检测结果回调接口
	 * @note -- 如果用在使用时没有设置此回调接口，默认会使用事件进行匹配状态结果的分发
	 * 事件名为：EVENT_LORD_PARALLEL_NET_RESULT，事件数据为:tuple< Statue, const WWInetAddress * > 类型
	 */
	void setCheckCallback(const FN_CheckCallback &callback);

	/*
	 * 设置超时时间
	 * @param[in] fTimeout -- 设置超时时间
	 * @note -- 默认的超时时间为10秒钟
	 */
	void setTimeout(float fTimeout);

	/*
	 * 设置用于并行检测的ip地址与端口
	 * @param[in] inetAddrs -- 用于检测的服务端地址与商品
	 * @return -- true:地址信息添加成功;false:地址信息添加失败
	 * @note -- 添加失败原因：可能并发正在进行中，可以先调用end()结束并发
	 */
	bool setSocketInetAddrs(const std::vector< WWInetAddress > &inetAddrs);

	/*
     * 并发开始
	 * @return -- true:并行开始;false:并发失败
	 * @note -- 并发失败原因：1、并发正在进行；2、服务端地址与端口信息没有设置
     */
    bool start();

    /*
     * 并发结束
	 * @return -- true:结束并发成功;false:结束并发失败
	 * @note -- 失败原因：并发没有进行，则无法结束并发
     */
    bool end();

	/*
	 * 是否正在进行并发检测
	 * @return -- true:并发正在进行中；false:并发没有进行；
	 */
	bool isParalleling() const;

	/*
	 * 检测是否有检测成功的服务端地址
	 * @note -- 用户不要自己调用此接口
	 */
	void update(float);

private:
	/*
	 * 服务端地址信息检测状态
	 */
	enum class CheckStatusType {
		INIT,				// 初始状态
		FAILURE,			// 失败状态，检测到地址信息不可用
		SUCCESS,			// 成功状态，检测到地址信息可用
	};

	/*
	 * 多线程处理数据结构
	 */
	struct SocketData : public cocos2d::Ref {
		WWInetAddress _inetAddr;			// 地址信息
		CheckStatusType _checkSuccessed;	// 检测状态值
		std::mutex _mutex;					// 互斥变量

		explicit SocketData(const WWInetAddress &inetAddr, CheckStatusType init = CheckStatusType::INIT)
			: _inetAddr(inetAddr), _checkSuccessed(init) {}
	};

	/*
	 * 并发线程入口函数
	 * @note -- 自己不要调用此函数
	 */
	void checkThreadFunc(SocketData *pData);

private:
	void generateSocketDatas();
	void destroySocketDatas();

	/*
	 * 如果用户没有设置回调接口，则使用事件方式将并发匹配的结果派发出去
	 * @param[in] val -- 并发匹配状态
	 * @param[in] inetAddr -- 并发匹配地址值
	 * @note -- 对于匹配状态为非Statue::SUCCESS的情况下，inetAddr参数现在默认使用第一个地址值
	 */
	void dispatchNetEventWithInetAddr(Statue val, const WWInetAddress &inetAddr);

private:
	CC_DISALLOW_COPY_AND_ASSIGN(SocketParallelCheck);

private:
	cocos2d::Scheduler *_scheduler;

	bool m_isParalleling;								// 当前是否正在并发标记
	bool m_threadExitTag;								// 线程退出标记

	FN_CheckCallback m_callback;						// 并发检测结果回调	

	std::vector< WWInetAddress > m_inetAddrs;			// 缓存的ip地址与商品信息
	std::vector< SocketData * > m_socketDatas;			// 根据缓存信息分配的线程检测信息
	std::vector< std::thread > m_parallels;				// 并发线程池

	float m_totalSchedulerTime;							// 超时时长
	float m_timeout;									// 超时计数变量
};

#endif
