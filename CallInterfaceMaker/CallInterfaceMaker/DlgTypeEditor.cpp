/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgTypeEditor.cpp                                        */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgTypeEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTypeEditor.h"




// CDlgTypeEditor 对话框

IMPLEMENT_DYNAMIC(CDlgTypeEditor, CDialog)

CDlgTypeEditor::CDlgTypeEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTypeEditor::IDD, pParent)
	, m_TypeName(_T(""))
	, m_CType(_T(""))
	, m_IsRefType(FALSE)
	, m_UseParamBind(FALSE)
	, m_IsBaseType(FALSE)
	, m_IsVariableLen(FALSE)
{
	m_IsModified=false;
	m_CurSelectItem=-1;
}

CDlgTypeEditor::~CDlgTypeEditor()
{
}

void CDlgTypeEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tbOperations);
	DDX_Control(pDX, IDC_EDIT_OPERATION, m_edOperation);
	DDX_Control(pDX, IDC_LIST_TYPES, m_lvTypeList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_TypeName);
	DDX_Text(pDX, IDC_EDIT_CTYPE, m_CType);
	DDX_Check(pDX, IDC_CHECK_IS_REF_TYPE, m_IsRefType);
	DDX_Check(pDX, IDC_CHECK_DB_USE_PARAM_BIND, m_UseParamBind);
	DDX_Check(pDX, IDC_CHECK_BASE_TYPE, m_IsBaseType);
	DDX_Check(pDX, IDC_CHECK_VARIABLE_LEN, m_IsVariableLen);
}


BEGIN_MESSAGE_MAP(CDlgTypeEditor, CDialog)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &CDlgTypeEditor::OnTcnSelchangingTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgTypeEditor::OnTcnSelchangeTab1)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_CTYPE, &CDlgTypeEditor::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_OPERATION, &CDlgTypeEditor::OnEnChangeEdit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TYPES, &CDlgTypeEditor::OnNMClickListTypes)
	ON_BN_CLICKED(ID_NEW, &CDlgTypeEditor::OnBnClickedNew)
	ON_BN_CLICKED(ID_DEL, &CDlgTypeEditor::OnBnClickedDel)
	
	ON_BN_CLICKED(IDC_CHECK_IS_REF_TYPE, &CDlgTypeEditor::OnBnClickedCheckIsRefType)
END_MESSAGE_MAP()


// CDlgTypeEditor 消息处理程序

BOOL CDlgTypeEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	TabInitOperations(m_tbOperations);

	m_tbOperations.SetCurSel(0);
	m_lvTypeList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_lvTypeList.InsertColumn(0,"名称",LVCFMT_LEFT,80);
	m_lvTypeList.InsertColumn(1,"C类型",LVCFMT_LEFT,80);

	FillList();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTypeEditor::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	POSITION Pos=m_lvTypeList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvTypeList.GetNextSelectedItem(Pos);
		FetchItemData(Item);
	}
}

void CDlgTypeEditor::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	POSITION Pos=m_lvTypeList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvTypeList.GetNextSelectedItem(Pos);
		ShowItemData(Item);
	}
}


void CDlgTypeEditor::OnEnChangeEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_IsModified=true;
}

void CDlgTypeEditor::OnNMClickListTypes(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	int NewSelect=-1;
	POSITION Pos=m_lvTypeList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		NewSelect=m_lvTypeList.GetNextSelectedItem(Pos);
	}
	
	if(NewSelect!=m_CurSelectItem)
	{
		if(m_CurSelectItem>=0)
			FetchItemData(m_CurSelectItem);
		m_CurSelectItem=NewSelect;			
		ShowItemData(m_CurSelectItem);
		
	}
	
	*pResult = 0;
}

void CDlgTypeEditor::ShowItemData(int Item)
{
	if(Item<0)
	{
		m_TypeName.Empty();
		m_CType.Empty();
		m_edOperation.SetWindowText("");		
		m_IsRefType = FALSE;
		m_UseParamBind = FALSE;
		m_IsBaseType = FALSE;
		m_IsVariableLen = FALSE;
		UpdateData(false);
		return;
	}
	UINT Index=m_lvTypeList.GetItemData(Item);
	if(Index<m_TypeList.size())
	{
		m_TypeName=m_TypeList[Index].Name;
		m_CType=m_TypeList[Index].CType;
		m_IsRefType = (m_TypeList[Index].Flag&TYPE_DEFINE_FLAG_REF_TYPE) ? TRUE : FALSE;
		m_UseParamBind = (m_TypeList[Index].Flag&TYPE_DEFINE_FLAG_DB_USE_PARAM_BIND) ? TRUE : FALSE;
		m_IsBaseType = (m_TypeList[Index].Flag & TYPE_DEFINE_FLAG_BASE_TYPE) ? TRUE : FALSE;
		m_IsVariableLen = (m_TypeList[Index].Flag & TYPE_DEFINE_FLAG_VARIABLE_LEN) ? TRUE : FALSE;
		GetDlgItem(IDC_EDIT_NAME)->EnableWindow((m_TypeList[Index].Flag & TYPE_DEFINE_FLAG_SYSTEM_TYPE) ? FALSE : TRUE);
		GetDlgItem(IDC_EDIT_CTYPE)->EnableWindow((m_TypeList[Index].Flag & TYPE_DEFINE_FLAG_SYSTEM_TYPE) ? FALSE : TRUE);
		
		TabShowOperations(m_tbOperations, m_edOperation, m_TypeList[Index].GenerateOperations);
		UpdateData(false);
	}
}
void CDlgTypeEditor::FetchItemData(int Item)
{	
	UINT Index=m_lvTypeList.GetItemData(Item);
	if(Index<m_TypeList.size())
	{
		UpdateData(true);
		m_TypeList[Index].Name=m_TypeName;
		m_TypeList[Index].CType=m_CType;
		if (m_IsRefType)
		{
			m_TypeList[Index].Flag |= TYPE_DEFINE_FLAG_REF_TYPE;
		}
		else
		{
			m_TypeList[Index].Flag &= ~TYPE_DEFINE_FLAG_REF_TYPE;
		}
		if(m_UseParamBind)
		{
			m_TypeList[Index].Flag |= TYPE_DEFINE_FLAG_DB_USE_PARAM_BIND;
		}
		else
		{
			m_TypeList[Index].Flag &= ~TYPE_DEFINE_FLAG_DB_USE_PARAM_BIND;
		}
		if(m_IsBaseType)
		{
			m_TypeList[Index].Flag |= TYPE_DEFINE_FLAG_BASE_TYPE;
		}
		else
		{
			m_TypeList[Index].Flag &= ~TYPE_DEFINE_FLAG_BASE_TYPE;
		}
		if (m_IsVariableLen)
		{
			m_TypeList[Index].Flag |= TYPE_DEFINE_FLAG_VARIABLE_LEN;
		}
		else
		{
			m_TypeList[Index].Flag &= ~TYPE_DEFINE_FLAG_VARIABLE_LEN;
		}
		TabFetchOperations(m_tbOperations, m_edOperation, m_TypeList[Index].GenerateOperations);
		
		m_lvTypeList.SetItemText(Item,0,m_TypeList[Index].Name);
		m_lvTypeList.SetItemText(Item,1,m_TypeList[Index].CType);		
	}
	m_IsModified = false;
}
void CDlgTypeEditor::OnBnClickedNew()
{
	// TODO: 在此添加控件通知处理程序代码
	TYPE_DEFINE NewType;
	NewType.Name="新类型";
	NewType.CType="char";
	m_TypeList.push_back(NewType);
	UINT Index=m_TypeList.size()-1;
	m_IsModified=true;
	int Item=m_lvTypeList.InsertItem(m_lvTypeList.GetItemCount(),m_TypeList[Index].Name);
	m_lvTypeList.SetItemText(Item,1,m_TypeList[Index].CType);
	m_lvTypeList.SetItemData(Item,Index);
	
}

void CDlgTypeEditor::OnBnClickedDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvTypeList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvTypeList.GetNextSelectedItem(Pos);
		if(AfxMessageBox("是否要删除该类型定义?",MB_YESNO)==IDYES)
		{
			UINT Index=m_lvTypeList.GetItemData(Item);
			m_TypeList.erase(m_TypeList.begin()+Index);
			m_CurSelectItem = -1;
			FillList();
		}
	}
}


void CDlgTypeEditor::FillList()
{
	m_lvTypeList.DeleteAllItems();
	for(size_t i=0;i<m_TypeList.size();i++)
	{
		int Item=m_lvTypeList.InsertItem(i,m_TypeList[i].Name);
		m_lvTypeList.SetItemText(Item,1,m_TypeList[i].CType);
		m_lvTypeList.SetItemData(Item,i);
	}
}
void CDlgTypeEditor::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_IsModified)
	{
		if(AfxMessageBox("是否要放弃已作出的更改？",MB_YESNO)==IDYES)
		{
			CDialog::OnCancel();
		}
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CDlgTypeEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_IsModified)
		FetchItemData(m_CurSelectItem);
	CDialog::OnOK();
}




void CDlgTypeEditor::OnBnClickedCheckIsRefType()
{
	// TODO:  在此添加控件通知处理程序代码
	m_IsModified = true;
}


