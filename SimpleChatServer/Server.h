#pragma once
#include "TCPServer.h"

class Server
{
public:
	Server();
	~Server();
	void Run(int port);
private:
	TCPServer tcp;
};