// DlgGenerateOperationsEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgGenerateOperationsEditor.h"




// CDlgGenerateOperationsEditor �Ի���

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


// CDlgGenerateOperationsEditor ��Ϣ�������

BOOL CDlgGenerateOperationsEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	TabInitOperations(m_tbOperations);

	m_tbOperations.SetCurSel(0);

	ShowItemData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgGenerateOperationsEditor::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(m_IsModified)
	{
		if(AfxMessageBox(_T("�Ƿ�Ҫ�����������ĸ��ģ�"),MB_YESNO)==IDYES)
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
	// TODO: �ڴ����ר�ô����/����û���

	if(m_IsModified)
		FetchItemData();

	CDialog::OnOK();
}

void CDlgGenerateOperationsEditor::OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	FetchItemData();
}
void CDlgGenerateOperationsEditor::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	ShowItemData();
}

void CDlgGenerateOperationsEditor::OnEnChangeEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

