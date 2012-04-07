#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include "precomp.h"
#include "cbenchanted.h"

int main(int argc, char** argv) {
	CBEnchanted cb;
	#ifdef TEST
	if (argc == 2) {
		if (cb.init(string(argv[1]))) {
			cb.run();
			cb.cleanup();
		}
		al_uninstall_system();
	}
	else {
		cerr << "Usage: CBEnchanted file.exe" << endl;
		return -1;
	}
	#else
		fs::path path(fs::initial_path<fs::path>());
		path = fs::system_complete(fs::path(argv[0]));

		cb.init(path.generic_string());
		cb.run();
		cb.cleanup();
	#endif


	return 0;
}

