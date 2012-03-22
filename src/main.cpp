#include "precomp.h"
#include "cbenchanted.h"

int main(int argc, char** argv) {
	CBEnchanted cb;

	std::string exe = "karttaTesti2.exe";
	if (argc == 2) exe = std::string(argv[1]);
	if (cb.init(exe))
	{
		cb.run();
		cb.cleanup();
	}
	al_uninstall_system();
	
	return 0;
}

