#pragma once

#include <WinUser.h>
#include <Windows.h>
#include <string>
#include <Psapi.h>
#include"MyProcess.h"
#include <map>
#include <WinDef.h>


class MyProcessManager
{

	MyProcess* pFocus;
	//BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
	MyProcessManager(const MyProcessManager&);
	//MyProcessManager& operator= (const MyProcessManager&);
	MyProcessManager();
public:
	static MyProcessManager& CreateInstance();
	static MyProcess& getProcessByPid(DWORD pid);
	bool InitClass();
	~MyProcessManager();
	static std::map < DWORD, MyProcess* > MyProcessManager::pMap;
	void UpdateListProcess();
	
	void UpdateFocus(DWORD pid);
	//void PrintAll();
	std::string PrintAll();
	void RemoveElement(DWORD);
	void AddElement(DWORD);

};

