#pragma once
#include<map>

class Event
{
public:
	Event();
	~Event();

public:
	static unsigned long reg();
	static void remove();
	static void set(unsigned long eventId);
	static void get();
};

