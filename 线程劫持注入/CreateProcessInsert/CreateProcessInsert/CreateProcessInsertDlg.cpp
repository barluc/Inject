
// CreateProcessInsertDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CreateProcessInsert.h"
#include "CreateProcessInsertDlg.h"
#include "afxdialogex.h"
#include "inject.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCreateProcessInsertDlg �Ի���



CCreateProcessInsertDlg::CCreateProcessInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateProcessInsertDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreateProcessInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCreateProcessInsertDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSERT, &CCreateProcessInsertDlg::OnBnClickedInsert)
END_MESSAGE_MAP()


// CCreateProcessInsertDlg ��Ϣ�������

BOOL CCreateProcessInsertDlg::OnInitDialog()
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

void CCreateProcessInsertDlg::OnPaint()
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
HCURSOR CCreateProcessInsertDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCreateProcessInsertDlg::OnBnClickedInsert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PROCESS_INFORMATION stProcessInfo;                   // �洢������Ϣ��PROCESS_INFORMATION �ṹ��
	::memset(&stProcessInfo, 0 ,sizeof(stProcessInfo));  //����ṹ���ڴ�

	STARTUPINFO stStartUpInfo;                                    //���̵���������ʾ��Ϣ��STARTUPINFO�ṹ�� 
	::memset(&stStartUpInfo, 0 ,sizeof(stStartUpInfo)); //����ṹ���ڴ�
	stStartUpInfo.cb = sizeof(stStartUpInfo);

	LPCWSTR ProcName;                                          //Ŀ����̵�ַ��
	LPVOID    lpAddr;                                               //Ŀ����������ڴ�ռ��ָ��
	LPCWSTR lpBuffer;                                             //dll��ַ
	PTHREAD_START_ROUTINE pnfStartAddr ;       //LoadLibrary��ַ
	
	AdjustProcessTokenPrivilege();

	ProcName = L"D:\\Program Files (x86)\\Notepad++\\notepad++.exe";
	if(	OpenTargetProcess(ProcName , stStartUpInfo , stProcessInfo ) == FALSE )
	{
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}

	HANDLE hTargetProcess= stProcessInfo.hProcess;	    //Ŀ����̵ľ��
	if( TagetAlloc(hTargetProcess , lpAddr) == FALSE )
	{
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}

	lpBuffer= TEXT("C:\\Users\\10178\\Desktop\\InjectDllFile.dll");
	if(WriteDLLToTarget(hTargetProcess, lpAddr , lpBuffer)== FALSE )
	{
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}

	HANDLE	 hTargetThread = stProcessInfo.hThread;       //Ŀ���̵߳ľ��
	//��ȡLoadLibrary��ַ
	pnfStartAddr = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle(TEXT("Kernel32")) , "LoadLibraryW");
	if(pnfStartAddr == NULL)
	{
		OutputDebugString(L"��ȡLoadLibrary��ַʧ��");
		MessageBox(L"ע��ʧ��" , L"��ʾ" , MB_OK);
		return;
	}
	if( CreateThreadInTarget(hTargetProcess  , pnfStartAddr , lpAddr) == FALSE)
	{
		MessageBox(L"ע��ʧ��",L"��ʾ",MB_OK);
		return;
	}

	//�ָ�ע���Ŀ���̹߳���״̬
	 ResumeThread(hTargetThread);

}
