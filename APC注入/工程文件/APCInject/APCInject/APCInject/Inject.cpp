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

//��Ŀ����̵�ַ�ռ��з�������ص�DLL·���ռ�
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

//�Ѵ����ص�DLL·��д��Ŀ����̿ռ�
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

