
// IMEInject.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIMEInjectApp:
// �йش����ʵ�֣������ IMEInject.cpp
//

class CIMEInjectApp : public CWinApp
{
public:
	CIMEInjectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIMEInjectApp theApp;