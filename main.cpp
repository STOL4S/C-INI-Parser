#include <iostream>
#include <fstream>
#include <cstdlib>

#include "INIFile.h"

using namespace std;

int main(int argc, char* argv[])
{
	INIFile IF = INIFile("Test.ini");

	cout << IF.ToString() << endl;

	DWORD TEST = 0;
	IF.GetValueByName("TEST_ADDRESS", TEST);

	IF.Save("Test2.ini");
	cin.get();

	return 0;
}