// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <tchar.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	WCHAR buf[MAX_PATH];
	GetModuleFileName(GetModuleHandle(0) , buf , sizeof buf);
	MessageBox(NULL , buf , _T("���뷨��װ�ɹ�") , MB_OK);
	LoadLibrary(L"InjectDll.dll");

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

