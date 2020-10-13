
// CreateRemoteThreadInsertDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateRemoteThreadInsert.h"
#include "CreateRemoteThreadInsertDlg.h"
#include "afxdialogex.h"
#include "ThreadInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCreateRemoteThreadInsertDlg �Ի���



CCreateRemoteThreadInsertDlg::CCreateRemoteThreadInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateRemoteThreadInsertDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreateRemoteThreadInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCreateRemoteThreadInsertDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSERT, &CCreateRemoteThreadInsertDlg::OnBnClickedInsert)
END_MESSAGE_MAP()


// CCreateRemoteThreadInsertDlg ��Ϣ�������

BOOL CCreateRemoteThreadInsertDlg::OnInitDialog()
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

void CCreateRemoteThreadInsertDlg::OnPaint()
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
HCURSOR CCreateRemoteThreadInsertDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCreateRemoteThreadInsertDlg::OnBnClickedInsert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//HWND hwnd;//Ŀ����̴��ھ��
	DWORD hpid;//Ŀ�����pid
	HANDLE htargetprocess;//Ŀ����̾��
	LPVOID lpAddr;//Ŀ���߳������ڴ�ռ��ָ��
	//	LPVOID lpBuffer;//dll��ַ

	AdjustProcessTokenPrivilege();
	////����Ŀ����̴���
	//hwnd=::FindWindow(TEXT("Edit"),NULL);
	//if(hwnd==NULL)
	//{
	//	OutputDebugString(L"���Ҵ���ʧ��");
	//	MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
	//	return;
	//}
	//GetWindowThreadProcessId(hwnd,&hpid);

	hpid=GetProcessIDByName(L"Calculator.exe");

	if(hpid==0)
	{
		OutputDebugString(L"��ȡ���ڽ���PIDʧ��");
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}
	htargetprocess=OpenTargetProcess(hpid);
	if(htargetprocess==NULL)
	{
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}
	if(TagetAlloc(htargetprocess, lpAddr) == FALSE)
	{
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}

	if(WriteDLLToTarget(htargetprocess, lpAddr , TEXT("C:\\Users\\10178\\Desktop\\InjectDllFile.dll") )== FALSE )
	{
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}

	PTHREAD_START_ROUTINE pnfStartAddr ;
	pnfStartAddr=(PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	if(pnfStartAddr==NULL)
	{
		OutputDebugString(L"��ȡLoadLibrary��ַʧ��");
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}
	if( CreateThreadInTarget(htargetprocess,pnfStartAddr,lpAddr) == FALSE)
	{
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}

}
