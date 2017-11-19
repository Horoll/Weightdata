#pragma once

#include "Serial.h"
#include "Fun.h"

#define WEIGHT_LENGTH 4
#define WRITE_MIN_LENGTH 8
#define RECORD_BUF 2048

typedef struct {
	unsigned long id;    /* ��¼id�ţ�Ψһ  */
	unsigned short addr; /* ͨ����ַ0-99    */
	unsigned long time;  /* ʱ���          */
	long value;          /* ����ֵ ��λ��g  */
	long ref_value;      /* �ο�����ֵ      */
}record;

class Modbus : private Serial
{
public:
	Modbus();
	~Modbus();

public:
	static void exec(const tstring com, const vector<unsigned char> addrs, const SCP scp = SCP(), const unsigned short frequency = 10);

private:
	void getWeight(const vector<unsigned char> &addrs);
	void setRefValue(long refValue);
	void setFrequency(unsigned short frequency);
	void setScp(SCP &scp);
	static void display(const record &rec);

private:
	unsigned short frequency;
	record rec;

public:
	static bool isEndingRead;
	static bool isDisplay;
	static queue<record> recBuf;
};

