#pragma once
#include "afxwin.h"


// CDlgStructMemberEditor �Ի���

class CDlgStructMemberEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgStructMemberEditor)
protected:
	CComboBox			m_cbType;
public:
	STRUCT_MEMBER_INFO			m_StructMemberInfo;
	CDlgStructMemberEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgStructMemberEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_STRUCT_MEMBER_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	
public:
	
	
	CComboBox m_cbDBIndexType;
};
