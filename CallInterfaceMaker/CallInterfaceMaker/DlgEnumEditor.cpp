// DlgStructEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgEnumEditor.h"




// CDlgEnumEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgEnumEditor, CDialog)

CDlgEnumEditor::CDlgEnumEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnumEditor::IDD, pParent)	
{
}

CDlgEnumEditor::~CDlgEnumEditor()
{
}

void CDlgEnumEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEMBER_LIST, m_lvMemberList);
	DDX_Control(pDX, IDC_COMBO_BIND_DATA_TYPE, m_cbBindDataType);

	DDX_Text(pDX, IDC_EDIT_NAME, m_EnumDefineInfo.Name);
	DDX_Text(pDX, IDC_EDIT_SHORT_NAME, m_EnumDefineInfo.ShortName);	
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_EnumDefineInfo.Description);


	DDX_Check(pDX, IDC_CHECK_IS_64BIT, m_EnumDefineInfo.Flag, ENUM_DEFINE_FLAG_IS_64BIT);
	DDX_Check(pDX, IDC_CHECK_IS_FLAG, m_EnumDefineInfo.Flag, ENUM_DEFINE_FLAG_IS_FLAG);
	DDX_Check(pDX, IDC_CHECK_EXPORT_STR_VALUE, m_EnumDefineInfo.Flag, ENUM_DEFINE_FLAG_EXPORT_STR_VALUE);
	DDX_Check(pDX, IDC_CHECK_EXPORT_STR_TRANS_FN, m_EnumDefineInfo.Flag, ENUM_DEFINE_FLAG_EXPORT_STR_TRANS_FN);
	DDX_Check(pDX, IDC_CHECK_EXPORT_ENUM_LIST, m_EnumDefineInfo.Flag, ENUM_DEFINE_FLAG_EXPORT_ENUM_LIST);
	DDX_Check(pDX, IDC_CHECK_EXPORT_BIND_DATA_PROCESS, m_EnumDefineInfo.Flag, ENUM_DEFINE_FLAG_EXPORT_BIND_DATA_PROCESS);


	
		

	DDX_Text(pDX, IDC_EDIT_MEMBER_NAME, m_EnumMemberInfo.Name);
	DDX_Text(pDX, IDC_EDIT_MEMBER_VALUE, m_EnumMemberInfo.Value);
	DDX_Text(pDX, IDC_EDIT_MEMBER_STR_VALUE, m_EnumMemberInfo.StrValue);
	DDX_Text(pDX, IDC_EDIT_MEMBER_DESCRIPTION, m_EnumMemberInfo.Description);

	DDX_Check(pDX, IDC_CHECK_NOT_EXPORT_OTHER, m_EnumMemberInfo.Flag, ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER);
	DDX_Check(pDX, IDC_CHECK_IS_BIT_MASK, m_EnumMemberInfo.Flag, ENUM_MEMBER_FLAG_IS_BIT_MASK);

	if (pDX->m_bSaveAndValidate)
	{
		m_cbBindDataType.GetWindowText(m_EnumMemberInfo.BindDataType);
		if (m_EnumMemberInfo.BindDataType == "��")
			m_EnumMemberInfo.BindDataType = "";
	}
	else
	{
		m_cbBindDataType.SetWindowText(m_EnumMemberInfo.BindDataType);
	}
}


BEGIN_MESSAGE_MAP(CDlgEnumEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MEMBER, &CDlgEnumEditor::OnBnClickedButtonAddMember)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_MEMBER, &CDlgEnumEditor::OnBnClickedButtonEditMember)
	ON_BN_CLICKED(IDC_BUTTON_DEL_MEMBER, &CDlgEnumEditor::OnBnClickedButtonDelMember)
	ON_BN_CLICKED(IDOK, &CDlgEnumEditor::OnBnClickedOk)
	
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_MOVE_UP, &CDlgEnumEditor::OnBnClickedButtonMemberMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MEMBER_MOVE_DOWN, &CDlgEnumEditor::OnBnClickedButtonMemberMoveDown)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MEMBER_LIST, &CDlgEnumEditor::OnNMClickListMemberList)
	ON_BN_CLICKED(IDC_BUTTON_SORT, &CDlgEnumEditor::OnBnClickedButtonSort)
END_MESSAGE_MAP()


// CDlgEnumEditor ��Ϣ�������
void CDlgEnumEditor::FillList()
{
	m_lvMemberList.DeleteAllItems();
	CString Temp;
	for (size_t i = 0; i < m_EnumDefineInfo.MemberList.size(); i++)
	{
		int Item = m_lvMemberList.InsertItem(i, m_EnumDefineInfo.MemberList[i].Name);
		m_lvMemberList.SetItemText(Item, 1, m_EnumDefineInfo.MemberList[i].Value);
		Temp.Format("0x%X", m_EnumDefineInfo.MemberList[i].Flag);
		m_lvMemberList.SetItemText(Item, 2, Temp);
		m_lvMemberList.SetItemText(Item, 3, m_EnumDefineInfo.MemberList[i].StrValue);
		m_lvMemberList.SetItemText(Item, 4, m_EnumDefineInfo.MemberList[i].BindDataType);
		m_lvMemberList.SetItemText(Item, 5, m_EnumDefineInfo.MemberList[i].Description);
		m_lvMemberList.SetItemData(Item, i);
	}

	ListCtrlColAutoFit(m_lvMemberList);
}

void CDlgEnumEditor::SelectItemByName(LPCTSTR szName)
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

BOOL CDlgEnumEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lvMemberList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvMemberList.InsertColumn(0,_T("����"),LVCFMT_LEFT,300);
	m_lvMemberList.InsertColumn(1,_T("ֵ"),LVCFMT_LEFT,60);	
	m_lvMemberList.InsertColumn(2, _T("��־λ"), LVCFMT_LEFT, 60);
	m_lvMemberList.InsertColumn(3,_T("��ʾ��"),LVCFMT_LEFT,100);
	m_lvMemberList.InsertColumn(4, _T("������������"), LVCFMT_LEFT, 100);
	m_lvMemberList.InsertColumn(5, _T("����"), LVCFMT_LEFT, 100);

	m_cbBindDataType.ResetContent();
	m_cbBindDataType.AddString(_T("��"));
	for (UINT i = 0; i < GetMainDlg()->GetVarTypeCount(); i++)
	{
		m_cbBindDataType.AddString(GetMainDlg()->GetVarType(i)->Name);
	}
	
	FillList();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgEnumEditor::OnBnClickedButtonAddMember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	if(m_EnumMemberInfo.Name.IsEmpty())
	{
		AfxMessageBox("�������Ա����");
		return;
	}
	
	m_EnumDefineInfo.MemberList.push_back(m_EnumMemberInfo);
	FillList();
	m_lvMemberList.EnsureVisible(m_lvMemberList.GetItemCount()-1,false);
}

void CDlgEnumEditor::OnBnClickedButtonEditMember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		size_t Index=m_lvMemberList.GetItemData(Item);
		if(Index<m_EnumDefineInfo.MemberList.size())
		{
			UpdateData(true);
			m_EnumDefineInfo.MemberList[Index]=m_EnumMemberInfo;
			FillList();
		}
	}
}

void CDlgEnumEditor::OnBnClickedButtonDelMember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		size_t Index=m_lvMemberList.GetItemData(Item);
		CString Msg;
		Msg.Format("�Ƿ�Ҫɾ����Ա[%s]",
			m_lvMemberList.GetItemText(Item,0));
		if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
		{
			m_EnumDefineInfo.MemberList.erase(m_EnumDefineInfo.MemberList.begin()+Index);
			FillList();
		}
	}
}



void CDlgEnumEditor::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);	

	m_EnumDefineInfo.Name.Trim();

	if(m_EnumDefineInfo.ShortName.IsEmpty())
		m_EnumDefineInfo.ShortName=m_EnumDefineInfo.Name;	

	UpdateData(false);
	OnOK();
}



void CDlgEnumEditor::OnBnClickedButtonMemberMoveUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMemberList.GetItemData(Item);
		if(Index>0&&Index<m_EnumDefineInfo.MemberList.size())
		{
			ENUM_MEMBER_INFO Temp=m_EnumDefineInfo.MemberList[Index];
			m_EnumDefineInfo.MemberList.erase(m_EnumDefineInfo.MemberList.begin()+Index);
			m_EnumDefineInfo.MemberList.insert(m_EnumDefineInfo.MemberList.begin()+Index-1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgEnumEditor::OnBnClickedButtonMemberMoveDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvMemberList.GetItemData(Item);
		if(Index<m_EnumDefineInfo.MemberList.size()-1)
		{
			ENUM_MEMBER_INFO Temp=m_EnumDefineInfo.MemberList[Index];
			m_EnumDefineInfo.MemberList.erase(m_EnumDefineInfo.MemberList.begin()+Index);
			m_EnumDefineInfo.MemberList.insert(m_EnumDefineInfo.MemberList.begin()+Index+1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}


void CDlgEnumEditor::OnNMClickListMemberList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	POSITION Pos=m_lvMemberList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvMemberList.GetNextSelectedItem(Pos);
		size_t Index=m_lvMemberList.GetItemData(Item);
		if(Index<m_EnumDefineInfo.MemberList.size())
		{
			UpdateData(true);
			m_EnumMemberInfo=m_EnumDefineInfo.MemberList[Index];
			UpdateData(false);
		}
	}
}

static int MemberComp(LPCVOID p1, LPCVOID p2)
{
	const ENUM_MEMBER_INFO* pInfo1 = (const ENUM_MEMBER_INFO*)p1;
	const ENUM_MEMBER_INFO* pInfo2 = (const ENUM_MEMBER_INFO*)p2;
	return strcmp(pInfo1->Name, pInfo2->Name);
}

void CDlgEnumEditor::OnBnClickedButtonSort()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	qsort(m_EnumDefineInfo.MemberList.begin()._Ptr, m_EnumDefineInfo.MemberList.size(), sizeof(ENUM_MEMBER_INFO), MemberComp);
	FillList();
}
