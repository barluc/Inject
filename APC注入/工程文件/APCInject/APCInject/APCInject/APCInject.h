
// APCInject.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAPCInjectApp:
// �йش����ʵ�֣������ APCInject.cpp
//

class CAPCInjectApp : public CWinApp
{
public:
	CAPCInjectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAPCInjectApp theApp;