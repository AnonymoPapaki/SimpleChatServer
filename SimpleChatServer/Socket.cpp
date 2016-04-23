#include "Socket.h"

struct _NeverUseThis_StartUpAndCleanUp
{
	_NeverUseThis_StartUpAndCleanUp()
	{
		WSADATA data;
		WSAStartup(MAKEWORD(2, 2), &data);
	}
	~_NeverUseThis_StartUpAndCleanUp()
	{
		WSACleanup();
	}
} __NeverUseThis_StartUpAndCleanUp;