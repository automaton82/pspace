#include <iostream>
using namespace std;

#include "SubspaceLVZ.h"

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cout << "Usage: " << argv[0] << " <input-file>" << endl;
		return 0;
	}

	SubspaceLVZ lvz;
	lvz.load(argv[1], "", true);

	return 0;
}