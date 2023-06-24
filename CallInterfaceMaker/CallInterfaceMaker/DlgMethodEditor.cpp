/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DlgMethodEditor.cpp                                      */
/*      ��������:  2012��04��22��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// DlgMethodEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CallInterfaceMaker.h"
#include "DlgMethodEditor.h"


// CDlgMethodEditor �Ի���

IMPLEMENT_DYNAMIC(CDlgMethodEditor, CDialog)

CDlgMethodEditor::CDlgMethodEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMethodEditor::IDD, pParent)
{
	m_CurTabType = -1;
}

CDlgMethodEditor::~CDlgMethodEditor()
{
}

void CDlgMethodEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PARAM, m_lvParamList);
	DDX_Control(pDX, IDC_COMBO_METHOD_TYPE, m_cbType);
	DDX_Control(pDX, IDC_TAB_PARAM_LIST, m_tabParamList);

	DDX_Text(pDX, IDC_EDIT_NAME, m_MethodInfo.Name);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_MethodInfo.Description);
	DDX_Text(pDX, IDC_EDIT_METHOD_ID, m_MethodInfo.ID);
	BOOL CanCache = (m_MethodInfo.Flag&INTERFACE_METHOD_FLAG_CAN_CACHE) ? TRUE : FALSE;
	BOOL NoCompress = (m_MethodInfo.Flag&INTERFACE_METHOD_FLAG_NO_COMPRESS) ? TRUE : FALSE;
	DDX_Check(pDX, IDC_CHECK_CAN_CACHE, CanCache);
	DDX_Check(pDX, IDC_CHECK_NO_COMPRESS, NoCompress);
	m_MethodInfo.Flag = (CanCache ? INTERFACE_METHOD_FLAG_CAN_CACHE : 0)|
		(NoCompress ? INTERFACE_METHOD_FLAG_NO_COMPRESS : 0);

	if (pDX->m_bSaveAndValidate)
	{
		m_MethodInfo.Type = m_cbType.GetCurSel();		
	}
	else
	{
		m_cbType.SetCurSel(m_MethodInfo.Type);
	}
	
}


BEGIN_MESSAGE_MAP(CDlgMethodEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PARAM, &CDlgMethodEditor::OnBnClickedButtonAddParam)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PARAM, &CDlgMethodEditor::OnBnClickedButtonEditParam)
	ON_BN_CLICKED(IDC_BUTTON_DEL_PARAM, &CDlgMethodEditor::OnBnClickedButtonDelParam)
	ON_BN_CLICKED(IDC_BUTTON_PARAM_UP, &CDlgMethodEditor::OnBnClickedButtonParamUp)
	ON_BN_CLICKED(IDC_BUTTON_PARAM_DOWN, &CDlgMethodEditor::OnBnClickedButtonParamDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PARAM, &CDlgMethodEditor::OnNMDblclkListParam)
	ON_CBN_SELCHANGE(IDC_COMBO_METHOD_TYPE, &CDlgMethodEditor::OnCbnSelchangeComboMethodType)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PARAM_LIST, &CDlgMethodEditor::OnTcnSelchangeTabParamList)
END_MESSAGE_MAP()


// CDlgMethodEditor ��Ϣ�������

BOOL CDlgMethodEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lvParamList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	m_lvParamList.InsertColumn(0,"����",LVCFMT_LEFT,150);
	m_lvParamList.InsertColumn(1,"ID",LVCFMT_LEFT,50);
	m_lvParamList.InsertColumn(2,"����",LVCFMT_LEFT,60);	
	m_lvParamList.InsertColumn(3,"����",LVCFMT_LEFT,50);
	m_lvParamList.InsertColumn(4,"ȱʡֵ",LVCFMT_LEFT,50);
	m_lvParamList.InsertColumn(5,"��Ϊ��",LVCFMT_LEFT,60);
	m_lvParamList.InsertColumn(6,"����",LVCFMT_LEFT,200);

	m_cbType.ResetContent();
	for (int i = 0; i < INTERFACE_METHOD_TYPE_MAX; i++)
	{
		m_cbType.AddString(g_szINTERFACE_METHOD_TYPE[i]);
	}

	UpdateData(false);
	m_CurTabType = -1;
	InitTab();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgMethodEditor::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(true);
	CDialog::OnOK();
}

void CDlgMethodEditor::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnCancel();
}

void CDlgMethodEditor::OnBnClickedButtonAddParam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgParamEditor Dlg;

	
	Dlg.SetID(m_MethodInfo.ParamIDSeed);
	
	if(Dlg.DoModal()==IDOK)
	{
		m_MethodInfo.ParamIDSeed++;
		METHOD_PARAM ParamInfo;
		Dlg.GetData(ParamInfo);

		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			m_MethodInfo.CallParamList.push_back(ParamInfo);
			AddListItem(ParamInfo, m_MethodInfo.CallParamList.size() - 1);
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			m_MethodInfo.AckParamList.push_back(ParamInfo);
			AddListItem(ParamInfo, m_MethodInfo.AckParamList.size() - 1);
			break;
		}
		
	}
}

void CDlgMethodEditor::OnBnClickedButtonEditParam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			if (Index < m_MethodInfo.CallParamList.size())
			{
				CDlgParamEditor Dlg;

				Dlg.SetData(m_MethodInfo.CallParamList[Index]);

				if (Dlg.DoModal() == IDOK)
				{
					Dlg.GetData(m_MethodInfo.CallParamList[Index]);
					EditListItem(Item, m_MethodInfo.CallParamList[Index]);
				}
			}
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			if (Index < m_MethodInfo.AckParamList.size())
			{
				CDlgParamEditor Dlg;

				Dlg.SetData(m_MethodInfo.AckParamList[Index]);

				if (Dlg.DoModal() == IDOK)
				{
					Dlg.GetData(m_MethodInfo.AckParamList[Index]);
					EditListItem(Item, m_MethodInfo.AckParamList[Index]);
				}
			}
			break;
		}
		
	}
	else
	{
		MessageBox("��ѡ��һ������");
	}
}

void CDlgMethodEditor::OnBnClickedButtonDelParam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			if (Index < m_MethodInfo.CallParamList.size())
			{
				CString Msg;
				Msg.Format("�Ƿ�Ҫɾ������[%s]",
					m_MethodInfo.CallParamList[Index].Name);
				if (AfxMessageBox(Msg, MB_YESNO) == IDYES)
				{
					m_MethodInfo.CallParamList.erase(m_MethodInfo.CallParamList.begin() + Index);
					FillListItem();
				}
			}
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			if (Index < m_MethodInfo.AckParamList.size())
			{
				CString Msg;
				Msg.Format("�Ƿ�Ҫɾ������[%s]",
					m_MethodInfo.AckParamList[Index].Name);
				if (AfxMessageBox(Msg, MB_YESNO) == IDYES)
				{
					m_MethodInfo.AckParamList.erase(m_MethodInfo.AckParamList.begin() + Index);
					FillListItem();
				}
			}
			break;
		}
		
	}
}
void CDlgMethodEditor::OnBnClickedButtonParamUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			if (Index > 0 && Index < m_MethodInfo.CallParamList.size())
			{
				METHOD_PARAM Temp = m_MethodInfo.CallParamList[Index];
				m_MethodInfo.CallParamList.erase(m_MethodInfo.CallParamList.begin() + Index);
				m_MethodInfo.CallParamList.insert(m_MethodInfo.CallParamList.begin() + Index - 1, Temp);
				FillListItem();
				SelectItemByName(Temp.Name);
			}
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			if (Index > 0 && Index < m_MethodInfo.AckParamList.size())
			{
				METHOD_PARAM Temp = m_MethodInfo.AckParamList[Index];
				m_MethodInfo.AckParamList.erase(m_MethodInfo.AckParamList.begin() + Index);
				m_MethodInfo.AckParamList.insert(m_MethodInfo.AckParamList.begin() + Index - 1, Temp);
				FillListItem();
				SelectItemByName(Temp.Name);
			}
			break;
		}
		
	}
}

void CDlgMethodEditor::OnBnClickedButtonParamDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos=m_lvParamList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvParamList.GetNextSelectedItem(Pos);
		UINT Index=(UINT)m_lvParamList.GetItemData(Item);
		switch (m_tabParamList.GetCurSel())
		{
		case INTERFACE_METHOD_PARAM_TYPE_CALL:
			if (Index < m_MethodInfo.CallParamList.size() - 1)
			{
				METHOD_PARAM Temp = m_MethodInfo.CallParamList[Index];
				m_MethodInfo.CallParamList.erase(m_MethodInfo.CallParamList.begin() + Index);
				m_MethodInfo.CallParamList.insert(m_MethodInfo.CallParamList.begin() + Index + 1, Temp);
				FillListItem();
				SelectItemByName(Temp.Name);
			}
			break;
		case INTERFACE_METHOD_PARAM_TYPE_ACK:
			if (Index < m_MethodInfo.AckParamList.size() - 1)
			{
				METHOD_PARAM Temp = m_MethodInfo.AckParamList[Index];
				m_MethodInfo.AckParamList.erase(m_MethodInfo.AckParamList.begin() + Index);
				m_MethodInfo.AckParamList.insert(m_MethodInfo.AckParamList.begin() + Index + 1, Temp);
				FillListItem();
				SelectItemByName(Temp.Name);
			}
			break;
		}		
	}
}

void CDlgMethodEditor::AddListItem(METHOD_PARAM& ParamInfo,UINT Index)
{
	CString Temp;
	int Item=m_lvParamList.InsertItem(m_lvParamList.GetItemCount(),ParamInfo.Name);
	m_lvParamList.SetItemData(Item,(INT_PTR)Index);

	Temp.Format("%u",ParamInfo.ID);
	m_lvParamList.SetItemText(Item,1,Temp);

	TYPE_DEFINE * pTypeInfo=GetMainDlg()->FindVarType(ParamInfo.Type);
	if(pTypeInfo)
	{
		CString Temp = pTypeInfo->Name;
		if(ParamInfo.IsReference)		
		{
			Temp=pTypeInfo->GenerateOperations.ReferenceDefine;
			Temp.Replace("<Type>",pTypeInfo->Name);
			m_lvParamList.SetItemText(Item,2,Temp);
		}
		
		m_lvParamList.SetItemText(Item,2,Temp);
	}
	else
	{
		m_lvParamList.SetItemText(Item,2,"δ֪");
	}
	Temp.Format("%d",ParamInfo.Length);
	m_lvParamList.SetItemText(Item,3,Temp);
	m_lvParamList.SetItemText(Item,4,ParamInfo.DefaultValue);
	if(ParamInfo.CanNull)
	{
		m_lvParamList.SetItemText(Item,5,"��");
	}
	else
	{
		m_lvParamList.SetItemText(Item,5,"��");
	}
	m_lvParamList.SetItemText(Item,6,ParamInfo.Description);
}

void CDlgMethodEditor::EditListItem(int Item,METHOD_PARAM& ParamInfo)
{
	CString Temp;
	m_lvParamList.SetItemText(Item,0,ParamInfo.Name);
	Temp.Format("%u",ParamInfo.ID);
	m_lvParamList.SetItemText(Item,1,Temp);

	TYPE_DEFINE * pTypeInfo=GetMainDlg()->FindVarType(ParamInfo.Type);
	if(pTypeInfo)
	{
		if(ParamInfo.IsReference)		
		{
			CString Temp=pTypeInfo->GenerateOperations.ReferenceDefine;
			Temp.Replace("<Type>",pTypeInfo->Name);
			m_lvParamList.SetItemText(Item,2,Temp);
		}
		else
			m_lvParamList.SetItemText(Item,2,pTypeInfo->Name);
	}
	else
	{
		m_lvParamList.SetItemText(Item,2,"δ֪");
	}
	Temp.Format("%d",ParamInfo.Length);
	m_lvParamList.SetItemText(Item,3,Temp);
	m_lvParamList.SetItemText(Item,4,ParamInfo.DefaultValue);
	if(ParamInfo.CanNull)
	{
		m_lvParamList.SetItemText(Item,5,"��Ϊ��");
	}
	else
	{
		m_lvParamList.SetItemText(Item,5,"����Ϊ��");
	}
	m_lvParamList.SetItemText(Item,6,ParamInfo.Description);
}

void CDlgMethodEditor::FillListItem()
{
	m_lvParamList.DeleteAllItems();
	switch (m_tabParamList.GetCurSel())
	{
	case INTERFACE_METHOD_PARAM_TYPE_CALL:
		for (UINT i = 0; i < m_MethodInfo.CallParamList.size(); i++)
		{
			AddListItem(m_MethodInfo.CallParamList[i], i);
		}
		break;
	case INTERFACE_METHOD_PARAM_TYPE_ACK:
		for (UINT i = 0; i < m_MethodInfo.AckParamList.size(); i++)
		{
			AddListItem(m_MethodInfo.AckParamList[i], i);
		}
		break;
	}
}

void CDlgMethodEditor::SelectItemByName(LPCTSTR szName)
{
	for(int i=0;i<m_lvParamList.GetItemCount();i++)
	{
		if(m_lvParamList.GetItemText(i,0)==szName)
		{
			m_lvParamList.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			m_lvParamList.EnsureVisible(i,false);
		}
		else
		{
			m_lvParamList.SetItemState(i,0,LVIS_SELECTED);
		}
	}
}

void CDlgMethodEditor::InitTab()
{	
	if (m_CurTabType != m_cbType.GetCurSel())
	{
		m_CurTabType = m_cbType.GetCurSel();
		m_tabParamList.DeleteAllItems();
		switch (m_CurTabType)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			m_tabParamList.InsertItem(0, "���ò���");
			m_tabParamList.InsertItem(1, "�ص�����");
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			m_tabParamList.InsertItem(0, "֪ͨ����");
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			m_tabParamList.InsertItem(0, "���ò���");
		}

		m_tabParamList.SetCurSel(0);

		FillListItem();
	}
}


void CDlgMethodEditor::OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	OnBnClickedButtonEditParam();
}


void CDlgMethodEditor::OnCbnSelchangeComboMethodType()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	InitTab();
}


void CDlgMethodEditor::OnTcnSelchangeTabParamList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	FillListItem();
}
