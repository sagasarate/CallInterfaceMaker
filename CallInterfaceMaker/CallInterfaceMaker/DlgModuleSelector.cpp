// DlgModuleSelector.cpp : ʵ���ļ�
//

#include "stdafx.h"



// CDlgModuleSelector �Ի���

IMPLEMENT_DYNAMIC(CDlgModuleSelector, CDialog)

CDlgModuleSelector::CDlgModuleSelector(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModuleSelector::IDD, pParent)
{

}

CDlgModuleSelector::~CDlgModuleSelector()
{
}

void CDlgModuleSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODULE, m_lvList);
}


BEGIN_MESSAGE_MAP(CDlgModuleSelector, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgModuleSelector::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgModuleSelector ��Ϣ�������


BOOL CDlgModuleSelector::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lvList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 60);
	m_lvList.InsertColumn(1, _T("ģ��"), LVCFMT_LEFT, 100);

	FillList();

	for (int i = 0; i < m_lvList.GetItemCount(); i++)
	{
		if (m_lvList.GetItemData(i) == GetMainDlg()->GetMainModuleID())
		{
			m_lvList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgModuleSelector::FillList()
{
	m_lvList.DeleteAllItems();
	CString Temp;
	vector<MODULE_DEFINE_INFO *>& ModuleList = GetMainDlg()->GetModuleList();
	for (size_t i = 0; i < ModuleList.size(); i++)
	{
		MODULE_DEFINE_INFO * pModuleInfo = ModuleList[i];
		Temp.Format("%d", pModuleInfo->ModuleID);
		int Item = m_lvList.InsertItem(i, Temp);
		m_lvList.SetItemText(Item, 1, pModuleInfo->Name);
		m_lvList.SetItemData(Item, pModuleInfo->ModuleID);
	}
}

void CDlgModuleSelector::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos = m_lvList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvList.GetNextSelectedItem(Pos);
		m_SelectedModuleID = m_lvList.GetItemData(Item);
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox("��ѡ��һ��ģ��");
	}
	
}
