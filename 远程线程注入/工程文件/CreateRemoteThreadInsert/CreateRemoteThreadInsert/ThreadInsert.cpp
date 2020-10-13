#include <windows.h>
#include "stdafx.h"
#include "ThreadInsert.h"

#include "tlhelp32.h"

//��������Ȩ��
BOOL AdjustProcessTokenPrivilege()
{
	CString str;
	HANDLE hToken;
	if (FALSE == OpenProcessToken( GetCurrentProcess() , TOKEN_ALL_ACCESS , &hToken))
	{
		OutputDebugString(L"������ʧ��");
		return FALSE;
	}

	//��ѯ������Ȩ��Ϣ
	LUID luid;
	if(FALSE==LookupPrivilegeValue(NULL , SE_DEBUG_NAME , &luid))
	{
		OutputDebugString(L"��ѯ������Ȩ��Ϣʧ��");
		return FALSE;
	}

	//�������Ʒ�����Ȩ
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(FALSE == AdjustTokenPrivileges(hToken , FALSE , &tkp , sizeof(tkp) , NULL , NULL))
	{
		OutputDebugString(L"���ڷ���������Ȩ����ʧ��");
		return FALSE;
	}
	return TRUE;
}

//���ݽ������Ʋ���pid
DWORD GetProcessIDByName(const wchar_t* pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS , 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe) ; ret ; ret = Process32Next(hSnapshot , &pe))
	{
		if (wcscmp(pe.szExeFile , pName) == 0)
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}


//�򿪴�ע��Ľ���
HANDLE OpenTargetProcess(DWORD dpid)
{
	HANDLE hTargetProcess;
	hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS , FALSE , dpid);
	if(hTargetProcess == NULL)
	{
		OutputDebugString(L"��ȡĿ����̾��ʧ��");
	}
	return hTargetProcess;
}

//�ڴ�ע��Ľ���������ռ�
BOOL TagetAlloc(HANDLE	 hTargetProcess,	LPVOID &lpAddr)
{
	lpAddr = VirtualAllocEx(hTargetProcess , NULL , 0x1000 , MEM_COMMIT , PAGE_EXECUTE_READ);
	if(lpAddr == NULL)
	{
		OutputDebugString(L"��Զ���߳����ڴ�ռ�����ʧ��");
		return FALSE;
	}
	return TRUE;
}
//��DLL·��д������Ŀռ�
BOOL WriteDLLToTarget(HANDLE hTargetProcess ,	LPVOID lpAddr , LPCWSTR lpBuffer)
{
	if(WriteProcessMemory(hTargetProcess ,
		lpAddr ,
		(PVOID)lpBuffer ,
		2 * (1 + lstrlen(lpBuffer)) ,
		NULL) == FALSE)
	{
		OutputDebugString(L"��Զ���߳���д��ʧ��");
		return FALSE;
	}
	return TRUE;
}

//��Ŀ������п����߳�
BOOL CreateThreadInTarget(HANDLE hTargetProcess , PTHREAD_START_ROUTINE pnfStartAddr , LPVOID lpAddr)
{
	HANDLE hThreadHandle;
	hThreadHandle = CreateRemoteThread(hTargetProcess , NULL , 0 , pnfStartAddr , lpAddr , 0 , NULL);
	if(hThreadHandle == NULL)
	{
		OutputDebugString(L"��Ŀ������д����߳�ʧ��");
		return FALSE;
	}
	return TRUE;
}
