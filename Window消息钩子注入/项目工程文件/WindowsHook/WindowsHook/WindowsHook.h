
// WindowsHook.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWindowsHookApp:
// �йش����ʵ�֣������ WindowsHook.cpp
//

class CWindowsHookApp : public CWinApp
{
public:
	CWindowsHookApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWindowsHookApp theApp;