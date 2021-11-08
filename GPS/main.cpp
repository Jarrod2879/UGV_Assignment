#include "GPS.h"

#using <System.dll>

#include <conio.h>//_kbhit()





using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

struct GPSTempStruct {
	unsigned char discard[44];
	double Northing;
	double Easting;
	double Height;
	unsigned char discard2[40];
	unsigned int Checksum;
};

int main()
{
	GPS myGPS;
	myGPS.setupSharedMemory();

	 SMObject GPSObj(_TEXT("GPSObj"), sizeof(SM_GPS));

	 GPSObj.SMCreate();
	 GPSObj.SMAccess();

	 SM_GPS* GPSSMptr = (SM_GPS*)GPSObj.pData;

	TcpClient^ Client;
	array<unsigned char>^ ReadData;

	int PortNumber = 24000;
	Client = gcnew TcpClient("192.168.1.200", PortNumber);

	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;//ms
	Client->SendTimeout = 500;//ms
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;

	ReadData = gcnew array<unsigned char>(112);

	NetworkStream^ Stream = Client->GetStream();


	while (1) {

		myGPS.getShutdownFlag();
		myGPS.setHeartbeat(false);
		
		if (Stream->DataAvailable) {
			System::Threading::Thread::Sleep(10);
			Stream->Read(ReadData, 0, ReadData->Length);

			GPSTempStruct gpsdata;
			unsigned char* BytePtr;
			BytePtr = (unsigned char*)&gpsdata;

			for (int i = 0; i < 112; i++) {
				Console::Write("{0:X2} ", ReadData[i]);
				*(BytePtr + i) = ReadData[i];
			}


			GPSSMptr->easting = gpsdata.Easting;
			GPSSMptr->northing = gpsdata.Northing;
			GPSSMptr->height = gpsdata.Height;
			

			unsigned int tempChecksum = CalculateBlockCRC32(108, BytePtr);

			printf("Northing: ");
			Console::WriteLine(gpsdata.Northing);
			printf("Easting: ");
			Console::WriteLine(gpsdata.Easting);
			printf("Height: ");
			Console::WriteLine(gpsdata.Height);
			

		}

		if (_kbhit()) {
			exit(0);
		}
		

	}

	
	exit(0);

	return 0;
}