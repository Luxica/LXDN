/****************************************************************************
****************************************************************************/
#include "WWSocket.h"
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static bool bStartup = false;
#endif

WWSocket::WWSocket()
: m_uSocket(INVALID_SOCKET)
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
if( !bStartup ) 
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	bStartup = true;
}
#endif
}

WWSocket::~WWSocket()
{
	ccClose();
}

bool WWSocket::ccInit()
{
	if( m_uSocket != INVALID_SOCKET )
	{
		ccClose();
	}

	m_uSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( m_uSocket == INVALID_SOCKET )
	{
		CCLOGERROR("create socket failed");
		return false;
	}

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	unsigned long ul = 1;
	int nRet = ioctlsocket(m_uSocket, FIONBIO, (unsigned long*)&ul);
	if( nRet == SOCKET_ERROR )
	{
		CCLOGERROR("set unblocking failed");
		ccClose();
		return false;
	}
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	int nFlags = fcntl(m_uSocket, F_GETFL, 0);
    int nRet = fcntl(m_uSocket, F_SETFL, nFlags | O_NONBLOCK);
	if( nRet == SOCKET_ERROR )
	{
		CCLOGERROR("set unblocking failed");
		ccClose();
		return false;
	}
#endif

	int nNoDelay = 1;
	if( setsockopt(m_uSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&nNoDelay, sizeof(nNoDelay)) == SOCKET_ERROR )
	{
		CCLOGERROR("set nodelay failed");
		ccClose();
		return false;
	}

	return true;
}

void WWSocket::ccClose()
{
	if( m_uSocket != INVALID_SOCKET )
	{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
		closesocket(m_uSocket);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		close(m_uSocket);
#endif
		m_uSocket = INVALID_SOCKET;
		log("m_uSocket=%d",m_uSocket);
	}
}

bool WWSocket::ccConnect()
{
    if(strlen(m_oInetAddress.getIp()) == 0 || m_oInetAddress.getPort() <= 0) {
        CCAssert(strlen(m_oInetAddress.getIp()) && m_oInetAddress.getPort() != 0, "_connect");
        return false;
    }

	if( m_uSocket == INVALID_SOCKET )
	{
		if(!ccInit())
		{
			return false;
		}
	}

	int nRet = ::connect(m_uSocket, m_oInetAddress, m_oInetAddress.getLength());
	if( nRet == 0 )
	{
		return true;
	}
	else
	{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
		int nError = WSAGetLastError();
		if( nError ==  WSAEWOULDBLOCK )
		{
			return true;
		}
		else
		{
			return false;
		}
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		if( nRet == SOCKET_ERROR && errno == EINPROGRESS )
		{
			return true;
		}
		else
		{
			return false;
		}
#endif
	}
}

void WWSocket::ccDisconnect()
{
	if( m_uSocket != INVALID_SOCKET )
	{
#if 0
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
		shutdown(m_uSocket, SD_BOTH);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        shutdown(m_uSocket, SHUT_RDWR);
#endif
#endif
		ccClose();
	}
}

void WWSocket::setInetAddress(const WWInetAddress& oInetAddress)
{
	m_oInetAddress = oInetAddress;
}

int WWSocket::ccRead(char* _buff, int _len)
{
	if (m_uSocket == INVALID_SOCKET)
	{
		log("WWSocket::ccRead....error!");
		return eSocketIoError;
	}
		

	return ::recv(m_uSocket, _buff, _len, 0);
}

int WWSocket::ccWrite(char* _buff, int _len)
{
	if (m_uSocket == INVALID_SOCKET)
	{
		log("WWSocket::ccWrite....error!");
		return eSocketIoError;
	}
		
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	return ::send(m_uSocket, _buff, _len, 0);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    return ::send(m_uSocket, _buff, _len, SO_NOSIGPIPE);
#endif
}

bool WWSocket::ccIsReadable()
{

	if( m_uSocket == INVALID_SOCKET )
			return false;

	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(m_uSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if( select((int)(m_uSocket + 1), &fd, NULL, NULL, &tv) > 0 )
	{
		if( FD_ISSET(m_uSocket, &fd) )
		{
			return true;
		}
	}

	return false;
}

bool WWSocket::ccIsWritable()
{

	if( m_uSocket == INVALID_SOCKET )
				return false;

	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(m_uSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if( select((int)(m_uSocket + 1), NULL, &fd, NULL, &tv) > 0 )
	{
		if( FD_ISSET(m_uSocket, &fd) )
		{
			return true;
		}
	}

	return false;
}

int WWSocket::ccIsConnected()
{

	if( m_uSocket == INVALID_SOCKET )
				return eSocketConnectFailed;

	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(m_uSocket, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if( select((int)(m_uSocket + 1), NULL, &fd, NULL, &tv) > 0 )
	{
		if( FD_ISSET(m_uSocket, &fd) )
		{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )

			return eSocketConnected;

#elif( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )

			int nError;
			socklen_t len = sizeof(nError);
			if( getsockopt(m_uSocket, SOL_SOCKET, SO_ERROR, &nError, &len) < 0 )
			{
				return eSocketConnectFailed;
			}
			if( nError == ECONNREFUSED || nError == ETIMEDOUT )
			{
				return eSocketConnectFailed;
			}
			return eSocketConnected;

#endif
		}
	}
	return eSocketConnecting;
}

