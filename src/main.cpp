#include "precomp.h"
#include "cbenchanted.h"

int main(int argc, char** argv) {
	CBEnchanted cb;
    cb.init(string("test.exe"));
	cb.run();
	cb.cleanup();
	
	return 0;
}

