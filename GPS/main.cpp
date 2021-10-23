#include "GPS.h"

int main()
{
	GPS myGPS;
	myGPS.setupSharedMemory();
	while (1) {

		myGPS.getData();

		if (myGPS.setHeartbeat(false) == 0) {
			break;
		};


	}

	return 0;
}