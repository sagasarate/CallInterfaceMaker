#pragma once


// CDlgModuleSelector �Ի���

class CDlgModuleSelector : public CDialog
{
	DECLARE_DYNAMIC(CDlgModuleSelector)
protected:
	CListCtrl m_lvList;
public:
	UINT m_SelectedModuleID;
	CDlgModuleSelector(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModuleSelector();

// �Ի�������
	enum { IDD = IDD_DIALOG_MODULE_SELECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void FillList();
	
	afx_msg void OnBnClickedOk();
};
