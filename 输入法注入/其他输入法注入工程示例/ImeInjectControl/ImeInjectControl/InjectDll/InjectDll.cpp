// InjectDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
   
	switch(ul_reason_for_call)
    {
      case DLL_PROCESS_ATTACH:
		  MessageBox(NULL ,  "�ɹ�ע��DLL", "���뷨ע��Dll", MB_OKCANCEL);
		 
		  break;
	  case DLL_THREAD_ATTACH:
		 break;

	  case DLL_THREAD_DETACH:
		 break;

      case DLL_PROCESS_DETACH:
		  MessageBox(NULL,"�ɹ�ж��DLL" ,"���뷨ע��Dll", MB_OKCANCEL);

        break;


      default:
        break;
    }
	return TRUE;
}

