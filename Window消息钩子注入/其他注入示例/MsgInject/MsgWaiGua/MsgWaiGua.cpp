#include <windows.h>

#pragma data_seg(SHARD_SEG_NAME)
static HHOOK g_hhook;
#pragma data_seg()



BOOL  _stdcall DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved )
{
  if( ul_reason_for_call == DLL_PROCESS_ATTACH )
	{
	//
	//����������Ŀ����̿ռ�HOOK�Ĵ���
	//
	  MessageBox(NULL, "SetWindowsHookEx inject success", "dll inject", MB_OK);
	}    
    return TRUE;
}

__declspec(dllexport)LRESULT MyMessageProc(int code, WPARAM wParam, LPARAM lParam)
{
  //
  //���Լ�����Ϣ�Ĵ���
  //
  return CallNextHookEx(g_hhook, code, wParam, lParam);
}
