#include "stdafx.h"
#include "MyProcessManager.h"
#include <iostream>
#include <string.h>
std::map  < DWORD, MyProcess* > MyProcessManager::pMap;

void PrintProcessNameAndID(DWORD);
void PrintProcessInfo(DWORD);
void PrintProcessInfo(MyProcess&);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
//MyProcessManager & MyProcessManager::operator=(const MyProcessManager &) { return NULL; }
MyProcessManager::MyProcessManager() { this->pFocus = new MyProcess(); }
MyProcessManager::~MyProcessManager() {}

void MyProcessManager::UpdateListProcess()
{


	DWORD aProcesses[1024], cbNeeded;
	for (auto j = this->pMap.begin(); j != this->pMap.end(); j++)
		delete (j->second);
	this->pMap.clear();
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return;
	}


	cbNeeded /= sizeof(DWORD);
	// Print the name and process identifier for each process.

	for (auto i = pMap.begin(); i != pMap.end(); i++) {
		DWORD j;
		for (j = 0; j < cbNeeded; j++)
			if (aProcesses[j] == (*i).second->getPid()) break;
		if (j == cbNeeded){
			delete (*i).second;
			pMap.erase(i->first);
		}
	}
	for (DWORD i = 0; i < cbNeeded; i++)
	{
		if(this->pMap.find(i)==this->pMap.end())
			PrintProcessNameAndID(aProcesses[i]);
	}
	PrintAll();
	//UpdateFocus();



	return;
}

void MyProcessManager::UpdateFocus(DWORD pid)
{
	if (pMap.find(pid) == pMap.end())return;
	MyProcess& p = MyProcessManager::getProcessByPid(pid);
	this->pFocus->setPid(pid);
	this->pFocus->setClassName(p.getClassName());
	this->pFocus->setMWND(p.getMWND());
	this->pFocus->setPNAME(p.getPName());
	this->pFocus->setTidWND(p.getTid());
	this->pFocus->setIsVisible(p.getIsVisible());
	this->pFocus->setTitleWND(p.getTitleWND());
	std::cout << "================== FOCUS PROCESS INFO ==================" << std::endl;
	PrintProcessInfo(*pFocus);
	std::cout << "================== END FOCUS PROCESS INFO ==================" << std::endl;

}


MyProcessManager& MyProcessManager::CreateInstance()
{
	static MyProcessManager mpm;
	return mpm;
}

MyProcess & MyProcessManager::getProcessByPid(DWORD pid)
{
	return *(MyProcessManager::pMap[pid]);
}

bool MyProcessManager::InitClass()
{


	DWORD aProcesses[1024], cbNeeded;
	PrintProcessNameAndID(GetCurrentProcessId());


	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return false;
	}

	
	cbNeeded /= sizeof(DWORD);
	// Print the name and process identifier for each process.

	for (DWORD i = 0; i < cbNeeded; i++)
	{
		if (aProcesses[i] != 0)
		{			
			PrintProcessNameAndID(aProcesses[i]);
		}
	}

	PrintAll();
	return true;


}




BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{

	DWORD lpdwProcessId, tid;
	char buffer[1024]="";

	DWORD d = GetCurrentProcessId();
	tid = GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	
	if (lpdwProcessId == lParam ) {
		if (IsWindowEnabled(hwnd)) {
			MyProcess & c = MyProcessManager::getProcessByPid((DWORD)lParam);
			c.setTidWND(tid);
			GetClassNameA(hwnd, buffer, sizeof(buffer));
			c.setMWND(hwnd);
			c.setClassName(buffer);
			if (d == lpdwProcessId)
				SetWindowTextA(hwnd, "Remote Controller-Server Side");
			GetWindowTextA(hwnd,buffer,1023);
			c.setTitleWND(buffer);
			
		//	std::cout <<"DEBUG vis " << c.getIsVisible()<< " " << lParam << std::endl;
			
		
		}
		return FALSE;
	}
	return TRUE;
}

void PrintProcessNameAndID(DWORD pid)
{
	char szProcessName[MAX_PATH] = "<unknown>";

	// Get a handle to the process.
	HMODULE hMod;
	DWORD cbNeeded;



	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, pid);


	// Get the process name.

	if (hProcess != NULL)
	{


		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseNameA(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
	}
	std::string s(szProcessName);
	// Print the process name and identifier.
	if (s.compare("<unknown>") != 0) {
	MyProcess* p = new MyProcess(pid);
	MyProcessManager::pMap[pid] = p;
	p->setPNAME(s);
	
	if (!EnumWindows(EnumWindowsProc, p->getPid())) {
		//HINSTANCE hInstance = (HINSTANCE)GetWindowLong(g_HWND, GWL_HINSTANCE);
		p->setIsVisible(IsWindowEnabled(p->getMWND()));
		if (p->getClassName().length()==0)
		{
			delete p;
			MyProcessManager::pMap.erase(pid);
		}

		}
	}
	CloseHandle(hProcess);
}

void PrintProcessInfo(DWORD pid) {

	MyProcess *p = MyProcessManager::pMap[pid];
	if (p == NULL || !p->getIsVisible()) return;
	std::cout << "Process id: " << p->getPid() << std::endl;
	std::cout << "Process name: " << p->getPName() << std::endl;
	std::cout << "Window title: " << p->getTitleWND() << std::endl;
	std::cout << "IsWindowVisible: " << p->getIsVisible() << std::endl;
	std::cout << "Class name: " << p->getClassName() << std::endl << std::endl;

}


void PrintProcessInfo(MyProcess& p){
	
	if (!p.getClassName().length()) return;
	std::cout << "Process id: " << p.getPid() << std::endl;
	std::cout << "Process name: " << p.getPName() << std::endl;
	std::cout << "Window title: " << p.getTitleWND() << std::endl;
	std::cout << "IsWindowVisible: " << p.getIsVisible() << std::endl;
	std::cout << "Class name: " << p.getClassName() << std::endl << std::endl;
	



}


std::string MyProcessManager::PrintAll() {
	std::string s;
	for (auto i = pMap.begin(); i != pMap.end(); i++) {
		PrintProcessInfo(*(*i).second);
		if(i->second->getClassName().length()!=0)
		s += i->second->toString();
	}
	return s;
}

void MyProcessManager::RemoveElement(DWORD c)
{
	delete pMap[c];
	pMap.erase(c);
}

void MyProcessManager::AddElement(DWORD c)
{
	PrintProcessNameAndID(c);
}
