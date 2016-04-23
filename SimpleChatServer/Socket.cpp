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


Socket::Socket()
	: _socket(INVALID_SOCKET)
{

}
Socket::Socket(SOCKET sock)
	: _socket(sock)
{
	
}
Socket::~Socket()
{
	if (this->_socket != INVALID_SOCKET)
		closesocket(this->_socket);
}
SOCKET Socket::GetSocketId()
{
	return this->_socket;
}
bool Socket::Recieve(std::string& str)
{
	throw;
}
bool Socket::Send(const std::string& str)
{
	throw;
}
void Socket::Close()
{
	closesocket(this->_socket);
	this->_socket = INVALID_SOCKET;
}