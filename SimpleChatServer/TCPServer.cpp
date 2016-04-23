#include "TCPServer.h"

TCPServer::TCPServer()
{
	Socket::Socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
}
TCPServer::~TCPServer()
{

}
void TCPServer::Start(int port)
{
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = 0;
	bind(this->GetSocketId(), (sockaddr*)&addr, sizeof addr);
	listen(this->GetSocketId(), SOMAXCONN);
}
Socket TCPServer::Accept()
{
	return Socket(accept(this->GetSocketId(), NULL, NULL));
}
void TCPServer::Close()
{
	Socket::Close();
}