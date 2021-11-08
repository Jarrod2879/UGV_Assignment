#include "Camera.h"
#include <tchar.h>

int	Camera::connect(String^ hostName, int portNumber)
{
	// YOUR CODE HERE
	return 1;
}
int Camera::setupSharedMemory()
{
	ProcessManagementData = new SMObject(_TEXT("PMObj"), sizeof(ProcessManagement));

	ProcessManagementData->SMAccess();

	// YOUR CODE HERE
	return 1;
}
int Camera::getData()
{
	return 1;
}
int Camera::checkData()
{
	// YOUR CODE HERE
	return 1;
}
int Camera::sendDataToSharedMemory()
{
	// YOUR CODE HERE
	return 1;
}
bool Camera::getShutdownFlag()
{
	// YOUR CODE HERE
	return 1;
}


int counter = 0;

int Camera::setHeartbeat(bool heartbeat)
{
	ProcessManagement* PMptr = (ProcessManagement*)ProcessManagementData->pData; // YOUR CODE HERE
	if (PMptr->Heartbeat.Flags.Laser == 1) {
		counter = 0;
		PMptr->Heartbeat.Flags.Laser = 0;
	}
	else {
		counter++;
		if (counter > 100) {
			//Console::WriteLine("An Error has occured");
			return 0;
		}
	}
	return 1;
}
Camera::~Camera()
{
	// YOUR CODE HERE
}



unsigned long CRC32Value(int i)
{
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for (j = 8; j > 0; j--)
	{
		if (ulCRC & 1)
			ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
		else
			ulCRC >>= 1;
	}
	return ulCRC;
}

unsigned long CalculateBlockCRC32(unsigned long ulCount, /* Number of bytes in the data block */
	unsigned char* ucBuffer) /* Data block */
{
	unsigned long ulTemp1;
	unsigned long ulTemp2;
	unsigned long ulCRC = 0;
	while (ulCount-- != 0)
	{
		ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value(((int)ulCRC ^ *ucBuffer++) & 0xff);
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return(ulCRC);
}