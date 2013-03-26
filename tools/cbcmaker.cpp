#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

using namespace std;

int main(int argc, char **argv) {
	if (argc != 4) {
		cout << "Usage: cbcmaker <compiler> <runtime> <new compiler>" << endl;
		return -1;
	}

	string s1 = string(argv[1]); // Compiler
	string s2 = string(argv[2]); // Runtime
	string s3 = string(argv[3]); // Output

	cout << "Opening files" << endl;

	ifstream input1(s1.c_str(), ios::binary); // Compiler
	ifstream input2(s2.c_str(), ios::binary); // Runtime
	ofstream output(s3.c_str(), ios::binary); // Output
	
	if (!input1.is_open()) {
		cerr << "Can't open the old compiler\n";
		return 1;
	}
	if (!input2.is_open()) {
		cerr << "Can't open the runtime file\n";
		return 1;
	}
	if (!output.is_open()) {
		cerr << "Can't open output file\n";
		return 1;
	}

	cout << "Finding runtime location and size" << endl;

	int32_t runtimePos, runtimeSize;

	input1.seekg(-4, ios::end);
	input1.read((char *)(&runtimePos), 4);
	cout << "Old runtime starts at: " << runtimePos << endl;
	input1.seekg(runtimePos + 4);
	input1.read((char *)(&runtimeSize), 4);
	cout << "Old runtime size: " << runtimeSize << endl;

	cout << "Finding size of new runtime" << endl;
	input2.seekg(0, ios::end);
	int32_t rSize = input2.tellg();
	cout << "New runtime size: " << rSize << endl;

	char *c1 = new char [runtimePos + 4];
	char *c2 = new char [rSize];

	input1.seekg(0);
	input1.read(c1, runtimePos + 4);
	input2.seekg(0);
	input2.read(c2, rSize);

	output.write(c1, runtimePos + 4);
	output.write((char *)(&rSize), 4);
	output.write(c2, rSize);
	output.write((char *)(&runtimePos), 4);

	output.close();
	input1.close();
	input2.close();
	return 0;
}
