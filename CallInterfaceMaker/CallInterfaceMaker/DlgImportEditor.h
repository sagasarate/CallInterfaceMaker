#pragma once
#include "afxcmn.h"


// CDlgImportEditor �Ի���

class CDlgImportEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgImportEditor)
protected:
	CListCtrl m_lvList;
public:
	CDlgImportEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgImportEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_IMPORT_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();

	void FillList();
	
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
