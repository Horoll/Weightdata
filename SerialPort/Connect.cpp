#include "stdafx.h"
#include "Connect.h"


Connect::Connect()
{
	sclient = INVALID_SOCKET;
}


Connect::~Connect()
{
	this->disconnect();
}


bool Connect::link(const string &server,const unsigned short &port) {
	sclient = INVALID_SOCKET;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		WSACleanup(); return false;
	}
		
	sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET){
		WSACleanup(); return false;
	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
	serAddr.sin_addr.S_un.S_addr = inet_addr(server.c_str());
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		WSACleanup(); sclient = INVALID_SOCKET; return false;
	}
	return true;
}


bool Connect::send(const string &sendbuf) {
	return ::send(sclient, sendbuf.c_str(), sendbuf.length(), 0) == SOCKET_ERROR ? false : true;
}
//open -h com5 -addr 08:05:06
//link -s start -host 10.2.130.178 -p 9000

void Connect::disconnect() {
	if (sclient != INVALID_SOCKET){
		shutdown(sclient, SD_SEND);
		closesocket(sclient);
		sclient = INVALID_SOCKET;
	}
	WSACleanup();
}


void Connect::exec(const string server, const unsigned short port) {
	Connect con;
	if (!con.link(server, port)) {
		cout << "Error: fail to connect to the server" << endl;
		return;
	}
	string data;
	unsigned long ms = 1000;
	Connect::isSending = true;
	while (Connect::isSending) {

		if (Modbus::recBuf.size() > 0) {
			auto iter = Modbus::recBuf.front();

				data = std::to_string(iter.addr) + ":" + std::to_string(iter.value) + ":" + std::to_string(iter.ref_value) + ":" + std::to_string(iter.time);
		
				if (con.send(data)) {
					Modbus::recBuf.pop();
					cout << data << endl;
				}
				else {
					cout << "Error: fail to send data" << endl;
					Fun::sleep(5000);
				}
		}
		
		Fun::sleep(ms);
	}
	con.disconnect();
}

bool Connect::isSending = true;