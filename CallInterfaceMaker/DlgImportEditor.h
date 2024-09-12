#pragma once
#include "afxcmn.h"


// CDlgImportEditor 对话框

class CDlgImportEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgImportEditor)
protected:
	CListCtrl m_lvList;
public:
	CDlgImportEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgImportEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_IMPORT_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();

	void FillList();
	
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
