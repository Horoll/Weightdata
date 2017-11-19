#include "stdafx.h"
#include "Serial.h"

// readEx callback function
void __stdcall readCallBack(unsigned long dwErr, unsigned long cbRead, LPOVERLAPPED lpOverlapped) {
	char lpMsgBuf[2048];
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)lpMsgBuf,
		0, NULL);
}



Serial::Serial()
{
	this->hCom = INVALID_HANDLE_VALUE;
	this->timeOuts.ReadIntervalTimeout = 20;
	this->timeOuts.ReadTotalTimeoutConstant = 2000;
	this->timeOuts.ReadTotalTimeoutMultiplier = 0;
	this->timeOuts.WriteTotalTimeoutConstant = 1000;
	this->timeOuts.WriteTotalTimeoutMultiplier = 0;
	this->dcb = { 0 };
}



Serial::~Serial()
{
		this->close();
}

bool Serial::open(tstring com) {
	this->close();
	if (com != L"undefined" && this->com != com)
		this->com = com;
	hCom = CreateFile(
		this->com.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,

		// FILE_FLAG_OVERLAPPED
		0,
		NULL);
	if (hCom != INVALID_HANDLE_VALUE) {
		unsigned long dwErrorFlags = 0;
		SetupComm(hCom, INPUT_BUFFER_SIZE, OUTPUT_BUFFER_SIZE);
		SetCommTimeouts(hCom, &timeOuts);
		PurgeComm(hCom, dwErrorFlags);
		return true;
	}
	return false;
}

bool Serial::set(SCP const scp) {
	if (hCom != INVALID_HANDLE_VALUE) {
		if (GetCommState(hCom, &dcb)) {
			this->scp = scp;
			dcb.BaudRate = this->scp.baudRate;
			dcb.Parity = this->scp.parity;
			dcb.StopBits = this->scp.stopBits;
			dcb.ByteSize = this->scp.byteSize;
			if (SetCommState(hCom, &dcb))
				return true;
		}
	}
	return false;
}

unsigned long Serial::read(unsigned long dwRead) {
	unsigned long lpRead = 0, dwErrorFlags;
	queue<unsigned char>::size_type bufSize;
	OVERLAPPED osReader = { 0 };
	COMSTAT ComStat;
	if (hCom != INVALID_HANDLE_VALUE) {
		memset(lpBuffer, 0, sizeof(lpBuffer));
		osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		ClearCommError(hCom, &dwErrorFlags, &ComStat);
		bufSize = readBuf.size();
		if (ReadFile(hCom, lpBuffer, dwRead, &lpRead, NULL) && (bufSize < INPUT_BUFFER_SIZE)) {
			for (unsigned long i = 0; i < lpRead; i++) {
				readBuf.push(lpBuffer[i]);
			}
		}
	}
	return lpRead;
}

unsigned long Serial::write(unsigned char data[], unsigned long n) {
	unsigned long dwWritten = 0;
	OVERLAPPED osWrite = { 0 };
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hCom != INVALID_HANDLE_VALUE) {
		WriteFile(
			hCom,
			data,
			n,
			&dwWritten,
			&osWrite);
	}
	return dwWritten;
}

/**
* readEx �����첽��ȡ�������˺������첽��ȡ��ǰ���ڻ��������ݲ�����ָ������ queue<char> readBuf �л��档
*        �����ڶ���������������������������ݣ��������ؿ��ƣ�Ϊ���ɿ���ȡ
* @param
*
*
*/
void Serial::readEx(unsigned long dwRead) {
	unsigned long dwErrorFlags;
	COMSTAT ComStat;

	// OVERLAPPED ������첽������û�����ã�������Ҫ����˲���
	OVERLAPPED lpOverlapped = { 0 };
	if (hCom != INVALID_HANDLE_VALUE) {

		// ����ͬ�� OVERLAPPED ���д��
		// lpOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		memset(lpBuffer, 0, sizeof(lpBuffer));
		ClearCommError(hCom, &dwErrorFlags, &ComStat);
		ReadFileEx(hCom, lpBuffer, dwRead, &lpOverlapped, readCallBack);
		WaitForSingleObjectEx(hCom, 5000, true);
	}
}

bool Serial::close() {
	if (hCom != INVALID_HANDLE_VALUE) {
		if (CloseHandle(hCom)) {
			hCom = INVALID_HANDLE_VALUE;
			return true;
		}
		else
			return false;
	}
	return true;
}
