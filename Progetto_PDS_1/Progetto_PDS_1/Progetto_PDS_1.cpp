// Progetto_PDS_1.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <thread>
#include "Progetto_PDS_1.h"
#include <oleacc.h>

MyProcessManager& p = MyProcessManager::CreateInstance();
DWORD prevthread = 0;
void CALLBACK WinEventProcFC(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
	DWORD c;
	GetWindowThreadProcessId(hwnd, &c);
	std::cout << c << std::endl;
	p.UpdateFocus(c);
}
/*
void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {

	char str[256];
	//if ( idObject!=0) return;

	GetClassNameA(hwnd, str,256);
	if (strlen(str) == 0) return;
	DWORD d;
	GetWindowThreadProcessId(hwnd, &d);
	if(d==dwEventThread)
	std::cout << "create: class name " << str << " " << d << std::endl;

}

*/
void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
	{
		IAccessible* pAcc = NULL;
		VARIANT varChild;
		IDispatch *d = NULL;
		
		if (idObject != 0) return;
		DWORD c;
		
	HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);
		if ((hr == S_OK) && (pAcc != NULL) && IsWindowEnabled(hwnd))
		{
		VARIANT varResult = {};
			pAcc->get_accRole(varChild, &varResult);
			BSTR bstrName;
				pAcc->get_accName(varChild, &bstrName);
				std::wstring s (L"");
				if (bstrName != NULL)
					s = bstrName;
			
				GetWindowThreadProcessId(hwnd, &c);
				if (event == EVENT_OBJECT_CREATE && s.length() != 0 && c != prevthread){
					HANDLE t = OpenThread(THREAD_QUERY_LIMITED_INFORMATION, false, dwEventThread);
					DWORD pd;
					GetWindowThreadProcessId(GetForegroundWindow(), &pd);
					
					std::cout << "Cchild pid " << pd<< std::endl;
					//printf("Create: ");
					prevthread = c;
					CloseHandle(t);
				}
				if (event == EVENT_OBJECT_DESTROY)
				{
					DWORD pd;
					GetWindowThreadProcessId(hwnd, &pd);
					std::cout << "Dchild pid " << pd << std::endl;
					
				}
		
		
				
		
				//SysFreeString(bstrName);
				//pAcc->Release();
			}
		
		}


/*
void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {

	DWORD c;
	HANDLE t = OpenThread(THREAD_QUERY_LIMITED_INFORMATION, false, dwEventThread);
	c = GetProcessIdOfThread(t);
	GetWindowThreadProcessId(hwnd, &c);
	IAccessible* pAcc = NULL;
	VARIANT varChild;
	//	IDispatch *d = NULL;

	//GetWindowThreadProcessId(hwnd, &c);
//	if ( dwEventThread != c) return;


	if (idObject != OBJID_WINDOW || !IsWindow(hwnd) || idChild != 0) return;
	HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);
	if ((hr == S_OK) && (pAcc != NULL))
	{

		VARIANT varResult = {};
		pAcc->get_accRole(varChild, &varResult);
		
	//	if (varResult.lVal != ROLE_SYSTEM_WINDOW) return;
		char kj[123];
		GetRoleTextA(varResult.lVal,kj,122);
			switch (event)
			{
			case EVENT_OBJECT_DESTROY: //3116
				std::cout << "obj destroyed: " << c << " " << idChild << " " << kj << " " << idObject << " " << dwEventThread << " " << event << std::endl;
				break;
			case EVENT_OBJECT_CREATE:

				std::cout << "obj created: " << c << " " << idChild << " " << kj<< " " << idObject << " " << dwEventThread << " " << event << std::endl;
				break;
			
			default:
				break;
			}



		}
	}
	*/
int main()
{


	
	p.InitClass();
	std::string str;

	//p.UpdateFocus();
	//Sleep(5000);
	//p.UpdateListProcess();
	//p.UpdateFocus();
	//std::cout << "fine" << std::endl;
	//Sleep(5000);
	//HOOKPROC* proc = proc1;

 
 std::thread t ([&]() { 
	MSG msg;

HWINEVENTHOOK hWinEventHook0 = SetWinEventHook(
		 EVENT_OBJECT_DESTROY, EVENT_OBJECT_DESTROY,
			 NULL, HandleWinEvent, 0, 0,
		 WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

		HWINEVENTHOOK hWinEventHook1 = SetWinEventHook(
		 EVENT_OBJECT_CREATE, EVENT_OBJECT_CREATE,
		 NULL, HandleWinEvent, 0, 0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
		
	 HWINEVENTHOOK h = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, 0, WinEventProcFC, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


			}
		);
	//SetWindowsHook(WH_CBT,);
// UnhookWinEvent(h);
	std::string st = p.PrintAll();
	std::cout << st<<std::endl<<st.length();
	My_Socket s;
	s.NewConnection_Server(1552);
	s.Sendind_Server((void*)st.c_str(),st.length());
	//UnhookWinEvent(h);
	Sleep(20000);

    return 0;
}


