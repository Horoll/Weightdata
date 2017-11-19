#include "stdafx.h"
#include "Dos.h"
#include "Fun.h"

Dos::Dos()
{
	this->cmd_init();
}


Dos::~Dos()
{
}


void Dos::cmd_init() {
	p.set_program_name("open");
	p.add<int>("port", 'p', "port number", true, 80, cmdline::range(1, 65535));
}


void Dos::listening() {
	string line;
	while (true) {
		getline(cin, line);
		if (line == "exit")
			break;
		else
			this->run(line);
	}
	std::exit(0);
}
void Dos::cmd_listening() {
	std::string line;
	std::vector<std::string> cmd;
	while (true) {
		getline(cin, line);
		Fun::split_str(line, cmd, " ");
		if (p.parse_check(cmd)) {
			p.exist("open");
			this->cmd_parser(line);
		}
		
	}
	std::exit(0);
}

void Dos::cmd_parser(const string &line) {

}

void Dos::run(const string &cmd) {
	if(cmd == "end")
		Modbus::isEndingRead = true;
	else if (this->cmdParsing(cmd)) {
		if (this->cmd.ins == "open") {
			this->cmdOpen();
		}
		else if (this->cmd.ins == "dis") {
			this->cmdDis();
		}
		else if (this->cmd.ins == "link") {
			this->cmdLink();
		}
	}
	if (this->e.code != 0)
		cout <<"Error: "<< this->e.mes << endl;
}



bool Dos::cmdParsing(const string &line) {
	string str = "";
	string parse = line;
	vector<string> ss;
	bool flag = false;
	parse.push_back(' ');
	for (auto c : parse) {
		if (c != ' ') {
			str.push_back(c);
			flag = true;
		}
		else if(flag && c == ' '){
			ss.push_back(str);
			str.clear();
			flag = false;
		}		
	}
	this->cmd.ins = ss.front();
	ss.erase(ss.begin());
	auto size = ss.size();
	if (size % 2) {
		this->e.code = 2;
		this->e.mes = "params error";
		return false;
	}
	else {
		this->cmd.params.clear();
		for (unsigned int i = 0; i < size; i += 2)
			this->cmd.params.insert(make_pair(ss.at(i), ss.at(i + 1)));
	}
	this->e.code = 0;
	return true;
}


void Dos::cmdOpen() {
	if (this->cmd.params.count("-h") == 1) {
		SCP scp = SCP();
		unsigned short frequency = 3;
		vector<unsigned char> addrs;
		addrs.push_back(0x08);
		string str;
		auto iter = this->cmd.params.find("-b");

		if (iter != this->cmd.params.end())
			scp.baudRate = atol(iter->second.c_str());
		iter = this->cmd.params.find("-p");
		if (iter != this->cmd.params.end())
			scp.parity = iter->second.at(0);
		iter = this->cmd.params.find("-s");
		if (iter != this->cmd.params.end())
			scp.stopBits = iter->second.at(0);
		iter = this->cmd.params.find("-d");
		if (iter != this->cmd.params.end())
			scp.byteSize = iter->second.at(0);
		iter = this->cmd.params.find("-f");
		if (iter != this->cmd.params.end())
			frequency = atoi(iter->second.c_str());
		iter = this->cmd.params.find("-addr");
		if (iter != this->cmd.params.end()) {
			vector<string> v;
			Fun::split_str(iter->second, v, ":");
			addrs.clear();
			for (auto i : v)
				addrs.push_back(atoi(i.c_str()));
		}
		std::thread t(Modbus::exec, Fun::str_to_wstr(this->cmd.params.find("-h")->second), addrs, scp, frequency);
		t.detach();
		this->e.code = 0;
	}
	else{
		this->e.code = 1;
		this->e.mes = "missing parameter '-h'";
	}
}


void Dos::cmdLink() {
	auto iter = cmd.params.find("-s");
	if (iter == this->cmd.params.end()) {
		this->e.code = 4;
		this->e.mes = "missing parameter '-s'";
		return;
	}
	else if (iter->second == "shut") {
		Connect::isSending = false;
	}
	else if (iter->second == "start") {
		string hostname;
		iter = cmd.params.find("-host");
		if (iter == this->cmd.params.end()) {
			this->e.code = 5;
			this->e.mes = "missing parameter '-host'";
			return;
		}
		hostname = iter->second;
		iter = cmd.params.find("-p");
		if (iter == this->cmd.params.end()) {
			this->e.code = 6;
			this->e.mes = "missing parameter '-p'";
			return;
		}
		std::thread t(Connect::exec, hostname, atoi(iter->second.c_str()));
		t.detach();
		this->e.code = 0;
	}
	
}


void Dos::cmdDis() {
	auto iter = cmd.params.find("-v");
	if (iter != this->cmd.params.end()) {
		this->e.code = 0;
		if (iter->second == "true")
			Modbus::isDisplay = true;
		else if (iter->second == "false")
			Modbus::isDisplay = false;
		else {
			this->e.code = 3;
			this->e.mes = "unknown parameter value";
		}
	}	
}





