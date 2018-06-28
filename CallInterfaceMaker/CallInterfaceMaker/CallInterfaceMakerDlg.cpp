/****************************************************************************/
/*                                                                          */
/*      文件名:    CallInterfaceMakerDlg.cpp                                */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// CallInterfaceMakerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CallInterfaceMakerDlg.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCallInterfaceMakerDlg 对话框




CCallInterfaceMakerDlg::CCallInterfaceMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCallInterfaceMakerDlg::IDD, pParent)
	, m_UTF8Export(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_SelectAll=true;
	m_MainModule.IDSeed=1;
	m_IsLoaded = false;
}

CCallInterfaceMakerDlg::~CCallInterfaceMakerDlg()
{
	m_MainModule.Clear();
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		m_ImportModuleList[i].Clear();
	}
	m_ImportModuleList.clear();
}

void CCallInterfaceMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INTERFACE_LIST, m_lvInterfaceList);
	DDX_Text(pDX, IDC_EDIT_NAME, m_MainModule.Name);
	DDX_Text(pDX, IDC_EDIT_MODULE_ID, m_MainModule.ModuleID);
	DDX_Check(pDX, IDC_UTF8_EXPORT, m_UTF8Export);
}

BEGIN_MESSAGE_MAP(CCallInterfaceMakerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ADD_INTERFACE, &CCallInterfaceMakerDlg::OnBnClickedButtonAddInterface)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_INTERFACE, &CCallInterfaceMakerDlg::OnBnClickedButtonEditInterface)
	ON_BN_CLICKED(IDC_BUTTON_DEL_INTERFACE, &CCallInterfaceMakerDlg::OnBnClickedButtonDelInterface)
	//ON_BN_CLICKED(IDC_BUTTON_EDIT_PREFIX, &CCallInterfaceMakerDlg::OnBnClickedButtonEditPrefix)
	//ON_BN_CLICKED(IDC_BUTTON_EDIT_POSTFIX, &CCallInterfaceMakerDlg::OnBnClickedButtonEditPostfix)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCallInterfaceMakerDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_AS, &CCallInterfaceMakerDlg::OnBnClickedButtonSaveAs)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CCallInterfaceMakerDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_EXPORT, &CCallInterfaceMakerDlg::OnBnClickedButtonSaveExport)
	ON_NOTIFY(NM_DBLCLK, IDC_INTERFACE_LIST, &CCallInterfaceMakerDlg::OnNMDblclkInterfaceList)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CCallInterfaceMakerDlg::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL_INTERFACE, &CCallInterfaceMakerDlg::OnBnClickedButtonSelectAllInterface)
	ON_BN_CLICKED(IDC_BUTTON_DATA_STRUCT_DEF, &CCallInterfaceMakerDlg::OnBnClickedButtonDataStructDef)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_INTERFACE_LIST, &CCallInterfaceMakerDlg::OnLvnItemchangedInterfaceList)
//ON_NOTIFY(HDN_ITEMCLICK, 0, &CCallInterfaceMakerDlg::OnHdnItemclickInterfaceList)
//ON_NOTIFY(NM_CLICK, IDC_INTERFACE_LIST, &CCallInterfaceMakerDlg::OnNMClickInterfaceList)
ON_BN_CLICKED(IDC_BUTTON_ARRANGE_IDS, &CCallInterfaceMakerDlg::OnBnClickedButtonArrangeIds)
ON_BN_CLICKED(IDC_BUTTON_SORT, &CCallInterfaceMakerDlg::OnBnClickedButtonSort)
ON_BN_CLICKED(IDC_BUTTON_SORT_ALL, &CCallInterfaceMakerDlg::OnBnClickedButtonSortAll)
ON_BN_CLICKED(IDC_BUTTON_IMPORT_EDIT, &CCallInterfaceMakerDlg::OnBnClickedButtonImportEdit)
ON_BN_CLICKED(IDC_BUTTON_MOVE_INTERFACE, &CCallInterfaceMakerDlg::OnBnClickedButtonMoveInterface)
END_MESSAGE_MAP()


// CCallInterfaceMakerDlg 消息处理程序

BOOL CCallInterfaceMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_TypeDefFileName=CFileTools::MakeModuleFullPath(NULL,TYPE_DEF_FILE_NAME);
	
	LoadTypeDef();
	LoadTemples();
	LoadConfig();
	LoadEnv();

	MakeCurVarType(true);

	m_lvInterfaceList.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	m_lvInterfaceList.InsertColumn(0,"名称",LVCFMT_LEFT,200);
	m_lvInterfaceList.InsertColumn(1, "模块", LVCFMT_LEFT, 100);
	m_lvInterfaceList.InsertColumn(2,"方法数量",LVCFMT_LEFT,60);
	m_lvInterfaceList.InsertColumn(3,"ID",LVCFMT_LEFT,60);
	m_lvInterfaceList.InsertColumn(4,"描述",LVCFMT_LEFT,200);




	FillListItem();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCallInterfaceMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCallInterfaceMakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCallInterfaceMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCallInterfaceMakerDlg::OnBnClickedButtonAddInterface()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgInterfaceEdtor Dlg;
		
	if(Dlg.DoModal()==IDOK)
	{
		CALLER_INTERFACE InterfaceInfo;
		Dlg.GetData(InterfaceInfo);
		AddInterfaceInfo(InterfaceInfo);
		FillListItem();
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonEditInterface()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvInterfaceList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvInterfaceList.GetNextSelectedItem(Pos);
		UINT Param=(UINT)m_lvInterfaceList.GetItemData(Item);
		UINT ModuleID = (Param >> 8) & 0x7F;
		UINT InterfaceID = Param & 0xFF;
		CALLER_INTERFACE * pInterfaceInfo = GetInterfaceInfoByID(ModuleID, InterfaceID);
		if (pInterfaceInfo)
		{
			CDlgInterfaceEdtor Dlg;
			Dlg.SetData(*pInterfaceInfo);

			if(Dlg.DoModal()==IDOK)
			{
				Dlg.GetData(*pInterfaceInfo);
				EditListItem(Item, *pInterfaceInfo);
			}
		}
	}
	else
	{
		MessageBox("请选择一个接口");
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonDelInterface()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvInterfaceList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvInterfaceList.GetNextSelectedItem(Pos);
		UINT Param = (UINT)m_lvInterfaceList.GetItemData(Item);
		UINT ModuleID = (Param >> 8) & 0x7F;
		UINT InterfaceID = Param & 0xFF;
		CALLER_INTERFACE * pInterfaceInfo = GetInterfaceInfoByID(ModuleID, InterfaceID);
		if (pInterfaceInfo)
		{
			CString Msg;
			Msg.Format("是否要删除接口[%s]",pInterfaceInfo->Name);
			if(AfxMessageBox(Msg,MB_YESNO)==IDYES)
			{
				if (DeleteInterfaceInfoByID(ModuleID, InterfaceID))
				{
					m_lvInterfaceList.DeleteItem(Item);
				}
			}
			
		}
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonMoveInterface()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgModuleSelector Dlg;
	if (Dlg.DoModal() == IDOK)
	{
		SaveInterfaceSelections();
		MODULE_DEFINE_INFO * pNewModule = GetModuleInfo(Dlg.m_SelectedModuleID);
		if (pNewModule)
		{
			POSITION Pos = m_lvInterfaceList.GetFirstSelectedItemPosition();
			while (Pos)
			{
				int Item = m_lvInterfaceList.GetNextSelectedItem(Pos);
				UINT Param = (UINT)m_lvInterfaceList.GetItemData(Item);
				UINT ModuleID = (Param >> 8) & 0x7F;
				UINT InterfaceID = Param & 0xFF;
				MODULE_DEFINE_INFO * pOldModule = GetModuleInfo(ModuleID);
				if (pOldModule)
				{
					for (size_t i = 0; i < pOldModule->InterfaceList.size(); i++)
					{
						if (pOldModule->InterfaceList[i].ID == InterfaceID)
						{
							pOldModule->InterfaceList[i].ID = pNewModule->IDSeed;
							pOldModule->InterfaceList[i].ModuleID = pNewModule->ModuleID;
							pNewModule->IDSeed++;
							pNewModule->InterfaceList.push_back(pOldModule->InterfaceList[i]);
							pOldModule->InterfaceList.erase(pOldModule->InterfaceList.begin() + i);
							break;
						}
					}
				}
			}
			RefreshInterfaceCache();
			FillListItem();
			RestoreInterfaceSelections();
		}
	}
}

//void CCallInterfaceMakerDlg::OnBnClickedButtonEditPrefix()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CDlgTextEditor Dlg;
//
//	Dlg.SetText(m_InterfaceDefine.InterfacePrefix);
//	if(Dlg.DoModal()==IDOK)
//	{
//		m_InterfaceDefine.InterfacePrefix = Dlg.GetText();
//	}
//}
//
//void CCallInterfaceMakerDlg::OnBnClickedButtonEditPostfix()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CDlgTextEditor Dlg;
//
//	Dlg.SetText(m_InterfaceDefine.InterfacePostFix);
//	if(Dlg.DoModal()==IDOK)
//	{
//		m_InterfaceDefine.InterfacePostFix = Dlg.GetText();
//	}
//}


void CCallInterfaceMakerDlg::AddListItem(CALLER_INTERFACE& InterfaceInfo)
{
	CString Temp;
	int Item=m_lvInterfaceList.InsertItem(m_lvInterfaceList.GetItemCount(),InterfaceInfo.Name);
	m_lvInterfaceList.SetItemData(Item, (INT_PTR)(((InterfaceInfo.ModuleID & 0x7F) << 8) | (InterfaceInfo.ID & 0xFF)));
	Temp.Format("%d",InterfaceInfo.MethodList.size());
	m_lvInterfaceList.SetItemText(Item, 1, GetModuleName(InterfaceInfo.ModuleID));
	m_lvInterfaceList.SetItemText(Item,2,Temp);
	Temp.Format("%d",InterfaceInfo.ID);
	m_lvInterfaceList.SetItemText(Item,3,Temp);
	m_lvInterfaceList.SetItemText(Item,4,InterfaceInfo.Description);
}
void CCallInterfaceMakerDlg::EditListItem(int Item,CALLER_INTERFACE& InterfaceInfo)
{
	CString Temp;
	m_lvInterfaceList.SetItemText(Item,0,InterfaceInfo.Name);
	Temp.Format("%d",InterfaceInfo.MethodList.size());
	m_lvInterfaceList.SetItemText(Item, 1, GetModuleName(InterfaceInfo.ModuleID));
	m_lvInterfaceList.SetItemText(Item,2,Temp);		
	Temp.Format("%d",InterfaceInfo.ID);
	m_lvInterfaceList.SetItemText(Item,3,Temp);
	m_lvInterfaceList.SetItemText(Item,4,InterfaceInfo.Description);
}
bool CCallInterfaceMakerDlg::IsListItemChecked(UINT ModuleID, UINT InterfaceID)
{
	for(int i=0;i<m_lvInterfaceList.GetItemCount();i++)
	{
		if(m_lvInterfaceList.GetCheck(i))
		{
			INT_PTR ItemData = (INT_PTR)(((ModuleID & 0x7F) << 8) | (InterfaceID & 0xFF));
			if ((UINT)m_lvInterfaceList.GetItemData(i) == ItemData)
				return true;
		}
	}
	return false;
}
void CCallInterfaceMakerDlg::FillListItem()
{
	m_lvInterfaceList.DeleteAllItems();
	for (UINT i = 0; i<m_AllInterfaceList.size(); i++)
	{
		AddListItem(*m_AllInterfaceList[i]);
	}
}

bool CCallInterfaceMakerDlg::LoadTypeDef()
{
	xml_parser Parser;

	if(Parser.parse_file(m_TypeDefFileName,pug::parse_trim_attribute))
	{
		xml_node TypeDef=Parser.document();
		if(TypeDef.moveto_child("TypeDef"))
		{
			for(UINT i=0;i<TypeDef.children();i++)
			{
				xml_node Type=TypeDef.child(i);
				if(_stricmp(Type.name(),"Type")==0)
				{
					TYPE_DEFINE TypeInfo;
					TypeInfo.Name=((LPCTSTR)Type.attribute("Name").getvalue());
					TypeInfo.CType = ((LPCTSTR)Type.attribute("CType").getvalue());
					TypeInfo.Flag = Type.attribute("Flag");
					LoadGenerateOperations(Type,TypeInfo.GenerateOperations);
					m_BaseTypeList.push_back(TypeInfo);
				}
			}
		}
		return true;
	}
	return false;
}
bool CCallInterfaceMakerDlg::SaveTypeDef()
{
	std::ofstream os;	
	os.open(m_TypeDefFileName, std::ios::trunc);	
	if (os.is_open())
	{
		pug::xml_parser Xml;

		Xml.create();
		xml_node Doc;
		Doc = Xml.document();
		xml_node pi = Doc.append_child(node_pi);
		pi.name(_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");
		pi.attribute(_T("encoding")) = _T("gb2312");
		xml_node TypeDef=Doc.append_child(node_element,"TypeDef");
		
		for(size_t i=0;i<m_BaseTypeList.size();i++)
		{			
			xml_node Type=TypeDef.append_child(node_element,"Type");

			Type.append_attribute("Name",(LPCTSTR)m_BaseTypeList[i].Name);			
			Type.append_attribute("CType",(LPCTSTR)m_BaseTypeList[i].CType);
			Type.append_attribute("Flag", m_BaseTypeList[i].Flag);
			SaveGenerateOperations(Type,m_BaseTypeList[i].GenerateOperations);
		}
		
		os << Xml.document();
		os.close();
		return true;
	}
	else
	{		
		return false;
	}
}
bool CCallInterfaceMakerDlg::LoadTemples()
{
	CFile TempleFile;

	CString FileName;

	FileName=CFileTools::MakeModuleFullPath(NULL,INTERFACE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_InterfaceHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}	

	FileName=CFileTools::MakeModuleFullPath(NULL,INTERFACE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_InterfaceTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}	

	FileName=CFileTools::MakeModuleFullPath(NULL,PARAM_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_ParamTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,PARAM_DESCRIPTION_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_ParamDescriptionTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,PACK_SIZE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_PackSizeTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,METHOD_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MethodHeaderTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,METHOD_HEADER_PURE_TAIL_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MethodHeaderPureTailTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,METHOD_CALLER_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MethodCallerSourceTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,MSG_PACK_METHOD_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgPackMethodHeaderTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,MSG_PACK_METHOD_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgPackMethodSourceTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,MSG_HANDLER_METHOD_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgHandlerMethodHeaderTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,MSG_HANDLER_METHOD_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgHandlerMethodSourceTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,MSG_MAP_INIT_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgMapInitTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	

	FileName=CFileTools::MakeModuleFullPath(NULL,MSG_CALLER_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgCallerHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,MSG_CALLER_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgCallerSourceTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,MSG_HANDLER_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgHandlerHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,MSG_HANDLER_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MsgHandlerSourceTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,DATA_OBJECT_DEFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectDefineHeaderTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,DATA_OBJECT_DEFINE_SOURCE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectDefineSourceTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}


	FileName=CFileTools::MakeModuleFullPath(NULL,STRUCT_DEFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_StructDefineHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_EnumDefineHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,SYSTEM_ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_SystemEnumDefineHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,STRUCT_DEFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_StructDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}
	
	FileName=CFileTools::MakeModuleFullPath(NULL,STRUCT_MEMBER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_StructMemberTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName = CFileTools::MakeModuleFullPath(NULL, DATA_OBJECT_MEMBER_TEMPLE_FILE_NAME);
	if (TempleFile.Open(FileName, CFile::modeRead | CFile::shareDenyNone))
	{
		ULONG64 Size = TempleFile.GetLength();
		char * Buffer = new char[Size + 1];
		TempleFile.Read(Buffer, Size);
		Buffer[Size] = 0;
		m_DataObjectMemberTemplate = Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s", FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,DATA_OBJECT_MODIFY_FLAGS_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectModifyFlagsTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}	

	FileName=CFileTools::MakeModuleFullPath(NULL,DATA_OBJECT_MODIFY_FLAG_DEFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectModifyFlagDefineTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,DATA_OBJECT_MODIFY_FLAG_UNIT_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_DataObjectModifyFlagUnitTemple=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,PACK_OPERATION_UNIT_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_PackOperationUnitTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,UNPACK_OPERATION_UNIT_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_UnpackOperationUnitTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName = CFileTools::MakeModuleFullPath(NULL, TO_XML_OPERATION_UNIT_TEMPLE_FILE_NAME);
	if (TempleFile.Open(FileName, CFile::modeRead | CFile::shareDenyNone))
	{
		ULONG64 Size = TempleFile.GetLength();
		char * Buffer = new char[Size + 1];
		TempleFile.Read(Buffer, Size);
		Buffer[Size] = 0;
		m_ToXMLOperationUnitTemplate = Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s", FileName);
		MessageBox(Msg);
	}

	FileName = CFileTools::MakeModuleFullPath(NULL, FROM_XML_OPERATION_UNIT_TEMPLE_FILE_NAME);
	if (TempleFile.Open(FileName, CFile::modeRead | CFile::shareDenyNone))
	{
		ULONG64 Size = TempleFile.GetLength();
		char * Buffer = new char[Size + 1];
		TempleFile.Read(Buffer, Size);
		Buffer[Size] = 0;
		m_FromXMLOperationUnitTemplate = Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s", FileName);
		MessageBox(Msg);
	}

	FileName = CFileTools::MakeModuleFullPath(NULL, STRUCT_XML_PROCESS_TEMPLE_FILE_NAME);
	if (TempleFile.Open(FileName, CFile::modeRead | CFile::shareDenyNone))
	{
		ULONG64 Size = TempleFile.GetLength();
		char * Buffer = new char[Size + 1];
		TempleFile.Read(Buffer, Size);
		Buffer[Size] = 0;
		m_StructXMLProcessTemplate = Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s", FileName);
		MessageBox(Msg);
	}

	FileName = CFileTools::MakeModuleFullPath(NULL, TO_JSON_OPERATION_UNIT_TEMPLE_FILE_NAME);
	if (TempleFile.Open(FileName, CFile::modeRead | CFile::shareDenyNone))
	{
		ULONG64 Size = TempleFile.GetLength();
		char * Buffer = new char[Size + 1];
		TempleFile.Read(Buffer, Size);
		Buffer[Size] = 0;
		m_ToJsonOperationUnitTemplate = Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s", FileName);
		MessageBox(Msg);
	}

	FileName = CFileTools::MakeModuleFullPath(NULL, FROM_JSON_OPERATION_UNIT_TEMPLE_FILE_NAME);
	if (TempleFile.Open(FileName, CFile::modeRead | CFile::shareDenyNone))
	{
		ULONG64 Size = TempleFile.GetLength();
		char * Buffer = new char[Size + 1];
		TempleFile.Read(Buffer, Size);
		Buffer[Size] = 0;
		m_FromJsonOperationUnitTemplate = Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s", FileName);
		MessageBox(Msg);
	}

	FileName = CFileTools::MakeModuleFullPath(NULL, STRUCT_JSON_PROCESS_TEMPLE_FILE_NAME);
	if (TempleFile.Open(FileName, CFile::modeRead | CFile::shareDenyNone))
	{
		ULONG64 Size = TempleFile.GetLength();
		char * Buffer = new char[Size + 1];
		TempleFile.Read(Buffer, Size);
		Buffer[Size] = 0;
		m_StructJsonProcessTemplate = Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s", FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,ENUM_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_EnumDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,ENUM_MEMBER_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_EnumMemberDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,ENUM_STR_VALUE_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_EnumStrValueDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,CONST_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_ConstDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,MACRO_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_MacroDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,CONST_DFINE_HEADER_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_ConstDefineHeaderTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}

	FileName=CFileTools::MakeModuleFullPath(NULL,STRING_CONST_DFINE_TEMPLE_FILE_NAME);
	if(TempleFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		ULONG64 Size=TempleFile.GetLength();
		char * Buffer=new char[Size+1];
		TempleFile.Read(Buffer,Size);
		Buffer[Size]=0;
		m_StringConstDefineTemplate=Buffer;
		delete[] Buffer;
		TempleFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
	}


	return true;
}

bool CCallInterfaceMakerDlg::LoadConfig()
{
	xml_parser Parser;

	CString FileName=CFileTools::MakeModuleFullPath(NULL,CONFIG_FILE_NAME);

	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{

		xml_node Config=Parser.document();
			
		if(Config.moveto_child("Config"))
		{
			{
				xml_node InterfaceHeaderExport=Config;
				if(InterfaceHeaderExport.moveto_child("InterfaceHeaderExport"))
				{
					m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport=(bool)InterfaceHeaderExport.attribute("IsExport");
					m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt = (LPCTSTR)InterfaceHeaderExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node StructExport=Config;
				if(StructExport.moveto_child("StructExport"))
				{
					m_InterfaceConfig.StructExportConfig.IsExport=(bool)StructExport.attribute("IsExport");
					m_InterfaceConfig.StructExportConfig.ExportExt = (LPCTSTR)StructExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node EnumExport=Config;
				if(EnumExport.moveto_child("EnumExport"))
				{
					m_InterfaceConfig.EnumExportConfig.IsExport=(bool)EnumExport.attribute("IsExport");
					m_InterfaceConfig.EnumExportConfig.ExportExt = (LPCTSTR)EnumExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node ConstExport=Config;
				if(ConstExport.moveto_child("ConstExport"))
				{
					m_InterfaceConfig.ConstExportConfig.IsExport=(bool)ConstExport.attribute("IsExport");
					m_InterfaceConfig.ConstExportConfig.ExportExt = (LPCTSTR)ConstExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DOHeaderExport=Config;
				if(DOHeaderExport.moveto_child("DataStructHeaderExport"))
				{
					m_InterfaceConfig.DataStructHeaderExportConfig.IsExport=(bool)DOHeaderExport.attribute("IsExport");
					m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt = (LPCTSTR)DOHeaderExport.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node InterfaceExportConfig=Config;
				if(InterfaceExportConfig.moveto_child("InterfaceExport"))
				{
					m_InterfaceConfig.InterfaceExportConfig.IsExport=(bool)InterfaceExportConfig.attribute("IsExport");
					m_InterfaceConfig.InterfaceExportConfig.ExportExt = (LPCTSTR)InterfaceExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node CallHeaderExportConfig=Config;
				if(CallHeaderExportConfig.moveto_child("CallHeaderExport"))
				{
					m_InterfaceConfig.CallHeaderExportConfig.IsExport=(bool)CallHeaderExportConfig.attribute("IsExport");
					m_InterfaceConfig.CallHeaderExportConfig.ExportExt = (LPCTSTR)CallHeaderExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node CallSourceExportConfig=Config;
				if(CallSourceExportConfig.moveto_child("CallSourceExport"))
				{
					m_InterfaceConfig.CallSourceExportConfig.IsExport=(bool)CallSourceExportConfig.attribute("IsExport");
					m_InterfaceConfig.CallSourceExportConfig.ExportExt = (LPCTSTR)CallSourceExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node HandlerHeaderExportConfig=Config;
				if(HandlerHeaderExportConfig.moveto_child("HandlerHeaderExport"))
				{
					m_InterfaceConfig.HandlerHeaderExportConfig.IsExport=(bool)HandlerHeaderExportConfig.attribute("IsExport");
					m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt = (LPCTSTR)HandlerHeaderExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node HandlerSourceExportConfig=Config;
				if(HandlerSourceExportConfig.moveto_child("HandlerSourceExport"))
				{
					m_InterfaceConfig.HandlerSourceExportConfig.IsExport=(bool)HandlerSourceExportConfig.attribute("IsExport");
					m_InterfaceConfig.HandlerSourceExportConfig.ExportExt = (LPCTSTR)HandlerSourceExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DataObjectHeaderExportConfig=Config;
				if(DataObjectHeaderExportConfig.moveto_child("DataObjectHeaderExport"))
				{
					m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport=(bool)DataObjectHeaderExportConfig.attribute("IsExport");
					m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt = (LPCTSTR)DataObjectHeaderExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DataObjectSourceExportExportConfig=Config;
				if(DataObjectSourceExportExportConfig.moveto_child("DataObjectSourceExport"))
				{
					m_InterfaceConfig.DataObjectSourceExportConfig.IsExport=(bool)DataObjectSourceExportExportConfig.attribute("IsExport");
					m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt = (LPCTSTR)DataObjectSourceExportExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DataObjectModifyFlagsExportConfig=Config;
				if(DataObjectModifyFlagsExportConfig.moveto_child("DataObjectModifyFlagsExport"))
				{
					m_InterfaceConfig.DataObjectModifyFlagsExportConfig.IsExport=(bool)DataObjectModifyFlagsExportConfig.attribute("IsExport");
					m_InterfaceConfig.DataObjectModifyFlagsExportConfig.ExportExt = (LPCTSTR)DataObjectModifyFlagsExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DefaultStructGenerateOperations=Config;
				if(DefaultStructGenerateOperations.moveto_child("DefaultStructGenerateOperations"))
				{
					LoadGenerateOperations(DefaultStructGenerateOperations,m_InterfaceConfig.DefaultStructGenerateOperations);
				}
			}
			{
				xml_node DefaultEnumGenerateOperations = Config;
				if (DefaultEnumGenerateOperations.moveto_child("DefaultEnumGenerateOperations"))
				{
					LoadGenerateOperations(DefaultEnumGenerateOperations, m_InterfaceConfig.DefaultEnumGenerateOperations);
				}
			}
			{
				xml_node ArrayDefineConfig=Config;
				if(ArrayDefineConfig.moveto_child("ArrayDefineConfig"))
				{
					LoadGenerateOperations(ArrayDefineConfig,m_InterfaceConfig.ArrayDefineConfig);
				}
			}
		}
			
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}
bool CCallInterfaceMakerDlg::SaveConfig()
{
	std::ofstream os;	
	CString FileName=CFileTools::MakeModuleFullPath(NULL,CONFIG_FILE_NAME);
	os.open(FileName, std::ios::trunc);	
	if (os.is_open())
	{
		pug::xml_parser Xml;

		Xml.create();
		xml_node Doc;
		Doc = Xml.document();
		xml_node pi = Doc.append_child(node_pi);
		pi.name(_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");
		pi.attribute(_T("encoding")) = _T("gb2312");

		xml_node Config=Doc.append_child(node_element,"Config");

		{
			xml_node InterfaceHeaderExport=Config.append_child(node_element,"InterfaceHeaderExport");
			InterfaceHeaderExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport);
			InterfaceHeaderExport.append_attribute("FileExt",
				m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt);
		}

		{
			xml_node StructExport=Config.append_child(node_element,"StructExport");
			StructExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.StructExportConfig.IsExport);
			StructExport.append_attribute("FileExt",
				m_InterfaceConfig.StructExportConfig.ExportExt);
		}

		{
			xml_node EnumExport=Config.append_child(node_element,"EnumExport");
			EnumExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.EnumExportConfig.IsExport);
			EnumExport.append_attribute("FileExt",
				m_InterfaceConfig.EnumExportConfig.ExportExt);
		}

		{
			xml_node ConstExport=Config.append_child(node_element,"ConstExport");
			ConstExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.ConstExportConfig.IsExport);
			ConstExport.append_attribute("FileExt",
				m_InterfaceConfig.ConstExportConfig.ExportExt);
		}

		{
			xml_node DOHeaderExport=Config.append_child(node_element,"DataStructHeaderExport");
			DOHeaderExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.DataStructHeaderExportConfig.IsExport);
			DOHeaderExport.append_attribute("FileExt",
				m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt);
		}

		{
			xml_node InterfaceExportConfig=Config.append_child(node_element,"InterfaceExport");
			InterfaceExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.InterfaceExportConfig.IsExport);
			InterfaceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.InterfaceExportConfig.ExportExt);
		}

		{
			xml_node CallHeaderExportConfig=Config.append_child(node_element,"CallHeaderExport");
			CallHeaderExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.CallHeaderExportConfig.IsExport);
			CallHeaderExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
		}

		{
			xml_node CallSourceExportConfig=Config.append_child(node_element,"CallSourceExport");
			CallSourceExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.CallSourceExportConfig.IsExport);
			CallSourceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.CallSourceExportConfig.ExportExt);
		}

		{
			xml_node HandlerHeaderExportConfig=Config.append_child(node_element,"HandlerHeaderExport");
			HandlerHeaderExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.HandlerHeaderExportConfig.IsExport);
			HandlerHeaderExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
		}

		{
			xml_node HandlerSourceExportConfig=Config.append_child(node_element,"HandlerSourceExport");
			HandlerSourceExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.HandlerSourceExportConfig.IsExport);
			HandlerSourceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.HandlerSourceExportConfig.ExportExt);
		}

		{
			xml_node DataObjectHeaderExportConfig=Config.append_child(node_element,"DataObjectHeaderExport");
			DataObjectHeaderExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport);
			DataObjectHeaderExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);
		}

		{
			xml_node DataObjectSourceExportConfig=Config.append_child(node_element,"DataObjectSourceExport");
			DataObjectSourceExportConfig.append_attribute("IsExport",
				(bool)m_InterfaceConfig.DataObjectSourceExportConfig.IsExport);
			DataObjectSourceExportConfig.append_attribute("FileExt",
				m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
		}

		{
			xml_node DataObjectModifyFlagsExport=Config.append_child(node_element,"DataObjectModifyFlagsExport");
			DataObjectModifyFlagsExport.append_attribute("IsExport",
				(bool)m_InterfaceConfig.DataObjectModifyFlagsExportConfig.IsExport);
			DataObjectModifyFlagsExport.append_attribute("FileExt",
				m_InterfaceConfig.DataObjectModifyFlagsExportConfig.ExportExt);
		}

		{
			xml_node DefaultStructPackOperation=Config.append_child(node_element,"DefaultStructGenerateOperations");

			SaveGenerateOperations(DefaultStructPackOperation,m_InterfaceConfig.DefaultStructGenerateOperations);			
		}

		{
			xml_node DefaultEnumGenerateOperations = Config.append_child(node_element, "DefaultEnumGenerateOperations");

			SaveGenerateOperations(DefaultEnumGenerateOperations, m_InterfaceConfig.DefaultEnumGenerateOperations);
		}

		{
			xml_node ArrayDefineConfig=Config.append_child(node_element,"ArrayDefineConfig");

			SaveGenerateOperations(ArrayDefineConfig,m_InterfaceConfig.ArrayDefineConfig);			
		}


		os << Xml.document();
		os.close();
		return true;
	}
	else
	{		
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
		return false;
	}
}

bool CCallInterfaceMakerDlg::LoadEnv()
{
	xml_parser Parser;

	CString FileName=CFileTools::MakeModuleFullPath(NULL,ENV_FILE_NAME);
	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{

		xml_node RunEnvironment=Parser.document();
			
		if(RunEnvironment.moveto_child("RunEnvironment"))
		{
			if (RunEnvironment.has_attribute("UTF8Export"))
				m_UTF8Export = ((bool)RunEnvironment.attribute("UTF8Export")) ? TRUE : FALSE;

			{
				xml_node ExportEnv=RunEnvironment;
				if(ExportEnv.moveto_child("ExportEnv"))
				{
					m_RunEnvInfo.RecentExportDir = ((LPCTSTR)ExportEnv.attribute("RecentExportDir").getvalue());
				}

				xml_node DataEnv=RunEnvironment;
				if(DataEnv.moveto_child("DataEnv"))
				{
					m_RunEnvInfo.RecentDataDir = ((LPCTSTR)DataEnv.attribute("RecentDataDir").getvalue());
				}
			}

			xml_node SelectedInterfaces=RunEnvironment;
			if(RunEnvironment.moveto_child("SelectedInterfaces"))
			{
				m_RunEnvInfo.SelectedInterfaces.clear();
				for(UINT i=0;i<RunEnvironment.children();i++)
				{
					CString InterfaceName=RunEnvironment.child(i).name();
					m_RunEnvInfo.SelectedInterfaces.push_back(InterfaceName);					
				}
			}
		}
			
		
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}
bool CCallInterfaceMakerDlg::SaveEnv()
{
	std::ofstream os;	
	CString FileName=CFileTools::MakeModuleFullPath(NULL,ENV_FILE_NAME);
	os.open(FileName, std::ios::trunc);	
	if (os.is_open())
	{
		pug::xml_parser Xml;

		Xml.create();
		xml_node Doc;
		Doc = Xml.document();
		xml_node pi = Doc.append_child(node_pi);
		pi.name(_T("xml"));
		pi.attribute(_T("version")) = _T("1.0");
		pi.attribute(_T("encoding")) = _T("gb2312");

		xml_node RunEnvironment=Doc.append_child(node_element,"RunEnvironment");

		RunEnvironment.append_attribute("UTF8Export", m_UTF8Export ? true : false);

		{
			xml_node ExportEnv=RunEnvironment.append_child(node_element,"ExportEnv");
			ExportEnv.append_attribute("RecentExportDir", m_RunEnvInfo.RecentExportDir);

			xml_node DataEnv=RunEnvironment.append_child(node_element,"DataEnv");
			DataEnv.append_attribute("RecentDataDir", m_RunEnvInfo.RecentDataDir);
		}

		xml_node SelectedInterfaces=RunEnvironment.append_child(node_element,"SelectedInterfaces");

		for (size_t i = 0; i < m_RunEnvInfo.SelectedInterfaces.size(); i++)
		{
			SelectedInterfaces.append_child(node_element, m_RunEnvInfo.SelectedInterfaces[i]);
		}
	
		os << Xml.document();
		os.close();
		return true;
	}
	else
	{	
		CString Msg;
		Msg.Format("无法打开文件%s",FileName);
		MessageBox(Msg);
		return false;
	}
}


void CCallInterfaceMakerDlg::RefreshDataCache()
{
	RefreshInterfaceCache();
	RefreshDataStructCache();
	RefreshModuleCache();
}

void CCallInterfaceMakerDlg::RefreshInterfaceCache()
{
	m_AllInterfaceList.clear();
	FetchInterfaceToCache(m_MainModule);
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		FetchInterfaceToCache(m_ImportModuleList[i]);
	}
}
void CCallInterfaceMakerDlg::RefreshDataStructCache()
{
	m_AllDataStructList.clear();
	FetchDataStructToCache(m_MainModule);
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		FetchDataStructToCache(m_ImportModuleList[i]);
	}
}

void CCallInterfaceMakerDlg::RefreshModuleCache()
{
	m_AllModuleList.clear();
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		m_AllModuleList.push_back(&m_ImportModuleList[i]);
	}
	m_AllModuleList.push_back(&m_MainModule);
}

void CCallInterfaceMakerDlg::FetchInterfaceToCache(MODULE_DEFINE_INFO& InterfaceDefines)
{
	for (size_t i = 0; i < InterfaceDefines.InterfaceList.size(); i++)
	{
		m_AllInterfaceList.push_back(&InterfaceDefines.InterfaceList[i]);
	}
}

void CCallInterfaceMakerDlg::FetchDataStructToCache(MODULE_DEFINE_INFO& InterfaceDefines)
{
	for (size_t i = 0; i < InterfaceDefines.DataStructDefineList.size(); i++)
	{
		m_AllDataStructList.push_back(InterfaceDefines.DataStructDefineList[i]);
	}
}

void CCallInterfaceMakerDlg::GetDataObjectListSorted(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, vector<STRUCT_DEFINE_INFO *>& DataObjectList)
{
	for (size_t i = 0; i < DataStructList.size(); i++)
	{
		if (DataStructList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)DataStructList[i];
			for (size_t j = 0; j < pStructList->StructList.size(); j++)
			{
				if (pStructList->StructList[j].Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				{
					DataObjectList.push_back(&pStructList->StructList[j]);					
				}
			}
		}
	}
	qsort(DataObjectList.begin()._Ptr, DataObjectList.size(), sizeof(STRUCT_DEFINE_INFO *), DataObjectComp);
}

int CCallInterfaceMakerDlg::DataObjectComp(LPCVOID p1, LPCVOID p2)
{
	const STRUCT_DEFINE_INFO * pDataObject1 = *((const STRUCT_DEFINE_INFO **)p1);
	const STRUCT_DEFINE_INFO * pDataObject2 = *((const STRUCT_DEFINE_INFO **)p2);
	if (pDataObject1->ObjectID < pDataObject2->ObjectID)
		return -1;
	else if (pDataObject1->ObjectID > pDataObject2->ObjectID)
		return 1;
	else
		return 0;
}

static int TypeDefineCompare(const void * p1, const void * p2)
{
	TYPE_DEFINE * Type1 = (TYPE_DEFINE *)p1;
	TYPE_DEFINE * Type2 = (TYPE_DEFINE *)p2;
	return strcmp(Type1->Name, Type2->Name);
}
void CCallInterfaceMakerDlg::MakeCurVarType(bool IncludeDataObject)
{
	m_CurVarTypeList=m_BaseTypeList;	
	for (size_t i = 0; i<m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)m_AllDataStructList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if (pStructList->IgnoreOnExport)
					continue;
				if ((pStructList->StructList[j].Flag&STRUCT_FLAG_IS_DATA_OBJECT)==0 || IncludeDataObject)
				{
					TYPE_DEFINE Type;
					Type.Name=pStructList->StructList[j].Name;
					Type.CType=pStructList->StructList[j].Name;
					Type.Flag=TYPE_DEFINE_FLAG_STRUCT;
					if (pStructList->StructList[j].Flag&STRUCT_FLAG_IS_DATA_OBJECT)
						Type.Flag|=TYPE_DEFINE_FLAG_DATA_OBJECT;
					Type.GenerateOperations=pStructList->StructList[j].GenerateOperations;			

					if(Type.GenerateOperations.PackOperation.IsEmpty())
						Type.GenerateOperations.PackOperation=m_InterfaceConfig.DefaultStructGenerateOperations.PackOperation;
					if(Type.GenerateOperations.UnpackOperation.IsEmpty())
						Type.GenerateOperations.UnpackOperation=m_InterfaceConfig.DefaultStructGenerateOperations.UnpackOperation;
					if(Type.GenerateOperations.SizeCaculateOperation.IsEmpty())
						Type.GenerateOperations.SizeCaculateOperation=m_InterfaceConfig.DefaultStructGenerateOperations.SizeCaculateOperation;
					if (Type.GenerateOperations.ToXMLOperation.IsEmpty())
						Type.GenerateOperations.ToXMLOperation = m_InterfaceConfig.DefaultStructGenerateOperations.ToXMLOperation;
					if (Type.GenerateOperations.FromXMLOperation.IsEmpty())
						Type.GenerateOperations.FromXMLOperation = m_InterfaceConfig.DefaultStructGenerateOperations.FromXMLOperation;
					if (Type.GenerateOperations.ToJsonOperation.IsEmpty())
						Type.GenerateOperations.ToJsonOperation = m_InterfaceConfig.DefaultStructGenerateOperations.ToJsonOperation;
					if (Type.GenerateOperations.FromJsonOperation.IsEmpty())
						Type.GenerateOperations.FromJsonOperation = m_InterfaceConfig.DefaultStructGenerateOperations.FromJsonOperation;
					if(Type.GenerateOperations.ReferenceDefine.IsEmpty())
						Type.GenerateOperations.ReferenceDefine=m_InterfaceConfig.DefaultStructGenerateOperations.ReferenceDefine;
					if(Type.GenerateOperations.ReferenceUse.IsEmpty())
						Type.GenerateOperations.ReferenceUse=m_InterfaceConfig.DefaultStructGenerateOperations.ReferenceUse;
					if(Type.GenerateOperations.VariableDefine.IsEmpty())
						Type.GenerateOperations.VariableDefine=m_InterfaceConfig.DefaultStructGenerateOperations.VariableDefine;
					if(Type.GenerateOperations.InitOperation.IsEmpty())
						Type.GenerateOperations.InitOperation=m_InterfaceConfig.DefaultStructGenerateOperations.InitOperation;
					if(Type.GenerateOperations.CloneOperation.IsEmpty())
						Type.GenerateOperations.CloneOperation=m_InterfaceConfig.DefaultStructGenerateOperations.CloneOperation;
					if(Type.GenerateOperations.GetMethodDeclare.IsEmpty())
						Type.GenerateOperations.GetMethodDeclare=m_InterfaceConfig.DefaultStructGenerateOperations.GetMethodDeclare;
					if(Type.GenerateOperations.GetMethodDefine.IsEmpty())
						Type.GenerateOperations.GetMethodDefine=m_InterfaceConfig.DefaultStructGenerateOperations.GetMethodDefine;
					if(Type.GenerateOperations.SetMethodDeclare.IsEmpty())
						Type.GenerateOperations.SetMethodDeclare=m_InterfaceConfig.DefaultStructGenerateOperations.SetMethodDeclare;
					if(Type.GenerateOperations.SetMethodDefine.IsEmpty())
						Type.GenerateOperations.SetMethodDefine=m_InterfaceConfig.DefaultStructGenerateOperations.SetMethodDefine;

					m_CurVarTypeList.push_back(Type);
				}
			}
		}
		else if (m_AllDataStructList[i]->ListType == DATA_STRUCT_ENUM)
		{
			ENUM_DEFINE_LIST * pEnumList = (ENUM_DEFINE_LIST *)m_AllDataStructList[i];
			for (size_t j = 0; j<pEnumList->EnumList.size(); j++)
			{
				if (pEnumList->IgnoreOnExport)
					continue;

				TYPE_DEFINE Type;
				Type.Name = pEnumList->EnumList[j].Name;
				Type.CType = pEnumList->EnumList[j].Name;
				Type.Flag = TYPE_DEFINE_FLAG_ENUM;
				if (pEnumList->EnumList[j].Flag&ENUM_DEFINE_FLAG_IS_64BIT)
					Type.Flag |= TYPE_DEFINE_FLAG_64BIT;
				Type.GenerateOperations = m_InterfaceConfig.DefaultEnumGenerateOperations;

				m_CurVarTypeList.push_back(Type);
			}
		}
	}
	if (m_CurVarTypeList.size()>m_BaseTypeList.size())
	{
		qsort(&(m_CurVarTypeList[m_BaseTypeList.size()]), m_CurVarTypeList.size() - m_BaseTypeList.size(),
			sizeof(TYPE_DEFINE), TypeDefineCompare);
	}
}


void CCallInterfaceMakerDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码	
	CFileDialog Dlg(true, "*.xml", "*.xml", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "XML Files(*.xml)|*,xml|All Files(*.*)|*.*||");

	if(!m_RunEnvInfo.RecentDataDir.IsEmpty())
	{
		Dlg.m_ofn.lpstrInitialDir=m_RunEnvInfo.RecentDataDir;
	}

	if(Dlg.DoModal()==IDOK)
	{
		if (LoadOverVersion(Dlg.GetPathName(), m_MainModule))
		{
			m_MainModule.ModuleDefineFileName = Dlg.GetPathName();
			for (size_t i = 0; i < m_MainModule.ImportList.size(); i++)
			{
				CEasyString FileName = CFileTools::MakeFullPath(CFileTools::GetPathDirectory(m_MainModule.ModuleDefineFileName) + DIR_SLASH + (LPCTSTR)m_MainModule.ImportList[i]);
				LoadModule(FileName);
			}
			RefreshDataCache();

			MakeCurVarType(true);


			FillListItem();
			UpdateData(false);
			RestoreInterfaceSelections();


			m_RunEnvInfo.RecentDataDir = (LPCTSTR)CFileTools::GetPathDirectory(Dlg.GetPathName());
			m_IsLoaded = true;
		}
	}
}
void CCallInterfaceMakerDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_MainModule.ModuleDefineFileName.IsEmpty())
	{
		OnBnClickedButtonSaveAs();
	}
	else
	{
		UpdateData(true);
		Save(m_MainModule);
		for (size_t i = 0; i < m_ImportModuleList.size(); i++)
		{
			Save(m_ImportModuleList[i]);
		}
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonSaveAs()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog Dlg(false,"*.xml","*.xml",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"XML Files(*.xml)|*,xml|All Files(*.*)|*.*||");

	if(Dlg.DoModal()==IDOK)
	{
		UpdateData(true);
		m_MainModule.ModuleDefineFileName = Dlg.GetPathName();
		Save(m_MainModule);
		for (size_t i = 0; i < m_ImportModuleList.size(); i++)
		{
			Save(m_ImportModuleList[i]);
		}

		m_RunEnvInfo.RecentDataDir=Dlg.GetFolderPath();
	}
}
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	{
		CString DefaultFolder=GetMainDlg()->GetRunEnv().RecentExportDir;
		if(!DefaultFolder.IsEmpty())
		{
			SendMessage(hwnd, BFFM_SETSELECTION,
				TRUE,(LPARAM)((LPCTSTR)DefaultFolder));
		}
	}
	return 0;
}

void CCallInterfaceMakerDlg::OnBnClickedButtonSaveExport()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);


	BROWSEINFO bi;
	char FilePath[MAX_PATH];
	LPITEMIDLIST pidl;

	ZeroMemory(&bi,sizeof(bi));

	//bi.pszDisplayName=FilePath;
	bi.lpszTitle="请选择输出目录";
	bi.ulFlags=BIF_RETURNONLYFSDIRS|BIF_USENEWUI|BIF_SHAREABLE;
	bi.lpfn=BrowseCallbackProc;
	pidl=::SHBrowseForFolder(&bi);
	if(pidl!=NULL)
	{
		::SHGetPathFromIDList(pidl,FilePath);
		IMalloc *pMalloc=NULL;
		::SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);
		pMalloc->Release();
		pMalloc=NULL;
		
		m_RunEnvInfo.RecentExportDir=FilePath;

		SaveInterfaceSelections();
		SaveEnv();		
		MakeCurVarType(true);
		Export(FilePath);		
		AfxMessageBox("导出完毕");	

	}	


	
}

void CCallInterfaceMakerDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	SaveConfig();
	if (m_IsLoaded)
		SaveEnv();
	CDialog::OnCancel();
}

void CCallInterfaceMakerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

bool CCallInterfaceMakerDlg::Save(MODULE_DEFINE_INFO& ModuleInfo)
{
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("gb2312");


	xml_node Interfaces = Doc.append_child(node_element, "Interfaces");
	Interfaces.append_attribute("Version", "2.0.0.0");
	Interfaces.append_attribute("Name", (LPCTSTR)ModuleInfo.Name);
	Interfaces.append_attribute("ModuleID", (UINT)ModuleInfo.ModuleID);
	Interfaces.append_attribute("IDSeed", (UINT)ModuleInfo.IDSeed);	

	xml_node Imports = Interfaces.append_child(node_element, "Imports");
	for (size_t i = 0; i < ModuleInfo.ImportList.size(); i++)
	{
		xml_node Import = Imports.append_child(node_element, "Import");
		Import.append_attribute("FileName", ModuleInfo.ImportList[i]);
	}

	xml_node GlobalDefine = Interfaces.append_child(node_element, "GlobalDefine");
	for (size_t i = 0; i < ModuleInfo.DataStructDefineList.size(); i++)
	{
		switch (ModuleInfo.DataStructDefineList[i]->ListType)
		{
		case DATA_STRUCT_CONST:
		{
			CONST_DEFINE_LIST * pConstList = (CONST_DEFINE_LIST *)ModuleInfo.DataStructDefineList[i];
			xml_node Consts = GlobalDefine.append_child(node_element, "Consts");
			Consts.append_attribute("Name", pConstList->ListName);
			Consts.append_attribute("IgnoreOnExport", pConstList->IgnoreOnExport);
			SaveConstDefine(Consts, pConstList->ConstList);
		}
		break;
		case DATA_STRUCT_ENUM:
		{
			ENUM_DEFINE_LIST * pEnumList = (ENUM_DEFINE_LIST *)ModuleInfo.DataStructDefineList[i];
			xml_node Enums = GlobalDefine.append_child(node_element, "Enums");
			Enums.append_attribute("Name", pEnumList->ListName);
			Enums.append_attribute("IgnoreOnExport", pEnumList->IgnoreOnExport);
			SaveEnumDefine(Enums, pEnumList->EnumList);
		}
		break;
		case DATA_STRUCT_STRUCT:
		{
			STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)ModuleInfo.DataStructDefineList[i];
			xml_node Structs = GlobalDefine.append_child(node_element, "Structs");
			Structs.append_attribute("Name", pStructList->ListName);
			Structs.append_attribute("IgnoreOnExport", pStructList->IgnoreOnExport);
			SaveStructDefine(Structs, pStructList->StructList);
		}
		break;
		}
	}

	xml_node DataObjectModifyFlags = GlobalDefine.append_child(node_element, "DataObjectModifyFlags");
	SaveDataObjectModifyFlag(DataObjectModifyFlags, ModuleInfo.DataObjectModifyFlags);

	SaveInterfaces(Interfaces, ModuleInfo.InterfaceList);

	xml_node Prefix = Interfaces.append_child(node_element, "Prefix");
	Prefix.append_attribute("Content", (LPCTSTR)ModuleInfo.InterfacePrefix);

	xml_node Postfix = Interfaces.append_child(node_element, "Postfix");
	Postfix.append_attribute("Content", (LPCTSTR)ModuleInfo.InterfacePostFix);

	return Xml.SaveToFile(Xml.document(), ModuleInfo.ModuleDefineFileName);
}



bool CCallInterfaceMakerDlg::Load(LPCTSTR FileName, MODULE_DEFINE_INFO& ModuleInfo)
{
	xml_parser Parser;

	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{
		ModuleInfo.Clear();
		ModuleInfo.ModuleDefineFileName = FileName;

		xml_node Interfaces=Parser.document();
		if(Interfaces.moveto_child("Interfaces"))
		{

			ModuleInfo.Name = (LPCTSTR)Interfaces.attribute("Name").getvalue();
			ModuleInfo.Name.Trim();
			ModuleInfo.ModuleID = (UINT)Interfaces.attribute("ModuleID");
			ModuleInfo.IDSeed = (UINT)Interfaces.attribute("IDSeed");

			xml_node Imports = Interfaces;
			if (Imports.moveto_child("Imports"))
			{
				for (UINT i = 0; i < Imports.children(); i++)
				{
					if (stricmp(Imports.child(i).get_name(), "Import") == 0)
					{
						CString ImportFileName = Imports.child(i).attribute("FileName").getvalue();
						if (!ImportFileName.IsEmpty())
							ModuleInfo.ImportList.push_back(ImportFileName);
					}
				}

			}

			xml_node GlobalDefine = Interfaces;
			if (GlobalDefine.moveto_child("GlobalDefine"))
			{
				for (UINT i = 0; i < GlobalDefine.children(); i++)
				{
					if (stricmp(GlobalDefine.child(i).get_name(), "Consts") == 0)
					{
						CONST_DEFINE_LIST * pConstList = new CONST_DEFINE_LIST;
						pConstList->ListType = DATA_STRUCT_CONST;
						pConstList->ModuleID = ModuleInfo.ModuleID;
						pConstList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute("Name").getvalue();
						pConstList->IgnoreOnExport = GlobalDefine.child(i).attribute("IgnoreOnExport");
						LoadConstDefine(GlobalDefine.child(i), pConstList->ConstList);
						ModuleInfo.DataStructDefineList.push_back(pConstList);
					}
					else if (stricmp(GlobalDefine.child(i).get_name(), "Enums") == 0)
					{
						ENUM_DEFINE_LIST * pEnumList = new ENUM_DEFINE_LIST;
						pEnumList->ListType = DATA_STRUCT_ENUM;
						pEnumList->ModuleID = ModuleInfo.ModuleID;
						pEnumList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute("Name").getvalue();
						pEnumList->IgnoreOnExport = GlobalDefine.child(i).attribute("IgnoreOnExport");
						LoadEnumDefine(GlobalDefine.child(i), pEnumList->EnumList);
						ModuleInfo.DataStructDefineList.push_back(pEnumList);
					}
					else if (stricmp(GlobalDefine.child(i).get_name(), "Structs") == 0)
					{
						STRUCT_DEFINE_LIST * pStructList = new STRUCT_DEFINE_LIST;
						pStructList->ListType = DATA_STRUCT_STRUCT;
						pStructList->ModuleID = ModuleInfo.ModuleID;
						pStructList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute("Name").getvalue();
						pStructList->IgnoreOnExport = GlobalDefine.child(i).attribute("IgnoreOnExport");
						LoadStructDefine(GlobalDefine.child(i), pStructList->StructList);
						ModuleInfo.DataStructDefineList.push_back(pStructList);
					}
					else if (stricmp(GlobalDefine.child(i).get_name(), "DataObjectModifyFlags") == 0)
					{
						LoadDataObjectModifyFlag(GlobalDefine.child(i), ModuleInfo.DataObjectModifyFlags);
					}
				}
			}
			LoadInterfaces(Interfaces, ModuleInfo.InterfaceList, ModuleInfo.ModuleID);
			
			xml_node Prefix=Interfaces;
			if(Prefix.moveto_child("Prefix"))
			{
				ModuleInfo.InterfacePrefix = ((LPCTSTR)Prefix.attribute("Content").getvalue());
			}
			xml_node Postfix=Interfaces;
			if(Postfix.moveto_child("Postfix"))
			{
				ModuleInfo.InterfacePostFix = ((LPCTSTR)Postfix.attribute("Content").getvalue());
			}		

			//LoadEnv();
			return true;
		}
	}
	
	return false;
}

bool CCallInterfaceMakerDlg::LoadOld(LPCTSTR FileName, INTERFANCE_DEFINES_OLD& ModuleInfo)
{
	xml_parser Parser;

	if (Parser.parse_file(FileName, pug::parse_trim_attribute))
	{
		ModuleInfo.Clear();

		xml_node Interfaces = Parser.document();
		if (Interfaces.moveto_child("Interfaces"))
		{

			ModuleInfo.Name = (LPCTSTR)Interfaces.attribute("Name").getvalue();
			ModuleInfo.Name.Trim();
			ModuleInfo.InterfaceIDSeed = (UINT)Interfaces.attribute("IDSeed");

			xml_node GlobalDefine = Interfaces;
			if (GlobalDefine.moveto_child("GlobalDefine"))
			{
				for (UINT i = 0; i < GlobalDefine.children(); i++)
				{
					if (stricmp(GlobalDefine.child(i).get_name(), "Consts") == 0)
					{
						CONST_DEFINE_LIST * pConstList = new CONST_DEFINE_LIST;
						pConstList->ListType = DATA_STRUCT_CONST;
						pConstList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute("Name").getvalue();
						pConstList->IgnoreOnExport = GlobalDefine.child(i).attribute("IgnoreOnExport");
						LoadConstDefine(GlobalDefine.child(i), pConstList->ConstList);
						ModuleInfo.DataStructDefines.DataStructDefineList.push_back(pConstList);
					}
					else if (stricmp(GlobalDefine.child(i).get_name(), "Enums") == 0)
					{
						ENUM_DEFINE_LIST * pEnumList = new ENUM_DEFINE_LIST;
						pEnumList->ListType = DATA_STRUCT_ENUM;
						pEnumList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute("Name").getvalue();
						pEnumList->IgnoreOnExport = GlobalDefine.child(i).attribute("IgnoreOnExport");
						LoadEnumDefine(GlobalDefine.child(i), pEnumList->EnumList);
						ModuleInfo.DataStructDefines.DataStructDefineList.push_back(pEnumList);
					}
					else if (stricmp(GlobalDefine.child(i).get_name(), "Structs") == 0)
					{
						STRUCT_DEFINE_LIST_OLD * pStructList = new STRUCT_DEFINE_LIST_OLD;
						pStructList->ListType = DATA_STRUCT_STRUCT;
						pStructList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute("Name").getvalue();
						pStructList->IgnoreOnExport = GlobalDefine.child(i).attribute("IgnoreOnExport");
						LoadStructDefineOld(GlobalDefine.child(i), pStructList->StructList);
						ModuleInfo.DataStructDefines.DataStructDefineList.push_back(pStructList);
					}
					else if (stricmp(GlobalDefine.child(i).get_name(), "DataObjectModifyFlags") == 0)
					{
						LoadDataObjectModifyFlag(GlobalDefine.child(i), ModuleInfo.DataStructDefines.DataObjectModifyFlags);
					}
				}
			}
			LoadInterfacesOld(Interfaces, ModuleInfo.InterfaceList);

			xml_node Prefix = Interfaces;
			if (Prefix.moveto_child("Prefix"))
			{
				ModuleInfo.InterfacePrefix = ((LPCTSTR)Prefix.attribute("Content").getvalue());
			}
			xml_node Postfix = Interfaces;
			if (Postfix.moveto_child("Postfix"))
			{
				ModuleInfo.InterfacePostFix = ((LPCTSTR)Postfix.attribute("Content").getvalue());
			}

			//LoadEnv();
			return true;
		}
	}

	return false;
}

bool CCallInterfaceMakerDlg::LoadOverVersion(LPCTSTR FileName, MODULE_DEFINE_INFO& ModuleInfo)
{
	UINT Version = GetFileVersion(FileName);
	if (Version == 0)
	{
		INTERFANCE_DEFINES_OLD OldInterfaceDefines;
		if (LoadOld(FileName, OldInterfaceDefines))
		{
			if (ConvertOldInterfaceDefines(OldInterfaceDefines, ModuleInfo))
			{
				ModuleInfo.ModuleDefineFileName = FileName;
				OldInterfaceDefines.Clear();
				return true;
			}
			else
			{
				OldInterfaceDefines.Clear();
				ModuleInfo.Clear();
				AfxMessageBox("转换旧版本文件失败");
				return false;
			}
		}
		else
		{
			OldInterfaceDefines.Clear();
			AfxMessageBox("加载失败");
			return false;
		}
	}
	else
	{
		if (Load(FileName, ModuleInfo))
		{
			return true;
		}
		else
		{
			ModuleInfo.Clear();
			AfxMessageBox("加载失败");
			return false;
		}
	}
}

UINT CCallInterfaceMakerDlg::GetFileVersion(LPCTSTR FileName)
{
	xml_parser Parser;

	if (Parser.parse_file(FileName, pug::parse_trim_attribute))
	{
		xml_node Interfaces = Parser.document();
		if (Interfaces.moveto_child("Interfaces"))
		{

			if (Interfaces.has_attribute("Version"))
			{
				return (UINT)Interfaces.attribute("Version");
			}
		}
	}
	return 0;
}

MODULE_DEFINE_INFO * CCallInterfaceMakerDlg::LoadModule(LPCTSTR szFileName)
{	
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		if (stricmp(m_ImportModuleList[i].ModuleDefineFileName, szFileName) == 0)
			return NULL;
	}
	MODULE_DEFINE_INFO ModuleDefine;
	if (LoadOverVersion(szFileName, ModuleDefine))
	{
		m_ImportModuleList.push_back(ModuleDefine);
		MODULE_DEFINE_INFO * pModuleIndo = m_ImportModuleList.end()._Ptr;
		for (size_t i = 0; i < ModuleDefine.ImportList.size(); i++)
		{
			CEasyString FileName = CFileTools::MakeFullPath(CFileTools::GetPathDirectory(ModuleDefine.ModuleDefineFileName) + DIR_SLASH + (LPCTSTR)ModuleDefine.ImportList[i]);
			LoadModule(FileName);
		}
		return pModuleIndo;
	}
	return NULL;
}

bool CCallInterfaceMakerDlg::SaveInterfaces(xml_node& Interfaces, vector<CALLER_INTERFACE>& InterfaceList)
{
	for (size_t i = 0; i < InterfaceList.size(); i++)
	{
		CALLER_INTERFACE& InterfaceInfo = InterfaceList[i];
		xml_node Interface = Interfaces.append_child(node_element, "Interface");

		Interface.append_attribute("Name", (LPCTSTR)InterfaceInfo.Name);
		Interface.append_attribute("ID", (UINT)InterfaceInfo.ID);
		Interface.append_attribute("IDSeed", (UINT)InterfaceInfo.IDSeed);
		Interface.append_attribute("Description", (LPCTSTR)InterfaceInfo.Description);

		//xml_node LocalDefine = Interface.append_child(node_element, "LocalDefine");
		//xml_node LocalStructs = LocalDefine.append_child(node_element, "Structs");

		for (size_t j = 0; j < InterfaceInfo.MethodList.size(); j++)
		{
			INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];
			xml_node Method = Interface.append_child(node_element, "Method");

			Method.append_attribute("Name", (LPCTSTR)MethodInfo.Name);
			Method.append_attribute("Type", (UINT)MethodInfo.Type);
			Method.append_attribute("ID", (UINT)MethodInfo.ID);
			Method.append_attribute("Flag", (UINT)MethodInfo.Flag);
			Method.append_attribute("ParamIDSeed", (UINT)MethodInfo.ParamIDSeed);
			Method.append_attribute("Description", (LPCTSTR)MethodInfo.Description);
			for (size_t k = 0; k < MethodInfo.CallParamList.size(); k++)
			{
				METHOD_PARAM& ParamInfo = MethodInfo.CallParamList[k];

				xml_node Param = Method.append_child(node_element, "CallParam");
				CString Temp;


				Param.append_attribute("Name", (LPCTSTR)ParamInfo.Name);
				Param.append_attribute("Type", (LPCTSTR)ParamInfo.Type);
				Param.append_attribute("ID", (UINT)ParamInfo.ID);
				Param.append_attribute("Length", (UINT)ParamInfo.Length);
				Param.append_attribute("CanNull", ParamInfo.CanNull);
				Param.append_attribute("IsReference", ParamInfo.IsReference);
				Param.append_attribute("DefaultValue", (LPCTSTR)ParamInfo.DefaultValue);
				Param.append_attribute("Description", (LPCTSTR)ParamInfo.Description);

			}
			for (size_t k = 0; k < MethodInfo.AckParamList.size(); k++)
			{
				METHOD_PARAM& ParamInfo = MethodInfo.AckParamList[k];

				xml_node Param = Method.append_child(node_element, "AckParam");
				CString Temp;


				Param.append_attribute("Name", (LPCTSTR)ParamInfo.Name);
				Param.append_attribute("Type", (LPCTSTR)ParamInfo.Type);
				Param.append_attribute("ID", (UINT)ParamInfo.ID);
				Param.append_attribute("Length", (UINT)ParamInfo.Length);
				Param.append_attribute("CanNull", ParamInfo.CanNull);
				Param.append_attribute("IsReference", ParamInfo.IsReference);
				Param.append_attribute("DefaultValue", (LPCTSTR)ParamInfo.DefaultValue);
				Param.append_attribute("Description", (LPCTSTR)ParamInfo.Description);
			}
		}
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadInterfaces(xml_node& Interfaces, vector<CALLER_INTERFACE>& InterfaceList, UINT ModuleID)
{
	for (UINT i = 0; i < Interfaces.children(); i++)
	{
		xml_node Interface = Interfaces.child(i);
		if (_stricmp(Interface.name(), "Interface") == 0)
		{
			CALLER_INTERFACE InterfaceInfo;

			InterfaceInfo.Name = (LPCTSTR)Interface.attribute("Name").getvalue();
			InterfaceInfo.Name.Trim();
			InterfaceInfo.ModuleID = ModuleID;
			InterfaceInfo.ID = (UINT)Interface.attribute("ID");
			InterfaceInfo.IDSeed = (UINT)Interface.attribute("IDSeed");
			InterfaceInfo.Description = ((LPCTSTR)Interface.attribute("Description").getvalue());



			for (UINT j = 0; j < Interface.children(); j++)
			{
				xml_node Method = Interface.child(j);
				if (_stricmp(Method.name(), "Method") == 0)
				{
					INTERFACE_METHOD MethodInfo;
					MethodInfo.Name = (LPCTSTR)Method.attribute("Name").getvalue();
					MethodInfo.Name.Trim();
					MethodInfo.Type = (UINT)Method.attribute("Type");
					MethodInfo.ID = (UINT)Method.attribute("ID");
					MethodInfo.Flag = (UINT)Method.attribute("Flag");
					MethodInfo.ParamIDSeed = (UINT)Method.attribute("ParamIDSeed");
					MethodInfo.Description = ((LPCTSTR)Method.attribute("Description").getvalue());

					for (UINT k = 0; k < Method.children(); k++)
					{
						xml_node Param = Method.child(k);
						if (_stricmp(Param.name(), "CallParam") == 0)
						{

							METHOD_PARAM ParamInfo;

							ParamInfo.Name = (LPCTSTR)Param.attribute("Name").getvalue();
							ParamInfo.Name.Trim();

							ParamInfo.Type = ((LPCTSTR)Param.attribute("Type").getvalue());
							ParamInfo.ID = (UINT)Param.attribute("ID");
							ParamInfo.Length = (UINT)Param.attribute("Length");
							ParamInfo.CanNull = Param.attribute("CanNull");
							ParamInfo.IsReference = Param.attribute("IsReference");

							ParamInfo.DefaultValue = ((LPCTSTR)Param.attribute("DefaultValue").getvalue());
							ParamInfo.DefaultValue.Trim();


							ParamInfo.Description = ((LPCTSTR)Param.attribute("Description").getvalue());


							MethodInfo.CallParamList.push_back(ParamInfo);
						}
						else if (_stricmp(Param.name(), "AckParam") == 0)
						{

							METHOD_PARAM ParamInfo;

							ParamInfo.Name = (LPCTSTR)Param.attribute("Name").getvalue();
							ParamInfo.Name.Trim();

							ParamInfo.Type = ((LPCTSTR)Param.attribute("Type").getvalue());
							ParamInfo.ID = (UINT)Param.attribute("ID");
							ParamInfo.Length = (UINT)Param.attribute("Length");
							ParamInfo.CanNull = Param.attribute("CanNull");
							ParamInfo.IsReference = Param.attribute("IsReference");

							ParamInfo.DefaultValue = ((LPCTSTR)Param.attribute("DefaultValue").getvalue());
							ParamInfo.DefaultValue.Trim();


							ParamInfo.Description = ((LPCTSTR)Param.attribute("Description").getvalue());


							MethodInfo.AckParamList.push_back(ParamInfo);
						}

					}
					InterfaceInfo.MethodList.push_back(MethodInfo);
				}
			}
			InterfaceList.push_back(InterfaceInfo);
		}
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadInterfacesOld(xml_node& Interfaces, vector<CALLER_INTERFACE_OLD>& InterfaceList)
{
	for (UINT i = 0; i < Interfaces.children(); i++)
	{
		xml_node Interface = Interfaces.child(i);
		if (_stricmp(Interface.name(), "Interface") == 0)
		{
			CALLER_INTERFACE_OLD InterfaceInfo;

			InterfaceInfo.Name = (LPCTSTR)Interface.attribute("Name").getvalue();
			InterfaceInfo.Name.Trim();
			InterfaceInfo.Type = Interface.attribute("Type");
			InterfaceInfo.ID = (UINT)Interface.attribute("ID");
			InterfaceInfo.IDSeed = (UINT)Interface.attribute("IDSeed");
			InterfaceInfo.Description = ((LPCTSTR)Interface.attribute("Description").getvalue());



			for (UINT j = 0; j < Interface.children(); j++)
			{
				xml_node Method = Interface.child(j);
				if (_stricmp(Method.name(), "Method") == 0)
				{
					INTERFACE_METHOD_OLD MethodInfo;
					MethodInfo.Name = (LPCTSTR)Method.attribute("Name").getvalue();
					MethodInfo.Name.Trim();
					MethodInfo.ID = (UINT)Method.attribute("ID");
					MethodInfo.Flag = (UINT)Method.attribute("Flag");
					MethodInfo.ParamIDSeed = (UINT)Method.attribute("ParamIDSeed");
					MethodInfo.Description = ((LPCTSTR)Method.attribute("Description").getvalue());

					for (UINT k = 0; k < Method.children(); k++)
					{
						xml_node Param = Method.child(k);
						if (_stricmp(Param.name(), "Param") == 0)
						{

							METHOD_PARAM ParamInfo;

							ParamInfo.Name = (LPCTSTR)Param.attribute("Name").getvalue();
							ParamInfo.Name.Trim();

							ParamInfo.Type = ((LPCTSTR)Param.attribute("Type").getvalue());
							ParamInfo.ID = (UINT)Param.attribute("ID");
							ParamInfo.Length = (UINT)Param.attribute("Length");
							ParamInfo.CanNull = Param.attribute("CanNull");
							ParamInfo.IsReference = Param.attribute("IsReference");

							ParamInfo.DefaultValue = ((LPCTSTR)Param.attribute("DefaultValue").getvalue());
							ParamInfo.DefaultValue.Trim();


							ParamInfo.Description = ((LPCTSTR)Param.attribute("Description").getvalue());


							MethodInfo.ParamList.push_back(ParamInfo);
						}

					}
					InterfaceInfo.MethodList.push_back(MethodInfo);
				}
			}
			InterfaceList.push_back(InterfaceInfo);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveStructDefine(xml_node& Structs,vector<STRUCT_DEFINE_INFO>& StructDefineList)
{
	for(size_t i=0;i<StructDefineList.size();i++)
	{
		xml_node Struct=Structs.append_child(node_element,"Struct");
		Struct.append_attribute("Name",(LPCTSTR)StructDefineList[i].Name);
		Struct.append_attribute("ShortName",(LPCTSTR)StructDefineList[i].ShortName);
		Struct.append_attribute("BaseStruct",(LPCTSTR)StructDefineList[i].BaseStruct);
		Struct.append_attribute("Description",(LPCTSTR)StructDefineList[i].Description);
		Struct.append_attribute("IDSeed",(UINT)StructDefineList[i].IDSeed);		
		Struct.append_attribute("Flag", (UINT)StructDefineList[i].Flag);
		Struct.append_attribute("ObjectID",(UINT)StructDefineList[i].ObjectID);

		xml_node GenerateOperations=Struct.append_child(node_element,"GenerateOperations");
		SaveGenerateOperations(GenerateOperations,StructDefineList[i].GenerateOperations);
		
		for(size_t j=0;j<StructDefineList[i].MemberList.size();j++)
		{
			xml_node Member=Struct.append_child(node_element,"Member");
			Member.append_attribute("Name",(LPCTSTR)StructDefineList[i].MemberList[j].Name);
			Member.append_attribute("Type",(LPCTSTR)StructDefineList[i].MemberList[j].Type);
			Member.append_attribute("Flag",(UINT)StructDefineList[i].MemberList[j].Flag);
			Member.append_attribute("IsArray",(bool)StructDefineList[i].MemberList[j].IsArray);
			Member.append_attribute("ArrayStartLength",(UINT)StructDefineList[i].MemberList[j].ArrayStartLength);
			Member.append_attribute("ArrayGrowLength",(UINT)StructDefineList[i].MemberList[j].ArrayGrowLength);
			Member.append_attribute("Description",(LPCTSTR)StructDefineList[i].MemberList[j].Description);
			Member.append_attribute("ID",(UINT)StructDefineList[i].MemberList[j].ID);			
		}
	}
	

	return true;
}

bool CCallInterfaceMakerDlg::LoadStructDefine(xml_node& Structs,vector<STRUCT_DEFINE_INFO>& StructDefineList)
{
	for(UINT i=0;i<Structs.children();i++)
	{
		xml_node Struct=Structs.child(i);
		if(_stricmp(Struct.name(),"Struct")==0)
		{
			STRUCT_DEFINE_INFO StructDefine;
			StructDefine.Name = (LPCTSTR)Struct.attribute("Name").getvalue();
			StructDefine.ShortName = (LPCTSTR)Struct.attribute("ShortName").getvalue();
			StructDefine.BaseStruct = (LPCTSTR)Struct.attribute("BaseStruct").getvalue();
			StructDefine.Description = ((LPCTSTR)Struct.attribute("Description").getvalue());
			StructDefine.IDSeed=(UINT)Struct.attribute("IDSeed");
			StructDefine.Flag = (UINT)Struct.attribute("Flag");
			StructDefine.ObjectID=(UINT)Struct.attribute("ObjectID");

			

			for(UINT j=0;j<Struct.children();j++)
			{
				xml_node Member=Struct.child(j);
				if(_stricmp(Member.name(),"Member")==0)
				{
					STRUCT_MEMBER_INFO MemberInfo;
					MemberInfo.Name = (LPCTSTR)Member.attribute("Name").getvalue();
					MemberInfo.Type = ((LPCTSTR)Member.attribute("Type").getvalue());
					MemberInfo.Flag=(UINT)Member.attribute("Flag");
					MemberInfo.IsArray=(bool)Member.attribute("IsArray");
					MemberInfo.ArrayStartLength=(UINT)Member.attribute("ArrayStartLength");
					MemberInfo.ArrayGrowLength=(UINT)Member.attribute("ArrayGrowLength");
					MemberInfo.Description = ((LPCTSTR)Member.attribute("Description").getvalue());
					MemberInfo.ID=(UINT)Member.attribute("ID");
					StructDefine.MemberList.push_back(MemberInfo);
				}
				else if(_stricmp(Member.name(),"GenerateOperations")==0)
				{
					LoadGenerateOperations(Member,StructDefine.GenerateOperations);
				}
			}
			StructDefineList.push_back(StructDefine);
		}		
	}
	return true;
}

bool CCallInterfaceMakerDlg::LoadStructDefineOld(xml_node& Structs, vector<STRUCT_DEFINE_INFO_OLD>& StructDefineList)
{
	for (UINT i = 0; i < Structs.children(); i++)
	{
		xml_node Struct = Structs.child(i);
		if (_stricmp(Struct.name(), "Struct") == 0)
		{
			STRUCT_DEFINE_INFO_OLD StructDefine;
			StructDefine.Name = (LPCTSTR)Struct.attribute("Name").getvalue();
			StructDefine.ShortName = (LPCTSTR)Struct.attribute("ShortName").getvalue();
			StructDefine.BaseStruct = (LPCTSTR)Struct.attribute("BaseStruct").getvalue();
			StructDefine.Description = ((LPCTSTR)Struct.attribute("Description").getvalue());
			StructDefine.IDSeed = (UINT)Struct.attribute("IDSeed");
			StructDefine.IsDataObject = (bool)Struct.attribute("IsDataObject");
			StructDefine.DeclareLater = (bool)Struct.attribute("DeclareLater");
			StructDefine.ObjectID = (UINT)Struct.attribute("ObjectID");



			for (UINT j = 0; j < Struct.children(); j++)
			{
				xml_node Member = Struct.child(j);
				if (_stricmp(Member.name(), "Member") == 0)
				{
					STRUCT_MEMBER_INFO MemberInfo;
					MemberInfo.Name = (LPCTSTR)Member.attribute("Name").getvalue();
					MemberInfo.Type = ((LPCTSTR)Member.attribute("Type").getvalue());
					MemberInfo.Flag = (UINT)Member.attribute("Flag");
					MemberInfo.IsArray = (bool)Member.attribute("IsArray");
					MemberInfo.ArrayStartLength = (UINT)Member.attribute("ArrayStartLength");
					MemberInfo.ArrayGrowLength = (UINT)Member.attribute("ArrayGrowLength");
					MemberInfo.Description = ((LPCTSTR)Member.attribute("Description").getvalue());
					MemberInfo.ID = (UINT)Member.attribute("ID");
					StructDefine.MemberList.push_back(MemberInfo);
				}
				else if (_stricmp(Member.name(), "GenerateOperations") == 0)
				{
					LoadGenerateOperations(Member, StructDefine.GenerateOperations);
				}
			}
			StructDefineList.push_back(StructDefine);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveEnumDefine(xml_node& Enums,vector<ENUM_DEFINE_INFO>& EnumDefineList)
{
	for(size_t i=0;i<EnumDefineList.size();i++)
	{
		xml_node Enum=Enums.append_child(node_element,"Enum");
		Enum.append_attribute("Name",(LPCTSTR)EnumDefineList[i].Name);
		Enum.append_attribute("ShortName",(LPCTSTR)EnumDefineList[i].ShortName);
		Enum.append_attribute("Flag",(UINT)EnumDefineList[i].Flag);
		Enum.append_attribute("Description",(LPCTSTR)EnumDefineList[i].Description);
				

		for(size_t j=0;j<EnumDefineList[i].MemberList.size();j++)
		{
			xml_node Member=Enum.append_child(node_element,"Member");
			Member.append_attribute("Name",(LPCTSTR)EnumDefineList[i].MemberList[j].Name);
			Member.append_attribute("Value",(LPCTSTR)EnumDefineList[i].MemberList[j].Value);
			Member.append_attribute("StrValue",(LPCTSTR)EnumDefineList[i].MemberList[j].StrValue);
			Member.append_attribute("Description",(LPCTSTR)EnumDefineList[i].MemberList[j].Description);
			
		}
	}


	return true;
}
bool CCallInterfaceMakerDlg::LoadEnumDefine(xml_node& Enums,vector<ENUM_DEFINE_INFO>& EnumDefineList)
{
	for(UINT i=0;i<Enums.children();i++)
	{
		xml_node Enum=Enums.child(i);
		if(_stricmp(Enum.name(),"Enum")==0)
		{
			ENUM_DEFINE_INFO EnumDefine;
			EnumDefine.Name = (LPCTSTR)Enum.attribute("Name").getvalue();
			EnumDefine.ShortName = (LPCTSTR)Enum.attribute("ShortName").getvalue();
			EnumDefine.Flag=(UINT)Enum.attribute("Flag");
			EnumDefine.Description = ((LPCTSTR)Enum.attribute("Description").getvalue());
			



			for(UINT j=0;j<Enum.children();j++)
			{
				xml_node Member=Enum.child(j);
				if(_stricmp(Member.name(),"Member")==0)
				{
					ENUM_MEMBER_INFO MemberInfo;
					MemberInfo.Name = (LPCTSTR)Member.attribute("Name").getvalue();
					MemberInfo.Value = ((LPCTSTR)Member.attribute("Value").getvalue());
					MemberInfo.StrValue = ((LPCTSTR)Member.attribute("StrValue").getvalue());
					MemberInfo.Description = ((LPCTSTR)Member.attribute("Description").getvalue());
					
					EnumDefine.MemberList.push_back(MemberInfo);
				}
				
			}
			EnumDefineList.push_back(EnumDefine);
		}		
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveConstDefine(xml_node& Consts,vector<CONST_DEFINE_INFO>& ConstDefineList)
{
	for(size_t i=0;i<ConstDefineList.size();i++)
	{
		xml_node Const=Consts.append_child(node_element,"Const");
		Const.append_attribute("Name",(LPCTSTR)ConstDefineList[i].Name);
		Const.append_attribute("Type",(LPCTSTR)ConstDefineList[i].Type);
		Const.append_attribute("Value",(LPCTSTR)ConstDefineList[i].Value);
		Const.append_attribute("Description",(LPCTSTR)ConstDefineList[i].Description);
	}


	return true;
}
bool CCallInterfaceMakerDlg::LoadConstDefine(xml_node& Consts,vector<CONST_DEFINE_INFO>& ConstDefineList)
{
	for(UINT i=0;i<Consts.children();i++)
	{
		xml_node Enum=Consts.child(i);
		if(_stricmp(Enum.name(),"Const")==0)
		{
			CONST_DEFINE_INFO ConstDefine;
			ConstDefine.Name = (LPCTSTR)Enum.attribute("Name").getvalue();
			ConstDefine.Type = (LPCTSTR)Enum.attribute("Type").getvalue();
			ConstDefine.Value = ((LPCTSTR)Enum.attribute("Value").getvalue());
			ConstDefine.Description = ((LPCTSTR)Enum.attribute("Description").getvalue());

			ConstDefineList.push_back(ConstDefine);
		}		
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveDataObjectModifyFlag(xml_node& ModifyFlags,vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags)
{
	for(size_t i=0;i<DataObjectModifyFlags.size();i++)
	{
		xml_node ModifyFlag=ModifyFlags.append_child(node_element,"ModifyFlag");
		ModifyFlag.append_attribute("Name",(LPCTSTR)DataObjectModifyFlags[i].Name);
		for(size_t j=0;j<DataObjectModifyFlags[i].ModifyFlag.size();j++)
		{
			xml_node Unit=ModifyFlag.append_child(node_element,"Unit");
			Unit.append_attribute("ClassName",(LPCTSTR)DataObjectModifyFlags[i].ModifyFlag[j].ClassName);
			for(size_t k=0;k<DataObjectModifyFlags[i].ModifyFlag[j].Members.size();k++)
			{
				xml_node Flag=Unit.append_child(node_element,"Flag");
				Flag.append_attribute("Name",(LPCTSTR)DataObjectModifyFlags[i].ModifyFlag[j].Members[k]);
			}
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::LoadDataObjectModifyFlag(xml_node& ModifyFlags,vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags)
{
	for(UINT i=0;i<ModifyFlags.children();i++)
	{
		xml_node ModifyFlag=ModifyFlags.child(i);
		if(_stricmp(ModifyFlag.name(),"ModifyFlag")==0)
		{
			DATA_OBJECT_MODIFY_FLAG MF;
			MF.Name = (LPCTSTR)ModifyFlag.attribute("Name").getvalue();
			for(UINT j=0;j<ModifyFlag.children();j++)
			{
				xml_node Unit=ModifyFlag.child(j);
				if(_stricmp(Unit.name(),"Unit")==0)
				{
					MODIFY_FLAG_UNIT MFU;
					MFU.ClassName = (LPCTSTR)Unit.attribute("ClassName").getvalue();
					for(UINT k=0;k<Unit.children();k++)
					{
						xml_node Flag=Unit.child(k);
						if(_stricmp(Flag.name(),"Flag")==0)
						{
							MFU.Members.push_back((LPCTSTR)Flag.attribute("Name").getvalue());
						}
					}
					MF.ModifyFlag.push_back(MFU);
				}
			}
			DataObjectModifyFlags.push_back(MF);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveGenerateOperations(xml_node& XmlNode,GENERATE_OPERATIONS_INFO& GenerateOperations)
{
	XmlNode.append_attribute("PackOperation",(LPCTSTR)GenerateOperations.PackOperation);
	XmlNode.append_attribute("UnpackOperation",(LPCTSTR)GenerateOperations.UnpackOperation);	
	XmlNode.append_attribute("SizeCaculateOperation",(LPCTSTR)GenerateOperations.SizeCaculateOperation);
	XmlNode.append_attribute("ToXMLOperation", (LPCTSTR)GenerateOperations.ToXMLOperation);
	XmlNode.append_attribute("FromXMLOperation", (LPCTSTR)GenerateOperations.FromXMLOperation);
	XmlNode.append_attribute("ToJsonOperation", (LPCTSTR)GenerateOperations.ToJsonOperation);
	XmlNode.append_attribute("FromJsonOperation", (LPCTSTR)GenerateOperations.FromJsonOperation);
	XmlNode.append_attribute("ReferenceDefine",(LPCTSTR)GenerateOperations.ReferenceDefine);
	XmlNode.append_attribute("ReferenceUse",(LPCTSTR)GenerateOperations.ReferenceUse);
	XmlNode.append_attribute("VariableDefine",(LPCTSTR)GenerateOperations.VariableDefine);
	XmlNode.append_attribute("InitOperation",(LPCTSTR)GenerateOperations.InitOperation);
	XmlNode.append_attribute("CloneOperation",(LPCTSTR)GenerateOperations.CloneOperation);
	XmlNode.append_attribute("GetMethodDeclare",(LPCTSTR)GenerateOperations.GetMethodDeclare);
	XmlNode.append_attribute("GetMethodDefine",(LPCTSTR)GenerateOperations.GetMethodDefine);
	XmlNode.append_attribute("SetMethodDeclare",(LPCTSTR)GenerateOperations.SetMethodDeclare);
	XmlNode.append_attribute("SetMethodDefine",(LPCTSTR)GenerateOperations.SetMethodDefine);
	XmlNode.append_attribute("IndexOperation",(LPCTSTR)GenerateOperations.IndexOperation);
	XmlNode.append_attribute("ConstIndexOperation",(LPCTSTR)GenerateOperations.ConstIndexOperation);

	return true;
}

bool CCallInterfaceMakerDlg::LoadGenerateOperations(xml_node& XmlNode,GENERATE_OPERATIONS_INFO& GenerateOperations)
{
	GenerateOperations.PackOperation = ((LPCTSTR)XmlNode.attribute("PackOperation").getvalue());
	GenerateOperations.UnpackOperation = ((LPCTSTR)XmlNode.attribute("UnpackOperation").getvalue());
	GenerateOperations.SizeCaculateOperation = ((LPCTSTR)XmlNode.attribute("SizeCaculateOperation").getvalue());
	GenerateOperations.ToXMLOperation = ((LPCTSTR)XmlNode.attribute("ToXMLOperation").getvalue());
	GenerateOperations.FromXMLOperation = ((LPCTSTR)XmlNode.attribute("FromXMLOperation").getvalue());
	GenerateOperations.ToJsonOperation = ((LPCTSTR)XmlNode.attribute("ToJsonOperation").getvalue());
	GenerateOperations.FromJsonOperation = ((LPCTSTR)XmlNode.attribute("FromJsonOperation").getvalue());
	GenerateOperations.ReferenceDefine = ((LPCTSTR)XmlNode.attribute("ReferenceDefine").getvalue());
	GenerateOperations.ReferenceUse = ((LPCTSTR)XmlNode.attribute("ReferenceUse").getvalue());
	GenerateOperations.VariableDefine = ((LPCTSTR)XmlNode.attribute("VariableDefine").getvalue());
	GenerateOperations.InitOperation = ((LPCTSTR)XmlNode.attribute("InitOperation").getvalue());
	GenerateOperations.CloneOperation = ((LPCTSTR)XmlNode.attribute("CloneOperation").getvalue());
	GenerateOperations.GetMethodDeclare = ((LPCTSTR)XmlNode.attribute("GetMethodDeclare").getvalue());
	GenerateOperations.GetMethodDefine = ((LPCTSTR)XmlNode.attribute("GetMethodDefine").getvalue());
	GenerateOperations.SetMethodDeclare = ((LPCTSTR)XmlNode.attribute("SetMethodDeclare").getvalue());
	GenerateOperations.SetMethodDefine = ((LPCTSTR)XmlNode.attribute("SetMethodDefine").getvalue());
	GenerateOperations.IndexOperation = ((LPCTSTR)XmlNode.attribute("IndexOperation").getvalue());
	GenerateOperations.ConstIndexOperation = ((LPCTSTR)XmlNode.attribute("ConstIndexOperation").getvalue());

	return true;
}

//CString CCallInterfaceMakerDlg::EncodeString(CString Str)
//{
//	Str.Replace("&","&amp;");
//	Str.Replace("<","&lt;");
//	Str.Replace(">","&gt;");
//	Str.Replace("\"","&quot;");
//	Str.Replace("\r","&rt;");
//	Str.Replace("\n","&nl;");
//	Str.Replace("	","&tab;");
//	return Str;
//}
//CString CCallInterfaceMakerDlg::DecodeString(CString Str)
//{
//	Str.Replace("&amp;","&");
//	Str.Replace("&lt;","<");
//	Str.Replace("&gt;",">");
//	Str.Replace("&quot;","\"");
//	Str.Replace("&rt;","\r");
//	Str.Replace("&nl;","\n");
//	Str.Replace("&tab;","	");
//	return Str;
//}

void CCallInterfaceMakerDlg::WriteStringToFile(CFile& OutputFile, CString& Str)
{
	if (m_UTF8Export)
	{
		size_t Len = AnsiToUTF8(Str, Str.GetLength(), NULL, 0);
		char * OutBuff = new char[Len];
		AnsiToUTF8(Str, Str.GetLength(), OutBuff, Len);
		UINT BomHeader = BMT_UTF_8;
		OutputFile.Write(&BomHeader, 3);
		OutputFile.Write(OutBuff, Len);
		delete[] OutBuff;
	}
	else
	{
		OutputFile.Write((LPCTSTR)Str, Str.GetLength());
	}
}



bool CCallInterfaceMakerDlg::EndWith(CString& Str, CString EndStr)
{
	if (Str.GetLength() >= EndStr.GetLength())
	{
		return strcmp((LPCTSTR)Str + (Str.GetLength() - EndStr.GetLength()), EndStr) == 0;
	}
	return false;
}

void CCallInterfaceMakerDlg::SortStructForExport(const vector<STRUCT_DEFINE_INFO>& SrcStructList, vector<STRUCT_DEFINE_INFO>& DestStructList)
{
	for (size_t s = 0; s < SrcStructList.size(); s++)
	{
		vector<CString> DependList;		
		GetStructDepends(SrcStructList, SrcStructList[s], DependList, 0);
		for (int i = (int)DependList.size() - 1; i >= 0; i--)
		{
			for (size_t j = 0; j < SrcStructList.size(); j++)
			{
				if (SrcStructList[j].Name == DependList[i])
				{
					AddUnique(DestStructList, SrcStructList[j]);
					break;
				}
			}
		}
		AddUnique(DestStructList, SrcStructList[s]);
	}
}

void CCallInterfaceMakerDlg::GetStructDepends(const vector<STRUCT_DEFINE_INFO>& StructList, const STRUCT_DEFINE_INFO& StructInfo, vector<CString>& DependList, int Depth)
{
	if (Depth > 32)
	{
		//AfxMessageBox("结构依赖搜索层数过多，可能有循环依赖");
		return;
	}
	if (!StructInfo.BaseStruct.IsEmpty())
	{
		TYPE_DEFINE * pType = FindVarType(StructInfo.BaseStruct);
		if (pType)
		{
			if (pType->Flag&TYPE_DEFINE_FLAG_STRUCT)
			{
				AddUnique(DependList, pType->Name);
				//找到依赖结构的信息
				for (size_t j = 0; j < StructList.size(); j++)
				{
					if (StructList[j].Name == pType->Name)
					{
						GetStructDepends(StructList, StructList[j], DependList, Depth + 1);
						break;
					}
				}
			}
		}
	}
	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		TYPE_DEFINE * pType = FindVarType(StructInfo.MemberList[i].Type);
		if (pType)
		{
			if (pType->Flag&TYPE_DEFINE_FLAG_STRUCT)
			{
				AddUnique(DependList, pType->Name);
				//找到依赖结构的信息
				for (size_t j = 0; j < StructList.size(); j++)
				{
					if (StructList[j].Name == pType->Name)
					{
						GetStructDepends(StructList, StructList[j], DependList, Depth + 1);
						break;
					}
				}
			}
		}
	}
}

void CCallInterfaceMakerDlg::AddUnique(vector<STRUCT_DEFINE_INFO>& StructList, const STRUCT_DEFINE_INFO& StructInfo)
{
	for (size_t i = 0; i < StructList.size(); i++)
	{
		if (StructList[i].Name == StructInfo.Name)
			return;
	}
	StructList.push_back(StructInfo);
}

void CCallInterfaceMakerDlg::SortDefinesForExport(const vector<BASE_DATA_STRUCT_DEFINE_LIST *>& SrcList, vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DestList)
{
	for (size_t i = 0; i < SrcList.size(); i++)
	{
		if (SrcList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			const STRUCT_DEFINE_LIST * pStructDefine = (const STRUCT_DEFINE_LIST *)SrcList[i];
			vector<CString> DependList;
			GetDefineDepends(SrcList, pStructDefine, DependList, 0);
			for (int i = (int)DependList.size() - 1; i >= 0; i--)
			{
				for (size_t j = 0; j < SrcList.size(); j++)
				{
					if (SrcList[j]->ListType == DATA_STRUCT_STRUCT && MakeDefineName((const STRUCT_DEFINE_LIST *)SrcList[j]) == DependList[i])
					{
						AddUnique(DestList, SrcList[j]);
						break;
					}
				}
			}
		}
		AddUnique(DestList, SrcList[i]);		
	}
}

void CCallInterfaceMakerDlg::GetDefineDepends(const vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DefineList, const STRUCT_DEFINE_LIST * pStructDefine, vector<CString>& DependList, int Depth)
{
	if (Depth > 32)
	{
		//AfxMessageBox("数据定义依赖搜索层数过多，可能有循环依赖");
		return;
	}

	vector<CString> StructDependList;
	GetStructForeignDepends(pStructDefine->StructList, StructDependList);

	for (size_t i = 0; i < DefineList.size(); i++)
	{
		if (DefineList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			const STRUCT_DEFINE_LIST * pDefine = (const STRUCT_DEFINE_LIST *)DefineList[i];
			if (HaveStructs(pDefine, StructDependList))
			{
				AddUnique(DependList, MakeDefineName(pDefine));
				GetDefineDepends(DefineList, pDefine, DependList, Depth + 1);
			}				
		}		
	}
}
void CCallInterfaceMakerDlg::GetStructForeignDepends(const vector<STRUCT_DEFINE_INFO>& StructList, vector<CString>& DependList)
{
	for (size_t s = 0; s < StructList.size(); s++)
	{
		const STRUCT_DEFINE_INFO& StructInfo = StructList[s];

		if (!StructInfo.BaseStruct.IsEmpty())
		{
			TYPE_DEFINE * pType = FindVarType(StructInfo.BaseStruct);
			if (pType)
			{
				if (pType->Flag&TYPE_DEFINE_FLAG_STRUCT)
				{
					bool IsExist = false;
					for (size_t j = 0; j < StructList.size(); j++)
					{
						if (StructList[j].Name == pType->Name)
						{
							IsExist = true;
							break;
						}
					}
					if (!IsExist)
						AddUnique(DependList, pType->Name);
				}
			}
		}
		for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
		{
			TYPE_DEFINE * pType = FindVarType(StructInfo.MemberList[i].Type);
			if (pType)
			{
				if (pType->Flag&TYPE_DEFINE_FLAG_STRUCT)
				{
					bool IsExist = false;
					for (size_t j = 0; j < StructList.size(); j++)
					{
						if (StructList[j].Name == pType->Name)
						{
							IsExist = true;
							break;
						}
					}
					if (!IsExist)
						AddUnique(DependList,pType->Name);
				}
			}
		}
	}
	
}

void CCallInterfaceMakerDlg::AddUnique(vector<CString>& NameList, const CString& Name)
{
	for (int i = (int)NameList.size() - 1; i >= 0; i--)
	{
		if (NameList[i] == Name)
		{
			NameList.erase(NameList.begin() + i);
		}
	}
	//for (size_t i = 0; i < NameList.size(); i++)
	//{
	//	if (NameList[i] == Name)
	//		return;
	//}
	NameList.push_back(Name);
}

void CCallInterfaceMakerDlg::AddUnique(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DefineList, BASE_DATA_STRUCT_DEFINE_LIST * pDefine)
{
	for (size_t i = 0; i < DefineList.size(); i++)
	{
		if (DefineList[i]->ListType == pDefine->ListType && DefineList[i] == pDefine)
			return;
	}
	DefineList.push_back(pDefine);
}

bool CCallInterfaceMakerDlg::HaveStructs(const STRUCT_DEFINE_LIST * pStructDefine, vector<CString>& StructNameList)
{
	for (size_t i = 0; i < pStructDefine->StructList.size(); i++)
	{
		for (size_t j = 0; j < StructNameList.size(); j++)
		{
			if (pStructDefine->StructList[i].Name == StructNameList[j])
				return true;
		}		
	}
	return false;
}

CString CCallInterfaceMakerDlg::MakeDefineName(const STRUCT_DEFINE_LIST * pStructDefine)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(pStructDefine->ModuleID);
	if (pModuleInfo)
	{
		return pModuleInfo->Name + ":" + pStructDefine->ListName;
	}
	return pStructDefine->ListName;
}

void CCallInterfaceMakerDlg::SortModuleForExport(const vector<MODULE_DEFINE_INFO *>& SrcList, vector<MODULE_DEFINE_INFO *>& DestList)
{
	for (size_t i = 0; i < SrcList.size(); i++)
	{
		
		const MODULE_DEFINE_INFO * pModuleInfo = (const MODULE_DEFINE_INFO *)SrcList[i];
		vector<CString> DependList;
		GetModuleDepends(SrcList, pModuleInfo, DependList, 0);
		for (int k = (int)DependList.size() - 1; k >= 0; k--)
		{
			for (size_t j = 0; j < SrcList.size(); j++)
			{
				if (SrcList[j]->Name == DependList[k])
				{
					AddUnique(DestList, SrcList[j]);
					break;
				}
			}
		}		
		AddUnique(DestList, SrcList[i]);
	}
}
void CCallInterfaceMakerDlg::GetModuleDepends(const vector<MODULE_DEFINE_INFO *>& ModuleList, const MODULE_DEFINE_INFO * pModuleInfo, vector<CString>& DependList, int Depth)
{
	if (Depth > 32)
	{
		//AfxMessageBox("数据定义依赖搜索层数过多，可能有循环依赖");
		return;
	}
	vector<CString> DefineDependList;
	const vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DefineList = pModuleInfo->DataStructDefineList;
	for (size_t i = 0; i < DefineList.size(); i++)
	{
		if (DefineList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			const STRUCT_DEFINE_LIST * pStructDefine = (const STRUCT_DEFINE_LIST *)DefineList[i];			
			GetDefineDepends(m_AllDataStructList, pStructDefine, DefineDependList, 0);
		}
	}

	for (size_t i = 0; i < DefineList.size(); i++)
	{
		if (DefineList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			const STRUCT_DEFINE_LIST * pStructDefine = (const STRUCT_DEFINE_LIST *)DefineList[i];
			for (int j = (int)DefineDependList.size() - 1; j >= 0; j--)
			{
				if (MakeDefineName(pStructDefine) == DefineDependList[j])
				{
					DefineDependList.erase(DefineDependList.begin() + j);
				}
			}
		}
	}
	
	for (size_t i = 0; i < ModuleList.size(); i++)
	{
		if (HaveDefines(ModuleList[i], DefineDependList))
		{
			GetModuleDepends(ModuleList, ModuleList[i], DependList, Depth + 1);
			AddUnique(DependList, ModuleList[i]->Name);
		}
	}

}

void CCallInterfaceMakerDlg::AddUnique(vector<MODULE_DEFINE_INFO *>& ModuleList, MODULE_DEFINE_INFO * pModuleInfo)
{
	for (size_t i = 0; i < ModuleList.size(); i++)
	{
		if (ModuleList[i] == pModuleInfo)
			return;
	}
	ModuleList.push_back(pModuleInfo);
}

bool CCallInterfaceMakerDlg::HaveDefines(const MODULE_DEFINE_INFO * pModuleInfo, vector<CString>& DefineNameList)
{
	for (size_t i = 0; i < pModuleInfo->DataStructDefineList.size(); i++)
	{
		for (size_t j = 0; j < DefineNameList.size(); j++)
		{
			if (pModuleInfo->DataStructDefineList[i]->ListType == DATA_STRUCT_STRUCT)
			{
				const STRUCT_DEFINE_LIST * pStructDefine = (const STRUCT_DEFINE_LIST *)pModuleInfo->DataStructDefineList[i];
				if (MakeDefineName(pStructDefine) == DefineNameList[j])
					return true;
			}
		}
	}
	return false;
}

void CCallInterfaceMakerDlg::SaveInterfaceSelections()
{
	m_RunEnvInfo.SelectedInterfaces.clear();

	for (int i = 0; i < m_lvInterfaceList.GetItemCount(); i++)
	{
		if (m_lvInterfaceList.GetCheck(i))
		{
			m_RunEnvInfo.SelectedInterfaces.push_back(m_lvInterfaceList.GetItemText(i, 0));
		}
	}
}

void CCallInterfaceMakerDlg::RestoreInterfaceSelections()
{
	
	for (int j = 0; j < m_lvInterfaceList.GetItemCount(); j++)
	{
		m_lvInterfaceList.SetCheck(j, FALSE);
		for (size_t i = 0; i < m_RunEnvInfo.SelectedInterfaces.size(); i++)
		{
			if (m_lvInterfaceList.GetItemText(j, 0) == m_RunEnvInfo.SelectedInterfaces[i])
			{
				m_lvInterfaceList.SetCheck(j, TRUE);
				break;
			}
		}		
	}
}


bool CCallInterfaceMakerDlg::ConvertOldInterfaceDefines(INTERFANCE_DEFINES_OLD& OldModuleInfo, MODULE_DEFINE_INFO& ModuleInfo)
{
	int ErrorCount = 0;
	int WarningCount = 0;
	ModuleInfo.Name = OldModuleInfo.Name;
	ModuleInfo.IDSeed = OldModuleInfo.InterfaceIDSeed;
	ModuleInfo.InterfacePrefix = OldModuleInfo.InterfacePrefix;
	ModuleInfo.InterfacePostFix = OldModuleInfo.InterfacePostFix;
	for (size_t i = 0; i < OldModuleInfo.InterfaceList.size(); i++)
	{
		CALLER_INTERFACE_OLD& OldInterfaceInfo = OldModuleInfo.InterfaceList[i];
		CALLER_INTERFACE InterfaceInfo;
		if (!EndWith(OldInterfaceInfo.Name, "Ack"))
		{
			InterfaceInfo.Name = OldInterfaceInfo.Name;
			InterfaceInfo.ID = OldInterfaceInfo.ID;
			InterfaceInfo.IDSeed = OldInterfaceInfo.IDSeed;
			InterfaceInfo.Description = OldInterfaceInfo.Description;

			InterfaceInfo.MethodList.resize(OldInterfaceInfo.MethodList.size());
			for (size_t j = 0; j < OldInterfaceInfo.MethodList.size(); j++)
			{
				INTERFACE_METHOD_OLD& OldMethodInfo = OldInterfaceInfo.MethodList[j];
				INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];
				
				MethodInfo.Name = OldMethodInfo.Name;
				MethodInfo.Type = INTERFACE_METHOD_TYPE_CALL;
				MethodInfo.CallParamList = OldMethodInfo.ParamList;				
				MethodInfo.ID = OldMethodInfo.ID;
				MethodInfo.Flag = OldMethodInfo.Flag;
				MethodInfo.ParamIDSeed = OldMethodInfo.ParamIDSeed;
				MethodInfo.Description = OldMethodInfo.Description;

				INTERFACE_METHOD_OLD * pAckMethodInfo = FindInterfaceMethod(OldModuleInfo, OldInterfaceInfo.Name, OldMethodInfo.Name, true);
				if (pAckMethodInfo)
				{
					MethodInfo.AckParamList = pAckMethodInfo->ParamList;					
				}
				else
				{
					MethodInfo.Type = INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT;
					CString Msg;
					Msg.Format("无法找到接口%s方法%s对应的回调方法", OldInterfaceInfo.Name, OldMethodInfo.Name);
					PrintImportantLog("ConvertWarnning", "%s", Msg);
					WarningCount++;
				}

				
			}

			CALLER_INTERFACE_OLD * pAckInterfaceInfo = FindInterface(OldModuleInfo, OldInterfaceInfo.Name, true);
			if (pAckInterfaceInfo)
			{
				for (size_t j = 0; j < pAckInterfaceInfo->MethodList.size(); j++)
				{
					INTERFACE_METHOD_OLD& OldMethodInfo = pAckInterfaceInfo->MethodList[j];
					if (EndWith(OldMethodInfo.Name, "Notify"))
					{
						INTERFACE_METHOD MethodInfo;
						MethodInfo.Name = OldMethodInfo.Name;
						MethodInfo.Type = INTERFACE_METHOD_TYPE_NOTIFY;
						MethodInfo.CallParamList = OldMethodInfo.ParamList;
						MethodInfo.ID = OldMethodInfo.ID;
						MethodInfo.Flag = OldMethodInfo.Flag;
						MethodInfo.ParamIDSeed = OldMethodInfo.ParamIDSeed;
						MethodInfo.Description = OldMethodInfo.Description;

						InterfaceInfo.MethodList.push_back(MethodInfo);
					}
					else
					{
						CString MethodName = OldMethodInfo.Name;
						if (EndWith(MethodName, "Ack"))
						{
							MethodName = MethodName.Left(MethodName.GetLength() - 3);
						}

						INTERFACE_METHOD_OLD * pCallMethod = FindMethod(OldInterfaceInfo.MethodList, MethodName, false);
						if (pCallMethod == NULL)
						{
							CString Msg;
							Msg.Format("无法找到接口%s回调方法%s对应的调用方法", OldInterfaceInfo.Name, OldMethodInfo.Name);
							PrintImportantLog("ConvertError", "%s", Msg);
							ErrorCount++;
						}
					}
				}
			}
			else
			{
				CString Msg;
				Msg.Format("无法找到接口%s对应的回调接口", OldInterfaceInfo.Name);
				PrintImportantLog("ConvertError", "%s", Msg);
				ErrorCount++;
			}
			ModuleInfo.InterfaceList.push_back(InterfaceInfo);
		}
		
	}
	ModuleInfo.DataObjectModifyFlags = OldModuleInfo.DataStructDefines.DataObjectModifyFlags;
	ModuleInfo.DataStructDefineList.resize(OldModuleInfo.DataStructDefines.DataStructDefineList.size());
	for (size_t i = 0; i < OldModuleInfo.DataStructDefines.DataStructDefineList.size(); i++)
	{
		BASE_DATA_STRUCT_DEFINE_LIST * pBaseDefine = OldModuleInfo.DataStructDefines.DataStructDefineList[i];

		switch (pBaseDefine->ListType)
		{
		case DATA_STRUCT_CONST:
		case DATA_STRUCT_ENUM:
			ModuleInfo.DataStructDefineList[i] = pBaseDefine;
			OldModuleInfo.DataStructDefines.DataStructDefineList[i] = NULL;
			break;
		case DATA_STRUCT_STRUCT:
			{
				STRUCT_DEFINE_LIST_OLD * pOldStructList = (STRUCT_DEFINE_LIST_OLD *)pBaseDefine;
				STRUCT_DEFINE_LIST * pStructList = new STRUCT_DEFINE_LIST();
				pStructList->ListType = pOldStructList->ListType;
				pStructList->ListName = pOldStructList->ListName;
				pStructList->IgnoreOnExport = pOldStructList->IgnoreOnExport;
				pStructList->StructList.resize(pOldStructList->StructList.size());
				for (size_t j = 0; j < pOldStructList->StructList.size(); j++)
				{
					STRUCT_DEFINE_INFO_OLD& OldStructInfo = pOldStructList->StructList[j];
					STRUCT_DEFINE_INFO& StructInfo = pStructList->StructList[j];
					StructInfo.Name = OldStructInfo.Name;
					StructInfo.ShortName = OldStructInfo.ShortName;
					StructInfo.BaseStruct = OldStructInfo.BaseStruct;
					StructInfo.MemberList = OldStructInfo.MemberList;
					StructInfo.Description = OldStructInfo.Description;
					StructInfo.Flag = (OldStructInfo.IsDataObject ? STRUCT_FLAG_IS_DATA_OBJECT : 0) |
						(OldStructInfo.DeclareLater ? STRUCT_FLAG_DECLARE_LATER : 0);
					StructInfo.ObjectID = OldStructInfo.ObjectID;
					StructInfo.IDSeed = OldStructInfo.IDSeed;
					StructInfo.GenerateOperations = OldStructInfo.GenerateOperations;
				}
				ModuleInfo.DataStructDefineList[i] = pStructList;
			}
			break;
		default:
			{
				CString Msg;
				Msg.Format("数据结构[%s]的类型[%d]错误", pBaseDefine->ListName, pBaseDefine->ListType);
				PrintImportantLog("ConvertError", "%s", Msg);
				ErrorCount++;
			}
		}
	}
	if (WarningCount || ErrorCount)
	{
		CString Msg;
		Msg.Format("从旧版转换过程中发现%d个警告,%d错误", WarningCount, ErrorCount);
		AfxMessageBox(Msg);
	}

	return true;
}

INTERFACE_METHOD_OLD * CCallInterfaceMakerDlg::FindInterfaceMethod(INTERFANCE_DEFINES_OLD& OldInterfaceDefines, CString InterfaceName, CString MethodName, bool FindAck)
{
	if (FindAck)
		InterfaceName += "Ack";
	
	for (size_t i = 0; i < OldInterfaceDefines.InterfaceList.size(); i++)
	{
		CALLER_INTERFACE_OLD& OldInterfaceInfo = OldInterfaceDefines.InterfaceList[i];
		if (OldInterfaceInfo.Name == InterfaceName)
		{
			INTERFACE_METHOD_OLD * pMethodInfo = FindMethod(OldInterfaceInfo.MethodList, MethodName, FindAck);
			if (pMethodInfo)
				return pMethodInfo;			
		}
	}
	
	return NULL;
}

CALLER_INTERFACE_OLD * CCallInterfaceMakerDlg::FindInterface(INTERFANCE_DEFINES_OLD& OldInterfaceDefines, CString InterfaceName, bool FindAck)
{
	if (FindAck)
		InterfaceName += "Ack";
	for (size_t i = 0; i < OldInterfaceDefines.InterfaceList.size(); i++)
	{
		CALLER_INTERFACE_OLD& OldInterfaceInfo = OldInterfaceDefines.InterfaceList[i];
		if (OldInterfaceInfo.Name == InterfaceName)
		{
			return &OldInterfaceInfo;
		}
	}
	return NULL;
}

INTERFACE_METHOD_OLD * CCallInterfaceMakerDlg::FindMethod(vector<INTERFACE_METHOD_OLD>& OldMethodList, CString MethodName, bool FindAck)
{
	if (FindAck)
		MethodName += "Ack";
	for (size_t j = 0; j < OldMethodList.size(); j++)
	{
		INTERFACE_METHOD_OLD& OldMethodInfo = OldMethodList[j];
		if (OldMethodInfo.Name == MethodName)
			return &OldMethodInfo;
	}
	return NULL;
}

bool CCallInterfaceMakerDlg::Export(LPCTSTR ExportDir)
{
	if (m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport)
	{
		CString FileName;

		FileName.Format("%sInterfaces.%s", m_MainModule.Name, m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt);
		ExportInterfaceHeader(m_AllInterfaceList, ExportDir, FileName);
	}

	if (m_InterfaceConfig.DataStructHeaderExportConfig.IsExport)
	{
		CString FileName;

		FileName.Format("%sDataStructs.%s", m_MainModule.Name, m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt);
		ExportDataStructDefineHeader(m_AllDataStructList, ExportDir, FileName, m_MainModule.Name);

	}
	CString Temp;

	Temp.Format("%sSystemEnums.%s", m_MainModule.Name, m_InterfaceConfig.EnumExportConfig.ExportExt);
	ExportSystemEnums(ExportDir, Temp, m_AllModuleList);

	ExportDataStructDefines(m_AllDataStructList, ExportDir, m_MainModule.Name);
	ExportDataObject(m_AllDataStructList, ExportDir, m_MainModule.Name);
	ExportInterfaces(m_AllInterfaceList, ExportDir, INTERFACE_METHOD_EXPORT_TYPE_CALL, m_MainModule.Name);
	ExportInterfaces(m_AllInterfaceList, ExportDir, INTERFACE_METHOD_EXPORT_TYPE_ACK, m_MainModule.Name);

	ExportDataObjectModifyFlags(m_MainModule.DataObjectModifyFlags, ExportDir, m_MainModule.Name);
	return true;
}

bool CCallInterfaceMakerDlg::ExportDataStructDefineHeader(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString Source=m_InterfaceHeaderTemplate;

		CString Includes;
		CString Temp;			

		Temp.Format("#include \"%sSystemEnums.%s\"\r\n", SpaceName, m_InterfaceConfig.EnumExportConfig.ExportExt);

		Includes+=Temp;

		vector<BASE_DATA_STRUCT_DEFINE_LIST *>	DataStructDefineList;
		SortDefinesForExport(DataStructList, DataStructDefineList);
		if (DataStructList.size() != DataStructDefineList.size())
			AfxMessageBox("数据定义排序错误");

		for(size_t i=0;i<DataStructDefineList.size();i++)
		{
			if (DataStructDefineList[i]->IgnoreOnExport)
				continue;

			CString ModuleName = GetModuleName(DataStructDefineList[i]->ModuleID);
			switch(DataStructDefineList[i]->ListType)
			{
			case DATA_STRUCT_CONST:
				{
					CONST_DEFINE_LIST * pConstList=(CONST_DEFINE_LIST *)DataStructDefineList[i];

					Temp.Format("#include \"%s%sConsts.%s\"\r\n", ModuleName, pConstList->ListName, m_InterfaceConfig.ConstExportConfig.ExportExt);

					Includes+=Temp;
				}
				break;
			case DATA_STRUCT_ENUM:
				{
					ENUM_DEFINE_LIST * pEnumList=(ENUM_DEFINE_LIST *)DataStructDefineList[i];

					Temp.Format("#include \"%s%sEnums.%s\"\r\n", ModuleName, pEnumList->ListName, m_InterfaceConfig.EnumExportConfig.ExportExt);
					Includes+=Temp;
				}
				break;
			case DATA_STRUCT_STRUCT:
				{
					STRUCT_DEFINE_LIST * pStructList=(STRUCT_DEFINE_LIST *)DataStructDefineList[i];

					Temp.Format("#include \"%s%sStructs.%s\"\r\n", ModuleName, pStructList->ListName, m_InterfaceConfig.StructExportConfig.ExportExt);
					Includes+=Temp;

					vector<STRUCT_DEFINE_INFO> StructList;
					SortStructForExport(pStructList->StructList, StructList);
					if (pStructList->StructList.size() != StructList.size())
						AfxMessageBox("结构排序错误：" + pStructList->ListName);

					for(size_t j=0;j<StructList.size();j++)
					{
						if (StructList[j].Flag&STRUCT_FLAG_IS_DATA_OBJECT)
						{
							if(m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport)
							{
								Temp.Format("#include \"%s.%s\"\r\n",ClassNameToFileName(StructList[j].Name),
									m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);
								Includes+=Temp;
							}
						}

					}
				}		
				break;
			}	
			
		}		

		if (m_InterfaceConfig.DataObjectModifyFlagsExportConfig.IsExport)
		{
			Temp.Format("#include \"DataObjectModifyFlags.%s\"\r\n", m_InterfaceConfig.DataObjectModifyFlagsExportConfig.ExportExt);
			Includes += Temp;
		}

		Source.Replace("<Prefix>","");
		Source.Replace("<Postfix>","");

		Source.Replace("<Includes>",Includes);

		WriteStringToFile(OutputFile, Source);
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportInterfaceHeader(vector<CALLER_INTERFACE *>& InterfaceList, LPCTSTR ExportDir, LPCTSTR FileName)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString ProtocolFile=m_InterfaceHeaderTemplate;
		
		ProtocolFile.Replace("<Prefix>", "");
		ProtocolFile.Replace("<Postfix>", "");
		
		CString Includes;
		CString Temp;	
		
		for (size_t i = 0; i<InterfaceList.size(); i++)
		{
			CALLER_INTERFACE * pInterfaceInfo = InterfaceList[i];
			if (IsListItemChecked(pInterfaceInfo->ModuleID, pInterfaceInfo->ID))
			{
				Temp.Format("#include \"%sInterface.%s\"\r\n", pInterfaceInfo->Name,
					m_InterfaceConfig.InterfaceExportConfig.ExportExt);
				Includes += Temp;
				Temp.Format("#include \"%sMsgCaller.%s\"\r\n", pInterfaceInfo->Name,
					m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
				Includes += Temp;
				Temp.Format("#include \"%sMsgHandler.%s\"\r\n", pInterfaceInfo->Name,
					m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
				Includes += Temp;

				Temp.Format("#include \"%sAckInterface.%s\"\r\n", pInterfaceInfo->Name,
					m_InterfaceConfig.InterfaceExportConfig.ExportExt);
				Includes += Temp;
				Temp.Format("#include \"%sAckMsgCaller.%s\"\r\n", pInterfaceInfo->Name,
					m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
				Includes += Temp;
				Temp.Format("#include \"%sAckMsgHandler.%s\"\r\n", pInterfaceInfo->Name,
					m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
				Includes += Temp;

				Includes += "\r\n";


			}
		}		
		
		ProtocolFile.Replace("<Includes>",Includes);

		WriteStringToFile(OutputFile, ProtocolFile);
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportStructs(LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName, vector<STRUCT_DEFINE_INFO>& StructList)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString DefineHeader=m_StructDefineHeaderTemplate;

		DefineHeader.Replace("<SpaceName>", SpaceName);
	

		CString LineSpace=GetLineSpace(DefineHeader,"<Structs>");
		CString Structs=MakeStructDefines(StructList,LineSpace);
		DefineHeader.Replace("<Structs>",Structs);

		WriteStringToFile(OutputFile, DefineHeader);
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportSystemEnums(LPCTSTR ExportDir, LPCTSTR FileName, vector<MODULE_DEFINE_INFO *>& AllModuleList)
{
	vector<ENUM_DEFINE_INFO> SystemEnumList;

	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name = "MODULE_IDS";
	EnumInfo.MemberList.resize(AllModuleList.size());
	for (size_t i = 0; i < AllModuleList.size(); i++)
	{
		EnumInfo.MemberList[i].Name = "MODULE_ID_" + ClassNameToUpper(AllModuleList[i]->Name);
		EnumInfo.MemberList[i].Value.Format("%d", AllModuleList[i]->ModuleID);
	}	
	SystemEnumList.push_back(EnumInfo);
	

	EnumInfo.Name="GET_MODIFY_FLAG_TYPE";	
	EnumInfo.MemberList.resize(3);
	EnumInfo.MemberList[0].Name="GMFT_COPY";
	EnumInfo.MemberList[0].Value.Empty();
	EnumInfo.MemberList[1].Name="GMFT_AND";
	EnumInfo.MemberList[1].Value.Empty();
	EnumInfo.MemberList[2].Name="GMFT_OR";
	EnumInfo.MemberList[2].Value.Empty();
	SystemEnumList.push_back(EnumInfo);	
	
	

	EnumInfo.MemberList.clear();
	MakeDataObjectSSTIDsEnum(m_AllDataStructList, EnumInfo);
	SystemEnumList.push_back(EnumInfo);

	EnumInfo.MemberList.clear();
	MakeDataObjectFlagIndicesEnum(m_AllDataStructList, EnumInfo);
	SystemEnumList.push_back(EnumInfo);

	for (size_t j = 0; j < AllModuleList.size(); j++)
	{
		if (AllModuleList[j]->InterfaceList.size())
		{
			EnumInfo.Name.Format("%s_INTERFACES", ClassNameToUpper(AllModuleList[j]->Name));
			EnumInfo.MemberList.resize(AllModuleList[j]->InterfaceList.size());
			for (size_t i = 0; i < AllModuleList[j]->InterfaceList.size(); i++)
			{
				EnumInfo.MemberList[i].Name.Format("%s_INTERFACE_%s", ClassNameToUpper(AllModuleList[j]->Name), ClassNameToUpper(AllModuleList[j]->InterfaceList[i].Name));
				EnumInfo.MemberList[i].Value.Format("%d", AllModuleList[j]->InterfaceList[i].ID);
			}
			SystemEnumList.push_back(EnumInfo);
		}
	}	

	return ExportEnums(ExportDir, FileName, SystemEnumList, m_MainModule.Name, "System", true);
}

bool CCallInterfaceMakerDlg::ExportEnums(LPCTSTR ExportDir, LPCTSTR FileName, vector<ENUM_DEFINE_INFO>& EnumList, LPCTSTR SpaceName, LPCTSTR ListName, bool IsSystem)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString EnumDefineHeader;

		if(IsSystem)
			EnumDefineHeader=m_SystemEnumDefineHeaderTemplate;
		else
			EnumDefineHeader=m_EnumDefineHeaderTemplate;

		CString SpaceNameUp = ClassNameToUpper(SpaceName);
		CString ListNameUp=ClassNameToUpper(ListName);

		EnumDefineHeader.Replace("<SpaceName>", SpaceName);

		EnumDefineHeader.Replace("<!SpaceName>",SpaceNameUp);
		EnumDefineHeader.Replace("<!ListName>",ListNameUp);
	

		CString LineSpace=GetLineSpace(EnumDefineHeader,"<Enums>");
		CString Enums=MakeEnumDefines(EnumList,LineSpace);
		EnumDefineHeader.Replace("<Enums>",Enums);

		CString EnumStrValuesName;
		EnumStrValuesName.Format("%s_ENUM_STR_VALUES",ListNameUp);
		EnumDefineHeader.Replace("<EnumStrValuesName>",EnumStrValuesName);

		LineSpace=GetLineSpace(EnumDefineHeader,"<EnumStrValues>");
		CString EnumStrValues=MakeEnumStrValues(EnumList,LineSpace);
		EnumDefineHeader.Replace("<EnumStrValues>",EnumStrValues);

		WriteStringToFile(OutputFile, EnumDefineHeader);
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportConsts(LPCTSTR ExportDir, LPCTSTR FileName, vector<CONST_DEFINE_INFO>& ConstList, LPCTSTR SpaceName, LPCTSTR ListName)
{
	CFile OutputFile;
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		CString ConstDefineHeader=m_ConstDefineHeaderTemplate;

		CString SpaceNameUp = ClassNameToUpper(SpaceName);
		CString ListNameUp=ClassNameToUpper(ListName);

		ConstDefineHeader.Replace("<SpaceName>", SpaceName);

		ConstDefineHeader.Replace("<!SpaceName>",SpaceNameUp);
		ConstDefineHeader.Replace("<!ListName>",ListNameUp);		

		CString LineSpace=GetLineSpace(ConstDefineHeader,"<Consts>");
		CString Consts=MakeConstDefines(ConstList,LineSpace);
		ConstDefineHeader.Replace("<Consts>",Consts);

		WriteStringToFile(OutputFile, ConstDefineHeader);
		OutputFile.Close();
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s",OutputFileName);
		MessageBox(Msg);
	}	
	return false;
}

bool CCallInterfaceMakerDlg::ExportDataStructDefines(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, LPCTSTR ExportDir, LPCTSTR SpaceName)
{
	CString Temp;
	for (size_t i = 0; i<DataStructList.size(); i++)
	{
		if (DataStructList[i]->IgnoreOnExport)
			continue;

		CString ModuleName = GetModuleName(DataStructList[i]->ModuleID);

		switch (DataStructList[i]->ListType)
		{
		case DATA_STRUCT_CONST:
			{
				CONST_DEFINE_LIST * pConstList = (CONST_DEFINE_LIST *)DataStructList[i];

				Temp.Format("%s%sConsts.%s", ModuleName, pConstList->ListName, m_InterfaceConfig.ConstExportConfig.ExportExt);

				ExportConsts(ExportDir, Temp, pConstList->ConstList, SpaceName, pConstList->ListName);
			}
			break;
		case DATA_STRUCT_ENUM:
			{
				ENUM_DEFINE_LIST * pEnumList = (ENUM_DEFINE_LIST *)DataStructList[i];

				Temp.Format("%s%sEnums.%s", ModuleName, pEnumList->ListName, m_InterfaceConfig.EnumExportConfig.ExportExt);

				ExportEnums(ExportDir, Temp, pEnumList->EnumList, SpaceName, pEnumList->ListName, false);
			}
			break;
		case DATA_STRUCT_STRUCT:
			{
				STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)DataStructList[i];

				Temp.Format("%s%sStructs.%s", ModuleName, pStructList->ListName, m_InterfaceConfig.StructExportConfig.ExportExt);

				vector<STRUCT_DEFINE_INFO> StructList;
				SortStructForExport(pStructList->StructList, StructList);
				if (pStructList->StructList.size() != StructList.size())
					AfxMessageBox("结构排序错误：" + pStructList->ListName);
				ExportStructs(ExportDir, Temp, SpaceName, StructList);
			}		
			break;
		}		
	}

	

	return true;
}

bool CCallInterfaceMakerDlg::ExportInterfaces(vector<CALLER_INTERFACE *>& InterfaceList, LPCTSTR ExportDir, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName)
{
	for (size_t i = 0; i<InterfaceList.size(); i++)
	{
		CALLER_INTERFACE& InterfaceInfo = *(InterfaceList[i]);

		if (IsListItemChecked(InterfaceInfo.ModuleID, InterfaceInfo.ID))
		{
			if(m_InterfaceConfig.InterfaceExportConfig.IsExport)
			{
				CFile OutputFile;
				CString OutputFileName;
				CString LineSpace;

				CString InterfaceName;
				if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
					InterfaceName = InterfaceInfo.Name + "Ack";
				else
					InterfaceName = InterfaceInfo.Name;

				OutputFileName.Format("%s\\%sInterface.%s", ExportDir, InterfaceName,
					m_InterfaceConfig.InterfaceExportConfig.ExportExt);
				if(OutputFile.Open(OutputFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
				{			
					CString Interface=m_InterfaceTemplate;

					if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
					{
						ENUM_DEFINE_INFO EnumInfo;

						EnumInfo.Name.Format("%s_METHODS", ClassNameToUpper(InterfaceName));

						EnumInfo.MemberList.resize(InterfaceInfo.MethodList.size());
						for (size_t j = 0; j < InterfaceInfo.MethodList.size(); j++)
						{
							EnumInfo.MemberList[j].Name.Format("METHOD_%s", ClassNameToUpper(InterfaceInfo.MethodList[j].Name));
							EnumInfo.MemberList[j].Value.Format("%d", InterfaceInfo.MethodList[j].ID);
						}

						LineSpace = GetLineSpace(Interface, "<MethodIDEnums>");
						CString MethodIDEnums = MakeEnumDefine(EnumInfo, LineSpace);
						Interface.Replace("<MethodIDEnums>", MethodIDEnums);
					}
					else
					{
						Interface.Replace("<MethodIDEnums>", "");
					}

					Interface.Replace("<SpaceName>", SpaceName);
					Interface.Replace("<InterfaceName>", InterfaceName);
					Interface.Replace("<InterfaceDescription>",InterfaceInfo.Description);
					Interface.Replace("<@InterfaceName>", InterfaceInfo.Name);
					LineSpace=GetLineSpace(Interface,"<Methods>");
					CString Methods = MakeMethods(InterfaceInfo.MethodList, ExportType, LineSpace, true);
					Interface.Replace("<Methods>",Methods);
					LineSpace=GetLineSpace(Interface,"<MethodSSTIDEnumDefine>");
					CString SSTIDDefines = MakeMethodSSTIDEnumDefines(InterfaceInfo, ExportType, LineSpace);
					Interface.Replace("<MethodSSTIDEnumDefine>",SSTIDDefines);

					WriteStringToFile(OutputFile, Interface);
					OutputFile.Close();
				}
				else
				{
					CString Msg;
					Msg.Format("无法打开文件%s",OutputFileName);
					MessageBox(Msg);
					return false;
				}	
			}
			ExportMsgCaller(ExportDir, SpaceName, InterfaceInfo, ExportType);
			ExportMsgHandler(ExportDir, SpaceName, InterfaceInfo, ExportType);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportMsgCaller(LPCTSTR ExportDir, LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType)
{
	CFile HeaderFile,SourceFile;

	CString HeaderFileName,SourceFileName;

	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + "Ack";
	else
		InterfaceName = InterfaceInfo.Name;


	if(m_InterfaceConfig.CallHeaderExportConfig.IsExport)
	{
		HeaderFileName.Format("%s\\%sMsgCaller.%s", ExportDir, InterfaceName,
			m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
		

		if(HeaderFile.Open(HeaderFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			CString Header=m_MsgCallerHeaderTemplate;

			Header.Replace("<SpaceName>", SpaceName);
			Header.Replace("<InterfaceName>", InterfaceName);
			Header.Replace("<@InterfaceName>", InterfaceInfo.Name);
			CString LineSpace=GetLineSpace(Header,"<Methods>");
			CString Methods = MakeMethods(InterfaceInfo.MethodList, ExportType, LineSpace, false);
			Header.Replace("<Methods>",Methods);

			LineSpace=GetLineSpace(Header,"<PackMethods>");
			Methods = MakeMsgPackMethods(InterfaceInfo.MethodList, ExportType, LineSpace);
			Header.Replace("<PackMethods>",Methods);

			WriteStringToFile(HeaderFile, Header);
			HeaderFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",HeaderFileName);
			MessageBox(Msg);
		}
	}

	if(m_InterfaceConfig.CallSourceExportConfig.IsExport)
	{
		SourceFileName.Format("%s\\%sMsgCaller.%s", ExportDir, InterfaceName,
			m_InterfaceConfig.CallSourceExportConfig.ExportExt);
		if(SourceFile.Open(SourceFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			CString Source=m_MsgCallerSourceTemplate;

			Source.Replace("<SpaceName>", SpaceName);
			Source.Replace("<InterfaceName>", InterfaceName);
			Source.Replace("<@InterfaceName>", InterfaceInfo.Name);

			CString LineSpace=GetLineSpace(Source,"<Methods>");
			CString Methods = MakeMethodsSource(SpaceName, InterfaceInfo, ExportType, LineSpace);
			Source.Replace("<Methods>",Methods);	

			LineSpace=GetLineSpace(Source,"<PackMethods>");
			Methods = MakeMsgPackMethodsSource(SpaceName, InterfaceInfo, ExportType, LineSpace);
			Source.Replace("<PackMethods>",Methods);

			WriteStringToFile(SourceFile, Source);
			SourceFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",SourceFileName);
			MessageBox(Msg);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportMsgHandler(LPCTSTR ExportDir, LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType)
{
	CFile HeaderFile,SourceFile;

	CString HeaderFileName,SourceFileName;

	
	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + "Ack";
	else
		InterfaceName = InterfaceInfo.Name;

	if(m_InterfaceConfig.HandlerHeaderExportConfig.IsExport)
	{
		HeaderFileName.Format("%s\\%sMsgHandler.%s", ExportDir, InterfaceName,
			m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
		if(HeaderFile.Open(HeaderFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			CString Header=m_MsgHandlerHeaderTemplate;

			Header.Replace("<SpaceName>", SpaceName);
			Header.Replace("<InterfaceName>", InterfaceName);
			Header.Replace("<@InterfaceName>", InterfaceInfo.Name);
			CString LineSpace=GetLineSpace(Header,"<Methods>");
			CString Methods = MakeMsgHandlerMethods(InterfaceInfo, ExportType, LineSpace);
			Header.Replace("<Methods>",Methods);

			WriteStringToFile(HeaderFile, Header);
			HeaderFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",HeaderFileName);
			MessageBox(Msg);
		}
	}

	if(m_InterfaceConfig.HandlerSourceExportConfig.IsExport)
	{
		SourceFileName.Format("%s\\%sMsgHandler.%s", ExportDir, InterfaceName,
			m_InterfaceConfig.HandlerSourceExportConfig.ExportExt);
		if(SourceFile.Open(SourceFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			CString Source=m_MsgHandlerSourceTemplate;

			Source.Replace("<SpaceName>", SpaceName);
			Source.Replace("<InterfaceName>", InterfaceName);
			Source.Replace("<@InterfaceName>", InterfaceInfo.Name);

			CString MethodCount;
			MethodCount.Format("%d",InterfaceInfo.MethodList.size());
			Source.Replace("<MethodCount>",MethodCount);

			CString LineSpace=GetLineSpace(Source,"<MsgMapInits>");
			CString MsgMapInits = MakeMsgMapInits(SpaceName, InterfaceInfo, ExportType, LineSpace);
			Source.Replace("<MsgMapInits>",MsgMapInits);	

			LineSpace=GetLineSpace(Source,"<Methods>");
			CString Methods = MakeMsgHandlerMethodsSource(InterfaceInfo, ExportType, LineSpace);
			Source.Replace("<Methods>",Methods);	

			WriteStringToFile(SourceFile, Source);
			SourceFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",SourceFileName);
			MessageBox(Msg);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportDataObject(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, LPCTSTR ExportDir, LPCTSTR SpaceName)
{
	CFile HeaderFile,SourceFile;

	CString HeaderFileName,SourceFileName;

	
	for (size_t i = 0; i<DataStructList.size(); i++)
	{
		if (DataStructList[i]->ListType == DATA_STRUCT_STRUCT && (!DataStructList[i]->IgnoreOnExport))
		{
			STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)DataStructList[i];
			for (size_t j = 0; j<pStructList->StructList.size(); j++)
			{
				STRUCT_DEFINE_INFO& StructInfo = pStructList->StructList[j];
				if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				{
					if (m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport)						
					{
						HeaderFileName.Format("%s\\%s.%s", ExportDir, ClassNameToFileName(StructInfo.Name),
							m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);

						CString Header = RestoreFileToTemplate(HeaderFileName, m_DataObjectDefineHeaderTemple);

						if (HeaderFile.Open(HeaderFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone))
						{


							CString LineSpace = GetLineSpace(m_MsgHandlerHeaderTemplate, "<Description>");
							CString Description = ToComment(StructInfo.Description, LineSpace);
							Header.Replace("<Description>", Description);


							Header.Replace("<ClassName>", StructInfo.Name);
							if (StructInfo.BaseStruct.IsEmpty())
							{
								RemoveBlock(Header, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
								Header.Replace("<IfNotHaveBaseClass>", "");
								Header.Replace("</IfNotHaveBaseClass>", "");
								Header.Replace("<BaseClass>", "");

							}
							else
							{
								Header.Replace("<IfHaveBaseClass>", "");
								Header.Replace("</IfHaveBaseClass>", "");
								RemoveBlock(Header, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
								Header.Replace("<BaseClass>", StructInfo.BaseStruct);
							}


							LineSpace = GetLineSpace(Header, "<Members>");
							CString Members = MakeStructMembers(StructInfo, LineSpace);
							Header.Replace("<Members>", Members);

							LineSpace = GetLineSpace(Header, "<SSTIDEnumDefine>");
							CString SSTIDEnum = MakeStructSSTIDEnumDefines(StructInfo, LineSpace);
							Header.Replace("<SSTIDEnumDefine>", SSTIDEnum);

							LineSpace = GetLineSpace(Header, "<ModifyFlagEnumDefine>");
							CString ModifyFlagEnumDefine = MakeDataObjectModifyFlagEnumDefine(StructInfo, LineSpace);
							Header.Replace("<ModifyFlagEnumDefine>", ModifyFlagEnumDefine);

							LineSpace = GetLineSpace(Header, "<SetMethodsDeclare>");
							CString SetMethodsDeclare = MakeDataObjectSetMethodsDeclare(StructInfo, LineSpace);
							Header.Replace("<SetMethodsDeclare>", SetMethodsDeclare);

							LineSpace = GetLineSpace(Header, "<GetMethodsDeclare>");
							CString GetMethodsDeclare = MakeDataObjectGetMethodsDeclare(StructInfo, LineSpace);
							Header.Replace("<GetMethodsDeclare>", GetMethodsDeclare);


							LineSpace = GetLineSpace(Header, "<SetMethodsDefine>");
							CString SetMethodsDefine = MakeDataObjectSetMethodsDefine(StructInfo, LineSpace);
							Header.Replace("<SetMethodsDefine>", SetMethodsDefine);

							LineSpace = GetLineSpace(Header, "<GetMethodsDefine>");
							CString GetMethodsDefine = MakeDataObjectGetMethodsDefine(StructInfo, LineSpace);
							Header.Replace("<GetMethodsDefine>", GetMethodsDefine);

							WriteStringToFile(HeaderFile, Header);
							HeaderFile.Close();
						}
						else
						{
							CString Msg;
							Msg.Format("无法打开文件%s", HeaderFileName);
							MessageBox(Msg);
						}
					}
					{
						SourceFileName.Format("%s\\%s.%s", ExportDir, ClassNameToFileName(StructInfo.Name),
							m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
						CString Source = RestoreFileToTemplate(SourceFileName, m_DataObjectDefineSourceTemple);
						if (SourceFile.Open(SourceFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone))
						{
							CString LineSpace;

							CString SSTIDEnumName;
							SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructInfo.Name));

							if (StructInfo.Flag&STRUCT_FLAG_EXPORT_XML_PROCESS)
							{
								LineSpace = GetLineSpace(Source, "<XMLProcess>");
								CString XMLProcess = MakeXMLProcess(StructInfo, SSTIDEnumName, LineSpace);
								Source.Replace("<XMLProcess>", XMLProcess);
							}
							else
							{
								Source.Replace("<XMLProcess>", "");
							}

							if (StructInfo.Flag&STRUCT_FLAG_EXPORT_JSON_PROCESS)
							{
								LineSpace = GetLineSpace(Source, "<JsonProcess>");
								CString JsonProcess = MakeJsonProcess(StructInfo, SSTIDEnumName, LineSpace);
								Source.Replace("<JsonProcess>", JsonProcess);
							}
							else
							{
								Source.Replace("<JsonProcess>", "");
							}

							LineSpace = GetLineSpace(m_MsgHandlerHeaderTemplate, "<Description>");
							CString Description = ToComment(StructInfo.Description, LineSpace);
							Source.Replace("<Description>", Description);

							Source.Replace("<SpaceName>", SpaceName);

							if (StructInfo.MemberList.size())
							{
								Source.Replace("<IfHaveMember>", "");
								Source.Replace("</IfHaveMember>", "");
							}
							else
							{
								RemoveBlock(Source, "<IfHaveMember>", "</IfHaveMember>");
							}

							if (StructInfo.BaseStruct.IsEmpty())
							{
								RemoveBlock(Source, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
								Source.Replace("<IfNotHaveBaseClass>", "");
								Source.Replace("</IfNotHaveBaseClass>", "");
								Source.Replace("<BaseClass>", "");

							}
							else
							{
								Source.Replace("<IfHaveBaseClass>", "");
								Source.Replace("</IfHaveBaseClass>", "");
								RemoveBlock(Source, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
								Source.Replace("<BaseClass>", StructInfo.BaseStruct);
								Source.Replace("<BaseClass>", StructInfo.BaseStruct);
							}

							Source.Replace("<ClassName>", StructInfo.Name);

							CString FullModifyFlag;
							FullModifyFlag.Format("DOMF_%s_FULL", ClassNameToUpper(StructInfo.Name));
							Source.Replace("<FULL_MODIFY_FLAG>", FullModifyFlag);


							LineSpace = GetLineSpace(Source, "<Members>");
							CString Members = MakeStructMembers(StructInfo, LineSpace);
							Source.Replace("<Members>", Members);

							LineSpace = GetLineSpace(Source, "<SSTIDEnumDefine>");
							CString SSTIDEnum = MakeStructSSTIDEnumDefines(StructInfo, LineSpace);
							Source.Replace("<SSTIDEnumDefine>", SSTIDEnum);

							LineSpace = GetLineSpace(Source, "<ModifyFlagEnumDefine>");
							CString ModifyFlagEnumDefine = MakeDataObjectModifyFlagEnumDefine(StructInfo, LineSpace);
							Source.Replace("<ModifyFlagEnumDefine>", ModifyFlagEnumDefine);

							LineSpace = GetLineSpace(Source, "<DataObjectMembersGetModifyFlag>");
							CString DataObjectMembersGetModifyFlag = MakeDataObjectMembersGetModifyFlag(StructInfo, LineSpace);
							Source.Replace("<DataObjectMembersGetModifyFlag>", DataObjectMembersGetModifyFlag);

							LineSpace = GetLineSpace(Source, "<DataObjectMembersIsModified>");
							CString DataObjectMembersIsModified = MakeDataDataObjectMembersIsModified(StructInfo, LineSpace);
							Source.Replace("<DataObjectMembersIsModified>", DataObjectMembersIsModified);

							LineSpace = GetLineSpace(Source, "<DataObjectMembersClearModifyFlag>");
							CString DataObjectMembersClearModifyFlag = MakeDataDataObjectMembersClearModifyFlag(StructInfo, LineSpace);
							Source.Replace("<DataObjectMembersClearModifyFlag>", DataObjectMembersClearModifyFlag);

							LineSpace = GetLineSpace(Source, "<SetMethodsDefine>");
							CString SetMethodsDefine = MakeDataObjectSetMethodsDefine(StructInfo, LineSpace);
							Source.Replace("<SetMethodsDefine>", SetMethodsDefine);

							LineSpace = GetLineSpace(Source, "<GetMethodsDefine>");
							CString GetMethodsDefine = MakeDataObjectGetMethodsDefine(StructInfo, LineSpace);
							Source.Replace("<GetMethodsDefine>", GetMethodsDefine);

							Source.Replace("<CLASS_FLAG_INDEX>", "DATA_OBJECT_FLAG_" + ClassNameToUpper(StructInfo.Name));


							LineSpace = GetLineSpace(Source, "<InitOperations>");
							CString InitOperations = MakeInitOperations(StructInfo, LineSpace);
							Source.Replace("<InitOperations>", InitOperations);





							LineSpace = GetLineSpace(Source, "<PackOperations>");
							CString PackOperations = MakePackOperations(StructInfo, SSTIDEnumName, LineSpace);
							Source.Replace("<PackOperations>", PackOperations);

							LineSpace = GetLineSpace(Source, "<UnpackOperations>");
							CString UnpackOperations = MakeUnpackOperations(StructInfo, SSTIDEnumName, LineSpace);
							Source.Replace("<UnpackOperations>", UnpackOperations);

							LineSpace = GetLineSpace(Source, "<PacketSizes>");
							CString PackSizes = MakePackSizes(StructInfo, LineSpace);
							Source.Replace("<PacketSizes>", PackSizes);

							//LineSpace = GetLineSpace(Source, "<ToXMLOperations>");
							//CString ToXMLOperations = MakeToXMLOperations(StructInfo, SSTIDEnumName, LineSpace);
							//Source.Replace("<ToXMLOperations>", ToXMLOperations);

							//LineSpace = GetLineSpace(Source, "<FromXMLOperations>");
							//CString FromXMLOperations = MakeFromXMLOperations(StructInfo, SSTIDEnumName, LineSpace);
							//Source.Replace("<FromXMLOperations>", FromXMLOperations);

							LineSpace = GetLineSpace(Source, "<CloneOperations>");
							CString CloneOperations = MakeCloneOperations(StructInfo, LineSpace, "", "DataObject.");
							Source.Replace("<CloneOperations>", CloneOperations);

							WriteStringToFile(SourceFile, Source);
							SourceFile.Close();
						}
						else
						{
							CString Msg;
							Msg.Format("无法打开文件%s", SourceFileName);
							MessageBox(Msg);
						}
					}
				}
			}
		}
	}
		
	

	
	return true;
}

bool CCallInterfaceMakerDlg::ExportDataObjectModifyFlags(vector<DATA_OBJECT_MODIFY_FLAG> DataObjectModifyFlagList, LPCTSTR ExportDir, LPCTSTR SpaceName)
{
	CFile HeaderFile;

	CString HeaderFileName;
	CString LineSpace;


	if(m_InterfaceConfig.DataObjectModifyFlagsExportConfig.IsExport)
	{
		HeaderFileName.Format("%s\\DataObjectModifyFlags.%s",ExportDir,
			m_InterfaceConfig.DataObjectModifyFlagsExportConfig.ExportExt);

		CString Header=m_DataObjectModifyFlagsTemple;

		if(HeaderFile.Open(HeaderFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			Header.Replace("<SpaceName>", SpaceName);

			LineSpace=GetLineSpace(Header,"<DataObjectModifyFlags>");
			CString DataObjectModifyFlags = MakeDataObjectModifyFlags(m_AllDataStructList, DataObjectModifyFlagList, LineSpace);
			Header.Replace("<DataObjectModifyFlags>",DataObjectModifyFlags);
			
			WriteStringToFile(HeaderFile, Header);
			HeaderFile.Close();
		}
		else
		{
			CString Msg;
			Msg.Format("无法打开文件%s",HeaderFileName);
			MessageBox(Msg);
		}
		
	}
	return true;
}

CString CCallInterfaceMakerDlg::MakeMethods(vector<INTERFACE_METHOD> MethodList, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace, bool IsPure)
{
	CString Methods;	
	for(size_t j=0;j<MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=MethodList[j];
		CString MethodName = MethodInfo.Name;

		vector<METHOD_PARAM> * pParamList = NULL;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			{
				pParamList = &MethodInfo.CallParamList;
			}				
			else
			{
				pParamList = &MethodInfo.AckParamList;
				MethodName = MethodInfo.Name + "Ack";
			}
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
				pParamList = &MethodInfo.CallParamList;
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
				pParamList = &MethodInfo.CallParamList;
			else
				continue;
			break;
		}
		
		if (pParamList)
		{
			CString Method = m_MethodHeaderTemple;
			Method.Replace("<MethodName>", MethodName);
			Method.Replace("<MethodDescription>", MethodInfo.Description);
			if (IsPure)
				Method.Replace("<IsPure>", m_MethodHeaderPureTailTemple);
			else
				Method.Replace("<IsPure>", ";");
			CString Params = MakeParams(*pParamList, true, true);
			CString LineSpace = GetLineSpace(Method, "<ParamDescriptions>");
			CString ParamDescriptions = MakeParamDescriptions(*pParamList, LineSpace);


			Method.Replace("<Params>", Params);
			Method.Replace("<ParamDescriptions>", ParamDescriptions);
			Methods += Method + "\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMethodSSTIDEnumDefines(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString MethodIDEnumDefines;

	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = ClassNameToUpper(InterfaceInfo.Name + "Ack");
	else
		InterfaceName = ClassNameToUpper(InterfaceInfo.Name);

	vector<INTERFACE_METHOD>& MethodList=InterfaceInfo.MethodList;
	for(size_t i=0;i<MethodList.size();i++)
	{
		INTERFACE_METHOD& MethodInfo=MethodList[i];
		CString MethodName = MethodInfo.Name;
		vector<METHOD_PARAM> * pParamList = NULL;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			{
				pParamList = &MethodInfo.CallParamList;
			}				
			else
			{
				pParamList = &MethodInfo.AckParamList;
				MethodName = MethodInfo.Name+"Ack";
			}				
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
				pParamList = &MethodInfo.CallParamList;
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
				pParamList = &MethodInfo.CallParamList;
			else
				continue;
			break;
		}

		if (pParamList)
		{
			ENUM_DEFINE_INFO EnumInfo;
						
			MethodName = ClassNameToUpper(MethodName);

			EnumInfo.Name.Format("%s_%s_MEMBER_IDS", InterfaceName, MethodName);
			EnumInfo.ShortName = EnumInfo.Name;

			EnumInfo.MemberList.resize(pParamList->size());

			

			for (size_t j = 0; j < pParamList->size(); j++)
			{
				CString ParamName;
				ParamName = ClassNameToUpper((*pParamList)[j].Name);
				EnumInfo.MemberList[j].Name.Format("SST_%s_%s", MethodName, ParamName);
				EnumInfo.MemberList[j].Value.Format("%d", (*pParamList)[j].ID);
			}

			MethodIDEnumDefines += MakeEnumDefine(EnumInfo, szLineSpace);
		}
		
	}
	
	return MethodIDEnumDefines;
}

CString CCallInterfaceMakerDlg::MakeStructSSTIDEnumDefines(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name.Format("%s_MEMBER_IDS",ClassNameToUpper(StructInfo.Name));
	EnumInfo.ShortName=EnumInfo.Name;

	EnumInfo.MemberList.resize(StructInfo.MemberList.size());

	CString StructName;
	StructName=StructInfo.ShortName;	

	if (!StructInfo.BaseStruct.IsEmpty())
	{
		if (StructInfo.ObjectID == 0)
		{
			CString Msg;
			Msg.Format("结构%s有基类，但没有设置ObjectID，会导致打解包异常", StructInfo.Name);
			AfxMessageBox(Msg);
		}
		else 
		{
			vector<UINT> ObjectIDList;
			ObjectIDList.push_back(StructInfo.ObjectID);
			if (!CheckBaseStructObjectID(ObjectIDList, StructInfo.BaseStruct))
			{
				CString Msg;
				Msg.Format("结构%s有基类中ObjectID有重复，会导致打解包异常", StructInfo.Name);
				AfxMessageBox(Msg);
			}
		}
	}

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		CString MemberName;
		MemberName=ClassNameToUpper(StructInfo.MemberList[j].Name);
		EnumInfo.MemberList[j].Name.Format("SST_%s_%s",StructName,MemberName);
		EnumInfo.MemberList[j].Value.Format("%d",StructInfo.ObjectID+StructInfo.MemberList[j].ID);
	}

	return MakeEnumDefine(EnumInfo,szLineSpace);


	
}

CString CCallInterfaceMakerDlg::MakeMsgPackMethods(vector<INTERFACE_METHOD>& MethodList, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString Methods;
	for(size_t j=0;j<MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=MethodList[j];
		CString MethodName = MethodInfo.Name;
		vector<METHOD_PARAM> * pParamList = NULL;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			{
				pParamList = &MethodInfo.CallParamList;
			}				
			else
			{
				pParamList = &MethodInfo.AckParamList;
				MethodName = MethodInfo.Name + "Ack";
			}				
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
				pParamList = &MethodInfo.CallParamList;
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
				pParamList = &MethodInfo.CallParamList;
			else
				continue;
			break;
		}
		if (pParamList)
		{
			CString Method = m_MsgPackMethodHeaderTemple;
			Method.Replace("<MethodName>", MethodName);
			CString Params = MakeParams(*pParamList, true, true);
			if (!Params.IsEmpty())
				Params = "," + Params;
			Method.Replace("<,Params>", Params);
			Methods += Method + "\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeParams(vector<METHOD_PARAM>& ParamList,bool IsDefine,bool IncludeDefaultValue)
{
	CString Params;
	for(size_t k=0;k<ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo=ParamList[k];

		TYPE_DEFINE * pTypeInfo=FindVarType(ParamInfo.Type);
		if(pTypeInfo)
		{		
			CString Param=m_ParamTemple;
			
			if(IsDefine)
			{
				Param.Replace("<ParamName>",ParamInfo.Name);
				if(ParamInfo.IsReference)
				{
					CString ReferenceDefine=pTypeInfo->GenerateOperations.ReferenceDefine;
					ReferenceDefine.Replace("<Type>",pTypeInfo->CType);
					Param.Replace("<ParamType>",ReferenceDefine);
				}
				else
				{
					Param.Replace("<ParamType>",pTypeInfo->CType);
				}

				
				if(ParamInfo.DefaultValue.IsEmpty()||(!IncludeDefaultValue))
				{
					Param.Replace("<ParamDefaultValue>","");
				}
				else
				{
					CString DefauleValue;
					DefauleValue.Format("= %s",ParamInfo.DefaultValue);
					Param.Replace("<ParamDefaultValue>",DefauleValue);
				}
			}
			else
			{
				if(ParamInfo.IsReference)
				{
					CString ReferenceUse=pTypeInfo->GenerateOperations.ReferenceUse;
					ReferenceUse.Replace("<Variable>",ParamInfo.Name);
					Param.Replace("<ParamName>",ReferenceUse);
				}
				else
				{
					Param.Replace("<ParamName>",ParamInfo.Name);
				}
				
				Param.Replace("<ParamType>","");
				Param.Replace("<ParamLenght>","");
				Param.Replace("<ParamDefaultValue>","");
			}
			if(k)
			{
				Params+=",";							
			}
			Params+=Param;
		}
	}
	return Params;
}

CString CCallInterfaceMakerDlg::MakeParamDescriptions(vector<METHOD_PARAM>& ParamList,LPCTSTR szLineSpace)
{
	CString ParamDescriptions;
	for(size_t k=0;k<ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo=ParamList[k];

		TYPE_DEFINE * pTypeInfo=FindVarType(ParamInfo.Type);
		if(pTypeInfo)
		{			

			CString ParamDescription=m_ParamDescriptionTemple;

			ParamDescription.Replace("<ParamName>",ParamInfo.Name);
			ParamDescription.Replace("<ParamDescript>",ParamInfo.Description);

			ParamDescriptions+=ParamDescription;
			if(k<ParamList.size()-1)
			{
				ParamDescriptions+="\r\n";
			}
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	ParamDescriptions.Replace("\r\n",Space);
	return ParamDescriptions;
}

CString CCallInterfaceMakerDlg::MakeParamDefines(vector<METHOD_PARAM>& ParamList,LPCTSTR szLineSpace)
{
	CString ParamDefines;

	int MaxLen=0;
	for(size_t i=0;i<ParamList.size();i++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(ParamList[i].Type);
		if(pTypeInfo)
		{
			if(pTypeInfo->CType.GetLength()>MaxLen)
				MaxLen=pTypeInfo->CType.GetLength();
		}
	}
	int SpaceLen=(((int)ceil(MaxLen/4.0f))+1)*4;

	for(size_t k=0;k<ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo=ParamList[k];

		TYPE_DEFINE * pTypeInfo=FindVarType(ParamInfo.Type);
		if(pTypeInfo)
		{		
			CString Param=pTypeInfo->GenerateOperations.VariableDefine;
			Param.Replace("<Variable>",ParamInfo.Name);			
			Param.Replace("<Type>",pTypeInfo->CType);	
			CString Space;
			int CurSpaceCount=(int)ceil((SpaceLen-pTypeInfo->CType.GetLength())/4.0f);
			while(CurSpaceCount)
			{
				Space+="	";
				CurSpaceCount--;
			}
			Param.Replace("<Space>",Space);
			
			ParamDefines+=Param;
			ParamDefines+="\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	ParamDefines.Replace("\r\n",Space);
	return ParamDefines;
}

CString CCallInterfaceMakerDlg::MakeMethodsSource(LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + "Ack";
	else
		InterfaceName = InterfaceInfo.Name;

	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString InterfaceNameUp = ClassNameToUpper(InterfaceName);
	CString ModuleID;
	CString InterfaceIDEnumName;
	CString InterfaceID;
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(InterfaceInfo.ModuleID);
	if (pModuleInfo)
	{
		InterfaceIDEnumName.Format("%s_INTERFACES", ClassNameToUpper(pModuleInfo->Name));
		InterfaceID.Format("%s_INTERFACE_%s", ClassNameToUpper(pModuleInfo->Name), ClassNameToUpper(InterfaceInfo.Name));
		ModuleID = "MODULE_ID_" + ClassNameToUpper(pModuleInfo->Name);
	}

	CString MethodIDEnumName;
	MethodIDEnumName.Format("%s_METHODS", ClassNameToUpper(InterfaceInfo.Name));

	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];
		CString MethodName = MethodInfo.Name;
		vector<METHOD_PARAM> * pParamList = NULL;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			{
				pParamList = &MethodInfo.CallParamList;
			}
			else
			{
				pParamList = &MethodInfo.AckParamList;
				MethodName = MethodInfo.Name + "Ack";
			}				
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
				pParamList = &MethodInfo.CallParamList;
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
				pParamList = &MethodInfo.CallParamList;
			else
				continue;
			break;
		}

		if (pParamList)
		{
			CString Method = m_MethodCallerSourceTemple;
			
			CString MethodNameUp = ClassNameToUpper(MethodName);

			Method.Replace("<ModuleID>", ModuleID);
			Method.Replace("<InterfaceName>", InterfaceName);
			Method.Replace("<MethodName>", MethodName);
			Method.Replace("<!SpaceName>", SpaceNameUp);
			Method.Replace("<!InterfaceName>", InterfaceNameUp);
			Method.Replace("<!MethodName>", MethodNameUp);
			Method.Replace("<@InterfaceName>", InterfaceInfo.Name);
			CString Params = MakeParams(*pParamList, true, true);
			Method.Replace("<Params>", Params);
			Params = MakeParams(*pParamList, false, true);
			if (!Params.IsEmpty())
				Params = "," + Params;
			Method.Replace("<,Params>", Params);
			CString LineSpace = GetLineSpace(Method, "<PacketSizes>");
			CString PackSizes = MakePackSizes(MethodInfo, ExportType, LineSpace);
			Method.Replace("<PacketSizes>", PackSizes);
			
			Method.Replace("<InterfaceIDEnumName>", InterfaceIDEnumName);
			Method.Replace("<InterfaceID>", InterfaceID);
			CString MethodID;
			MethodID.Format("METHOD_%s", ClassNameToUpper(MethodInfo.Name));
			Method.Replace("<MethodID>", MethodID);
			Method.Replace("<MethodIDEnumName>", MethodIDEnumName);
			Method.Replace("<IsAckMsg>", ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK ? "true" : "false");

			if (MethodInfo.Flag&INTERFACE_METHOD_FLAG_CAN_CACHE)
			{
				Method.Replace("<IfMsgCanCache>", "");
				Method.Replace("</IfMsgCanCache>", "");
			}
			else
			{
				RemoveBlock(Method, "<IfMsgCanCache>", "</IfMsgCanCache>");
			}

			Methods += Method + "\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgPackMethodsSource(LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + "Ack";
	else
		InterfaceName = InterfaceInfo.Name;

	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString InterfaceNameUp = ClassNameToUpper(InterfaceName);

	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString MethodName = MethodInfo.Name;
		vector<METHOD_PARAM> * pParamList = NULL;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			{
				pParamList = &MethodInfo.CallParamList;
			}
			else
			{
				pParamList = &MethodInfo.AckParamList;
				MethodName = MethodInfo.Name + "Ack";
			}
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
				pParamList = &MethodInfo.CallParamList;
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
				pParamList = &MethodInfo.CallParamList;
			else
				continue;
			break;
		}

		if (pParamList)
		{
			CString Method = m_MsgPackMethodSourceTemple;
			
			CString MethodNameUp = ClassNameToUpper(MethodName);
			Method.Replace("<InterfaceName>", InterfaceName);
			Method.Replace("<MethodName>", MethodName);
			Method.Replace("<!SpaceName>", SpaceNameUp);
			Method.Replace("<!InterfaceName>", InterfaceNameUp);
			Method.Replace("<!MethodName>", MethodNameUp);
			Method.Replace("<@InterfaceName>", InterfaceInfo.Name);
			CString Params = MakeParams(*pParamList, true, true);
			if (!Params.IsEmpty())
				Params = "," + Params;
			Method.Replace("<,Params>", Params);
			CString LineSpace = GetLineSpace(Method, "<PackOperations>");
			CString SSTIDEnumName;
			SSTIDEnumName.Format("%s_%s_MEMBER_IDS", ClassNameToUpper(InterfaceName), ClassNameToUpper(MethodName));
			CString PackOperations = MakePackOperations(MethodInfo, ExportType, SSTIDEnumName, LineSpace);
			Method.Replace("<PackOperations>", PackOperations);

			Methods += Method + "\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeInitOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString InitOperations;

	vector<METHOD_PARAM> * pParamList = NULL;
	switch (MethodInfo.Type)
	{
	case INTERFACE_METHOD_TYPE_CALL:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			pParamList = &MethodInfo.CallParamList;
		else
			pParamList = &MethodInfo.AckParamList;
		break;
	case INTERFACE_METHOD_TYPE_NOTIFY:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
			pParamList = &MethodInfo.CallParamList;
		break;
	case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			pParamList = &MethodInfo.CallParamList;		
		break;
	}
	if (pParamList)
	{
		for (size_t i = 0; i < pParamList->size(); i++)
		{

			TYPE_DEFINE * pTypeInfo = FindVarType((*pParamList)[i].Type);
			if (pTypeInfo)
			{
				if (!pTypeInfo->GenerateOperations.InitOperation.IsEmpty())
				{
					CString Operation = pTypeInfo->GenerateOperations.InitOperation;
					Operation.Replace("<Variable>", (*pParamList)[i].Name);

					InitOperations += Operation;
					InitOperations += "\r\n";
				}
			}
		}
		CString Space = "\r\n";
		Space += szLineSpace;
		InitOperations.Replace("\r\n", Space);
	}

	return InitOperations;
}

CString CCallInterfaceMakerDlg::MakePackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, CString SSTIDEnumName, LPCTSTR szLineSpace)
{
	CString PackOperations;
	CString MethodName = MethodInfo.Name;
	vector<METHOD_PARAM> * pParamList = NULL;
	switch (MethodInfo.Type)
	{
	case INTERFACE_METHOD_TYPE_CALL:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
		{
			pParamList = &MethodInfo.CallParamList;
		}
		else
		{
			pParamList = &MethodInfo.AckParamList;
			MethodName = MethodInfo.Name + "Ack";
		}
		break;
	case INTERFACE_METHOD_TYPE_NOTIFY:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
			pParamList = &MethodInfo.CallParamList;
		break;
	case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			pParamList = &MethodInfo.CallParamList;		
		break;
	}

	if (pParamList)
	{
		for (size_t i = 0; i < pParamList->size(); i++)
		{

			TYPE_DEFINE * pTypeInfo = FindVarType((*pParamList)[i].Type);
			if (pTypeInfo)
			{
				CString Operation = m_PackOperationUnitTemplate;


				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");


				Operation.Replace("<Variable>", (*pParamList)[i].Name);

				CString LineSpace = GetLineSpace(Operation, "<PackOperation>");
				CString PackOP = MakePackOperation((*pParamList)[i].Type,
					(*pParamList)[i].Name,
					MethodName, ClassNameToUpper(MethodName),
					SSTIDEnumName,
					false, false, false,
					LineSpace);

				Operation.Replace("<PackOperation>", PackOP);

				PackOperations += Operation;
				PackOperations += "\r\n";
			}
		}
		CString Space = "\r\n";
		Space += szLineSpace;
		PackOperations.Replace("\r\n", Space);
	}
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakeUnpackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, CString SSTIDEnumName, LPCTSTR szLineSpace)
{
	CString PackOperations;
	CString MethodName = MethodInfo.Name;
	vector<METHOD_PARAM> * pParamList = NULL;
	switch (MethodInfo.Type)
	{
	case INTERFACE_METHOD_TYPE_CALL:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
		{
			pParamList = &MethodInfo.CallParamList;
		}			
		else
		{
			pParamList = &MethodInfo.AckParamList;
			MethodName = MethodInfo.Name + "Ack";
		}			
		break;
	case INTERFACE_METHOD_TYPE_NOTIFY:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
			pParamList = &MethodInfo.CallParamList;
		break;
	case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			pParamList = &MethodInfo.CallParamList;		
		break;
	}

	if (pParamList)
	{
		for (size_t i = 0; i < pParamList->size(); i++)
		{

			TYPE_DEFINE * pTypeInfo = FindVarType((*pParamList)[i].Type);
			if (pTypeInfo)
			{
				CString Operation = m_UnpackOperationUnitTemplate;

				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");

				CString SST_ID;

				SST_ID.Format("SST_%s_%s", ClassNameToUpper(MethodName), ClassNameToUpper((*pParamList)[i].Name));
				SST_ID.MakeUpper();


				Operation.Replace("<SST_NAME>", SSTIDEnumName);
				Operation.Replace("<SST_ID>", SST_ID);

				Operation.Replace("<Variable>", (*pParamList)[i].Name);

				CString LineSpace = GetLineSpace(Operation, "<PackOperation>");
				CString UnpackOP = MakeUnpackOperation((*pParamList)[i].Type,
					(*pParamList)[i].Name,
					MethodName, ClassNameToUpper(MethodName),
					SSTIDEnumName,
					false, false, false,
					LineSpace);

				Operation.Replace("<UnpackOperation>", UnpackOP);

				PackOperations += Operation;
				PackOperations += "\r\n";
			}
		}
		CString Space = "\r\n";
		Space += szLineSpace;
		PackOperations.Replace("\r\n", Space);
	}
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakePackSizes(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString PackSizes;

	vector<METHOD_PARAM> * pParamList = NULL;
	switch (MethodInfo.Type)
	{
	case INTERFACE_METHOD_TYPE_CALL:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			pParamList = &MethodInfo.CallParamList;
		else
			pParamList = &MethodInfo.AckParamList;
		break;
	case INTERFACE_METHOD_TYPE_NOTIFY:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
			pParamList = &MethodInfo.CallParamList;
		break;
	case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
		if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			pParamList = &MethodInfo.CallParamList;		
		break;
	}

	if (pParamList)
	{
		for (size_t i = 0; i < pParamList->size(); i++)
		{

			TYPE_DEFINE * pTypeInfo = FindVarType((*pParamList)[i].Type);
			if (pTypeInfo)
			{
				CString Operation = pTypeInfo->GenerateOperations.SizeCaculateOperation;

				RemoveBlock(Operation, "<IfDataObject>", "</IfDataObject>");
				Operation.Replace("<IfNotDataObject>", "");
				Operation.Replace("</IfNotDataObject>", "");

				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");

				Operation.Replace("<Variable>", (*pParamList)[i].Name);
				Operation.Replace("<Type>", pTypeInfo->CType);

				PackSizes += "+";
				PackSizes += Operation;
				if (i < pParamList->size() - 1)
					PackSizes += "\r\n";
			}
		}
		CString Space = "\r\n";
		Space += szLineSpace;
		PackSizes.Replace("\r\n", Space);
	}
	return PackSizes;
}

CString CCallInterfaceMakerDlg::MakeInitOperations(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString InitOperations;

	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		if(pTypeInfo)
		{
			if(!pTypeInfo->GenerateOperations.InitOperation.IsEmpty())
			{
				CString Operation=pTypeInfo->GenerateOperations.InitOperation;
				if(StructInfo.MemberList[i].IsArray)
				{
					Operation=m_InterfaceConfig.ArrayDefineConfig.InitOperation;
				}
				if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
					Operation.Replace("<Variable>","m_"+StructInfo.MemberList[i].Name);
				else
					Operation.Replace("<Variable>",StructInfo.MemberList[i].Name);

				CString Temp;
				Temp.Format("%u",StructInfo.MemberList[i].ArrayStartLength);
				Operation.Replace("<ArrayStartLength>",Temp);
				Temp.Format("%u",StructInfo.MemberList[i].ArrayGrowLength);
				Operation.Replace("<ArrayGrowLength>",Temp);

				InitOperations+=Operation;
				InitOperations+="\r\n";
			}
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	InitOperations.Replace("\r\n",Space);
	return InitOperations;
}
CString CCallInterfaceMakerDlg::MakePackOperations(STRUCT_DEFINE_INFO& StructInfo,CString SSTIDEnumName,LPCTSTR szLineSpace)
{
	CString PackOperations;

	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;
		
		CString Operation=m_PackOperationUnitTemplate;
		if(StructInfo.MemberList[i].IsArray)
		{
			Operation=m_InterfaceConfig.ArrayDefineConfig.PackOperation;
		}

		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>","");
			Operation.Replace("</IfInDataObject>","");
			if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");
			}
			else
			{
				Operation.Replace("<IfNeedCheckModifyFlag>","");
				Operation.Replace("</IfNeedCheckModifyFlag>","");
			}
		}
		else
		{
			RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>","");
			Operation.Replace("</IfNotInDataObject>","");
			RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");
		}

		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			Operation.Replace("<Variable>","m_"+StructInfo.MemberList[i].Name);
		else
			Operation.Replace("<Variable>",StructInfo.MemberList[i].Name);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));			
		Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

		CString ModifyFlag="MF_"+ClassNameToUpper(StructInfo.MemberList[i].Name);
		Operation.Replace("<ModifyFlag>",ModifyFlag);

		CString LineSpace=GetLineSpace(Operation,"<PackOperation>");
		CString PackOP=MakePackOperation(StructInfo.MemberList[i].Type,
			StructInfo.MemberList[i].Name,
			StructInfo.Name,StructInfo.ShortName,
			SSTIDEnumName,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)!=0,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)!=0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
			StructInfo.MemberList[i].IsArray,
			LineSpace);

		Operation.Replace("<PackOperation>",PackOP);

		CString SST_ID;

		SST_ID.Format("SST_%s_%s",StructInfo.ShortName,ClassNameToUpper(StructInfo.MemberList[i].Name));
		SST_ID.MakeUpper();


		Operation.Replace("<SST_NAME>",SSTIDEnumName);
		Operation.Replace("<SST_ID>",SST_ID);


		PackOperations+=Operation;
		PackOperations+="\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackOperations.Replace("\r\n",Space);
	return PackOperations;
}
CString CCallInterfaceMakerDlg::MakeUnpackOperations(STRUCT_DEFINE_INFO& StructInfo,CString SSTIDEnumName,LPCTSTR szLineSpace)
{
	CString PackOperations;

	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{

		if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		if(pTypeInfo)
		{
			CString Operation=m_UnpackOperationUnitTemplate;
			if(StructInfo.MemberList[i].IsArray)
			{
				Operation=m_InterfaceConfig.ArrayDefineConfig.UnpackOperation;

				TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
				CString Space;
				if(pTypeInfo)
				{
					CString VariableDefine=pTypeInfo->GenerateOperations.VariableDefine;
					VariableDefine.Replace("<Type>",pTypeInfo->CType);					
					VariableDefine.Replace("<Space>","	");					
					VariableDefine.Replace("<Variable>","ArrayElement");
					Operation.Replace("<ArrayElementVariableDefine>",VariableDefine);

					CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
					VariableInit.Replace("<Variable>", "ArrayElement");
					Operation.Replace("<ArrayElementVariableInit>", VariableInit);
				}
			}

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>","");
				Operation.Replace("</IfInDataObject>","");
				if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");
				}
				else
				{
					Operation.Replace("<IfNeedCheckModifyFlag>","");
					Operation.Replace("</IfNeedCheckModifyFlag>","");
				}
			}
			else
			{
				RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>","");
				Operation.Replace("</IfNotInDataObject>","");
				RemoveBlock(Operation,"<IfNeedCheckModifyFlag>","</IfNeedCheckModifyFlag>");
			}

			CString SST_ID;
			
			SST_ID.Format("SST_%s_%s",StructInfo.ShortName,ClassNameToUpper(StructInfo.MemberList[i].Name));
			SST_ID.MakeUpper();
			

			Operation.Replace("<SST_NAME>",SSTIDEnumName);
			Operation.Replace("<SST_ID>",SST_ID);

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				Operation.Replace("<Variable>","m_"+StructInfo.MemberList[i].Name);
			else
				Operation.Replace("<Variable>",StructInfo.MemberList[i].Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));			
			Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

			CString ModifyFlag="MF_"+ClassNameToUpper(StructInfo.MemberList[i].Name);
			Operation.Replace("<ModifyFlag>",ModifyFlag);

			CString LineSpace=GetLineSpace(Operation,"<UnpackOperation>");
			CString UnpackOP=MakeUnpackOperation(StructInfo.MemberList[i].Type,
				StructInfo.MemberList[i].Name,
				StructInfo.Name,StructInfo.ShortName,
				SSTIDEnumName,
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)!=0,
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)!=0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
				StructInfo.MemberList[i].IsArray,
				LineSpace);

			Operation.Replace("<UnpackOperation>",UnpackOP);

			PackOperations+=Operation;
			PackOperations+="\r\n";
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackOperations.Replace("\r\n",Space);
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakePackSizes(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString PackSizes;

	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		if(pTypeInfo)
		{
			CString PackSize=m_PackSizeTemple;
			CString Operation=pTypeInfo->GenerateOperations.SizeCaculateOperation;		
			CString Variable;

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>","");
				Operation.Replace("</IfInDataObject>","");

				Variable="m_"+StructInfo.MemberList[i].Name;
				
			}
			else
			{
				RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>","");
				Operation.Replace("</IfNotInDataObject>","");

				Variable=StructInfo.MemberList[i].Name;
			}

			if(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
			{
				RemoveBlock(Operation,"<IfNotDataObject>","</IfNotDataObject>");
				Operation.Replace("<IfDataObject>","");
				Operation.Replace("</IfDataObject>","");
			}
			else
			{
				RemoveBlock(Operation,"<IfDataObject>","</IfDataObject>");
				Operation.Replace("<IfNotDataObject>","");
				Operation.Replace("</IfNotDataObject>","");
			}

			if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_64BIT)
			{
				RemoveBlock(Operation, "<IfNot64Bit>", "</IfNot64Bit>");
				Operation.Replace("<If64Bit>", "");
				Operation.Replace("</If64Bit>", "");
			}
			else
			{
				RemoveBlock(Operation, "<If64Bit>", "</If64Bit>");
				Operation.Replace("<IfNot64Bit>", "");
				Operation.Replace("</IfNot64Bit>", "");
			}

			

			if(StructInfo.MemberList[i].IsArray)
			{
				CString Var=m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
				Var.Replace("<Variable>",Variable);
				Var.Replace("<Index>","i");
				Operation.Replace("<Variable>",Var);
				Operation.Replace("<Type>",pTypeInfo->CType);
				CString ArrayOP=m_InterfaceConfig.ArrayDefineConfig.SizeCaculateOperation;
				ArrayOP.Replace("<Variable>",Variable);
				ArrayOP.Replace("<ArrayElementSize>",Operation);
				PackSize=ArrayOP;
			}
			else
			{
				Operation.Replace("<Variable>",Variable);
				Operation.Replace("<Type>",pTypeInfo->CType);
				PackSize.Replace("<PackSize>",Operation);
			}

			PackSizes+=PackSize;
			PackSizes+="\r\n";
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackSizes.Replace("\r\n",Space);
	return PackSizes;
}

CString CCallInterfaceMakerDlg::MakeXMLProcess(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace)
{
	CString XMLProcess = m_StructXMLProcessTemplate;

	CString LineSpace = GetLineSpace(XMLProcess, "<ToXMLOperations>");
	CString ToXMLOperations = MakeToXMLOperations(StructInfo, SSTIDEnumName, LineSpace);
	XMLProcess.Replace("<ToXMLOperations>", ToXMLOperations);

	LineSpace = GetLineSpace(XMLProcess, "<FromXMLOperations>");
	CString FromXMLOperations = MakeFromXMLOperations(StructInfo, SSTIDEnumName, LineSpace);
	XMLProcess.Replace("<FromXMLOperations>", FromXMLOperations);

	CString Space = "\r\n";
	Space += szLineSpace;
	XMLProcess.Replace("\r\n", Space);
	return XMLProcess;

}

CString CCallInterfaceMakerDlg::MakeToXMLOperations(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation = m_ToXMLOperationUnitTemplate;
		if (StructInfo.MemberList[i].IsArray)
		{
			Operation = m_InterfaceConfig.ArrayDefineConfig.ToXMLOperation;
		}

		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
			if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");
			}
			else
			{
				Operation.Replace("<IfNeedCheckModifyFlag>", "");
				Operation.Replace("</IfNeedCheckModifyFlag>", "");
			}
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
			RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");
		}

		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			Operation.Replace("<Variable>", "m_" + StructInfo.MemberList[i].Name);
		else
			Operation.Replace("<Variable>", StructInfo.MemberList[i].Name);

		Operation.Replace("<VariableName>", StructInfo.MemberList[i].Name);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(StructInfo.MemberList[i].Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		CString LineSpace = GetLineSpace(Operation, "<ToXMLOperation>");
		CString PackOP = MakeToXMLOperation(StructInfo.MemberList[i].Type,
			StructInfo.MemberList[i].Name,
			StructInfo.Name, StructInfo.ShortName,
			SSTIDEnumName,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)!=0,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)!=0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
			StructInfo.MemberList[i].IsArray,
			LineSpace);

		Operation.Replace("<ToXMLOperation>", PackOP);

		CString SST_ID;

		SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
		SST_ID.MakeUpper();


		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);


		Operations += Operation;
		Operations += "\r\n";
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeFromXMLOperations(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{

		if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE * pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			CString Operation = m_FromXMLOperationUnitTemplate;
			if (StructInfo.MemberList[i].IsArray)
			{
				Operation = m_InterfaceConfig.ArrayDefineConfig.FromXMLOperation;

				TYPE_DEFINE * pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
				CString Space;
				if (pTypeInfo)
				{
					CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
					VariableDefine.Replace("<Type>", pTypeInfo->CType);
					VariableDefine.Replace("<Space>", "	");
					VariableDefine.Replace("<Variable>", "ArrayElement");
					Operation.Replace("<ArrayElementVariableDefine>", VariableDefine);

					CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
					VariableInit.Replace("<Variable>", "ArrayElement");
					Operation.Replace("<ArrayElementVariableInit>", VariableInit);
				}
			}

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>", "");
				Operation.Replace("</IfInDataObject>", "");
				if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");
				}
				else
				{
					Operation.Replace("<IfNeedCheckModifyFlag>", "");
					Operation.Replace("</IfNeedCheckModifyFlag>", "");
				}
			}
			else
			{
				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");
			}

			CString SST_ID;

			SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
			SST_ID.MakeUpper();


			Operation.Replace("<SST_NAME>", SSTIDEnumName);
			Operation.Replace("<SST_ID>", SST_ID);

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				Operation.Replace("<Variable>", "m_" + StructInfo.MemberList[i].Name);
			else
				Operation.Replace("<Variable>", StructInfo.MemberList[i].Name);

			Operation.Replace("<VariableName>", StructInfo.MemberList[i].Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
			Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

			CString ModifyFlag = "MF_" + ClassNameToUpper(StructInfo.MemberList[i].Name);
			Operation.Replace("<ModifyFlag>", ModifyFlag);

			CString LineSpace = GetLineSpace(Operation, "<FromXMLOperation>");
			CString UnpackOP = MakeFromXMLOperation(StructInfo.MemberList[i].Type,
				StructInfo.MemberList[i].Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName,
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)!=0, 
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)!=0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
				StructInfo.MemberList[i].IsArray,
				LineSpace);

			Operation.Replace("<FromXMLOperation>", UnpackOP);

			Operations += Operation;
			Operations += "\r\n";
		}
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}


CString CCallInterfaceMakerDlg::MakeJsonProcess(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace)
{
	CString JsonProcess = m_StructJsonProcessTemplate;

	CString LineSpace = GetLineSpace(JsonProcess, "<ToJsonOperations>");
	CString ToJsonOperations = MakeToJsonOperations(StructInfo, SSTIDEnumName, LineSpace);
	JsonProcess.Replace("<ToJsonOperations>", ToJsonOperations);

	LineSpace = GetLineSpace(JsonProcess, "<FromJsonOperations>");
	CString FromJsonOperations = MakeFromJsonOperations(StructInfo, SSTIDEnumName, LineSpace);
	JsonProcess.Replace("<FromJsonOperations>", FromJsonOperations);

	CString Space = "\r\n";
	Space += szLineSpace;
	JsonProcess.Replace("\r\n", Space);
	return JsonProcess;
}
CString CCallInterfaceMakerDlg::MakeToJsonOperations(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation = m_ToJsonOperationUnitTemplate;
		if (StructInfo.MemberList[i].IsArray)
		{
			Operation = m_InterfaceConfig.ArrayDefineConfig.ToJsonOperation;
		}

		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
			if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");
			}
			else
			{
				Operation.Replace("<IfNeedCheckModifyFlag>", "");
				Operation.Replace("</IfNeedCheckModifyFlag>", "");
			}
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
			RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");
		}

		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			Operation.Replace("<Variable>", "m_" + StructInfo.MemberList[i].Name);
		else
			Operation.Replace("<Variable>", StructInfo.MemberList[i].Name);

		Operation.Replace("<VariableName>", StructInfo.MemberList[i].Name);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(StructInfo.MemberList[i].Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		CString LineSpace = GetLineSpace(Operation, "<ToJsonOperation>");
		CString PackOP = MakeToJsonOperation(StructInfo.MemberList[i].Type,
			StructInfo.MemberList[i].Name,
			StructInfo.Name, StructInfo.ShortName,
			SSTIDEnumName,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
			StructInfo.MemberList[i].IsArray,
			LineSpace);

		Operation.Replace("<ToJsonOperation>", PackOP);

		CString SST_ID;

		SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
		SST_ID.MakeUpper();


		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);


		Operations += Operation;
		Operations += "\r\n";
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeFromJsonOperations(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{

		if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE * pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			CString Operation = m_FromJsonOperationUnitTemplate;
			if (StructInfo.MemberList[i].IsArray)
			{
				Operation = m_InterfaceConfig.ArrayDefineConfig.FromJsonOperation;

				TYPE_DEFINE * pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
				CString Space;
				if (pTypeInfo)
				{
					CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
					VariableDefine.Replace("<Type>", pTypeInfo->CType);
					VariableDefine.Replace("<Space>", "	");
					VariableDefine.Replace("<Variable>", "ArrayElement");
					Operation.Replace("<ArrayElementVariableDefine>", VariableDefine);

					CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
					VariableInit.Replace("<Variable>", "ArrayElement");
					Operation.Replace("<ArrayElementVariableInit>", VariableInit);
				}
			}

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>", "");
				Operation.Replace("</IfInDataObject>", "");
				if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");
				}
				else
				{
					Operation.Replace("<IfNeedCheckModifyFlag>", "");
					Operation.Replace("</IfNeedCheckModifyFlag>", "");
				}
			}
			else
			{
				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfNeedCheckModifyFlag>", "</IfNeedCheckModifyFlag>");
			}

			CString SST_ID;

			SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
			SST_ID.MakeUpper();


			Operation.Replace("<SST_NAME>", SSTIDEnumName);
			Operation.Replace("<SST_ID>", SST_ID);

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				Operation.Replace("<Variable>", "m_" + StructInfo.MemberList[i].Name);
			else
				Operation.Replace("<Variable>", StructInfo.MemberList[i].Name);

			Operation.Replace("<VariableName>", StructInfo.MemberList[i].Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
			Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

			CString ModifyFlag = "MF_" + ClassNameToUpper(StructInfo.MemberList[i].Name);
			Operation.Replace("<ModifyFlag>", ModifyFlag);

			CString LineSpace = GetLineSpace(Operation, "<FromJsonOperation>");
			CString UnpackOP = MakeFromJsonOperation(StructInfo.MemberList[i].Type,
				StructInfo.MemberList[i].Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName,
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0,
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
				StructInfo.MemberList[i].IsArray,
				LineSpace);

			Operation.Replace("<FromJsonOperation>", UnpackOP);

			Operations += Operation;
			Operations += "\r\n";
		}
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}

CString CCallInterfaceMakerDlg::MakePackOperation(CString Type,CString Name,CString ParentName,CString ParentShortName,CString SSTIDEnumName,bool IsInDataObject,bool IsMFCheck,bool IsArray,LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo=FindVarType(Type);
	if(pTypeInfo)
	{
		Operation=pTypeInfo->GenerateOperations.PackOperation;

		if(IsMFCheck)
		{
			RemoveBlock(Operation,"<IfNotMFCheck>","</IfNotMFCheck>");
			Operation.Replace("<IfMFCheck>","");
			Operation.Replace("</IfMFCheck>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfMFCheck>","</IfMFCheck>");
			Operation.Replace("<IfNotMFCheck>","");
			Operation.Replace("</IfNotMFCheck>","");
		}

		if(IsInDataObject)
		{
			RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>","");
			Operation.Replace("</IfInDataObject>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>","");
			Operation.Replace("</IfNotInDataObject>","");
		}

		if(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation,"<IfNotDataObject>","</IfNotDataObject>");
			Operation.Replace("<IfDataObject>","");
			Operation.Replace("</IfDataObject>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfDataObject>","</IfDataObject>");
			Operation.Replace("<IfNotDataObject>","");
			Operation.Replace("</IfNotDataObject>","");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, "<IfNot64Bit>", "</IfNot64Bit>");
			Operation.Replace("<If64Bit>", "");
			Operation.Replace("</If64Bit>", "");
		}
		else
		{
			RemoveBlock(Operation, "<If64Bit>", "</If64Bit>");
			Operation.Replace("<IfNot64Bit>", "");
			Operation.Replace("</IfNot64Bit>", "");
		}

		CString SST_ID;
		
		
		SST_ID.Format("SST_%s_%s",ParentShortName,ClassNameToUpper(Name));
		SST_ID.MakeUpper();
		
		
		
		//CString VarName=Name;
		if(IsArray)
		{
			CString ArrayVar=m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
			ArrayVar.Replace("<Index>","i");
			Operation.Replace("<Variable>",ArrayVar);
		}
		
		if (IsInDataObject)
			Operation.Replace("<Variable>", "m_" + Name);
		else
			Operation.Replace("<Variable>", Name);
		
		Operation.Replace("<VariableName>", Name);
		Operation.Replace("<Type>",pTypeInfo->CType);
		

		Operation.Replace("<SST_NAME>",SSTIDEnumName);
		Operation.Replace("<SST_ID>",SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(ParentName));			
		Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

		CString ModifyFlag="MF_"+ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>",ModifyFlag);
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Operation.Replace("\r\n",Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeUnpackOperation(CString Type,CString Name,CString ParentName,CString ParentShortName,CString SSTIDEnumName,bool IsInDataObject,bool IsMonitorUpdate,bool IsArray,LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo=FindVarType(Type);
	if(pTypeInfo)
	{
		Operation=pTypeInfo->GenerateOperations.UnpackOperation;

		if(IsMonitorUpdate)
		{
			Operation.Replace("<IfMonitorUpdate>","");
			Operation.Replace("</IfMonitorUpdate>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfMonitorUpdate>","</IfMonitorUpdate>");
		}

		if(IsInDataObject)
		{
			RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>","");
			Operation.Replace("</IfInDataObject>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>","");
			Operation.Replace("</IfNotInDataObject>","");
		}

		if(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation,"<IfNotDataObject>","</IfNotDataObject>");
			Operation.Replace("<IfDataObject>","");
			Operation.Replace("</IfDataObject>","");
		}
		else
		{
			RemoveBlock(Operation,"<IfDataObject>","</IfDataObject>");
			Operation.Replace("<IfNotDataObject>","");
			Operation.Replace("</IfNotDataObject>","");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, "<IfNot64Bit>", "</IfNot64Bit>");
			Operation.Replace("<If64Bit>", "");
			Operation.Replace("</If64Bit>", "");
		}
		else
		{
			RemoveBlock(Operation, "<If64Bit>", "</If64Bit>");
			Operation.Replace("<IfNot64Bit>", "");
			Operation.Replace("</IfNot64Bit>", "");
		}

		CString SST_ID;
		SST_ID.Format("SST_%s_%s",ParentShortName,ClassNameToUpper(Name));
		SST_ID.MakeUpper();
		
		
		if(IsArray)
		{
			Operation.Replace("<Variable>","ArrayElement");
		}
		
		if (IsInDataObject)
			Operation.Replace("<Variable>", "m_" + Name);
		else
			Operation.Replace("<Variable>", Name);
		
		Operation.Replace("<VariableName>", Name);
		Operation.Replace("<Type>",pTypeInfo->CType);

		Operation.Replace("<SST_NAME>",SSTIDEnumName);
		Operation.Replace("<SST_ID>",SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(ParentName));			
		Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

		CString ModifyFlag="MF_"+ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>",ModifyFlag);

		
	}		

	CString Space="\r\n";
	Space+=szLineSpace;
	Operation.Replace("\r\n",Space);
	return Operation;
}
CString CCallInterfaceMakerDlg::MakeToXMLOperation(CString Type, CString Name, CString ParentName, CString ParentShortName, CString SSTIDEnumName, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.ToXMLOperation;

		if (IsMFCheck)
		{
			RemoveBlock(Operation, "<IfNotMFCheck>", "</IfNotMFCheck>");
			Operation.Replace("<IfMFCheck>", "");
			Operation.Replace("</IfMFCheck>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfMFCheck>", "</IfMFCheck>");
			Operation.Replace("<IfNotMFCheck>", "");
			Operation.Replace("</IfNotMFCheck>", "");
		}

		if (IsInDataObject)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotDataObject>", "</IfNotDataObject>");
			Operation.Replace("<IfDataObject>", "");
			Operation.Replace("</IfDataObject>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfDataObject>", "</IfDataObject>");
			Operation.Replace("<IfNotDataObject>", "");
			Operation.Replace("</IfNotDataObject>", "");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, "<IfNot64Bit>", "</IfNot64Bit>");
			Operation.Replace("<If64Bit>", "");
			Operation.Replace("</If64Bit>", "");
		}
		else
		{
			RemoveBlock(Operation, "<If64Bit>", "</If64Bit>");
			Operation.Replace("<IfNot64Bit>", "");
			Operation.Replace("</IfNot64Bit>", "");
		}

		CString SST_ID;


		SST_ID.Format("SST_%s_%s", ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();



		//CString VarName=Name;
		if (IsArray)
		{
			RemoveBlock(Operation, "<IfNotArray>", "</IfNotArray>");
			Operation.Replace("<IfArray>", "");
			Operation.Replace("</IfArray>", "");

			CString ArrayVar = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
			ArrayVar.Replace("<Index>", "i");
			Operation.Replace("<Variable>", ArrayVar);
		}
		else
		{
			RemoveBlock(Operation, "<IfArray>", "</IfArray>");
			Operation.Replace("<IfNotArray>", "");
			Operation.Replace("</IfNotArray>", "");
		}
		
		if (IsInDataObject)
			Operation.Replace("<Variable>", "m_" + Name);
		else
			Operation.Replace("<Variable>", Name);
		

		Operation.Replace("<VariableName>", Name);

		Operation.Replace("<Type>", pTypeInfo->CType);


		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeFromXMLOperation(CString Type, CString Name, CString ParentName, CString ParentShortName, CString SSTIDEnumName, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.FromXMLOperation;

		if (IsMonitorUpdate)
		{
			Operation.Replace("<IfMonitorUpdate>", "");
			Operation.Replace("</IfMonitorUpdate>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfMonitorUpdate>", "</IfMonitorUpdate>");
		}

		if (IsInDataObject)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotDataObject>", "</IfNotDataObject>");
			Operation.Replace("<IfDataObject>", "");
			Operation.Replace("</IfDataObject>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfDataObject>", "</IfDataObject>");
			Operation.Replace("<IfNotDataObject>", "");
			Operation.Replace("</IfNotDataObject>", "");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, "<IfNot64Bit>", "</IfNot64Bit>");
			Operation.Replace("<If64Bit>", "");
			Operation.Replace("</If64Bit>", "");
		}
		else
		{
			RemoveBlock(Operation, "<If64Bit>", "</If64Bit>");
			Operation.Replace("<IfNot64Bit>", "");
			Operation.Replace("</IfNot64Bit>", "");
		}

		CString SST_ID;
		SST_ID.Format("SST_%s_%s", ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();


		if (IsArray)
		{
			RemoveBlock(Operation, "<IfNotArray>", "</IfNotArray>");
			Operation.Replace("<IfArray>", "");
			Operation.Replace("</IfArray>", "");

			Operation.Replace("<Variable>", "ArrayElement");
		}
		else
		{
			RemoveBlock(Operation, "<IfArray>", "</IfArray>");
			Operation.Replace("<IfNotArray>", "");
			Operation.Replace("</IfNotArray>", "");
		}
		
		if (IsInDataObject)
			Operation.Replace("<Variable>", "m_" + Name);
		else
			Operation.Replace("<Variable>", Name);
		

		Operation.Replace("<VariableName>", Name);
		Operation.Replace("<Type>", pTypeInfo->CType);

		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);


	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeToJsonOperation(CString Type, CString Name, CString ParentName, CString ParentShortName, CString SSTIDEnumName, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.ToJsonOperation;

		if (IsMFCheck)
		{
			RemoveBlock(Operation, "<IfNotMFCheck>", "</IfNotMFCheck>");
			Operation.Replace("<IfMFCheck>", "");
			Operation.Replace("</IfMFCheck>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfMFCheck>", "</IfMFCheck>");
			Operation.Replace("<IfNotMFCheck>", "");
			Operation.Replace("</IfNotMFCheck>", "");
		}

		if (IsInDataObject)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotDataObject>", "</IfNotDataObject>");
			Operation.Replace("<IfDataObject>", "");
			Operation.Replace("</IfDataObject>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfDataObject>", "</IfDataObject>");
			Operation.Replace("<IfNotDataObject>", "");
			Operation.Replace("</IfNotDataObject>", "");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, "<IfNot64Bit>", "</IfNot64Bit>");
			Operation.Replace("<If64Bit>", "");
			Operation.Replace("</If64Bit>", "");
		}
		else
		{
			RemoveBlock(Operation, "<If64Bit>", "</If64Bit>");
			Operation.Replace("<IfNot64Bit>", "");
			Operation.Replace("</IfNot64Bit>", "");
		}

		CString SST_ID;


		SST_ID.Format("SST_%s_%s", ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();



		//CString VarName=Name;
		if (IsArray)
		{
			RemoveBlock(Operation, "<IfNotArray>", "</IfNotArray>");
			Operation.Replace("<IfArray>", "");
			Operation.Replace("</IfArray>", "");

			CString ArrayVar = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
			ArrayVar.Replace("<Index>", "i");
			Operation.Replace("<Variable>", ArrayVar);
		}
		else
		{
			RemoveBlock(Operation, "<IfArray>", "</IfArray>");
			Operation.Replace("<IfNotArray>", "");
			Operation.Replace("</IfNotArray>", "");
		}

		if (IsInDataObject)
			Operation.Replace("<Variable>", "m_" + Name);
		else
			Operation.Replace("<Variable>", Name);


		Operation.Replace("<VariableName>", Name);

		Operation.Replace("<Type>", pTypeInfo->CType);


		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}
CString CCallInterfaceMakerDlg::MakeFromJsonOperation(CString Type, CString Name, CString ParentName, CString ParentShortName, CString SSTIDEnumName, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.FromJsonOperation;

		if (IsMonitorUpdate)
		{
			Operation.Replace("<IfMonitorUpdate>", "");
			Operation.Replace("</IfMonitorUpdate>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfMonitorUpdate>", "</IfMonitorUpdate>");
		}

		if (IsInDataObject)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotDataObject>", "</IfNotDataObject>");
			Operation.Replace("<IfDataObject>", "");
			Operation.Replace("</IfDataObject>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfDataObject>", "</IfDataObject>");
			Operation.Replace("<IfNotDataObject>", "");
			Operation.Replace("</IfNotDataObject>", "");
		}

		if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, "<IfNot64Bit>", "</IfNot64Bit>");
			Operation.Replace("<If64Bit>", "");
			Operation.Replace("</If64Bit>", "");
		}
		else
		{
			RemoveBlock(Operation, "<If64Bit>", "</If64Bit>");
			Operation.Replace("<IfNot64Bit>", "");
			Operation.Replace("</IfNot64Bit>", "");
		}

		CString SST_ID;
		SST_ID.Format("SST_%s_%s", ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();


		if (IsArray)
		{
			RemoveBlock(Operation, "<IfNotArray>", "</IfNotArray>");
			Operation.Replace("<IfArray>", "");
			Operation.Replace("</IfArray>", "");

			Operation.Replace("<Variable>", "ArrayElement");
		}
		else
		{
			RemoveBlock(Operation, "<IfArray>", "</IfArray>");
			Operation.Replace("<IfNotArray>", "");
			Operation.Replace("</IfNotArray>", "");
		}

		if (IsInDataObject)
			Operation.Replace("<Variable>", "m_" + Name);
		else
			Operation.Replace("<Variable>", Name);


		Operation.Replace("<VariableName>", Name);
		Operation.Replace("<Type>", pTypeInfo->CType);

		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);


	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeMsgHandlerMethods(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];

		CString MethodName = MethodInfo.Name;
		bool IsExport = true;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType != INTERFACE_METHOD_EXPORT_TYPE_CALL)			
			{
				MethodName = MethodInfo.Name + "Ack";
			}
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
				IsExport = false;
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
				IsExport = false;
			break;
		}

		if (IsExport)
		{
			CString Method = m_MsgHandlerMethodHeaderTemple;
			Method.Replace("<MethodName>", MethodName);
			Methods += Method + "\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgHandlerMethodsSource(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + "Ack";
	else
		InterfaceName = InterfaceInfo.Name;

	CString Methods;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString MethodName = MethodInfo.Name;
		vector<METHOD_PARAM> * pParamList = NULL;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
			{
				pParamList = &MethodInfo.CallParamList;
			}				
			else
			{
				pParamList = &MethodInfo.AckParamList;
				MethodName = MethodInfo.Name + "Ack";
			}				
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
				pParamList = &MethodInfo.CallParamList;
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
				pParamList = &MethodInfo.CallParamList;			
			break;
		}

		if (pParamList)
		{
			CString Method = m_MsgHandlerMethodSourceTemple;

			if (pParamList->size())
			{
				Method.Replace("<IfHaveMember>", "");
				Method.Replace("</IfHaveMember>", "");
			}
			else
			{
				RemoveBlock(Method, "<IfHaveMember>", "</IfHaveMember>");
			}


			Method.Replace("<InterfaceName>", InterfaceName);
			Method.Replace("<@InterfaceName>", InterfaceInfo.Name);
			Method.Replace("<MethodName>", MethodName);
			CString Params = MakeParams(*pParamList, false, true);
			Method.Replace("<Params>", Params);
			CString LineSpace = GetLineSpace(Method, "<ParamDefines>");
			CString ParamDefines = MakeParamDefines(*pParamList, LineSpace);
			Method.Replace("<ParamDefines>", ParamDefines);

			LineSpace = GetLineSpace(Method, "<ParamInitOperations>");
			CString ParamInitOperations = MakeInitOperations(MethodInfo, ExportType, LineSpace);
			Method.Replace("<ParamInitOperations>", ParamInitOperations);

			LineSpace = GetLineSpace(Method, "<UnpackOperations>");
			CString SSTIDEnumName;
			SSTIDEnumName.Format("%s_%s_MEMBER_IDS", ClassNameToUpper(InterfaceName), ClassNameToUpper(MethodName));
			CString UnpackOperations = MakeUnpackOperations(MethodInfo, ExportType, SSTIDEnumName, LineSpace);
			Method.Replace("<UnpackOperations>", UnpackOperations);

			Methods += Method + "\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgMapInits(LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + "Ack";
	else
		InterfaceName = InterfaceInfo.Name;
	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString InterfaceNameUp = ClassNameToUpper(InterfaceName);
	CString InterfaceIDEnumName;
	CString InterfaceID;
	CString ModuleID;
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(InterfaceInfo.ModuleID);
	if (pModuleInfo)
	{
		InterfaceIDEnumName.Format("%s_INTERFACES", ClassNameToUpper(pModuleInfo->Name));
		InterfaceID.Format("%s_INTERFACE_%s", ClassNameToUpper(pModuleInfo->Name), ClassNameToUpper(InterfaceInfo.Name));	
		ModuleID = "MODULE_ID_" + ClassNameToUpper(pModuleInfo->Name);
	}

	
	

	CString MethodIDEnumName;
	MethodIDEnumName.Format("%s_METHODS", ClassNameToUpper(InterfaceInfo.Name));

	CString MapInits;
	for(size_t j=0;j<InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo=InterfaceInfo.MethodList[j];
		CString MapInit=m_MsgMapInitTemple;

		CString MethodName = MethodInfo.Name;
		bool IsExport = true;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType != INTERFACE_METHOD_EXPORT_TYPE_CALL)
			{
				MethodName = MethodInfo.Name + "Ack";
			}
			break;
		case INTERFACE_METHOD_TYPE_NOTIFY:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
				IsExport = false;
			break;
		case INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT:
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
				IsExport = false;
			break;
		}
		
		if (IsExport)
		{
			CString MethodNameUp = ClassNameToUpper(MethodName);

			MapInit.Replace("<ModuleID>", ModuleID);
			MapInit.Replace("<InterfaceName>", InterfaceName);
			MapInit.Replace("<MethodName>", MethodName);
			MapInit.Replace("<!SpaceName>", SpaceNameUp);
			MapInit.Replace("<!InterfaceName>", InterfaceNameUp);
			MapInit.Replace("<!MethodName>", MethodNameUp);
			MapInit.Replace("<@InterfaceName>", InterfaceInfo.Name);

			MapInit.Replace("<InterfaceIDEnumName>", InterfaceIDEnumName);			
			MapInit.Replace("<InterfaceID>", InterfaceID);
			CString MethodID;
			MethodID.Format("METHOD_%s", ClassNameToUpper(MethodInfo.Name));
			MapInit.Replace("<MethodID>", MethodID);
			MapInit.Replace("<MethodIDEnumName>", MethodIDEnumName);
			MapInit.Replace("<IsAckMsg>", ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK ? "true" : "false");

			MapInits += MapInit + "\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	MapInits.Replace("\r\n",Space);
	return MapInits;
}

CString CCallInterfaceMakerDlg::MakeStructDefines(vector<STRUCT_DEFINE_INFO>& StructDefineList,LPCTSTR szLineSpace)
{
	CString StructDefines;
	for(size_t i=0;i<StructDefineList.size();i++)
	{		
		if ((StructDefineList[i].Flag&STRUCT_FLAG_IS_DATA_OBJECT) == 0)
		{
			CString Struct=m_StructDefineTemplate;

			CString LineSpace;

			CString SSTIDEnumName;
			SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructDefineList[i].Name));

			if (StructDefineList[i].Flag&STRUCT_FLAG_EXPORT_XML_PROCESS)
			{
				LineSpace = GetLineSpace(Struct, "<XMLProcess>");
				CString XMLProcess = MakeXMLProcess(StructDefineList[i], SSTIDEnumName, LineSpace);
				Struct.Replace("<XMLProcess>", XMLProcess);
			}
			else
			{
				Struct.Replace("<XMLProcess>", "");
			}

			if (StructDefineList[i].Flag&STRUCT_FLAG_EXPORT_JSON_PROCESS)
			{
				LineSpace = GetLineSpace(Struct, "<JsonProcess>");
				CString JsonProcess = MakeJsonProcess(StructDefineList[i], SSTIDEnumName, LineSpace);
				Struct.Replace("<JsonProcess>", JsonProcess);
			}
			else
			{
				Struct.Replace("<JsonProcess>", "");
			}

			if(StructDefineList[i].MemberList.size())
			{
				Struct.Replace("<IfHaveMember>","");
				Struct.Replace("</IfHaveMember>","");
			}
			else
			{
				RemoveBlock(Struct,"<IfHaveMember>","</IfHaveMember>");
			}

			if (StructDefineList[i].BaseStruct.IsEmpty())
			{
				RemoveBlock(Struct, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
				Struct.Replace("<IfNotHaveBaseClass>", "");
				Struct.Replace("</IfNotHaveBaseClass>", "");
				Struct.Replace("<BaseClass>", "");

			}
			else
			{
				Struct.Replace("<IfHaveBaseClass>", "");
				Struct.Replace("</IfHaveBaseClass>", "");
				RemoveBlock(Struct, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
				Struct.Replace("<BaseClass>", StructDefineList[i].BaseStruct);
			}

			LineSpace=GetLineSpace(Struct,"<StructDescription>");
			CString StructDescription=ToComment(StructDefineList[i].Description,LineSpace);
			Struct.Replace("<StructDescription>",StructDescription);
			Struct.Replace("<StructName>",StructDefineList[i].Name);
			CString BaseStruct;
			if(!StructDefineList[i].BaseStruct.IsEmpty())
			{
				BaseStruct=":"+StructDefineList[i].BaseStruct;
			}
			Struct.Replace("<BaseStruct>",BaseStruct);
			LineSpace=GetLineSpace(Struct,"<Members>");
			CString Members=MakeStructMembers(StructDefineList[i],LineSpace);
			Struct.Replace("<Members>",Members);

			LineSpace=GetLineSpace(Struct,"<SSTIDEnumDefine>");
			CString SSTIDEnum=MakeStructSSTIDEnumDefines(StructDefineList[i],LineSpace);
			Struct.Replace("<SSTIDEnumDefine>",SSTIDEnum);

			LineSpace=GetLineSpace(Struct,"<InitOperations>");
			CString InitOperations=MakeInitOperations(StructDefineList[i],LineSpace);
			Struct.Replace("<InitOperations>",InitOperations);

			

			LineSpace=GetLineSpace(Struct,"<PackOperations>");
			CString PackOperations=MakePackOperations(StructDefineList[i],SSTIDEnumName,LineSpace);
			Struct.Replace("<PackOperations>",PackOperations);

			LineSpace=GetLineSpace(Struct,"<UnpackOperations>");
			CString UnpackOperations=MakeUnpackOperations(StructDefineList[i],SSTIDEnumName,LineSpace);
			Struct.Replace("<UnpackOperations>",UnpackOperations);

			LineSpace=GetLineSpace(Struct,"<PacketSizes>");
			CString PackSizes=MakePackSizes(StructDefineList[i],LineSpace);
			Struct.Replace("<PacketSizes>",PackSizes);

			//LineSpace = GetLineSpace(Struct, "<ToXMLOperations>");
			//CString ToXMLOperations = MakeToXMLOperations(StructDefineList[i], SSTIDEnumName, LineSpace);
			//Struct.Replace("<ToXMLOperations>", ToXMLOperations);

			//LineSpace = GetLineSpace(Struct, "<FromXMLOperations>");
			//CString FromXMLOperations = MakeFromXMLOperations(StructDefineList[i], SSTIDEnumName, LineSpace);
			//Struct.Replace("<FromXMLOperations>", FromXMLOperations);

			

			LineSpace=GetLineSpace(Struct,"<CloneFromOperations>");
			CString CloneOperations=MakeCloneOperations(StructDefineList[i],LineSpace,"","Value.");
			Struct.Replace("<CloneFromOperations>",CloneOperations);

			LineSpace=GetLineSpace(Struct,"<CloneToOperations>");
			CloneOperations=MakeCloneOperations(StructDefineList[i],LineSpace,"CloneObject.","");
			Struct.Replace("<CloneToOperations>",CloneOperations);

			StructDefines+=Struct+"\r\n\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	StructDefines.Replace("\r\n",Space);
	return StructDefines;
}

CString CCallInterfaceMakerDlg::MakeStructMembers(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Members;
	int MaxLen=0;
	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		if(pTypeInfo)
		{
			if(pTypeInfo->CType.GetLength()>MaxLen)
				MaxLen=pTypeInfo->CType.GetLength();
		}
		
	}
	CString MemberTemplate;
	if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
		MemberTemplate = m_DataObjectMemberTemplate;
	else
		MemberTemplate = m_StructMemberTemplate;

	int HeadLen = GetHeadLen(MemberTemplate, "<VariableDefine>");
	MaxLen+=HeadLen;
	int SpaceLen=(((int)ceil(MaxLen/4.0f))+1)*4-HeadLen;
	
	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		CString Member = MemberTemplate;
		
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[i].Type);
		CString Space;
		if(pTypeInfo)
		{
			CString VariableDefine=pTypeInfo->GenerateOperations.VariableDefine;

			if(StructInfo.MemberList[i].IsArray)
			{
				VariableDefine=m_InterfaceConfig.ArrayDefineConfig.VariableDefine;
			}

			VariableDefine.Replace("<Type>",pTypeInfo->CType);
			int CurSpaceCount=(int)ceil((SpaceLen-pTypeInfo->CType.GetLength())/4.0f);
			while(CurSpaceCount)
			{
				Space+="	";
				CurSpaceCount--;
			}
			VariableDefine.Replace("<Space>",Space);
			CString VarName;
			
				
			VarName=StructInfo.MemberList[i].Name;
			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				VarName="m_"+VarName;
			}
			VariableDefine.Replace("<Variable>",VarName);
			Member.Replace("<VariableDefine>",VariableDefine);
		}
		CString Description=StructInfo.MemberList[i].Description;
		if(!Description.IsEmpty())
		{
			Description="//"+Description;
			Description.Replace("\r","");
			Description.Replace("\n","");
		}
		Member.Replace("<Description>",Description);

		Members+=Member+"\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Members.Replace("\r\n",Space);
	return Members;
}

CString CCallInterfaceMakerDlg::MakeCloneOperations(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace,LPCTSTR szLeftValue,LPCTSTR szRightValue)
{
	CString Source;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Operation=pTypeInfo->GenerateOperations.CloneOperation;
			if(StructInfo.MemberList[j].IsArray)
				Operation=m_InterfaceConfig.ArrayDefineConfig.CloneOperation;

			if(StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)
			{
				RemoveBlock(Operation,"<IfMonitorUpdate>","</IfMonitorUpdate>");
			}
			else
			{
				Operation.Replace("<IfMonitorUpdate>","");
				Operation.Replace("</IfMonitorUpdate>","");
			}

			if(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)
			{
				RemoveBlock(Operation,"<IfNotDataObject>","</IfNotDataObject>");
				Operation.Replace("<IfDataObject>","");
				Operation.Replace("</IfDataObject>","");
			}
			else
			{
				RemoveBlock(Operation,"<IfDataObject>","</IfDataObject>");
				Operation.Replace("<IfNotDataObject>","");
				Operation.Replace("</IfNotDataObject>","");
			}

			if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_64BIT)
			{
				RemoveBlock(Operation, "<IfNot64Bit>", "</IfNot64Bit>");
				Operation.Replace("<If64Bit>", "");
				Operation.Replace("</If64Bit>", "");
			}
			else
			{
				RemoveBlock(Operation, "<If64Bit>", "</If64Bit>");
				Operation.Replace("<IfNot64Bit>", "");
				Operation.Replace("</IfNot64Bit>", "");
			}

			if (pTypeInfo->Flag&(TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
			{
				RemoveBlock(Operation, "<IfNotRefType>", "</IfNotRefType>");
				Operation.Replace("<IfRefType>", "");
				Operation.Replace("</IfRefType>", "");
			}
			else
			{
				RemoveBlock(Operation, "<IfRefType>", "</IfRefType>");
				Operation.Replace("<IfNotRefType>", "");
				Operation.Replace("</IfNotRefType>", "");
			}

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>","");
				Operation.Replace("</IfInDataObject>","");
			}
			else
			{
				RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>","");
				Operation.Replace("</IfNotInDataObject>","");
			}

			Operation.Replace("<LeftValue>",szLeftValue);
			Operation.Replace("<RightValue>",szRightValue);

			Operation.Replace("<Type>",pTypeInfo->CType);
			Operation.Replace("<VariableName>",StructInfo.MemberList[j].Name);
			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				Operation.Replace("<Variable>","m_"+StructInfo.MemberList[j].Name);
			else
				Operation.Replace("<Variable>",StructInfo.MemberList[j].Name);
			Operation.Replace("<ClassName>",StructInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));			
			Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

			CString ModifyFlag="MF_"+ClassNameToUpper(StructInfo.MemberList[j].Name);
			Operation.Replace("<ModifyFlag>",ModifyFlag);

			Source+=Operation+"\r\n";
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

bool CCallInterfaceMakerDlg::MakeDataObjectFlagIndicesEnum(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& AllDataStructList, ENUM_DEFINE_INFO& EnumInfo)
{

	EnumInfo.Name="DATA_OBJECT_FLAGS";
	EnumInfo.ShortName=EnumInfo.Name;

	vector<STRUCT_DEFINE_INFO *> DataObjectList;
	GetDataObjectListSorted(AllDataStructList, DataObjectList);

	for (size_t i = 0; i<DataObjectList.size(); i++)
	{
		ENUM_MEMBER_INFO MemberInfo;
		CString MemberName;
		MemberName = ClassNameToUpper(DataObjectList[i]->Name);
		MemberInfo.Name.Format("DATA_OBJECT_FLAG_%s", MemberName);

		EnumInfo.MemberList.push_back(MemberInfo);
	}
	
	ENUM_MEMBER_INFO MemberInfo;
	MemberInfo.Name="DATA_OBJECT_FLAG_MAX";
	EnumInfo.MemberList.push_back(MemberInfo);
	
	return true;
}

bool CCallInterfaceMakerDlg::MakeDataObjectSSTIDsEnum(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, ENUM_DEFINE_INFO& EnumInfo)
{
	EnumInfo.Name="DATA_OBJECT_SSTIDS";
	EnumInfo.ShortName=EnumInfo.Name;

	vector<STRUCT_DEFINE_INFO *> DataObjectList;
	GetDataObjectListSorted(DataStructList, DataObjectList);

	for (size_t i = 0; i<DataObjectList.size(); i++)
	{
		ENUM_MEMBER_INFO MemberInfo;
		CString MemberName;
		MemberName = ClassNameToUpper(DataObjectList[i]->Name);
		MemberInfo.Name.Format("SST_DO_%s", MemberName);
		MemberInfo.Value.Format("%d", DataObjectList[i]->ObjectID);

		EnumInfo.MemberList.push_back(MemberInfo);
	}	
	return true;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagEnumDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{

	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));
	EnumInfo.ShortName=EnumInfo.Name;
	EnumInfo.Flag|=ENUM_DEFINE_FLAG_IS_64BIT;

	EnumInfo.MemberList.resize(StructInfo.MemberList.size()+1);

	CString EnumType="UINT64";
	TYPE_DEFINE * pType=FindVarType(EnumType);
	if(pType)
		EnumType=pType->CType;

	UINT64 MF_ALL=0;
	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		CString MemberName;
		MemberName=ClassNameToUpper(StructInfo.MemberList[j].Name);
		EnumInfo.MemberList[j].Name.Format("MF_%s",MemberName);
		EnumInfo.MemberList[j].Value.Format("(((%s)1)<<%d)",EnumType,j);
		MF_ALL=(MF_ALL<<1)|1;
	}
	EnumInfo.MemberList[StructInfo.MemberList.size()].Name="MF_ALL";
	EnumInfo.MemberList[StructInfo.MemberList.size()].Value.Format("0x%llX",MF_ALL);

	return MakeEnumDefine(EnumInfo,szLineSpace);

	
}

CString CCallInterfaceMakerDlg::MakeDataObjectMembersGetModifyFlag(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Source;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo&&(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)&&
			((StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)==0)&&
			(!StructInfo.MemberList[j].IsArray))
		{
			CString Temp;

			Temp.Format("m_%s.GetModifyFlag(ModifyFlags,GetType,MemberFlags);\r\n",StructInfo.MemberList[j].Name);

			Source+=Temp;
		}		
	}


	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataDataObjectMembersIsModified(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Source;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo&&(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)&&
			((StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)==0)&&
			(!StructInfo.MemberList[j].IsArray))
		{
			CString Temp;

			Temp.Format("||m_%s.IsModified(MemberFlags)\r\n",StructInfo.MemberList[j].Name);

			Source+=Temp;
		}		
	}

	if(Source.IsEmpty())
		Source="||false";

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataDataObjectMembersClearModifyFlag(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Source;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo&&(pTypeInfo->Flag&TYPE_DEFINE_FLAG_DATA_OBJECT)&&
			((StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)==0)&&
			(!StructInfo.MemberList[j].IsArray))
		{
			CString Temp;

			Temp.Format("m_%s.ClearModifyFlag(MemberFlags);\r\n",StructInfo.MemberList[j].Name);

			Source+=Temp;
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}



CString CCallInterfaceMakerDlg::MakeDataObjectSetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Methods;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Method=pTypeInfo->GenerateOperations.SetMethodDeclare;
			if(StructInfo.MemberList[j].IsArray)
				Method=m_InterfaceConfig.ArrayDefineConfig.SetMethodDeclare;

			Method.Replace("<Type>",pTypeInfo->CType);
			Method.Replace("<VariableName>",StructInfo.MemberList[j].Name);

			Methods+=Method+"\r\n";
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectSetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Methods;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Method=pTypeInfo->GenerateOperations.SetMethodDefine;
			if(StructInfo.MemberList[j].IsArray)
				Method=m_InterfaceConfig.ArrayDefineConfig.SetMethodDefine;

			if(StructInfo.MemberList[j].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)
			{
				RemoveBlock(Method,"<IfMonitorUpdate>","</IfMonitorUpdate>");
			}
			else
			{
				Method.Replace("<IfMonitorUpdate>","");
				Method.Replace("</IfMonitorUpdate>","");
			}

			if (pTypeInfo->Flag&(TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
			{
				RemoveBlock(Method, "<IfNotRefType>", "</IfNotRefType>");
				Method.Replace("<IfRefType>", "");
				Method.Replace("</IfRefType>", "");
			}
			else
			{
				RemoveBlock(Method, "<IfRefType>", "</IfRefType>");
				Method.Replace("<IfNotRefType>", "");
				Method.Replace("</IfNotRefType>", "");
			}

			if (pTypeInfo->Flag&TYPE_DEFINE_FLAG_64BIT)
			{
				RemoveBlock(Method, "<IfNot64Bit>", "</IfNot64Bit>");
				Method.Replace("<If64Bit>", "");
				Method.Replace("</If64Bit>", "");
			}
			else
			{
				RemoveBlock(Method, "<If64Bit>", "</If64Bit>");
				Method.Replace("<IfNot64Bit>", "");
				Method.Replace("</IfNot64Bit>", "");
			}

			Method.Replace("<Type>",pTypeInfo->CType);
			Method.Replace("<VariableName>",StructInfo.MemberList[j].Name);
			Method.Replace("<Variable>","m_"+StructInfo.MemberList[j].Name);
			Method.Replace("<ClassName>",StructInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));			
			Method.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

			CString ModifyFlag="MF_"+ClassNameToUpper(StructInfo.MemberList[j].Name);
			Method.Replace("<ModifyFlag>",ModifyFlag);

			Methods+=Method+"\r\n";
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectGetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Methods;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Method=pTypeInfo->GenerateOperations.GetMethodDeclare;
			if(StructInfo.MemberList[j].IsArray)
				Method=m_InterfaceConfig.ArrayDefineConfig.GetMethodDeclare;

			Method.Replace("<Type>",pTypeInfo->CType);
			Method.Replace("<VariableName>",StructInfo.MemberList[j].Name);
			CString ConstMethod = Method;
			Method.Replace("<const>", "");
			Methods += Method + "\r\n";

			if (ConstMethod.Find("<const>") >= 0)
			{
				if (pTypeInfo->Flag&(TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
				{
					ConstMethod.Replace("<const>", "const");
					Methods += ConstMethod + "\r\n";
				}
			}
		}
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectGetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace)
{
	CString Methods;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Method=pTypeInfo->GenerateOperations.GetMethodDefine;
			if(StructInfo.MemberList[j].IsArray)
				Method=m_InterfaceConfig.ArrayDefineConfig.GetMethodDefine;

			Method.Replace("<Type>",pTypeInfo->CType);
			Method.Replace("<VariableName>",StructInfo.MemberList[j].Name);
			Method.Replace("<Variable>","m_"+StructInfo.MemberList[j].Name);
			Method.Replace("<ClassName>",StructInfo.Name);
			CString ConstMethod = Method;			
			Method.Replace("<const>", "");
			Methods+=Method+"\r\n";

			if (ConstMethod.Find("<const>") >= 0)
			{
				if (pTypeInfo->Flag&(TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
				{
					ConstMethod.Replace("<const>", "const");
					Methods += ConstMethod + "\r\n";
				}
			}
		}		
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlags(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& AllDataStructList, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags, LPCTSTR szLineSpace)
{
	CString Source;

	vector<STRUCT_DEFINE_INFO *> DataObjectList;

	GetDataObjectListSorted(AllDataStructList, DataObjectList);
	

	UINT ModifyFlagDefineSize=0;

	for (size_t i = 0; i<AllDataStructList.size(); i++)
	{
		if (AllDataStructList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)AllDataStructList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if (pStructList->StructList[j].Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				{
					CString Define=m_DataObjectModifyFlagDefineTemple;

					DATA_OBJECT_MODIFY_FLAG ModifyFlag=MakeDataObjectFullModifyFlag(pStructList->StructList[j],DataObjectList);		

					ModifyFlagDefineSize=ModifyFlag.ModifyFlag.size();

					ModifyFlag.Name.Format("DOMF_%s_FULL",ClassNameToUpper(pStructList->StructList[j].Name));
					Define.Replace("<FlagName>",ModifyFlag.Name);


					CString Space=GetLineSpace(Define,"<FlagData>");
					CString FlagData=MakeDataObjectModifyFlagData(ModifyFlag,Space);
					Define.Replace("<FlagData>",FlagData);

					Source+=Define+"\r\n";
				}
			}
		}
	}

	for (size_t i = 0; i<DataObjectModifyFlags.size(); i++)
	{
		
		CString Define=m_DataObjectModifyFlagDefineTemple;		

		if (DataObjectModifyFlags[i].ModifyFlag.size() != ModifyFlagDefineSize)
		{
			CString Msg;
			Msg.Format("[%s]的大小不正确,", DataObjectModifyFlags[i].Name);
			AfxMessageBox(Msg);
		}
		
		Define.Replace("<FlagName>", DataObjectModifyFlags[i].Name);


		CString Space=GetLineSpace(Define,"<FlagData>");
		CString FlagData = MakeDataObjectModifyFlagData(DataObjectModifyFlags[i], Space);
		Define.Replace("<FlagData>",FlagData);

		Source+=Define+"\r\n";
		
	}
	
	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagData(DATA_OBJECT_MODIFY_FLAG& ModifyFlag,LPCTSTR szLineSpace)
{
	CString Source;

	for(size_t i=0;i<ModifyFlag.ModifyFlag.size();i++)
	{
		CString Member;
		for(size_t j=0;j<ModifyFlag.ModifyFlag[i].Members.size();j++)
		{
			
			CString FlagUnit=m_DataObjectModifyFlagUnitTemple;

			FlagUnit.Replace("<ClassName>",ModifyFlag.ModifyFlag[i].ClassName);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(ModifyFlag.ModifyFlag[i].ClassName));			
			FlagUnit.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

			CString Flag;
			Flag.Format("MF_%s",ClassNameToUpper(ModifyFlag.ModifyFlag[i].Members[j]));
			FlagUnit.Replace("<ModifyFlag>",Flag);
			if(j)
				Member+="|";
			Member+=FlagUnit;
		}
		if(Member.IsEmpty())
			Member="0";
		if(i)
			Source+=",\r\n";
		Source+=Member;
	}

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeEnumDefine(ENUM_DEFINE_INFO& EnumInfo,LPCTSTR szLineSpace)
{
	CString Source=m_EnumDefineTemplate;

	Source.Replace("<EnumName>",EnumInfo.Name);
	
	if(EnumInfo.Description.IsEmpty())
		Source.Replace("<Description>","");
	else
		Source.Replace("<Description>","//"+EnumInfo.Description);

	CString TypeCaster = "";

	if(EnumInfo.Flag&ENUM_DEFINE_FLAG_IS_64BIT)
	{
		TYPE_DEFINE * pType=FindVarType("UINT64");
		if (pType)
		{
			Source.Replace("<EnumType>", ":" + pType->CType);
			TypeCaster = "(" + pType->CType + ")";
		}
	}
	else
	{
		Source.Replace("<EnumType>","");
	}
	

	CString Space=GetLineSpace(Source,"<EnumMembers>");	

	CString EnumMembers;

	UINT MF_ALL=0;
	for(size_t i=0;i<EnumInfo.MemberList.size();i++)
	{
		
		CString Member=m_EnumMemberDefineTemplate;

		Member.Replace("<Name>",EnumInfo.MemberList[i].Name);

		CString EnumValue = EnumInfo.MemberList[i].Value;
		
		EnumValue.Replace("<EnumType>", TypeCaster);

		if (EnumValue.IsEmpty())
			Member.Replace("<Value>","");
		else
			Member.Replace("<Value>", "=" + EnumValue);

		if(EnumInfo.MemberList[i].Description.IsEmpty())
			Member.Replace("<Description>","");
		else
			Member.Replace("<Description>","//"+EnumInfo.MemberList[i].Description);

		if(i)
			EnumMembers+=Space;
		EnumMembers+=Member+"\r\n";		
	}	

	Source.Replace("<EnumMembers>",EnumMembers);

	Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeEnumDefines(vector<ENUM_DEFINE_INFO>& EnumDefineList,LPCTSTR szLineSpace)
{
	CString EnumDefines;
	for(size_t i=0;i<EnumDefineList.size();i++)
	{		
		CString Enum=MakeEnumDefine(EnumDefineList[i],"");		

		EnumDefines+=Enum+"\r\n\r\n";
		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	EnumDefines.Replace("\r\n",Space);
	return EnumDefines;
}

CString CCallInterfaceMakerDlg::MakeEnumStrValue(ENUM_DEFINE_INFO& EnumInfo,LPCTSTR szLineSpace)
{
	CString Source=m_EnumStrValueDefineTemplate;

	Source.Replace("<EnumName>",EnumInfo.Name);

	CString MemberCount;

	MemberCount.Format("%d",EnumInfo.MemberList.size());

	Source.Replace("<EnumMemberCount>",MemberCount);


	CString StrValues;

	for(size_t i=0;i<EnumInfo.MemberList.size();i++)
	{
		if(i)
			StrValues+=",";

		StrValues+=MakeStringConst("\""+EnumInfo.MemberList[i].StrValue+"\"");
	}	

	Source.Replace("<EnumStrValues>",StrValues);

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeConstDefines(vector<CONST_DEFINE_INFO>& ConstDefineList,LPCTSTR szLineSpace)
{
	CString Source;
	
	
	for(size_t i=0;i<ConstDefineList.size();i++)
	{
		TYPE_DEFINE * pType=FindVarType(ConstDefineList[i].Type);
		CString Const;
		if(pType)
		{
			Const=m_ConstDefineTemplate;

			Const.Replace("<Type>",pType->CType);
		}
		else
		{
			Const=m_MacroDefineTemplate;
		}

		if(ConstDefineList[i].Description.IsEmpty())
			Const.Replace("<Description>","");
		else
			Const.Replace("<Description>","\\"+ConstDefineList[i].Description);
		Const.Replace("<Name>",ConstDefineList[i].Name);
		CString Value=ConstDefineList[i].Value;
		Value.Trim();
		if(Value.GetLength()&&Value[0]=='"'&&Value[Value.GetLength()-1]=='"')
			Value=MakeStringConst(Value);		
		Const.Replace("<Value>",Value);

		Source+=Const+"\r\n\r\n";

	}	

	

	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeStringConst(LPCTSTR StrValue)
{
	CString Value=m_StringConstDefineTemplate;

	Value.Replace("<StrValue>",StrValue);

	return Value;
}

CString CCallInterfaceMakerDlg::MakeEnumStrValues(vector<ENUM_DEFINE_INFO>& EnumDefineList,LPCTSTR szLineSpace)
{
	CString EnumDefines;
	for(size_t i=0;i<EnumDefineList.size();i++)
	{		
		if(EnumDefineList[i].Flag&ENUM_DEFINE_FLAG_EXPORT_STR_VALUE)
		{
			CString StrValues=MakeEnumStrValue(EnumDefineList[i],"");

			EnumDefines+=StrValues+"\r\n\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	EnumDefines.Replace("\r\n",Space);
	return EnumDefines;
}

DATA_OBJECT_MODIFY_FLAG CCallInterfaceMakerDlg::MakeDataObjectFullModifyFlag(STRUCT_DEFINE_INFO& StructInfo,vector<STRUCT_DEFINE_INFO *>& DataObjectList)
{
	DATA_OBJECT_MODIFY_FLAG ModifyFlag;

	ModifyFlag.ModifyFlag.resize(DataObjectList.size());

	for(size_t i=0;i<DataObjectList.size();i++)
	{
		if(DataObjectList[i]->Name==StructInfo.Name||HaveMemberByType(StructInfo,DataObjectList[i]->Name))
		{
			ModifyFlag.ModifyFlag[i].ClassName=DataObjectList[i]->Name;
			for(size_t j=0;j<DataObjectList[i]->MemberList.size();j++)
			{
				if((DataObjectList[i]->MemberList[j].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)==0)
				{
					ModifyFlag.ModifyFlag[i].Members.push_back(DataObjectList[i]->MemberList[j].Name);
				}
			}
		}
	}

	return ModifyFlag;
}

bool CCallInterfaceMakerDlg::HaveMemberByType(STRUCT_DEFINE_INFO& StructInfo, CString Type, int Depth)
{
	if(StructInfo.BaseStruct==Type)
		return true;
	Depth--;
	if (Depth <= 0)
	{
		//AfxMessageBox("结构递归过多，可能有循环嵌套");
		return false;
	}

	STRUCT_DEFINE_INFO * pStructInfo = NULL;
	for(size_t i=0;i<StructInfo.MemberList.size();i++)
	{
		if(StructInfo.MemberList[i].Type==Type)
			return true;		
	}
	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		pStructInfo = GetStructDefineInfo(StructInfo.MemberList[i].Type);
		if (pStructInfo)
		{
			if (HaveMemberByType(*pStructInfo, Type, Depth))
				return true;
		}
	}
	pStructInfo = GetStructDefineInfo(StructInfo.BaseStruct);
	if (pStructInfo)
	{
		if (HaveMemberByType(*pStructInfo, Type, Depth))
			return true;
	}
	return false;
}

STRUCT_DEFINE_INFO * CCallInterfaceMakerDlg::GetStructDefineInfo(CString Type)
{
	for (size_t i = 0; i<m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)m_AllDataStructList[i];
			for(size_t j=0;j<pStructList->StructList.size();j++)
			{
				if(pStructList->StructList[j].Name==Type)
					return &(pStructList->StructList[j]);
			}
		}
	}	
	return NULL;
}

void CCallInterfaceMakerDlg::GetDataObjectDefineList(vector<STRUCT_DEFINE_INFO *>& DataObjectList)
{
	GetDataObjectListSorted(m_AllDataStructList, DataObjectList);	
}

MODULE_DEFINE_INFO * CCallInterfaceMakerDlg::GetModuleInfo(UINT ModuleID)
{
	if (m_MainModule.ModuleID == ModuleID)
		return &m_MainModule;
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		if (m_ImportModuleList[i].ModuleID == ModuleID)
			return &m_ImportModuleList[i];
	}
	return &m_MainModule;
}

MODULE_DEFINE_INFO * CCallInterfaceMakerDlg::GetModuleInfoByName(LPCTSTR szModuleName)
{
	if (m_MainModule.Name == szModuleName)
		return &m_MainModule;
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		if (m_ImportModuleList[i].Name == szModuleName)
			return &m_ImportModuleList[i];
	}
	return &m_MainModule;
}

LPCTSTR CCallInterfaceMakerDlg::GetModuleName(UINT ModuleID)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(ModuleID);
	if (pModuleInfo)
		return pModuleInfo->Name;
	return "未知";
}

CALLER_INTERFACE * CCallInterfaceMakerDlg::AddInterfaceInfo(CALLER_INTERFACE& InterfaceInfo)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(InterfaceInfo.ModuleID);
	if (pModuleInfo)
	{
		InterfaceInfo.ID = pModuleInfo->IDSeed;
		InterfaceInfo.ModuleID = pModuleInfo->ModuleID;
		pModuleInfo->IDSeed++;
		pModuleInfo->InterfaceList.push_back(InterfaceInfo);
		RefreshInterfaceCache();
		return pModuleInfo->InterfaceList.end()._Ptr;
	}
	return NULL;
}
CALLER_INTERFACE * CCallInterfaceMakerDlg::GetInterfaceInfoByID(UINT ModuleID, UINT InterfaceID)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(ModuleID);
	if (pModuleInfo)
	{
		for (size_t i = 0; i < pModuleInfo->InterfaceList.size(); i++)
		{
			if (pModuleInfo->InterfaceList[i].ID == InterfaceID)
			{
				return &pModuleInfo->InterfaceList[i];
			}
		}
	}
	return NULL;
}
CALLER_INTERFACE * CCallInterfaceMakerDlg::GetInterfaceInfoByName(LPCTSTR ModuleName, LPCTSTR InterfaceName)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfoByName(ModuleName);
	if (pModuleInfo)
	{
		for (size_t i = 0; i < pModuleInfo->InterfaceList.size(); i++)
		{
			if (pModuleInfo->InterfaceList[i].Name == InterfaceName)
			{
				return &pModuleInfo->InterfaceList[i];
			}
		}
	}
	return NULL;
}
bool CCallInterfaceMakerDlg::DeleteInterfaceInfoByID(UINT ModuleID, UINT InterfaceID)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(ModuleID);
	if (pModuleInfo)
	{
		for (size_t i = 0; i < pModuleInfo->InterfaceList.size(); i++)
		{
			if (pModuleInfo->InterfaceList[i].ID == InterfaceID)
			{
				for (size_t j = 0; j < m_AllInterfaceList.size(); j++)
				{
					if (m_AllInterfaceList[j] == &(pModuleInfo->InterfaceList[i]))
					{
						m_AllInterfaceList.erase(m_AllInterfaceList.begin() + j);
						break;
					}
				}
				pModuleInfo->InterfaceList.erase(pModuleInfo->InterfaceList.begin() + i);
				return true;
			}
		}
	}
	return false;
}

BASE_DATA_STRUCT_DEFINE_LIST * CCallInterfaceMakerDlg::NewDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(ModuleID);
	if (pModuleInfo)
	{
		BASE_DATA_STRUCT_DEFINE_LIST * pDataStructList = NULL;
		switch (Type)
		{
		case DATA_STRUCT_CONST:
			pDataStructList = new CONST_DEFINE_LIST;
			pDataStructList->ListType = DATA_STRUCT_CONST;
			break;
		case DATA_STRUCT_ENUM:			
			pDataStructList = new ENUM_DEFINE_LIST;
			pDataStructList->ListType = DATA_STRUCT_ENUM;
			break;
		case DATA_STRUCT_STRUCT:
			pDataStructList = new STRUCT_DEFINE_LIST;
			pDataStructList->ListType = DATA_STRUCT_STRUCT;
			break;		
		}
		if (pDataStructList)
		{
			pDataStructList->ModuleID = pModuleInfo->ModuleID;
			pDataStructList->ListName = Name;
			pModuleInfo->DataStructDefineList.push_back(pDataStructList);
			return pDataStructList;
		}
	}
	return NULL;
}
BASE_DATA_STRUCT_DEFINE_LIST * CCallInterfaceMakerDlg::GetDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(ModuleID);
	if (pModuleInfo)
	{
		for (size_t i = 0; i < pModuleInfo->DataStructDefineList.size(); i++)
		{
			if (pModuleInfo->DataStructDefineList[i]->ListName == Name)
				return pModuleInfo->DataStructDefineList[i];
		}
	}
	return NULL;
}
bool CCallInterfaceMakerDlg::DeleteDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(ModuleID);
	if (pModuleInfo)
	{
		for (size_t i = 0; i < pModuleInfo->DataStructDefineList.size(); i++)
		{
			if (pModuleInfo->DataStructDefineList[i]->ListName == Name)
			{
				for (size_t j = 0; j < m_AllDataStructList.size(); j++)
				{
					if (m_AllDataStructList[j] == pModuleInfo->DataStructDefineList[i])
					{
						m_AllDataStructList.erase(m_AllDataStructList.begin() + j);
						break;
					}
				}
				pModuleInfo->DataStructDefineList.erase(pModuleInfo->DataStructDefineList.begin() + i);
				return true;
			}
				
		}
	}
	return false;
}

bool CCallInterfaceMakerDlg::DataStructListMoveUp(BASE_DATA_STRUCT_DEFINE_LIST * pList)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(pList->ModuleID);
	if (pModuleInfo)
	{
		for (size_t i = 0; i < pModuleInfo->DataStructDefineList.size(); i++)
		{
			if (pModuleInfo->DataStructDefineList[i] == pList)
			{
				if (i > 0)
				{
					pModuleInfo->DataStructDefineList.erase(pModuleInfo->DataStructDefineList.begin() + i);
					pModuleInfo->DataStructDefineList.insert(pModuleInfo->DataStructDefineList.begin() + i - 1, pList);
					return true;
				}				
				return false;
			}

		}
	}
	return false;
}
bool CCallInterfaceMakerDlg::DataStructListMoveDown(BASE_DATA_STRUCT_DEFINE_LIST * pList)
{
	MODULE_DEFINE_INFO * pModuleInfo = GetModuleInfo(pList->ModuleID);
	if (pModuleInfo)
	{
		for (size_t i = 0; i < pModuleInfo->DataStructDefineList.size(); i++)
		{
			if (pModuleInfo->DataStructDefineList[i] == pList)
			{
				if (i < pModuleInfo->DataStructDefineList.size() - 1)
				{
					pModuleInfo->DataStructDefineList.erase(pModuleInfo->DataStructDefineList.begin() + i);
					pModuleInfo->DataStructDefineList.insert(pModuleInfo->DataStructDefineList.begin() + i + 1, pList);
					return true;
				}
				return false;
			}

		}
	}
	return false;
}

bool CCallInterfaceMakerDlg::DataStructListMoveToModule(BASE_DATA_STRUCT_DEFINE_LIST * pList, MODULE_DEFINE_INFO * pModuleInfo)
{
	MODULE_DEFINE_INFO * pOldModuleInfo = GetModuleInfo(pList->ModuleID);
	if (pOldModuleInfo)
	{
		for (size_t i = 0; i < pOldModuleInfo->DataStructDefineList.size(); i++)
		{
			if (pOldModuleInfo->DataStructDefineList[i] == pList)
			{
				pOldModuleInfo->DataStructDefineList.erase(pOldModuleInfo->DataStructDefineList.begin() + i);
				break;
			}
		}
		pModuleInfo->DataStructDefineList.push_back(pList);
		pList->ModuleID = pModuleInfo->ModuleID;		
		return true;
	}
	return false;
}
bool CCallInterfaceMakerDlg::AddModule(LPCTSTR szFileName)
{
	if (stricmp(m_MainModule.ModuleDefineFileName, szFileName) == 0)
	{
		AfxMessageBox("无法添加主模块作为引用模块");
		return false;
	}
	MODULE_DEFINE_INFO * pModuleIndo = LoadModule(szFileName);
	if (pModuleIndo)
	{
		CEasyString RelativePath = CFileTools::GetRelativePath(CFileTools::GetPathDirectory(m_MainModule.ModuleDefineFileName), szFileName);
		m_MainModule.ImportList.push_back((LPCTSTR)RelativePath);
		return true;
	}
	return false;
}

bool CCallInterfaceMakerDlg::DeleteModule(UINT ModuleID)
{
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		if (m_ImportModuleList[i].ModuleID == ModuleID)
		{
			m_ImportModuleList[i].Clear();
			m_ImportModuleList.erase(m_ImportModuleList.begin() + i);
			return true;
		}
	}
	return false;
}

void CCallInterfaceMakerDlg::OnNMDblclkInterfaceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnBnClickedButtonEditInterface();
}

void CCallInterfaceMakerDlg::OnBnClickedButtonConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgConfig Dlg;
	Dlg.m_Config=m_InterfaceConfig;
	if(Dlg.DoModal()==IDOK)
	{
		m_InterfaceConfig=Dlg.m_Config;
		SaveConfig();
	}
}

CString CCallInterfaceMakerDlg::GetLineSpace(CString& TempleStr,LPCTSTR szWord)
{
	CString Space;

	int Pos=TempleStr.Find(szWord);
	if(Pos>=0)
	{
		Space=TempleStr.Left(Pos);
		int SpaceLen=0;
		for(int i=Space.GetLength()-1;i>=0;i--)
		{
			if(Space[i]!=' '&&Space[i]!='	')
				break;
			SpaceLen++;
		}
		Space=Space.Right(SpaceLen);
	}
	return Space;
}
int CCallInterfaceMakerDlg::GetHeadLen(CString& TempleStr,LPCTSTR szWord)
{
	CString Head;
	int HeadLen=0;

	int Pos=TempleStr.Find(szWord);
	if(Pos>=0)
	{
		Head=TempleStr.Left(Pos);
		for(int i=Head.GetLength()-1;i>=0;i--)
		{
			if(Head[i]=='\r'||Head[i]=='\n')
				break;
			HeadLen++;
		}
	}
	return HeadLen;
}
CString CCallInterfaceMakerDlg::ToComment(CString& Content,LPCTSTR szLineSpace)
{
	CString ToComment="//"+Content;
	CString Space="\r\n//";
	Space+=szLineSpace;
	ToComment.Replace("\r\n",Space);
	return ToComment;
}

CString CCallInterfaceMakerDlg::ClassNameToFileName(LPCTSTR szClassName)
{
	CString Output=szClassName;
	Output.Trim();
	if(Output.GetLength()>=2)
	{
		if(Output[0]=='C'&&IsBigChar(Output[1]))
			Output.Delete(0);
	}
	return Output;
}

CString CCallInterfaceMakerDlg::ClassNameToUpper(LPCTSTR szClassName)
{
	CString Output=ClassNameToFileName(szClassName);
	for(int i=Output.GetLength()-1;i>0;i--)
	{
		if (IsBigChar(Output[i]) && i && (!IsBigChar(Output[i - 1])) && (Output[i - 1] != '_'))
		{
			Output.Insert(i,'_');
		}
	}
	Output.MakeUpper();
	return Output;
}

bool CCallInterfaceMakerDlg::IsBigChar(TCHAR c)
{
	return c>='A'&&c<='Z';
}


void CCallInterfaceMakerDlg::OnBnClickedButtonSelectAllInterface()
{
	// TODO: 在此添加控件通知处理程序代码

	for(int i=0;i<m_lvInterfaceList.GetItemCount();i++)
	{
		m_lvInterfaceList.SetCheck(i,m_SelectAll);
	}
	m_SelectAll=!m_SelectAll;
}



void CCallInterfaceMakerDlg::RemoveBlock(CString& Template,LPCTSTR szBlockStart,LPCTSTR szBlockEnd)
{
	int StartPos=Template.Find(szBlockStart);
	while(StartPos>=0)
	{
		int EndPos=Template.Find(szBlockEnd);
		if(EndPos<0)
			return;
		EndPos+=strlen(szBlockEnd);
		if(EndPos<Template.GetLength()-1&&Template[EndPos]=='\r'&&Template[EndPos+1]=='\n')
			EndPos+=2;
		int CheckPos=StartPos;
		bool NeedCutHeader = (CheckPos>0);
		CheckPos--;
		while(CheckPos>=0)
		{
			if (Template[CheckPos] == '\r' || Template[CheckPos] == '\n')
				break;
			else if (Template[CheckPos] != ' '&&Template[CheckPos] != '\t')
			{
				NeedCutHeader = false;
				break;
			}
			CheckPos--;
		}
		if(NeedCutHeader)
		{
			while(StartPos>0)
			{
				if(Template[StartPos-1]!=' '&&Template[StartPos-1]!='\t')//&&Template[StartPos-1]!='\r'&&Template[StartPos-1]!='\n')
					break;
				StartPos--;
			}
		}
		Template.Delete(StartPos,EndPos-StartPos);
		StartPos=Template.Find(szBlockStart);
	}
}

CString CCallInterfaceMakerDlg::RestoreFileToTemplate(LPCTSTR szFileName,const CString& szTemplate)
{
	CFileStatus FileStatus;
	if(CFile::GetStatus(szFileName,FileStatus))
	{
		CStringFile InputFile;
		if (InputFile.LoadFile(szFileName, false))
		{
			return RestoreToTemplate(InputFile.GetData(), szTemplate);
		}
	}
	
	return szTemplate;
}

CString CCallInterfaceMakerDlg::RestoreToTemplate(LPCTSTR szInput,const CString& szTemplate)
{
	CString Output=szInput;
	int BlockIndex=1;
	while(true)
	{
		CString BlockStart;
		CString BlockEnd;
		BlockStart.Format("<GenerateArea%dStart>",BlockIndex);
		BlockEnd.Format("<GenerateArea%dEnd>",BlockIndex);
		int TemplateStartPos=szTemplate.Find(BlockStart);
		int TemplateEndPos=szTemplate.Find(BlockEnd);
		int InputStartPos=Output.Find(BlockStart);
		int InputEndPos=Output.Find(BlockEnd);
		if(TemplateStartPos>=0&&TemplateEndPos>=0&&InputStartPos>=0&&InputEndPos>=0)
		{
			CString TemplateBlock=szTemplate.Mid(TemplateStartPos,TemplateEndPos-TemplateStartPos+BlockEnd.GetLength());
			Output.Delete(InputStartPos,InputEndPos-InputStartPos+BlockEnd.GetLength());
			Output.Insert(InputStartPos,TemplateBlock);
		}
		else
		{
			break;
		}
		BlockIndex++;
	}
	return Output;
}

void CCallInterfaceMakerDlg::GetDataStructPacketList(int Type,vector<CString>& NameList)
{
	for (size_t i = 0; i<m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == Type)
			NameList.push_back(m_AllDataStructList[i]->ListName);
	}
}

bool CCallInterfaceMakerDlg::MoveDataStruct(int Type,CString SourcePacket,CString TargetPacket,CString Name)
{
	BASE_DATA_STRUCT_DEFINE_LIST * pSourcePacket=NULL;
	BASE_DATA_STRUCT_DEFINE_LIST * pTargetPacket=NULL;
	for (size_t i = 0; i<m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == Type&&m_AllDataStructList[i]->ListName == SourcePacket)
			pSourcePacket = m_AllDataStructList[i];
		if (m_AllDataStructList[i]->ListType == Type&&m_AllDataStructList[i]->ListName == TargetPacket)
			pTargetPacket = m_AllDataStructList[i];
		if(pSourcePacket&&pTargetPacket)
			break;
	}
	if(pSourcePacket&&pTargetPacket)
	{
		switch(Type)
		{
		case DATA_STRUCT_CONST:
			{
				CONST_DEFINE_LIST * pSourceList=(CONST_DEFINE_LIST *)pSourcePacket;
				CONST_DEFINE_LIST * pTargetList=(CONST_DEFINE_LIST *)pTargetPacket;
				for(size_t i=0;i<pSourceList->ConstList.size();i++)
				{
					if(pSourceList->ConstList[i].Name==Name)
					{
						pTargetList->ConstList.push_back(pSourceList->ConstList[i]);
						pSourceList->ConstList.erase(pSourceList->ConstList.begin()+i);
						return true;
					}
				}
			}
			break;
		case DATA_STRUCT_ENUM:
			{
				ENUM_DEFINE_LIST * pSourceList=(ENUM_DEFINE_LIST *)pSourcePacket;
				ENUM_DEFINE_LIST * pTargetList=(ENUM_DEFINE_LIST *)pTargetPacket;
				for(size_t i=0;i<pSourceList->EnumList.size();i++)
				{
					if(pSourceList->EnumList[i].Name==Name)
					{
						pTargetList->EnumList.push_back(pSourceList->EnumList[i]);
						pSourceList->EnumList.erase(pSourceList->EnumList.begin()+i);
						return true;
					}
				}
			}
			break;
		case DATA_STRUCT_STRUCT:
			{
				STRUCT_DEFINE_LIST * pSourceList=(STRUCT_DEFINE_LIST *)pSourcePacket;
				STRUCT_DEFINE_LIST * pTargetList=(STRUCT_DEFINE_LIST *)pTargetPacket;
				for(size_t i=0;i<pSourceList->StructList.size();i++)
				{
					if(pSourceList->StructList[i].Name==Name)
					{
						pTargetList->StructList.push_back(pSourceList->StructList[i]);
						pSourceList->StructList.erase(pSourceList->StructList.begin()+i);
						return true;
					}
				}
			}
			break;
		}
	}
	return false;
}

void CCallInterfaceMakerDlg::OnBnClickedButtonDataStructDef()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgDataStructDefine Dlg;

	Dlg.m_DataStructDefineList = m_AllDataStructList;
	Dlg.m_pDataObjectModifyFlags = &m_MainModule.DataObjectModifyFlags;
	Dlg.DoModal();
	RefreshDataStructCache();
}






void CCallInterfaceMakerDlg::OnBnClickedButtonArrangeIds()
{
	// TODO:  在此添加控件通知处理程序代码
	if(AfxMessageBox("是否要重排所有的ID，这将使接口和旧版本不兼容？",MB_YESNO)==IDYES)
	{
		for (size_t i = 0; i < m_AllModuleList.size(); i++)
		{
			ArrangeIds(*m_AllModuleList[i]);
		}
		SaveInterfaceSelections();
		FillListItem();
		RestoreInterfaceSelections();
	}
}

int CCallInterfaceMakerDlg::InterfaceInfoComp(LPCVOID p1, LPCVOID p2)
{
	const CALLER_INTERFACE * pInterfaceInfo1 = (const CALLER_INTERFACE *)p1;
	const CALLER_INTERFACE * pInterfaceInfo2 = (const CALLER_INTERFACE *)p2;
	return strcmp(pInterfaceInfo1->Name, pInterfaceInfo2->Name);
}

int CCallInterfaceMakerDlg::InterfaceMethodComp(LPCVOID p1, LPCVOID p2)
{
	const INTERFACE_METHOD * pMethodInfo1 = (const INTERFACE_METHOD *)p1;
	const INTERFACE_METHOD * pMethodInfo2 = (const INTERFACE_METHOD *)p2;
	return strcmp(pMethodInfo1->Name, pMethodInfo2->Name); 
}

int CCallInterfaceMakerDlg::StructComp(LPCVOID p1, LPCVOID p2)
{
	const STRUCT_DEFINE_INFO * pInfo1 = (const STRUCT_DEFINE_INFO *)p1;
	const STRUCT_DEFINE_INFO * pInfo2 = (const STRUCT_DEFINE_INFO *)p2;
	return strcmp(pInfo1->Name, pInfo2->Name);
}

int CCallInterfaceMakerDlg::EnumComp(LPCVOID p1, LPCVOID p2)
{
	const ENUM_DEFINE_INFO * pInfo1 = (const ENUM_DEFINE_INFO *)p1;
	const ENUM_DEFINE_INFO * pInfo2 = (const ENUM_DEFINE_INFO *)p2;
	return strcmp(pInfo1->Name, pInfo2->Name);
}

int CCallInterfaceMakerDlg::ConstComp(LPCVOID p1, LPCVOID p2)
{
	const CONST_DEFINE_INFO * pInfo1 = (const CONST_DEFINE_INFO *)p1;
	const CONST_DEFINE_INFO * pInfo2 = (const CONST_DEFINE_INFO *)p2;
	return strcmp(pInfo1->Name, pInfo2->Name);
}

void CCallInterfaceMakerDlg::OnBnClickedButtonSort()
{
	// TODO:  在此添加控件通知处理程序代码
	SaveInterfaceSelections();

	qsort(m_MainModule.InterfaceList.begin()._Ptr, m_MainModule.InterfaceList.size(), sizeof(CALLER_INTERFACE), InterfaceInfoComp);
	RefreshInterfaceCache();

	FillListItem();

	RestoreInterfaceSelections();
}


void CCallInterfaceMakerDlg::OnBnClickedButtonSortAll()
{
	// TODO:  在此添加控件通知处理程序代码
	SaveInterfaceSelections();
	qsort(m_MainModule.InterfaceList.begin()._Ptr, m_MainModule.InterfaceList.size(), sizeof(CALLER_INTERFACE), InterfaceInfoComp);
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		qsort(m_ImportModuleList[i].InterfaceList.begin()._Ptr, m_ImportModuleList[i].InterfaceList.size(), sizeof(CALLER_INTERFACE), InterfaceInfoComp);
	}

	for (size_t i = 0; i <m_AllInterfaceList.size(); i++)
	{
		CALLER_INTERFACE * pInterfaceInfo = m_AllInterfaceList[i];
		qsort(pInterfaceInfo->MethodList.begin()._Ptr, pInterfaceInfo->MethodList.size(), sizeof(INTERFACE_METHOD), InterfaceMethodComp);
	}

	for (size_t i = 0; i < m_AllDataStructList.size(); i++)
	{
		switch (m_AllDataStructList[i]->ListType)
		{
		case DATA_STRUCT_CONST:
			{
				CONST_DEFINE_LIST * pConstList = (CONST_DEFINE_LIST *)m_AllDataStructList[i];
				qsort(pConstList->ConstList.begin()._Ptr, pConstList->ConstList.size(), sizeof(CONST_DEFINE_INFO), ConstComp);
			}
			break;
		case DATA_STRUCT_ENUM:
			{
				ENUM_DEFINE_LIST * pEnumList = (ENUM_DEFINE_LIST *)m_AllDataStructList[i];
				qsort(pEnumList->EnumList.begin()._Ptr, pEnumList->EnumList.size(), sizeof(ENUM_DEFINE_INFO), EnumComp);
			}
			break;
		case DATA_STRUCT_STRUCT:
		{
			STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)m_AllDataStructList[i];
			qsort(pStructList->StructList.begin()._Ptr, pStructList->StructList.size(), sizeof(STRUCT_DEFINE_INFO), StructComp);
		}
		break;
		}
	}
	RefreshDataCache();
	FillListItem();
	RestoreInterfaceSelections();
}


void CCallInterfaceMakerDlg::OnBnClickedButtonImportEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgImportEditor Dlg;

	Dlg.DoModal();

	SaveInterfaceSelections();
	RefreshDataCache();
	MakeCurVarType(true);
	FillListItem();
	RestoreInterfaceSelections();
}

void CCallInterfaceMakerDlg::ArrangeIds(MODULE_DEFINE_INFO& ModuleInfo)
{
	ModuleInfo.IDSeed = 1;
	for (size_t i = 0; i < ModuleInfo.InterfaceList.size(); i++)
	{
		CALLER_INTERFACE& InterfaceInfo = ModuleInfo.InterfaceList[i];
		InterfaceInfo.ID = ModuleInfo.IDSeed;
		ModuleInfo.IDSeed++;
		InterfaceInfo.IDSeed = 1;
		for (size_t j = 0; j < InterfaceInfo.MethodList.size(); j++)
		{
			INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];
			MethodInfo.ID = InterfaceInfo.IDSeed;
			InterfaceInfo.IDSeed++;
			MethodInfo.ParamIDSeed = 1;
			for (size_t k = 0; k < MethodInfo.CallParamList.size(); k++)
			{
				METHOD_PARAM& ParamInfo = MethodInfo.CallParamList[k];
				ParamInfo.ID = MethodInfo.ParamIDSeed;
				MethodInfo.ParamIDSeed++;
			}
			for (size_t k = 0; k < MethodInfo.AckParamList.size(); k++)
			{
				METHOD_PARAM& ParamInfo = MethodInfo.AckParamList[k];
				ParamInfo.ID = MethodInfo.ParamIDSeed;
				MethodInfo.ParamIDSeed++;
			}
		}
	}

	for (size_t i = 0; i < ModuleInfo.DataStructDefineList.size(); i++)
	{
		if (ModuleInfo.DataStructDefineList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST * pStructList = (STRUCT_DEFINE_LIST *)ModuleInfo.DataStructDefineList[i];
			for (size_t j = 0; j < pStructList->StructList.size(); j++)
			{
				STRUCT_DEFINE_INFO& StructInfo = pStructList->StructList[j];
				StructInfo.IDSeed = 1;
				for (size_t k = 0; k < StructInfo.MemberList.size(); k++)
				{
					STRUCT_MEMBER_INFO& MemberInfo = StructInfo.MemberList[k];
					MemberInfo.ID = StructInfo.IDSeed;
					StructInfo.IDSeed++;
				}
			}
		}
	}
}

bool CCallInterfaceMakerDlg::CheckBaseStructObjectID(vector<UINT>& ObjectIDList, LPCTSTR szBaseStructName)
{
	STRUCT_DEFINE_INFO * pStructInfo = GetStructDefineInfo(szBaseStructName);
	if (pStructInfo)
	{
		for (size_t i = 0; i < ObjectIDList.size(); i++)
		{
			if (pStructInfo->ObjectID == ObjectIDList[i])
				return false;
		}		
		ObjectIDList.push_back(pStructInfo->ObjectID);
		if (!pStructInfo->BaseStruct.IsEmpty())
			return CheckBaseStructObjectID(ObjectIDList, pStructInfo->BaseStruct);
		return true;
	}
	return false;
}