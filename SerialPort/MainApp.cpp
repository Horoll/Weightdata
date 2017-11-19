#include "stdafx.h"
#include "MainApp.h"
#include "Dos.h"

void MainApp::exec() {
	Dos dos;
	dos.listening();
	std::exit(0);
}
