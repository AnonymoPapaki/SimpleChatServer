#pragma once
#include <WinSock2.h>
#include <string>

class Socket
{
public:
	Socket();
	Socket(SOCKET socketId);
	~Socket();

	SOCKET getSocketId();
	bool Recieve(std::string& str);
	bool Send(const std::string& str);
	void Close();

private:
	SOCKET _socket;
};