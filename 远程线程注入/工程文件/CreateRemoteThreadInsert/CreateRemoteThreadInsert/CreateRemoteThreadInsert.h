
// CreateRemoteThreadInsert.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCreateRemoteThreadInsertApp:
// �йش����ʵ�֣������ CreateRemoteThreadInsert.cpp
//

class CCreateRemoteThreadInsertApp : public CWinApp
{
public:
	CCreateRemoteThreadInsertApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCreateRemoteThreadInsertApp theApp;