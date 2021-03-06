//Compile in a C++ CLR empty project
#using <System.dll>

#include <conio.h>//_kbhit()
#include "Laser.h"

#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <Windows.h>


using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;


#define PI 3.1416

int main()
{ 
	Laser myLaser;

	myLaser.setupSharedMemory();

	SMObject LASERObj(_TEXT("LASERObj"), sizeof(SM_Laser));

	LASERObj.SMCreate();
	LASERObj.SMAccess();

	SM_Laser* laser = (SM_Laser*)LASERObj.pData;

	// LMS151 port number must be 2111
	int PortNumber = 23000;
	// Pointer to TcpClent type object on managed heap
	TcpClient^ Client;
	// arrays of unsigned chars to send and receive data
	array<unsigned char>^ SendData;
	array<unsigned char>^ ReadData;
	char RxData[2500];
	// String command to ask for Channel 1 analogue voltage from the PLC
	// These command are available on Galil RIO47122 command reference manual
	// available online
	String^ AskScan = gcnew String("sRN LMDscandata");
	// String to store received data for display
	String^ ResponseData;

	// Creat TcpClient object and connect to it
	Client = gcnew TcpClient("192.168.1.200", PortNumber);
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;//ms
	Client->SendTimeout = 500;//ms
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;

	// unsigned char arrays of 16 bytes each are created on managed heap
	SendData = gcnew array<unsigned char>(16);
	ReadData = gcnew array<unsigned char>(2500);
	// Convert string command to an array of unsigned char
	SendData = System::Text::Encoding::ASCII->GetBytes(AskScan);


	// Get the network streab object associated with client so we 
	// can use it to read and write
	NetworkStream^ Stream = Client->GetStream();

	//Authentication
	String^ Auth = gcnew String("5255853\n");

	String^ AuthMessage = gcnew String("OK\n");

	array<unsigned char>^ sendAuth = System::Text::Encoding::ASCII->GetBytes(Auth);

	Stream->Write(sendAuth, 0, sendAuth->Length);

	System::Threading::Thread::Sleep(10);

	array<unsigned char>^ ReadAuth = gcnew array<unsigned char>(16);

	Stream->Read(ReadAuth, 0, ReadAuth->Length);

	String^ AuthResponse = System::Text::Encoding::ASCII->GetString(ReadAuth);

	//Console::WriteLine(AuthResponse);

	//Loop
	while (!_kbhit())
	{
		if (myLaser.setHeartbeat(false) == 0) {
			exit(0);
		
		}
		else {
			// Write command asking for data
			Stream->WriteByte(0x02);
			Stream->Write(SendData, 0, SendData->Length);
			Stream->WriteByte(0x03);
			// Wait for the server to prepare the data, 1 ms would be sufficient, but used 10 ms
			System::Threading::Thread::Sleep(10);
			// Read the incoming data
			Stream->Read(ReadData, 0, ReadData->Length);
			// Convert incoming data from an array of unsigned char bytes to an ASCII string
			ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
			// Print the received string on the screen
			//Console::WriteLine(ResponseData);

			//data conversion attempt 1
			/*array<System::String^>^ Fragments = nullptr;
			double StartAngle;
			double Resolution;
			int NumRanges;
			array<double>^ Range;
			array<double>^ RangeX;
			array<double>^ RangeY;

			Fragments = ResponseData->Split(' ');
			StartAngle = System::Convert::ToInt32(Fragments[23], 16);
			Resolution = System::Convert::ToInt32(Fragments[24], 16) / 10000.0;
			NumRanges = System::Convert::ToInt32(Fragments[25], 16);

			Console::Write("{0,10:F3} {1,10:F3} {2}", StartAngle, Resolution, NumRanges);
			Range = gcnew array<double>(NumRanges);
			RangeX = gcnew array<double>(NumRanges);
			RangeY = gcnew array<double>(NumRanges);
			for (int i = 0; i < NumRanges; i++) {
				Range[i] = System::Convert::ToInt32(Fragments[26 + i], 16);
				//Console::Write("{0,10:F3}", Range[i]);
				RangeX[i] = Range[i] * Math::Sin(i * Resolution * Math::PI / 180.0);
				RangeY[i] = -Range[i] * Math::Cos(i * Resolution * Math::PI / 180.0);
			
			}
			Console::WriteLine("\t[{0,10:F3}, {1,10:F3}]", RangeX[360], RangeY[360]);

			Console::WriteLine("");
			*/
			//myLaser.getData();

			//data conversion attempt 2

			for (int i = 0; i < 2500; i++)
				RxData[i] = ReadData[i];

			std::string ScanResp = RxData;
			std::istringstream is(ScanResp);
			std::string Fragments;
			std::string Resol;
			std::string Range[361];
			double conRange[361];
			double numPoints;
			int count = 0;
			double resolution;
			double startAngle;

			for (int i = 0; i < 26; i++) {
				is >> Fragments;
				if (i == 23) {
					is >> std::hex >> startAngle;
				}
				else if (i == 24) {
					is >> std::hex >> resolution;
					const char* res = Resol.c_str();
					resolution = std::strtol(res, NULL, 16);
					resolution = resolution / 10000;
				}
				else if (i == 25) {
					is >> std::hex >> numPoints;
				}
			}

			
			for (int i = 0; i < numPoints; i++) {
				//Grab hex data
				is >> std::hex >> Range[i];
				//Convert to decimal
				const char* conv = Range[i].c_str();
				conRange[i] = std::strtol(conv, NULL, 16);

				laser->x[count] = conRange[i] * (cos(180 - (resolution * count) * (PI / 180)));
				laser->y[count] = conRange[i] * (sin(180 - (resolution * count) * (PI / 180)));
				count++;
				System::Threading::Thread::Sleep(100);
				std::cout << "x: " << laser->x[count] << " y: " << laser->y[count] << std::endl;
			}

			if (count > 361) {
				count = 0;
			}

		}
	}

	Stream->Close();
	Client->Close();

	Console::ReadKey();
	Console::ReadKey();


	return 0;
}