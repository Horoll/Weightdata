#pragma once

#include "Connect.h"
#include "Modbus.h"
#include "cmdline.h"

typedef struct {
	string ins;
	map<string, string> params;
}CMD;

class Dos
{
public:
	Dos();
	~Dos();

public:
	void listening();
	void cmd_listening();

private:
	void cmd_init();
	void run(const string &cmd);
	bool cmdParsing(const string &line);
	void cmd_parser(const string &line);
	void cmdOpen();
	void cmdLink();
	void cmdDis();

private:
	cmdline::parser p;
	CMD cmd;
	error e;
};

