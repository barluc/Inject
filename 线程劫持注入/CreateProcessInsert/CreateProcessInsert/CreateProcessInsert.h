
// CreateProcessInsert.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCreateProcessInsertApp:
// �йش����ʵ�֣������ CreateProcessInsert.cpp
//

class CCreateProcessInsertApp : public CWinApp
{
public:
	CCreateProcessInsertApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCreateProcessInsertApp theApp;