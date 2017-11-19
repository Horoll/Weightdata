#pragma once

#include "Source.h"
#include "Modbus.h"
class Connect
{
public:
	Connect();
	~Connect();

public:
	static void exec(const string server, const unsigned short port);

private:
	bool link(const string &server, const unsigned short &port);
	bool send(const string &sendbuf);
	void disconnect();

private:
	WSADATA wsaData;
	SOCKET sclient;
	sockaddr_in serAddr;

public:
	static bool isSending;
};

