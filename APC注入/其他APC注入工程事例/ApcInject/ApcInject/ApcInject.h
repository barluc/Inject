// ApcInject.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CApcInjectApp:
// �йش����ʵ�֣������ ApcInject.cpp
//

class CApcInjectApp : public CWinApp
{
public:
	CApcInjectApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CApcInjectApp theApp;