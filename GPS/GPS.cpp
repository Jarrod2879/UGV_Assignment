#include "GPS.h"
#include <tchar.h>
#include "../include/SMObject.h"
#include "../include/smstructs.h"
#include "../include/UGV_module.h"


int	GPS::connect(String^ hostName, int portNumber) 
{
	// YOUR CODE HERE
	return 1;
}
int GPS::setupSharedMemory() 
{
	ProcessManagementData = new SMObject(_TEXT("PMObj"), sizeof(ProcessManagement));

	ProcessManagementData->SMAccess();

	// YOUR CODE HERE
	return 1;
}
int GPS::getData() 
{
	ProcessManagement* PMptr = (ProcessManagement*)ProcessManagementData->pData;
	//Console::WriteLine("{0}", PMptr->LifeCounter);
	
	// YOUR CODE HERE
	return 1;
}
int GPS::checkData() 
{
	// YOUR CODE HERE
	return 1;
}
int GPS::sendDataToSharedMemory() 
{
	// YOUR CODE HERE
	return 1;
}
bool GPS::getShutdownFlag() 
{
	ProcessManagement* PMptr = (ProcessManagement*)ProcessManagementData->pData;

	if (PMptr->Shutdown.Status == 1 || PMptr->ShutdownCounter > 1000) {
		exit(0);
	}
	return 1;
}



int GPS::setHeartbeat(bool heartbeat)
{
	ProcessManagement* PMptr = (ProcessManagement*)ProcessManagementData->pData; // YOUR CODE HERE
	if (PMptr->Heartbeat.Flags.GPS == 0) {

		PMptr->ShutdownCounter++;
	}

	if (PMptr->Heartbeat.Flags.GPS == 1) {

		PMptr->Heartbeat.Flags.GPS = 0;
		PMptr->ShutdownCounter = 0;
	}

	if (PMptr->ShutdownCounter > 100)
		exit(0);
	if (PMptr->Shutdown.Status == 1)
		exit(0);
	return 1;
}
GPS::~GPS()
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
