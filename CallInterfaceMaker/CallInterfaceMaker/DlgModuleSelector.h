#pragma once


// CDlgModuleSelector 对话框

class CDlgModuleSelector : public CDialog
{
	DECLARE_DYNAMIC(CDlgModuleSelector)
protected:
	CListCtrl m_lvList;
public:
	UINT m_SelectedModuleID;
	CDlgModuleSelector(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModuleSelector();

// 对话框数据
	enum { IDD = IDD_DIALOG_MODULE_SELECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void FillList();
	
	afx_msg void OnBnClickedOk();
};
