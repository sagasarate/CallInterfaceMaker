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
	, m_UTF8Export(TRUE)
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
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("utf-8");
	xml_node TypeDef = Doc.append_child(node_element, "TypeDef");

	for (size_t i = 0; i < m_BaseTypeList.size(); i++)
	{
		xml_node Type = TypeDef.append_child(node_element, "Type");

		Type.append_attribute("Name", (LPCTSTR)m_BaseTypeList[i].Name);
		Type.append_attribute("CType", (LPCTSTR)m_BaseTypeList[i].CType);
		Type.append_attribute("Flag", m_BaseTypeList[i].Flag);
		SaveGenerateOperations(Type, m_BaseTypeList[i].GenerateOperations);
	}

	if (!Xml.SaveToFile(Xml.document(), m_TypeDefFileName, CP_UTF8))
	{
		CString Msg;
		Msg.Format("保存文件失败%s", m_TypeDefFileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadTemple(LPCTSTR FileName, CString& TemplateStr)
{
	CStringFile TempleFile;

	CEasyString FilePath = CFileTools::MakeModuleFullPath(NULL, FileName);
	if (TempleFile.LoadFile(FilePath, false))
	{
		TemplateStr = TempleFile.GetData();
		return true;
	}
	else
	{
		CString Msg;
		Msg.Format("无法打开文件%s", (LPCTSTR)FilePath);
		MessageBox(Msg);
		return false;
	}
}
bool CCallInterfaceMakerDlg::LoadTemples()
{
	LoadTemple(ALL_DEFINES_TEMPLE_FILE_NAME, m_AllDefinesTemplate);
	LoadTemple(INTERFACE_HEADER_TEMPLE_FILE_NAME, m_InterfaceHeaderTemplate);
	LoadTemple(INTERFACE_TEMPLE_FILE_NAME, m_InterfaceTemplate);
	LoadTemple(PARAM_DEFINE_TEMPLE_FILE_NAME, m_ParamDefineTemple);
	LoadTemple(PARAM_TRANS_TEMPLE_FILE_NAME, m_ParamTransTemple);
	LoadTemple(PARAM_DESCRIPTION_TEMPLE_FILE_NAME, m_ParamDescriptionTemple);
	LoadTemple(PACK_SIZE_TEMPLE_FILE_NAME, m_PackSizeTemple);
	LoadTemple(METHOD_HEADER_TEMPLE_FILE_NAME, m_MethodHeaderTemple);
	LoadTemple(METHOD_HEADER_PURE_TAIL_TEMPLE_FILE_NAME, m_MethodHeaderPureTailTemple);
	LoadTemple(METHOD_CALLER_SOURCE_TEMPLE_FILE_NAME, m_MethodCallerSourceTemple);
	LoadTemple(MSG_PACK_METHOD_HEADER_TEMPLE_FILE_NAME, m_MsgPackMethodHeaderTemple);
	LoadTemple(MSG_PACK_METHOD_SOURCE_TEMPLE_FILE_NAME, m_MsgPackMethodSourceTemple);
	LoadTemple(MSG_HANDLER_METHOD_HEADER_TEMPLE_FILE_NAME, m_MsgHandlerMethodHeaderTemple);
	LoadTemple(MSG_HANDLER_METHOD_SOURCE_TEMPLE_FILE_NAME, m_MsgHandlerMethodSourceTemple);
	LoadTemple(MSG_MAP_INIT_TEMPLE_FILE_NAME, m_MsgMapInitTemple);
	LoadTemple(MSG_CALLER_HEADER_TEMPLE_FILE_NAME, m_MsgCallerHeaderTemplate);
	LoadTemple(MSG_CALLER_SOURCE_TEMPLE_FILE_NAME, m_MsgCallerSourceTemplate);
	LoadTemple(MSG_HANDLER_HEADER_TEMPLE_FILE_NAME, m_MsgHandlerHeaderTemplate);
	LoadTemple(MSG_HANDLER_SOURCE_TEMPLE_FILE_NAME, m_MsgHandlerSourceTemplate);
	LoadTemple(DATA_OBJECT_DEFINE_HEADER_TEMPLE_FILE_NAME, m_DataObjectDefineHeaderTemple);
	LoadTemple(DATA_OBJECT_DEFINE_SOURCE_TEMPLE_FILE_NAME, m_DataObjectDefineSourceTemple);
	LoadTemple(STRUCT_DEFINE_HEADER_TEMPLE_FILE_NAME, m_StructDefineHeaderTemplate);
	LoadTemple(ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME, m_EnumDefineHeaderTemplate);
	LoadTemple(SYSTEM_ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME, m_SystemEnumDefineHeaderTemplate);
	LoadTemple(STRUCT_DEFINE_TEMPLE_FILE_NAME, m_StructDefineTemplate);
	LoadTemple(STRUCT_MEMBER_TEMPLE_FILE_NAME, m_StructMemberTemplate);
	LoadTemple(DATA_OBJECT_MEMBER_TEMPLE_FILE_NAME, m_DataObjectMemberTemplate);
	LoadTemple(DATA_OBJECT_MODIFY_FLAGS_HEADER_TEMPLE_FILE_NAME, m_DataObjectModifyFlagsHeaderTemple);
	LoadTemple(DATA_OBJECT_MODIFY_FLAGS_SOURCE_TEMPLE_FILE_NAME, m_DataObjectModifyFlagsSourceTemple);
	LoadTemple(DATA_OBJECT_MODIFY_FLAG_HEADER_DEFINE_TEMPLE_FILE_NAME, m_DataObjectModifyFlagHeaderDefineTemple);
	LoadTemple(DATA_OBJECT_MODIFY_FLAG_DEFINE_TEMPLE_FILE_NAME, m_DataObjectModifyFlagDefineTemple);
	LoadTemple(DATA_OBJECT_MODIFY_FLAG_UNIT_TEMPLE_FILE_NAME, m_DataObjectModifyFlagUnitTemple);
	LoadTemple(PACK_OPERATION_UNIT_TEMPLE_FILE_NAME, m_PackOperationUnitTemplate);
	LoadTemple(UNPACK_OPERATION_UNIT_TEMPLE_FILE_NAME, m_UnpackOperationUnitTemplate);
	LoadTemple(TO_XML_OPERATION_UNIT_TEMPLE_FILE_NAME, m_ToXMLOperationUnitTemplate);
	LoadTemple(FROM_XML_OPERATION_UNIT_TEMPLE_FILE_NAME, m_FromXMLOperationUnitTemplate);
	LoadTemple(STRUCT_XML_PROCESS_TEMPLE_FILE_NAME, m_StructXMLProcessTemplate);
	LoadTemple(TO_JSON_OPERATION_UNIT_TEMPLE_FILE_NAME, m_ToJsonOperationUnitTemplate);
	LoadTemple(FROM_JSON_OPERATION_UNIT_TEMPLE_FILE_NAME, m_FromJsonOperationUnitTemplate);
	LoadTemple(STRUCT_JSON_PROCESS_TEMPLE_FILE_NAME, m_StructJsonProcessTemplate);
	LoadTemple(DATA_OBJECT_JSON_PROCESS_TEMPLE_FILE_NAME, m_DataObjectJsonProcessTemplate);
	LoadTemple(DATA_OBJECT_JSON_PROCESS_HEADER_TEMPLE_FILE_NAME, m_DataObjectJsonProcessHeaderTemplate);
	LoadTemple(STRUCT_DB_PROCESS_TEMPLE_FILE_NAME, m_StructDBProcessTemplate);
	LoadTemple(DATA_OBJECT_DB_PROCESS_TEMPLE_FILE_NAME, m_DataObjectDBProcessTemplate);
	LoadTemple(DATA_OBJECT_DB_PROCESS_HEADER_TEMPLE_FILE_NAME, m_DataObjectDBProcessHeaderTemplate);
	LoadTemple(DB_FIELD_NAME_TEMPLE_FILE_NAME, m_DBFieldNameTemplate);
	LoadTemple(DB_FIELD_DEFINE_UNIT_TEMPLE_FILE_NAME, m_DBFieldDefineUnitTemplate);
	LoadTemple(DB_KEY_DEFINE_UNIT_TEMPLE_FILE_NAME, m_DBKeyDefineUnitTemplate);
	LoadTemple(DB_PRIMARY_KEY_DEFINE_UNIT_TEMPLE_FILE_NAME, m_DBPrimaryKeyDefineUnitTemplate);
	LoadTemple(DB_UNIQUE_KEY_DEFINE_UNIT_TEMPLE_FILE_NAME, m_DBUniqueKeyDefineUnitTemplate);
	LoadTemple(ENUM_DFINE_TEMPLE_FILE_NAME, m_EnumDefineTemplate);
	LoadTemple(ENUM_DFINE_64_TEMPLE_FILE_NAME, m_EnumDefine64Template);
	LoadTemple(ENUM_MEMBER_DFINE_TEMPLE_FILE_NAME, m_EnumMemberDefineTemplate);
	LoadTemple(ENUM_MEMBER_DFINE_64_TEMPLE_FILE_NAME, m_EnumMemberDefine64Template);
	LoadTemple(ENUM_BIT_MASK_MEMBER_DFINE_TEMPLE_FILE_NAME, m_EnumBitMaskMemberDefineTemplate);
	LoadTemple(ENUM_BIT_MASK_MEMBER_DFINE_64_TEMPLE_FILE_NAME, m_EnumBitMaskMemberDefine64Template);
	LoadTemple(ENUM_STR_VALUE_UNIT_TEMPLE_FILE_NAME, m_EnumStrValueUnitTemplate);
	LoadTemple(ENUM_LIST_UNIT_TEMPLE_FILE_NAME, m_EnumListUnitTemplate);
	LoadTemple(ENUM_TO_STR_UNIT_TEMPLE_FILE_NAME, m_EnumToStrUnitTemplate);
	LoadTemple(STR_TO_ENUM_UNIT_TEMPLE_FILE_NAME, m_StrToEnumUnitTemplate);
	LoadTemple(ENUM_BIND_DATA_FILL_UNIT_TEMPLE_FILE_NAME, m_EnumBindDataFillUnit);
	//LoadTemple(ENUM_BIND_DATA_FETCH_UNIT_TEMPLE_FILE_NAME, m_EnumBindDataFetchUnit);
	LoadTemple(CONST_DFINE_TEMPLE_FILE_NAME, m_ConstDefineTemplate);
	LoadTemple(MACRO_DFINE_TEMPLE_FILE_NAME, m_MacroDefineTemplate);
	LoadTemple(CONST_DFINE_HEADER_TEMPLE_FILE_NAME, m_ConstDefineHeaderTemplate);
	LoadTemple(STRING_CONST_DFINE_TEMPLE_FILE_NAME, m_StringConstDefineTemplate);
	LoadTemple(IMPORT_DECLARE_TEMPLE_FILE_NAME, m_ImportDeclareTemplate);
	LoadTemple(DATA_OBJECT_CLEAR_MODIFY_FLAG_DEFINE_TEMPLE_FILE_NAME, m_DataObjectClearModifyFlagDefineTemplate);
	LoadTemple(DATA_OBJECT_GET_MODIFY_FLAG_DEFINE_TEMPLE_FILE_NAME, m_DataObjectGetModifyFlagDefineTemplate);
	LoadTemple(DATA_OBJECT_IS_MODIFIED_DEFINE_TEMPLE_FILE_NAME, m_DataObjectIsModifiedDefineTemplate);
	LoadTemple(STRUCT_EDITOR_PROCESS_TEMPLE_FILE_NAME, m_StructEditorProcessTemplate);
	LoadTemple(CREATE_LIST_HEADER_UNIT_TEMPLE_FILE_NAME, m_CreateListHeaderUnitTemplate);
	LoadTemple(FILL_LIST_ITEM_UNIT_TEMPLE_FILE_NAME, m_FillListItemUnitTemplate);	
	//LoadTemple(PROPERTY_GRID_FETCH_UNIT_TEMPLE_FILE_NAME, m_PropertyGridFetchUnitTemplate);
	//LoadTemple(PROPERTY_GRID_ADD_ITEM_UNIT_TEMPLE_FILE_NAME, m_PropertyGridAddItemUnitTemplate);
	//LoadTemple(PROPERTY_GRID_DEL_ITEM_UNIT_TEMPLE_FILE_NAME, m_PropertyGridDelItemUnitTemplate);
	//LoadTemple(PROPERTY_GRID_MOVE_ITEM_UNIT_TEMPLE_FILE_NAME, m_PropertyGridMoveItemUnitTemplate);
	LoadTemple(STRUCT_LOG_PROCESS_TEMPLE_FILE_NAME, m_StructLogProcessTemplate);
	LoadTemple(STRUCT_LUA_PROCESS_TEMPLE_FILE_NAME, m_StructLuaProcessTemplate);
	LoadTemple(DATA_OBJECT_LUA_PROCESS_TEMPLE_FILE_NAME, m_DataObjectLuaProcessTemplate);
	LoadTemple(DATA_OBJECT_LUA_PROCESS_HEADER_TEMPLE_FILE_NAME, m_DataObjectLuaProcessHeaderTemplate);
	LoadTemple(TO_LUA_TABLE_OPERATION_UNIT_TEMPLE_FILE_NAME, m_ToLuaTableOperationUnitTemplate);
	LoadTemple(FROM_LUA_TABLE_OPERATION_UNIT_TEMPLE_FILE_NAME, m_FromLuaTableOperationUnitTemplate);
	LoadTemple(STRUCT_XLS_PROCESS_TEMPLE_FILE_NAME, m_StructXLSProcessTemplate);
	
	

	return true;
}

bool CCallInterfaceMakerDlg::LoadConfig()
{
	xml_parser Parser;

	CString FileName=CFileTools::MakeModuleFullPath(NULL,CONFIG_FILE_NAME);

	if(Parser.parse_file(FileName,0))
	{

		xml_node Config=Parser.document();
			
		if(Config.moveto_child("Config"))
		{
			{
				xml_node OneFileExport = Config;
				if (OneFileExport.moveto_child("OneFileExport"))
				{
					m_InterfaceConfig.OneFileExportConfig.IsExport = (bool)OneFileExport.attribute("IsExport");
					m_InterfaceConfig.OneFileExportConfig.ExportExt = (LPCTSTR)OneFileExport.attribute("FileExt").getvalue();
				}
			}
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
				if(DataObjectModifyFlagsExportConfig.moveto_child("DataObjectModifyFlagsHeaderExport"))
				{
					m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.IsExport=(bool)DataObjectModifyFlagsExportConfig.attribute("IsExport");
					m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt = (LPCTSTR)DataObjectModifyFlagsExportConfig.attribute("FileExt").getvalue();
				}
			}
			{
				xml_node DataObjectModifyFlagsExportConfig = Config;
				if (DataObjectModifyFlagsExportConfig.moveto_child("DataObjectModifyFlagsSourceExport"))
				{
					m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.IsExport = (bool)DataObjectModifyFlagsExportConfig.attribute("IsExport");
					m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.ExportExt = (LPCTSTR)DataObjectModifyFlagsExportConfig.attribute("FileExt").getvalue();
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
				xml_node DefaultEnum64GenerateOperations = Config;
				if (DefaultEnum64GenerateOperations.moveto_child("DefaultEnum64GenerateOperations"))
				{
					LoadGenerateOperations(DefaultEnum64GenerateOperations, m_InterfaceConfig.DefaultEnum64GenerateOperations);
				}
			}
			{
				xml_node ArrayDefineConfig=Config;
				if(ArrayDefineConfig.moveto_child("ArrayDefineConfig"))
				{
					LoadGenerateOperations(ArrayDefineConfig,m_InterfaceConfig.ArrayDefineConfig);
				}
			}
			{
				xml_node OtherConfig = Config;
				if (OtherConfig.moveto_child("OtherConfig"))
				{
					if (OtherConfig.has_attribute("CommentPrefix"))
						m_InterfaceConfig.CommentPrefix = OtherConfig.attribute("CommentPrefix").getvalue();

					if (OtherConfig.has_attribute("LocalVariableDefinePrefix"))
						m_InterfaceConfig.LocalVariableDefinePrefix = OtherConfig.attribute("LocalVariableDefinePrefix").getvalue();

					if (OtherConfig.has_attribute("MemberVariablePrefix"))
						m_InterfaceConfig.MemberVariablePrefix = OtherConfig.attribute("MemberVariablePrefix").getvalue();

					if (OtherConfig.has_attribute("DefaultPacketName"))
						m_InterfaceConfig.DefaultPacketName = OtherConfig.attribute("DefaultPacketName").getvalue();

					if (OtherConfig.has_attribute("DefaultPacketMemberName"))
						m_InterfaceConfig.DefaultPacketMemberName = OtherConfig.attribute("DefaultPacketMemberName").getvalue();

					if (OtherConfig.has_attribute("SupportBigInt"))
						m_InterfaceConfig.SupportBigInt = (bool)OtherConfig.attribute("DefaultPacketMeSupportBigIntmberName");
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
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("utf-8");

	xml_node Config = Doc.append_child(node_element, "Config");

	{
		xml_node InterfaceHeaderExport = Config.append_child(node_element, "OneFileExport");
		InterfaceHeaderExport.append_attribute("IsExport",
			(bool)m_InterfaceConfig.OneFileExportConfig.IsExport);
		InterfaceHeaderExport.append_attribute("FileExt",
			m_InterfaceConfig.OneFileExportConfig.ExportExt);
	}
	{
		xml_node InterfaceHeaderExport = Config.append_child(node_element, "InterfaceHeaderExport");
		InterfaceHeaderExport.append_attribute("IsExport",
			(bool)m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport);
		InterfaceHeaderExport.append_attribute("FileExt",
			m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt);
	}

	{
		xml_node StructExport = Config.append_child(node_element, "StructExport");
		StructExport.append_attribute("IsExport",
			(bool)m_InterfaceConfig.StructExportConfig.IsExport);
		StructExport.append_attribute("FileExt",
			m_InterfaceConfig.StructExportConfig.ExportExt);
	}

	{
		xml_node EnumExport = Config.append_child(node_element, "EnumExport");
		EnumExport.append_attribute("IsExport",
			(bool)m_InterfaceConfig.EnumExportConfig.IsExport);
		EnumExport.append_attribute("FileExt",
			m_InterfaceConfig.EnumExportConfig.ExportExt);
	}

	{
		xml_node ConstExport = Config.append_child(node_element, "ConstExport");
		ConstExport.append_attribute("IsExport",
			(bool)m_InterfaceConfig.ConstExportConfig.IsExport);
		ConstExport.append_attribute("FileExt",
			m_InterfaceConfig.ConstExportConfig.ExportExt);
	}

	{
		xml_node DOHeaderExport = Config.append_child(node_element, "DataStructHeaderExport");
		DOHeaderExport.append_attribute("IsExport",
			(bool)m_InterfaceConfig.DataStructHeaderExportConfig.IsExport);
		DOHeaderExport.append_attribute("FileExt",
			m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt);
	}

	{
		xml_node InterfaceExportConfig = Config.append_child(node_element, "InterfaceExport");
		InterfaceExportConfig.append_attribute("IsExport",
			(bool)m_InterfaceConfig.InterfaceExportConfig.IsExport);
		InterfaceExportConfig.append_attribute("FileExt",
			m_InterfaceConfig.InterfaceExportConfig.ExportExt);
	}

	{
		xml_node CallHeaderExportConfig = Config.append_child(node_element, "CallHeaderExport");
		CallHeaderExportConfig.append_attribute("IsExport",
			(bool)m_InterfaceConfig.CallHeaderExportConfig.IsExport);
		CallHeaderExportConfig.append_attribute("FileExt",
			m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
	}

	{
		xml_node CallSourceExportConfig = Config.append_child(node_element, "CallSourceExport");
		CallSourceExportConfig.append_attribute("IsExport",
			(bool)m_InterfaceConfig.CallSourceExportConfig.IsExport);
		CallSourceExportConfig.append_attribute("FileExt",
			m_InterfaceConfig.CallSourceExportConfig.ExportExt);
	}

	{
		xml_node HandlerHeaderExportConfig = Config.append_child(node_element, "HandlerHeaderExport");
		HandlerHeaderExportConfig.append_attribute("IsExport",
			(bool)m_InterfaceConfig.HandlerHeaderExportConfig.IsExport);
		HandlerHeaderExportConfig.append_attribute("FileExt",
			m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
	}

	{
		xml_node HandlerSourceExportConfig = Config.append_child(node_element, "HandlerSourceExport");
		HandlerSourceExportConfig.append_attribute("IsExport",
			(bool)m_InterfaceConfig.HandlerSourceExportConfig.IsExport);
		HandlerSourceExportConfig.append_attribute("FileExt",
			m_InterfaceConfig.HandlerSourceExportConfig.ExportExt);
	}

	{
		xml_node DataObjectHeaderExportConfig = Config.append_child(node_element, "DataObjectHeaderExport");
		DataObjectHeaderExportConfig.append_attribute("IsExport",
			(bool)m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport);
		DataObjectHeaderExportConfig.append_attribute("FileExt",
			m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);
	}

	{
		xml_node DataObjectSourceExportConfig = Config.append_child(node_element, "DataObjectSourceExport");
		DataObjectSourceExportConfig.append_attribute("IsExport",
			(bool)m_InterfaceConfig.DataObjectSourceExportConfig.IsExport);
		DataObjectSourceExportConfig.append_attribute("FileExt",
			m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
	}

	{
		xml_node DataObjectModifyFlagsExport = Config.append_child(node_element, "DataObjectModifyFlagsHeaderExport");
		DataObjectModifyFlagsExport.append_attribute("IsExport",
			(bool)m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.IsExport);
		DataObjectModifyFlagsExport.append_attribute("FileExt",
			m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt);
	}

	{
		xml_node DataObjectModifyFlagsExport = Config.append_child(node_element, "DataObjectModifyFlagsSourceExport");
		DataObjectModifyFlagsExport.append_attribute("IsExport",
			(bool)m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.IsExport);
		DataObjectModifyFlagsExport.append_attribute("FileExt",
			m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.ExportExt);
	}

	{
		xml_node DefaultStructPackOperation = Config.append_child(node_element, "DefaultStructGenerateOperations");

		SaveGenerateOperations(DefaultStructPackOperation, m_InterfaceConfig.DefaultStructGenerateOperations);
	}

	{
		xml_node DefaultEnumGenerateOperations = Config.append_child(node_element, "DefaultEnumGenerateOperations");

		SaveGenerateOperations(DefaultEnumGenerateOperations, m_InterfaceConfig.DefaultEnumGenerateOperations);
	}
	{
		xml_node DefaultEnum64GenerateOperations = Config.append_child(node_element, "DefaultEnum64GenerateOperations");

		SaveGenerateOperations(DefaultEnum64GenerateOperations, m_InterfaceConfig.DefaultEnum64GenerateOperations);
	}
	{
		xml_node ArrayDefineConfig = Config.append_child(node_element, "ArrayDefineConfig");

		SaveGenerateOperations(ArrayDefineConfig, m_InterfaceConfig.ArrayDefineConfig);
	}

	{
		xml_node OtherConfig = Config.append_child(node_element, "OtherConfig");

		OtherConfig.append_attribute("CommentPrefix",
			m_InterfaceConfig.CommentPrefix);

		OtherConfig.append_attribute("LocalVariableDefinePrefix",
			m_InterfaceConfig.LocalVariableDefinePrefix);

		OtherConfig.append_attribute("MemberVariablePrefix",
			m_InterfaceConfig.MemberVariablePrefix);

		OtherConfig.append_attribute("DefaultPacketName",
			m_InterfaceConfig.DefaultPacketName);

		OtherConfig.append_attribute("DefaultPacketMemberName",
			m_InterfaceConfig.DefaultPacketMemberName);

		OtherConfig.append_attribute("SupportBigInt",
			(bool)m_InterfaceConfig.SupportBigInt);
	}

	CString FileName = CFileTools::MakeModuleFullPath(NULL, CONFIG_FILE_NAME);
	if (!Xml.SaveToFile(Xml.document(), FileName, CP_UTF8))
	{
		CString Msg;
		Msg.Format("保存文件失败%s", FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
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
	
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("utf-8");

	xml_node RunEnvironment = Doc.append_child(node_element, "RunEnvironment");

	RunEnvironment.append_attribute("UTF8Export", m_UTF8Export ? true : false);

	{
		xml_node ExportEnv = RunEnvironment.append_child(node_element, "ExportEnv");
		ExportEnv.append_attribute("RecentExportDir", m_RunEnvInfo.RecentExportDir);

		xml_node DataEnv = RunEnvironment.append_child(node_element, "DataEnv");
		DataEnv.append_attribute("RecentDataDir", m_RunEnvInfo.RecentDataDir);
	}

	xml_node SelectedInterfaces = RunEnvironment.append_child(node_element, "SelectedInterfaces");

	for (size_t i = 0; i < m_RunEnvInfo.SelectedInterfaces.size(); i++)
	{
		SelectedInterfaces.append_child(node_element, m_RunEnvInfo.SelectedInterfaces[i]);
	}

	CString FileName = CFileTools::MakeModuleFullPath(NULL, ENV_FILE_NAME);
	if (!Xml.SaveToFile(Xml.document(), FileName, CP_UTF8))
	{
		CString Msg;
		Msg.Format("保存文件失败%s", FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
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
				//if (pStructList->IgnoreOnExport)
				//	continue;
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
					if (Type.GenerateOperations.ToStringOperation.IsEmpty())
						Type.GenerateOperations.ToStringOperation = m_InterfaceConfig.DefaultStructGenerateOperations.ToStringOperation;
					if (Type.GenerateOperations.DBFieldDefineOperation.IsEmpty())
						Type.GenerateOperations.DBFieldDefineOperation = m_InterfaceConfig.DefaultStructGenerateOperations.DBFieldDefineOperation;
					if (Type.GenerateOperations.DBInsertFormatOperation.IsEmpty())
						Type.GenerateOperations.DBInsertFormatOperation = m_InterfaceConfig.DefaultStructGenerateOperations.DBInsertFormatOperation;
					if (Type.GenerateOperations.DBPutOperation.IsEmpty())
						Type.GenerateOperations.DBPutOperation = m_InterfaceConfig.DefaultStructGenerateOperations.DBPutOperation;
					if (Type.GenerateOperations.DBGetOperation.IsEmpty())
						Type.GenerateOperations.DBGetOperation = m_InterfaceConfig.DefaultStructGenerateOperations.DBGetOperation;
					if (Type.GenerateOperations.PropertyGridFillOperation.IsEmpty())
						Type.GenerateOperations.PropertyGridFillOperation = m_InterfaceConfig.DefaultStructGenerateOperations.PropertyGridFillOperation;
					//if (Type.GenerateOperations.PropertyGridFetchOperation.IsEmpty())
					//	Type.GenerateOperations.PropertyGridFetchOperation = m_InterfaceConfig.DefaultStructGenerateOperations.PropertyGridFetchOperation;
					if (Type.GenerateOperations.LogSendOperation.IsEmpty())
						Type.GenerateOperations.LogSendOperation = m_InterfaceConfig.DefaultStructGenerateOperations.LogSendOperation;
					if (Type.GenerateOperations.ToLuaOperation.IsEmpty())
						Type.GenerateOperations.ToLuaOperation = m_InterfaceConfig.DefaultStructGenerateOperations.ToLuaOperation;
					if (Type.GenerateOperations.FromLuaOperation.IsEmpty())
						Type.GenerateOperations.FromLuaOperation = m_InterfaceConfig.DefaultStructGenerateOperations.FromLuaOperation;
					if (Type.GenerateOperations.CreateXLSColumnOperation.IsEmpty())
						Type.GenerateOperations.CreateXLSColumnOperation = m_InterfaceConfig.DefaultStructGenerateOperations.CreateXLSColumnOperation;
					if (Type.GenerateOperations.CheckXLSColumnOperation.IsEmpty())
						Type.GenerateOperations.CheckXLSColumnOperation = m_InterfaceConfig.DefaultStructGenerateOperations.CheckXLSColumnOperation;
					if (Type.GenerateOperations.ToXLSOperation.IsEmpty())
						Type.GenerateOperations.ToXLSOperation = m_InterfaceConfig.DefaultStructGenerateOperations.ToXLSOperation;
					if (Type.GenerateOperations.FromXLSOperation.IsEmpty())
						Type.GenerateOperations.FromXLSOperation = m_InterfaceConfig.DefaultStructGenerateOperations.FromXLSOperation;

					m_CurVarTypeList.push_back(Type);
				}
			}
		}
		else if (m_AllDataStructList[i]->ListType == DATA_STRUCT_ENUM)
		{
			ENUM_DEFINE_LIST * pEnumList = (ENUM_DEFINE_LIST *)m_AllDataStructList[i];
			for (size_t j = 0; j<pEnumList->EnumList.size(); j++)
			{
				//if (pEnumList->IgnoreOnExport)
				//	continue;

				TYPE_DEFINE Type;
				Type.Name = pEnumList->EnumList[j].Name;
				Type.CType = pEnumList->EnumList[j].Name;
				Type.Flag = TYPE_DEFINE_FLAG_ENUM;
				if (pEnumList->EnumList[j].Flag & ENUM_DEFINE_FLAG_EXPORT_STR_TRANS_FN)
					Type.Flag |= TYPE_DEFINE_FLAG_ENUM_HAVE_STR_TRANS;
				if (pEnumList->EnumList[j].Flag & ENUM_DEFINE_FLAG_IS_64BIT)
				{
					Type.Flag |= TYPE_DEFINE_FLAG_64BIT;
					Type.GenerateOperations = m_InterfaceConfig.DefaultEnum64GenerateOperations;
				}
				else
				{
					Type.GenerateOperations = m_InterfaceConfig.DefaultEnumGenerateOperations;
				}

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

		OnBnClickedButtonSave();
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
	pi.attribute(_T("encoding")) = _T("utf-8");


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

	return Xml.SaveToFile(Xml.document(), ModuleInfo.ModuleDefineFileName, CP_UTF8);
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
			CString Msg;
			Msg.Format("加载失败%s", FileName);
			AfxMessageBox(Msg);
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
			CString Msg;
			Msg.Format("加载失败%s", FileName);
			AfxMessageBox(Msg);
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
				Param.append_attribute("PackFlag", (LPCTSTR)ParamInfo.PackFlag);

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
				Param.append_attribute("PackFlag", (LPCTSTR)ParamInfo.PackFlag);
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
							ParamInfo.PackFlag = ((LPCTSTR)Param.attribute("PackFlag").getvalue());


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
							ParamInfo.PackFlag = ((LPCTSTR)Param.attribute("PackFlag").getvalue());

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
			Member.append_attribute("DBIndexType", StructDefineList[i].MemberList[j].DBIndexType);
			Member.append_attribute("Flag",(UINT)StructDefineList[i].MemberList[j].Flag);
			Member.append_attribute("IsArray",(bool)StructDefineList[i].MemberList[j].IsArray);
			Member.append_attribute("ArrayStartLength",(UINT)StructDefineList[i].MemberList[j].ArrayStartLength);
			Member.append_attribute("ArrayGrowLength",(UINT)StructDefineList[i].MemberList[j].ArrayGrowLength);
			Member.append_attribute("DBLength", (LPCTSTR)StructDefineList[i].MemberList[j].DBLength);
			Member.append_attribute("ShowName", (LPCTSTR)StructDefineList[i].MemberList[j].ShowName);
			Member.append_attribute("Description",(LPCTSTR)StructDefineList[i].MemberList[j].Description);
			Member.append_attribute("PackFlag", (LPCTSTR)StructDefineList[i].MemberList[j].PackFlag);
			Member.append_attribute("ID",(UINT)StructDefineList[i].MemberList[j].ID);			
			Member.append_attribute("BindData", (LPCTSTR)StructDefineList[i].MemberList[j].BindData);
			Member.append_attribute("ExtendType", (LPCTSTR)StructDefineList[i].MemberList[j].ExtendType);
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
				if (_stricmp(Member.name(), "Member") == 0)
				{
					STRUCT_MEMBER_INFO MemberInfo;
					MemberInfo.Name = (LPCTSTR)Member.attribute("Name").getvalue();
					MemberInfo.Type = ((LPCTSTR)Member.attribute("Type").getvalue());
					MemberInfo.DBIndexType = Member.attribute("DBIndexType");
					MemberInfo.Flag = (UINT)Member.attribute("Flag");
					MemberInfo.IsArray = (bool)Member.attribute("IsArray");
					MemberInfo.ArrayStartLength = (UINT)Member.attribute("ArrayStartLength");
					MemberInfo.ArrayGrowLength = (UINT)Member.attribute("ArrayGrowLength");
					MemberInfo.DBLength = (LPCTSTR)Member.attribute("DBLength").getvalue();
					MemberInfo.ShowName = ((LPCTSTR)Member.attribute("ShowName").getvalue());
					MemberInfo.Description = ((LPCTSTR)Member.attribute("Description").getvalue());
					MemberInfo.PackFlag = ((LPCTSTR)Member.attribute("PackFlag").getvalue());
					MemberInfo.ID = (UINT)Member.attribute("ID");
					MemberInfo.BindData = ((LPCTSTR)Member.attribute("BindData").getvalue());
					MemberInfo.ExtendType = ((LPCTSTR)Member.attribute("ExtendType").getvalue());
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
			Member.append_attribute("Flag", EnumDefineList[i].MemberList[j].Flag);
			Member.append_attribute("StrValue",(LPCTSTR)EnumDefineList[i].MemberList[j].StrValue);
			Member.append_attribute("Description",(LPCTSTR)EnumDefineList[i].MemberList[j].Description);
			Member.append_attribute("BindDataType", (LPCTSTR)EnumDefineList[i].MemberList[j].BindDataType);
			
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
					MemberInfo.Flag = (UINT)Member.attribute("Flag");
					MemberInfo.StrValue = ((LPCTSTR)Member.attribute("StrValue").getvalue());
					MemberInfo.Description = ((LPCTSTR)Member.attribute("Description").getvalue());
					MemberInfo.BindDataType = ((LPCTSTR)Member.attribute("BindDataType").getvalue());
					
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
	XmlNode.append_attribute("ToStringOperation", (LPCTSTR)GenerateOperations.ToStringOperation);
	XmlNode.append_attribute("DBFieldDefineOperation", (LPCTSTR)GenerateOperations.DBFieldDefineOperation);
	XmlNode.append_attribute("DBInsertFormatOperation", (LPCTSTR)GenerateOperations.DBInsertFormatOperation);
	XmlNode.append_attribute("DBPutOperation", (LPCTSTR)GenerateOperations.DBPutOperation);
	XmlNode.append_attribute("DBGetOperation", (LPCTSTR)GenerateOperations.DBGetOperation);
	XmlNode.append_attribute("PropertyGridFillOperation", (LPCTSTR)GenerateOperations.PropertyGridFillOperation);
	//XmlNode.append_attribute("PropertyGridFetchOperation", (LPCTSTR)GenerateOperations.PropertyGridFetchOperation);
	XmlNode.append_attribute("LogSendOperation", (LPCTSTR)GenerateOperations.LogSendOperation);
	XmlNode.append_attribute("ToLuaOperation", (LPCTSTR)GenerateOperations.ToLuaOperation);
	XmlNode.append_attribute("FromLuaOperation", (LPCTSTR)GenerateOperations.FromLuaOperation);
	XmlNode.append_attribute("CreateXLSColumnOperation", (LPCTSTR)GenerateOperations.CreateXLSColumnOperation);
	XmlNode.append_attribute("CheckXLSColumnOperation", (LPCTSTR)GenerateOperations.CheckXLSColumnOperation);
	XmlNode.append_attribute("ToXLSOperation", (LPCTSTR)GenerateOperations.ToXLSOperation);
	XmlNode.append_attribute("FromXLSOperation", (LPCTSTR)GenerateOperations.FromXLSOperation);

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
	GenerateOperations.ToStringOperation = ((LPCTSTR)XmlNode.attribute("ToStringOperation").getvalue());
	GenerateOperations.DBFieldDefineOperation = ((LPCTSTR)XmlNode.attribute("DBFieldDefineOperation").getvalue());
	GenerateOperations.DBInsertFormatOperation = ((LPCTSTR)XmlNode.attribute("DBInsertFormatOperation").getvalue());
	GenerateOperations.DBPutOperation = ((LPCTSTR)XmlNode.attribute("DBPutOperation").getvalue());
	GenerateOperations.DBGetOperation = ((LPCTSTR)XmlNode.attribute("DBGetOperation").getvalue());
	GenerateOperations.PropertyGridFillOperation = ((LPCTSTR)XmlNode.attribute("PropertyGridFillOperation").getvalue());
	//GenerateOperations.PropertyGridFetchOperation = ((LPCTSTR)XmlNode.attribute("PropertyGridFetchOperation").getvalue());
	GenerateOperations.LogSendOperation = ((LPCTSTR)XmlNode.attribute("LogSendOperation").getvalue());
	GenerateOperations.ToLuaOperation = ((LPCTSTR)XmlNode.attribute("ToLuaOperation").getvalue());
	GenerateOperations.FromLuaOperation = ((LPCTSTR)XmlNode.attribute("FromLuaOperation").getvalue());
	GenerateOperations.CreateXLSColumnOperation = ((LPCTSTR)XmlNode.attribute("CreateXLSColumnOperation").getvalue());
	GenerateOperations.CheckXLSColumnOperation = ((LPCTSTR)XmlNode.attribute("CheckXLSColumnOperation").getvalue());
	GenerateOperations.ToXLSOperation = ((LPCTSTR)XmlNode.attribute("ToXLSOperation").getvalue());
	GenerateOperations.FromXLSOperation = ((LPCTSTR)XmlNode.attribute("FromXLSOperation").getvalue());

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

void CCallInterfaceMakerDlg::WriteStringToFile(LPCTSTR OutputFileName, CString& Str)
{
	if (m_InterfaceConfig.OneFileExportConfig.IsExport)
	{
		m_OneFileExportCache.Append(Str);
	}
	else
	{
		CStringFile OutputFile;
		if (m_UTF8Export)
			OutputFile.SetSaveCodePage(CP_UTF8);
		if (!OutputFile.SaveToFile(Str, OutputFileName))
		{
			CString Msg;
			Msg.Format("无法打开文件%s", OutputFileName);
			MessageBox(Msg);
		}
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

void CCallInterfaceMakerDlg::SortStructForExport(vector<STRUCT_DEFINE_INFO>& SrcStructList, vector<STRUCT_DEFINE_INFO*>& DestStructList)
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
					AddUnique(DestStructList, &(SrcStructList[j]));
					break;
				}
			}
		}
		AddUnique(DestStructList, &(SrcStructList[s]));
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
				if (pType->Name != StructInfo.Name)
				{
					//排除自我依赖
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
}

void CCallInterfaceMakerDlg::AddUnique(vector<STRUCT_DEFINE_INFO*>& StructList, STRUCT_DEFINE_INFO* pStructInfo)
{
	for (size_t i = 0; i < StructList.size(); i++)
	{
		if (StructList[i]->Name == pStructInfo->Name)
			return;
	}
	StructList.push_back(pStructInfo);
}

void CCallInterfaceMakerDlg::SortDefinesForExport(vector<BASE_DATA_STRUCT_DEFINE_LIST*>& SrcList,
	vector<CONST_DEFINE_LIST*>& ConstList, vector<ENUM_DEFINE_LIST*>& EnumList, vector<STRUCT_DEFINE_LIST2*>& StructList)
{
	//列表间排序
	for (BASE_DATA_STRUCT_DEFINE_LIST* pDefineList : SrcList)
	{
		if (pDefineList->ListType == DATA_STRUCT_CONST)
			AddUnique(ConstList, pDefineList);
	}
	for (BASE_DATA_STRUCT_DEFINE_LIST* pDefineList : SrcList)
	{
		if (pDefineList->ListType == DATA_STRUCT_ENUM)
			AddUnique(EnumList, pDefineList);
	}
	vector<STRUCT_DEFINE_LIST*> TempList;
	for (BASE_DATA_STRUCT_DEFINE_LIST* pDefineList : SrcList)
	{
		if (pDefineList->ListType == DATA_STRUCT_STRUCT)
		{
			vector<CString> DependList;
			GetDefineDepends(SrcList, (STRUCT_DEFINE_LIST*)pDefineList, DependList, 0);
			for (int i = (int)DependList.size() - 1; i >= 0; i--)
			{
				for (size_t j = 0; j < SrcList.size(); j++)
				{
					if (SrcList[j]->ListType == DATA_STRUCT_STRUCT && MakeDefineName((const STRUCT_DEFINE_LIST*)SrcList[j]) == DependList[i])
					{
						AddUnique(TempList, SrcList[j]);
						break;
					}
				}
			}
			AddUnique(TempList, pDefineList);
		}
	}
	//对列表内部排序
	for (STRUCT_DEFINE_LIST* pDefineList : TempList)
	{
		if (pDefineList->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST2* pList = new STRUCT_DEFINE_LIST2();
			*((BASE_DATA_STRUCT_DEFINE_LIST*)pList) = *((BASE_DATA_STRUCT_DEFINE_LIST*)pDefineList);
			SortStructForExport(pDefineList->StructList, pList->StructList);
			StructList.push_back(pList);
		}
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
				//else if (pType->Flag & TYPE_DEFINE_FLAG_ENUM)
				//{
				//	ENUM_DEFINE_INFO* pEnum = GetEnumDefine(pType->CType);
				//	if (pEnum)
				//	{
				//		for (ENUM_MEMBER_INFO& Member : pEnum->MemberList)
				//		{
				//			if (!Member.BindDataType.IsEmpty())
				//			{
				//				if (GetStructDefineInfo(Member.BindDataType))
				//				{
				//					bool IsExist = false;
				//					for (size_t j = 0; j < StructList.size(); j++)
				//					{
				//						if (StructList[j].Name == Member.BindDataType)
				//						{
				//							IsExist = true;
				//							break;
				//						}
				//					}
				//					if (!IsExist)
				//						AddUnique(DependList, Member.BindDataType);
				//				}
				//			}
				//		}
				//	}
				//}
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
				//else if (pType->Flag & TYPE_DEFINE_FLAG_ENUM)
				//{
				//	ENUM_DEFINE_INFO* pEnum = GetEnumDefine(pType->CType);
				//	if (pEnum)
				//	{
				//		for (ENUM_MEMBER_INFO& Member : pEnum->MemberList)
				//		{
				//			if (!Member.BindDataType.IsEmpty())
				//			{
				//				if (GetStructDefineInfo(Member.BindDataType))
				//				{
				//					bool IsExist = false;
				//					for (size_t j = 0; j < StructList.size(); j++)
				//					{
				//						if (StructList[j].Name == Member.BindDataType)
				//						{
				//							IsExist = true;
				//							break;
				//						}
				//					}
				//					if (!IsExist)
				//						AddUnique(DependList, Member.BindDataType);
				//				}
				//			}
				//		}
				//	}
				//}
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

INTERFACE_METHOD_OLD * CCallInterfaceMakerDlg::FindInterfaceMethod(INTERFANCE_DEFINES_OLD& OldInterfaceDefines, CString InterfaceName, LPCTSTR MethodName, bool FindAck)
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
	m_OneFileExportCache.Clear();

	vector<CONST_DEFINE_LIST*> ConstList;
	vector<ENUM_DEFINE_LIST*> EnumList;
	vector<STRUCT_DEFINE_LIST2*> StructList;
	SortDefinesForExport(m_AllDataStructList, ConstList, EnumList, StructList);
	if (ConstList.size() + EnumList.size() + StructList.size() != m_AllDataStructList.size())
	{
		AfxMessageBox("数据定义排序错误");
		return false;
	}

	if (m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport)
	{
		CString FileName;

		FileName.Format("%sInterfaces.%s", (LPCTSTR)m_MainModule.Name, (LPCTSTR)m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt);
		ExportInterfaceHeader(m_AllInterfaceList, ExportDir, FileName);
	}

	if (m_InterfaceConfig.DataStructHeaderExportConfig.IsExport)
	{
		CString FileName;

		FileName.Format("%sDataStructs.%s", (LPCTSTR)m_MainModule.Name, (LPCTSTR)m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt);
		ExportDataStructDefineHeader(ConstList, EnumList, StructList, ExportDir, FileName, m_MainModule.Name);

	}
	CString Temp;

	Temp.Format("%sSystemEnums.%s", (LPCTSTR)m_MainModule.Name, (LPCTSTR)m_InterfaceConfig.EnumExportConfig.ExportExt);
	ExportSystemEnums(ExportDir, Temp, m_AllModuleList);

	ExportDataStructDefines(ConstList, EnumList, StructList, ExportDir, m_MainModule.Name);
	ExportDataObject(StructList, ExportDir, m_MainModule.Name);
	ExportInterfaces(m_AllInterfaceList, ExportDir, INTERFACE_METHOD_EXPORT_TYPE_CALL, m_MainModule.Name);
	ExportInterfaces(m_AllInterfaceList, ExportDir, INTERFACE_METHOD_EXPORT_TYPE_ACK, m_MainModule.Name);

	ExportDataObjectModifyFlags(m_MainModule.DataObjectModifyFlags, ExportDir, m_MainModule.Name);
	for (STRUCT_DEFINE_LIST2* pList : StructList)
	{
		SAFE_DELETE(pList);
	}
	StructList.clear();

	if (m_InterfaceConfig.OneFileExportConfig.IsExport)
	{
		//单一文件导出保存
		CString Source = m_AllDefinesTemplate;
		CString LineSpace = GetLineSpace(Source, "<AllDefines>");
		Source.Replace("<SpaceName>", m_MainModule.Name);
		CString AllDefines;
		IncSpace(m_OneFileExportCache.GetStr(), AllDefines, LineSpace);
		Source.Replace("<AllDefines>", AllDefines);
		

		CStringFile OutputFile;
		if (m_UTF8Export)
			OutputFile.SetSaveCodePage(CP_UTF8);
		CString OutputFileName;
		OutputFileName.Format("%s\\%sAllDefines.%s", ExportDir, (LPCTSTR)m_MainModule.Name, (LPCTSTR)m_InterfaceConfig.OneFileExportConfig.ExportExt);
		if (!OutputFile.SaveToFile(Source, OutputFileName))
		{
			CString Msg;
			Msg.Format("无法打开文件%s", OutputFileName);
			MessageBox(Msg);
		}
	}

	return true;
}

bool CCallInterfaceMakerDlg::ExportDataStructDefineHeader(vector<CONST_DEFINE_LIST*>& ConstList, vector<ENUM_DEFINE_LIST*>& EnumList, vector<STRUCT_DEFINE_LIST2*>& StructList,
	LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName)
{
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	
	CString Source = m_InterfaceHeaderTemplate;

	CString Includes;
	CString Include;
	CString ImportFileName;

	Include = m_ImportDeclareTemplate;
	ImportFileName.Format("%sSystemEnums.%s", SpaceName, m_InterfaceConfig.EnumExportConfig.ExportExt);
	Include.Replace("<ImportFileName>", ImportFileName);
	Includes += Include;

	if (m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.IsExport)
	{
		Include = m_ImportDeclareTemplate;
		ImportFileName.Format("DataObjectModifyFlags.%s", m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt);
		Include.Replace("<ImportFileName>", ImportFileName);
		Includes += Include;
	}
	for (CONST_DEFINE_LIST* pList : ConstList)
	{
		if (pList->IgnoreOnExport)
			continue;
		CString ModuleName = GetModuleName(pList->ModuleID);
		Include = m_ImportDeclareTemplate;
		ImportFileName.Format("%s%sConsts.%s", ModuleName, pList->ListName, m_InterfaceConfig.ConstExportConfig.ExportExt);
		Include.Replace("<ImportFileName>", ImportFileName);
		Includes += Include;
	}
	for (ENUM_DEFINE_LIST* pList : EnumList)
	{
		if (pList->IgnoreOnExport)
			continue;
		CString ModuleName = GetModuleName(pList->ModuleID);
		Include = m_ImportDeclareTemplate;
		ImportFileName.Format("%s%sEnums.%s", ModuleName, pList->ListName, m_InterfaceConfig.EnumExportConfig.ExportExt);
		Include.Replace("<ImportFileName>", ImportFileName);
		Includes += Include;
	}

	

	for (STRUCT_DEFINE_LIST2* pList : StructList)
	{
		if (pList->IgnoreOnExport)
			continue;

		CString ModuleName = GetModuleName(pList->ModuleID);
		
		Include = m_ImportDeclareTemplate;
		ImportFileName.Format("%s%sStructs.%s", ModuleName, pList->ListName, m_InterfaceConfig.StructExportConfig.ExportExt);
		Include.Replace("<ImportFileName>", ImportFileName);
		Includes += Include;
		
		for (STRUCT_DEFINE_INFO* pInfo : pList->StructList)
		{
			if (pInfo->Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				Include = m_ImportDeclareTemplate;
				ImportFileName.Format("%s.%s", ClassNameToFileName(pInfo->Name),
					m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport ?
					m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt : m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
				Include.Replace("<ImportFileName>", ImportFileName);
				Includes += Include;
			}
		}
	}


	Source.Replace("<Prefix>", "");
	Source.Replace("<Postfix>", "");

	Source.Replace("<Includes>", Includes);

	WriteStringToFile(OutputFileName, Source);	

	return false;
}

bool CCallInterfaceMakerDlg::ExportInterfaceHeader(vector<CALLER_INTERFACE *>& InterfaceList, LPCTSTR ExportDir, LPCTSTR FileName)
{
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	
	CString ProtocolFile = m_InterfaceHeaderTemplate;

	ProtocolFile.Replace("<Prefix>", "");
	ProtocolFile.Replace("<Postfix>", "");

	CString Includes;
	CString Include;
	CString ImportFileName;

	for (size_t i = 0; i < InterfaceList.size(); i++)
	{
		CALLER_INTERFACE* pInterfaceInfo = InterfaceList[i];
		if (IsListItemChecked(pInterfaceInfo->ModuleID, pInterfaceInfo->ID))
		{
			Include = m_ImportDeclareTemplate;
			ImportFileName.Format("%sInterface.%s", pInterfaceInfo->Name,
				m_InterfaceConfig.InterfaceExportConfig.ExportExt);
			Include.Replace("<ImportFileName>", ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format("%sMsgCaller.%s", pInterfaceInfo->Name,
				m_InterfaceConfig.CallHeaderExportConfig.ExportExt.IsEmpty() ?
				m_InterfaceConfig.CallSourceExportConfig.ExportExt : m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
			Include.Replace("<ImportFileName>", ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format("%sMsgHandler.%s", pInterfaceInfo->Name,
				m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt.IsEmpty() ?
				m_InterfaceConfig.HandlerSourceExportConfig.ExportExt : m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
			Include.Replace("<ImportFileName>", ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format("%sAckInterface.%s", pInterfaceInfo->Name,
				m_InterfaceConfig.InterfaceExportConfig.ExportExt);
			Include.Replace("<ImportFileName>", ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format("%sAckMsgCaller.%s", pInterfaceInfo->Name,
				m_InterfaceConfig.CallHeaderExportConfig.ExportExt.IsEmpty() ?
				m_InterfaceConfig.CallSourceExportConfig.ExportExt : m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
			Include.Replace("<ImportFileName>", ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format("%sAckMsgHandler.%s", pInterfaceInfo->Name,
				m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt.IsEmpty() ?
				m_InterfaceConfig.HandlerSourceExportConfig.ExportExt : m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
			Include.Replace("<ImportFileName>", ImportFileName);
			Includes += Include;

			Includes += "\r\n";


		}
	}

	ProtocolFile.Replace("<Includes>", Includes);

	WriteStringToFile(OutputFileName, ProtocolFile);
	
	return false;
}

bool CCallInterfaceMakerDlg::ExportStructs(LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName, vector<STRUCT_DEFINE_INFO*>& StructList)
{
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	
	CString DefineHeader = m_StructDefineHeaderTemplate;

	DefineHeader.Replace("<SpaceName>", SpaceName);


	CString LineSpace = GetLineSpace(DefineHeader, "<Structs>");
	CString Structs = MakeStructDefines(StructList, SpaceName, LineSpace);
	DefineHeader.Replace("<Structs>", Structs);

	WriteStringToFile(OutputFileName, DefineHeader);
		
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
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	CString EnumDefineHeader;

	if (IsSystem)
		EnumDefineHeader = m_SystemEnumDefineHeaderTemplate;
	else
		EnumDefineHeader = m_EnumDefineHeaderTemplate;

	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString ListNameUp = ClassNameToUpper(ListName);

	EnumDefineHeader.Replace("<SpaceName>", SpaceName);

	EnumDefineHeader.Replace("<!SpaceName>", SpaceNameUp);
	EnumDefineHeader.Replace("<!ListName>", ListNameUp);


	CString LineSpace = GetLineSpace(EnumDefineHeader, "<Enums>");
	CString Enums = MakeEnumDefines(EnumList, SpaceName, LineSpace);
	EnumDefineHeader.Replace("<Enums>", Enums);

	CString EnumStrValuesName;
	EnumStrValuesName.Format("%s_ENUM_STR_VALUES", ListNameUp);
	EnumDefineHeader.Replace("<EnumStrValuesName>", EnumStrValuesName);

	//LineSpace = GetLineSpace(EnumDefineHeader, "<EnumStrValues>");
	//CString EnumStrValues = MakeEnumStrValues(EnumList, SpaceName, LineSpace);
	//EnumDefineHeader.Replace("<EnumStrValues>", EnumStrValues);


	WriteStringToFile(OutputFileName, EnumDefineHeader);
	
	return false;
}

bool CCallInterfaceMakerDlg::ExportConsts(LPCTSTR ExportDir, LPCTSTR FileName, vector<CONST_DEFINE_INFO>& ConstList, LPCTSTR SpaceName, LPCTSTR ListName)
{
	CString OutputFileName;

	OutputFileName=ExportDir;
	OutputFileName+="\\";
	OutputFileName+=FileName;

	CString ConstDefineHeader = m_ConstDefineHeaderTemplate;

	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString ListNameUp = ClassNameToUpper(ListName);

	ConstDefineHeader.Replace("<SpaceName>", SpaceName);

	ConstDefineHeader.Replace("<!SpaceName>", SpaceNameUp);
	ConstDefineHeader.Replace("<!ListName>", ListNameUp);

	CString LineSpace = GetLineSpace(ConstDefineHeader, "<Consts>");
	CString Consts = MakeConstDefines(ConstList, SpaceName, LineSpace);
	ConstDefineHeader.Replace("<Consts>", Consts);

	WriteStringToFile(OutputFileName, ConstDefineHeader);
	
	return false;
}

bool CCallInterfaceMakerDlg::ExportDataStructDefines(vector<CONST_DEFINE_LIST*>& ConstList, vector<ENUM_DEFINE_LIST*>& EnumList, vector<STRUCT_DEFINE_LIST2*>& StructList,
	LPCTSTR ExportDir, LPCTSTR SpaceName)
{
	CString Temp;
	for (CONST_DEFINE_LIST* pList : ConstList)
	{
		if (pList->IgnoreOnExport)
			continue;
		CString ModuleName = GetModuleName(pList->ModuleID);
		Temp.Format("%s%sConsts.%s", ModuleName, pList->ListName, m_InterfaceConfig.ConstExportConfig.ExportExt);
		ExportConsts(ExportDir, Temp, pList->ConstList, SpaceName, pList->ListName);
	}
	for (ENUM_DEFINE_LIST* pList : EnumList)
	{
		if (pList->IgnoreOnExport)
			continue;
		CString ModuleName = GetModuleName(pList->ModuleID);
		Temp.Format("%s%sEnums.%s", ModuleName, pList->ListName, m_InterfaceConfig.EnumExportConfig.ExportExt);
		ExportEnums(ExportDir, Temp, pList->EnumList, SpaceName, pList->ListName, false);
	}
	for (STRUCT_DEFINE_LIST2* pList : StructList)
	{
		if (pList->IgnoreOnExport)
			continue;
		CString ModuleName = GetModuleName(pList->ModuleID);
		Temp.Format("%s%sStructs.%s", ModuleName, pList->ListName, m_InterfaceConfig.StructExportConfig.ExportExt);
		ExportStructs(ExportDir, Temp, SpaceName, pList->StructList);
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
				CString OutputFileName;
				CString LineSpace;

				CString InterfaceName;
				if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
					InterfaceName = InterfaceInfo.Name + "Ack";
				else
					InterfaceName = InterfaceInfo.Name;

				OutputFileName.Format("%s\\%sInterface.%s", ExportDir, InterfaceName,
					m_InterfaceConfig.InterfaceExportConfig.ExportExt);

				CString Interface = m_InterfaceTemplate;

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
					CString MethodIDEnums = MakeEnumDefine(EnumInfo, SpaceName, LineSpace);
					Interface.Replace("<MethodIDEnums>", MethodIDEnums);
				}
				else
				{
					Interface.Replace("<MethodIDEnums>", "");
				}

				Interface.Replace("<SpaceName>", SpaceName);
				Interface.Replace("<InterfaceName>", InterfaceName);
				Interface.Replace("<InterfaceDescription>", InterfaceInfo.Description);
				Interface.Replace("<@InterfaceName>", InterfaceInfo.Name);
				LineSpace = GetLineSpace(Interface, "<Methods>");
				CString Methods = MakeMethods(InterfaceInfo, SpaceName, ExportType, LineSpace, true);
				Interface.Replace("<Methods>", Methods);
				LineSpace = GetLineSpace(Interface, "<MethodSSTIDEnumDefine>");
				CString SSTIDDefines = MakeMethodSSTIDEnumDefines(InterfaceInfo, ExportType, SpaceName, LineSpace);
				Interface.Replace("<MethodSSTIDEnumDefine>", SSTIDDefines);

				WriteStringToFile(OutputFileName, Interface);
					
			}
			ExportMsgCaller(ExportDir, SpaceName, InterfaceInfo, ExportType);
			ExportMsgHandler(ExportDir, SpaceName, InterfaceInfo, ExportType);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportMsgCaller(LPCTSTR ExportDir, LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType)
{

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
		
		
		CString Header=m_MsgCallerHeaderTemplate;

		Header.Replace("<SpaceName>", SpaceName);
		Header.Replace("<InterfaceName>", InterfaceName);
		Header.Replace("<@InterfaceName>", InterfaceInfo.Name);
		CString LineSpace=GetLineSpace(Header,"<Methods>");
		CString Methods = MakeMethods(InterfaceInfo, SpaceName, ExportType, LineSpace, false);
		Header.Replace("<Methods>",Methods);

		LineSpace=GetLineSpace(Header,"<PackMethods>");
		Methods = MakeMsgPackMethods(InterfaceInfo.MethodList, ExportType, LineSpace);
		Header.Replace("<PackMethods>",Methods);

		WriteStringToFile(HeaderFileName, Header);
	}

	if(m_InterfaceConfig.CallSourceExportConfig.IsExport)
	{
		SourceFileName.Format("%s\\%sMsgCaller.%s", ExportDir, InterfaceName,
			m_InterfaceConfig.CallSourceExportConfig.ExportExt);
		
		CString Source = m_MsgCallerSourceTemplate;

		Source.Replace("<SpaceName>", SpaceName);
		Source.Replace("<InterfaceName>", InterfaceName);
		Source.Replace("<@InterfaceName>", InterfaceInfo.Name);

		CString LineSpace = GetLineSpace(Source, "<Methods>");
		CString Methods = MakeMethodsSource(InterfaceInfo, SpaceName, ExportType, LineSpace);
		Source.Replace("<Methods>", Methods);

		LineSpace = GetLineSpace(Source, "<PackMethods>");
		Methods = MakeMsgPackMethodsSource(SpaceName, InterfaceInfo, ExportType, LineSpace);
		Source.Replace("<PackMethods>", Methods);

		WriteStringToFile(SourceFileName, Source);
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportMsgHandler(LPCTSTR ExportDir, LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType)
{
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

		CString Header = m_MsgHandlerHeaderTemplate;

		Header.Replace("<SpaceName>", SpaceName);
		Header.Replace("<InterfaceName>", InterfaceName);
		Header.Replace("<@InterfaceName>", InterfaceInfo.Name);
		CString LineSpace = GetLineSpace(Header, "<Methods>");
		CString Methods = MakeMsgHandlerMethods(InterfaceInfo, ExportType, LineSpace);
		Header.Replace("<Methods>", Methods);

		WriteStringToFile(HeaderFileName, Header);
	}

	if(m_InterfaceConfig.HandlerSourceExportConfig.IsExport)
	{
		SourceFileName.Format("%s\\%sMsgHandler.%s", ExportDir, InterfaceName,
			m_InterfaceConfig.HandlerSourceExportConfig.ExportExt);
		
		CString Source = m_MsgHandlerSourceTemplate;

		Source.Replace("<SpaceName>", SpaceName);
		Source.Replace("<InterfaceName>", InterfaceName);
		Source.Replace("<@InterfaceName>", InterfaceInfo.Name);

		CString MethodCount;
		MethodCount.Format("%d", InterfaceInfo.MethodList.size());
		Source.Replace("<MethodCount>", MethodCount);

		CString LineSpace = GetLineSpace(Source, "<MsgMapInits>");
		CString MsgMapInits = MakeMsgMapInits(InterfaceInfo, SpaceName, ExportType, LineSpace);
		Source.Replace("<MsgMapInits>", MsgMapInits);

		LineSpace = GetLineSpace(Source, "<Methods>");
		CString Methods = MakeMsgHandlerMethodsSource(InterfaceInfo, ExportType, SpaceName, LineSpace);
		Source.Replace("<Methods>", Methods);

		WriteStringToFile(SourceFileName, Source);
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportDataObject(vector<STRUCT_DEFINE_LIST2*>& StructList, LPCTSTR ExportDir, LPCTSTR SpaceName)
{
	CString HeaderFileName,SourceFileName;
	
	for (STRUCT_DEFINE_LIST2* pList : StructList)
	{
		if (!pList->IgnoreOnExport)
		{
			for (STRUCT_DEFINE_INFO* pInfo : pList->StructList)
			{
				if (pInfo->Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				{
					CString SSTIDEnumName;
					SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(pInfo->Name));

					if (m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport)
					{
						HeaderFileName.Format("%s\\%s.%s", ExportDir, ClassNameToFileName(pInfo->Name),
							m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);

						CString Header = RestoreFileToTemplate(HeaderFileName, m_DataObjectDefineHeaderTemple);



						Header.Replace("<SpaceName>", SpaceName);

						CString LineSpace = GetLineSpace(m_MsgHandlerHeaderTemplate, "<Description>");
						CString Description = ToComment(pInfo->Description, LineSpace);
						Header.Replace("<Description>", Description);


						Header.Replace("<ClassName>", pInfo->Name);
						if (pInfo->BaseStruct.IsEmpty())
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
							Header.Replace("<BaseClass>", pInfo->BaseStruct);
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_JSON_PROCESS)
						{
							LineSpace = GetLineSpace(Header, "<JsonProcess>");
							CString Process = m_DataObjectJsonProcessHeaderTemplate;
							Process.Replace("\r\n", "\r\n" + LineSpace);
							Header.Replace("<JsonProcess>", Process);
						}
						else
						{
							Header.Replace("<JsonProcess>", "");
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_DB_PROCESS)
						{
							LineSpace = GetLineSpace(Header, "<DBProcess>");
							CString Process = m_DataObjectDBProcessHeaderTemplate;
							Process.Replace("\r\n", "\r\n" + LineSpace);
							Header.Replace("<DBProcess>", Process);
						}
						else
						{
							Header.Replace("<DBProcess>", "");
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_LUA_PROCESS)
						{
							LineSpace = GetLineSpace(Header, "<LuaProcess>");
							CString Process = m_DataObjectLuaProcessHeaderTemplate;
							Process.Replace("\r\n", "\r\n" + LineSpace);
							Header.Replace("<LuaProcess>", Process);

							LineSpace = GetLineSpace(Header, "<LuaProcessSource>");
							Process = MakeDataObjectLuaProcess(*pInfo, SSTIDEnumName, SpaceName, LineSpace);
							Header.Replace("<LuaProcessSource>", Process);
						}
						else
						{
							Header.Replace("<LuaProcess>", "");
							Header.Replace("<LuaProcessSource>", "");
						}

						LineSpace = GetLineSpace(Header, "<Members>");
						CString Members = MakeStructMembers(*pInfo, ";", SpaceName, LineSpace);
						Header.Replace("<Members>", Members);

						LineSpace = GetLineSpace(Header, "<Members,>");
						Members = MakeStructMembers(*pInfo, ",", SpaceName, LineSpace);
						Header.Replace("<Members,>", Members);

						LineSpace = GetLineSpace(Header, "<SSTIDEnumDefine>");
						CString SSTIDEnum = MakeStructSSTIDEnumDefines(*pInfo, SpaceName, LineSpace);
						Header.Replace("<SSTIDEnumDefine>", SSTIDEnum);

						LineSpace = GetLineSpace(Header, "<ModifyFlagEnumDefine>");
						CString ModifyFlagEnumDefine = MakeDataObjectModifyFlagEnumDefine(*pInfo, SpaceName, LineSpace);
						Header.Replace("<ModifyFlagEnumDefine>", ModifyFlagEnumDefine);

						LineSpace = GetLineSpace(Header, "<SetMethodsDeclare>");
						CString SetMethodsDeclare = MakeDataObjectSetMethodsDeclare(*pInfo, LineSpace);
						Header.Replace("<SetMethodsDeclare>", SetMethodsDeclare);

						LineSpace = GetLineSpace(Header, "<GetMethodsDeclare>");
						CString GetMethodsDeclare = MakeDataObjectGetMethodsDeclare(*pInfo, LineSpace);
						Header.Replace("<GetMethodsDeclare>", GetMethodsDeclare);


						LineSpace = GetLineSpace(Header, "<SetMethodsDefine>");
						CString SetMethodsDefine = MakeDataObjectSetMethodsDefine(*pInfo, SpaceName, LineSpace);
						Header.Replace("<SetMethodsDefine>", SetMethodsDefine);

						LineSpace = GetLineSpace(Header, "<GetMethodsDefine>");
						CString GetMethodsDefine = MakeDataObjectGetMethodsDefine(*pInfo, SpaceName, LineSpace);
						Header.Replace("<GetMethodsDefine>", GetMethodsDefine);

						Header.Replace("<CLASS_FLAG_INDEX>", "DATA_OBJECT_FLAG_" + ClassNameToUpper(pInfo->Name));

						WriteStringToFile(HeaderFileName, Header);
					}
					{
						SourceFileName.Format("%s\\%s.%s", ExportDir, ClassNameToFileName(pInfo->Name),
							m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
						CString Source = RestoreFileToTemplate(SourceFileName, m_DataObjectDefineSourceTemple);

						Source.Replace("<SpaceName>", SpaceName);

						CString LineSpace;

						

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_XML_PROCESS)
						{
							LineSpace = GetLineSpace(Source, "<XMLProcess>");
							CString XMLProcess = MakeXMLProcess(*pInfo, SSTIDEnumName, SpaceName, LineSpace);
							Source.Replace("<XMLProcess>", XMLProcess);
						}
						else
						{
							Source.Replace("<XMLProcess>", "");
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_JSON_PROCESS)
						{
							LineSpace = GetLineSpace(Source, "<JsonProcess>");
							CString JsonProcess = MakeDataObjectJsonProcess(*pInfo, SSTIDEnumName, SpaceName, LineSpace);
							Source.Replace("<JsonProcess>", JsonProcess);
						}
						else
						{
							Source.Replace("<JsonProcess>", "");
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_DB_PROCESS)
						{
							LineSpace = GetLineSpace(Source, "<DBProcess>");
							CString Process = MakeDataObjectDBProcess(*pInfo, SSTIDEnumName, SpaceName, LineSpace);
							Source.Replace("<DBProcess>", Process);
						}
						else
						{
							Source.Replace("<DBProcess>", "");
						}

						

						LineSpace = GetLineSpace(m_MsgHandlerHeaderTemplate, "<Description>");
						CString Description = ToComment(pInfo->Description, LineSpace);
						Source.Replace("<Description>", Description);

						Source.Replace("<SpaceName>", SpaceName);

						if (pInfo->MemberList.size())
						{
							Source.Replace("<IfHaveMember>", "");
							Source.Replace("</IfHaveMember>", "");
						}
						else
						{
							RemoveBlock(Source, "<IfHaveMember>", "</IfHaveMember>");
						}

						if (pInfo->BaseStruct.IsEmpty())
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
							Source.Replace("<BaseClass>", pInfo->BaseStruct);
							Source.Replace("<BaseClass>", pInfo->BaseStruct);
						}

						Source.Replace("<ClassName>", pInfo->Name);

						CString FullModifyFlag;
						FullModifyFlag.Format("DOMF_%s_FULL", ClassNameToUpper(pInfo->Name));
						Source.Replace("<FULL_MODIFY_FLAG>", FullModifyFlag);


						LineSpace = GetLineSpace(Source, "<Members>");
						CString Members = MakeStructMembers(*pInfo, ";", SpaceName, LineSpace);
						Source.Replace("<Members>", Members);

						LineSpace = GetLineSpace(Source, "<Members,>");
						Members = MakeStructMembers(*pInfo, ",", SpaceName, LineSpace);
						Source.Replace("<Members,>", Members);

						LineSpace = GetLineSpace(Source, "<SSTIDEnumDefine>");
						CString SSTIDEnum = MakeStructSSTIDEnumDefines(*pInfo, SpaceName, LineSpace);
						Source.Replace("<SSTIDEnumDefine>", SSTIDEnum);

						LineSpace = GetLineSpace(Source, "<ModifyFlagEnumDefine>");
						CString ModifyFlagEnumDefine = MakeDataObjectModifyFlagEnumDefine(*pInfo, SpaceName, LineSpace);
						Source.Replace("<ModifyFlagEnumDefine>", ModifyFlagEnumDefine);

						LineSpace = GetLineSpace(Source, "<DataObjectMembersGetModifyFlag>");
						CString DataObjectMembersGetModifyFlag = MakeDataObjectMembersGetModifyFlag(*pInfo, LineSpace);
						Source.Replace("<DataObjectMembersGetModifyFlag>", DataObjectMembersGetModifyFlag);

						LineSpace = GetLineSpace(Source, "<DataObjectMembersIsModified>");
						CString DataObjectMembersIsModified = MakeDataDataObjectMembersIsModified(*pInfo, LineSpace);
						Source.Replace("<DataObjectMembersIsModified>", DataObjectMembersIsModified);

						LineSpace = GetLineSpace(Source, "<DataObjectMembersClearModifyFlag>");
						CString DataObjectMembersClearModifyFlag = MakeDataDataObjectMembersClearModifyFlag(*pInfo, LineSpace);
						Source.Replace("<DataObjectMembersClearModifyFlag>", DataObjectMembersClearModifyFlag);

						LineSpace = GetLineSpace(Source, "<SetMethodsDefine>");
						CString SetMethodsDefine = MakeDataObjectSetMethodsDefine(*pInfo, SpaceName, LineSpace);
						Source.Replace("<SetMethodsDefine>", SetMethodsDefine);

						LineSpace = GetLineSpace(Source, "<GetMethodsDefine>");
						CString GetMethodsDefine = MakeDataObjectGetMethodsDefine(*pInfo, SpaceName, LineSpace);
						Source.Replace("<GetMethodsDefine>", GetMethodsDefine);

						Source.Replace("<CLASS_FLAG_INDEX>", "DATA_OBJECT_FLAG_" + ClassNameToUpper(pInfo->Name));


						LineSpace = GetLineSpace(Source, "<InitOperations>");
						CString InitOperations = MakeInitOperations(*pInfo, LineSpace);
						Source.Replace("<InitOperations>", InitOperations);


						CString PacketName = m_InterfaceConfig.DefaultPacketName;
						CString PacketMemberName = m_InterfaceConfig.DefaultPacketMemberName;


						LineSpace = GetLineSpace(Source, "<PackOperations>");
						CString PackOperations = MakePackOperations(*pInfo, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
						Source.Replace("<PackOperations>", PackOperations);

						LineSpace = GetLineSpace(Source, "<UnpackOperations>");
						CString UnpackOperations = MakeUnpackOperations(*pInfo, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
						Source.Replace("<UnpackOperations>", UnpackOperations);

						LineSpace = GetLineSpace(Source, "<PacketSizes>");
						CString PackSizes = MakePackSizes(*pInfo, SpaceName, LineSpace);
						Source.Replace("<PacketSizes>", PackSizes);

						//LineSpace = GetLineSpace(Source, "<ToXMLOperations>");
						//CString ToXMLOperations = MakeToXMLOperations(StructInfo, SSTIDEnumName, LineSpace);
						//Source.Replace("<ToXMLOperations>", ToXMLOperations);

						//LineSpace = GetLineSpace(Source, "<FromXMLOperations>");
						//CString FromXMLOperations = MakeFromXMLOperations(StructInfo, SSTIDEnumName, LineSpace);
						//Source.Replace("<FromXMLOperations>", FromXMLOperations);

						LineSpace = GetLineSpace(Source, "<CloneOperations>");
						CString CloneOperations = MakeCloneOperations(*pInfo, m_InterfaceConfig.MemberVariablePrefix, "DataObject.", SpaceName, LineSpace);
						Source.Replace("<CloneOperations>", CloneOperations);

						WriteStringToFile(SourceFileName, Source);
					}
				}
			}
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportDataObjectModifyFlags(vector<DATA_OBJECT_MODIFY_FLAG> DataObjectModifyFlagList, LPCTSTR ExportDir, LPCTSTR SpaceName)
{

	if (m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.IsExport)
	{
		CString HeaderFileName;
		CString LineSpace;

		HeaderFileName.Format("%s\\DataObjectModifyFlags.%s", ExportDir,
			m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt);

		CString Header = m_DataObjectModifyFlagsHeaderTemple;

		
		Header.Replace("<SpaceName>", SpaceName);

		LineSpace = GetLineSpace(Header, "<DataObjectModifyFlags>");
		CString DataObjectModifyFlags = MakeDataObjectModifyFlagsHeader(m_AllDataStructList, DataObjectModifyFlagList, SpaceName, LineSpace);
		Header.Replace("<DataObjectModifyFlags>", DataObjectModifyFlags);

		WriteStringToFile(HeaderFileName, Header);
	}


	if(m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.IsExport)
	{
		CString SourceFileName;
		CString LineSpace;

		SourceFileName.Format("%s\\DataObjectModifyFlags.%s",ExportDir,
			m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.ExportExt);

		CString Source=m_DataObjectModifyFlagsSourceTemple;

		Source.Replace("<SpaceName>", SpaceName);

		LineSpace = GetLineSpace(Source, "<DataObjectModifyFlags>");
		CString DataObjectModifyFlags = MakeDataObjectModifyFlagsSource(m_AllDataStructList, DataObjectModifyFlagList, SpaceName, LineSpace);
		Source.Replace("<DataObjectModifyFlags>", DataObjectModifyFlags);

		WriteStringToFile(SourceFileName, Source);
	}
	return true;
}

CString CCallInterfaceMakerDlg::MakeMethods(CALLER_INTERFACE& InterfaceInfo, LPCTSTR SpaceName, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace, bool IsPure)
{
	CString Methods;
	for (INTERFACE_METHOD& MethodInfo : InterfaceInfo.MethodList)
	{
		CString MethodName = MethodInfo.Name;

		vector<METHOD_PARAM>* pParamList = NULL;
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

			CString InterfaceName;
			if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
				InterfaceName = InterfaceInfo.Name + "Ack";
			else
				InterfaceName = InterfaceInfo.Name;

			Method.Replace("<SpaceName>", SpaceName);
			Method.Replace("<InterfaceName>", InterfaceName);
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
	CString Space = "\r\n";
	Space += szLineSpace;
	Methods.Replace("\r\n", Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMethodSSTIDEnumDefines(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName, LPCTSTR szLineSpace)
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

			MethodIDEnumDefines += MakeEnumDefine(EnumInfo, SpaceName, szLineSpace);
		}
		
	}
	
	return MethodIDEnumDefines;
}

CString CCallInterfaceMakerDlg::MakeStructSSTIDEnumDefines(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
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

	return MakeEnumDefine(EnumInfo, SpaceName, szLineSpace);


	
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
			CString Param;
			
			if(IsDefine)
				Param = m_ParamDefineTemple;
			else
				Param = m_ParamTransTemple;

			
			if (ParamInfo.IsReference)
			{
				if (IsDefine)
				{
					CString ReferenceDefine = pTypeInfo->GenerateOperations.ReferenceDefine;
					ReferenceDefine.Replace("<Type>", pTypeInfo->CType);
					Param.Replace("<ParamType>", ReferenceDefine);

					Param.Replace("<ParamName>", ParamInfo.Name);
				}
				else
				{
					CString ReferenceUse = pTypeInfo->GenerateOperations.ReferenceUse;
					ReferenceUse.Replace("<Variable>", ParamInfo.Name);
					Param.Replace("<ParamName>", ReferenceUse);
				}
			}
			else
			{
				Param.Replace("<ParamName>", ParamInfo.Name);
				Param.Replace("<ParamType>", pTypeInfo->CType);
			}


			if (ParamInfo.DefaultValue.IsEmpty() || (!IncludeDefaultValue))
			{
				Param.Replace("<ParamDefaultValue>", "");
			}
			else
			{
				CString DefauleValue;
				DefauleValue.Format("= %s", ParamInfo.DefaultValue);
				Param.Replace("<ParamDefaultValue>", DefauleValue);
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
			ParamDescription.Replace("<ParamType>", pTypeInfo->CType);
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

CString CCallInterfaceMakerDlg::MakeParamDefines(vector<METHOD_PARAM>& ParamList, LPCTSTR SpaceName, LPCTSTR szSplitChar, LPCTSTR szLineSpace)
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
			Param.Replace("<SpaceName>", SpaceName);
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

			Param = m_InterfaceConfig.LocalVariableDefinePrefix + Param;
			
			ParamDefines += Param + szSplitChar;
			ParamDefines+="\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	ParamDefines.Replace("\r\n",Space);
	return ParamDefines;
}

CString CCallInterfaceMakerDlg::MakeMethodsSource(CALLER_INTERFACE& InterfaceInfo, LPCTSTR SpaceName, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
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
			Method.Replace("<SpaceName>", SpaceName);
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

			if (MethodInfo.Flag&INTERFACE_METHOD_FLAG_NO_COMPRESS)
			{
				Method.Replace("<IfMsgNoCompress>", "");
				Method.Replace("</IfMsgNoCompress>", "");
			}
			else
			{
				RemoveBlock(Method, "<IfMsgNoCompress>", "</IfMsgNoCompress>");
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
			Method.Replace("<SpaceName>", SpaceName);
			Method.Replace("<!SpaceName>", SpaceNameUp);
			Method.Replace("<!InterfaceName>", InterfaceNameUp);
			Method.Replace("<!MethodName>", MethodNameUp);
			Method.Replace("<@InterfaceName>", InterfaceInfo.Name);
			CString Params = MakeParams(*pParamList, true, true);
			if (!Params.IsEmpty())
				Params = "," + Params;
			Method.Replace("<,Params>", Params);
			CString LineSpace = GetLineSpace(Method, "<PackOperations>");
			CString PacketName = m_InterfaceConfig.DefaultPacketName;
			CString PacketMemberName = m_InterfaceConfig.DefaultPacketMemberName;
			CString SSTIDEnumName;
			SSTIDEnumName.Format("%s_%s_MEMBER_IDS", ClassNameToUpper(InterfaceName), ClassNameToUpper(MethodName));
			CString PackOperations = MakePackOperations(MethodInfo, ExportType, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
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
					Operation.Replace("<Type>", pTypeInfo->CType);
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

CString CCallInterfaceMakerDlg::MakePackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace)
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
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");


				Operation.Replace("<Variable>", (*pParamList)[i].Name);		


				CString LineSpace = GetLineSpace(Operation, "<PackOperation>");
				UINT OperationFlag = PACK_OPERATION_FLAG_IN_INTERFACE;
				CString PackOP = MakePackOperation((*pParamList)[i].Type,
					(*pParamList)[i].Name,
					MethodName, ClassNameToUpper(MethodName),
					SSTIDEnumName, (*pParamList)[i].PackFlag,
					PacketName, PacketMemberName,
					OperationFlag,
					SpaceName, LineSpace);

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

CString CCallInterfaceMakerDlg::MakeUnpackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace)
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
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				RemoveBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
							

				CString SST_ID;

				SST_ID.Format("SST_%s_%s", ClassNameToUpper(MethodName), ClassNameToUpper((*pParamList)[i].Name));
				SST_ID.MakeUpper();


				Operation.Replace("<SpaceName>", SpaceName);
				Operation.Replace("<SST_NAME>", SSTIDEnumName);
				Operation.Replace("<SST_ID>", SST_ID);

				Operation.Replace("<Variable>", (*pParamList)[i].Name);


				CString LineSpace = GetLineSpace(Operation, "<PackOperation>");
				UINT OperationFlag = PACK_OPERATION_FLAG_IN_INTERFACE;
				CString UnpackOP = MakeUnpackOperation((*pParamList)[i].Type,
					(*pParamList)[i].Name,
					MethodName, ClassNameToUpper(MethodName),
					SSTIDEnumName, (*pParamList)[i].PackFlag,
					PacketName, PacketMemberName,
					OperationFlag,
					SpaceName, LineSpace);

				Operation.Replace("<UnpackOperation>", UnpackOP);

				if (i < pParamList->size() - 1)
				{
					Operation.Replace("<IfNotLastItem>", "");
					Operation.Replace("</IfNotLastItem>", "");
				}
				else
				{
					RemoveBlock(Operation, "<IfNotLastItem>", "</IfNotLastItem>");
				}

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

				if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
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

				RemoveBlock(Operation, "<IfInArray>", "</IfInArray>");
				Operation.Replace("<IfNotInArray>", "");
				Operation.Replace("</IfNotInArray>", "");

				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");

				Operation.Replace("<Variable>", (*pParamList)[i].Name);
				Operation.Replace("<Type>", pTypeInfo->CType);									
				Operation.Replace("<PackFlag>", (*pParamList)[i].PackFlag);

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
					Operation = m_InterfaceConfig.ArrayDefineConfig.InitOperation;
					CString Temp;
					Temp.Format("%u", StructInfo.MemberList[i].ArrayStartLength);
					Operation.Replace("<ArrayStartLength>", Temp);
					Temp.Format("%u", StructInfo.MemberList[i].ArrayGrowLength);
					Operation.Replace("<ArrayGrowLength>", Temp);

					Operation = ProcessArrayOperation(Operation, pTypeInfo);
				}
				CString VarName = StructInfo.MemberList[i].Name;
				if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
					VarName = "m_" + VarName;
				VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
				Operation.Replace("<Variable>", VarName);
				Operation.Replace("<Type>", pTypeInfo->CType);

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
CString CCallInterfaceMakerDlg::MakePackOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace)
{
	CString PackOperations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation = m_PackOperationUnitTemplate;
		CString LocalPacketName = PacketName;
		CString LocalPacketMemberName = PacketMemberName;
		if (StructInfo.MemberList[i].IsArray)
		{
			Operation = m_InterfaceConfig.ArrayDefineConfig.PackOperation;
			Operation.Replace("<Packet>", PacketName);
			Operation.Replace("<PacketMember>", PacketMemberName);
			CheckNameChange(Operation, LocalPacketName, "<ChangePacketName=");
			CheckNameChange(Operation, LocalPacketMemberName, "<ChangePacketMemberName=");
			TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
			Operation = ProcessArrayOperation(Operation, pTypeInfo);
		}

		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			RetainBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				RetainBlock(Operation, "<IfNotCheckMF>", "</IfNotCheckMF>");
			}
			else
			{
				RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				RemoveBlock(Operation, "<IfNotCheckMF>", "</IfNotCheckMF>");
			}
		}
		else
		{			
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			RetainBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			RetainBlock(Operation, "<IfNotCheckMF>", "</IfNotCheckMF>");
		}

		CString VarName = StructInfo.MemberList[i].Name;
		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			VarName = "m_" + VarName;
		VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;		
		Operation.Replace("<Variable>", VarName);

		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			if (StructInfo.MemberList[i].IsArray)
				Operation.Replace("<OrginType>", pTypeInfo->Name + "[]");
			else
				Operation.Replace("<OrginType>", pTypeInfo->Name);
		}


		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(StructInfo.MemberList[i].Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);

		CString LineSpace = GetLineSpace(Operation, "<PackOperation>");
		UINT OperationFlag = (((StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0) ? PACK_OPERATION_FLAG_IN_DATA_OBJECT : 0)
			| (((StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0)) ? PACK_OPERATION_FLAG_NEED_MF_CHECK : 0)
			| ((StructInfo.MemberList[i].IsArray) ? PACK_OPERATION_FLAG_IS_ARRAY : 0);
		CString PackOP = MakePackOperation(StructInfo.MemberList[i].Type,
			StructInfo.MemberList[i].Name,
			StructInfo.Name, StructInfo.ShortName,
			SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
			LocalPacketName, LocalPacketMemberName,
			OperationFlag,
			SpaceName, LineSpace);

		Operation.Replace("<PackOperation>", PackOP);

		CString SST_ID;

		SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
		SST_ID.MakeUpper();


		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);


		PackOperations += Operation;
		PackOperations += "\r\n";
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	PackOperations.Replace("\r\n", Space);
	return PackOperations;
}
CString CCallInterfaceMakerDlg::MakeUnpackOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace)
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
			CString LocalPacketName = PacketName;
			CString LocalPacketMemberName = PacketMemberName;
			if (StructInfo.MemberList[i].IsArray)
			{
				Operation = m_InterfaceConfig.ArrayDefineConfig.UnpackOperation;

				CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
				VariableDefine.Replace("<Type>", pTypeInfo->CType);				
				VariableDefine.Replace("<Space>", "	");
				VariableDefine.Replace("<Variable>", "ArrayElement");
				VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
				Operation.Replace("<ArrayElementVariableDefine>", VariableDefine);

				CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
				VariableInit.Replace("<Variable>", "ArrayElement");
				Operation.Replace("<ArrayElementVariableInit>", VariableInit);

				Operation.Replace("<Packet>", PacketName);
				Operation.Replace("<PacketMember>", PacketMemberName);
				CheckNameChange(Operation, LocalPacketName, "<ChangePacketName=");
				CheckNameChange(Operation, LocalPacketMemberName, "<ChangePacketMemberName=");
				Operation = ProcessArrayOperation(Operation, pTypeInfo);
			}

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation,"<IfNotInDataObject>","</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>","");
				Operation.Replace("</IfInDataObject>","");
				if(StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation,"<IfCheckMF>","</IfCheckMF>");
				}
				else
				{
					RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
				RetainBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
			}
			else
			{
				RemoveBlock(Operation,"<IfInDataObject>","</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>","");
				Operation.Replace("</IfNotInDataObject>","");
				RemoveBlock(Operation,"<IfCheckMF>","</IfCheckMF>");
				RemoveBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
			}
			
			CString SST_ID;
			
			SST_ID.Format("SST_%s_%s",StructInfo.ShortName,ClassNameToUpper(StructInfo.MemberList[i].Name));
			SST_ID.MakeUpper();
			

			Operation.Replace("<SST_NAME>",SSTIDEnumName);
			Operation.Replace("<SST_ID>",SST_ID);

			CString VarName = StructInfo.MemberList[i].Name;

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				VarName = "m_" + VarName;
			
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
			Operation.Replace("<Variable>", VarName);

			

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS",ClassNameToUpper(StructInfo.Name));			
			Operation.Replace("<ModifyFlagEnumName>",ModifyFlagEnumName);

			CString ModifyFlag="MF_"+ClassNameToUpper(StructInfo.MemberList[i].Name);
			Operation.Replace("<ModifyFlag>",ModifyFlag);
			Operation.Replace("<SpaceName>", SpaceName);

			CString LineSpace=GetLineSpace(Operation,"<UnpackOperation>");
			UINT OperationFlag = (((StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0) ? PACK_OPERATION_FLAG_IN_DATA_OBJECT : 0)
				| (((StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0)) ? PACK_OPERATION_FLAG_NEED_MF_CHECK : 0)
				| ((StructInfo.MemberList[i].IsArray) ? PACK_OPERATION_FLAG_IS_ARRAY : 0);
			CString UnpackOP = MakeUnpackOperation(StructInfo.MemberList[i].Type,
				StructInfo.MemberList[i].Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
				LocalPacketName, LocalPacketMemberName,
				OperationFlag,
				SpaceName, LineSpace);

			Operation.Replace("<UnpackOperation>",UnpackOP);

			//if (i < StructInfo.MemberList.size() - 1)
			//{
			//	RemoveBlock(Operation, "<IfLastItem>", "</IfLastItem>");
			//}
			//else
			//{
			//	RetainBlock(Operation, "<IfLastItem>", "</IfLastItem>");
			//}

			PackOperations+=Operation;
			PackOperations+="\r\n";
		}		
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	PackOperations.Replace("\r\n",Space);
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakePackSizes(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
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

			Variable = m_InterfaceConfig.MemberVariablePrefix + Variable;

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

			Operation.Replace("<PackFlag>", StructInfo.MemberList[i].PackFlag);

			if(StructInfo.MemberList[i].IsArray)
			{
				CString Var = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
				Var.Replace("<Variable>", Variable);
				Var.Replace("<Index>", "i");
				Operation.Replace("<Variable>", Var);
				Operation.Replace("<Type>", pTypeInfo->CType);

				CString ArrayOP = m_InterfaceConfig.ArrayDefineConfig.SizeCaculateOperation;
				
				ArrayOP.Replace("<Variable>", Variable);
				ArrayOP.Replace("<ArrayElementSize>", Operation);
				RemoveBlock(ArrayOP, "<IfNotInArray>", "</IfNotInArray>");
				ArrayOP.Replace("<IfInArray>", "");
				ArrayOP.Replace("</IfInArray>", "");

				PackSize = ProcessArrayOperation(ArrayOP, pTypeInfo);
			}
			else
			{
				Operation.Replace("<Variable>",Variable);
				Operation.Replace("<Type>",pTypeInfo->CType);
				RemoveBlock(Operation, "<IfInArray>", "</IfInArray>");
				Operation.Replace("<IfNotInArray>", "");
				Operation.Replace("</IfNotInArray>", "");
				PackSize.Replace("<PackSize>",Operation);
			}

			

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(PackSize, "<IfCheckMF>", "</IfCheckMF>");
				}
				else
				{
					RetainBlock(PackSize, "<IfCheckMF>", "</IfCheckMF>");

					CString ModifyFlagEnumName;
					ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
					PackSize.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

					CString ModifyFlag = "MF_" + ClassNameToUpper(StructInfo.MemberList[i].Name);
					PackSize.Replace("<ModifyFlag>", ModifyFlag);

					PackSize.Replace("<SpaceName>", SpaceName);
				}
			}
			else
			{
				RemoveBlock(PackSize, "<IfCheckMF>", "</IfCheckMF>");
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

CString CCallInterfaceMakerDlg::MakeXMLProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString XMLProcess = m_StructXMLProcessTemplate;

	CString LineSpace = GetLineSpace(XMLProcess, "<ToXMLOperations>");
	CString ToXMLOperations = MakeToXMLOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	XMLProcess.Replace("<ToXMLOperations>", ToXMLOperations);

	LineSpace = GetLineSpace(XMLProcess, "<FromXMLOperations>");
	CString FromXMLOperations = MakeFromXMLOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	XMLProcess.Replace("<FromXMLOperations>", FromXMLOperations);

	CString Space = "\r\n";
	Space += szLineSpace;
	XMLProcess.Replace("\r\n", Space);
	return XMLProcess;

}

CString CCallInterfaceMakerDlg::MakeToXMLOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation = m_ToXMLOperationUnitTemplate;
		if (StructInfo.MemberList[i].IsArray)
		{			
			TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);			
			Operation = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.ToXMLOperation, pTypeInfo);
		}

		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
			if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}
			else
			{
				RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
			RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
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

		Operation.Replace("<SpaceName>", SpaceName);

		CString LineSpace = GetLineSpace(Operation, "<ToXMLOperation>");
		CString PackOP = MakeToXMLOperation(StructInfo.MemberList[i].Type,
			StructInfo.MemberList[i].Name,
			StructInfo.Name, StructInfo.ShortName,
			SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
			StructInfo.MemberList[i].IsArray,
			SpaceName, LineSpace);

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
CString CCallInterfaceMakerDlg::MakeFromXMLOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
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

				CString Space;

				CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
				VariableDefine.Replace("<Type>", pTypeInfo->CType);
				VariableDefine.Replace("<Space>", "	");
				VariableDefine.Replace("<Variable>", "ArrayElement");
				VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
				Operation.Replace("<ArrayElementVariableDefine>", VariableDefine);

				CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
				VariableInit.Replace("<Variable>", "ArrayElement");
				Operation.Replace("<ArrayElementVariableInit>", VariableInit);

				Operation = ProcessArrayOperation(Operation, pTypeInfo);
			}

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>", "");
				Operation.Replace("</IfInDataObject>", "");
				if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
				else
				{
					RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
			}
			else
			{
				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
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

			Operation.Replace("<SpaceName>", SpaceName);

			CString LineSpace = GetLineSpace(Operation, "<FromXMLOperation>");
			CString UnpackOP = MakeFromXMLOperation(StructInfo.MemberList[i].Type,
				StructInfo.MemberList[i].Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0,
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
				StructInfo.MemberList[i].IsArray,
				SpaceName, LineSpace);

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


CString CCallInterfaceMakerDlg::MakeJsonProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString JsonProcess = m_StructJsonProcessTemplate;

	CString LineSpace = GetLineSpace(JsonProcess, "<ToJsonOperations>");
	CString ToJsonOperations = MakeToJsonOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	JsonProcess.Replace("<ToJsonOperations>", ToJsonOperations);

	LineSpace = GetLineSpace(JsonProcess, "<FromJsonOperations>");
	CString FromJsonOperations = MakeFromJsonOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	JsonProcess.Replace("<FromJsonOperations>", FromJsonOperations);

	CString Space = "\r\n";
	Space += szLineSpace;
	JsonProcess.Replace("\r\n", Space);
	return JsonProcess;
}

CString CCallInterfaceMakerDlg::MakeDataObjectJsonProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString JsonProcess = m_DataObjectJsonProcessTemplate;

	if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
	{
		RemoveBlock(JsonProcess, "<IfNotInDataObject>", "</IfNotInDataObject>");
		JsonProcess.Replace("<IfInDataObject>", "");
		JsonProcess.Replace("</IfInDataObject>", "");
	}
	else
	{
		RemoveBlock(JsonProcess, "<IfInDataObject>", "</IfInDataObject>");
		JsonProcess.Replace("<IfNotInDataObject>", "");
		JsonProcess.Replace("</IfNotInDataObject>", "");
	}

	CString LineSpace = GetLineSpace(JsonProcess, "<ToJsonOperations>");
	CString ToJsonOperations = MakeToJsonOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	JsonProcess.Replace("<ToJsonOperations>", ToJsonOperations);

	LineSpace = GetLineSpace(JsonProcess, "<FromJsonOperations>");
	CString FromJsonOperations = MakeFromJsonOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	JsonProcess.Replace("<FromJsonOperations>", FromJsonOperations);

	CString Space = "\r\n";
	Space += szLineSpace;
	JsonProcess.Replace("\r\n", Space);
	return JsonProcess;
}
CString CCallInterfaceMakerDlg::MakeToJsonOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation = m_ToJsonOperationUnitTemplate;
		if (StructInfo.MemberList[i].IsArray)
		{
			TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
			Operation = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.ToJsonOperation, pTypeInfo);
		}

		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
			if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}
			else
			{
				RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
			RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
		}

		CString VarName = StructInfo.MemberList[i].Name;
		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			VarName = "m_" + VarName;
		VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
		Operation.Replace("<Variable>", VarName);		

		Operation.Replace("<VariableName>", StructInfo.MemberList[i].Name);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(StructInfo.MemberList[i].Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);

		CString LineSpace = GetLineSpace(Operation, "<ToJsonOperation>");
		CString PackOP = MakeToJsonOperation(StructInfo.MemberList[i].Type,
			StructInfo.MemberList[i].Name,
			StructInfo.Name, StructInfo.ShortName,
			SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
			StructInfo.MemberList[i].IsArray,
			SpaceName, LineSpace);

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
CString CCallInterfaceMakerDlg::MakeFromJsonOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
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
				
				CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
				VariableDefine.Replace("<Type>", pTypeInfo->CType);
				VariableDefine.Replace("<Space>", "	");
				VariableDefine.Replace("<Variable>", "ArrayElement");
				VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
				Operation.Replace("<ArrayElementVariableDefine>", VariableDefine);

				CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
				VariableInit.Replace("<Variable>", "ArrayElement");
				Operation.Replace("<ArrayElementVariableInit>", VariableInit);

				Operation = ProcessArrayOperation(Operation, pTypeInfo);
			}

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>", "");
				Operation.Replace("</IfInDataObject>", "");
				if (StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
				else
				{
					RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
				RetainBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
			}
			else
			{
				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				RemoveBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
			}

			CString SST_ID;

			SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
			SST_ID.MakeUpper();


			Operation.Replace("<SST_NAME>", SSTIDEnumName);
			Operation.Replace("<SST_ID>", SST_ID);

			CString VarName = StructInfo.MemberList[i].Name;
			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				VarName = "m_" + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
			Operation.Replace("<Variable>", VarName);			

			Operation.Replace("<VariableName>", StructInfo.MemberList[i].Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
			Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

			CString ModifyFlag = "MF_" + ClassNameToUpper(StructInfo.MemberList[i].Name);
			Operation.Replace("<ModifyFlag>", ModifyFlag);

			Operation.Replace("<SpaceName>", SpaceName);

			CString LineSpace = GetLineSpace(Operation, "<FromJsonOperation>");
			CString UnpackOP = MakeFromJsonOperation(StructInfo.MemberList[i].Type,
				StructInfo.MemberList[i].Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0,
				(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag&STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
				StructInfo.MemberList[i].IsArray,
				SpaceName, LineSpace);

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

CString CCallInterfaceMakerDlg::MakePackOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, LPCTSTR PacketName, LPCTSTR PacketMemberName, UINT OperationFlag, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.PackOperation;

		//if (OperationFlag & PACK_OPERATION_FLAG_NEED_MF_CHECK)
		//{
		//	RemoveBlock(Operation, "<IfNotMFCheck>", "</IfNotMFCheck>");
		//	Operation.Replace("<IfMFCheck>", "");
		//	Operation.Replace("</IfMFCheck>", "");
		//}
		//else
		//{
		//	RemoveBlock(Operation, "<IfMFCheck>", "</IfMFCheck>");
		//	Operation.Replace("<IfNotMFCheck>", "");
		//	Operation.Replace("</IfNotMFCheck>", "");
		//}

		if (OperationFlag & PACK_OPERATION_FLAG_IN_DATA_OBJECT)
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
		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
		{
			RemoveBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
			Operation.Replace("<IfNotInArray>", "");
			Operation.Replace("</IfNotInArray>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfInArray>", "</IfInArray>");
			Operation.Replace("<IfNotInArray>", "");
			Operation.Replace("</IfNotInArray>", "");
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
		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
		{
			CString ArrayVar = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
			ArrayVar.Replace("<Index>", "i");
			Operation.Replace("<Variable>", ArrayVar);
			Operation = ProcessArrayOperation(Operation, pTypeInfo);
		}

		CString VarName = Name;
		if((OperationFlag & PACK_OPERATION_FLAG_IN_INTERFACE)==0)
		{
			if (OperationFlag & PACK_OPERATION_FLAG_IN_DATA_OBJECT)
				VarName = "m_" + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
		}

		Operation.Replace("<Variable>", VarName);

		Operation.Replace("<VariableName>", Name);
		Operation.Replace("<Type>", pTypeInfo->CType);
		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
			Operation.Replace("<OrginType>", pTypeInfo->Name + "[]");
		else
			Operation.Replace("<OrginType>", pTypeInfo->Name);

		Operation.Replace("<PackFlag>", PackFlag);
		Operation.Replace("<Packet>", PacketName);
		Operation.Replace("<PacketMember>", PacketMemberName);

		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeUnpackOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, LPCTSTR PacketName, LPCTSTR PacketMemberName, UINT OperationFlag, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.UnpackOperation;

		if (OperationFlag & PACK_OPERATION_FLAG_IN_DATA_OBJECT)
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

		if (OperationFlag & PACK_OPERATION_FLAG_IN_INTERFACE)
		{
			RemoveBlock(Operation, "<IfNotInInterface>", "</IfNotInInterface>");
			Operation.Replace("<IfInInterface>", "");
			Operation.Replace("</IfInInterface>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfInInterface>", "</IfInInterface>");
			Operation.Replace("<IfNotInInterface>", "");
			Operation.Replace("</IfNotInInterface>", "");
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


		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
		{
			Operation.Replace("<Variable>", "ArrayElement");
		}

		CString VarName = Name;
		if((OperationFlag & PACK_OPERATION_FLAG_IN_INTERFACE)==0)
		{
			if (OperationFlag & PACK_OPERATION_FLAG_IN_DATA_OBJECT)
				VarName = "m_" + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
		}

		Operation.Replace("<Variable>", VarName);

		Operation.Replace("<VariableName>", Name);
		Operation.Replace("<Type>", pTypeInfo->CType);
		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
			Operation.Replace("<OrginType>", pTypeInfo->Name + "[]");
		else
			Operation.Replace("<OrginType>", pTypeInfo->Name);

		Operation.Replace("<PackFlag>", PackFlag);
		Operation.Replace("<Packet>", PacketName);
		Operation.Replace("<PacketMember>", PacketMemberName);

		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}
CString CCallInterfaceMakerDlg::MakeToXMLOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.ToXMLOperation;

		//if (IsMFCheck)
		//{
		//	RemoveBlock(Operation, "<IfNotMFCheck>", "</IfNotMFCheck>");
		//	Operation.Replace("<IfMFCheck>", "");
		//	Operation.Replace("</IfMFCheck>", "");
		//}
		//else
		//{
		//	RemoveBlock(Operation, "<IfMFCheck>", "</IfMFCheck>");
		//	Operation.Replace("<IfNotMFCheck>", "");
		//	Operation.Replace("</IfNotMFCheck>", "");
		//}

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
			Operation = ProcessArrayOperation(Operation, pTypeInfo);
		}
		else
		{
			RemoveBlock(Operation, "<IfArray>", "</IfArray>");
			Operation.Replace("<IfNotArray>", "");
			Operation.Replace("</IfNotArray>", "");
		}
		
		if (IsInDataObject)
			Operation.Replace("<Variable>", CString("m_") + Name);
		else
			Operation.Replace("<Variable>", Name);
		

		Operation.Replace("<VariableName>", Name);

		Operation.Replace("<Type>", pTypeInfo->CType);

		Operation.Replace("<PackFlag>", PackFlag);

		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeFromXMLOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.FromXMLOperation;

		if (IsMonitorUpdate)
		{
			RetainBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
		}
		else
		{
			RemoveBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
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
			Operation.Replace("<Variable>", CString("m_") + Name);
		else
			Operation.Replace("<Variable>", Name);
		

		Operation.Replace("<VariableName>", Name);
		Operation.Replace("<Type>", pTypeInfo->CType);

		Operation.Replace("<PackFlag>", PackFlag);

		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeToJsonOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.ToJsonOperation;

		//if (IsMFCheck)
		//{
		//	RemoveBlock(Operation, "<IfNotMFCheck>", "</IfNotMFCheck>");
		//	Operation.Replace("<IfMFCheck>", "");
		//	Operation.Replace("</IfMFCheck>", "");
		//}
		//else
		//{
		//	RemoveBlock(Operation, "<IfMFCheck>", "</IfMFCheck>");
		//	Operation.Replace("<IfNotMFCheck>", "");
		//	Operation.Replace("</IfNotMFCheck>", "");
		//}

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
			Operation = ProcessArrayOperation(Operation, pTypeInfo);
		}
		else
		{
			RemoveBlock(Operation, "<IfArray>", "</IfArray>");
			Operation.Replace("<IfNotArray>", "");
			Operation.Replace("</IfNotArray>", "");
		}

		CString VarName = Name;
		if (IsInDataObject)
			VarName = "m_" + VarName;
		VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;

		Operation.Replace("<Variable>", VarName);

		Operation.Replace("<VariableName>", Name);

		Operation.Replace("<Type>", pTypeInfo->CType);

		Operation.Replace("<PackFlag>", PackFlag);

		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}
CString CCallInterfaceMakerDlg::MakeFromJsonOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.FromJsonOperation;

		if (IsMonitorUpdate)
		{
			RetainBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
		}
		else
		{
			RemoveBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
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

		CString VarName = Name;
		if (IsInDataObject)
			VarName = "m_" + VarName;
		VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;

		Operation.Replace("<Variable>", VarName);

		Operation.Replace("<VariableName>", Name);
		Operation.Replace("<Type>", pTypeInfo->CType);

		Operation.Replace("<PackFlag>", PackFlag);

		Operation.Replace("<SST_NAME>", SSTIDEnumName);
		Operation.Replace("<SST_ID>", SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeDBProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString DBProcess = m_StructDBProcessTemplate;

	DBProcess.Replace("<ClassName>", StructInfo.Name);

	CString LineSpace = GetLineSpace(DBProcess, "<DBFieldDefineOperations>");
	CString DBFieldDefineOperations = MakeDBFieldDefineOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace("<DBFieldDefineOperations>", DBFieldDefineOperations);

	LineSpace = GetLineSpace(DBProcess, "<DBIndexDefineOperations>");
	CString DBIndexDefineOperations = MakeDBIndexDefineOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace("<DBIndexDefineOperations>", DBIndexDefineOperations);

	LineSpace = GetLineSpace(DBProcess, "<InsertFieldNames>");
	CString InsertFieldNames = MakeDBFields(StructInfo, DB_INDEX_TYPE_NONE);
	DBProcess.Replace("<InsertFieldNames>", InsertFieldNames);

	LineSpace = GetLineSpace(DBProcess, "<InsertFieldFormats>");
	CString InsertFieldFormats = MakeDBFieldFormats(StructInfo);
	DBProcess.Replace("<InsertFieldFormats>", InsertFieldFormats);

	LineSpace = GetLineSpace(DBProcess, "<DBPutParamBindOperations>");
	CString DBPutParamBindOperations = MakeDBPutParamBindOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace("<DBPutParamBindOperations>", DBPutParamBindOperations);

	LineSpace = GetLineSpace(DBProcess, "<DBPutOperations>");
	CString DBPutOperations;
	STRUCT_DEFINE_INFO * pStructInfo = &StructInfo;
	while (pStructInfo)
	{
		DBPutOperations = MakeDBPutOperations(*pStructInfo, SSTIDEnumName, SpaceName, LineSpace) + DBPutOperations;
		pStructInfo = GetStructDefineInfo(pStructInfo->BaseStruct);
	}
	DBProcess.Replace("<DBPutOperations>", DBPutOperations);

	CString Space = "\r\n";
	Space += szLineSpace;
	DBProcess.Replace("\r\n", Space);
	return DBProcess;
}
CString CCallInterfaceMakerDlg::MakeDataObjectDBProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString DBProcess = m_DataObjectDBProcessTemplate;

	CString LineSpace = GetLineSpace(DBProcess, "<DBFieldDefineOperations>");
	CString DBFieldDefineOperations = MakeDBFieldDefineOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace("<DBFieldDefineOperations>", DBFieldDefineOperations);

	LineSpace = GetLineSpace(DBProcess, "<DBIndexDefineOperations>");
	CString DBIndexDefineOperations = MakeDBIndexDefineOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace("<DBIndexDefineOperations>", DBIndexDefineOperations);

	LineSpace = GetLineSpace(DBProcess, "<InsertFieldNames>");
	CString InsertFieldNames = MakeDBFields(StructInfo, DB_INDEX_TYPE_NONE);
	DBProcess.Replace("<InsertFieldNames>", InsertFieldNames);

	LineSpace = GetLineSpace(DBProcess, "<InsertFieldFormats>");
	CString InsertFieldFormats = MakeDBFieldFormats(StructInfo);
	DBProcess.Replace("<InsertFieldFormats>", InsertFieldFormats);

	LineSpace = GetLineSpace(DBProcess, "<DBPutParamBindOperations>");
	CString DBPutParamBindOperations = MakeDBPutParamBindOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace("<DBPutParamBindOperations>", DBPutParamBindOperations);

	LineSpace = GetLineSpace(DBProcess, "<DBPutOperations>");
	CString DBPutOperations = MakeDBPutOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace("<DBPutOperations>", DBPutOperations);

	CString Space = "\r\n";
	Space += szLineSpace;
	DBProcess.Replace("\r\n", Space);
	return DBProcess;
}
CString CCallInterfaceMakerDlg::MakeDBFieldDefineOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation = m_DBFieldDefineUnitTemplate;
		
		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
			if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}
			else
			{
				RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
			RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
		}		

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(MemberInfo.Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);

		CString LineSpace = GetLineSpace(Operation, "<DBFieldDefine>");
		CString DBFieldDefine = MakeDBFieldDefineOperation(MemberInfo.Type,
			MemberInfo.Name,
			StructInfo.Name, StructInfo.ShortName,
			SSTIDEnumName,
			(StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT) != 0,
			MemberInfo.IsArray,
			MemberInfo.DBLength,
			SpaceName, LineSpace);

		Operation.Replace("<DBFieldDefine>", DBFieldDefine);

		
		Operations += Operation;
		Operations += "\r\n";
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeDBIndexDefineOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;
	CString PrimaryKeyFields = MakeDBFields(StructInfo, DB_INDEX_TYPE_PRIMARY_KEY);
	bool IsPrimaryDefinded = false;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation;
		switch (MemberInfo.DBIndexType)
		{
		case DB_INDEX_TYPE_KEY:
			Operation = m_DBKeyDefineUnitTemplate;
			break;
		case DB_INDEX_TYPE_PRIMARY_KEY:
			if (IsPrimaryDefinded)
			{
				continue;
			}
			else
			{
				Operation = m_DBPrimaryKeyDefineUnitTemplate;
				IsPrimaryDefinded = true;
			}
			break;
		case DB_INDEX_TYPE_UNIQUE_KEY:
			Operation = m_DBUniqueKeyDefineUnitTemplate;
			break;
		default:
			continue;
		}

		if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
			Operation.Replace("<IfInDataObject>", "");
			Operation.Replace("</IfInDataObject>", "");
			if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}
			else
			{
				RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}
		}
		else
		{
			RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
			Operation.Replace("<IfNotInDataObject>", "");
			Operation.Replace("</IfNotInDataObject>", "");
			RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
		}

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
		Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

		CString ModifyFlag = "MF_" + ClassNameToUpper(MemberInfo.Name);
		Operation.Replace("<ModifyFlag>", ModifyFlag);

		Operation.Replace("<SpaceName>", SpaceName);
		Operation.Replace("<FieldNames>", PrimaryKeyFields);
		Operation.Replace("<FieldName>", MemberInfo.Name);
		Operation.Replace("<!FieldName>", ClassNameToUpper(MemberInfo.Name));

		Operations += Operation;
		Operations += "\r\n";
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}

CString CCallInterfaceMakerDlg::MakeDBPutParamBindOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE * pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			if (((pTypeInfo->Flag&(TYPE_DEFINE_FLAG_DB_USE_PARAM_BIND | TYPE_DEFINE_FLAG_STRUCT)) == 0) && (!MemberInfo.IsArray))
				continue;

			CString Operation;

			if (MemberInfo.IsArray)
			{
				CString Var = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
				Var.Replace("<Variable>", MemberInfo.Name);
				Var.Replace("<Index>", "i");
				CString SizeCaculateOperation = pTypeInfo->GenerateOperations.SizeCaculateOperation;
				SizeCaculateOperation.Replace("<Variable>", Var);
				SizeCaculateOperation.Replace("<Type>", pTypeInfo->CType);
				RemoveBlock(SizeCaculateOperation, "<IfInArray>", "</IfInArray>");
				SizeCaculateOperation.Replace("<IfNotInArray>", "");
				SizeCaculateOperation.Replace("</IfNotInArray>", "");
				CString PackOperation = pTypeInfo->GenerateOperations.PackOperation;
				PackOperation.Replace("<Variable>", Var);
				PackOperation.Replace("<Type>", pTypeInfo->CType);
				RemoveBlock(PackOperation, "<IfInArray>", "</IfInArray>");
				PackOperation.Replace("<IfNotInArray>", "");
				PackOperation.Replace("</IfNotInArray>", "");

				Operation = m_InterfaceConfig.ArrayDefineConfig.DBPutOperation;
				Operation.Replace("<ArrayElementSize>", SizeCaculateOperation);
				CString LineSpace = GetLineSpace(Operation, "<PackOperation>");
				CString Space = "\r\n";
				Space += LineSpace;
				PackOperation.Replace("\r\n", Space);
				Operation.Replace("<PackOperation>", PackOperation);		

				Operation = ProcessArrayOperation(Operation, pTypeInfo);
				
			}
			else
			{
				Operation = pTypeInfo->GenerateOperations.DBPutOperation;
			}

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>", "");
				Operation.Replace("</IfInDataObject>", "");
				if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
				else
				{
					RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
			}
			else
			{
				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
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

			CString SST_ID;

			SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
			SST_ID.MakeUpper();


			Operation.Replace("<SST_NAME>", SSTIDEnumName);
			Operation.Replace("<SST_ID>", SST_ID);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
			Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

			CString ModifyFlag = "MF_" + ClassNameToUpper(MemberInfo.Name);
			Operation.Replace("<ModifyFlag>", ModifyFlag);

			Operation.Replace("<SpaceName>", SpaceName);
			Operation.Replace("<FieldName>", MemberInfo.Name);
			Operation.Replace("<!FieldName>", ClassNameToUpper(MemberInfo.Name));

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				Operation.Replace("<Variable>", CString("m_") + MemberInfo.Name);
			else
				Operation.Replace("<Variable>", MemberInfo.Name);


			Operation.Replace("<VariableName>", MemberInfo.Name);

			Operation.Replace("<Type>", pTypeInfo->CType);

			CString FullModifyFlag;
			FullModifyFlag.Format("DOMF_%s_FULL", ClassNameToUpper(MemberInfo.Type));
			Operation.Replace("<FULL_MODIFY_FLAG>", FullModifyFlag);

			Operations += Operation;
			Operations += "\r\n";
			Operations += "\r\n";
		}
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeDBPutOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;
	UINT Count = 0;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE * pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			if ((pTypeInfo->Flag&(TYPE_DEFINE_FLAG_DB_USE_PARAM_BIND | TYPE_DEFINE_FLAG_STRUCT)) || MemberInfo.IsArray)
				continue;

			CString Operation = pTypeInfo->GenerateOperations.DBPutOperation;

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>", "");
				Operation.Replace("</IfInDataObject>", "");
				if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
				else
				{
					RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
			}
			else
			{
				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
			Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

			CString ModifyFlag = "MF_" + ClassNameToUpper(MemberInfo.Name);
			Operation.Replace("<ModifyFlag>", ModifyFlag);

			Operation.Replace("<SpaceName>", SpaceName);
			Operation.Replace("<FieldName>", MemberInfo.Name);
			Operation.Replace("<!FieldName>", ClassNameToUpper(MemberInfo.Name));

			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
				Operation.Replace("<Variable>", CString("m_") + MemberInfo.Name);
			else
				Operation.Replace("<Variable>", MemberInfo.Name);


			Operation.Replace("<VariableName>", MemberInfo.Name);

			Operation.Replace("<Type>", pTypeInfo->CType);

			CString FullModifyFlag;
			FullModifyFlag.Format("DOMF_%s_FULL", ClassNameToUpper(MemberInfo.Type));
			Operation.Replace("<FULL_MODIFY_FLAG>", FullModifyFlag);

			Operations += "," + Operation;
			Operations += "\r\n";
			Count++;
		}
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}

CString CCallInterfaceMakerDlg::MakeDBFields(STRUCT_DEFINE_INFO& StructInfo, BYTE DBIndexType)
{
	CString Fields;

	UINT Count = 0;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		if ((DBIndexType == DB_INDEX_TYPE_NONE) || (MemberInfo.DBIndexType == DBIndexType))
		{
			CString Operation = m_DBFieldNameTemplate;
			if (Count)
			{
				Operation.Replace("<IfNotFirst>", "");
				Operation.Replace("</IfNotFirst>", "");
			}
			else
			{
				RemoveBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
			}
			Operation.Replace("<FieldName>", MemberInfo.Name);
			Fields += Operation;
			Count++;
		}
	}
	return Fields;
}
CString CCallInterfaceMakerDlg::MakeDBFieldDefineOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, bool IsInDataObject, bool IsArray, LPCTSTR DBLength, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE * pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.DBFieldDefineOperation;

		if (IsArray)
		{
			RemoveBlock(Operation, "<IfNotArray>", "</IfNotArray>");
			Operation.Replace("<IfArray>", "");
			Operation.Replace("</IfArray>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfArray>", "</IfArray>");
			Operation.Replace("<IfNotArray>", "");
			Operation.Replace("</IfNotArray>", "");
		}

		Operation.Replace("<FieldName>", Name);		
		Operation.Replace("<DBLength>", DBLength);

		CString FullModifyFlag;
		FullModifyFlag.Format("DOMF_%s_FULL", ClassNameToUpper(Name));
		Operation.Replace("<FULL_MODIFY_FLAG>", FullModifyFlag);

		Operation.Replace("<VariableName>", Name);
		Operation.Replace("<Type>", pTypeInfo->CType);
		Operation.Replace("<SpaceName>", SpaceName);
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeDBFieldFormats(STRUCT_DEFINE_INFO& StructInfo)
{
	CString Formats;

	UINT Count = 0;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag&STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE * pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			if (Count)
				Formats += ", " + pTypeInfo->GenerateOperations.DBInsertFormatOperation;
			else
				Formats += pTypeInfo->GenerateOperations.DBInsertFormatOperation;
			Count++;
		}
	}
	return Formats;
}

CString CCallInterfaceMakerDlg::MakeLogProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString DBProcess = m_StructLogProcessTemplate;

	DBProcess.Replace("<ClassName>", StructInfo.Name);

	UINT MemberCount = GetStructMemberCount(StructInfo);
	CString Temp;
	Temp.Format("%u", MemberCount);
	DBProcess.Replace("<LogDataCount>", Temp);

	CString LineSpace = GetLineSpace(DBProcess, "<LogDataList>");
	CString Operations = MakeLogDataList(StructInfo, LineSpace);
	DBProcess.Replace("<LogDataList>", Operations);

	CString Space = "\r\n";
	Space += szLineSpace;
	DBProcess.Replace("\r\n", Space);
	return DBProcess;
}

CString CCallInterfaceMakerDlg::MakeLogDataList(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Operations;

	if (!StructInfo.BaseStruct.IsEmpty())
	{
		STRUCT_DEFINE_INFO* pStructInfo = GetStructDefineInfo(StructInfo.BaseStruct);
		if (pStructInfo)
		{
			Operations += MakeLogDataList(*pStructInfo, szLineSpace);
		}
	}

	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (!Operations.IsEmpty())
		{
			Operations += ",\r\n";
			Operations += szLineSpace;
		}			

		TYPE_DEFINE* pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			CString Operation = pTypeInfo->GenerateOperations.LogSendOperation;
			Operation.Replace("<VariableName>", MemberInfo.Name);
			CString VarName;
			VarName = MemberInfo.Name;
			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				VarName = "m_" + VarName;
			}
			Operation.Replace("<Variable>", VarName);
			Operations += Operation;
		}
		else
		{
			Operations += "<ErrorType>";
		}
	}

	return Operations;
}

CString CCallInterfaceMakerDlg::MakeLuaProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Process = m_StructLuaProcessTemplate;

	Process.Replace("<ClassName>", StructInfo.Name);

	if (StructInfo.BaseStruct.IsEmpty())
	{
		RemoveBlock(Process, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
		RetainBlock(Process, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
		Process.Replace("<BaseClass>", "");

	}
	else
	{
		RetainBlock(Process, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
		RemoveBlock(Process, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
		Process.Replace("<BaseClass>", StructInfo.BaseStruct);
	}
		

	CString LineSpace = GetLineSpace(Process, "<ToLuaTableOperations>");
	CString Operations = MakeToLuaTableOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	Process.Replace("<ToLuaTableOperations>", Operations);

	LineSpace = GetLineSpace(Process, "<FromLuaTableOperations>");
	Operations = MakeFromLuaTableOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	Process.Replace("<FromLuaTableOperations>", Operations);

	CString Space = "\r\n";
	Space += szLineSpace;
	Process.Replace("\r\n", Space);
	return Process;
}

CString CCallInterfaceMakerDlg::MakeDataObjectLuaProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Process = m_DataObjectLuaProcessTemplate;

	Process.Replace("<ClassName>", StructInfo.Name);

	if (StructInfo.BaseStruct.IsEmpty())
	{
		RemoveBlock(Process, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
		RetainBlock(Process, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
		Process.Replace("<BaseClass>", "");

	}
	else
	{
		RetainBlock(Process, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
		RemoveBlock(Process, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
		Process.Replace("<BaseClass>", StructInfo.BaseStruct);
	}	

	CString LineSpace = GetLineSpace(Process, "<ToLuaTableOperations>");
	CString Operations = MakeToLuaTableOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	Process.Replace("<ToLuaTableOperations>", Operations);

	LineSpace = GetLineSpace(Process, "<FromLuaTableOperations>");
	Operations = MakeFromLuaTableOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	Process.Replace("<FromLuaTableOperations>", Operations);

	CString Space = "\r\n";
	Space += szLineSpace;
	Process.Replace("\r\n", Space);
	return Process;
}

CString CCallInterfaceMakerDlg::MakeToLuaTableOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	STRUCT_MEMBER_INFO* pKeyMember = NULL;

	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_IGNORE_IN_LUA_PROCESS)
			continue;
		CString Operation = m_ToLuaTableOperationUnitTemplate;
		TYPE_DEFINE* pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo && (!pTypeInfo->GenerateOperations.ToLuaOperation.IsEmpty()))
		{
			if (MemberInfo.IsArray)
			{
				Operation = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.ToLuaOperation, pTypeInfo);
				STRUCT_MEMBER_INFO* pKeyMember = NULL;
				if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_STRUCT)
				{
					pKeyMember = GetStructKeyMember(pTypeInfo->Name);
				}
				if (pKeyMember)
				{
					RemoveBlock(Operation, "<IfNotHaveKey>", "</IfNotHaveKey>");
					RetainBlock(Operation, "<IfHaveKey>", "</IfHaveKey>");
					Operation.Replace("<StructKey>", pKeyMember->Name);
				}
				else
				{
					RemoveBlock(Operation, "<IfHaveKey>", "</IfHaveKey>");
					RetainBlock(Operation, "<IfNotHaveKey>", "</IfNotHaveKey>");
				}
			}

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>", "");
				Operation.Replace("</IfInDataObject>", "");
				if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
				else
				{
					RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
			}
			else
			{
				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
			}

			CString VarName = MemberInfo.Name;
			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				VarName = "m_" + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
			Operation.Replace("<Variable>", VarName);

			Operation.Replace("<VariableName>", MemberInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
			Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

			CString ModifyFlag = "MF_" + ClassNameToUpper(MemberInfo.Name);
			Operation.Replace("<ModifyFlag>", ModifyFlag);

			Operation.Replace("<SpaceName>", SpaceName);

			CString LineSpace = GetLineSpace(Operation, "<ToLuaOperation>");
			CString PackOP = MakeToLuaTableOperation(pTypeInfo,
				MemberInfo.Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName,
				(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0,
				MemberInfo.IsArray,
				SpaceName, LineSpace);

			Operation.Replace("<ToLuaOperation>", PackOP);

			CString SST_ID;

			SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
			SST_ID.MakeUpper();


			Operation.Replace("<SST_NAME>", SSTIDEnumName);
			Operation.Replace("<SST_ID>", SST_ID);

			//if (MemberInfo.IsArray)
			//{
			//	CString ArrayOperation = Operation;
			//	Operation = m_ToLuaTableOperationUnitTemplate;
			//	Operation.Replace("<Variable>", VarName);
			//	Operation.Replace("<VariableName>", MemberInfo.Name);
			//	Operation.Replace("<ToLuaOperation>", ArrayOperation);
			//}

			Operations += Operation;
			Operations += "\r\n";
		}
		
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}

CString CCallInterfaceMakerDlg::MakeToLuaTableOperation(TYPE_DEFINE* pTypeInfo, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, bool IsInDataObject, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	
	Operation = pTypeInfo->GenerateOperations.ToLuaOperation;

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

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
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

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
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
		Operation = ProcessArrayOperation(Operation, pTypeInfo);
	}
	else
	{
		RemoveBlock(Operation, "<IfArray>", "</IfArray>");
		Operation.Replace("<IfNotArray>", "");
		Operation.Replace("</IfNotArray>", "");
	}	

	CString VarName = Name;
	if (IsInDataObject)
		VarName = "m_" + VarName;
	VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;

	Operation.Replace("<Variable>", VarName);

	Operation.Replace("<VariableName>", Name);

	Operation.Replace("<Type>", pTypeInfo->CType);

	Operation.Replace("<SST_NAME>", SSTIDEnumName);
	Operation.Replace("<SST_ID>", SST_ID);

	CString ModifyFlagEnumName;
	ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
	Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

	CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
	Operation.Replace("<ModifyFlag>", ModifyFlag);

	Operation.Replace("<SpaceName>", SpaceName);
	

	CString Space = "\r\n";
	Space += szLineSpace;
	Operation.Replace("\r\n", Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeFromLuaTableOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	STRUCT_MEMBER_INFO* pKeyMember = NULL;

	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_IGNORE_IN_LUA_PROCESS)
			continue;
		CString Operation = m_FromLuaTableOperationUnitTemplate;
		TYPE_DEFINE* pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo && (!pTypeInfo->GenerateOperations.FromLuaOperation.IsEmpty()))
		{
			if (MemberInfo.IsArray)
			{
				Operation = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.FromLuaOperation, pTypeInfo);

				CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
				VariableDefine.Replace("<Type>", pTypeInfo->CType);
				VariableDefine.Replace("<Space>", "	");
				VariableDefine.Replace("<Variable>", "ArrayElement");
				VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
				Operation.Replace("<ArrayElementVariableDefine>", VariableDefine);

				CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
				VariableInit.Replace("<Variable>", "ArrayElement");
				Operation.Replace("<ArrayElementVariableInit>", VariableInit);

				STRUCT_MEMBER_INFO* pKeyMember = NULL;
				if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_STRUCT)
				{
					pKeyMember = GetStructKeyMember(pTypeInfo->Name);
				}
				if (pKeyMember)
				{
					RemoveBlock(Operation, "<IfNotHaveKey>", "</IfNotHaveKey>");
					RetainBlock(Operation, "<IfHaveKey>", "</IfHaveKey>");
					Operation.Replace("<StructKey>", pKeyMember->Name);
				}
				else
				{
					RemoveBlock(Operation, "<IfHaveKey>", "</IfHaveKey>");
					RetainBlock(Operation, "<IfNotHaveKey>", "</IfNotHaveKey>");
				}
			}

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
				Operation.Replace("<IfInDataObject>", "");
				Operation.Replace("</IfInDataObject>", "");
				if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
				else
				{
					RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				}
				RetainBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
			}
			else
			{
				RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
				Operation.Replace("<IfNotInDataObject>", "");
				Operation.Replace("</IfNotInDataObject>", "");
				RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
				RemoveBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
			}

			CString VarName = MemberInfo.Name;
			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				VarName = "m_" + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
			Operation.Replace("<Variable>", VarName);

			Operation.Replace("<VariableName>", MemberInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
			Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

			CString ModifyFlag = "MF_" + ClassNameToUpper(MemberInfo.Name);
			Operation.Replace("<ModifyFlag>", ModifyFlag);

			Operation.Replace("<SpaceName>", SpaceName);

			CString LineSpace = GetLineSpace(Operation, "<FromLuaOperation>");
			CString PackOP = MakeFromLuaTableOperation(pTypeInfo,
				MemberInfo.Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName,
				(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0,
				MemberInfo.IsArray,
				SpaceName, LineSpace);

			Operation.Replace("<FromLuaOperation>", PackOP);

			CString SST_ID;

			SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
			SST_ID.MakeUpper();


			Operation.Replace("<SST_NAME>", SSTIDEnumName);
			Operation.Replace("<SST_ID>", SST_ID);

			//if (MemberInfo.IsArray)
			//{
			//	CString ArrayOperation = Operation;
			//	Operation = m_FromLuaTableOperationUnitTemplate;
			//	Operation.Replace("<Variable>", VarName);
			//	Operation.Replace("<VariableName>", MemberInfo.Name);
			//	Operation.Replace("<FromLuaOperation>", ArrayOperation);
			//}

			Operations += Operation;
			Operations += "\r\n";
		}

	}
	CString Space = "\r\n";
	Space += szLineSpace;
	Operations.Replace("\r\n", Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeFromLuaTableOperation(TYPE_DEFINE* pTypeInfo, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, bool IsInDataObject, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;


	Operation = pTypeInfo->GenerateOperations.FromLuaOperation;

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

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
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

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
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

		Operation.Replace("<Variable>", "ArrayElement");
	}
	else
	{
		RemoveBlock(Operation, "<IfArray>", "</IfArray>");
		Operation.Replace("<IfNotArray>", "");
		Operation.Replace("</IfNotArray>", "");
	}
	CString VarName = Name;
	if (IsInDataObject)
		VarName = "m_" + VarName;
	VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;

	Operation.Replace("<Variable>", VarName);

	Operation.Replace("<VariableName>", Name);

	Operation.Replace("<Type>", pTypeInfo->CType);

	Operation.Replace("<SST_NAME>", SSTIDEnumName);
	Operation.Replace("<SST_ID>", SST_ID);

	CString ModifyFlagEnumName;
	ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ParentName));
	Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

	CString ModifyFlag = "MF_" + ClassNameToUpper(Name);
	Operation.Replace("<ModifyFlag>", ModifyFlag);

	Operation.Replace("<SpaceName>", SpaceName);


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

CString CCallInterfaceMakerDlg::MakeMsgHandlerMethodsSource(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName, LPCTSTR szLineSpace)
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

			Method.Replace("<SpaceName>", SpaceName);
			Method.Replace("<InterfaceName>", InterfaceName);
			Method.Replace("<@InterfaceName>", InterfaceInfo.Name);
			Method.Replace("<MethodName>", MethodName);
			CString Params = MakeParams(*pParamList, false, true);
			Method.Replace("<Params>", Params);
			CString LineSpace = GetLineSpace(Method, "<ParamDefines>");
			CString ParamDefines = MakeParamDefines(*pParamList, SpaceName, ";", LineSpace);
			Method.Replace("<ParamDefines>", ParamDefines);

			LineSpace = GetLineSpace(Method, "<ParamInitOperations>");
			CString ParamInitOperations = MakeInitOperations(MethodInfo, ExportType, LineSpace);
			Method.Replace("<ParamInitOperations>", ParamInitOperations);

			CString PacketName = m_InterfaceConfig.DefaultPacketName;
			CString PacketMemberName = m_InterfaceConfig.DefaultPacketMemberName;

			LineSpace = GetLineSpace(Method, "<UnpackOperations>");
			CString SSTIDEnumName;
			SSTIDEnumName.Format("%s_%s_MEMBER_IDS", ClassNameToUpper(InterfaceName), ClassNameToUpper(MethodName));
			CString UnpackOperations = MakeUnpackOperations(MethodInfo, ExportType, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
			Method.Replace("<UnpackOperations>", UnpackOperations);

			Methods += Method + "\r\n";
		}
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Methods.Replace("\r\n",Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgMapInits(CALLER_INTERFACE& InterfaceInfo, LPCTSTR SpaceName, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
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
			MapInit.Replace("<SpaceName>", SpaceName);
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

CString CCallInterfaceMakerDlg::MakeStructDefines(vector<STRUCT_DEFINE_INFO*>& StructDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString StructDefines;
	for (size_t i = 0; i < StructDefineList.size(); i++)
	{
		STRUCT_DEFINE_INFO& StructDefine = *(StructDefineList[i]);
		if ((StructDefine.Flag & STRUCT_FLAG_IS_DATA_OBJECT) == 0)
		{
			CString Struct = m_StructDefineTemplate;

			Struct.Replace("<SpaceName>", SpaceName);

			CString LineSpace;
			bool NeedVTable = false;

			CString SSTIDEnumName;
			SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructDefine.Name));

			if (StructDefine.Flag & STRUCT_FLAG_EXPORT_XML_PROCESS)
			{
				LineSpace = GetLineSpace(Struct, "<XMLProcess>");
				CString XMLProcess = MakeXMLProcess(StructDefine, SSTIDEnumName, SpaceName, LineSpace);
				Struct.Replace("<XMLProcess>", XMLProcess);
			}
			else
			{
				Struct.Replace("<XMLProcess>", "");
			}

			if (StructDefine.Flag & STRUCT_FLAG_EXPORT_JSON_PROCESS)
			{
				LineSpace = GetLineSpace(Struct, "<JsonProcess>");
				CString JsonProcess = MakeJsonProcess(StructDefine, SSTIDEnumName, SpaceName, LineSpace);
				Struct.Replace("<JsonProcess>", JsonProcess);
			}
			else
			{
				Struct.Replace("<JsonProcess>", "");
			}

			if (StructDefine.Flag & STRUCT_FLAG_EXPORT_DB_PROCESS)
			{
				LineSpace = GetLineSpace(Struct, "<DBProcess>");
				CString Process = MakeDBProcess(StructDefine, SSTIDEnumName, SpaceName, LineSpace);
				Struct.Replace("<DBProcess>", Process);
				NeedVTable = true;
			}
			else
			{
				Struct.Replace("<DBProcess>", "");
			}

			if (Struct.Find("<EditorProcess>") >= 0)
			{
				if (StructDefine.Flag & STRUCT_FLAG_EXPORT_EDITOR_PROCESS)
				{
					LineSpace = GetLineSpace(Struct, "<EditorProcess>");
					CString Process = MakeStructEditorProcess(StructDefine, LineSpace);
					Struct.Replace("<EditorProcess>", Process);
				}
				else
				{
					Struct.Replace("<EditorProcess>", "");
				}
			}

			if (Struct.Find("<LogProcess>") >= 0)
			{
				if (StructDefine.Flag & STRUCT_FLAG_EXPORT_LOG_PROCESS)
				{
					LineSpace = GetLineSpace(Struct, "<LogProcess>");
					CString Process = MakeLogProcess(StructDefine, SpaceName, LineSpace);
					Struct.Replace("<LogProcess>", Process);
					NeedVTable = true;
				}
				else
				{
					Struct.Replace("<LogProcess>", "");
				}
			}

			if(Struct.Find("<LuaProcess>") >= 0)
			{
				if (StructDefine.Flag & STRUCT_FLAG_EXPORT_LUA_PROCESS)
				{
					LineSpace = GetLineSpace(Struct, "<LuaProcess>");
					CString Process = MakeLuaProcess(StructDefine, SSTIDEnumName, SpaceName, LineSpace);
					Struct.Replace("<LuaProcess>", Process);
					NeedVTable = true;
				}
				else
				{
					Struct.Replace("<LuaProcess>", "");
				}
			}

			if (Struct.Find("<XLSProcess>") >= 0)
			{
				if (StructDefine.Flag & STRUCT_FLAG_EXPORT_XLS_PROCESS)
				{
					LineSpace = GetLineSpace(Struct, "<XLSProcess>");
					CString Process = MakeStructXLSProcess(StructDefine, LineSpace);
					Struct.Replace("<XLSProcess>", Process);
				}
				else
				{
					Struct.Replace("<XLSProcess>", "");
				}
			}

			if (NeedVTable)
				RetainBlock(Struct, "<NeedVTable>", "</NeedVTable>");
			else
				RemoveBlock(Struct, "<NeedVTable>", "</NeedVTable>");

			if (StructDefine.MemberList.size())
			{
				Struct.Replace("<IfHaveMember>", "");
				Struct.Replace("</IfHaveMember>", "");
			}
			else
			{
				RemoveBlock(Struct, "<IfHaveMember>", "</IfHaveMember>");
			}

			if (StructDefine.BaseStruct.IsEmpty())
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
				Struct.Replace("<BaseClass>", StructDefine.BaseStruct);
			}

			LineSpace = GetLineSpace(Struct, "<StructDescription>");
			CString StructDescription = ToComment(StructDefine.Description, LineSpace);
			Struct.Replace("<StructDescription>", StructDescription);
			Struct.Replace("<StructName>", StructDefine.Name);
			CString BaseStruct;
			if (!StructDefine.BaseStruct.IsEmpty())
			{
				BaseStruct = StructDefine.BaseStruct;
			}
			Struct.Replace("<BaseStruct>", BaseStruct);

			LineSpace = GetLineSpace(Struct, "<Members>");
			CString Members = MakeStructMembers(StructDefine, ";", SpaceName, LineSpace);
			Struct.Replace("<Members>", Members);

			LineSpace = GetLineSpace(Struct, "<Members,>");
			Members = MakeStructMembers(StructDefine, ",", SpaceName, LineSpace);
			Struct.Replace("<Members,>", Members);

			LineSpace = GetLineSpace(Struct, "<SSTIDEnumDefine>");
			CString SSTIDEnum = MakeStructSSTIDEnumDefines(StructDefine, SpaceName, LineSpace);
			Struct.Replace("<SSTIDEnumDefine>", SSTIDEnum);

			LineSpace = GetLineSpace(Struct, "<InitOperations>");
			CString InitOperations = MakeInitOperations(StructDefine, LineSpace);
			Struct.Replace("<InitOperations>", InitOperations);

			CString PacketName = m_InterfaceConfig.DefaultPacketName;
			CString PacketMemberName = m_InterfaceConfig.DefaultPacketMemberName;

			LineSpace = GetLineSpace(Struct, "<PackOperations>");
			CString PackOperations = MakePackOperations(StructDefine, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
			Struct.Replace("<PackOperations>", PackOperations);

			LineSpace = GetLineSpace(Struct, "<UnpackOperations>");
			CString UnpackOperations = MakeUnpackOperations(StructDefine, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
			Struct.Replace("<UnpackOperations>", UnpackOperations);

			LineSpace = GetLineSpace(Struct, "<PacketSizes>");
			CString PackSizes = MakePackSizes(StructDefine, SpaceName, LineSpace);
			Struct.Replace("<PacketSizes>", PackSizes);

			//LineSpace = GetLineSpace(Struct, "<ToXMLOperations>");
			//CString ToXMLOperations = MakeToXMLOperations(StructDefine, SSTIDEnumName, LineSpace);
			//Struct.Replace("<ToXMLOperations>", ToXMLOperations);

			//LineSpace = GetLineSpace(Struct, "<FromXMLOperations>");
			//CString FromXMLOperations = MakeFromXMLOperations(StructDefine, SSTIDEnumName, LineSpace);
			//Struct.Replace("<FromXMLOperations>", FromXMLOperations);



			LineSpace = GetLineSpace(Struct, "<CloneFromOperations>");
			CString CloneOperations = MakeCloneOperations(StructDefine, m_InterfaceConfig.MemberVariablePrefix, "Value.", SpaceName, LineSpace);
			Struct.Replace("<CloneFromOperations>", CloneOperations);

			//LineSpace = GetLineSpace(Struct, "<CloneToOperations>");
			//CloneOperations = MakeCloneOperations(StructDefine, LineSpace, "CloneObject.", m_InterfaceConfig.MemberVariablePrefix, SpaceName);
			//Struct.Replace("<CloneToOperations>", CloneOperations);

			StructDefines += Struct + "\r\n\r\n";
		}
	}
	CString Space = "\r\n";
	Space += szLineSpace;
	StructDefines.Replace("\r\n", Space);
	return StructDefines;
}

CString CCallInterfaceMakerDlg::MakeStructMembers(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SplitChar, LPCTSTR SpaceName, LPCTSTR szLineSpace)
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
				VariableDefine = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.VariableDefine, pTypeInfo);
			}

			VariableDefine.Replace("<Type>",pTypeInfo->CType);
			int CurSpaceCount=(int)ceil((SpaceLen-pTypeInfo->CType.GetLength())/4.0f);
			while(CurSpaceCount)
			{
				Space+="	";
				CurSpaceCount--;
			}
			VariableDefine.Replace("<Space>",Space);
			VariableDefine.Replace("<SpaceName>", SpaceName);
			CString VarName;
			
				
			VarName=StructInfo.MemberList[i].Name;
			if (StructInfo.Flag&STRUCT_FLAG_IS_DATA_OBJECT)
			{
				VarName="m_"+VarName;
			}
			VariableDefine.Replace("<Variable>",VarName);
			VariableDefine = VariableDefine + SplitChar;
			Member.Replace("<VariableDefine>",VariableDefine);
		}
		CString Description=StructInfo.MemberList[i].Description;
		if(!Description.IsEmpty())
		{
			Description = m_InterfaceConfig.CommentPrefix + Description;
			Description.Replace("\r","");
			Description.Replace("\n","");
		}		
		Member.Replace("<Description>",Description);

		Members += Member + "\r\n";
	}
	CString Space="\r\n";
	Space+=szLineSpace;
	Members.Replace("\r\n",Space);
	return Members;
}

CString CCallInterfaceMakerDlg::MakeCloneOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLeftValue, LPCTSTR szRightValue, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Source;

	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			
			CString Operation = MakeCloneOperation(MemberInfo.Name, pTypeInfo, MemberInfo.IsArray,
				StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT, StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT, 
				(MemberInfo.Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0, MemberInfo.PackFlag,
				szLeftValue, szRightValue, StructInfo.Name, SpaceName, NULL);
			Source += Operation + "\r\n";
		}
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Source.Replace("\r\n", Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeCloneOperation(LPCTSTR VariableName, TYPE_DEFINE* pTypeInfo, bool IsArray, bool InDataObject, bool BeCheckMF, bool BeUpdateMF,
	LPCTSTR PackFlag, LPCTSTR szLeftValue, LPCTSTR szRightValue, LPCTSTR ClassName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;
	if (IsArray)
	{
		Operation = m_InterfaceConfig.ArrayDefineConfig.CloneOperation;
		CString SubOperation = pTypeInfo->GenerateOperations.CloneOperation;
		CString LineSpace = GetLineSpace(Operation, "<CloneOperation>");
		RemoveBlock(SubOperation, "<IfNotInArray>", "</IfNotInArray>");
		RetainBlock(SubOperation, "<IfInArray>", "</IfInArray>");
		CString ArrayVar = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
		ArrayVar.Replace("<Index>", "i");
		SubOperation.Replace("<Variable>", ArrayVar);
		RemoveBlock(SubOperation, "<IfInDataObject>", "</IfInDataObject>");
		RemoveBlock(SubOperation, "<IfCheckMF>", "</IfCheckMF>");
		RemoveBlock(SubOperation, "<IfUpdateMF>", "</IfUpdateMF>");
		SubOperation.Replace("\r\n", "\r\n" + LineSpace);
		Operation.Replace("<CloneOperation>", SubOperation);

		CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
		VariableDefine.Replace("<Type>", pTypeInfo->CType);
		VariableDefine.Replace("<Space>", "	");
		VariableDefine.Replace("<Variable>", "ArrayElement");
		VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
		Operation.Replace("<ArrayElementVariableDefine>", VariableDefine);

		CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
		VariableInit.Replace("<Variable>", "ArrayElement");
		Operation.Replace("<ArrayElementVariableInit>", VariableInit);
	}
	else
	{
		Operation = pTypeInfo->GenerateOperations.CloneOperation;
	}

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
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

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
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

	if (pTypeInfo->Flag & (TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
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
	CString VarName = VariableName;
	if (InDataObject)
	{
		VarName = "m_" + VarName;
		RemoveBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
		RetainBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");

		if (BeCheckMF)
			RetainBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
		else
			RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");

		if (BeUpdateMF)
			RetainBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
		else
			RemoveBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
	}
	else
	{
		RemoveBlock(Operation, "<IfInDataObject>", "</IfInDataObject>");
		RetainBlock(Operation, "<IfNotInDataObject>", "</IfNotInDataObject>");
		RemoveBlock(Operation, "<IfCheckMF>", "</IfCheckMF>");
		RemoveBlock(Operation, "<IfUpdateMF>", "</IfUpdateMF>");
	}

	Operation.Replace("<LeftValue>", szLeftValue);
	Operation.Replace("<RightValue>", szRightValue);

	Operation.Replace("<Type>", pTypeInfo->CType);
	Operation.Replace("<VariableName>", VariableName);

	Operation.Replace("<PackFlag>", PackFlag);

	Operation.Replace("<Variable>", VarName);
	Operation.Replace("<ClassName>", ClassName);

	CString ModifyFlagEnumName;
	ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ClassName));
	Operation.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

	CString ModifyFlag = "MF_" + ClassNameToUpper(VariableName);
	Operation.Replace("<ModifyFlag>", ModifyFlag);

	Operation.Replace("<SpaceName>", SpaceName);

	if (szLineSpace && (szLineSpace[0]))
	{
		CString Space = "\r\n";
		Space += szLineSpace;
		Operation.Replace("\r\n", Space);
	}
	return Operation;
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

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagEnumDefine(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{

	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
	EnumInfo.ShortName = EnumInfo.Name;
	EnumInfo.Flag |= ENUM_DEFINE_FLAG_IS_64BIT;

	EnumInfo.MemberList.resize(StructInfo.MemberList.size());

	CString EnumType = "UINT64";
	TYPE_DEFINE* pType = FindVarType(EnumType);
	if (pType)
		EnumType = pType->CType;

	//UINT64 MF_ALL=0;
	for (size_t j = 0; j < StructInfo.MemberList.size(); j++)
	{
		CString MemberName;
		MemberName = ClassNameToUpper(StructInfo.MemberList[j].Name);
		EnumInfo.MemberList[j].Name.Format("MF_%s", MemberName);
		EnumInfo.MemberList[j].Value.Format("%d", j);
		EnumInfo.MemberList[j].Flag = ENUM_MEMBER_FLAG_IS_BIT_MASK;
		//MF_ALL=(MF_ALL<<1)|1;
	}
	//EnumInfo.MemberList[StructInfo.MemberList.size()].Name="MF_ALL";
	//EnumInfo.MemberList[StructInfo.MemberList.size()].Value.Format("0x%llX",MF_ALL);

	return MakeEnumDefine(EnumInfo, SpaceName, szLineSpace);
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
			CString Temp = m_DataObjectGetModifyFlagDefineTemplate;

			CString VarName = m_InterfaceConfig.MemberVariablePrefix + "m_" + StructInfo.MemberList[j].Name;			
			Temp.Replace("<Variable>", VarName);

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
			CString Temp = m_DataObjectIsModifiedDefineTemplate;

			CString VarName = m_InterfaceConfig.MemberVariablePrefix + "m_" + StructInfo.MemberList[j].Name;
			Temp.Replace("<Variable>", VarName);

			Source+=Temp;
		}		
	}

	//if(Source.IsEmpty())
	//	Source="||false";

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
			CString Temp = m_DataObjectClearModifyFlagDefineTemplate;

			CString VarName = m_InterfaceConfig.MemberVariablePrefix + "m_" + StructInfo.MemberList[j].Name;
			Temp.Replace("<Variable>", VarName);

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
			if (StructInfo.MemberList[j].IsArray)
			{
				Method = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.SetMethodDeclare, pTypeInfo);
			}

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

CString CCallInterfaceMakerDlg::MakeDataObjectSetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Methods;

	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			CString Method = pTypeInfo->GenerateOperations.SetMethodDefine;
			if (MemberInfo.IsArray)
			{
				Method = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.SetMethodDefine, pTypeInfo);
				CString PackFlag;
				if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
					PackFlag.Format("DOMF_%s_FULL", ClassNameToUpper(pTypeInfo->Name));
				CString LineSpace = GetLineSpace(Method, "<CloneOperation>");
				CString CloneOperation = MakeCloneOperation(MemberInfo.Name, pTypeInfo, true, true, false,
					(MemberInfo.Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0, PackFlag, "", "Value", StructInfo.Name, SpaceName, LineSpace);
				Method.Replace("<CloneOperation>", CloneOperation);
			}

			if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)
			{
				RemoveBlock(Method, "<IfUpdateMF>", "</IfUpdateMF>");
			}
			else
			{
				RetainBlock(Method, "<IfUpdateMF>", "</IfUpdateMF>");
			}

			if (pTypeInfo->Flag & (TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
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

			if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
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

			Method.Replace("<Type>", pTypeInfo->CType);
			Method.Replace("<VariableName>", MemberInfo.Name);

			CString VarName = m_InterfaceConfig.MemberVariablePrefix + "m_" + MemberInfo.Name;
			Method.Replace("<Variable>", VarName);
			Method.Replace("<ClassName>", StructInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(StructInfo.Name));
			Method.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

			CString ModifyFlag = "MF_" + ClassNameToUpper(MemberInfo.Name);
			Method.Replace("<ModifyFlag>", ModifyFlag);

			Method.Replace("<SpaceName>", SpaceName);



			Methods += Method + "\r\n";
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
			if (StructInfo.MemberList[j].IsArray)
			{
				Method = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.GetMethodDeclare, pTypeInfo);
			}

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

CString CCallInterfaceMakerDlg::MakeDataObjectGetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Methods;

	for(size_t j=0;j<StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE * pTypeInfo=FindVarType(StructInfo.MemberList[j].Type);
		if(pTypeInfo)
		{
			CString Method=pTypeInfo->GenerateOperations.GetMethodDefine;
			if (StructInfo.MemberList[j].IsArray)
			{
				Method = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.GetMethodDefine, pTypeInfo);
			}

			Method.Replace("<Type>",pTypeInfo->CType);
			Method.Replace("<VariableName>",StructInfo.MemberList[j].Name);
			CString VarName = m_InterfaceConfig.MemberVariablePrefix + "m_" + StructInfo.MemberList[j].Name;
			Method.Replace("<Variable>", VarName);
			Method.Replace("<ClassName>",StructInfo.Name);
			Method.Replace("<SpaceName>", SpaceName);

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

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagsHeader(vector<BASE_DATA_STRUCT_DEFINE_LIST*>& AllDataStructList, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Source;

	vector<STRUCT_DEFINE_INFO*> DataObjectList;

	GetDataObjectListSorted(AllDataStructList, DataObjectList);


	UINT ModifyFlagDefineSize = 0;

	for (size_t i = 0; i < AllDataStructList.size(); i++)
	{
		if (AllDataStructList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST* pStructList = (STRUCT_DEFINE_LIST*)AllDataStructList[i];
			for (size_t j = 0; j < pStructList->StructList.size(); j++)
			{
				if (pStructList->StructList[j].Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				{
					CString Define = m_DataObjectModifyFlagHeaderDefineTemple;

					CString Name;
					Name.Format("DOMF_%s_FULL", ClassNameToUpper(pStructList->StructList[j].Name));
					Define.Replace("<FlagName>", Name);

					Source += Define + "\r\n";
				}
			}
		}
	}

	for (size_t i = 0; i < DataObjectModifyFlags.size(); i++)
	{

		CString Define = m_DataObjectModifyFlagHeaderDefineTemple;

		Define.Replace("<FlagName>", DataObjectModifyFlags[i].Name);
		Source += Define + "\r\n";

	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Source.Replace("\r\n", Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagsSource(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& AllDataStructList, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags, LPCTSTR SpaceName, LPCTSTR szLineSpace)
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
					Define.Replace("<SpaceName>", SpaceName);

					CString Space=GetLineSpace(Define,"<FlagData>");
					CString FlagData = MakeDataObjectModifyFlagData(ModifyFlag, SpaceName, Space);
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
		Define.Replace("<SpaceName>", SpaceName);

		CString Space=GetLineSpace(Define,"<FlagData>");
		CString FlagData = MakeDataObjectModifyFlagData(DataObjectModifyFlags[i], SpaceName, Space);
		Define.Replace("<FlagData>",FlagData);

		Source+=Define+"\r\n";
		
	}
	
	CString Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagData(DATA_OBJECT_MODIFY_FLAG& ModifyFlag, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Source;
	CString Template = m_DataObjectModifyFlagUnitTemple;
	CString ObjSeparatorStr = GetBlock(Template, "<ObjSeparator>", "</ObjSeparator>");
	RemoveBlock(Template, "<ObjSeparator>", "</ObjSeparator>");
	CString FlagSeparatorStr = GetBlock(Template, "<FlagSeparator>", "</FlagSeparator>");
	RemoveBlock(Template, "<FlagSeparator>", "</FlagSeparator>");
	CString ZeroValue = GetBlock(Template, "<ZeroValue>", "</ZeroValue>");
	RemoveBlock(Template, "<ZeroValue>", "</ZeroValue>");
	CString ObjTemplate = GetBlock(Template, "<FlagUnit>", "</FlagUnit>");
	ReplaceBlock(Template, "<FlagUnit>", "</FlagUnit>", "<ObjFlag>");
	for (size_t i = 0; i < ModifyFlag.ModifyFlag.size(); i++)
	{
		CString Member;
		for (size_t j = 0; j < ModifyFlag.ModifyFlag[i].Members.size(); j++)
		{

			CString FlagUnit = ObjTemplate;

			FlagUnit.Replace("<SpaceName>", SpaceName);
			FlagUnit.Replace("<ClassName>", ModifyFlag.ModifyFlag[i].ClassName);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format("%s_MODIFY_FLAGS", ClassNameToUpper(ModifyFlag.ModifyFlag[i].ClassName));
			FlagUnit.Replace("<ModifyFlagEnumName>", ModifyFlagEnumName);

			CString Flag;
			Flag.Format("MF_%s", ClassNameToUpper(ModifyFlag.ModifyFlag[i].Members[j]));
			FlagUnit.Replace("<ModifyFlag>", Flag);
			if (j)
				Member += FlagSeparatorStr;
			Member += FlagUnit;
		}		
		if (i)
		{
			Source += ObjSeparatorStr + "\r\n";
		}
		if (Member.IsEmpty())
		{
			Source += ZeroValue;
		}
		else			
		{
			CString Line = Template;
			Line.Replace("<ObjFlag>", Member);
			Source += Line;
		}		
	}

	CString Space = "\r\n";
	Space += szLineSpace;
	Source.Replace("\r\n", Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeEnumDefine(ENUM_DEFINE_INFO& EnumInfo, LPCTSTR SpaceName,LPCTSTR szLineSpace)
{
	CString Source;

	if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
		Source = m_EnumDefine64Template;
	else
		Source = m_EnumDefineTemplate;
	Source.Replace("<EnumName>",EnumInfo.Name);
	Source.Replace("<SpaceName>", SpaceName);
	
	if (EnumInfo.Description.IsEmpty())
		Source.Replace("<Description>", "");
	else
		Source.Replace("<Description>", m_InterfaceConfig.CommentPrefix + EnumInfo.Description);
	
	if (EnumInfo.MemberList.size())
	{
		Source.Replace("<LastEnumValue>", EnumInfo.MemberList[EnumInfo.MemberList.size() - 1].Name);
	}

	CString MemberCount;

	MemberCount.Format("%d", GetEnumMemberCount(EnumInfo));

	Source.Replace("<EnumMemberCount>", MemberCount);

	if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_FLAG)
	{
		SelectBlock(Source, "IsFlagEnum", true);
	}
	else
	{
		SelectBlock(Source, "IsFlagEnum", false);
	}
	

	CString Space=GetLineSpace(Source,"<EnumMembers>");	

	CString EnumMembers;

	int Value = 0;
	for(size_t i=0;i<EnumInfo.MemberList.size();i++)
	{
		
		CString Member;
		if (EnumInfo.MemberList[i].Flag & ENUM_MEMBER_FLAG_IS_BIT_MASK)
		{
			if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
				Member = m_EnumBitMaskMemberDefine64Template;
			else
				Member = m_EnumBitMaskMemberDefineTemplate;
		}
		else
		{
			if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
				Member = m_EnumMemberDefine64Template;
			else
				Member = m_EnumMemberDefineTemplate;
		}

		Member.Replace("<Name>",EnumInfo.MemberList[i].Name);
		Member.Replace("<SpaceName>", SpaceName);
		Member.Replace("<EnumName>", EnumInfo.Name);


		CString EnumValue = EnumInfo.MemberList[i].Value;

		if (!EnumValue.IsEmpty())
			Value = atoi(EnumValue);
		
		

		if (EnumValue.IsEmpty())
		{
			RemoveBlock(EnumValue, "<IfHaveValue>", "</IfHaveValue>");
		}			
		else
		{
			EnumValue.Replace("<IfHaveValue>", "");
			EnumValue.Replace("</IfHaveValue>", "");
		}			

		//Member.Replace("<Value>", EnumValue);

		if (EnumValue.IsEmpty())
		{
			EnumValue.Format("%d", Value);
			Member.Replace("<Value>", EnumValue);
		}			
		else
		{
			Member.Replace("<Value>", EnumValue);
		}			
		Value++;

		CString Desc = EnumInfo.MemberList[i].Description;
		if (!EnumInfo.MemberList[i].BindDataType.IsEmpty())
			Desc += "，参考" + EnumInfo.MemberList[i].BindDataType;


		if (Desc.IsEmpty())
			Member.Replace("<Description>", "");
		else
			Member.Replace("<Description>", m_InterfaceConfig.CommentPrefix + Desc);
			

		if(i)
			EnumMembers+=Space;
		EnumMembers+=Member+"\r\n";		
	}	

	Source.Replace("<EnumMembers>",EnumMembers);

	if (EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_ENUM_LIST)
	{
		RetainBlock(Source, "<IfExportEnumList>", "</IfExportEnumList>");
		CString Operatios;

		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		{
			if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			{
				CString Operation = m_EnumListUnitTemplate;
				if (!Operatios.IsEmpty())
				{
					RetainBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
				}
				else
				{
					RemoveBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
				}
				if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
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
				Operation.Replace("<EnumName>", EnumInfo.Name);
				Operation.Replace("<EnumValue>", Info.Name);
				Operatios.Append(Operation);
			}
		}

		Source.Replace("<EnumValues>", Operatios);
	}
	else
	{
		RemoveBlock(Source, "<IfExportEnumList>", "</IfExportEnumList>");
	}

	if ((EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_STR_VALUE) && IsNaturalEnum(EnumInfo))
	{
		RetainBlock(Source, "<IfExportEnumStrList>", "</IfExportEnumStrList>");
		CString Operatios;

		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		{
			if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			{
				CString Operation = m_EnumStrValueUnitTemplate;
				if (!Operatios.IsEmpty())
				{
					RetainBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
				}
				else
				{
					RemoveBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
				}
				if (Info.StrValue.IsEmpty())
					Operation.Replace("<EnumStrValue>", MakeStringConst(Info.Name));
				else
					Operation.Replace("<EnumStrValue>", MakeStringConst(Info.StrValue));
				Operatios.Append(Operation);
			}
		}

		Source.Replace("<EnumStrValues>", Operatios);

		Operatios.Empty();
		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		{
			if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			{
				CString Operation = m_EnumStrValueUnitTemplate;
				if (!Operatios.IsEmpty())
				{
					RetainBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
				}
				else
				{
					RemoveBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
				}
				if (Info.StrValue.IsEmpty())
					Operation.Replace("<EnumStrValue>", MakeStringConst(Info.Name));
				else
					Operation.Replace("<EnumStrValue>", MakeStringConst(Info.Description));
				Operatios.Append(Operation);
			}
		}

		Source.Replace("<EnumStrDescs>", Operatios);
	}
	else
	{
		RemoveBlock(Source, "<IfExportEnumStrList>", "</IfExportEnumStrList>");
	}
	
	if(EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_STR_TRANS_FN)
	{
		RetainBlock(Source, "<IfExportEnumTransFn>", "</IfExportEnumTransFn>");

		CString Operations;
		CString LineSpace;

		if ((EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_STR_VALUE) && IsNaturalEnum(EnumInfo))
		{
			RemoveBlock(Source, "<NotHaveEnumStrArray>", "</NotHaveEnumStrArray>");
			RetainBlock(Source, "<HaveEnumStrArray>", "</HaveEnumStrArray>");
		}
		else
		{
			RemoveBlock(Source, "<HaveEnumStrArray>", "</HaveEnumStrArray>");
			RetainBlock(Source, "<NotHaveEnumStrArray>", "</NotHaveEnumStrArray>");

			Operations.Empty();
			LineSpace = GetLineSpace(Source, "<EnumToStrs>");
			for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
			{
				if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
				{
					CString Operation = m_EnumToStrUnitTemplate;
					if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_FLAG)
					{
						SelectBlock(Operation, "IsFlagEnum", true);
					}
					else
					{
						SelectBlock(Operation, "IsFlagEnum", false);
					}
					if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
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
					Operation.Replace("<EnumName>", EnumInfo.Name);
					Operation.Replace("<EnumValue>", Info.Name);
					if (Info.StrValue.IsEmpty())
						Operation.Replace("<EnumStrValue>", MakeStringConst(Info.Name));
					else
						Operation.Replace("<EnumStrValue>", MakeStringConst(Info.StrValue));
					if (!Operations.IsEmpty())
						Operations.Append("\r\n");
					Operations.Append(Operation);
				}
			}
			Operations.Replace("\r\n", "\r\n" + LineSpace);
			Source.Replace("<EnumToStrs>", Operations);

			Operations.Empty();
			LineSpace = GetLineSpace(Source, "<EnumToDescs>");
			for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
			{
				if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
				{
					CString Operation = m_EnumToStrUnitTemplate;
					if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_FLAG)
					{
						SelectBlock(Operation, "IsFlagEnum", true);
					}
					else
					{
						SelectBlock(Operation, "IsFlagEnum", false);
					}
					if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
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
					Operation.Replace("<EnumName>", EnumInfo.Name);
					Operation.Replace("<EnumValue>", Info.Name);
					if (Info.StrValue.IsEmpty())
						Operation.Replace("<EnumStrValue>", MakeStringConst(Info.Name));
					else
						Operation.Replace("<EnumStrValue>", MakeStringConst(Info.Description));
					if (!Operations.IsEmpty())
						Operations.Append("\r\n");
					Operations.Append(Operation);
				}
			}
			Operations.Replace("\r\n", "\r\n" + LineSpace);
			Source.Replace("<EnumToDescs>", Operations);
		}

		Operations.Empty();
		LineSpace = GetLineSpace(Source, "<StrToEnums>");
		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		{
			if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			{
				CString Operation = m_StrToEnumUnitTemplate;
				if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_FLAG)
				{
					SelectBlock(Operation, "IsFlagEnum", true);
				}
				else
				{
					SelectBlock(Operation, "IsFlagEnum", false);
				}
				if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
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
				Operation.Replace("<EnumName>", EnumInfo.Name);
				Operation.Replace("<EnumValue>", Info.Name);
				if (Info.StrValue.IsEmpty())
					Operation.Replace("<EnumStrValue>", MakeStringConst(Info.Name));
				else
					Operation.Replace("<EnumStrValue>", MakeStringConst(Info.StrValue));
				if (!Operations.IsEmpty())
				{
					RetainBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
					Operations.Append("\r\n");
				}
				else
				{
					RemoveBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
				}

				Operations.Append(Operation);
			}
		}
		Operations.Replace("\r\n", "\r\n" + LineSpace);
		Source.Replace("<StrToEnums>", Operations);
	}
	else
	{
		RemoveBlock(Source, "<IfExportEnumTransFn>", "</IfExportEnumTransFn>");
	}

	if ((EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_BIND_DATA_PROCESS))
	{
		RetainBlock(Source, "<IfExportBindDataProcess>", "</IfExportBindDataProcess>");
		
		CString LineSpace;
		CString Operations;
		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		{
			if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			{
				if (!Info.BindDataType.IsEmpty())
				{
					CString Operation = m_EnumBindDataFillUnit;
					Operation.Replace("<EnumName>", EnumInfo.Name);
					Operation.Replace("<EnumValue>", Info.Name);
					STRUCT_DEFINE_INFO* pStruct = GetStructDefineInfo(Info.BindDataType);
					if (pStruct)
					{
						SelectBlock(Operation, "HaveBindStruct", true);
						CString Temp;
						Temp.Format("%u", pStruct->MemberList.size());
						Operation.Replace("<BindDataLen>", Temp);
						LineSpace = GetLineSpace(Operation, "<PropertyGridFillOperations>");
						Temp = MakePropertyGridFillOperations(*pStruct, true, LineSpace);
						Operation.Replace("<PropertyGridFillOperations>", Temp);						
					}
					else
					{
						SelectBlock(Operation, "HaveBindStruct", false);
					}
					if (!Operations.IsEmpty())
						Operations.Append("\r\n");
					Operations.Append(Operation);
				}				
			}
		}
		LineSpace = GetLineSpace(Source, "<BindDataFillOperations>");
		Operations.Replace("\r\n", "\r\n" + LineSpace);
		Source.Replace("<BindDataFillOperations>", Operations);

		//Operations.Empty();
		//for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		//{
		//	if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
		//	{
		//		if (!Info.BindDataType.IsEmpty())
		//		{
		//			CString Operation = m_EnumBindDataFetchUnit;
		//			Operation.Replace("<EnumName>", EnumInfo.Name);
		//			Operation.Replace("<EnumValue>", Info.Name);
		//			STRUCT_DEFINE_INFO* pStruct = GetStructDefineInfo(Info.BindDataType);
		//			if (pStruct)
		//			{
		//				SelectBlock(Operation, "HaveBindStruct", true);
		//				CString Temp;
		//				Temp.Format("%u", pStruct->MemberList.size());
		//				Operation.Replace("<BindDataLen>", Temp);
		//				LineSpace = GetLineSpace(Operation, "<PropertyGridFetchOperations>");
		//				Temp = MakePropertyGridFetchOperations(*pStruct, true, LineSpace);
		//				Operation.Replace("<PropertyGridFetchOperations>", Temp);
		//			}
		//			else
		//			{
		//				SelectBlock(Operation, "HaveBindStruct", false);
		//			}
		//			if (!Operations.IsEmpty())
		//				Operations.Append("\r\n");
		//			Operations.Append(Operation);
		//		}
		//	}
		//}
		//LineSpace = GetLineSpace(Source, "<BindDataFetchOperations>");
		//Operations.Replace("\r\n", "\r\n" + LineSpace);
		//Source.Replace("<BindDataFetchOperations>", Operations);
	}
	else
	{
		RemoveBlock(Source, "<IfExportBindDataProcess>", "</IfExportBindDataProcess>");
	}

	Space="\r\n";
	Space+=szLineSpace;
	Source.Replace("\r\n",Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeEnumDefines(vector<ENUM_DEFINE_INFO>& EnumDefineList, LPCTSTR SpaceName,LPCTSTR szLineSpace)
{
	CString EnumDefines;
	for (size_t i = 0; i < EnumDefineList.size(); i++)
	{
		CString Enum = MakeEnumDefine(EnumDefineList[i], SpaceName, "");

		EnumDefines += Enum + "\r\n\r\n";

	}
	CString Space="\r\n";
	Space+=szLineSpace;
	EnumDefines.Replace("\r\n",Space);
	return EnumDefines;
}

//CString CCallInterfaceMakerDlg::MakeEnumStrValue(ENUM_DEFINE_INFO& EnumInfo, LPCTSTR SpaceName,LPCTSTR szLineSpace)
//{
//	CString Source=m_EnumStrValueDefineTemplate;
//
//	Source.Replace("<EnumName>",EnumInfo.Name);
//	Source.Replace("<SpaceName>", SpaceName);
//
//	CString MemberCount;
//
//	MemberCount.Format("%d",EnumInfo.MemberList.size());
//
//	Source.Replace("<EnumMemberCount>",MemberCount);
//
//
//	CString StrValues;
//
//	for(size_t i=0;i<EnumInfo.MemberList.size();i++)
//	{
//		if(i)
//			StrValues+=",";
//		if (EnumInfo.MemberList[i].StrValue.IsEmpty())
//			StrValues += MakeStringConst(EnumInfo.MemberList[i].Name);
//		else
//			StrValues += MakeStringConst(EnumInfo.MemberList[i].StrValue);
//	}	
//
//	Source.Replace("<EnumStrValues>",StrValues);
//
//	CString Space="\r\n";
//	Space+=szLineSpace;
//	Source.Replace("\r\n",Space);
//	return Source;
//}

CString CCallInterfaceMakerDlg::MakeConstDefines(vector<CONST_DEFINE_INFO>& ConstDefineList, LPCTSTR SpaceName,LPCTSTR szLineSpace)
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
			Const.Replace("<SpaceName>", SpaceName);
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
		if (Value.GetLength() && Value[0] == '"' && Value[Value.GetLength() - 1] == '"')
		{
			Value.Trim('"');
			Value = MakeStringConst(Value);
		}			
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

CString CCallInterfaceMakerDlg::MakeToStringExp(STRUCT_MEMBER_INFO& MemberInfo, TYPE_DEFINE* pType, bool IsInDataObject)
{
	CString ToStringOP = pType->GenerateOperations.ToStringOperation;	
	SelectBlock(ToStringOP, "HaveStrTrans", pType->Flag & TYPE_DEFINE_FLAG_ENUM_HAVE_STR_TRANS);
	SelectBlock(ToStringOP, "HaveExtendType", !MemberInfo.ExtendType.IsEmpty());
	CString VarName = MemberInfo.Name;
	if (IsInDataObject)
		VarName = "m_" + VarName;
	VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
	ToStringOP.Replace("<Variable>", VarName);
	ToStringOP.Replace("<Type>", pType->CType);
	ToStringOP.Replace("<ExtendType>", MemberInfo.ExtendType);
	return ToStringOP;
}

//CString CCallInterfaceMakerDlg::MakeEnumStrValues(vector<ENUM_DEFINE_INFO>& EnumDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
//{
//	CString EnumDefines;
//	for(size_t i=0;i<EnumDefineList.size();i++)
//	{		
//		if ((EnumDefineList[i].Flag & ENUM_DEFINE_FLAG_EXPORT_STR_VALUE) && IsNaturalEnum(EnumDefineList[i]))
//		{
//			CString StrValues = MakeEnumStrValue(EnumDefineList[i], SpaceName, "");
//
//			EnumDefines += StrValues + "\r\n\r\n";
//		}
//	}
//	CString Space="\r\n";
//	Space+=szLineSpace;
//	EnumDefines.Replace("\r\n",Space);
//	return EnumDefines;
//}

//CString CCallInterfaceMakerDlg::MakeEnumStrTransFn(ENUM_DEFINE_INFO& EnumInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
//{
//	CString Source = m_EnumStrTransFnTemplate;
//
//	Source.Replace("<EnumName>", EnumInfo.Name);
//	Source.Replace("<SpaceName>", SpaceName);
//
//	if (EnumInfo.MemberList.size())
//	{
//		Source.Replace("<LastEnumValue>", EnumInfo.MemberList[EnumInfo.MemberList.size() - 1].Name);
//	}
//
//	CString MemberCount;
//
//	MemberCount.Format("%d", EnumInfo.MemberList.size());
//
//	Source.Replace("<EnumMemberCount>", MemberCount);
//
//	CString Operatios;
//	CString LineSpace;
//
//	if ((EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_STR_VALUE) && IsNaturalEnum(EnumInfo))
//	{
//		RemoveBlock(Source, "<NotHaveEnumStrArray>", "</NotHaveEnumStrArray>");
//		RetainBlock(Source, "<HaveEnumStrArray>", "</HaveEnumStrArray>");
//	}
//	else
//	{
//		RemoveBlock(Source, "<HaveEnumStrArray>", "</HaveEnumStrArray>");
//		RetainBlock(Source, "<NotHaveEnumStrArray>", "</NotHaveEnumStrArray>");
//
//		Operatios.Empty();
//		LineSpace = GetLineSpace(Source, "<EnumToStrs>");
//		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
//		{
//			CString Operation = m_EnumToStrUnitTemplate;
//			Operation.Replace("<EnumName>", EnumInfo.Name);
//			Operation.Replace("<EnumValue>", Info.Name);
//			if (Info.StrValue.IsEmpty())
//				Operation.Replace("<EnumStrValue>", MakeStringConst(Info.Name));
//			else
//				Operation.Replace("<EnumStrValue>", MakeStringConst(Info.StrValue));
//			if (!Operatios.IsEmpty())
//				Operatios.Append("\r\n");
//			Operatios.Append(Operation);
//		}
//		Operatios.Replace("\r\n", "\r\n" + LineSpace);
//		Source.Replace("<EnumToStrs>", Operatios);
//	}
//
//	Operatios.Empty();
//	LineSpace = GetLineSpace(Source, "<StrToEnums>");
//	for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
//	{
//		CString Operation = m_StrToEnumUnitTemplate;
//		Operation.Replace("<EnumName>", EnumInfo.Name);
//		Operation.Replace("<EnumValue>", Info.Name);
//		if (Info.StrValue.IsEmpty())
//			Operation.Replace("<EnumStrValue>", MakeStringConst(Info.Name));
//		else
//			Operation.Replace("<EnumStrValue>", MakeStringConst(Info.StrValue));
//		if (!Operatios.IsEmpty())
//		{
//			RetainBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
//			Operatios.Append("\r\n");
//		}			
//		else
//		{
//			RemoveBlock(Operation, "<IfNotFirst>", "</IfNotFirst>");
//		}
//			
//		Operatios.Append(Operation);
//	}
//	Operatios.Replace("\r\n", "\r\n" + LineSpace);
//	Source.Replace("<StrToEnums>", Operatios);
//
//	CString Space = "\r\n";
//	Space += szLineSpace;
//	Source.Replace("\r\n", Space);
//	return Source;
//}
//CString CCallInterfaceMakerDlg::MakeEnumStrTransFns(vector<ENUM_DEFINE_INFO>& EnumDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
//{
//	CString EnumDefines;
//	for (size_t i = 0; i < EnumDefineList.size(); i++)
//	{
//		if (EnumDefineList[i].Flag & ENUM_DEFINE_FLAG_EXPORT_STR_TRANS_FN)
//		{
//			CString StrValues = MakeEnumStrTransFn(EnumDefineList[i], SpaceName, "");
//
//			EnumDefines += StrValues;
//		}
//	}
//	CString Space = "\r\n";
//	Space += szLineSpace;
//	EnumDefines.Replace("\r\n", Space);
//	return EnumDefines;
//}

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

CString CCallInterfaceMakerDlg::MakeStructEditorProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Source = m_StructEditorProcessTemplate;

	Source.Replace("<ClassName>", StructInfo.Name);

	if (StructInfo.BaseStruct.IsEmpty())
	{
		RemoveBlock(Source, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
		RetainBlock(Source, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
	}
	else
	{
		RemoveBlock(Source, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
		RetainBlock(Source, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
		Source.Replace("<BaseClass>", StructInfo.BaseStruct);
	}

	CString SSTIDEnumName;
	SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructInfo.Name));

	CString Operations;
	CString LineSpace = GetLineSpace(Source, "<CreateListHeaderOperations>");
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if ((!MemberInfo.ShowName.IsEmpty()) && (!MemberInfo.IsArray) && ((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_EDITOR_LIST) == 0))
		{
			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
			if (pType && ((pType->Flag & TYPE_DEFINE_FLAG_STRUCT) == 0))
			{
				CString Operation = m_CreateListHeaderUnitTemplate;
				Operation.Replace("<VarShowName>", MakeStringConst(MemberInfo.ShowName));
				if (!Operations.IsEmpty())
					Operations.Append("\r\n");
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace("\r\n", "\r\n" + LineSpace);
	Source.Replace("<CreateListHeaderOperations>", Operations);

	Operations.Empty();
	LineSpace = GetLineSpace(Source, "<FillListItemOperations>");
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{		
		if ((!MemberInfo.ShowName.IsEmpty()) && (!MemberInfo.IsArray) && ((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_EDITOR_LIST) == 0))
		{
			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
			if (pType && ((pType->Flag & TYPE_DEFINE_FLAG_STRUCT) == 0))
			{
				CString Operation = m_FillListItemUnitTemplate;
				
				Operation.Replace("<VarStrValue>", MakeToStringExp(MemberInfo, pType, false));
				if (!Operations.IsEmpty())
					Operations.Append("\r\n");
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace("\r\n", "\r\n" + LineSpace);
	Source.Replace("<FillListItemOperations>", Operations);

	CString Temp;
	Temp.Format("%u", StructInfo.MemberList.size());
	Source.Replace("<BindDataLen>", Temp);

	LineSpace = GetLineSpace(Source, "<PropertyGridFillOperations>");
	Operations = MakePropertyGridFillOperations(StructInfo, false, LineSpace);
	Source.Replace("<PropertyGridFillOperations>", Operations);

	LineSpace = GetLineSpace(Source, "<PropertyGridBindDataFillOperations>");
	Operations = MakePropertyGridFillOperations(StructInfo, true, LineSpace);
	Source.Replace("<PropertyGridBindDataFillOperations>", Operations);

	//LineSpace = GetLineSpace(Source, "<PropertyGridFetchOperations>");
	//Operations = MakePropertyGridFetchOperations(StructInfo, false, LineSpace);
	//Source.Replace("<PropertyGridFetchOperations>", Operations);

	//LineSpace = GetLineSpace(Source, "<PropertyGridAddItemOperations>");
	//Operations = MakePropertyGridAddItemOperations(StructInfo, false, LineSpace);
	//Source.Replace("<PropertyGridAddItemOperations>", Operations);

	//LineSpace = GetLineSpace(Source, "<PropertyGridDelItemOperations>");
	//Operations = MakePropertyGridDelItemOperations(StructInfo, false, LineSpace);
	//Source.Replace("<PropertyGridDelItemOperations>", Operations);

	//LineSpace = GetLineSpace(Source, "<PropertyGridMoveItemOperations>");
	//Operations = MakePropertyGridMoveItemOperations(StructInfo, false, LineSpace);
	//Source.Replace("<PropertyGridMoveItemOperations>", Operations);

	CString Space = "\r\n";
	Space += szLineSpace;
	Source.Replace("\r\n", Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakePropertyGridFillOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
{
	CString Operations;
	CString SSTIDEnumName;
	SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructInfo.Name));
	
	UINT Index = 0;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_PROPERTY_GRID) == 0) &&
			((!MemberInfo.IsArray) || (IsBeBind(StructInfo, MemberInfo.Name) == NULL)))
		{
			CString Operation = MakePropertyGridFillOperation(StructInfo, MemberInfo, Index, SSTIDEnumName, ReplaceByBindData, "");
			Operations.Append(Operation);
			Operations.Append("\r\n");
		}
		Index++;
	}
	CString LineSpace;
	LineSpace.Format("\r\n%s", szLineSpace);
	Operations.Replace("\r\n", LineSpace);
	return Operations;
}

CString CCallInterfaceMakerDlg::MakePropertyGridFillOperation(STRUCT_DEFINE_INFO& StructInfo, STRUCT_MEMBER_INFO& MemberInfo, UINT MemberIndex, LPCTSTR SSTIDEnumName, bool ReplaceByBindData, LPCTSTR szLineSpace)
{	
	CString Operation;
	TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
	if (pType)
	{		
		if (MemberInfo.IsArray)
		{
			Operation = m_InterfaceConfig.ArrayDefineConfig.PropertyGridFillOperation;			
			CString SubOperation = pType->GenerateOperations.PropertyGridFillOperation;
			CString LineSpace = GetLineSpace(Operation, "<PropertyGridFillOperation>");
			SubOperation.Replace("\r\n", "\r\n" + LineSpace);
			RemoveBlock(SubOperation, "<IfNotInArray>", "</IfNotInArray>");
			RetainBlock(SubOperation, "<IfInArray>", "</IfInArray>");
			CString Var = m_InterfaceConfig.ArrayDefineConfig.IndexOperation;			
			Var.Replace("<Variable>", MemberInfo.Name);
			Var.Replace("<Index>", "i");
			SubOperation.Replace("<Variable>", Var);
			SubOperation.Replace("<Index>", "i");
			Operation.Replace("<PropertyGridFillOperation>", SubOperation);
		}
		else
		{
			Operation = pType->GenerateOperations.PropertyGridFillOperation;
			RemoveBlock(Operation, "<IfInArray>", "</IfInArray>");
			RetainBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
		}
		if (!Operation.IsEmpty())
		{
			if (pType->Flag & TYPE_DEFINE_FLAG_ENUM)
			{
				ENUM_DEFINE_INFO* pEnum = GetEnumDefine(pType->CType);
				if (pEnum)
				{
					CString MemberCount;
					MemberCount.Format("%d", GetEnumMemberCount(*pEnum));
					Operation.Replace("<EnumMemberCount>", MemberCount);
					if (pEnum->Flag & ENUM_DEFINE_FLAG_IS_FLAG)
					{
						SelectBlock(Operation, "IsFlagEnum", true);
					}
					else
					{
						SelectBlock(Operation, "IsFlagEnum", false);
					}					
				}

				if (IsBeBind(StructInfo, MemberInfo.Name))
				{
					SelectBlock(Operation, "BeBind", true);
				}
				else
				{
					SelectBlock(Operation, "BeBind", false);
				}
			}

			if (!MemberInfo.BindData.IsEmpty())
			{
				SelectBlock(Operation, "HaveBindData", true);
				STRUCT_MEMBER_INFO* pBindData = NULL;
				for (STRUCT_MEMBER_INFO& Info : StructInfo.MemberList)
				{
					if (Info.Name == MemberInfo.BindData)
					{
						pBindData = &Info;
						break;
					}
				}
				if (pBindData)
				{
					TYPE_DEFINE* pBindDataType = FindVarType(pBindData->Type);
					if (pBindDataType)
						Operation.Replace("<BindParentType>", pBindDataType->CType);
				}						
			}
			else
			{
				SelectBlock(Operation, "HaveBindData", false);
			}
			
			Operation.Replace("<VarShowName>", MakeStringConst(MemberInfo.ShowName));
			Operation.Replace("<Description>", MakeStringConst(MemberInfo.Description));
			Operation.Replace("<VarStrValue>", MakeToStringExp(MemberInfo, pType, false));
			Operation.Replace("<Type>", pType->CType);

			SelectBlock(Operation, "HaveExtendType", !MemberInfo.ExtendType.IsEmpty());
			Operation.Replace("<ExtendType>", MemberInfo.ExtendType);

			if (ReplaceByBindData)
			{
				SelectBlock(Operation, "InBindData", true);								
			}
			else
			{
				SelectBlock(Operation, "InBindData", false);
				CString VarName = MemberInfo.Name;
				if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
					VarName = "m_" + VarName;
				VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
				Operation.Replace("<Variable>", VarName);
				Operation.Replace("<VariableName>", MemberInfo.Name);
				CString SST_ID;
				SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
				SST_ID.MakeUpper();
				Operation.Replace("<StructName>", StructInfo.Name);
				Operation.Replace("<SST_NAME>", SSTIDEnumName);
				Operation.Replace("<SST_ID>", SST_ID);
				Operation.Replace("<Index>", "-1");
			}
		}
	}
	CString LineSpace;
	LineSpace.Format("\r\n%s", szLineSpace);
	Operation.Replace("\r\n", LineSpace);
	return Operation;
}

//CString CCallInterfaceMakerDlg::MakePropertyGridFetchOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operations;
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructInfo.Name));
//
//	UINT Index = 0;
//	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
//	{			
//		CString Operation = m_PropertyGridFetchUnitTemplate;
//
//		if (MemberInfo.IsArray)
//		{
//			RemoveBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
//			RetainBlock(Operation, "<IfInArray>", "</IfInArray>");
//		}
//		else
//		{
//			RemoveBlock(Operation, "<IfInArray>", "</IfInArray>");
//			RetainBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
//		}
//
//		if (ReplaceByBindData)
//		{
//			SelectBlock(Operation, "InBindData", true);			
//		}
//		else
//		{
//			SelectBlock(Operation, "InBindData", false);
//			Operation.Replace("<Variable>", MemberInfo.Name);
//		}
//		
//		STRUCT_MEMBER_INFO* pBindParent = NULL;
//		if (MemberInfo.IsArray)
//		{
//			pBindParent = IsBeBind(StructInfo, MemberInfo.Name);
//			if (pBindParent)
//			{
//				SelectBlock(Operation, "IsBindData", true);
//				Operation.Replace("<BindEnumName>", pBindParent->Type);
//				Operation.Replace("<BindEnumVariable>", pBindParent->Name);
//			}
//			else
//			{
//				SelectBlock(Operation, "IsBindData", false);
//			}
//		}
//		else
//		{
//			SelectBlock(Operation, "IsBindData", false);
//		}
//
//		if (pBindParent == NULL)
//		{
//			CString LineSpace = GetLineSpace(Operation, "<PropertyGridFetchOperation>");
//			CString FetchOperation = MakePropertyGridFetchOperation(StructInfo, MemberInfo, Index, ReplaceByBindData, LineSpace);
//			Operation.Replace("<PropertyGridFetchOperation>", FetchOperation);
//		}
//
//		CString SST_ID;
//		SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
//		SST_ID.MakeUpper();
//		Operation.Replace("<StructName>", StructInfo.Name);
//		Operation.Replace("<SST_NAME>", SSTIDEnumName);
//		Operation.Replace("<SST_ID>", SST_ID);
//
//
//		Operations.Append(Operation);
//		Operations.Append("\r\n");
//			
//
//		Index++;
//	}
//	CString LineSpace;
//	LineSpace.Format("\r\n%s", szLineSpace);
//	Operations.Replace("\r\n", LineSpace);
//	return Operations;
//}
//
//CString CCallInterfaceMakerDlg::MakePropertyGridFetchOperation(STRUCT_DEFINE_INFO& StructInfo, STRUCT_MEMBER_INFO& MemberInfo, UINT MemberIndex, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operation;
//	TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
//	if (pType)
//	{		
//		if (MemberInfo.IsArray)
//		{
//			Operation = m_InterfaceConfig.ArrayDefineConfig.PropertyGridFetchOperation;
//			if (ReplaceByBindData)
//				SelectBlock(Operation, "InBindData", true);
//			else
//				SelectBlock(Operation, "InBindData", false);
//			CString SubOperation = pType->GenerateOperations.PropertyGridFetchOperation;
//			CString LineSpace = GetLineSpace(Operation, "<PropertyGridFetchOperation>");
//			SubOperation.Replace("\r\n", "\r\n" + LineSpace);
//			RemoveBlock(SubOperation, "<IfNotInArray>", "</IfNotInArray>");
//			RetainBlock(SubOperation, "<IfInArray>", "</IfInArray>");
//			CString Var = m_InterfaceConfig.ArrayDefineConfig.IndexOperation;
//			Var.Replace("<Variable>", MemberInfo.Name);
//			Var.Replace("<Index>", "i");
//			SubOperation.Replace("<Variable>", Var);
//			SubOperation.Replace("<Index>", "i");
//			Operation.Replace("<PropertyGridFetchOperation>", SubOperation);
//		}
//		else
//		{
//			Operation = pType->GenerateOperations.PropertyGridFetchOperation;
//			RemoveBlock(Operation, "<IfInArray>", "</IfInArray>");
//			RetainBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
//		}
//		if (!Operation.IsEmpty())
//		{
//			if (pType->Flag & TYPE_DEFINE_FLAG_ENUM)
//			{
//				ENUM_DEFINE_INFO* pEnum = GetEnumDefine(pType->CType);
//				if (pEnum)
//				{
//					CString MemberCount;
//					MemberCount.Format("%d", GetEnumMemberCount(*pEnum));
//					Operation.Replace("<EnumMemberCount>", MemberCount);
//					if (pEnum->Flag & ENUM_DEFINE_FLAG_IS_FLAG)
//					{
//						SelectBlock(Operation, "IsFlagEnum", true);
//					}
//					else
//					{
//						SelectBlock(Operation, "IsFlagEnum", false);
//					}
//					if (!MemberInfo.BindData.IsEmpty())
//					{
//						SelectBlock(Operation, "HaveBindData", true);						
//						STRUCT_MEMBER_INFO* pBindData = NULL;
//						for (STRUCT_MEMBER_INFO& Info : StructInfo.MemberList)
//						{
//							if (Info.Name == MemberInfo.BindData)
//							{
//								pBindData = &Info;
//								break;
//							}
//						}
//						if (pBindData)
//						{
//							Operation.Replace("<BindData>", pBindData->Name);
//							TYPE_DEFINE* pBindDataType = FindVarType(pBindData->Type);
//							if (pBindDataType)
//								Operation.Replace("<BindDataType>", pBindDataType->CType);
//						}
//						else
//						{
//							Operation.Replace("<BindData>", "BindData");
//							Operation.Replace("<BindDataType>", "T");
//						}
//					}
//					else
//					{
//						SelectBlock(Operation, "HaveBindData", false);
//					}
//				}
//			}		
//			if (ReplaceByBindData)
//			{
//				SelectBlock(Operation, "InBindData", true);				
//			}
//			else
//			{
//				SelectBlock(Operation, "InBindData", false);				
//				Operation.Replace("<Variable>", MemberInfo.Name);
//			}
//			Operation.Replace("<Type>", pType->CType);			
//			Operation.Replace("<VarShowName>", MakeStringConst(MemberInfo.ShowName));
//			Operation.Replace("<Description>", MakeStringConst(MemberInfo.Description));
//			Operation.Replace("<VarStrValue>", MakeToStringExp(MemberInfo, pType));
//			Operation.Replace("<Index>", "-1");
//
//			
//		}
//	}
//	CString LineSpace;
//	LineSpace.Format("\r\n%s", szLineSpace);
//	Operation.Replace("\r\n", LineSpace);
//	return Operation;
//}
//
//CString CCallInterfaceMakerDlg::MakePropertyGridAddItemOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operations;
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructInfo.Name));
//
//	UINT Index = 0;
//	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
//	{
//		TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
//		if (pType && ((pType->Flag & TYPE_DEFINE_FLAG_STRUCT) || MemberInfo.IsArray))
//		{
//			CString Operation = m_PropertyGridAddItemUnitTemplate;
//			if (MemberInfo.IsArray)
//			{
//				RemoveBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
//				RetainBlock(Operation, "<IfInArray>", "</IfInArray>");
//			}
//			else
//			{
//				RemoveBlock(Operation, "<IfInArray>", "</IfInArray>");
//				RetainBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
//			}
//			if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
//			{
//				RetainBlock(Operation, "<IfIsStruct>", "</IfIsStruct>");
//			}
//			else
//			{
//				RemoveBlock(Operation, "<IfIsStruct>", "</IfIsStruct>");
//			}
//			Operation.Replace("<Type>", pType->CType);
//			Operation.Replace("<VarShowName>", MakeStringConst(MemberInfo.ShowName));
//			Operation.Replace("<Description>", MakeStringConst(MemberInfo.Description));
//			Operation.Replace("<VarStrValue>", MakeToStringExp(MemberInfo, pType));
//			Operation.Replace("<Index>", "-1");
//
//			if (ReplaceByBindData)
//			{
//				SelectBlock(Operation, "InBindData", true);				
//			}
//			else
//			{
//				SelectBlock(Operation, "InBindData", false);
//				Operation.Replace("<Variable>", MemberInfo.Name);
//				CString SST_ID;
//				SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
//				SST_ID.MakeUpper();
//				Operation.Replace("<StructName>", StructInfo.Name);
//				Operation.Replace("<SST_NAME>", SSTIDEnumName);
//				Operation.Replace("<SST_ID>", SST_ID);
//			}
//
//			CString LineSpace = GetLineSpace(Operation, "<PropertyGridRefreshOperation>");
//			CString RefreshOperation = MakePropertyGridFillOperation(StructInfo, MemberInfo, 0, SSTIDEnumName, ReplaceByBindData, LineSpace);
//			Operation.Replace("<PropertyGridRefreshOperation>", RefreshOperation);
//
//			Operations.Append(Operation);
//			Operations.Append("\r\n");
//		}
//		Index++;
//	}
//	CString LineSpace;
//	LineSpace.Format("\r\n%s", szLineSpace);
//	Operations.Replace("\r\n", LineSpace);
//	return Operations;
//}
//
//CString CCallInterfaceMakerDlg::MakePropertyGridDelItemOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operations;
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructInfo.Name));
//
//	UINT Index = 0;
//	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
//	{
//		TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
//		if (pType && ((pType->Flag & TYPE_DEFINE_FLAG_STRUCT) || MemberInfo.IsArray))
//		{
//			CString Operation = m_PropertyGridDelItemUnitTemplate;
//			if (MemberInfo.IsArray)
//			{
//				RemoveBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
//				RetainBlock(Operation, "<IfInArray>", "</IfInArray>");
//			}
//			else
//			{
//				RemoveBlock(Operation, "<IfInArray>", "</IfInArray>");
//				RetainBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
//			}
//			if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
//			{
//				RetainBlock(Operation, "<IfIsStruct>", "</IfIsStruct>");
//			}
//			else
//			{
//				RemoveBlock(Operation, "<IfIsStruct>", "</IfIsStruct>");
//			}
//			Operation.Replace("<Type>", pType->CType);
//			Operation.Replace("<VarShowName>", MakeStringConst(MemberInfo.ShowName));
//			Operation.Replace("<Description>", MakeStringConst(MemberInfo.Description));
//			Operation.Replace("<VarStrValue>", MakeToStringExp(MemberInfo, pType));
//			Operation.Replace("<Index>", "-1");
//
//			if (ReplaceByBindData)
//			{
//				SelectBlock(Operation, "InBindData", true);				
//			}
//			else
//			{
//				SelectBlock(Operation, "InBindData", false);
//				Operation.Replace("<Variable>", MemberInfo.Name);
//				CString SST_ID;
//				SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
//				SST_ID.MakeUpper();
//				Operation.Replace("<StructName>", StructInfo.Name);
//				Operation.Replace("<SST_NAME>", SSTIDEnumName);
//				Operation.Replace("<SST_ID>", SST_ID);
//			}
//
//			CString LineSpace = GetLineSpace(Operation, "<PropertyGridRefreshOperation>");
//			CString RefreshOperation = MakePropertyGridFillOperation(StructInfo, MemberInfo, 0, SSTIDEnumName, ReplaceByBindData, LineSpace);
//			Operation.Replace("<PropertyGridRefreshOperation>", RefreshOperation);
//
//			Operations.Append(Operation);
//			Operations.Append("\r\n");
//		}
//		Index++;
//	}
//	CString LineSpace;
//	LineSpace.Format("\r\n%s", szLineSpace);
//	Operations.Replace("\r\n", LineSpace);
//	return Operations;
//}
//
//CString CCallInterfaceMakerDlg::MakePropertyGridMoveItemOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operations;
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructInfo.Name));
//
//	UINT Index = 0;
//	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
//	{
//		TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
//		if (pType && ((pType->Flag & TYPE_DEFINE_FLAG_STRUCT) || MemberInfo.IsArray))
//		{
//			CString Operation = m_PropertyGridMoveItemUnitTemplate;
//			if (MemberInfo.IsArray)
//			{
//				RemoveBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
//				RetainBlock(Operation, "<IfInArray>", "</IfInArray>");
//			}
//			else
//			{
//				RemoveBlock(Operation, "<IfInArray>", "</IfInArray>");
//				RetainBlock(Operation, "<IfNotInArray>", "</IfNotInArray>");
//			}
//			if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
//			{
//				RetainBlock(Operation, "<IfIsStruct>", "</IfIsStruct>");
//			}
//			else
//			{
//				RemoveBlock(Operation, "<IfIsStruct>", "</IfIsStruct>");
//			}
//			Operation.Replace("<Type>", pType->CType);
//			Operation.Replace("<VarShowName>", MakeStringConst(MemberInfo.ShowName));
//			Operation.Replace("<Description>", MakeStringConst(MemberInfo.Description));
//			Operation.Replace("<VarStrValue>", MakeToStringExp(MemberInfo, pType));
//			Operation.Replace("<Index>", "-1");
//
//			if (ReplaceByBindData)
//			{
//				SelectBlock(Operation, "InBindData", true);				
//			}
//			else
//			{
//				SelectBlock(Operation, "InBindData", false);
//				Operation.Replace("<Variable>", MemberInfo.Name);
//				CString SST_ID;
//				SST_ID.Format("SST_%s_%s", StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
//				SST_ID.MakeUpper();
//				Operation.Replace("<StructName>", StructInfo.Name);
//				Operation.Replace("<SST_NAME>", SSTIDEnumName);
//				Operation.Replace("<SST_ID>", SST_ID);
//			}
//
//			CString LineSpace = GetLineSpace(Operation, "<PropertyGridRefreshOperation>");
//			CString RefreshOperation = MakePropertyGridFillOperation(StructInfo, MemberInfo, 0, SSTIDEnumName, ReplaceByBindData, LineSpace);
//			Operation.Replace("<PropertyGridRefreshOperation>", RefreshOperation);
//
//			Operations.Append(Operation);
//			Operations.Append("\r\n");
//		}
//		Index++;
//	}
//	CString LineSpace;
//	LineSpace.Format("\r\n%s", szLineSpace);
//	Operations.Replace("\r\n", LineSpace);
//	return Operations;
//}

CString CCallInterfaceMakerDlg::MakeStructXLSProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Source = m_StructXLSProcessTemplate;

	Source.Replace("<ClassName>", StructInfo.Name);

	if (StructInfo.BaseStruct.IsEmpty())
	{
		RemoveBlock(Source, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
		RetainBlock(Source, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
	}
	else
	{
		RemoveBlock(Source, "<IfNotHaveBaseClass>", "</IfNotHaveBaseClass>");
		RetainBlock(Source, "<IfHaveBaseClass>", "</IfHaveBaseClass>");
		Source.Replace("<BaseClass>", StructInfo.BaseStruct);
	}

	CString SSTIDEnumName;
	SSTIDEnumName.Format("%s_MEMBER_IDS", ClassNameToUpper(StructInfo.Name));

	CString Operations;
	CString LineSpace = GetLineSpace(Source, "<CreateXLSColumnOperations>");
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if ((!MemberInfo.ShowName.IsEmpty()) && ((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_XLS) == 0))
		{
			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
			if (pType)
			{
				CString Operation;
				if (MemberInfo.IsArray)
					Operation = m_InterfaceConfig.ArrayDefineConfig.CreateXLSColumnOperation;
				else
					Operation = pType->GenerateOperations.CreateXLSColumnOperation;
				Operation.Replace("<VariableName>", MemberInfo.Name);
				Operation.Replace("<Type>", pType->CType);
				Operation.Replace("<OrginType>", pType->Name);
				Operation.Replace("<Description>", MakeStringConst(MemberInfo.Description));
				if (!Operations.IsEmpty())
					Operations.Append("\r\n");
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace("\r\n", "\r\n" + LineSpace);
	Source.Replace("<CreateXLSColumnOperations>", Operations);

	Operations.Empty();
	LineSpace = GetLineSpace(Source, "<CheckXLSColumnOperations>");
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if ((!MemberInfo.ShowName.IsEmpty()) && ((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_XLS) == 0))
		{
			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
			if (pType)
			{
				CString Operation;
				if (MemberInfo.IsArray)
					Operation = m_InterfaceConfig.ArrayDefineConfig.CheckXLSColumnOperation;
				else
					Operation = pType->GenerateOperations.CheckXLSColumnOperation;
				Operation.Replace("<VariableName>", MemberInfo.Name);
				if (!Operations.IsEmpty())
					Operations.Append("\r\n");
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace("\r\n", "\r\n" + LineSpace);
	Source.Replace("<CheckXLSColumnOperations>", Operations);

	Operations.Empty();
	LineSpace = GetLineSpace(Source, "<ToXLSOperations>");
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if ((!MemberInfo.ShowName.IsEmpty()) && ((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_XLS) == 0))
		{
			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
			if (pType)
			{
				CString Operation;
				if(MemberInfo.IsArray)
					Operation = m_InterfaceConfig.ArrayDefineConfig.ToXLSOperation;
				else
					Operation = pType->GenerateOperations.ToXLSOperation;
				Operation.Replace("<Type>", pType->CType);
				Operation.Replace("<OrginType>", pType->Name);
				CString VarName = MemberInfo.Name;
				if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
					VarName = "m_" + VarName;
				VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
				Operation.Replace("<Variable>", VarName);				
				if (!Operations.IsEmpty())
					Operations.Append("\r\n");
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace("\r\n", "\r\n" + LineSpace);
	Source.Replace("<ToXLSOperations>", Operations);

	Operations.Empty();
	LineSpace = GetLineSpace(Source, "<FromXLSOperations>");
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if ((!MemberInfo.ShowName.IsEmpty()) && ((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_XLS) == 0))
		{
			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
			if (pType)
			{
				CString Operation;
				if (MemberInfo.IsArray)
					Operation = m_InterfaceConfig.ArrayDefineConfig.FromXLSOperation;
				else
					Operation = pType->GenerateOperations.FromXLSOperation;
				Operation.Replace("<Type>", pType->CType);
				Operation.Replace("<OrginType>", pType->Name);
				CString VarName = MemberInfo.Name;
				if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
					VarName = "m_" + VarName;
				VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
				Operation.Replace("<Variable>", VarName);
				Operation.Replace("<VariableName>", MemberInfo.Name);
				if (!Operations.IsEmpty())
					Operations.Append("\r\n");
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace("\r\n", "\r\n" + LineSpace);
	Source.Replace("<FromXLSOperations>", Operations);
		

	CString Space = "\r\n";
	Space += szLineSpace;
	Source.Replace("\r\n", Space);
	return Source;
}

bool CCallInterfaceMakerDlg::HaveMemberByType(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR Type, int Depth)
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

STRUCT_DEFINE_INFO * CCallInterfaceMakerDlg::GetStructDefineInfo(LPCTSTR Type)
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
ENUM_DEFINE_INFO* CCallInterfaceMakerDlg::GetEnumDefine(LPCTSTR EnumName)
{
	for (size_t i = 0; i < m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == DATA_STRUCT_ENUM)
		{
			ENUM_DEFINE_LIST* pEnumList = (ENUM_DEFINE_LIST*)m_AllDataStructList[i];
			for (ENUM_DEFINE_INFO& Info : pEnumList->EnumList)
			{
				if (Info.Name == EnumName)
					return &Info;
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
			for (size_t j = 0; j < m_MainModule.ImportList.size(); j++)
			{
				if (m_MainModule.ImportList[j] == m_ImportModuleList[i].Name)
				{
					m_MainModule.ImportList.erase(m_MainModule.ImportList.begin() + i);
				}
			}

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
	CString ToComment = m_InterfaceConfig.CommentPrefix + Content;
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



bool CCallInterfaceMakerDlg::RemoveBlock(CString& Template, LPCTSTR szBlockStart, LPCTSTR szBlockEnd)
{
	int StartBlockLen = strlen(szBlockStart);
	int EndBlockLen = strlen(szBlockEnd);
	int StartPos = Template.Find(szBlockStart);
	bool Removed = false;
	while (StartPos >= 0)
	{
		//如果有嵌套，先处理最内层
		int Next = Template.Find(szBlockStart, StartPos + StartBlockLen);
		while (Next >= 0)
		{
			StartPos = Next;
			Next = Template.Find(szBlockStart, StartPos + StartBlockLen);
		}
		int EndPos = Template.Find(szBlockEnd, StartPos);
		if (EndPos < 0)
			return Removed;
		EndPos += EndBlockLen;
		//判断是否要去除整行
		int Prefix = IsEmptyPrefix(Template, StartPos);
		int Suffix = IsEmptySuffix(Template, EndPos);
		int CutStart, CutEnd;
		if ((Prefix >= 0) && (Suffix >= 0))
		{
			CutStart = Prefix;
			CutEnd = Suffix;
		}
		else
		{
			CutStart = StartPos;
			CutEnd = EndPos;
		}
		//if ((EndPos < Template.GetLength() - 1) && (Template[EndPos] == '\r') && (Template[EndPos + 1] == '\n')
		//	&& ((StartPos == 0) || IsEmptyPrefix(Template, StartPos)))
		//{
		//	EndPos += 2;
		//}
		//int CheckPos = StartPos;
		//bool NeedCutHeader = (CheckPos > 0);
		//CheckPos--;
		//while (CheckPos >= 0)
		//{
		//	if (Template[CheckPos] == '\r' || Template[CheckPos] == '\n')
		//		break;
		//	else if (Template[CheckPos] != ' ' && Template[CheckPos] != '\t')
		//	{
		//		NeedCutHeader = false;
		//		break;
		//	}
		//	CheckPos--;
		//}
		//if (NeedCutHeader)
		//{
		//	while (StartPos > 0)
		//	{
		//		if (Template[StartPos - 1] != ' ' && Template[StartPos - 1] != '\t')//&&Template[StartPos-1]!='\r'&&Template[StartPos-1]!='\n')
		//			break;
		//		StartPos--;
		//	}
		//}
		Template.Delete(CutStart, CutEnd - CutStart);
		Removed = true;
		StartPos = Template.Find(szBlockStart);
	}
	return Removed;
}
void CCallInterfaceMakerDlg::RetainBlock(CString& Template, LPCTSTR szBlockStart, LPCTSTR szBlockEnd)
{
	RemoveMark(Template, szBlockStart);
	RemoveMark(Template, szBlockEnd);
}
void CCallInterfaceMakerDlg::SelectBlock(CString& Template, LPCTSTR szBlockName, bool SelectWitch)
{
	CString StartMark, EndMark;
	if (SelectWitch)
	{
		StartMark.Format("<%s>", szBlockName);
		RemoveMark(Template, StartMark);
		StartMark.Format("<!%s>", szBlockName);
		EndMark.Format("</%s>", szBlockName);
		RemoveBlock(Template, StartMark, EndMark);
	}
	else
	{
		StartMark.Format("<%s>", szBlockName);
		EndMark.Format("<!%s>", szBlockName);
		RemoveBlock(Template, StartMark, EndMark);
		EndMark.Format("</%s>", szBlockName);
		RemoveMark(Template, EndMark);
	}
}
void CCallInterfaceMakerDlg::RemoveMark(CString& Template, LPCTSTR szMark)
{
	int BlockSize = strlen(szMark);
	int StartPos = Template.Find(szMark);
	while (StartPos >= 0)
	{
		int EndPos = StartPos + BlockSize;
		bool NeedCutHeader = (StartPos > 0);
		if (EndPos < Template.GetLength() - 1 && Template[EndPos] == '\r' && Template[EndPos + 1] == '\n')
		{
			//EndPos += 2;
		}
		else
		{
			NeedCutHeader = false;
		}
		if(NeedCutHeader)
		{
			int CheckPos = StartPos;
			CheckPos--;
			while (CheckPos >= 0)
			{
				if (Template[CheckPos] == '\r' || Template[CheckPos] == '\n')
					break;
				else if (Template[CheckPos] != ' ' && Template[CheckPos] != '\t')
				{
					NeedCutHeader = false;
					break;
				}
				CheckPos--;
			}
			if (NeedCutHeader)
			{
				if (Template[EndPos] == '\r' && Template[EndPos + 1] == '\n')
					EndPos += 2;
				while (StartPos > 0)
				{
					if (Template[StartPos - 1] != ' ' && Template[StartPos - 1] != '\t')//&&Template[StartPos-1]!='\r'&&Template[StartPos-1]!='\n')
						break;
					StartPos--;
				}
			}
		}
		Template.Delete(StartPos, EndPos - StartPos);
		StartPos = Template.Find(szMark);
	}
}
void CCallInterfaceMakerDlg::ReplaceBlock(CString& Template, LPCTSTR szBlockStart, LPCTSTR szBlockEnd, LPCTSTR ReplaceStr)
{
	int StartPos = Template.Find(szBlockStart);
	while (StartPos >= 0)
	{
		int EndPos = Template.Find(szBlockEnd);
		if (EndPos < 0)
			return;
		EndPos += strlen(szBlockEnd);
		if (EndPos < Template.GetLength() - 1 && Template[EndPos] == '\r' && Template[EndPos + 1] == '\n')
			EndPos += 2;
		int CheckPos = StartPos;
		bool NeedCutHeader = (CheckPos > 0);
		CheckPos--;
		while (CheckPos >= 0)
		{
			if (Template[CheckPos] == '\r' || Template[CheckPos] == '\n')
				break;
			else if (Template[CheckPos] != ' ' && Template[CheckPos] != '\t')
			{
				NeedCutHeader = false;
				break;
			}
			CheckPos--;
		}
		if (NeedCutHeader)
		{
			while (StartPos > 0)
			{
				if (Template[StartPos - 1] != ' ' && Template[StartPos - 1] != '\t')//&&Template[StartPos-1]!='\r'&&Template[StartPos-1]!='\n')
					break;
				StartPos--;
			}
		}
		Template.Delete(StartPos, EndPos - StartPos);
		Template.Insert(StartPos, ReplaceStr);
		StartPos = Template.Find(szBlockStart);
	}
}
CString CCallInterfaceMakerDlg::GetBlock(const CString& Template, LPCTSTR szBlockStart, LPCTSTR szBlockEnd)
{
	CString BlockStr;
	int StartPos = Template.Find(szBlockStart);
	if (StartPos >= 0)
	{
		StartPos+= strlen(szBlockStart);
		int EndPos = Template.Find(szBlockEnd, StartPos);
		if (EndPos > StartPos)
		{
			BlockStr = Template.Mid(StartPos, EndPos - StartPos);
		}
	}
	return BlockStr;
}
int CCallInterfaceMakerDlg::IsEmptyPrefix(CString& Template, int Pos)
{
	Pos--;
	while (Pos >= 0)
	{
		if ((Template[Pos] == '\r') || (Template[Pos] == '\n'))
			break;
		if ((Template[Pos] != ' ') && (Template[Pos] != '\t'))
			return -1;
		Pos--;
	}
	return Pos + 1;
}
int CCallInterfaceMakerDlg::IsEmptySuffix(CString& Template, int Pos)
{
	int Len = Template.GetLength();
	while (Pos < Len)
	{
		if ((Template[Pos] == '\r') || (Template[Pos] == '\n'))
			break;
		if ((Template[Pos] != ' ') && (Template[Pos] != '\t'))
			return -1;
		Pos++;
	}
	while (Pos < Len)
	{
		if ((Template[Pos] != '\r') && (Template[Pos] != '\n'))
			break;
		Pos++;
	}
	return Pos;
}

CString CCallInterfaceMakerDlg::RestoreFileToTemplate(LPCTSTR szFileName,const CString& szTemplate)
{
	if (CFileTools::IsFileExist(szFileName))
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

bool CCallInterfaceMakerDlg::MoveDataStruct(int Type, LPCTSTR SourcePacket, LPCTSTR TargetPacket, LPCTSTR Name)
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

CString CCallInterfaceMakerDlg::ProcessArrayOperation(CString Operation, TYPE_DEFINE* pElementType)
{
	if(pElementType)
	{
		if (pElementType->Flag & TYPE_DEFINE_FLAG_BASE_TYPE)
		{
			RemoveBlock(Operation, "<IfIsNotBaseType>", "</IfIsNotBaseType>");
			Operation.Replace("<IfIsBaseType>", "");
			Operation.Replace("</IfIsBaseType>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfIsBaseType>", "</IfIsBaseType>");
			Operation.Replace("<IfIsNotBaseType>", "");
			Operation.Replace("</IfIsNotBaseType>", "");
		}		
		
		if ((pElementType->Flag & TYPE_DEFINE_FLAG_VARIABLE_LEN)|| (pElementType->Flag & TYPE_DEFINE_FLAG_STRUCT))
		{
			RemoveBlock(Operation, "<IfIsFixLenType>", "</IfIsFixLenType>");
			Operation.Replace("<IfIsVariableLenType>", "");
			Operation.Replace("</IfIsVariableLenType>", "");
		}
		else
		{
			RemoveBlock(Operation, "<IfIsVariableLenType>", "</IfIsVariableLenType>");
			Operation.Replace("<IfIsFixLenType>", "");
			Operation.Replace("</IfIsFixLenType>", "");
		}

		Operation.Replace("<ElementType>", pElementType->CType);
		Operation.Replace("<Index>", "i");
	}
	return Operation;
}

bool CCallInterfaceMakerDlg::CheckNameChange(CString& Operation, CString& Name, LPCTSTR szChangeToken)
{
	int Start = Operation.Find(szChangeToken);
	if (Start >= 0)
	{
		UINT TokenLen = _tcslen(szChangeToken);
		int End = Operation.Find('>', Start + TokenLen);
		if (End >= 0)
		{
			Name = Operation.Mid(Start + TokenLen, End - Start - TokenLen);
			Operation.Delete(Start, End - Start + 1);
		}
	}
	return false;
}

void CCallInterfaceMakerDlg::IncSpace(const CEasyString& InputStr, CString& OutputStr, const CString& Space)
{
	CEasyArray<int> ReplacePosList(512, 1024);
	int Pos = InputStr.Find("\r\n", 0);
	while (Pos >= 0)
	{
		ReplacePosList.Add(Pos);
		Pos = InputStr.Find("\r\n", Pos + 2);
	}
	if (ReplacePosList.GetCount())
	{
		const char* pSrc = (LPCTSTR)InputStr;
		char* pBuffer = OutputStr.GetBuffer(InputStr.GetLength() + Space.GetLength() * ReplacePosList.GetCount() + 1);
		int OutPos = 0;
		int StartPos = 0;
		for (int Pos : ReplacePosList)
		{
			strncpy(pBuffer + OutPos, pSrc + StartPos, 2 + Pos - StartPos);
			OutPos += 2 + Pos - StartPos;
			StartPos += 2 + Pos - StartPos;
			strncpy(pBuffer + OutPos, Space, Space.GetLength());
			OutPos += Space.GetLength();
		}
		if (StartPos < InputStr.GetLength() - 1)
		{
			strncpy(pBuffer + OutPos, pSrc + StartPos, InputStr.GetLength() - StartPos - 1);
			OutPos += InputStr.GetLength() - StartPos - 1;
			pBuffer[OutPos] = 0;
		}
		OutputStr.ReleaseBuffer(OutPos);
	}
	else
	{
		OutputStr = InputStr;
	}
}

bool CCallInterfaceMakerDlg::IsNaturalEnum(ENUM_DEFINE_INFO& EnumInfo)
{
	for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
	{
		if (!Info.Value.IsEmpty())
			return false;
	}
	return true;
}

UINT CCallInterfaceMakerDlg::GetEnumMemberCount(ENUM_DEFINE_INFO& EnumInfo)
{
	UINT Count = 0;
	for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
	{
		if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			Count++;
	}
	return Count;
}

UINT CCallInterfaceMakerDlg::GetStructMemberCount(STRUCT_DEFINE_INFO& StructInfo)
{
	UINT Count = 0;
	if (!StructInfo.BaseStruct.IsEmpty())
	{
		STRUCT_DEFINE_INFO* pStructInfo = GetStructDefineInfo(StructInfo.BaseStruct);
		if (pStructInfo)
		{
			Count += GetStructMemberCount(*pStructInfo);
		}
	}
	Count += StructInfo.MemberList.size();
	return Count;
}
STRUCT_MEMBER_INFO* CCallInterfaceMakerDlg::IsBeBind(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR MemberName)
{
	for (STRUCT_MEMBER_INFO& Member : StructInfo.MemberList)
	{
		if (Member.BindData == MemberName)
			return &Member;
	}
	return NULL;
}

STRUCT_MEMBER_INFO* CCallInterfaceMakerDlg::GetStructKeyMember(LPCTSTR StructName)
{
	STRUCT_DEFINE_INFO* pStructInfo = GetStructDefineInfo(StructName);
	if (pStructInfo)
	{
		for (STRUCT_MEMBER_INFO& Member : pStructInfo->MemberList)
		{
			if (Member.Flag & STRUCT_MEMBER_FLAG_IS_KEY)
				return &Member;
		}
	}
	return NULL;
}
