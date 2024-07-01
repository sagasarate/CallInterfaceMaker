// DlgGenerateOperationsEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgGenerateOperationsEditor.h"




// CDlgGenerateOperationsEditor 对话框

IMPLEMENT_DYNAMIC(CDlgGenerateOperationsEditor, CDialog)

CDlgGenerateOperationsEditor::CDlgGenerateOperationsEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGenerateOperationsEditor::IDD, pParent)
{
	m_IsModified=false;
	m_ShowIndexOperation=false;
}

CDlgGenerateOperationsEditor::~CDlgGenerateOperationsEditor()
{
}

void CDlgGenerateOperationsEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_OPERATION, m_edOperation);
	DDX_Control(pDX, IDC_TAB, m_tbOperations);
}


BEGIN_MESSAGE_MAP(CDlgGenerateOperationsEditor, CDialog)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, &CDlgGenerateOperationsEditor::OnTcnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgGenerateOperationsEditor::OnTcnSelchangeTab)	
	ON_EN_CHANGE(IDC_EDIT_OPERATION, &CDlgGenerateOperationsEditor::OnEnChangeEdit)
END_MESSAGE_MAP()


// CDlgGenerateOperationsEditor 消息处理程序

BOOL CDlgGenerateOperationsEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	TabInitOperations(m_tbOperations);

	m_tbOperations.SetCurSel(0);

	ShowItemData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgGenerateOperationsEditor::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_IsModified)
	{
		if(AfxMessageBox(_T("是否要放弃已作出的更改？"),MB_YESNO)==IDYES)
		{
			CDialog::OnCancel();
		}
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CDlgGenerateOperationsEditor::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	if(m_IsModified)
		FetchItemData();

	CDialog::OnOK();
}

void CDlgGenerateOperationsEditor::OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	FetchItemData();
}
void CDlgGenerateOperationsEditor::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	ShowItemData();
}

void CDlgGenerateOperationsEditor::OnEnChangeEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_IsModified=true;
}

void CDlgGenerateOperationsEditor::ShowItemData()
{
	TabShowOperations(m_tbOperations, m_edOperation, m_GenerateOperations);	
	
	UpdateData(false);
}
void CDlgGenerateOperationsEditor::FetchItemData()
{	
	UpdateData(true);
	
	TabFetchOperations(m_tbOperations, m_edOperation, m_GenerateOperations);
}

