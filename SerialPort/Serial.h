#pragma once

#include "Source.h"

#define INPUT_BUFFER_SIZE 1024
#define OUTPUT_BUFFER_SIZE 1024
#define READ_BUFFER_SIZE 2048
#define READ_SIZE 1024

typedef struct SCP{
	unsigned long baudRate;

	//ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS
	unsigned char stopBits;
	unsigned char byteSize;

	//EVENPARITY,MARKPARITY,NOPARITY,ODDPARITY
	unsigned char parity;
	SCP (unsigned long baudRate = 9600, unsigned char stopBits = 2, unsigned char byteSize = 8, unsigned char parity = 0) :
		baudRate(baudRate), stopBits(stopBits), byteSize(byteSize), parity(parity){}
}

// SCP means Serial control parameters
SCP;


class Serial
{
public:
	Serial();
	Serial(const tstring com, const SCP scp = SCP()) :
		com(com), scp(scp){
		this->hCom = INVALID_HANDLE_VALUE;
		this->timeOuts.ReadIntervalTimeout = 20;
		this->timeOuts.ReadTotalTimeoutConstant = 2000;
		this->timeOuts.ReadTotalTimeoutMultiplier = 0;
		this->timeOuts.WriteTotalTimeoutConstant = 1000;
		this->timeOuts.WriteTotalTimeoutMultiplier = 0;
		this->dcb = { 0 };
	}
	~Serial();

public:
	bool open(tstring com = L"undefined");
	bool set(SCP const scp);
	unsigned long read(unsigned long dwRead);
	void readEx(unsigned long dwRead);
	unsigned long write(unsigned char data[], unsigned long n);
	int writeEx(unsigned char content[]);
	bool close();

public:
	queue<unsigned char> readBuf;

public:
	tstring com;
	SCP scp;
	HANDLE hCom;
	COMMTIMEOUTS timeOuts;
	DCB dcb;
	unsigned char lpBuffer[READ_SIZE];
};

