#pragma once
#include "Source.h"

typedef struct {
	int code;
	std::string mes;
}error;

class Fun
{
public:
	static void calccrc(unsigned char data[], unsigned long n);
	static void calc_complement(unsigned char arr[], unsigned long n);
	static long calc_num(unsigned char arr[]);
	static void split_str(const std::string& s, std::vector<std::string>& v, const std::string& c);
	static wstring str_to_wstr(const string &str);
	static void sleep(const unsigned long &ms);
};


