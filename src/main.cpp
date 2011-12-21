#include "precomp.h"
#include "cbenchanted.h"

#ifdef WIN32
#include <Windows.h>
int CALLBACK WinMain(__in  HINSTANCE hInstance,__in  HINSTANCE hPrevInstance,__in  LPSTR lpCmdLine,__in  int nCmdShow ) {
#else
int main(int argc, char** argv) {
#endif
	CBEnchanted cb;
    cb.init(string("pallerot.exe"));
	cb.run();
	cb.cleanup();
	
	return 0;
}

