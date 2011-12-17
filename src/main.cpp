#include "precomp.h"
#include "cbemu.h"

int mainn(const std::vector<CL_String> &args) {
	try
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;

		CL_SetupGL setup_gl;
		
		CL_ConsoleLogger logger;
		
		CBEmu *cb = new CBEmu;
		cb->init(string("pallerot.exe"));
		cb->run();
		cb->cleanup();
		
		return 0;
	}
	catch(CL_Exception &exception)
	{
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}
}

CL_ClanApplication app(mainn);
