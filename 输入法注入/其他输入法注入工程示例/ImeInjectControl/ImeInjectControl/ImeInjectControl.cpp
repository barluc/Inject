// ImeInjectControl.cpp : Defines the entry point for the console application.

#include "stdio.h"
#include "stdafx.h"
#pragma comment(lib, "ImeDllLoader.lib")

BOOL SetupIME(LPCTSTR lpImePath,LPCTSTR lpInjectDll);

int main(int argc, char* argv[])
{
	CHAR szImeBuf[MAX_PATH] = {0};
	CHAR szInjectDll[MAX_PATH] = {0};
	printf("����ime�ļ���·��:");
	scanf("%s", szImeBuf);
	printf("�����ע��dll�ļ���·��:");
	scanf("%s", szInjectDll);
	SetupIME(szImeBuf,szInjectDll);
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

	lpPosition = strrchr(lpImePath, '\\');
	strcat(strSystemDir, "\\");
	strcat(strSystemDir, ++lpPosition);

		

	//ime���뷨�ļ�����ϵͳĿ¼��
	printf("copy %s, %s\n", lpImePath, strSystemDir);	
	BOOL bCopySuccess = CopyFile(lpImePath,strSystemDir,false);
	if(!bCopySuccess)
	{
		printf("copy fail \n");
		return bSuccess;
	}

	
		
	//��װime���뷨�ļ�
	HKL hImeFile = ImmInstallIME(strImeSetupPath, "winsun���뷨");
	if( ImmIsIME(hImeFile) )
	{
		//��װ�ɹ�
		bSuccess = true;
		printf("��װ�ɹ�");
	}
	else
	{
		//��װʧ��
		//MessageBox(NULL, "��װʧ��", "��װ���뷨", MB_OKCANCEL);
		printf("��װʧ��");
	}
	return bSuccess;
}
