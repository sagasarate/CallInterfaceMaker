#pragma once
#include "afxwin.h"


// CDlgStructMemberEditor 对话框

class CDlgStructMemberEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgStructMemberEditor)
protected:
	CComboBox			m_cbType;
public:
	STRUCT_MEMBER_INFO			m_StructMemberInfo;
	CDlgStructMemberEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStructMemberEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_STRUCT_MEMBER_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	
public:
	
	
	CComboBox m_cbDBIndexType;
};
