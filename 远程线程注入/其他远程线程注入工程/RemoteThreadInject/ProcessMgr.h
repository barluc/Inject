#ifndef _PROCESS_MGR_
#define _PROCESS_MGR_

#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>
#include <shlwapi.h>
#include <Tlhelp32.h>
	//
	//ͨ�����������ؽ���ID
	//
	DWORD GetTargetProcessIdFromProcname(LPCTSTR lpProcessName);
	
	DWORD GetTargetThreadIdFromProcname(LPCTSTR  lpProcessName);
	
	//
	//����������Ȩ
	//
	bool AdjustProcessPrivileges(HANDLE hProcess, LPCTSTR PrivilegeName);
	
#ifdef __cplusplus
}
#endif

#endif 
