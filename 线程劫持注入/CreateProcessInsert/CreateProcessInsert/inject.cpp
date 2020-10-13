#include "stdafx.h"
#include <Windows.h>
#include "Inject.h"

//��Suspend��Ŀ�����
BOOL OpenTargetProcess(LPCWSTR ProcName , STARTUPINFO  &st , PROCESS_INFORMATION &pi )
{
	if( !CreateProcess(ProcName , NULL ,NULL , NULL , FALSE , CREATE_SUSPENDED , NULL , NULL , &st , &pi))
	{
		OutputDebugString(L"��Ŀ�����ʧ��");
		return FALSE;
	}
	return  TRUE;
}

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
