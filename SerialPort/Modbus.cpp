#include "stdafx.h"
#include "Modbus.h"
#ifdef _WIN32  
#include <winsock2.h>  
#include <time.h>  
#else  
#include <sys/time.h>  
#endif  

unsigned long long GetCurrentTimeMsec()
{
#ifdef _WIN32  
	struct timeval tv;
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tv.tv_sec = clock;
	tv.tv_usec = wtm.wMilliseconds * 1000;
	return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#else  
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#endif  
}

Modbus::Modbus() : Serial()
{
	this->rec.ref_value = 0;
}


Modbus::~Modbus()
{
}

void Modbus::exec(const tstring com, const vector<unsigned char> addrs, const SCP scp, const unsigned short frequency) {
	Modbus md;
	Modbus::isEndingRead = false;
	Modbus::isDisplay = false;
	md.frequency = frequency;
	if (md.open(com) && md.set(scp)) {
		md.getWeight(addrs);
		md.close();
	}
	else
	    cout << "Error: open serial port failed" << endl;
	return;
}

void Modbus::getWeight(const vector<unsigned char> &addrs) {
	int i;
	unsigned short j, data_length;

	// ¶Á±£³Ö¼Ä´æÆ÷µÄ RTU ÃüÁî
	unsigned char writeBuf[] = { 0x00,0x03,0x00,0x28,0x00,0x02,0x00,0x00 };
	unsigned char weight[4];
	unsigned long bytes;
	time_t t;
	unsigned long ms = 1000 / this->frequency;
	int cnt = 0;
	while (!(Modbus::isEndingRead)) {
		t = GetCurrentTimeMsec();
		for (i = addrs.size()-1; i >= 0; i--) {
			writeBuf[0] = addrs.at(i);
			Fun::calccrc(writeBuf, WRITE_MIN_LENGTH);
			this->write(writeBuf, WRITE_MIN_LENGTH);
			bytes = this->read(WEIGHT_LENGTH + 5);
			if (bytes == WEIGHT_LENGTH + 5 && readBuf.size() >= WEIGHT_LENGTH + 5) {
				rec.addr = readBuf.front(); readBuf.pop();
				readBuf.pop();
				data_length = readBuf.front(); readBuf.pop();
				for (j = 0; j < data_length; j++) {
					weight[j] = readBuf.front(); readBuf.pop();
				}
				rec.value = Fun::calc_num(weight);
				rec.time = t;
				rec.id = (rec.time << 2) + rec.addr;
				readBuf.pop();
				readBuf.pop();
				if(Modbus::recBuf.size() <= RECORD_BUF)
				    Modbus::recBuf.push(rec);
				if (Modbus::isDisplay)
					Modbus::display(rec);
			}
		}
		Fun::sleep(ms);
	}
}

void Modbus::setRefValue(long refValue) {
	this->rec.ref_value = refValue;
}

void Modbus::setFrequency(unsigned short frequency) {
	this->frequency = frequency;
}

void Modbus::setScp(SCP &scp) {
	this->set(scp);
}


void Modbus::display(const record &rec) {
	cout << "id: " << rec.id;
	cout << "  addr: " << rec.addr;
	cout << "  weight(g): " <<setw(8)<<setiosflags(ios::left)<< rec.value;
	cout << "  ref(g): " << rec.ref_value;
	cout << "  time: " << rec.time << endl;
}


bool Modbus::isEndingRead = false;
bool Modbus::isDisplay = false;
queue<record> Modbus::recBuf;