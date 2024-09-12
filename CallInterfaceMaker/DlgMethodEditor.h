/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgMethodEditor.h                                        */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgMethodEditor 对话框

class CDlgMethodEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgMethodEditor)
protected:
	CListCtrl					m_lvParamList;
	CComboBox					m_cbType;
	CTabCtrl					m_tabParamList;
	int							m_CurTabType;

	INTERFACE_METHOD			m_MethodInfo;
public:
	CDlgMethodEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMethodEditor();

	// 对话框数据
	enum { IDD = IDD_DIALOG_METHOD_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	void AddListItem(METHOD_PARAM& ParamInfo, UINT Index);
	void FillList();
	void FillListItem(int Item, METHOD_PARAM& ParamInfo);
	void SelectItemByName(LPCTSTR szName);
	void InitTab();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnBnClickedButtonAddParam();
	afx_msg void OnBnClickedButtonEditParam();
	afx_msg void OnBnClickedButtonDelParam();
	afx_msg void OnBnClickedButtonParamUp();
	afx_msg void OnBnClickedButtonParamDown();
	afx_msg void OnNMDblclkListParam(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeComboMethodType();
public:
	void SetData(INTERFACE_METHOD& MethodInfo);
	void SetID(UINT ID);
	void GetData(INTERFACE_METHOD& MethodInfo);







	afx_msg void OnTcnSelchangeTabParamList(NMHDR* pNMHDR, LRESULT* pResult);
};

inline void CDlgMethodEditor::SetData(INTERFACE_METHOD& MethodInfo)
{
	m_MethodInfo = MethodInfo;
}
inline void CDlgMethodEditor::SetID(UINT ID)
{
	m_MethodInfo.ID = ID;
}
inline void CDlgMethodEditor::GetData(INTERFACE_METHOD& MethodInfo)
{
	MethodInfo = m_MethodInfo;
}