/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgInterfaceEdtor.cpp                                    */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgInterfaceEdtor.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgInterfaceEdtor.h"


// CDlgInterfaceEdtor 对话框

IMPLEMENT_DYNAMIC(CDlgInterfaceEdtor, CDialog)

CDlgInterfaceEdtor::CDlgInterfaceEdtor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInterfaceEdtor::IDD, pParent)
{
}

CDlgInterfaceEdtor::~CDlgInterfaceEdtor()
{
}

void CDlgInterfaceEdtor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_METHOD, m_lvMethodList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_InterfaceInfo.Name);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_InterfaceInfo.Description);	
	DDX_Text(pDX, IDC_EDIT_ID, m_InterfaceInfo.ID);
}


BEGIN_MESSAGE_MAP(CDlgInterfaceEdtor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_METHOD, &CDlgInterfaceEdtor::OnBnClickedButtonAddMethod)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_METHOD, &CDlgInterfaceEdtor::OnBnClickedButtonEditMethod)
	ON_BN_CLICKED(IDC_BUTTON_DEL_METHOD, &CDlgInterfaceEdtor::OnBnClickedButtonDelMethod)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_METHOD, &CDlgInterfaceEdtor::OnNMDblclkListMethod)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CDlgInterfaceEdtor::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, &CDlgInterfaceEdtor::OnBnClickedButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_SORT, &CDlgInterfaceEdtor::OnBnClickedButtonSort)
	ON_BN_CLICKED(IDC_BUTTON_CLONE, &CDlgInterfaceEdtor::OnBnClickedButtonClone)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CDlgInterfaceEdtor::OnBnClickedButtonMove)
END_MESSAGE_MAP()


// CDlgInterfaceEdtor 消息处理程序

BOOL CDlgInterfaceEdtor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_lvMethodList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	m_lvMethodList.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 200);
	m_lvMethodList.InsertColumn(1, _T("类型"), LVCFMT_LEFT, 80);
	m_lvMethodList.InsertColumn(2, _T("调用参数"), LVCFMT_LEFT, 60);
	m_lvMethodList.InsertColumn(3, _T("回调参数"), LVCFMT_LEFT, 60);
	m_lvMethodList.InsertColumn(4, _T("Flag"), LVCFMT_LEFT, 60);
	m_lvMethodList.InsertColumn(5, _T("ID"), LVCFMT_LEFT, 60);
	m_lvMethodList.InsertColumn(6, _T("描述"), LVCFMT_LEFT, 200);


	FillListItem();

	

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgInterfaceEdtor::OnBnClickedButtonAddMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgMethodEditor Dlg;

	Dlg.SetID(m_InterfaceInfo.IDSeed);
	if(Dlg.DoModal()==IDOK)
	{
		m_InterfaceInfo.IDSeed++;
		INTERFACE_METHOD MethodInfo;
		Dlg.GetData(MethodInfo);
		m_InterfaceInfo.MethodList.push_back(MethodInfo);
		AddListItem(MethodInfo,m_InterfaceInfo.MethodList.size()-1);
	}
}

void CDlgInterfaceEdtor::OnBnClickedButtonEditMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMethodList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMethodList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMethodList.GetItemData(Item);
		if(Index<m_InterfaceInfo.MethodList.size())
		{
			CDlgMethodEditor Dlg;

			Dlg.SetData(m_InterfaceInfo.MethodList[Index]);

			if(Dlg.DoModal()==IDOK)
			{
				Dlg.GetData(m_InterfaceInfo.MethodList[Index]);
				EditListItem(Item,m_InterfaceInfo.MethodList[Index]);
			}
		}
	}
	else
	{
		MessageBox(_T("请选择一个方法"));
	}
}

void CDlgInterfaceEdtor::OnBnClickedButtonDelMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMethodList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMethodList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMethodList.GetItemData(Item);
		if(Index<m_InterfaceInfo.MethodList.size())
		{
			CString Msg;
			Msg.Format(_T("是否要删除方法[%s]"),
				m_InterfaceInfo.MethodList[Index].Name);
			if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
			{
				m_InterfaceInfo.MethodList.erase(m_InterfaceInfo.MethodList.begin()+Index);
				FillListItem();
			}
			
		}
	}
}

void CDlgInterfaceEdtor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);	
	CDialog::OnOK();
}

void CDlgInterfaceEdtor::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

void CDlgInterfaceEdtor::AddListItem(INTERFACE_METHOD& MethodInfo,UINT Index)
{
	CString Temp;
	int Item=m_lvMethodList.InsertItem(m_lvMethodList.GetItemCount(),MethodInfo.Name);
	m_lvMethodList.SetItemData(Item,(INT_PTR)Index);

	m_lvMethodList.SetItemText(Item, 1, 
		MethodInfo.Type >= 0 && MethodInfo.Type<INTERFACE_METHOD_TYPE_MAX ? g_szINTERFACE_METHOD_TYPE[MethodInfo.Type]:_T("未知"));
	
	Temp.Format(_T("%d"),MethodInfo.CallParamList.size());
	m_lvMethodList.SetItemText(Item,2,Temp);
	Temp.Format(_T("%d"), MethodInfo.AckParamList.size());
	m_lvMethodList.SetItemText(Item, 3, Temp);
	Temp.Format(_T("%u"), MethodInfo.Flag);
	m_lvMethodList.SetItemText(Item, 4, Temp);
	Temp.Format(_T("%d"),MethodInfo.ID);
	m_lvMethodList.SetItemText(Item,5,Temp);
	m_lvMethodList.SetItemText(Item,6,MethodInfo.Description);

}
void CDlgInterfaceEdtor::EditListItem(int Item,INTERFACE_METHOD& MethodInfo)
{
	CString Temp;
	m_lvMethodList.SetItemText(Item,0,MethodInfo.Name);

	m_lvMethodList.SetItemText(Item, 1,
		MethodInfo.Type >= 0 && MethodInfo.Type < INTERFACE_METHOD_TYPE_MAX ? g_szINTERFACE_METHOD_TYPE[MethodInfo.Type] : _T("未知"));

	Temp.Format(_T("%d"),MethodInfo.CallParamList.size());
	m_lvMethodList.SetItemText(Item,2,Temp);
	Temp.Format(_T("%d"), MethodInfo.AckParamList.size());
	m_lvMethodList.SetItemText(Item, 3, Temp);
	Temp.Format(_T("%u"), MethodInfo.Flag);
	m_lvMethodList.SetItemText(Item, 4, Temp);
	Temp.Format(_T("%d"),MethodInfo.ID);
	m_lvMethodList.SetItemText(Item,5,Temp);
	m_lvMethodList.SetItemText(Item,6,MethodInfo.Description);
}
void CDlgInterfaceEdtor::FillListItem()
{
	m_lvMethodList.DeleteAllItems();
	for(UINT i=0;i<m_InterfaceInfo.MethodList.size();i++)
	{
		AddListItem(m_InterfaceInfo.MethodList[i],i);
	}
}

void CDlgInterfaceEdtor::SelectItemByName(LPCTSTR szName)
{
	for(int i=0;i<m_lvMethodList.GetItemCount();i++)
	{
		if(m_lvMethodList.GetItemText(i,0)==szName)
		{
			m_lvMethodList.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			m_lvMethodList.EnsureVisible(i,false);
		}
		else
		{
			m_lvMethodList.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}

void CDlgInterfaceEdtor::OnNMDblclkListMethod(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedButtonEditMethod();
}


void CDlgInterfaceEdtor::OnBnClickedButtonMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMethodList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMethodList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMethodList.GetItemData(Item);
		if(Index>0&&Index<m_InterfaceInfo.MethodList.size())
		{
			INTERFACE_METHOD Temp=m_InterfaceInfo.MethodList[Index];
			m_InterfaceInfo.MethodList.erase(m_InterfaceInfo.MethodList.begin()+Index);
			m_InterfaceInfo.MethodList.insert(m_InterfaceInfo.MethodList.begin()+Index-1,Temp);
			FillListItem();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgInterfaceEdtor::OnBnClickedButtonMoveDown()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMethodList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMethodList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMethodList.GetItemData(Item);
		if(Index<m_InterfaceInfo.MethodList.size()-1)
		{
			INTERFACE_METHOD Temp=m_InterfaceInfo.MethodList[Index];
			m_InterfaceInfo.MethodList.erase(m_InterfaceInfo.MethodList.begin()+Index);
			m_InterfaceInfo.MethodList.insert(m_InterfaceInfo.MethodList.begin()+Index+1,Temp);
			FillListItem();
			SelectItemByName(Temp.Name);
		}
	}
}


void CDlgInterfaceEdtor::OnBnClickedButtonSort()
{
	// TODO:  在此添加控件通知处理程序代码

	qsort(m_InterfaceInfo.MethodList.begin()._Ptr, m_InterfaceInfo.MethodList.size(), sizeof(INTERFACE_METHOD), CCallInterfaceMakerDlg::InterfaceMethodComp);
	FillListItem();
}


void CDlgInterfaceEdtor::OnBnClickedButtonClone()
{
	// TODO:  在此添加控件通知处理程序代码
	POSITION Pos = m_lvMethodList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvMethodList.GetNextSelectedItem(Pos);
		UINT Index = (UINT)m_lvMethodList.GetItemData(Item);
		if (Index < m_InterfaceInfo.MethodList.size())
		{
			vector<CALLER_INTERFACE *>& InterfaceList = GetMainDlg()->GetInterfaceList();
			CDlgListSelector Dlg;

			Dlg.m_Title = _T("请选择接口");
			Dlg.m_Items.resize(InterfaceList.size());
			for (size_t i = 0; i < InterfaceList.size(); i++)
			{
				if (InterfaceList[i]->ModuleID == m_InterfaceInfo.ModuleID&&
					InterfaceList[i]->Name == m_InterfaceInfo.Name)
					continue;
				MODULE_DEFINE_INFO * pModule = GetMainDlg()->GetModuleInfo(InterfaceList[i]->ModuleID);
				if (pModule)
				{
					Dlg.m_Items[i] = InterfaceList[i]->Name + _T("@") + pModule->Name;
				}
			}
			if (Dlg.DoModal() == IDOK)
			{
				int Pos = Dlg.m_SelectedItem.Find('@');
				if (Pos >= 0)
				{
					CString InterfaceName = Dlg.m_SelectedItem.Left(Pos);
					CString ModuleName = Dlg.m_SelectedItem.Right(Dlg.m_SelectedItem.GetLength() - Pos - 1);
					CALLER_INTERFACE * pInterface = GetMainDlg()->GetInterfaceInfoByName(ModuleName, InterfaceName);
					if (pInterface)
					{
						pInterface->MethodList.push_back(m_InterfaceInfo.MethodList[Index]);
						pInterface->MethodList[pInterface->MethodList.size() - 1].ID = pInterface->IDSeed;
						pInterface->IDSeed++;
						AfxMessageBox(_T("复制成功"));
					}
				}
			}
		}
	}


	
}


void CDlgInterfaceEdtor::OnBnClickedButtonMove()
{
	// TODO:  在此添加控件通知处理程序代码
	POSITION Pos = m_lvMethodList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvMethodList.GetNextSelectedItem(Pos);
		UINT Index = (UINT)m_lvMethodList.GetItemData(Item);
		if (Index < m_InterfaceInfo.MethodList.size() - 1)
		{
			vector<CALLER_INTERFACE *>& InterfaceList = GetMainDlg()->GetInterfaceList();
			CDlgListSelector Dlg;

			Dlg.m_Title = _T("请选择接口");
			Dlg.m_Items.resize(InterfaceList.size());
			for (size_t i = 0; i < InterfaceList.size(); i++)
			{
				if (InterfaceList[i]->ModuleID==m_InterfaceInfo.ModuleID&&
					InterfaceList[i]->Name == m_InterfaceInfo.Name)
					continue;
				MODULE_DEFINE_INFO * pModule = GetMainDlg()->GetModuleInfo(InterfaceList[i]->ModuleID);
				if (pModule)
				{
					Dlg.m_Items[i] = InterfaceList[i]->Name + _T("@") + pModule->Name;
				}
			}
			if (Dlg.DoModal() == IDOK)
			{
				int Pos = Dlg.m_SelectedItem.Find('@');
				if (Pos >= 0)
				{
					CString ModuleName = Dlg.m_SelectedItem.Left(Pos);
					CString InterfaceName = Dlg.m_SelectedItem.Right(Dlg.m_SelectedItem.GetLength() - Pos - 1);
					CALLER_INTERFACE * pInterface = GetMainDlg()->GetInterfaceInfoByName(ModuleName, InterfaceName);
					if (pInterface)
					{
						pInterface->MethodList.push_back(m_InterfaceInfo.MethodList[Index]);
						pInterface->MethodList[pInterface->MethodList.size() - 1].ID = pInterface->IDSeed;
						pInterface->IDSeed++;
						m_InterfaceInfo.MethodList.erase(m_InterfaceInfo.MethodList.begin() + Index);
						AfxMessageBox(_T("移动成功"));
						FillListItem();
					}
				}
			}
		}
	}
}
