
// WindowsHookDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WindowsHook.h"
#include "WindowsHookDlg.h"
#include "afxdialogex.h"
#include "tlhelp32.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWindowsHookDlg �Ի���



CWindowsHookDlg::CWindowsHookDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowsHookDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindowsHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWindowsHookDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INJECT, &CWindowsHookDlg::OnBnClickedInject)
	ON_BN_CLICKED(IDC_EXIT, &CWindowsHookDlg::OnBnClickedExit)
END_MESSAGE_MAP()


// CWindowsHookDlg ��Ϣ�������

BOOL CWindowsHookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWindowsHookDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWindowsHookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




DWORD CWindowsHookDlg::GetProcessIDByName(const wchar_t* pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS , 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe) ; ret ; ret = Process32Next(hSnapshot , &pe))
	{
		if (wcscmp(pe.szExeFile , pName) == 0)
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	CloseHandle(hSnapshot);
	return 0;
}

DWORD CWindowsHookDlg::GetThreadIdByProID(ULONG32 ulTargetProcessID)
{	
	HANDLE Handle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (Handle != INVALID_HANDLE_VALUE)	
	{		THREADENTRY32 te;		te.dwSize = sizeof(te);
	if (Thread32First(Handle, &te))
	{			
		do			
		{				
			if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID))
			{					
				if (te.th32OwnerProcessID == ulTargetProcessID)					
				{						
					HANDLE hThread = OpenThread(READ_CONTROL, FALSE, te.th32ThreadID);						
					if (!hThread)						
					{							
						OutputDebugString(_T("���߳̾����ȡʧ��"));
					}						
					else						
					{							
						return te.th32ThreadID;						
					}					
				}				
			}			
		} while (Thread32Next(Handle, &te));		
	}	
	}	CloseHandle(Handle);	
	return 0;
}

BOOL CWindowsHookDlg::InjectDllBySetWindowsHook()
{
	HMODULE m_hmDll = LoadLibrary(_T("WindowsHookInjectDLL.dll"));
	if (m_hmDll == NULL)
	{
		OutputDebugString(_T("DLL����ʧ��"));
		return FALSE;
	}

	HOOKPROC sub_address = NULL;
	sub_address = (HOOKPROC)GetProcAddress(m_hmDll,"MyMessageProcess");
	if(sub_address == NULL)
	{
		OutputDebugString(_T("��ȡ������ַʧ��"));
		return FALSE;
	}

	DWORD hPid;
	hPid=GetProcessIDByName(_T("notepad++.exe"));
	if(hPid == NULL)
	{
		OutputDebugString(_T("��ȡĿ�����PIDʧ��"));
		return FALSE;

	}

	DWORD dwThreadID;
	dwThreadID = GetThreadIdByProID(hPid);
	if(dwThreadID == NULL)
	{
		OutputDebugString(_T("��ȡĿ��������߳̾��ʧ��"));
		return FALSE;
	}

	m_hHook = SetWindowsHookEx(WH_KEYBOARD , sub_address , m_hmDll , dwThreadID);
	if(m_hHook == NULL)
	{
		OutputDebugString(_T("��������ʧ��"));
		return FALSE;
	}

	return TRUE;
}


void CWindowsHookDlg::OnBnClickedInject()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bInject;
	bInject = InjectDllBySetWindowsHook();
	if(bInject)
	{
		MessageBox(_T("ע��ɹ�"));
	}
}


void CWindowsHookDlg::OnBnClickedExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(m_hHook)
		UnhookWindowsHookEx(m_hHook);

	if(m_hmDll)
		FreeLibrary(m_hmDll);

	CDialogEx::OnCancel();
}
