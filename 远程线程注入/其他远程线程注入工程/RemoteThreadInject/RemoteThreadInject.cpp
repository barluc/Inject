#include "RemoteThreadInject.h"
#include "DbgInfo.h"
#include "ProcessMgr.h"



void main(void)
{
	CHAR szBuf[MAX_PATH] = {0};
	CHAR szDll[MAX_PATH] = {0};
	INJECT_PARAM stInjectParam = {0};
	printf("please input injected process name: ");
	scanf("%s", szBuf);
	printf("please input injected Dll name: ");
	scanf("%s", szDll);

	
	
	//����ע�����
	stInjectParam.bInjectCode = FALSE;
	stInjectParam.bProcessName = TRUE;
	strncpy(stInjectParam.strProcessName, szBuf, MAX_PATH);

	ZeroMemory(szBuf, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szBuf);
	strcat(szBuf, "\\");
	strcat(szBuf, szDll);
	OutputDebugString(szBuf);
	strncpy(stInjectParam.strDllPath, szBuf, MAX_PATH);

	RemoteThreadInject(&stInjectParam);

}

__declspec(naked) void loadDll(void) 
{ 
	_asm{ 
		// Placeholder for the return address 
		push 0xDEADBEEF 
			
			// Save the flags and registers 
			pushfd 
			pushad 
			
			// Placeholder for the string address and LoadLibrary 
			push 0xDEADBEEF 
			mov eax, 0xDEADBEEF 
			
			// Call LoadLibrary with the string parameter 
			call eax 
			
			// Restore the registers and flags 
			popad 
			popfd 
			
			// Return control to the hijacked thread 
			ret 
			NOP
			NOP
	} 
} 

__declspec(naked) void loadDll_end(void) 
{ 
} 

//
//����CreateRemoteThread+WriteProcessMemroyע��DLL��code
//
bool  RemoteThreadInject(PINJECT_PARAM pInjectParam)
{
   bool bSuccess;
   PVOID stub;
   ULONG uCodeLenth = 0;
   CONTEXT ctx; 
   PDWORD  pfLoadDllAddr = NULL, pDllStr =NULL, pRetAddr = NULL;
   PUCHAR  pInjectCode = NULL;
   unsigned long uPID, threadID, stubLen, oldIP, oldprot, loadLibAddy; 
   HANDLE hProcess = NULL, hThread = NULL;
   HMODULE hMod = NULL;
   DWORD dwTid = 0, dwWrite;;
   LPSTR  lpRemoteString = NULL, dllString = NULL;
   LPCTSTR lpDllString = NULL, lpProcessName = NULL;
   PTHREAD_START_ROUTINE pfnThreadRtn = NULL;

   lpDllString    = pInjectParam->strDllPath;
   lpProcessName  = pInjectParam->strProcessName;
   bSuccess       = false;
   
   
   PRT(("[!] RemoteThreadInject Enter...\n")); 

   //
   //����������Ȩ
   //
     if(!AdjustProcessPrivileges(GetCurrentProcess(), "SeDebugPrivilege"))
	 {
	    PRT(("[-] AdjustProcessPrivileges ret false!\n"));
		goto Exit;
	 }
   //
   //��ȡLoadLibraryA��ַ
   //
      pfnThreadRtn = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	  loadLibAddy   = (ULONG)pfnThreadRtn;
	  if (pfnThreadRtn == NULL)
	  {
		  PRT(("[-] GetProcAddress Failed:%d\n",GetLastError()));
		  goto Exit;
	  }
    
     uPID = GetTargetProcessIdFromProcname(pInjectParam->strProcessName); 
     if(uPID == -1)
     { 
		 PRT(("[-] GetTargetProcessIdFromProcname Failed!\n"));
		 goto Exit;
     }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, uPID); //CREATE_THREAD_ACCESS
    if(!hProcess)
	{
	   	 PRT(("[-] OpenProcess Failed!\n"));
		 goto Exit;
	}

   if(!pInjectParam->bInjectCode)
   {
     //
	 //inject by CreateRemoteThread & LoadLibraryA
     //
       lpRemoteString = (LPSTR)VirtualAllocEx(hProcess, NULL, strlen(lpDllString)+1, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE); 

       WriteProcessMemory(hProcess, (LPVOID)lpRemoteString, (LPVOID)lpDllString,strlen(lpDllString), NULL); 

       hThread = CreateRemoteThread(hProcess, NULL, NULL, pfnThreadRtn, lpRemoteString, NULL, &dwTid); 

       if(hThread)
         WaitForSingleObject( hThread, INFINITE );

       VirtualFreeEx( hProcess, lpRemoteString, strlen(lpDllString), MEM_DECOMMIT );   
       if(hThread)
         CloseHandle( hThread ); 
       if(hProcess)  
         CloseHandle(hProcess); 
   }
   //ע�����
//     else
//    {
//      //
//      //inject code method 
//      //
//  
// 	   //
// 	   //��Ŀ������У������ע�����Ŀռ�
// 	   //
//        stub = VirtualAllocEx(hProcess, NULL, pInjectParam->dwExtendLength, MEM_COMMIT, PAGE_EXECUTE_READWRITE); 
// 	   if(!stub)
// 	   {
//            PRT(("[-] VirtualAllocEx Failed!\n"));
// 		   goto Exit;
// 	   }
// 
// 
// 	   ////
// 	   ////��dll������д��Ŀ�������
// 	   //// 
// 	   //
// 	   //��ȡĿ����̵��߳�ID���򿪲����Ҹ��߳�
// 	   //
// 	   threadID = GetTargetThreadIdFromProcname(lpProcessName); 
//        hThread = OpenThread((THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME), false, threadID); 
//        SuspendThread(hThread);
//        
// 	   //
// 	   //��ȡ���̵߳������Ļ���
// 	   //
// 	   ctx.ContextFlags = CONTEXT_CONTROL; 
//        GetThreadContext(hThread, &ctx); 
// 
// 	   //
// 	   //��ȡ�����Ļ����д�ִ�е�����ָ���ַ
// 	   //
//        oldIP = ctx.Eip; 
// 
// 	   //
// 	   //�滻�����Ļ���������ָ���ַ��ʹ��ָ�����ǵ�ע�����stub
// 	   //
//        ctx.Eip = (DWORD)stub; 
//        ctx.ContextFlags = CONTEXT_CONTROL; 
// 
// 
// 	   pRetAddr       =  (DWORD *)(pInjectParam->cExtend + 1);
// 	   *pRetAddr      =  oldIP;
// 
// 	   pDllStr        =  (DWORD *)(pInjectParam->cExtend + 9);
// 	   *pDllStr       =   ((DWORD)stub + 24);
// 
// 	   
// 	   pfLoadDllAddr  =  (DWORD *)(pInjectParam->cExtend + 15);
// 	   *pfLoadDllAddr =   loadLibAddy;
//        
// 	   ////
// 	   ////�ı��ע�뺯���ı������ԣ�����̬��д��������
// 	   ////
// 	   //
// 	   //����ע�����д��Ŀ�������
// 	   //
//           bSuccess = WriteProcessMemory(hProcess, stub, pInjectParam->cExtend, pInjectParam->dwExtendLength, &dwWrite); 
// 		  if(!bSuccess || (dwWrite != pInjectParam->dwExtendLength))
// 		  {
// 		     PRT(("[-] VirtualAllocEx Failed!\n"));
// 		     goto Exit;
// 		  }
// 
// 
// 	   //
// 	   //���������߳�������
// 	   //
//        SetThreadContext(hThread, &ctx); 
//        
// 	   //
// 	   //�ָ����̵߳�ִ��
// 	   //
//        ResumeThread(hThread); 
// 
//        Sleep(5000); 
// 
//        VirtualFreeEx(hProcess, stub, pInjectParam->dwExtendLength, MEM_DECOMMIT); 
// 	   CloseHandle(hThread);
//        CloseHandle(hProcess); 
//    
//    }

  PRT(("[!] RemoteThreadInject Exit...\n")); 
  bSuccess = true;
Exit:
   return bSuccess;
}
