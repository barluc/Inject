#ifndef _REMOTE_THREAD_INJECT_H_
#define _REMOTE_THREAD_INJECT_H_

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>


#ifdef _cplusplus
extern "C"{
#endif


	//
	//����ע�뷽�����յ�ע�����
	//
	typedef struct _INJECT_PARAM{
		
		TCHAR  strDllPath[MAX_PATH]; //injected Dll Path
		bool   bProcessName;         //bProcessNameΪ�٣���strProcess��ű�ע����̵�·���������෴�� 
		TCHAR  strProcessName[MAX_PATH]; 
		bool   bInjectCode;           //bInjectCode = true,��ʾע�����code���ҷ���cExtend�С�
		DWORD  dwSize;               //�ýṹ��Ĵ�С��
		DWORD  dwExtendLength;       //ָ��cExtend����ĳ���
		char   cExtend[1];           //�����չ��Ϣ������code�ȡ�
		
	}INJECT_PARAM, *PINJECT_PARAM;

	//
	//����Զ�߳�ע��
	//
	bool RemoteThreadInject(PINJECT_PARAM);
	
	
	
#ifdef _cplusplus
}
#endif
#endif