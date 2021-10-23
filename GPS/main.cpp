#include "GPS.h"

int main()
{
	GPS myGPS;
	myGPS.setupSharedMemory();
	while (1) {

		myGPS.getData();

		myGPS.setHeartbeat(false);

	}

	return 0;
}