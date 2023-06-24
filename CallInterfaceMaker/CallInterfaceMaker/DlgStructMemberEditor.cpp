// DlgStructMemberEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgStructMemberEditor.h"




// CDlgStructMemberEditor 对话框

IMPLEMENT_DYNAMIC(CDlgStructMemberEditor, CDialog)

CDlgStructMemberEditor::CDlgStructMemberEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStructMemberEditor::IDD, pParent)
{
	
}

CDlgStructMemberEditor::~CDlgStructMemberEditor()
{
}

void CDlgStructMemberEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cbType);
	DDX_Control(pDX, IDC_COMBO_DB_INDEX_TYPE, m_cbDBIndexType);

	DDX_Text(pDX, IDC_EDIT_NAME, m_StructMemberInfo.Name);
	
	DDX_Check(pDX, IDC_CHECK_IS_ARRAY, m_StructMemberInfo.IsArray);
	DDX_Text(pDX, IDC_EDIT_ARRAY_START_LENGTH, m_StructMemberInfo.ArrayStartLength);
	DDX_Text(pDX, IDC_EDIT_ARRAY_GROW_LENGTH, m_StructMemberInfo.ArrayGrowLength);
	DDX_Text(pDX, IDC_EDIT_SHOW_NAME, m_StructMemberInfo.ShowName);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_StructMemberInfo.Description);

	DDX_Check(pDX, IDC_CHECK_NOT_MONITOR_UPDATE, m_StructMemberInfo.Flag, STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE);
	DDX_Check(pDX, IDC_CHECK_EXCLUDE_IN_PACKET, m_StructMemberInfo.Flag, STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET);
	DDX_Check(pDX, IDC_CHECK_MERGE_TO_PARENT, m_StructMemberInfo.Flag, STRUCT_MEMBER_FLAG_MERGE_TO_PARENT);
	DDX_Check(pDX, IDC_CHECK_MUST_PACK, m_StructMemberInfo.Flag, STRUCT_MEMBER_FLAG_MUST_PACK);
	DDX_Check(pDX, IDC_CHECK_HIDE_IN_EDITOR_LIST, m_StructMemberInfo.Flag, STRUCT_MEMBER_FLAG_HIDE_IN_EDITOR_LIST);
	DDX_Check(pDX, IDC_CHECK_HIDE_IN_PROPERTY_GRID, m_StructMemberInfo.Flag, STRUCT_MEMBER_FLAG_HIDE_IN_PROPERTY_GRID);
	DDX_Check(pDX, IDC_CHECK_IGNORE_IN_LUA_PROCESS, m_StructMemberInfo.Flag, STRUCT_MEMBER_FLAG_IGNORE_IN_LUA_PROCESS);
	DDX_Check(pDX, IDC_CHECK_HIDE_IN_XLS, m_StructMemberInfo.Flag, STRUCT_MEMBER_FLAG_HIDE_IN_XLS);
	DDX_Check(pDX, IDC_CHECK_IS_KEY, m_StructMemberInfo.Flag, STRUCT_MEMBER_FLAG_IS_KEY);

	
	DDX_Text(pDX, IDC_EDIT_DB_LENGTH, m_StructMemberInfo.DBLength);
	DDX_Text(pDX, IDC_EDIT_PACK_FLAG, m_StructMemberInfo.PackFlag);
	DDX_Text(pDX, IDC_EDIT_BIND_DATA, m_StructMemberInfo.BindData);
	DDX_Text(pDX, IDC_EDIT_EXTEND_TYPE, m_StructMemberInfo.ExtendType);
}


BEGIN_MESSAGE_MAP(CDlgStructMemberEditor, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgStructMemberEditor::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgStructMemberEditor 消息处理程序

BOOL CDlgStructMemberEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int Sel=-1;
	for(UINT i=0;i<GetMainDlg()->GetVarTypeCount();i++)
	{
		m_cbType.InsertString(i,GetMainDlg()->GetVarType(i)->Name);
		if(m_StructMemberInfo.Type==GetMainDlg()->GetVarType(i)->Name)
			Sel=i;
	}
	m_cbType.SetCurSel(Sel);

	m_cbDBIndexType.ResetContent();
	for (UINT i = 0; i < DB_INDEX_TYPE_MAX; i++)
	{
		m_cbDBIndexType.InsertString(i, g_szDB_INDEX_TYPE[i]);
	}
	m_cbDBIndexType.SetCurSel(m_StructMemberInfo.DBIndexType);

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgStructMemberEditor::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_cbType.GetWindowText(m_StructMemberInfo.Type);
	m_StructMemberInfo.DBIndexType = m_cbDBIndexType.GetCurSel();
	m_StructMemberInfo.Name.Trim();
	m_StructMemberInfo.Description.Trim();
	UpdateData(false);
	OnOK();
}
