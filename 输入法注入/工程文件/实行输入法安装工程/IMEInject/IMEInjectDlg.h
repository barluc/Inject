
// IMEInjectDlg.h : ͷ�ļ�
//

#pragma once


// CIMEInjectDlg �Ի���
class CIMEInjectDlg : public CDialogEx
{
// ����
public:
	CIMEInjectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IMEINJECT_DIALOG };

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
	afx_msg void OnBnClickedImeinsert();
	PVOID m_retV;
	HKL m_hImeFile32;
	afx_msg void OnBnClickedDelete();
};
