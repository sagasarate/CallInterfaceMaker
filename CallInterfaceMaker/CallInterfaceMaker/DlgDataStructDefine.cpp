// DlgDataStructDefine.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgDataStructDefine.h"


// CDlgDataStructDefine 对话框

IMPLEMENT_DYNAMIC(CDlgDataStructDefine, CDialog)

CDlgDataStructDefine::CDlgDataStructDefine(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDataStructDefine::IDD, pParent)
{
	
}

CDlgDataStructDefine::~CDlgDataStructDefine()
{
}

void CDlgDataStructDefine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_lvList);
}


BEGIN_MESSAGE_MAP(CDlgDataStructDefine, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEW_CONST, &CDlgDataStructDefine::OnBnClickedButtonNewConst)
	ON_BN_CLICKED(IDC_BUTTON_NEW_ENUM, &CDlgDataStructDefine::OnBnClickedButtonNewEnum)
	ON_BN_CLICKED(IDC_BUTTON_NEW_STRUCT, &CDlgDataStructDefine::OnBnClickedButtonNewStruct)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgDataStructDefine::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgDataStructDefine::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CDlgDataStructDefine::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_OVE_DOWN, &CDlgDataStructDefine::OnBnClickedButtonOveDown)
	ON_BN_CLICKED(ID_DATA_OBJECT_MODIFY_FLAGS_MANAGER, &CDlgDataStructDefine::OnBnClickedDataObjectModifyFlagsManager)
	ON_BN_CLICKED(IDOK, &CDlgDataStructDefine::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgDataStructDefine::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_BASE_TYPE_DEF, &CDlgDataStructDefine::OnBnClickedButtonBaseTypeDef)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CDlgDataStructDefine::OnNMDblclkList)
	ON_BN_CLICKED(IDC_BUTTON_CLONE, &CDlgDataStructDefine::OnBnClickedButtonClone)
	ON_BN_CLICKED(IDC_BUTTON_MOVE2, &CDlgDataStructDefine::OnBnClickedButtonMove2)
END_MESSAGE_MAP()


// CDlgDataStructDefine 消息处理程序

BOOL CDlgDataStructDefine::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvList.InsertColumn(0,_T("名称"),LVCFMT_LEFT,200);
	m_lvList.InsertColumn(1,_T("类型"),LVCFMT_LEFT,80);
	m_lvList.InsertColumn(2, _T("模块"), LVCFMT_LEFT, 100);

	FillList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDataStructDefine::FillList()
{
	m_lvList.DeleteAllItems();
	for(size_t i=0;i<m_DataStructDefineList.size();i++)
	{
		int Item=m_lvList.InsertItem(i,m_DataStructDefineList[i]->ListName);
		m_lvList.SetItemText(Item,1,g_szDATA_STRUCT_TYPE[m_DataStructDefineList[i]->ListType]);
		m_lvList.SetItemText(Item, 2, GetMainDlg()->GetModuleName(m_DataStructDefineList[i]->ModuleID));
		m_lvList.SetItemData(Item,i);
	}
}
void CDlgDataStructDefine::SelectItemByName(LPCTSTR szName,LPCTSTR szSubName)
{
	for(int i=0;i<m_lvList.GetItemCount();i++)
	{
		if(m_lvList.GetItemText(i,0)==szName&&m_lvList.GetItemText(i,1)==szSubName)
		{
			m_lvList.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			m_lvList.EnsureVisible(i,false);
		}
		else
		{
			m_lvList.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}
void CDlgDataStructDefine::OnBnClickedButtonNewConst()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgModuleSelector DlgModuleSelector;
	if (DlgModuleSelector.DoModal()==IDOK)
	{
		MODULE_DEFINE_INFO * pModule = GetMainDlg()->GetModuleInfo(DlgModuleSelector.m_SelectedModuleID);
		if (pModule)
		{
			CDlgConstList Dlg;

			if (Dlg.DoModal() == IDOK)
			{
				CONST_DEFINE_LIST * pConstList = (CONST_DEFINE_LIST *)GetMainDlg()->NewDataStructList(pModule->ModuleID, DATA_STRUCT_CONST, Dlg.m_ListName);
				if (pConstList)
				{
					pConstList->IgnoreOnExport = Dlg.m_IgnoreOnExport;
					pConstList->ConstList = Dlg.m_ConstDefineList;
					m_DataStructDefineList.push_back(pConstList);
					FillList();
				}
			}
		}
	}
}

void CDlgDataStructDefine::OnBnClickedButtonNewEnum()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgModuleSelector DlgModuleSelector;
	if (DlgModuleSelector.DoModal() == IDOK)
	{
		MODULE_DEFINE_INFO * pModule = GetMainDlg()->GetModuleInfo(DlgModuleSelector.m_SelectedModuleID);
		if (pModule)
		{
			CDlgEnumList Dlg;

			if (Dlg.DoModal() == IDOK)
			{
				ENUM_DEFINE_LIST * pEnumList = (ENUM_DEFINE_LIST *)GetMainDlg()->NewDataStructList(pModule->ModuleID, DATA_STRUCT_ENUM, Dlg.m_ListName);
				if (pEnumList)
				{
					pEnumList->IgnoreOnExport = Dlg.m_IgnoreOnExport;
					pEnumList->EnumList = Dlg.m_EnumDefineList;
					m_DataStructDefineList.push_back(pEnumList);
					FillList();
				}
			}
		}
	}
}

void CDlgDataStructDefine::OnBnClickedButtonNewStruct()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgModuleSelector DlgModuleSelector;
	if (DlgModuleSelector.DoModal() == IDOK)
	{
		MODULE_DEFINE_INFO * pModule = GetMainDlg()->GetModuleInfo(DlgModuleSelector.m_SelectedModuleID);
		if (pModule)
		{
			CDlgStructList Dlg;

			if (Dlg.DoModal() == IDOK)
			{
				STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)GetMainDlg()->NewDataStructList(pModule->ModuleID, DATA_STRUCT_STRUCT, Dlg.m_ListName);
				if (pStructList)
				{
					pStructList->IgnoreOnExport = Dlg.m_IgnoreOnExport;
					pStructList->StructList = Dlg.m_StructDefineList;
					m_DataStructDefineList.push_back(pStructList);
					FillList();
				}
			}
		}
	}
}

void CDlgDataStructDefine::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		size_t Index=m_lvList.GetItemData(Item);
		if(Index<m_DataStructDefineList.size())
		{
			switch(m_DataStructDefineList[Index]->ListType)
			{
			case DATA_STRUCT_CONST:
				{
					CONST_DEFINE_LIST * pConstList=(CONST_DEFINE_LIST *)m_DataStructDefineList[Index];

					CDlgConstList Dlg;
					Dlg.m_ListName=pConstList->ListName;
					Dlg.m_ConstDefineList=pConstList->ConstList;
					Dlg.m_IgnoreOnExport = pConstList->IgnoreOnExport;
					if(Dlg.DoModal()==IDOK)
					{
						pConstList->ListName=Dlg.m_ListName;
						pConstList->IgnoreOnExport = Dlg.m_IgnoreOnExport;
						pConstList->ConstList=Dlg.m_ConstDefineList;
						FillList();
					}
				}
				break;
			case DATA_STRUCT_ENUM:
				{
					ENUM_DEFINE_LIST * pEnumList=(ENUM_DEFINE_LIST *)m_DataStructDefineList[Index];

					CDlgEnumList Dlg;
					Dlg.m_ListName=pEnumList->ListName;
					Dlg.m_EnumDefineList=pEnumList->EnumList;
					Dlg.m_IgnoreOnExport = pEnumList->IgnoreOnExport;
					if(Dlg.DoModal()==IDOK)
					{
						pEnumList->ListName=Dlg.m_ListName;
						pEnumList->IgnoreOnExport = Dlg.m_IgnoreOnExport;
						pEnumList->EnumList=Dlg.m_EnumDefineList;
						FillList();
					}
				}
				break;
			case DATA_STRUCT_STRUCT:
				{
					STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)m_DataStructDefineList[Index];

					CDlgStructList Dlg;
					Dlg.m_ListName=pStructList->ListName;
					Dlg.m_StructDefineList=pStructList->StructList;
					Dlg.m_IgnoreOnExport = pStructList->IgnoreOnExport;
					if(Dlg.DoModal()==IDOK)
					{
						pStructList->ListName=Dlg.m_ListName;
						pStructList->IgnoreOnExport = Dlg.m_IgnoreOnExport;
						pStructList->StructList=Dlg.m_StructDefineList;
						GetMainDlg()->MakeCurVarType(true);
						FillList();
					}
				}		
				break;
			}		
			
		}
	}
}

void CDlgDataStructDefine::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		size_t Index=m_lvList.GetItemData(Item);
		if(Index<m_DataStructDefineList.size())
		{
			CString Temp;
			Temp.Format("是否要删除数据结构[%s]？",m_DataStructDefineList[Index]->ListName);
			if(AfxMessageBox(Temp,MB_YESNO)==IDYES)
			{
				if(GetMainDlg()->DeleteDataStructList(m_DataStructDefineList[Index]->ModuleID, 
					(DATA_STRUCT_TYPE)m_DataStructDefineList[Index]->ListType,
					m_DataStructDefineList[Index]->ListName))
				{
					m_DataStructDefineList.erase(m_DataStructDefineList.begin() + Index);
					FillList();
				}
			}

		}
	}
}

void CDlgDataStructDefine::OnBnClickedButtonMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvList.GetItemData(Item);
		if(Index>0&&Index<m_DataStructDefineList.size())
		{
			BASE_DATA_STRUCT_DEFINE_LIST * pTemp=m_DataStructDefineList[Index];
			if (GetMainDlg()->DataStructListMoveUp(pTemp))
			{
				m_DataStructDefineList.erase(m_DataStructDefineList.begin() + Index);
				m_DataStructDefineList.insert(m_DataStructDefineList.begin() + Index - 1, pTemp);
				FillList();
				SelectItemByName(pTemp->ListName, g_szDATA_STRUCT_TYPE[pTemp->ListType]);
			}
		}
	}
}

void CDlgDataStructDefine::OnBnClickedButtonOveDown()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvList.GetItemData(Item);
		if(Index<m_DataStructDefineList.size()-1)
		{
			BASE_DATA_STRUCT_DEFINE_LIST * pTemp=m_DataStructDefineList[Index];
			if (GetMainDlg()->DataStructListMoveDown(pTemp))
			{
				m_DataStructDefineList.erase(m_DataStructDefineList.begin() + Index);
				m_DataStructDefineList.insert(m_DataStructDefineList.begin() + Index + 1, pTemp);
				FillList();
				SelectItemByName(pTemp->ListName, g_szDATA_STRUCT_TYPE[pTemp->ListType]);
			}
		}
	}
}

void CDlgDataStructDefine::OnBnClickedDataObjectModifyFlagsManager()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgDataObjectModifyFlagManager Dlg;
	Dlg.m_DataObjectModifyFlags = *m_pDataObjectModifyFlags;
	if(Dlg.DoModal()==IDOK)
	{
		*m_pDataObjectModifyFlags = Dlg.m_DataObjectModifyFlags;
	}
}

void CDlgDataStructDefine::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDlgDataStructDefine::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDlgDataStructDefine::OnBnClickedButtonBaseTypeDef()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgTypeEditor Dlg;

	Dlg.m_TypeList = GetMainDlg()->GetBaseTypeList();
	if(Dlg.DoModal()==IDOK)
	{
		GetMainDlg()->GetBaseTypeList() = Dlg.m_TypeList;
		GetMainDlg()->SaveTypeDef();
	}
}

void CDlgDataStructDefine::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	OnBnClickedButtonEdit();
}


void CDlgDataStructDefine::OnBnClickedButtonClone()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgDataStructDefine::OnBnClickedButtonMove2()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgModuleSelector Dlg;

	if (Dlg.DoModal() == IDOK)
	{
		POSITION Pos = m_lvList.GetFirstSelectedItemPosition();
		while (Pos)
		{
			int Item = m_lvList.GetNextSelectedItem(Pos);
			UINT Index = (UINT)m_lvList.GetItemData(Item);
			if (Index < m_DataStructDefineList.size() - 1)
			{
				BASE_DATA_STRUCT_DEFINE_LIST * pTemp = m_DataStructDefineList[Index];

				MODULE_DEFINE_INFO * pModuleInfo = GetMainDlg()->GetModuleInfo(Dlg.m_SelectedModuleID);
				if (pModuleInfo&&pModuleInfo->ModuleID != pTemp->ModuleID)
				{
					GetMainDlg()->DataStructListMoveToModule(pTemp, pModuleInfo);
				}
			}
		}
		FillList();
	}
}
