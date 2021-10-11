#include "GPS.h"

int main()
{
	GPS myGPS;
	myGPS.setupSharedMemory();
	while (1) {

		myGPS.getData();
	}

	return 0;
}