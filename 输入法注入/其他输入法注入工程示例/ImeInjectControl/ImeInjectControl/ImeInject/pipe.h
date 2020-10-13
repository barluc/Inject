#ifndef _PIPE_H_
#define _PIPE_H_
#include <windows.h>
#ifndef OutputDebugStr
#define OutputDebugStr OutputDebugString
#endif

void PipeServer(LPCSTR lpPipeName, HANDLE hEvent);
void PipeClient(LPCSTR lpPipeName);

extern HANDLE g_hEvent;
extern HANDLE g_hNotifyEvent;

typedef struct _PIPE_COMMUNICATION
{
	DWORD Cmd;             //==1,load;==2,unload; ==3,wait
	DWORD CallBackData1;
	DWORD CallBackData2;
	DWORD CallBackData3;
	DWORD OnloadDllWhenExit;    // �����뷨�˳�ʱ�Ƿ�ж�ؿͻ�DLL  0-�ǣ�1-��
	DWORD LoadNextWhenActive;    // �������뷨����ʱ���Ƿ��Զ�����һ�����뷨 0-��1-��
	char  DLLString[256];

}PIPE_COMMUNICATION,*PPIPE_COMMUNICATION;

void SetPipeInfo(PPIPE_COMMUNICATION pstPipeInfo);
void GetPipeInfo(PPIPE_COMMUNICATION pstPipeInfo);

#endif