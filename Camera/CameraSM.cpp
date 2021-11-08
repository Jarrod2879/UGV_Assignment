#include "CameraClass.h"
#include <tchar.h>

int	CameraSM::connect(String^ hostName, int portNumber)
{
	// YOUR CODE HERE
	return 1;
}
int CameraSM::setupSharedMemory()
{
	ProcessManagementData = new SMObject(_TEXT("PMObj"), sizeof(ProcessManagement));

	ProcessManagementData->SMAccess();

	// YOUR CODE HERE
	return 1;
}
int CameraSM::getData()
{
	return 1;
}
int CameraSM::checkData()
{
	// YOUR CODE HERE
	return 1;
}
int CameraSM::sendDataToSharedMemory()
{
	// YOUR CODE HERE
	return 1;
}
bool CameraSM::getShutdownFlag()
{
	// YOUR CODE HERE
	return 1;
}


int counter = 0;

int CameraSM::setHeartbeat(bool heartbeat)
{
	SMObject PMObj(TEXT("ProcessManagement"), sizeof(ProcessManagement));
	double TimeStamp;
	__int64 Frequency, Counter;
	int Shutdown = 0x00;

	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);

	PMObj.SMCreate();
	PMObj.SMAccess();

	ProcessManagement* PMptr = (ProcessManagement*)PMObj.pData;

	QueryPerformanceFrequency((LARGE_INTEGER*)&Counter);
	TimeStamp = (double)Counter / (double)Frequency * 1000;

	if (PMptr->Heartbeat.Flags.Camera == 1) {
		counter = 0;
		PMptr->Heartbeat.Flags.Camera = 0;
	}
	else {
		counter++;
		if (counter > 100) {
			//Console::WriteLine("An Error has occured");
			exit(0);
		}
	}
}
CameraSM::~CameraSM()
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