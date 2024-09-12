/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgMethodEditor.h                                        */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgMethodEditor �Ի���

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
	CDlgMethodEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMethodEditor();

	// �Ի�������
	enum { IDD = IDD_DIALOG_METHOD_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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