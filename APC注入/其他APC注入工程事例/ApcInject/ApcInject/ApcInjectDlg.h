// ApcInjectDlg.h : ͷ�ļ�
//

#pragma once


// CApcInjectDlg �Ի���
class CApcInjectDlg : public CDialog
{
// ����
public:
	CApcInjectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_APCINJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	CString m_strExePath;
public:
	CString m_strDllPath;
public:
	afx_msg void OnBnClickedButton3();
};
