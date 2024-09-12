// DlgImportEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgImportEditor.h"



// CDlgImportEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgImportEditor, CDialog)

CDlgImportEditor::CDlgImportEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImportEditor::IDD, pParent)
{

}

CDlgImportEditor::~CDlgImportEditor()
{
}

void CDlgImportEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODULE, m_lvList);
}


BEGIN_MESSAGE_MAP(CDlgImportEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgImportEditor::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgImportEditor::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CDlgImportEditor ��Ϣ�������


BOOL CDlgImportEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lvList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 60);
	m_lvList.InsertColumn(1, _T("ģ��"), LVCFMT_LEFT, 100);
	m_lvList.InsertColumn(2, _T("·��"), LVCFMT_LEFT, 200);

	FillList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgImportEditor::FillList()
{
	m_lvList.DeleteAllItems();
	CString Temp;
	vector<MODULE_DEFINE_INFO>& ModuleList = GetMainDlg()->GetImportModuleList();
	for (size_t i = 0; i < ModuleList.size(); i++)
	{
		MODULE_DEFINE_INFO& ModuleInfo = ModuleList[i];
		Temp.Format(_T("%d"), ModuleInfo.ModuleID);
		int Item = m_lvList.InsertItem(i, Temp);
		m_lvList.SetItemText(Item, 1, ModuleInfo.Name);
		m_lvList.SetItemText(Item, 2, ModuleInfo.ModuleDefineFileName);
		m_lvList.SetItemData(Item, ModuleInfo.ModuleID);
	}
}

void CDlgImportEditor::OnBnClickedButtonAdd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog Dlg(true, _T("*.xml"), _T("*.xml"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("XML Files(*.xml)|*,xml|All Files(*.*)|*.*||"));

	//if (!m_RunEnvInfo.RecentDataDir.IsEmpty())
	//{
	//	Dlg.m_ofn.lpstrInitialDir = m_RunEnvInfo.RecentDataDir;
	//}

	if (Dlg.DoModal() == IDOK)
	{
		GetMainDlg()->AddModule(Dlg.GetPathName());
		FillList();
	}
}


void CDlgImportEditor::OnBnClickedButtonDel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos = m_lvList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvList.GetNextSelectedItem(Pos);
		UINT ModuleID = m_lvList.GetItemData(Item);
		MODULE_DEFINE_INFO * pModule=GetMainDlg()->GetModuleInfo(ModuleID);
		if (pModule)
		{
			if (AfxMessageBox(_T("�Ƿ�Ҫɾ������ģ�飺") + pModule->Name, MB_YESNO) == IDYES)
			{
				GetMainDlg()->DeleteModule(ModuleID);
				FillList();
			}
		}
	}
}
