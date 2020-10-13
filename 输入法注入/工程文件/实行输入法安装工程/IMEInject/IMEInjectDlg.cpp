
// IMEInjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IMEInject.h"
#include "IMEInjectDlg.h"
#include "afxdialogex.h"
#include <imm.h>
#pragma comment(lib, "IMM32.LIB")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIMEInjectDlg �Ի���



CIMEInjectDlg::CIMEInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIMEInjectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIMEInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIMEInjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_IMEINSERT, &CIMEInjectDlg::OnBnClickedImeinsert)
	ON_BN_CLICKED(IDC_DELETE, &CIMEInjectDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CIMEInjectDlg ��Ϣ�������

BOOL CIMEInjectDlg::OnInitDialog()
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

void CIMEInjectDlg::OnPaint()
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
HCURSOR CIMEInjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIMEInjectDlg::OnBnClickedImeinsert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::SystemParametersInfo(
		SPI_GETDEFAULTINPUTLANG,
		0,
		&m_retV,
		0);
	m_hImeFile32 = ImmInstallIME(_T("IMEDLL.ime"),_T("���뷨"));

	if( ImmIsIME(m_hImeFile32))
	{
		SystemParametersInfo(
			SPI_GETDEFAULTINPUTLANG,
			0,
			&m_hImeFile32,
			SPIF_SENDWININICHANGE
			);
		::MessageBox(NULL, _T("���뷨�������") ,  _T("��ʾ") , MB_OK);
	}
	else
	{
		::MessageBox(NULL, _T("���뷨����ʧ��,���1.�Ƿ��ǹ���ԱȨ������,2.ime��dll�ļ��Ƿ񿽱���C:\Windows\SysWOW64Ŀ¼��,3.����exeĿ¼���Ƿ��������ļ�") ,  _T("��ʾ") ,  MB_OK);
	}
}


void CIMEInjectDlg::OnBnClickedDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::SystemParametersInfo(
		SPI_SETDEFAULTINPUTLANG,
		0,
		&m_retV,
		SPIF_SENDWININICHANGE
		);

	if (m_hImeFile32 != NULL)
	{

		if(UnloadKeyboardLayout(m_hImeFile32))
		{
			::MessageBox (NULL , _T("���뷨ж�سɹ�") ,  _T("��ʾ") , MB_OK);
		}
		else
		{
			::MessageBox (NULL , _T("���뷨ж��ʧ��,���ֶ�ж��") ,  _T("��ʾ") , MB_OK);
		}
	}
	else
	{
		::MessageBox (NULL , _T("�Ȱ�װ���뷨") ,  _T("��ʾ") , MB_OK);

	}
}
