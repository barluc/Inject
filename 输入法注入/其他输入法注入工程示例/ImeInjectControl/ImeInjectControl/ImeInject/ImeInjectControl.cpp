// ImeInjectControl.cpp : Defines the entry point for the console application.

#include "stdio.h"
#include "pipe.h"
//#pragma comment(lib, "ImeDllLoader.lib")

HANDLE g_hEvent = NULL;
HANDLE g_hNotifyEvent = NULL;

BOOL SetupIME(LPCTSTR lpImePath,LPCTSTR lpInjectDll);
DWORD WINAPI PipeServerThread(	LPVOID lpThreadID);
int main(int argc, char* argv[])
{
	CHAR szImeBuf[MAX_PATH] = {0};
	CHAR szInjectDll[MAX_PATH] = {0};
	CHAR szSetup[5]={0};
	CHAR szSelect[5]={0};


	printf("�Ƿ��Ѿ���װ������뷨:\"YES\" or \"NO\" :");
	scanf("%s", szSetup);

	if (stricmp(szSetup, "no") == 0)
	{
		printf("����ime�ļ���·��:");
		scanf("%s", szImeBuf);
	}

	printf("�����ע��dll�ļ���·��:");
	scanf("%s", szInjectDll);
	
	
	g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if ( g_hEvent == NULL)
	{
		printf("main CreateEvent fail! \n");
		return -1;
	}

	g_hNotifyEvent = CreateEvent(NULL, FALSE, FALSE, "WinsunEvent");
	if (g_hNotifyEvent == NULL)
	{
		OutputDebugStr ("main Failed to create named event");
		return -1;
	}
		
	
	//�����ܵ��������߳�
	HANDLE	hNewThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PipeServerThread, NULL, NULL, NULL);
	if ( !hNewThread )
	{
             OutputDebugStr("[!] SetupIME CreateThread error\n");
			 return FALSE;
	}

	if (stricmp(szSetup, "no") == 0)
	{
		SetupIME(szImeBuf,szInjectDll);
	}
	


	while (TRUE)
	{
		printf("\nж�ػ����ע���dll,\"uload\" or \"load\" : ");
		scanf("%s", szSelect);
		if(stricmp(szSelect, "uload") == 0)
		{
			//֪ͨ���뷨������ж��ע���dll
			//��ʼ���ܵ���Ҫ���ݵ���Ϣ
			PIPE_COMMUNICATION stPipeInfo = {0};
			stPipeInfo.Cmd = 2;
			SetPipeInfo(&stPipeInfo);
			SetEvent(g_hEvent);
		}
		else if(stricmp(szSelect, "load") == 0)
		{
			//��ʼ���ܵ���Ҫ���ݵ���Ϣ
			PIPE_COMMUNICATION stPipeInfo = {0};
			stPipeInfo.Cmd = 1;
			strcpy(stPipeInfo.DLLString, szInjectDll);
			SetPipeInfo(&stPipeInfo);
			SetEvent(g_hEvent);
		}
		else
		{
			//��ʼ���ܵ���Ҫ���ݵ���Ϣ
			PIPE_COMMUNICATION stPipeInfo = {0};
			stPipeInfo.Cmd = 3;
			SetPipeInfo(&stPipeInfo);
			SetEvent(g_hEvent);
		}
	}

	return 0;
}

BOOL SetupIME(LPCTSTR lpImePath, LPCTSTR lpInjectDll)
{
	BOOL bSuccess = FALSE;
	LPSTR lpPosition = NULL;
	CHAR strSystemDir[MAX_PATH] = {0};
	CHAR strImeSetupPath[MAX_PATH] = {0};
	GetSystemDirectory(strSystemDir, MAX_PATH);

	if (lpImePath == NULL || lpInjectDll == NULL)
	{
		return bSuccess;
	}

	//lpPosition = strrchr(lpImePath, '\\');
	//strcat(strSystemDir, "\\");
	//strcat(strSystemDir, ++lpPosition);

		

	//ime���뷨�ļ�����ϵͳĿ¼��
//	printf("copy %s, %s\n", lpImePath, strSystemDir);	
	BOOL bCopySuccess = CopyFile(lpImePath,strSystemDir,false);
	if(!bCopySuccess)
	{
		printf("copy fail \n");
		return bSuccess;
	}

	//��ʼ���ܵ���Ҫ���ݵ���Ϣ
// 	PIPE_COMMUNICATION stPipeInfo = {0};
// 	stPipeInfo.Cmd = 1;
// 	strcpy(stPipeInfo.DLLString,lpInjectDll); 
// 	SetPipeInfo(&stPipeInfo);


	
		
	//��װime���뷨�ļ�
	HKL hImeFile = ImmInstallIME(strSystemDir, "������뷨");
	if( ImmIsIME(hImeFile) )
	{
		//��װ�ɹ�
		bSuccess = true;
		printf("���뷨��װ�ɹ�");
	}
	else
	{
		//��װʧ��
		//MessageBox(NULL, "��װʧ��", "��װ���뷨", MB_OKCANCEL);
		
		printf("���뷨��װʧ�� %d",GetLastError());
	}
	return bSuccess;
}

DWORD WINAPI PipeServerThread(
						LPVOID lpThreadID
					)
{

	PipeServer("\\\\.\\pipe\\winsun_ime_load_dll",g_hEvent);
	return 1;
}
