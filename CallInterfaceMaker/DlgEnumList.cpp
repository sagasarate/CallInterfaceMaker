// DlgStructEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgEnumList.h"




// CDlgEnumList �Ի���

IMPLEMENT_DYNAMIC(CDlgEnumList, CDialog)

CDlgEnumList::CDlgEnumList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnumList::IDD, pParent)
	, m_ListName(_T(""))
	, m_IgnoreOnExport(FALSE)
{

}

CDlgEnumList::~CDlgEnumList()
{
}

void CDlgEnumList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ENUM, m_lvEnum);
	DDX_Text(pDX, IDC_EDIT_LIST_NAME, m_ListName);
	DDX_Check(pDX, IDC_CHECK_IGNORE_ON_EXPORT, m_IgnoreOnExport);
}


BEGIN_MESSAGE_MAP(CDlgEnumList, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDlgEnumList::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgEnumList::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgEnumList::OnBnClickedButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ENUM, &CDlgEnumList::OnNMDblclkListStruct)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CDlgEnumList::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_OVE_DOWN, &CDlgEnumList::OnBnClickedButtonOveDown)
	ON_BN_CLICKED(IDC_BUTTON_CLONE, &CDlgEnumList::OnBnClickedButtonClone)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CDlgEnumList::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDOK, &CDlgEnumList::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SORT, &CDlgEnumList::OnBnClickedButtonSort)
END_MESSAGE_MAP()


// CDlgEnumList ��Ϣ�������
void CDlgEnumList::FillList()
{
	m_lvEnum.DeleteAllItems();
	for(size_t i=0;i<m_EnumDefineList.size();i++)
	{
		int Item=m_lvEnum.InsertItem(i,_T(""));
		FillItem(Item,m_EnumDefineList[i],i);
	}
}
void CDlgEnumList::FillItem(int Item,ENUM_DEFINE_INFO& EnumDefine,size_t Index)
{
	m_lvEnum.SetItemText(Item,0,EnumDefine.Name);
	m_lvEnum.SetItemText(Item,1,EnumDefine.Description);
	m_lvEnum.SetItemData(Item,Index);
}
void CDlgEnumList::SelectItemByName(LPCTSTR szName)
{
	for(int i=0;i<m_lvEnum.GetItemCount();i++)
	{
		if(m_lvEnum.GetItemText(i,0)==szName)
		{
			m_lvEnum.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			m_lvEnum.EnsureVisible(i,false);
		}
		else
		{
			m_lvEnum.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}
BOOL CDlgEnumList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_lvEnum.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lvEnum.InsertColumn(0,_T("����"),LVCFMT_LEFT,200);
	m_lvEnum.InsertColumn(1,_T("����"),LVCFMT_LEFT,200);

	FillList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgEnumList::OnBnClickedButtonNew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgEnumEditor Dlg;

	if(Dlg.DoModal()==IDOK)
	{
		m_EnumDefineList.push_back(Dlg.m_EnumDefineInfo);
		FillList();
	}
}

void CDlgEnumList::OnBnClickedButtonEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvEnum.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvEnum.GetNextSelectedItem(Pos);
		size_t Index=m_lvEnum.GetItemData(Item);
		if(Index<m_EnumDefineList.size())
		{
			CDlgEnumEditor Dlg;

			Dlg.m_EnumDefineInfo=m_EnumDefineList[Index];
			if(Dlg.DoModal()==IDOK)
			{
				m_EnumDefineList[Index]=Dlg.m_EnumDefineInfo;
				FillList();
			}
		}
	}
}

void CDlgEnumList::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvEnum.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvEnum.GetNextSelectedItem(Pos);
		size_t Index=m_lvEnum.GetItemData(Item);
		CString Msg;
		Msg.Format(_T("�Ƿ�Ҫɾ��ö��[%s]"),
			m_lvEnum.GetItemText(Item,0));
		if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
		{
			m_EnumDefineList.erase(m_EnumDefineList.begin()+Index);
			FillList();
		}
	}
}

void CDlgEnumList::OnNMDblclkListStruct(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	OnBnClickedButtonEdit();
}

void CDlgEnumList::OnBnClickedButtonMoveUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvEnum.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvEnum.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvEnum.GetItemData(Item);
		if(Index>0&&Index<m_EnumDefineList.size())
		{
			ENUM_DEFINE_INFO Temp=m_EnumDefineList[Index];
			m_EnumDefineList.erase(m_EnumDefineList.begin()+Index);
			m_EnumDefineList.insert(m_EnumDefineList.begin()+Index-1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}

void CDlgEnumList::OnBnClickedButtonOveDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvEnum.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvEnum.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvEnum.GetItemData(Item);
		if(Index<m_EnumDefineList.size()-1)
		{
			ENUM_DEFINE_INFO Temp=m_EnumDefineList[Index];
			m_EnumDefineList.erase(m_EnumDefineList.begin()+Index);
			m_EnumDefineList.insert(m_EnumDefineList.begin()+Index+1,Temp);
			FillList();
			SelectItemByName(Temp.Name);
		}
	}
}


void CDlgEnumList::OnBnClickedButtonClone()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvEnum.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvEnum.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvEnum.GetItemData(Item);
		if(Index>0&&Index<m_EnumDefineList.size())
		{
			m_EnumDefineList.push_back(m_EnumDefineList[Index]);
			FillList();
		}
	}
}

void CDlgEnumList::OnBnClickedButtonMove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vector<UINT> MoveList;

	int Item = 0;
	POSITION Pos=m_lvEnum.GetFirstSelectedItemPosition();
	while(Pos)
	{
		Item=m_lvEnum.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvEnum.GetItemData(Item);
		if(Index<m_EnumDefineList.size())
		{
			MoveList.push_back(Index);		
		}
	}

	if(MoveList.size())
	{
		vector<CString> StructPacketList;
		GetMainDlg()->GetDataStructPacketList(DATA_STRUCT_ENUM,StructPacketList);
		CDlgListSelector Dlg;
		Dlg.Init(_T("�ƶ���..."),StructPacketList);
		if(Dlg.DoModal()==IDOK)
		{
			for(size_t i=0;i<MoveList.size();i++)
			{
				UINT Index=MoveList[i];			

				if(!GetMainDlg()->MoveDataStruct(DATA_STRUCT_ENUM,m_ListName,Dlg.m_SelectedItem,m_EnumDefineList[Index].Name))				
				{
					AfxMessageBox(_T("ת��ʧ��"));
					return;
				}
			}
			for(int i=(int)MoveList.size()-1;i>=0;i--)
			{
				UINT Index=MoveList[i];			
				m_EnumDefineList.erase(m_EnumDefineList.begin()+Index);					
			}
		}
		FillList();
		if (Item >= m_lvEnum.GetItemCount())
			Item = m_lvEnum.GetItemCount() - 1;
		m_lvEnum.EnsureVisible(Item, true);
	}
}


void CDlgEnumList::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}


void CDlgEnumList::OnBnClickedButtonSort()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	qsort(m_EnumDefineList.begin()._Ptr, m_EnumDefineList.size(), sizeof(ENUM_DEFINE_INFO), CCallInterfaceMakerDlg::EnumComp);
	FillList();
}
