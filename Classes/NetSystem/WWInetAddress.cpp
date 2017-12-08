/****************************************************************************

****************************************************************************/
#include "WWInetAddress.h"
USING_NS_CC;

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	#pragma comment(lib, "Ws2_32.lib")
#endif


WWInetAddress::WWInetAddress()
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	sin_len = sizeof(struct sockaddr_in);
	sin_family = AF_INET;
	sin_addr.s_addr = INADDR_ANY;
	sin_port = 0;
	memset(sin_zero, 0, 8);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	sin_family = AF_INET;
	sin_addr.s_addr = INADDR_ANY;
	sin_port = 0;
	memset(sin_zero, 0, 8);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	sin_family = AF_INET;
	sin_addr.s_addr = INADDR_ANY;
	sin_port = 0;
	memset(sin_zero, 0, 8);
#endif
}

WWInetAddress::WWInetAddress(const char* ip, unsigned short port)
{
	sin_family = AF_INET;
	sin_addr.s_addr = inet_addr(ip);
	sin_port = htons(port);
	memset(sin_zero, 0, 8);
}

WWInetAddress::WWInetAddress(const struct sockaddr * addr)
{
	memcpy(&this->sin_family, addr, sizeof(struct sockaddr));
}

WWInetAddress::~WWInetAddress(void)
{
	
}

WWInetAddress::operator struct sockaddr*()
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	return (struct sockaddr *)(&this->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	return (struct sockaddr *)(&this->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	return (struct sockaddr *)(&this->sin_len);
#endif
}

WWInetAddress::operator const struct sockaddr*() const
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	return (const struct sockaddr *)(&this->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	return (const struct sockaddr *)(&this->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	return (const struct sockaddr *)(&this->sin_len);
#endif
}

const char* WWInetAddress::getHostAddress() const
{
	static char addr[64];
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	sprintf_s(addr, 64, "%s:%u", inet_ntoa(sin_addr), getPort());
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	snprintf(addr, 64, "%s:%u", inet_ntoa(sin_addr), getPort());
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	snprintf(addr, 64, "%s:%u", inet_ntoa(sin_addr), getPort());
#endif
	return addr;
}

const char* WWInetAddress::getIp() const
{
	return inet_ntoa(sin_addr);
}

unsigned short WWInetAddress::getPort() const
{
	return ntohs(sin_port);
}

void WWInetAddress::setIp(const char* ip)
{
	sin_addr.s_addr = inet_addr(ip);
}

void WWInetAddress::setIp(unsigned int ip)
{
	sin_addr.s_addr = ip;
}

void WWInetAddress::setPort(unsigned short port)
{
	sin_port = htons(port);
}

void WWInetAddress::setHost(const char* name)
{
	hostent* h = gethostbyname(name);
	if( h != NULL )
	{
		sin_addr.s_addr = *((u_long *)h->h_addr_list[0]);
	}
}

int WWInetAddress::getLength()
{ 
	return sizeof(sockaddr_in); 
}

