/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgMethodEditor.cpp                                      */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgMethodEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgMethodEditor.h"


// CDlgMethodEditor 对话框

IMPLEMENT_DYNAMIC(CDlgMethodEditor, CDialog)

CDlgMethodEditor::CDlgMethodEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMethodEditor::IDD, pParent)
{
	m_CurTabType = -1;
}

CDlgMethodEditor::~CDlgMethodEditor()
{
}

void CDlgMethodEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PARAM, m_lvParamList);
	DDX_Control(pDX, IDC_COMBO_METHOD_TYPE, m_cbType);
	DDX_Control(pDX, IDC_TAB_PARAM_LIST, m_tabParamList);

	DDX_Text(pDX, IDC_EDIT_NAME, m_MethodInfo.Name);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_MethodInfo.Description);
	DDX_Text(pDX, IDC_EDIT_METHOD_ID, m_MethodInfo.ID);
	BOOL CanCache = (m_MethodInfo.Flag & INTERFACE_METHOD_FLAG_CAN_CACHE) ? TRUE : FALSE;
	BOOL NoCompress = (m_MethodInfo.Flag & INTERFACE_METHOD_FLAG_NO_COMPRESS) ? TRUE : FALSE;
	DDX_Check(pDX, IDC_CHECK_CAN_CACHE, CanCache);
	DDX_Check(pDX, IDC_CHECK_NO_COMPRESS, NoCompress);
	m_MethodInfo.Flag = (CanCache ? INTERFACE_METHOD_FLAG_CAN_CACHE : 0) |
		(NoCompress ? INTERFACE_METHOD_FLAG_NO_COMPRESS : 0);

	if (pDX->m_bSaveAndValidate)
	{
		m_MethodInfo.Type = m_cbType.GetCurSel();
	}
	else
	{
		m_cbType.SetCurSel(m_MethodInfo.Type);
	}

}


BEGIN_MESSAGE_MAP(CDlgMethodEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PARAM, &CDlgMethodEditor::OnBnClickedButtonAddParam)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PARAM, &CDlgMethodEditor::OnBnClickedButtonEditParam)
	ON_BN_CLICKED(IDC_BUTTON_DEL_PARAM, &CDlgMethodEditor::OnBnClickedButtonDelParam)
	ON_BN_CLICKED(IDC_BUTTON_PARAM_UP, &CDlgMethodEditor::OnBnClickedButtonParamUp)
	ON_BN_CLICKED(IDC_BUTTON_PARAM_DOWN, &CDlgMethodEditor::OnBnClickedButtonParamDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PARAM, &CDlgMethodEditor::OnNMDblclkListParam)
	ON_CBN_SELCHANGE(IDC_COMBO_METHOD_TYPE, &CDlgMethodEditor::OnCbnSelchangeComboMethodType)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PARAM_LIST, &CDlgMethodEditor::OnTcnSelchangeTabParamList)
END_MESSAGE_MAP()


// CDlgMethodEditor 消息处理程序

BOOL CDlgMethodEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvParamList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	m_lvParamList.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 150);
	m_lvParamList.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 50);
	m_lvParamList.InsertColumn(2, _T("类型"), LVCFMT_LEFT, 60);
	m_lvParamList.InsertColumn(3, _T("长度"), LVCFMT_LEFT, 50);
	m_lvParamList.InsertColumn(4, _T("缺省值"), LVCFMT_LEFT, 50);
	m_lvParamList.InsertColumn(5, _T("可为空"), LVCFMT_LEFT, 60);
	m_lvParamList.InsertColumn(6, _T("描述"), LVCFMT_LEFT, 200);

	m_cbType.ResetContent();
	for (int i = 0; i < INTERFACE_METHOD_TYPE_MAX; i++)
	{
		m_cbType.AddString(g_szINTERFACE_METHOD_TYPE[i]);
	}

	UpdateData(false);
	m_CurTabType = -1;
	InitTab();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgMethodEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	CDialog::OnOK();
}

void CDlgMethodEditor::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

void CDlgMethodEditor::OnBnClickedButtonAddParam()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgParamEditor Dlg;


	Dlg.SetID(m_MethodInfo.ParamIDSeed);

	if (Dlg.DoModal() == IDOK)
	{
		m_MethodInfo.ParamIDSeed++;
		METHOD_PARAM ParamInfo;
		Dlg.GetData(ParamInfo);

		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			m_MethodInfo.CallParamList.push_back(ParamInfo);
			AddListItem(ParamInfo, m_MethodInfo.CallParamList.size() - 1);
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			m_MethodInfo.AckParamList.push_back(ParamInfo);
			AddListItem(ParamInfo, m_MethodInfo.AckParamList.size() - 1);
			break;
		}
		ListCtrlColAutoFit(m_lvParamList);
	}
}

void CDlgMethodEditor::OnBnClickedButtonEditParam()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos = m_lvParamList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index = (UINT)m_lvParamList.GetItemData(Item);
		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			if (Index < m_MethodInfo.CallParamList.size())
			{
				CDlgParamEditor Dlg;

				Dlg.SetData(m_MethodInfo.CallParamList[Index]);

				if (Dlg.DoModal() == IDOK)
				{
					Dlg.GetData(m_MethodInfo.CallParamList[Index]);
					FillListItem(Item, m_MethodInfo.CallParamList[Index]);
				}
			}
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			if (Index < m_MethodInfo.AckParamList.size())
			{
				CDlgParamEditor Dlg;

				Dlg.SetData(m_MethodInfo.AckParamList[Index]);

				if (Dlg.DoModal() == IDOK)
				{
					Dlg.GetData(m_MethodInfo.AckParamList[Index]);
					FillListItem(Item, m_MethodInfo.AckParamList[Index]);
				}
			}
			break;
		}
		ListCtrlColAutoFit(m_lvParamList);
	}
	else
	{
		MessageBox(_T("请选择一个参数"));
	}
}

void CDlgMethodEditor::OnBnClickedButtonDelParam()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos = m_lvParamList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index = (UINT)m_lvParamList.GetItemData(Item);
		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			if (Index < m_MethodInfo.CallParamList.size())
			{
				CString Msg;
				Msg.Format(_T("是否要删除参数[%s]"),
					m_MethodInfo.CallParamList[Index].Name);
				if (AfxMessageBox(Msg, MB_YESNO) == IDYES)
				{
					m_MethodInfo.CallParamList.erase(m_MethodInfo.CallParamList.begin() + Index);
					FillList();
				}
			}
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			if (Index < m_MethodInfo.AckParamList.size())
			{
				CString Msg;
				Msg.Format(_T("是否要删除参数[%s]"),
					m_MethodInfo.AckParamList[Index].Name);
				if (AfxMessageBox(Msg, MB_YESNO) == IDYES)
				{
					m_MethodInfo.AckParamList.erase(m_MethodInfo.AckParamList.begin() + Index);
					FillList();
				}
			}
			break;
		}

	}
}
void CDlgMethodEditor::OnBnClickedButtonParamUp()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos = m_lvParamList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index = (UINT)m_lvParamList.GetItemData(Item);
		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			if (Index > 0 && Index < m_MethodInfo.CallParamList.size())
			{
				METHOD_PARAM Temp = m_MethodInfo.CallParamList[Index];
				m_MethodInfo.CallParamList.erase(m_MethodInfo.CallParamList.begin() + Index);
				m_MethodInfo.CallParamList.insert(m_MethodInfo.CallParamList.begin() + Index - 1, Temp);
				FillList();
				SelectItemByName(Temp.Name);
			}
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			if (Index > 0 && Index < m_MethodInfo.AckParamList.size())
			{
				METHOD_PARAM Temp = m_MethodInfo.AckParamList[Index];
				m_MethodInfo.AckParamList.erase(m_MethodInfo.AckParamList.begin() + Index);
				m_MethodInfo.AckParamList.insert(m_MethodInfo.AckParamList.begin() + Index - 1, Temp);
				FillList();
				SelectItemByName(Temp.Name);
			}
			break;
		}

	}
}

void CDlgMethodEditor::OnBnClickedButtonParamDown()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos = m_lvParamList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index = (UINT)m_lvParamList.GetItemData(Item);
		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			if (Index < m_MethodInfo.CallParamList.size() - 1)
			{
				METHOD_PARAM Temp = m_MethodInfo.CallParamList[Index];
				m_MethodInfo.CallParamList.erase(m_MethodInfo.CallParamList.begin() + Index);
				m_MethodInfo.CallParamList.insert(m_MethodInfo.CallParamList.begin() + Index + 1, Temp);
				FillList();
				SelectItemByName(Temp.Name);
			}
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			if (Index < m_MethodInfo.AckParamList.size() - 1)
			{
				METHOD_PARAM Temp = m_MethodInfo.AckParamList[Index];
				m_MethodInfo.AckParamList.erase(m_MethodInfo.AckParamList.begin() + Index);
				m_MethodInfo.AckParamList.insert(m_MethodInfo.AckParamList.begin() + Index + 1, Temp);
				FillList();
				SelectItemByName(Temp.Name);
			}
			break;
		}
	}
}

void CDlgMethodEditor::AddListItem(METHOD_PARAM& ParamInfo, UINT Index)
{
	int Item = m_lvParamList.InsertItem(Index, _T(""));
	FillListItem(Item, ParamInfo);
	m_lvParamList.SetItemData(Item, (INT_PTR)Index);
}

void CDlgMethodEditor::FillList()
{
	m_lvParamList.DeleteAllItems();
	switch (m_tabParamList.GetCurSel())
	{
	case INTERFACE_METHOD_PARAM_TYPE_CALL:
		for (UINT i = 0; i < m_MethodInfo.CallParamList.size(); i++)
		{
			AddListItem(m_MethodInfo.CallParamList[i], i);
		}
		break;
	case INTERFACE_METHOD_PARAM_TYPE_ACK:
		for (UINT i = 0; i < m_MethodInfo.AckParamList.size(); i++)
		{
			AddListItem(m_MethodInfo.AckParamList[i], i);
		}
		break;
	}
	ListCtrlColAutoFit(m_lvParamList);
}

void CDlgMethodEditor::FillListItem(int Item, METHOD_PARAM& ParamInfo)
{
	CString Temp;

	m_lvParamList.SetItemText(Item,0, ParamInfo.Name);
	Temp.Format(_T("%u"), ParamInfo.ID);
	m_lvParamList.SetItemText(Item, 1, Temp);

	TYPE_DEFINE* pTypeInfo = GetMainDlg()->FindVarType(ParamInfo.Type);
	if (pTypeInfo)
	{
		CString Temp = pTypeInfo->Name;
		if (ParamInfo.IsArray)
		{
			Temp = GetMainDlg()->GetConfig().ArrayDefineConfig.ReferenceDefine;
			Temp.Replace(_T("<Type>"), pTypeInfo->Name);
		}			
		else if (ParamInfo.IsReference)
		{
			Temp = pTypeInfo->GenerateOperations.ReferenceDefine;
			Temp.Replace(_T("<Type>"), pTypeInfo->Name);
		}
		

		m_lvParamList.SetItemText(Item, 2, Temp);
	}
	else
	{
		m_lvParamList.SetItemText(Item, 2, _T("未知"));
	}
	Temp.Format(_T("%d"), ParamInfo.Length);
	m_lvParamList.SetItemText(Item, 3, Temp);
	m_lvParamList.SetItemText(Item, 4, ParamInfo.DefaultValue);
	if (ParamInfo.CanNull)
	{
		m_lvParamList.SetItemText(Item, 5, _T("是"));
	}
	else
	{
		m_lvParamList.SetItemText(Item, 5, _T("否"));
	}
	m_lvParamList.SetItemText(Item, 6, ParamInfo.Description);
}

void CDlgMethodEditor::SelectItemByName(LPCTSTR szName)
{
	for (int i = 0;i < m_lvParamList.GetItemCount();i++)
	{
		if (m_lvParamList.GetItemText(i, 0) == szName)
		{
			m_lvParamList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_lvParamList.EnsureVisible(i, false);
		}
		else
		{
			m_lvParamList.SetItemState(i, 0, LVIS_SELECTED);
		}
	}
}

void CDlgMethodEditor::InitTab()
{
	if (m_CurTabType != m_cbType.GetCurSel())
	{
		m_CurTabType = m_cbType.GetCurSel();
		m_tabParamList.DeleteAllItems();
		switch (m_CurTabType)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			m_tabParamList.InsertItem(0, _T("调用参数"));
			m_tabParamList.InsertItem(1, _T("回调参数"));
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			m_tabParamList.InsertItem(0, _T("通知参数"));
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			m_tabParamList.InsertItem(0, _T("调用参数"));
		}

		m_tabParamList.SetCurSel(0);

		FillList();
	}
}


void CDlgMethodEditor::OnNMDblclkListParam(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedButtonEditParam();
}


void CDlgMethodEditor::OnCbnSelchangeComboMethodType()
{
	// TODO:  在此添加控件通知处理程序代码
	InitTab();
}


void CDlgMethodEditor::OnTcnSelchangeTabParamList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	FillList();
}
