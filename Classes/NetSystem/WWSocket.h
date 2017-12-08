/****************************************************************************
****************************************************************************/
#ifndef __WW_SOCKET_H__
#define __WW_SOCKET_H__

#include "cocos2d.h"
#include "WWNetMacros.h"
#include "WWInetAddress.h"
#include "WWBuffer.h"
#include <string>


/**
 * enum   : WWSocketStatus
 * descpt : socket status define
 */
enum WWSocketStatus
{
	eSocketConnected			=  1,
	eSocketConnecting			=  2,
	eSocketDisconnected			=  3,       //主动关闭连接
	eSocketConnectFailed		=  4,
	eSocketConnectTimeout		=  5,
	eSocketNoReceiveInTime		=  6,		//接收数据超时
	eSocketDisconnectedForBg	=  7,		//进入后台关闭
	eSocketHandshakingFailed 	=  8,		//握手失败
	eSocketIoClosed				=  0,
	eSocketIoError				= -1,
	eSocketCreateFailed			= -2,
};

/**
 * calss  : WWSocket
 * descpt : socket manager object
 */
class WWSocket
{
public:
	WWSocket();
	virtual ~WWSocket();
	bool ccInit();
	int  ccRead(char* _buff, int _len);
	int  ccWrite(char* _buff, int _len);
	int  ccIsConnected();
	bool ccIsReadable();
	bool ccIsWritable();
	void ccClose();
	bool ccConnect();
	void ccDisconnect();
	void setInetAddress(const WWInetAddress& oInetAddress);

protected:
	SOCKET m_uSocket;
	WWInetAddress m_oInetAddress;
};



#endif //__CCNET_SOCKET_H__
