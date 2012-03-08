#include "precomp.h"
#include "cbenchanted.h"

int main(int argc, char** argv) {
	CBEnchanted cb;

	std::string exe = "karttaTesti.exe";
	//if (argc == 2) exe = std::string(argv[1]);
	if (cb.init(exe))
	{
		cb.run();
		cb.cleanup();
	}
	
	return 0;
}

