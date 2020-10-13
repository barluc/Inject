// ApcInjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ApcInject.h"
#include "ApcInjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CApcInjectDlg �Ի���




CApcInjectDlg::CApcInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CApcInjectDlg::IDD, pParent)
	, m_strExePath(_T(""))
	, m_strDllPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CApcInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strExePath);
	DDX_Text(pDX, IDC_EDIT2, m_strDllPath);
}

BEGIN_MESSAGE_MAP(CApcInjectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CApcInjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CApcInjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CApcInjectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CApcInjectDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CApcInjectDlg ��Ϣ�������

BOOL CApcInjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CApcInjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CApcInjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CApcInjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//APC inject 
//
typedef LONG NTSTATUS, *PNTSTATUS;
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

typedef enum _SECTION_INHERIT 
{
	ViewShare = 1,
	ViewUnmap = 2
} SECTION_INHERIT;



DWORD APCInject(PCHAR sProcName,PCHAR sDllName){
	DWORD dRet=0;
	OutputDebugString("[+] APCInject  enter !");

	//����ָ��NtMapViewOfSection�ĺ���ָ������
	typedef DWORD (WINAPI *tNtMapViewOfSection)(HANDLE,HANDLE,LPVOID,ULONG,SIZE_T,LARGE_INTEGER*,SIZE_T*,SECTION_INHERIT,ULONG,ULONG);
	tNtMapViewOfSection NtMapViewOfSection=(tNtMapViewOfSection)GetProcAddress(GetModuleHandle("ntdll.dll"),"NtMapViewOfSection");
	if(!NtMapViewOfSection)
	{
		OutputDebugString("[-] APCInject GetProcAddress NtMapViewOfSection error!");
		return -1;
	}
		

	//����buffer
	HANDLE hFile=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,strlen(sDllName)+1,NULL);
	if(!hFile)
	{
		OutputDebugString("[-] APCInject CreateFileMapping error!");
		return -2;
	}
		
	PCHAR hView=(PCHAR)MapViewOfFile(hFile,FILE_MAP_ALL_ACCESS,0,0,0);
	if(!hView)
	{
		OutputDebugString("[-] APCInject MapViewOfFile error!");
		CloseHandle(hFile);
		return -3;
	}
	else
	{
		//��buffer���ô�ע���dll·��
		strcpy(hView,sDllName);
	}
		

	// ����Ŀ�����
	PROCESS_INFORMATION pi;STARTUPINFO st;
	ZeroMemory(&pi,sizeof(pi));
	ZeroMemory(&st,sizeof(st));
	st.cb=sizeof(STARTUPINFO);
	//��suspend��ʽ��������
	if(CreateProcess(sProcName,NULL,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&st,&pi))
	{
		LPVOID RemoteString=NULL;
		ULONG ViewSize=0;
		void * lpDllName   = NULL;
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//Ŀ����̵�ַ�ռ�����ע��dll·���ռ�
		//lpDllName = VirtualAllocEx(pi.hProcess, NULL, (strlen(sDllName) + 1), MEM_COMMIT, PAGE_READWRITE); 
		//if(lpDllName)
		//{
		//	//�Ѵ�ע��dll·��д��Ŀ����̿ռ�
		//	if(WriteProcessMemory(pi.hProcess, lpDllName, sDllName,strlen(sDllName), NULL))
		//	{
		//		LPVOID nLoadLibrary=(LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"),"LoadLibraryA");
		//		//����QueueUserAPC��Զ�̲߳���һ��APC�����APC����LoadLibrary
		//		if(!QueueUserAPC((PAPCFUNC)nLoadLibrary,pi.hThread,(ULONG_PTR)lpDllName))
		//		{
		//			OutputDebugString("[-] APCInject QueueUserAPC call  error!");
		//			dRet=-6;
		//		}
		//	}
		//	else
		//	{
		//		OutputDebugString("[-] APCInject WriteProcessMemory call  error!");
		//		dRet=-5;
		//	}
		//}
		//else
		//{
		//	OutputDebugString("[-] APCInject VirtualAllocEx call  error!");
		//	dRet=-4;
		//}
		///////////////////////////////////////////////////////////////////////////////////////////////////

		//����ע��dll·��ӳ�䵽Ŀ����̵�ַ�ռ�
		if(NtMapViewOfSection(hFile,pi.hProcess,&RemoteString,0,0,NULL,&ViewSize,ViewShare,0,PAGE_READONLY)==0)
		{
			LPVOID nLoadLibrary=(LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"),"LoadLibraryA");
			//����QueueUserAPC��Զ�̲߳���һ��APC�����APC����LoadLibrary
			if(!QueueUserAPC((PAPCFUNC)nLoadLibrary,pi.hThread,(ULONG_PTR)0))
			{
				OutputDebugString("[-] APCInject QueueUserAPC call  error!");
				dRet=-6;
			}
				
		}
		else
		{
			OutputDebugString("[-] APCInject NtMapViewOfSection call  error!");
			dRet=-5;
		}
		//�ָ����߳�
		ResumeThread(pi.hThread);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		OutputDebugString("[-] APCInject CreateProcess error!");
		dRet=-4;
	}
		
	UnmapViewOfFile(hView);
	CloseHandle(hFile);
	OutputDebugString("[+] APCInject  exit !");
	return dRet;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CApcInjectDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CApcInjectDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#define DLG_FILTER "ALL Files (*.*)|*.*||" 
#define DLG_EXT   "* " 

	CFileDialog dlg(TRUE, _T(DLG_EXT), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, _T(DLG_FILTER)); 

	if (dlg.DoModal() == IDOK) 
	{ 
		POSITION pos = dlg.GetStartPosition(); 

		while (pos != 0) 
		{ 
			m_strExePath = dlg.GetNextPathName(pos); 
		} 
		UpdateData(FALSE);
	} 
}

void CApcInjectDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#define DLG_FILTER "ALL Files (*.*)|*.*||" 
#define DLG_EXT   "* " 

	CFileDialog dlg(TRUE, _T(DLG_EXT), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, _T(DLG_FILTER)); 

	if (dlg.DoModal() == IDOK) 
	{ 
		POSITION pos = dlg.GetStartPosition(); 

		while (pos != 0) 
		{ 
			m_strDllPath = dlg.GetNextPathName(pos); 
		} 
		UpdateData(FALSE);
	} 
}

void CApcInjectDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	APCInject( m_strExePath.GetBuffer(m_strExePath.GetLength()),m_strDllPath.GetBuffer(m_strDllPath.GetLength()));
}
