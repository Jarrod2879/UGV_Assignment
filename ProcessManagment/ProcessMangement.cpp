
#using <System.dll>
#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>

#include "SMStructs.h"
#include "SMObject.h"
#include "../include/smstructs.h"
#include "../include/SMObject.h"

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

#define NUM_UNITS 5

bool IsProcessRunning(const char* processName);
void StartProcesses();
void checkHeartbeats(ProcessManagement* PMSMptr);

//defining start up sequence
TCHAR Units[10][20] = //
{
	TEXT("GPS.exe"),
	TEXT("Camera.exe"),
	TEXT("Display.exe"),
	TEXT("LASER.exe"),
	TEXT("UGV.exe"),
	
	
};

int main()
{
	//start all 5 modules
	SMObject PMObj(_TEXT("PMObj"), sizeof(ProcessManagement));

	PMObj.SMCreate(); 
	PMObj.SMAccess();

	ProcessManagement* PMSMptr = (ProcessManagement*)PMObj.pData;

	StartProcesses();
	
	while (1) {
		PMSMptr->LifeCounter++;
		checkHeartbeats(PMSMptr);
		if (_kbhit()) break;
	}
	PMSMptr->Shutdown.Status = 1;
	Sleep(200);
	PMSMptr->Shutdown.Status = 1;
	return 0;
}


//Is process running function
bool IsProcessRunning(const char* processName)
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_stricmp((const char *)entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}


void StartProcesses()
{
	STARTUPINFO s[10];
	PROCESS_INFORMATION p[10];

	for (int i = 0; i < NUM_UNITS; i++)
	{
		if (!IsProcessRunning((const char *)Units[i]))
		{
			ZeroMemory(&s[i], sizeof(s[i]));
			s[i].cb = sizeof(s[i]);
			ZeroMemory(&p[i], sizeof(p[i]));

			if (!CreateProcess(NULL, Units[i], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &s[i], &p[i]))
			{
				printf("%s failed (%d).\n", Units[i], GetLastError());
				_getch();
			}
			std::cout << "Started: " << Units[i] << std::endl;
			Sleep(100);
		}
	}
}

int gpscounter = 0;
int lasercounter = 0;
int cameracounter = 0;
int displaycounter = 0;
int ugvcounter = 0;

void checkHeartbeats(ProcessManagement* PMSMptr)
{
	//GPS
	if (PMSMptr->Heartbeat.Flags.GPS == 0) {
		PMSMptr->Heartbeat.Flags.GPS = 1;
		gpscounter = 0;

	}
	else {
		gpscounter++;
		if (gpscounter > 100000) {
			Console::WriteLine("An Error has occured, Attempting to Restart");
			StartProcesses();
		}
	}

	//Laser
	if (PMSMptr->Heartbeat.Flags.Laser == 0) {
		PMSMptr->Heartbeat.Flags.Laser = 1;
		lasercounter = 0;

	}
	else {
		lasercounter++;
		if (lasercounter > 10000) {
			//PMSMptr->Shutdown.Status = 1;
			//exit(0);
			//Console::WriteLine("{0}", lasercounter);
		}
	}

	//Camera
	if (PMSMptr->Heartbeat.Flags.Camera == 0) {
		PMSMptr->Heartbeat.Flags.Camera = 1;
		cameracounter = 0;

	}
	else {
		cameracounter++;
		if (cameracounter > 10000) {
			//PMSMptr->Shutdown.Status = 1;
			//exit(0);
		}
	}

	//Display
	if (PMSMptr->Heartbeat.Flags.OpenGL == 0) {
		PMSMptr->Heartbeat.Flags.OpenGL = 1;
		displaycounter = 0;

	}
	else {
		displaycounter++;
		if (displaycounter > 100000) {
			Console::WriteLine("An Error has occured, Attempting to Restart");
			StartProcesses();
		}
	}

	//UGV
	if (PMSMptr->Heartbeat.Flags.VehicleControl == 0) {
		PMSMptr->Heartbeat.Flags.VehicleControl = 1;
		ugvcounter = 0;

	}
	else {
		ugvcounter++;
		if (ugvcounter > 100000) {
			Console::WriteLine("An Error has occured, Attempting to Restart");
			StartProcesses();
		}
	}

}

