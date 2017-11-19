#include "stdafx.h"
#include "Fun.h"

void Fun::calccrc(unsigned char data[], unsigned long n) {
	unsigned int crc = 0xffff,i;
	unsigned char j, TT;
	for (i = 0; i < n - 2; i++) {
		crc = crc ^ data[i];
		for (j = 0; j < 8; j++)
		{
			TT = crc & 1;
			crc = crc >> 1;
			crc = crc & 0x7fff;
			if (TT == 1)
				crc = crc ^ 0xa001;
			crc = crc & 0xffff;
		}
	}
	data[n - 2] = crc & 0xff;
	data[n - 1] = crc / 0x100;
}

void Fun::calc_complement(unsigned char arr[], unsigned long n) {
	unsigned long t = n;
	unsigned char carry = 0x00, i, j;
	while (n--) {
		i = ~arr[n];
		j = carry + (n == t - 1 ? 0x01 : 0x00);
		arr[n] = i + j;
		carry = (i == 0xFF && j >= 0x01) ? 0x01 : 0x00;
	}
	arr[0] |= 0x80;
}

long Fun::calc_num(unsigned char arr[]) {
	long num = 0;
	int n = sizeof(arr) / sizeof(unsigned char);
	for (int i = 0; i < n - 1; i++) {
		num += arr[i];
		num = num << 8;
	}
	num += arr[n -1];
	if (arr[0] & 0x80) {
		num = num << (sizeof(long) - sizeof(unsigned char) * n);
		num = num >> (sizeof(long) - sizeof(unsigned char) * n);
	}
	return num;
}


void Fun::split_str(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	v.clear();
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		if (s.substr(pos1, pos2 - pos1).length() > 0)
			v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}


wstring Fun::str_to_wstr(const string &str) {
	std::wstring wstr(str.length(), L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	return wstr;
}

void Fun::sleep(const unsigned long &ms) {
	std::chrono::milliseconds dura(ms);
	std::this_thread::sleep_for(dura);
}