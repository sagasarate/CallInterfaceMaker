// DlgStructEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgStructEditor.h"




// CDlgStructEditor 对话框

IMPLEMENT_DYNAMIC(CDlgStructEditor, CDialog)

CDlgStructEditor::CDlgStructEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStructEditor::IDD, pParent)
{
	m_StructDefineInfo.IDSeed=1;

	//m_StructDefineInfo.PackOperation=GetMainDlg()->GetConfig().DefaultStructPackOperation;
	//m_StructDefineInfo.UnpackOperation=GetMainDlg()->GetConfig().DefaultStructUnpackOperation;
	//m_StructDefineInfo.SizeCaculateOperation=GetMainDlg()->GetConfig().DefaultStructSizeCaculateOperation;
	//m_StructDefineInfo.ReferenceDefine=GetMainDlg()->GetConfig().DefaultStructReferenceDefine;
}

CDlgStructEditor::~CDlgStructEditor()
{
}

void CDlgStructEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STRUCT_NAME, m_StructDefineInfo.Name);
	DDX_Text(pDX, IDC_EDIT_STRUCT_SHORT_NAME, m_StructDefineInfo.ShortName);
	DDX_Control(pDX, IDC_COMBO_BASE_STRUCT, m_cbBaseStruct);
	DDX_Control(pDX, IDC_LIST_MEMBER_LIST, m_lvMemberList);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_StructDefineInfo.Description);
	DDX_Text(pDX, IDC_EDIT_OBJECT_ID, m_StructDefineInfo.ObjectID);
	DDX_Check(pDX, IDC_CHECK_IS_DATA_OBJECT, m_StructDefineInfo.Flag, STRUCT_FLAG_IS_DATA_OBJECT);
	DDX_Check(pDX, IDC_CHECK_DECLARE_LATER, m_StructDefineInfo.Flag, STRUCT_FLAG_DECLARE_LATER);
	DDX_Check(pDX, IDC_CHECK_EXPORT_XML_PROCESS, m_StructDefineInfo.Flag, STRUCT_FLAG_EXPORT_XML_PROCESS);
	DDX_Check(pDX, IDC_CHECK_EXPORT_JSON_PROCESS, m_StructDefineInfo.Flag, STRUCT_FLAG_EXPORT_JSON_PROCESS);
	DDX_Check(pDX, IDC_CHECK_EXPORT_DB_PROCESS, m_StructDefineInfo.Flag, STRUCT_FLAG_EXPORT_DB_PROCESS);
	DDX_Check(pDX, IDC_CHECK_EXPORT_EDITOR_PROCESS, m_StructDefineInfo.Flag, STRUCT_FLAG_EXPORT_EDITOR_PROCESS);
	DDX_Check(pDX, IDC_CHECK_EXPORT_LOG_PROCESS, m_StructDefineInfo.Flag, STRUCT_FLAG_EXPORT_LOG_PROCESS);
	DDX_Check(pDX, IDC_CHECK_EXPORT_LUA_PROCESS, m_StructDefineInfo.Flag, STRUCT_FLAG_EXPORT_LUA_PROCESS);
	DDX_Check(pDX, IDC_CHECK_EXPORT_XLS_PROCESS, m_StructDefineInfo.Flag, STRUCT_FLAG_EXPORT_XLS_PROCESS);
}


BEGIN_MESSAGE_MAP(CDlgStructEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MEMBER, &CDlgStructEditor::OnBnClickedButtonAddMember)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_MEMBER, &CDlgStructEditor::OnBnClickedButtonEditMember)
	ON_BN_CLICKED(IDC_BUTTON_DEL_MEMBER, &CDlgStructEditor::OnBnClickedButtonDelMember)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MEMBER_LIST, &CDlgStructEditor::OnNMDblclkListMemberList)
	ON_BN_CLICKED(IDOK, &CDlgStructEditor::OnBnClickedOk)
	
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_MOVE_UP, &CDlgStructEditor::OnBnClickedButtonMemberMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_MOVE_DOWN, &CDlgStructEditor::OnBnClickedButtonMemberMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_GENERATE_OPERATIONS, &CDlgStructEditor::OnBnClickedButtonMemberGenerateOperations)
	ON_BN_CLICKED(IDC_BUTTON_SORT, &CDlgStructEditor::OnBnClickedButtonSort)
END_MESSAGE_MAP()


// CDlgStructEditor 消息处理程序
void CDlgStructEditor::FillList()
{
	m_lvMemberList.DeleteAllItems();
	int i = 0;
	for (STRUCT_MEMBER_INFO& Member : m_StructDefineInfo.MemberList)
	{
		CString Temp;

		int Item = m_lvMemberList.InsertItem(m_lvMemberList.GetItemCount(), Member.Name);
		
		if (Member.IsArray)
			Temp.Format("%s[%u,%u]", Member.Type, Member.ArrayStartLength, Member.ArrayGrowLength);
		else
			Temp = Member.Type;
		m_lvMemberList.SetItemText(Item, 1, Temp);
		Temp.Format("%u", Member.ID);
		m_lvMemberList.SetItemText(Item, 2, Temp);
		Temp.Format("0x%X", Member.Flag);
		m_lvMemberList.SetItemText(Item, 3, Temp);
		m_lvMemberList.SetItemText(Item, 4, Member.ShowName);
		m_lvMemberList.SetItemText(Item, 5, Member.BindData);
		m_lvMemberList.SetItemText(Item, 6, Member.DBLength);
		if (Member.DBIndexType < DB_INDEX_TYPE_MAX)
			m_lvMemberList.SetItemText(Item, 7, g_szDB_INDEX_TYPE[Member.DBIndexType]);
		m_lvMemberList.SetItemText(Item, 8, Member.ExtendType);
		m_lvMemberList.SetItemText(Item, 9, Member.PackFlag);
		m_lvMemberList.SetItemText(Item, 10, Member.Description);
		m_lvMemberList.SetItemData(Item, i++);
	}

	ListCtrlColAutoFit(m_lvMemberList);
}

void CDlgStructEditor::SelectItemByName(LPCTSTR szName)
{
	for(int i=0;i<m_lvMemberList.GetItemCount();i++)
	{
		if(m_lvMemberList.GetItemText(i,0)==szName)
		{
			m_lvMemberList.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			m_lvMemberList.EnsureVisible(i,false);
		}
		else
		{
			m_lvMemberList.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}

BOOL CDlgStructEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lvMemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvMemberList.InsertColumn(0,_T("名称"),LVCFMT_LEFT,100);
	m_lvMemberList.InsertColumn(1,_T("类型"),LVCFMT_LEFT,100);
	m_lvMemberList.InsertColumn(2,_T("ID"),LVCFMT_LEFT,60);
	m_lvMemberList.InsertColumn(3, _T("Flag"), LVCFMT_LEFT, 60);
	m_lvMemberList.InsertColumn(4, _T("显示名"), LVCFMT_LEFT, 100);
	m_lvMemberList.InsertColumn(5, _T("关联数据"), LVCFMT_LEFT, 100);
	m_lvMemberList.InsertColumn(6, _T("数据库长度"), LVCFMT_LEFT, 80);
	m_lvMemberList.InsertColumn(7, _T("索引类型"), LVCFMT_LEFT, 60);
	m_lvMemberList.InsertColumn(8, _T("扩展类型"), LVCFMT_LEFT, 60);
	m_lvMemberList.InsertColumn(9, _T("打包标志位"), LVCFMT_LEFT, 100);
	m_lvMemberList.InsertColumn(10, _T("描述"), LVCFMT_LEFT, 100);

	

	m_cbBaseStruct.ResetContent();
	m_cbBaseStruct.AddString(_T("无"));
	for(UINT i=0;i<GetMainDlg()->GetVarTypeCount();i++)
	{
		if(GetMainDlg()->GetVarType(i)->Flag&TYPE_DEFINE_FLAG_STRUCT)
		{
			m_cbBaseStruct.AddString(GetMainDlg()->GetVarType(i)->Name);
		}
	}	

	if(!m_StructDefineInfo.BaseStruct.IsEmpty())
	{
		for(int i=0;i<m_cbBaseStruct.GetCount();i++)
		{
			CString Temp;
			m_cbBaseStruct.GetLBText(i,Temp);
			if(Temp==m_StructDefineInfo.BaseStruct)
			{
				m_cbBaseStruct.SetCurSel(i);
				break;
			}
		}
	}
	FillList();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgStructEditor::OnBnClickedButtonAddMember()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgStructMemberEditor Dlg;

	if(Dlg.DoModal()==IDOK)
	{
		Dlg.m_StructMemberInfo.ID=m_StructDefineInfo.IDSeed;
		m_StructDefineInfo.IDSeed++;
		m_StructDefineInfo.MemberList.push_back(Dlg.m_StructMemberInfo);
		FillList();
	}
}

void CDlgStructEditor::OnBnClickedButtonEditMember()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		size_t Index=m_lvMemberList.GetItemData(Item);
		if(Index<m_StructDefineInfo.MemberList.size())
		{
			CDlgStructMemberEditor Dlg;

			Dlg.m_StructMemberInfo=m_StructDefineInfo.MemberList[Index];
			if(Dlg.DoModal()==IDOK)
			{
				m_StructDefineInfo.MemberList[Index]=Dlg.m_StructMemberInfo;
				FillList();
			}
		}
	}
}

void CDlgStructEditor::OnBnClickedButtonDelMember()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		size_t Index=m_lvMemberList.GetItemData(Item);
		CString Msg;
		Msg.Format("是否要删除成员[%s]",
			m_lvMemberList.GetItemText(Item,0));
		if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
		{
			m_StructDefineInfo.MemberList.erase(m_StructDefineInfo.MemberList.begin()+Index);
			FillList();
		}
	}
}

void CDlgStructEditor::OnNMDblclkListMemberList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonEditMember();
	*pResult = 0;
}

void CDlgStructEditor::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_cbBaseStruct.GetWindowText(m_StructDefineInfo.BaseStruct);
	if(m_StructDefineInfo.BaseStruct=="无")
		m_StructDefineInfo.BaseStruct="";


	m_StructDefineInfo.Name.Trim();
	m_StructDefineInfo.BaseStruct.Trim();

	if(m_StructDefineInfo.ShortName.IsEmpty())
		m_StructDefineInfo.ShortName=m_StructDefineInfo.Name;
	UpdateData(false);
	OnOK();
}



void CDlgStructEditor::OnBnClickedButtonMemberMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMemberList.GetItemData(Item);
		if(Index>0&&Index<m_StructDefineInfo.MemberList.size())
		{
			STRUCT_MEMBER_INFO Temp=m_StructDefineInfo.MemberList[Index];
			m_StructDefineInfo.MemberList.erase(m_StructDefineInfo.MemberList.begin()+Index);
			m_StructDefineInfo.MemberList.insert(m_StructDefineInfo.MemberList.begin()+Index-1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgStructEditor::OnBnClickedButtonMemberMoveDown()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMemberList.GetItemData(Item);
		if(Index<m_StructDefineInfo.MemberList.size()-1)
		{
			STRUCT_MEMBER_INFO Temp=m_StructDefineInfo.MemberList[Index];
			m_StructDefineInfo.MemberList.erase(m_StructDefineInfo.MemberList.begin()+Index);
			m_StructDefineInfo.MemberList.insert(m_StructDefineInfo.MemberList.begin()+Index+1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgStructEditor::OnBnClickedButtonMemberGenerateOperations()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgGenerateOperationsEditor Dlg;
	Dlg.m_GenerateOperations=m_StructDefineInfo.GenerateOperations;
	if(Dlg.DoModal()==IDOK)
	{
		m_StructDefineInfo.GenerateOperations=Dlg.m_GenerateOperations;
	}
}

static int MemberComp(LPCVOID p1, LPCVOID p2)
{
	const STRUCT_MEMBER_INFO* pInfo1 = (const STRUCT_MEMBER_INFO*)p1;
	const STRUCT_MEMBER_INFO* pInfo2 = (const STRUCT_MEMBER_INFO*)p2;
	return strcmp(pInfo1->Name, pInfo2->Name);
}

void CDlgStructEditor::OnBnClickedButtonSort()
{
	// TODO: 在此添加控件通知处理程序代码
	qsort(m_StructDefineInfo.MemberList.begin()._Ptr, m_StructDefineInfo.MemberList.size(), sizeof(STRUCT_MEMBER_INFO), MemberComp);
	FillList();
}
