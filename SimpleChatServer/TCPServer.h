#pragma once
#include "Socket.h"

class TCPServer 
	: public Socket
{
public:
	TCPServer();
	~TCPServer();
	void Start(int port);
	Socket Accept();
	void Close();
	
};