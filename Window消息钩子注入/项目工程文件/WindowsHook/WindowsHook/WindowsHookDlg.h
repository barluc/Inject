
// WindowsHookDlg.h : ͷ�ļ�
//

#pragma once


// CWindowsHookDlg �Ի���
class CWindowsHookDlg : public CDialogEx
{
// ����
public:
	CWindowsHookDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WINDOWSHOOK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInject();
	BOOL InjectDllBySetWindowsHook(void);
	DWORD GetProcessIDByName(const wchar_t* pName);
	DWORD GetThreadIdByProID(ULONG32 ulTargetProcessID);

public:
	DWORD m_dwId;
	HHOOK m_hHook;
	HMODULE m_hmDll;

	afx_msg void OnBnClickedExit();
};
