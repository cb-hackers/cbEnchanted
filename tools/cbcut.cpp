#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

using namespace std;

int main(int argc, char **argv) {
	if (argc != 3) {
		cerr << "Usage: cbcut myCoolBasicProgram.exe cbEnchanted.exe" << endl;
		return -1;
	}
	string s1 = string(argv[1]);
	string s2 = string(argv[2]);

	int32_t startPos;
	int32_t endPos;

	uint32_t size;

	ifstream input(s1.c_str(), ios::binary);
	input.seekg(-4, ios::end);
	endPos = input.tellg();
	endPos += 4;
	input.read((char *)(&startPos), 4);
	size = startPos;
	startPos = endPos - startPos;
	input.seekg(startPos);
	char *code = new char [size];
	input.read(code, size);
	input.close();

	ofstream output(s2.c_str(), ios::binary | ios::out | ios::ate | ios::app);
	output.write(code, size);
	output.close();

	delete code;

}
