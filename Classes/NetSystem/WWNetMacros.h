/****************************************************************************

****************************************************************************/
#ifndef __WWNET_MACROS_H__
#define __WWNET_MACROS_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
typedef unsigned int          SOCKET;
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (SOCKET)(~0)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR            (-1)
#endif
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#endif


//是否使用数据包头长度
#ifndef USING_PACKAGE_HEAD_LENGTH
#define USING_PACKAGE_HEAD_LENGTH 1
#endif
//超时时间
#ifndef SOCKET_SOTIMEOUT
#define SOCKET_SOTIMEOUT 15.0f
#endif
//心跳间隔时间
#ifndef HEART_BEAT_DELAY_TIME
#define HEART_BEAT_DELAY_TIME 5.0f
#endif


//接受数据间隔时间
#ifndef RECIVE_DATA_DELAY_TIME
#define RECIVE_DATA_DELAY_TIME 30
#endif

//读取缓冲区大小
#ifndef SOCKET_READ_BUFFER_SIZE
#define SOCKET_READ_BUFFER_SIZE 8192
#endif
//内存字节序是否反序
#ifndef MEMORYTYPE_REVERSE
#define MEMORYTYPE_REVERSE 0
#endif
//是否一帧内收发所有数据（0 则一帧收发一次数据，1则读完）
#ifndef HANDLE_ON_SINGLE_FRAME
#define HANDLE_ON_SINGLE_FRAME 0
#endif

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//写网络数据：0 无日志  ；1 打印接收长度 ；2 打印长度加信息
#define DEBUG_LOG_WRITE 2


//读网络数据：0 无日志  ；1 打印接收长度 ；2 打印长度加信息
#define DEBUG_LOG_READ 2
#else

#define DEBUG_LOG_WRITE 1
#define DEBUG_LOG_READ 1

#endif

//WWBuffer默认构造大小
#ifndef WWBuffer_ORGINAL_SIZE
#define WWBuffer_ORGINAL_SIZE 512
#endif
//socket连接失败重试次数
#ifndef SOCKET_CONNECT_FAIL_COUNT
#define SOCKET_CONNECT_FAIL_COUNT 1
#endif
//http获取socket地址连接失败重试次数
#ifndef HTTP_GETSOCKET_FAIL_COUNT
#define HTTP_GETSOCKET_FAIL_COUNT 1
#endif


#endif //__WWNET_MACROS_H__
