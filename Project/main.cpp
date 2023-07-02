#include <iostream>
#include <fstream>
#include <cstdlib>

#include "INIFile.h"

using namespace std;

int main(int argc, char* argv[])
{
	INIFile IF = INIFile("Test.ini");

	std::string Buffer = "";
	IF.ToString(Buffer);

	cout << Buffer.c_str() << endl;

	DWORD TEST = 0;
	IF.GetValueByName("TEST_ADDRESS", TEST);

	IF.Save("Test2.ini");
	cin.get();

	//Test Change

	return 0;
}