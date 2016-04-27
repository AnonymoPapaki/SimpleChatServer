#include <WinSock2.h>
#include <map>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
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

struct ClientInfo
{
	std::string Name;
	bool IsConnected;
	bool RemoveFlag;
};


SOCKET listener;
std::map<SOCKET, ClientInfo> clients;


void InitListener(int port);
void ClientRequest(SOCKET sock);
void SendToAll(const std::string& sender, const std::string& msg);
void SendTo(const std::string& sender, const std::string& to, const std::string& msg);
void SendOnlineList();

int main(int argc, char** argv)
{
	
	InitListener(9090);
	for (;;)
	{
		std::vector<SOCKET> forRemove;
		fd_set fds = { 0 };

		FD_SET(listener, &fds);
		for (auto client : clients)
			FD_SET(client.first, &fds);

		select(0, &fds, NULL, NULL, NULL);

		if (FD_ISSET(listener, &fds))
			clients[accept(listener, NULL, NULL)] = ClientInfo{ "",false,false };

		for (auto client : clients)
			if (FD_ISSET(client.first, &fds))
				ClientRequest(client.first);

		
		for (auto client : clients)
			if (client.second.RemoveFlag)
				forRemove.push_back(client.first);

		for (auto sock : forRemove)
		{
			closesocket(sock);
			clients.erase(sock);
		}
		if (forRemove.size() > 0)
			SendOnlineList();
	}
	return 0;
}

void InitListener(int port)
{
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = 0;

	listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(listener, (sockaddr*)&addr, sizeof addr);
	listen(listener, SOMAXCONN);
}

void ClientRequest(SOCKET sock)
{
	char buf[1024] = { 0 };
	int size;
	size = recv(sock, buf, sizeof buf, 0);
	if (size <= 0)
	{
		clients[sock].RemoveFlag = true;
		return;
	}
	std::string packet(buf);
	std::istringstream ss(packet);
	std::string header,body,tmp;

	std::getline(ss, header, '$');
	std::getline(ss, body, '$');

	std::istringstream ssHeader(header);

	ssHeader >> tmp;
	if (tmp == "setName")
	{
		ssHeader >> clients[sock].Name;
		clients[sock].IsConnected = true;
		SendOnlineList();
	}
	else if (tmp == "send")
	{
		ssHeader >> tmp;
		if (tmp == "all")
			SendToAll(clients[sock].Name, body);
		else
			SendTo(clients[sock].Name, tmp, body);
	}
}

void SendToAll(const std::string& sender, const std::string& msg)
{
	std::stringstream ss;
	ss << "msg all\n"
		<< "sender " << sender
		<< "$"
		<< msg;
	for (auto client : clients)
		send(client.first, ss.str().c_str(), ss.str().size(), MSG_OOB);
}
void SendTo(const std::string& sender, const std::string& to, const std::string& msg)
{
	std::stringstream ss;
	ss << "msg pm\n"
		<< "sender " << sender
		<< "$"
		<< msg;
	for (auto client : clients)
		if (client.second.Name == to)
			send(client.first, ss.str().c_str(), ss.str().size(), MSG_OOB);
}

void SendOnlineList()
{
	std::stringstream ss;
	ss << "list$";
	for (auto client : clients)
		ss << client.second.Name << "\n";
	for (auto client : clients)
		send(client.first, ss.str().c_str(), ss.str().size(), MSG_OOB);
}
