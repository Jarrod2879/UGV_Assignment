#using <System.dll>
#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>

#include "../include/smstructs.h"
#include "../include/SMObject.h"

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

int main()
{
	SMObject PMObj(_TEXT("PMObj"), sizeof(ProcessManagement));
	SMObject UGVObj(_TEXT("UGVObj"), sizeof(SM_VehicleControl));

	PMObj.SMAccess();
	ProcessManagement* PMData = (ProcessManagement*)PMObj.pData;

	UGVObj.SMCreate();
	UGVObj.SMAccess();

	int PortNumber = 25000;
	TcpClient^ Client;
	double flag = 1;

	array<unsigned char>^ SendData = gcnew array<unsigned char>(100);

	Client = gcnew TcpClient("192.168.1.200", PortNumber);
	Client->NoDelay = true;
	Client->ReceiveTimeout = 1500;//ms
	Client->SendTimeout = 500;//ms
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;

	NetworkStream^ Stream = Client->GetStream();
	
	while (1) {
		double speed = 10;
		double steering = 10;

		char a[100];
		sprintf(a, "# %f %f %f #", steering, speed, flag);

		String^ SendString = gcnew String(a);
		SendData = System::Text::Encoding::ASCII->GetBytes(SendString);
		Stream->Write(SendData, 0, SendData->Length);
		System::Threading::Thread::Sleep(500);

		if (PMData->Heartbeat.Flags.VehicleControl == 0) {
			PMData->ShutdownCounter++;
		}
		if (PMData->Heartbeat.Flags.VehicleControl == 1) {

			PMData->Heartbeat.Flags.VehicleControl = 0;
			PMData->ShutdownCounter = 0;
		}
		if (PMData->ShutdownCounter > 100) {
			exit(0);
		}
		if (PMData->Shutdown.Status == 1) {
			exit(0);
		}
		if (_kbhit()) {
			exit(0);
		}
	}
}