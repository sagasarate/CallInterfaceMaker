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
	m_SelectAll = true;
	m_MainModule.IDSeed = 1;
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

	if (!LoadTypeDefByJson())
		LoadTypeDefByXML();
	if (!LoadConfigByJson())
		LoadConfigByXML();
	if (!LoadEnvByJson())
		LoadEnvByXML();
	LoadTemples();




	MakeCurVarType(true);

	m_lvInterfaceList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	m_lvInterfaceList.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 200);
	m_lvInterfaceList.InsertColumn(1, _T("模块"), LVCFMT_LEFT, 100);
	m_lvInterfaceList.InsertColumn(2, _T("方法数量"), LVCFMT_LEFT, 60);
	m_lvInterfaceList.InsertColumn(3, _T("ID"), LVCFMT_LEFT, 60);
	m_lvInterfaceList.InsertColumn(4, _T("描述"), LVCFMT_LEFT, 200);




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

	if (Dlg.DoModal() == IDOK)
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
	POSITION Pos = m_lvInterfaceList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvInterfaceList.GetNextSelectedItem(Pos);
		UINT Param = (UINT)m_lvInterfaceList.GetItemData(Item);
		UINT ModuleID = (Param >> 8) & 0x7F;
		UINT InterfaceID = Param & 0xFF;
		CALLER_INTERFACE* pInterfaceInfo = GetInterfaceInfoByID(ModuleID, InterfaceID);
		if (pInterfaceInfo)
		{
			CDlgInterfaceEdtor Dlg;
			Dlg.SetData(*pInterfaceInfo);

			if (Dlg.DoModal() == IDOK)
			{
				Dlg.GetData(*pInterfaceInfo);
				EditListItem(Item, *pInterfaceInfo);
			}
		}
	}
	else
	{
		MessageBox(_T("请选择一个接口"));
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonDelInterface()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos = m_lvInterfaceList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvInterfaceList.GetNextSelectedItem(Pos);
		UINT Param = (UINT)m_lvInterfaceList.GetItemData(Item);
		UINT ModuleID = (Param >> 8) & 0x7F;
		UINT InterfaceID = Param & 0xFF;
		CALLER_INTERFACE* pInterfaceInfo = GetInterfaceInfoByID(ModuleID, InterfaceID);
		if (pInterfaceInfo)
		{
			CString Msg;
			Msg.Format(_T("是否要删除接口[%s]"), pInterfaceInfo->Name);
			if (AfxMessageBox(Msg, MB_YESNO) == IDYES)
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
		MODULE_DEFINE_INFO* pNewModule = GetModuleInfo(Dlg.m_SelectedModuleID);
		if (pNewModule)
		{
			POSITION Pos = m_lvInterfaceList.GetFirstSelectedItemPosition();
			while (Pos)
			{
				int Item = m_lvInterfaceList.GetNextSelectedItem(Pos);
				UINT Param = (UINT)m_lvInterfaceList.GetItemData(Item);
				UINT ModuleID = (Param >> 8) & 0x7F;
				UINT InterfaceID = Param & 0xFF;
				MODULE_DEFINE_INFO* pOldModule = GetModuleInfo(ModuleID);
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
	int Item = m_lvInterfaceList.InsertItem(m_lvInterfaceList.GetItemCount(), InterfaceInfo.Name);
	m_lvInterfaceList.SetItemData(Item, (INT_PTR)(((InterfaceInfo.ModuleID & 0x7F) << 8) | (InterfaceInfo.ID & 0xFF)));
	Temp.Format(_T("%d"), InterfaceInfo.MethodList.size());
	m_lvInterfaceList.SetItemText(Item, 1, GetModuleName(InterfaceInfo.ModuleID));
	m_lvInterfaceList.SetItemText(Item, 2, Temp);
	Temp.Format(_T("%d"), InterfaceInfo.ID);
	m_lvInterfaceList.SetItemText(Item, 3, Temp);
	m_lvInterfaceList.SetItemText(Item, 4, InterfaceInfo.Description);
}
void CCallInterfaceMakerDlg::EditListItem(int Item, CALLER_INTERFACE& InterfaceInfo)
{
	CString Temp;
	m_lvInterfaceList.SetItemText(Item, 0, InterfaceInfo.Name);
	Temp.Format(_T("%d"), InterfaceInfo.MethodList.size());
	m_lvInterfaceList.SetItemText(Item, 1, GetModuleName(InterfaceInfo.ModuleID));
	m_lvInterfaceList.SetItemText(Item, 2, Temp);
	Temp.Format(_T("%d"), InterfaceInfo.ID);
	m_lvInterfaceList.SetItemText(Item, 3, Temp);
	m_lvInterfaceList.SetItemText(Item, 4, InterfaceInfo.Description);
}
bool CCallInterfaceMakerDlg::IsListItemChecked(UINT ModuleID, UINT InterfaceID)
{
	for (int i = 0;i < m_lvInterfaceList.GetItemCount();i++)
	{
		if (m_lvInterfaceList.GetCheck(i))
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
	for (UINT i = 0; i < m_AllInterfaceList.size(); i++)
	{
		AddListItem(*m_AllInterfaceList[i]);
	}
}

bool CCallInterfaceMakerDlg::LoadTypeDefByXML()
{
	xml_parser Parser;
	CEasyString FileName = CFileTools::MakeModuleFullPath(NULL, TYPE_DEF_FILE_NAME_XML);
	if (Parser.parse_file(FileName, pug::parse_trim_attribute))
	{
		xml_node TypeDef = Parser.document();
		if (TypeDef.moveto_child(_T("TypeDef")))
		{
			for (UINT i = 0;i < TypeDef.children();i++)
			{
				xml_node Type = TypeDef.child(i);
				if (_tcsicmp(Type.name(), _T("Type")) == 0)
				{
					TYPE_DEFINE TypeInfo;
					TypeInfo.Name = ((LPCTSTR)Type.attribute(_T("Name")).getvalue());
					TypeInfo.CType = ((LPCTSTR)Type.attribute(_T("CType")).getvalue());
					TypeInfo.Flag = Type.attribute(_T("Flag"));
					LoadGenerateOperations(Type, TypeInfo.GenerateOperations);
					m_BaseTypeList.push_back(TypeInfo);
				}
			}
		}
		return true;
	}
	return false;
}
bool CCallInterfaceMakerDlg::SaveTypeDefByXML()
{
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("utf-8");
	xml_node TypeDef = Doc.append_child(node_element, _T("TypeDef"));

	for (size_t i = 0; i < m_BaseTypeList.size(); i++)
	{
		xml_node Type = TypeDef.append_child(node_element, _T("Type"));

		Type.append_attribute(_T("Name"), (LPCTSTR)m_BaseTypeList[i].Name);
		Type.append_attribute(_T("CType"), (LPCTSTR)m_BaseTypeList[i].CType);
		Type.append_attribute(_T("Flag"), m_BaseTypeList[i].Flag);
		SaveGenerateOperations(Type, m_BaseTypeList[i].GenerateOperations);
	}
	CEasyString FileName = CFileTools::MakeModuleFullPath(NULL, TYPE_DEF_FILE_NAME_XML);
	if (!Xml.SaveToFile(Xml.document(), FileName, CP_UTF8))
	{
		CString Msg;
		Msg.Format(_T("保存文件失败%s"), FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadTypeDefByJson()
{
	CEasyString FileName = CFileTools::MakeModuleFullPath(NULL, TYPE_DEF_FILE_NAME_JSON);
	CStringFile JsonFile;
	JsonFile.SetLocalCodePage(CP_UTF8);

	if (JsonFile.LoadFile(FileName, false))
	{
		if (JsonFile.GetDataLen())
		{
			rapidjson::Document Root;
			Root.Parse(JsonFile.GetData());
			if (!Root.HasParseError())
			{
				if (Root.IsArray())
				{
					for (size_t i = 0; i < Root.Size(); i++)
					{
						rapidjson::Value& Entry = Root[(rapidjson::SizeType)i];
						if (Entry.IsObject())
						{
							TYPE_DEFINE TypeDefine;
							GetJsonValue(Entry[_T("Name")], TypeDefine.Name);
							GetJsonValue(Entry[_T("CType")], TypeDefine.CType);
							GetJsonValue(Entry[_T("Flag")], TypeDefine.Flag);
							{
								rapidjson::Value& Value = Entry[_T("GenerateOperations")];
								if (Value.IsObject())
									LoadGenerateOperations(Value, TypeDefine.GenerateOperations);
							}
							m_BaseTypeList.push_back(TypeDefine);
						}
					}
					return true;
				}
				else
				{
					AfxMessageBoxEx(MB_OK, 0, _T("文件格式错误:%s"), (LPCTSTR)FileName);
				}
			}
		}
	}
	else
	{
		AfxMessageBoxEx(MB_OK, 0, _T("打开文件失败:%s"), (LPCTSTR)FileName);
	}
	return false;
}
bool CCallInterfaceMakerDlg::SaveTypeDefByJson()
{
	rapidjson::Document Root;
	rapidjson::Document::AllocatorType& Alloc = Root.GetAllocator();

	Root.SetArray();

	for (size_t i = 0; i < m_BaseTypeList.size(); i++)
	{
		rapidjson::Value TypeNode(rapidjson::kObjectType);

		TypeNode.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)m_BaseTypeList[i].Name, m_BaseTypeList[i].Name.GetLength()), Alloc);
		TypeNode.AddMember(_T("CType"), rapidjson::Value((LPCTSTR)m_BaseTypeList[i].CType, m_BaseTypeList[i].CType.GetLength()), Alloc);
		TypeNode.AddMember(_T("Flag"), m_BaseTypeList[i].Flag, Alloc);

		rapidjson::Value OperationsNode(rapidjson::kObjectType);
		if (SaveGenerateOperations(OperationsNode, Alloc, m_BaseTypeList[i].GenerateOperations))
			TypeNode.AddMember(_T("GenerateOperations"), OperationsNode, Alloc);

		Root.PushBack(TypeNode, Alloc);
	}

#ifdef UNICODE
	rapidjson::GenericStringBuffer<rapidjson::UTF16<> > buffer;
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF16<> >, rapidjson::UTF16<>, rapidjson::UTF16<> > writer(buffer);
#else
	rapidjson::GenericStringBuffer<rapidjson::UTF8<> > buffer;
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<> >, rapidjson::UTF8<>, rapidjson::UTF8<> > writer(buffer);
#endif

	Root.Accept(writer);

	CStringFile JsonFile;

	JsonFile.SetSaveCodePage(CP_UTF8);
	JsonFile.LoadFromString(buffer.GetString(), -1, false);
	CEasyString FileName = CFileTools::MakeModuleFullPath(NULL, TYPE_DEF_FILE_NAME_JSON);
	if (JsonFile.SaveToFile(FileName))
	{
		return true;
	}
	else
	{
		CString Msg;
		Msg.Format(_T("保存文件失败%s"), FileName);
		MessageBox(Msg);
		return false;
	}
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
		Msg.Format(_T("无法打开文件%s"), (LPCTSTR)FilePath);
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
	//LoadTemple(STRUCT_EDITOR_PROCESS_TEMPLE_FILE_NAME, m_StructEditorProcessTemplate);
	//LoadTemple(CREATE_LIST_HEADER_UNIT_TEMPLE_FILE_NAME, m_CreateListHeaderUnitTemplate);
	//LoadTemple(FILL_LIST_ITEM_UNIT_TEMPLE_FILE_NAME, m_FillListItemUnitTemplate);
	//LoadTemple(PROPERTY_GRID_FETCH_UNIT_TEMPLE_FILE_NAME, m_PropertyGridFetchUnitTemplate);
	//LoadTemple(PROPERTY_GRID_ADD_ITEM_UNIT_TEMPLE_FILE_NAME, m_PropertyGridAddItemUnitTemplate);
	//LoadTemple(PROPERTY_GRID_DEL_ITEM_UNIT_TEMPLE_FILE_NAME, m_PropertyGridDelItemUnitTemplate);
	//LoadTemple(PROPERTY_GRID_MOVE_ITEM_UNIT_TEMPLE_FILE_NAME, m_PropertyGridMoveItemUnitTemplate);
	LoadTemple(STRUCT_FILE_LOG_PROCESS_TEMPLE_FILE_NAME, m_StructFileLogProcessTemplate);
	LoadTemple(STRUCT_ALI_LOG_PROCESS_TEMPLE_FILE_NAME, m_StructAliLogProcessTemplate);
	LoadTemple(STRUCT_LUA_PROCESS_TEMPLE_FILE_NAME, m_StructLuaProcessTemplate);
	LoadTemple(DATA_OBJECT_LUA_PROCESS_TEMPLE_FILE_NAME, m_DataObjectLuaProcessTemplate);
	LoadTemple(DATA_OBJECT_LUA_PROCESS_HEADER_TEMPLE_FILE_NAME, m_DataObjectLuaProcessHeaderTemplate);
	LoadTemple(TO_LUA_TABLE_OPERATION_UNIT_TEMPLE_FILE_NAME, m_ToLuaTableOperationUnitTemplate);
	LoadTemple(FROM_LUA_TABLE_OPERATION_UNIT_TEMPLE_FILE_NAME, m_FromLuaTableOperationUnitTemplate);
	LoadTemple(STRUCT_XLS_PROCESS_TEMPLE_FILE_NAME, m_StructXLSProcessTemplate);
	LoadTemple(DEPEND_IMPORT_TEMPLE_FILE_NAME, m_DependImportTemplate);



	return true;
}

bool CCallInterfaceMakerDlg::LoadConfigByXML()
{
	xml_parser Parser;

	CString FileName = CFileTools::MakeModuleFullPath(NULL, CONFIG_FILE_NAME_XML);

	if (Parser.parse_file(FileName, 0))
	{

		xml_node Config = Parser.document();

		if (Config.moveto_child(_T("Config")))
		{
			{
				xml_node OneFileExport = Config;
				if (OneFileExport.moveto_child(_T("OneFileExport")))
				{
					m_InterfaceConfig.OneFileExportConfig.IsExport = (bool)OneFileExport.attribute(_T("IsExport"));
					m_InterfaceConfig.OneFileExportConfig.ExportExt = (LPCTSTR)OneFileExport.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node InterfaceHeaderExport = Config;
				if (InterfaceHeaderExport.moveto_child(_T("InterfaceHeaderExport")))
				{
					m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport = (bool)InterfaceHeaderExport.attribute(_T("IsExport"));
					m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt = (LPCTSTR)InterfaceHeaderExport.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node StructExport = Config;
				if (StructExport.moveto_child(_T("StructExport")))
				{
					m_InterfaceConfig.StructExportConfig.IsExport = (bool)StructExport.attribute(_T("IsExport"));
					m_InterfaceConfig.StructExportConfig.ExportExt = (LPCTSTR)StructExport.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node EnumExport = Config;
				if (EnumExport.moveto_child(_T("EnumExport")))
				{
					m_InterfaceConfig.EnumExportConfig.IsExport = (bool)EnumExport.attribute(_T("IsExport"));
					m_InterfaceConfig.EnumExportConfig.ExportExt = (LPCTSTR)EnumExport.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node ConstExport = Config;
				if (ConstExport.moveto_child(_T("ConstExport")))
				{
					m_InterfaceConfig.ConstExportConfig.IsExport = (bool)ConstExport.attribute(_T("IsExport"));
					m_InterfaceConfig.ConstExportConfig.ExportExt = (LPCTSTR)ConstExport.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node DOHeaderExport = Config;
				if (DOHeaderExport.moveto_child(_T("DataStructHeaderExport")))
				{
					m_InterfaceConfig.DataStructHeaderExportConfig.IsExport = (bool)DOHeaderExport.attribute(_T("IsExport"));
					m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt = (LPCTSTR)DOHeaderExport.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node InterfaceExportConfig = Config;
				if (InterfaceExportConfig.moveto_child(_T("InterfaceExport")))
				{
					m_InterfaceConfig.InterfaceExportConfig.IsExport = (bool)InterfaceExportConfig.attribute(_T("IsExport"));
					m_InterfaceConfig.InterfaceExportConfig.ExportExt = (LPCTSTR)InterfaceExportConfig.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node CallHeaderExportConfig = Config;
				if (CallHeaderExportConfig.moveto_child(_T("CallHeaderExport")))
				{
					m_InterfaceConfig.CallHeaderExportConfig.IsExport = (bool)CallHeaderExportConfig.attribute(_T("IsExport"));
					m_InterfaceConfig.CallHeaderExportConfig.ExportExt = (LPCTSTR)CallHeaderExportConfig.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node CallSourceExportConfig = Config;
				if (CallSourceExportConfig.moveto_child(_T("CallSourceExport")))
				{
					m_InterfaceConfig.CallSourceExportConfig.IsExport = (bool)CallSourceExportConfig.attribute(_T("IsExport"));
					m_InterfaceConfig.CallSourceExportConfig.ExportExt = (LPCTSTR)CallSourceExportConfig.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node HandlerHeaderExportConfig = Config;
				if (HandlerHeaderExportConfig.moveto_child(_T("HandlerHeaderExport")))
				{
					m_InterfaceConfig.HandlerHeaderExportConfig.IsExport = (bool)HandlerHeaderExportConfig.attribute(_T("IsExport"));
					m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt = (LPCTSTR)HandlerHeaderExportConfig.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node HandlerSourceExportConfig = Config;
				if (HandlerSourceExportConfig.moveto_child(_T("HandlerSourceExport")))
				{
					m_InterfaceConfig.HandlerSourceExportConfig.IsExport = (bool)HandlerSourceExportConfig.attribute(_T("IsExport"));
					m_InterfaceConfig.HandlerSourceExportConfig.ExportExt = (LPCTSTR)HandlerSourceExportConfig.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node DataObjectHeaderExportConfig = Config;
				if (DataObjectHeaderExportConfig.moveto_child(_T("DataObjectHeaderExport")))
				{
					m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport = (bool)DataObjectHeaderExportConfig.attribute(_T("IsExport"));
					m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt = (LPCTSTR)DataObjectHeaderExportConfig.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node DataObjectSourceExportExportConfig = Config;
				if (DataObjectSourceExportExportConfig.moveto_child(_T("DataObjectSourceExport")))
				{
					m_InterfaceConfig.DataObjectSourceExportConfig.IsExport = (bool)DataObjectSourceExportExportConfig.attribute(_T("IsExport"));
					m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt = (LPCTSTR)DataObjectSourceExportExportConfig.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node DataObjectModifyFlagsExportConfig = Config;
				if (DataObjectModifyFlagsExportConfig.moveto_child(_T("DataObjectModifyFlagsHeaderExport")))
				{
					m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.IsExport = (bool)DataObjectModifyFlagsExportConfig.attribute(_T("IsExport"));
					m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt = (LPCTSTR)DataObjectModifyFlagsExportConfig.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node DataObjectModifyFlagsExportConfig = Config;
				if (DataObjectModifyFlagsExportConfig.moveto_child(_T("DataObjectModifyFlagsSourceExport")))
				{
					m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.IsExport = (bool)DataObjectModifyFlagsExportConfig.attribute(_T("IsExport"));
					m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.ExportExt = (LPCTSTR)DataObjectModifyFlagsExportConfig.attribute(_T("FileExt")).getvalue();
				}
			}
			{
				xml_node DefaultStructGenerateOperations = Config;
				if (DefaultStructGenerateOperations.moveto_child(_T("DefaultStructGenerateOperations")))
				{
					LoadGenerateOperations(DefaultStructGenerateOperations, m_InterfaceConfig.DefaultStructGenerateOperations);
				}
			}
			{
				xml_node DefaultEnumGenerateOperations = Config;
				if (DefaultEnumGenerateOperations.moveto_child(_T("DefaultEnumGenerateOperations")))
				{
					LoadGenerateOperations(DefaultEnumGenerateOperations, m_InterfaceConfig.DefaultEnumGenerateOperations);
				}
			}
			{
				xml_node DefaultEnum64GenerateOperations = Config;
				if (DefaultEnum64GenerateOperations.moveto_child(_T("DefaultEnum64GenerateOperations")))
				{
					LoadGenerateOperations(DefaultEnum64GenerateOperations, m_InterfaceConfig.DefaultEnum64GenerateOperations);
				}
			}
			{
				xml_node ArrayDefineConfig = Config;
				if (ArrayDefineConfig.moveto_child(_T("ArrayDefineConfig")))
				{
					LoadGenerateOperations(ArrayDefineConfig, m_InterfaceConfig.ArrayDefineConfig);
				}
			}
			{
				xml_node OtherConfig = Config;
				if (OtherConfig.moveto_child(_T("OtherConfig")))
				{
					if (OtherConfig.has_attribute(_T("CommentPrefix")))
						m_InterfaceConfig.CommentPrefix = OtherConfig.attribute(_T("CommentPrefix")).getvalue();

					if (OtherConfig.has_attribute(_T("LocalVariableDefinePrefix")))
						m_InterfaceConfig.LocalVariableDefinePrefix = OtherConfig.attribute(_T("LocalVariableDefinePrefix")).getvalue();

					if (OtherConfig.has_attribute(_T("MemberVariablePrefix")))
						m_InterfaceConfig.MemberVariablePrefix = OtherConfig.attribute(_T("MemberVariablePrefix")).getvalue();

					if (OtherConfig.has_attribute(_T("DefaultPacketName")))
						m_InterfaceConfig.DefaultPacketName = OtherConfig.attribute(_T("DefaultPacketName")).getvalue();

					if (OtherConfig.has_attribute(_T("DefaultPacketMemberName")))
						m_InterfaceConfig.DefaultPacketMemberName = OtherConfig.attribute(_T("DefaultPacketMemberName")).getvalue();

					if (OtherConfig.has_attribute(_T("SupportBigInt")))
						m_InterfaceConfig.SupportBigInt = OtherConfig.attribute(_T("SupportBigInt"));

					if (OtherConfig.has_attribute(_T("ForceExportAll")))
						m_InterfaceConfig.ForceExportAll = OtherConfig.attribute(_T("ForceExportAll"));
				}
			}
		}

	}
	else
	{
		CString Msg;
		Msg.Format(_T("无法打开文件%s"), FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}
bool CCallInterfaceMakerDlg::SaveConfigByXML()
{
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("utf-8");

	xml_node Config = Doc.append_child(node_element, _T("Config"));

	{
		xml_node InterfaceHeaderExport = Config.append_child(node_element, _T("OneFileExport"));
		InterfaceHeaderExport.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.OneFileExportConfig.IsExport);
		InterfaceHeaderExport.append_attribute(_T("FileExt"),
			m_InterfaceConfig.OneFileExportConfig.ExportExt);
	}
	{
		xml_node InterfaceHeaderExport = Config.append_child(node_element, _T("InterfaceHeaderExport"));
		InterfaceHeaderExport.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport);
		InterfaceHeaderExport.append_attribute(_T("FileExt"),
			m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt);
	}

	{
		xml_node StructExport = Config.append_child(node_element, _T("StructExport"));
		StructExport.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.StructExportConfig.IsExport);
		StructExport.append_attribute(_T("FileExt"),
			m_InterfaceConfig.StructExportConfig.ExportExt);
	}

	{
		xml_node EnumExport = Config.append_child(node_element, _T("EnumExport"));
		EnumExport.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.EnumExportConfig.IsExport);
		EnumExport.append_attribute(_T("FileExt"),
			m_InterfaceConfig.EnumExportConfig.ExportExt);
	}

	{
		xml_node ConstExport = Config.append_child(node_element, _T("ConstExport"));
		ConstExport.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.ConstExportConfig.IsExport);
		ConstExport.append_attribute(_T("FileExt"),
			m_InterfaceConfig.ConstExportConfig.ExportExt);
	}

	{
		xml_node DOHeaderExport = Config.append_child(node_element, _T("DataStructHeaderExport"));
		DOHeaderExport.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.DataStructHeaderExportConfig.IsExport);
		DOHeaderExport.append_attribute(_T("FileExt"),
			m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt);
	}

	{
		xml_node InterfaceExportConfig = Config.append_child(node_element, _T("InterfaceExport"));
		InterfaceExportConfig.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.InterfaceExportConfig.IsExport);
		InterfaceExportConfig.append_attribute(_T("FileExt"),
			m_InterfaceConfig.InterfaceExportConfig.ExportExt);
	}

	{
		xml_node CallHeaderExportConfig = Config.append_child(node_element, _T("CallHeaderExport"));
		CallHeaderExportConfig.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.CallHeaderExportConfig.IsExport);
		CallHeaderExportConfig.append_attribute(_T("FileExt"),
			m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
	}

	{
		xml_node CallSourceExportConfig = Config.append_child(node_element, _T("CallSourceExport"));
		CallSourceExportConfig.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.CallSourceExportConfig.IsExport);
		CallSourceExportConfig.append_attribute(_T("FileExt"),
			m_InterfaceConfig.CallSourceExportConfig.ExportExt);
	}

	{
		xml_node HandlerHeaderExportConfig = Config.append_child(node_element, _T("HandlerHeaderExport"));
		HandlerHeaderExportConfig.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.HandlerHeaderExportConfig.IsExport);
		HandlerHeaderExportConfig.append_attribute(_T("FileExt"),
			m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
	}

	{
		xml_node HandlerSourceExportConfig = Config.append_child(node_element, _T("HandlerSourceExport"));
		HandlerSourceExportConfig.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.HandlerSourceExportConfig.IsExport);
		HandlerSourceExportConfig.append_attribute(_T("FileExt"),
			m_InterfaceConfig.HandlerSourceExportConfig.ExportExt);
	}

	{
		xml_node DataObjectHeaderExportConfig = Config.append_child(node_element, _T("DataObjectHeaderExport"));
		DataObjectHeaderExportConfig.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport);
		DataObjectHeaderExportConfig.append_attribute(_T("FileExt"),
			m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);
	}

	{
		xml_node DataObjectSourceExportConfig = Config.append_child(node_element, _T("DataObjectSourceExport"));
		DataObjectSourceExportConfig.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.DataObjectSourceExportConfig.IsExport);
		DataObjectSourceExportConfig.append_attribute(_T("FileExt"),
			m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
	}

	{
		xml_node DataObjectModifyFlagsExport = Config.append_child(node_element, _T("DataObjectModifyFlagsHeaderExport"));
		DataObjectModifyFlagsExport.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.IsExport);
		DataObjectModifyFlagsExport.append_attribute(_T("FileExt"),
			m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt);
	}

	{
		xml_node DataObjectModifyFlagsExport = Config.append_child(node_element, _T("DataObjectModifyFlagsSourceExport"));
		DataObjectModifyFlagsExport.append_attribute(_T("IsExport"),
			(bool)m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.IsExport);
		DataObjectModifyFlagsExport.append_attribute(_T("FileExt"),
			m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.ExportExt);
	}

	{
		xml_node DefaultStructPackOperation = Config.append_child(node_element, _T("DefaultStructGenerateOperations"));

		SaveGenerateOperations(DefaultStructPackOperation, m_InterfaceConfig.DefaultStructGenerateOperations);
	}

	{
		xml_node DefaultEnumGenerateOperations = Config.append_child(node_element, _T("DefaultEnumGenerateOperations"));

		SaveGenerateOperations(DefaultEnumGenerateOperations, m_InterfaceConfig.DefaultEnumGenerateOperations);
	}
	{
		xml_node DefaultEnum64GenerateOperations = Config.append_child(node_element, _T("DefaultEnum64GenerateOperations"));

		SaveGenerateOperations(DefaultEnum64GenerateOperations, m_InterfaceConfig.DefaultEnum64GenerateOperations);
	}
	{
		xml_node ArrayDefineConfig = Config.append_child(node_element, _T("ArrayDefineConfig"));

		SaveGenerateOperations(ArrayDefineConfig, m_InterfaceConfig.ArrayDefineConfig);
	}

	{
		xml_node OtherConfig = Config.append_child(node_element, _T("OtherConfig"));

		OtherConfig.append_attribute(_T("CommentPrefix"),
			m_InterfaceConfig.CommentPrefix);

		OtherConfig.append_attribute(_T("LocalVariableDefinePrefix"),
			m_InterfaceConfig.LocalVariableDefinePrefix);

		OtherConfig.append_attribute(_T("MemberVariablePrefix"),
			m_InterfaceConfig.MemberVariablePrefix);

		OtherConfig.append_attribute(_T("DefaultPacketName"),
			m_InterfaceConfig.DefaultPacketName);

		OtherConfig.append_attribute(_T("DefaultPacketMemberName"),
			m_InterfaceConfig.DefaultPacketMemberName);

		OtherConfig.append_attribute(_T("SupportBigInt"),
			m_InterfaceConfig.SupportBigInt);

		OtherConfig.append_attribute(_T("ForceExportAll"),
			m_InterfaceConfig.ForceExportAll);
	}

	CString FileName = CFileTools::MakeModuleFullPath(NULL, CONFIG_FILE_NAME_XML);
	if (!Xml.SaveToFile(Xml.document(), FileName, CP_UTF8))
	{
		CString Msg;
		Msg.Format(_T("保存文件失败%s"), FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}

bool CCallInterfaceMakerDlg::LoadConfigByJson()
{
	CString FileName = CFileTools::MakeModuleFullPath(NULL, CONFIG_FILE_NAME_JSON);
	CStringFile JsonFile;
	JsonFile.SetLocalCodePage(CP_UTF8);

	if (JsonFile.LoadFile(FileName, false))
	{
		if (JsonFile.GetDataLen())
		{
			rapidjson::Document Root;
			Root.Parse(JsonFile.GetData());
			if (!Root.HasParseError())
			{
				if (Root.IsObject())
				{
					{
						rapidjson::Value& Value = Root[_T("OneFileExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.OneFileExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("InterfaceHeaderExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.InterfaceHeaderExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("StructExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.StructExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("EnumExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.EnumExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("ConstExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.ConstExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("DataStructHeaderExport")];
						if (Value.IsObject())

							LoadExportConfig(Value, m_InterfaceConfig.DataStructHeaderExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("InterfaceExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.InterfaceExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("CallHeaderExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.CallHeaderExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("CallSourceExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.CallSourceExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("HandlerHeaderExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.HandlerHeaderExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("HandlerSourceExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.HandlerSourceExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("DataObjectHeaderExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.DataObjectHeaderExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("DataObjectSourceExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.DataObjectSourceExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("DataObjectModifyFlagsHeaderExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("DataObjectModifyFlagsSourceExport")];
						if (Value.IsObject())
							LoadExportConfig(Value, m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig);
					}
					{
						rapidjson::Value& Value = Root[_T("DefaultStructGenerateOperations")];
						if (Value.IsObject())
							LoadGenerateOperations(Value, m_InterfaceConfig.DefaultStructGenerateOperations);
					}
					{
						rapidjson::Value& Value = Root[_T("DefaultEnumGenerateOperations")];
						if (Value.IsObject())
							LoadGenerateOperations(Value, m_InterfaceConfig.DefaultEnumGenerateOperations);
					}
					{
						rapidjson::Value& Value = Root[_T("DefaultEnum64GenerateOperations")];
						if (Value.IsObject())
							LoadGenerateOperations(Value, m_InterfaceConfig.DefaultEnum64GenerateOperations);
					}
					{
						rapidjson::Value& Value = Root[_T("ArrayDefineConfig")];
						if (Value.IsObject())
							LoadGenerateOperations(Value, m_InterfaceConfig.ArrayDefineConfig);
					}
					{
						rapidjson::Value& Entry = Root[_T("OtherConfig")];
						if (Entry.IsObject())
						{
							GetJsonValue(Entry[_T("CommentPrefix")], m_InterfaceConfig.CommentPrefix);
							GetJsonValue(Entry[_T("LocalVariableDefinePrefix")], m_InterfaceConfig.LocalVariableDefinePrefix);
							GetJsonValue(Entry[_T("MemberVariablePrefix")], m_InterfaceConfig.MemberVariablePrefix);
							GetJsonValue(Entry[_T("DefaultPacketName")], m_InterfaceConfig.DefaultPacketName);
							GetJsonValue(Entry[_T("DefaultPacketMemberName")], m_InterfaceConfig.DefaultPacketMemberName);
							{
								rapidjson::Value& Value = Entry[_T("SupportBigInt")];
								if (Value.IsBool())
								{
									m_InterfaceConfig.SupportBigInt = Value.GetBool();
								}
							}
							{
								rapidjson::Value& Value = Entry[_T("ForceExportAll")];
								if (Value.IsBool())
								{
									m_InterfaceConfig.ForceExportAll = Value.GetBool();
								}
							}
						}
					}
					return true;
				}
				else
				{
					AfxMessageBoxEx(MB_OK, 0, _T("文件格式错误:%s"), (LPCTSTR)FileName);
				}
			}
		}
	}
	else
	{
		AfxMessageBoxEx(MB_OK, 0, _T("打开文件失败:%s"), (LPCTSTR)FileName);
	}
	return false;
}

bool CCallInterfaceMakerDlg::SaveConfigByJson()
{
	rapidjson::Document Root;
	rapidjson::Document::AllocatorType& Alloc = Root.GetAllocator();

	Root.SetObject();

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.OneFileExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.OneFileExportConfig.ExportExt, m_InterfaceConfig.OneFileExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("OneFileExport"), Value, Alloc);
	}
	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt, m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("InterfaceHeaderExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.StructExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.StructExportConfig.ExportExt, m_InterfaceConfig.StructExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("StructExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.EnumExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.EnumExportConfig.ExportExt, m_InterfaceConfig.EnumExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("EnumExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.ConstExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.ConstExportConfig.ExportExt, m_InterfaceConfig.ConstExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("ConstExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.DataStructHeaderExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt, m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("DataStructHeaderExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.InterfaceExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.InterfaceExportConfig.ExportExt, m_InterfaceConfig.InterfaceExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("InterfaceExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.CallHeaderExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.CallHeaderExportConfig.ExportExt, m_InterfaceConfig.CallHeaderExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("CallHeaderExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.CallSourceExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.CallSourceExportConfig.ExportExt, m_InterfaceConfig.CallSourceExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("CallSourceExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.HandlerHeaderExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt, m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("HandlerHeaderExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.HandlerSourceExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.HandlerSourceExportConfig.ExportExt, m_InterfaceConfig.HandlerSourceExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("HandlerSourceExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt, m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("DataObjectHeaderExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.DataObjectSourceExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt, m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("DataObjectSourceExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt, m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("DataObjectModifyFlagsHeaderExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("IsExport"), (bool)m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.IsExport, Alloc);
		Value.AddMember(_T("ExportExt"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.ExportExt, m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.ExportExt.GetLength()), Alloc);
		Root.AddMember(_T("DataObjectModifyFlagsSourceExport"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		SaveGenerateOperations(Value, Alloc, m_InterfaceConfig.DefaultStructGenerateOperations);
		Root.AddMember(_T("DefaultStructGenerateOperations"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		SaveGenerateOperations(Value, Alloc, m_InterfaceConfig.DefaultEnumGenerateOperations);
		Root.AddMember(_T("DefaultEnumGenerateOperations"), Value, Alloc);
	}
	{
		rapidjson::Value Value(rapidjson::kObjectType);
		SaveGenerateOperations(Value, Alloc, m_InterfaceConfig.DefaultEnum64GenerateOperations);
		Root.AddMember(_T("DefaultEnum64GenerateOperations"), Value, Alloc);
	}
	{
		rapidjson::Value Value(rapidjson::kObjectType);
		SaveGenerateOperations(Value, Alloc, m_InterfaceConfig.ArrayDefineConfig);
		Root.AddMember(_T("ArrayDefineConfig"), Value, Alloc);
	}

	{
		rapidjson::Value Value(rapidjson::kObjectType);
		Value.AddMember(_T("CommentPrefix"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.CommentPrefix, m_InterfaceConfig.CommentPrefix.GetLength()), Alloc);
		Value.AddMember(_T("LocalVariableDefinePrefix"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.LocalVariableDefinePrefix, m_InterfaceConfig.LocalVariableDefinePrefix.GetLength()), Alloc);
		Value.AddMember(_T("MemberVariablePrefix"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.MemberVariablePrefix, m_InterfaceConfig.MemberVariablePrefix.GetLength()), Alloc);
		Value.AddMember(_T("DefaultPacketName"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.DefaultPacketName, m_InterfaceConfig.DefaultPacketName.GetLength()), Alloc);
		Value.AddMember(_T("DefaultPacketMemberName"), rapidjson::Value((LPCTSTR)m_InterfaceConfig.DefaultPacketMemberName, m_InterfaceConfig.DefaultPacketMemberName.GetLength()), Alloc);
		Value.AddMember(_T("SupportBigInt"), m_InterfaceConfig.SupportBigInt, Alloc);
		Value.AddMember(_T("ForceExportAll"), m_InterfaceConfig.ForceExportAll, Alloc);
		Root.AddMember(_T("OtherConfig"), Value, Alloc);
	}

#ifdef UNICODE
	rapidjson::GenericStringBuffer<rapidjson::UTF16<> > buffer;
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF16<> >, rapidjson::UTF16<>, rapidjson::UTF16<> > writer(buffer);
#else
	rapidjson::GenericStringBuffer<rapidjson::UTF8<> > buffer;
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<> >, rapidjson::UTF8<>, rapidjson::UTF8<> > writer(buffer);
#endif

	Root.Accept(writer);

	CStringFile JsonFile;

	JsonFile.SetSaveCodePage(CP_UTF8);
	JsonFile.LoadFromString(buffer.GetString(), -1, false);
	CEasyString FileName = CFileTools::MakeModuleFullPath(NULL, CONFIG_FILE_NAME_JSON);
	if (JsonFile.SaveToFile(FileName))
	{
		return true;
	}
	else
	{
		CString Msg;
		Msg.Format(_T("保存文件失败%s"), FileName);
		MessageBox(Msg);
		return false;
	}
}

bool CCallInterfaceMakerDlg::LoadEnvByXML()
{
	xml_parser Parser;

	CString FileName = CFileTools::MakeModuleFullPath(NULL, ENV_FILE_NAME_XML);
	if (Parser.parse_file(FileName, pug::parse_trim_attribute))
	{

		xml_node RunEnvironment = Parser.document();

		if (RunEnvironment.moveto_child(_T("RunEnvironment")))
		{
			if (RunEnvironment.has_attribute(_T("UTF8Export")))
				m_UTF8Export = ((bool)RunEnvironment.attribute(_T("UTF8Export"))) ? TRUE : FALSE;

			{
				xml_node ExportEnv = RunEnvironment;
				if (ExportEnv.moveto_child(_T("ExportEnv")))
				{
					m_RunEnvInfo.RecentExportDir = ((LPCTSTR)ExportEnv.attribute(_T("RecentExportDir")).getvalue());
				}

				xml_node DataEnv = RunEnvironment;
				if (DataEnv.moveto_child(_T("DataEnv")))
				{
					m_RunEnvInfo.RecentOpenPath = ((LPCTSTR)DataEnv.attribute(_T("RecentDataDir")).getvalue());
				}
			}

			xml_node SelectedInterfaces = RunEnvironment;
			if (RunEnvironment.moveto_child(_T("SelectedInterfaces")))
			{
				m_RunEnvInfo.SelectedInterfaces.clear();
				for (UINT i = 0;i < RunEnvironment.children();i++)
				{
					CString InterfaceName = RunEnvironment.child(i).name();
					m_RunEnvInfo.SelectedInterfaces.push_back(InterfaceName);
				}
			}
		}


	}
	else
	{
		CString Msg;
		Msg.Format(_T("无法打开文件%s"), FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}
bool CCallInterfaceMakerDlg::SaveEnvByXML()
{

	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("utf-8");

	xml_node RunEnvironment = Doc.append_child(node_element, _T("RunEnvironment"));

	RunEnvironment.append_attribute(_T("UTF8Export"), m_UTF8Export ? true : false);

	{
		xml_node ExportEnv = RunEnvironment.append_child(node_element, _T("ExportEnv"));
		ExportEnv.append_attribute(_T("RecentExportDir"), m_RunEnvInfo.RecentExportDir);

		xml_node DataEnv = RunEnvironment.append_child(node_element, _T("DataEnv"));
		DataEnv.append_attribute(_T("RecentDataDir"), m_RunEnvInfo.RecentOpenPath);
	}

	xml_node SelectedInterfaces = RunEnvironment.append_child(node_element, _T("SelectedInterfaces"));

	for (size_t i = 0; i < m_RunEnvInfo.SelectedInterfaces.size(); i++)
	{
		SelectedInterfaces.append_child(node_element, m_RunEnvInfo.SelectedInterfaces[i]);
	}

	CString FileName = CFileTools::MakeModuleFullPath(NULL, ENV_FILE_NAME_XML);
	if (!Xml.SaveToFile(Xml.document(), FileName, CP_UTF8))
	{
		CString Msg;
		Msg.Format(_T("保存文件失败%s"), FileName);
		MessageBox(Msg);
		return false;
	}
	return true;
}

bool CCallInterfaceMakerDlg::LoadEnvByJson()
{
	CEasyString FileName = CFileTools::MakeModuleFullPath(NULL, ENV_FILE_NAME_JSON);
	CStringFile JsonFile;
	JsonFile.SetLocalCodePage(CP_UTF8);

	if (JsonFile.LoadFile(FileName, false))
	{
		if (JsonFile.GetDataLen())
		{
			rapidjson::Document Root;
			Root.Parse(JsonFile.GetData());
			if (!Root.HasParseError())
			{
				if (Root.IsObject())
				{
					GetJsonValue(Root[_T("RecentOpenPath")], m_RunEnvInfo.RecentOpenPath);
					GetJsonValue(Root[_T("RecentExportDir")], m_RunEnvInfo.RecentExportDir);
					{
						rapidjson::Value& Value = Root[_T("SelectedInterfaces")];
						if (Value.IsArray())
						{
							for (size_t i = 0; i < Value.Size(); i++)
							{
								rapidjson::Value& Entry = Value[(rapidjson::SizeType)i];
								if (Entry.IsString())
									m_RunEnvInfo.SelectedInterfaces.push_back(Entry.GetString());
							}
						}
					}
					return true;
				}
				else
				{
					AfxMessageBoxEx(MB_OK, 0, _T("文件格式错误:%s"), (LPCTSTR)FileName);
				}
			}
		}
	}
	else
	{
		AfxMessageBoxEx(MB_OK, 0, _T("打开文件失败:%s"), (LPCTSTR)FileName);
	}
	return false;
}
bool CCallInterfaceMakerDlg::SaveEnvByJson()
{
	rapidjson::Document Root;
	rapidjson::Document::AllocatorType& Alloc = Root.GetAllocator();

	Root.SetObject();

	Root.AddMember(_T("RecentOpenPath"), rapidjson::Value((LPCTSTR)m_RunEnvInfo.RecentOpenPath, m_RunEnvInfo.RecentOpenPath.GetLength()), Alloc);
	Root.AddMember(_T("RecentExportDir"), rapidjson::Value((LPCTSTR)m_RunEnvInfo.RecentExportDir, m_RunEnvInfo.RecentExportDir.GetLength()), Alloc);

	{
		rapidjson::Value Value(rapidjson::kArrayType);
		for (size_t i = 0; i < m_RunEnvInfo.SelectedInterfaces.size(); i++)
		{
			Value.PushBack(rapidjson::Value((LPCTSTR)m_RunEnvInfo.SelectedInterfaces[i], m_RunEnvInfo.SelectedInterfaces[i].GetLength()), Alloc);
		}
		Root.AddMember(_T("SelectedInterfaces"), Value, Alloc);
	}

#ifdef UNICODE
	rapidjson::GenericStringBuffer<rapidjson::UTF16<> > buffer;
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF16<> >, rapidjson::UTF16<>, rapidjson::UTF16<> > writer(buffer);
#else
	rapidjson::GenericStringBuffer<rapidjson::UTF8<> > buffer;
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<> >, rapidjson::UTF8<>, rapidjson::UTF8<> > writer(buffer);
#endif

	Root.Accept(writer);

	CStringFile JsonFile;

	JsonFile.SetSaveCodePage(CP_UTF8);
	JsonFile.LoadFromString(buffer.GetString(), -1, false);
	CEasyString FileName = CFileTools::MakeModuleFullPath(NULL, ENV_FILE_NAME_JSON);
	if (JsonFile.SaveToFile(FileName))
	{
		return true;
	}
	else
	{
		CString Msg;
		Msg.Format(_T("保存文件失败%s"), FileName);
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

void CCallInterfaceMakerDlg::GetDataObjectListSorted(vector<BASE_DATA_STRUCT_DEFINE_LIST*>& DataStructList, vector<STRUCT_DEFINE_INFO*>& DataObjectList)
{
	for (size_t i = 0; i < DataStructList.size(); i++)
	{
		if (DataStructList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST* pStructList = (STRUCT_DEFINE_LIST*)DataStructList[i];
			for (size_t j = 0; j < pStructList->StructList.size(); j++)
			{
				if (pStructList->StructList[j].Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				{
					DataObjectList.push_back(&pStructList->StructList[j]);
				}
			}
		}
	}
	qsort(DataObjectList.begin()._Ptr, DataObjectList.size(), sizeof(STRUCT_DEFINE_INFO*), DataObjectComp);
}

int CCallInterfaceMakerDlg::DataObjectComp(LPCVOID p1, LPCVOID p2)
{
	const STRUCT_DEFINE_INFO* pDataObject1 = *((const STRUCT_DEFINE_INFO**)p1);
	const STRUCT_DEFINE_INFO* pDataObject2 = *((const STRUCT_DEFINE_INFO**)p2);
	if (pDataObject1->ObjectID < pDataObject2->ObjectID)
		return -1;
	else if (pDataObject1->ObjectID > pDataObject2->ObjectID)
		return 1;
	else
		return 0;
}

int CCallInterfaceMakerDlg::ModifyFlagComp(LPCVOID p1, LPCVOID p2)
{
	const DATA_OBJECT_MODIFY_FLAG* pInfo1 = (const DATA_OBJECT_MODIFY_FLAG*)p1;
	const DATA_OBJECT_MODIFY_FLAG* pInfo2 = (const DATA_OBJECT_MODIFY_FLAG*)p2;
	return _tcscmp(pInfo1->Name, pInfo2->Name);
}

static int TypeDefineCompare(const void* p1, const void* p2)
{
	TYPE_DEFINE* Type1 = (TYPE_DEFINE*)p1;
	TYPE_DEFINE* Type2 = (TYPE_DEFINE*)p2;
	return _tcscmp(Type1->Name, Type2->Name);
}
void CCallInterfaceMakerDlg::MakeCurVarType(bool IncludeDataObject)
{
	m_CurVarTypeList = m_BaseTypeList;
	for (size_t i = 0; i < m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST* pStructList = (STRUCT_DEFINE_LIST*)m_AllDataStructList[i];
			for (size_t j = 0;j < pStructList->StructList.size();j++)
			{
				//if (pStructList->IgnoreOnExport)
				//	continue;
				if ((pStructList->StructList[j].Flag & STRUCT_FLAG_IS_DATA_OBJECT) == 0 || IncludeDataObject)
				{
					TYPE_DEFINE Type;
					Type.Name = pStructList->StructList[j].Name;
					Type.CType = pStructList->StructList[j].Name;
					Type.SourceListName = pStructList->ListName;
					Type.Flag = TYPE_DEFINE_FLAG_STRUCT;
					if (pStructList->StructList[j].Flag & STRUCT_FLAG_IS_DATA_OBJECT)
						Type.Flag |= TYPE_DEFINE_FLAG_DATA_OBJECT;
					Type.GenerateOperations = pStructList->StructList[j].GenerateOperations;

					if (Type.GenerateOperations.PackOperation.IsEmpty())
						Type.GenerateOperations.PackOperation = m_InterfaceConfig.DefaultStructGenerateOperations.PackOperation;
					if (Type.GenerateOperations.UnpackOperation.IsEmpty())
						Type.GenerateOperations.UnpackOperation = m_InterfaceConfig.DefaultStructGenerateOperations.UnpackOperation;
					if (Type.GenerateOperations.SizeCaculateOperation.IsEmpty())
						Type.GenerateOperations.SizeCaculateOperation = m_InterfaceConfig.DefaultStructGenerateOperations.SizeCaculateOperation;
					if (Type.GenerateOperations.ToXMLOperation.IsEmpty())
						Type.GenerateOperations.ToXMLOperation = m_InterfaceConfig.DefaultStructGenerateOperations.ToXMLOperation;
					if (Type.GenerateOperations.FromXMLOperation.IsEmpty())
						Type.GenerateOperations.FromXMLOperation = m_InterfaceConfig.DefaultStructGenerateOperations.FromXMLOperation;
					if (Type.GenerateOperations.ToJsonOperation.IsEmpty())
						Type.GenerateOperations.ToJsonOperation = m_InterfaceConfig.DefaultStructGenerateOperations.ToJsonOperation;
					if (Type.GenerateOperations.FromJsonOperation.IsEmpty())
						Type.GenerateOperations.FromJsonOperation = m_InterfaceConfig.DefaultStructGenerateOperations.FromJsonOperation;
					if (Type.GenerateOperations.ReferenceDefine.IsEmpty())
						Type.GenerateOperations.ReferenceDefine = m_InterfaceConfig.DefaultStructGenerateOperations.ReferenceDefine;
					if (Type.GenerateOperations.ReferenceUse.IsEmpty())
						Type.GenerateOperations.ReferenceUse = m_InterfaceConfig.DefaultStructGenerateOperations.ReferenceUse;
					if (Type.GenerateOperations.VariableDefine.IsEmpty())
						Type.GenerateOperations.VariableDefine = m_InterfaceConfig.DefaultStructGenerateOperations.VariableDefine;
					if (Type.GenerateOperations.InitOperation.IsEmpty())
						Type.GenerateOperations.InitOperation = m_InterfaceConfig.DefaultStructGenerateOperations.InitOperation;
					if (Type.GenerateOperations.CloneOperation.IsEmpty())
						Type.GenerateOperations.CloneOperation = m_InterfaceConfig.DefaultStructGenerateOperations.CloneOperation;
					if (Type.GenerateOperations.GetMethodDeclare.IsEmpty())
						Type.GenerateOperations.GetMethodDeclare = m_InterfaceConfig.DefaultStructGenerateOperations.GetMethodDeclare;
					if (Type.GenerateOperations.GetMethodDefine.IsEmpty())
						Type.GenerateOperations.GetMethodDefine = m_InterfaceConfig.DefaultStructGenerateOperations.GetMethodDefine;
					if (Type.GenerateOperations.SetMethodDeclare.IsEmpty())
						Type.GenerateOperations.SetMethodDeclare = m_InterfaceConfig.DefaultStructGenerateOperations.SetMethodDeclare;
					if (Type.GenerateOperations.SetMethodDefine.IsEmpty())
						Type.GenerateOperations.SetMethodDefine = m_InterfaceConfig.DefaultStructGenerateOperations.SetMethodDefine;
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
					if (Type.GenerateOperations.FileLogFillOperation.IsEmpty())
						Type.GenerateOperations.FileLogFillOperation = m_InterfaceConfig.DefaultStructGenerateOperations.FileLogFillOperation;
					//if (Type.GenerateOperations.PropertyGridFetchOperation.IsEmpty())
					//	Type.GenerateOperations.PropertyGridFetchOperation = m_InterfaceConfig.DefaultStructGenerateOperations.PropertyGridFetchOperation;
					if (Type.GenerateOperations.AliLogSendOperation.IsEmpty())
						Type.GenerateOperations.AliLogSendOperation = m_InterfaceConfig.DefaultStructGenerateOperations.AliLogSendOperation;
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
					if (Type.GenerateOperations.FormatSpecOperation.IsEmpty())
						Type.GenerateOperations.FormatSpecOperation = m_InterfaceConfig.DefaultStructGenerateOperations.FormatSpecOperation;

					m_CurVarTypeList.push_back(Type);
				}
			}
		}
		else if (m_AllDataStructList[i]->ListType == DATA_STRUCT_ENUM)
		{
			ENUM_DEFINE_LIST* pEnumList = (ENUM_DEFINE_LIST*)m_AllDataStructList[i];
			for (size_t j = 0; j < pEnumList->EnumList.size(); j++)
			{
				//if (pEnumList->IgnoreOnExport)
				//	continue;

				TYPE_DEFINE Type;
				Type.Name = pEnumList->EnumList[j].Name;
				Type.CType = pEnumList->EnumList[j].Name;
				Type.SourceListName = pEnumList->ListName;
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
	if (m_CurVarTypeList.size() > m_BaseTypeList.size())
	{
		qsort(&(m_CurVarTypeList[m_BaseTypeList.size()]), m_CurVarTypeList.size() - m_BaseTypeList.size(),
			sizeof(TYPE_DEFINE), TypeDefineCompare);
	}
}


void CCallInterfaceMakerDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码	
	CFileDialog Dlg(true, _T("*.xml"), _T("*.xml"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("XML Files(*.xml)|*,xml|All Files(*.*)|*.*||"));

	if (!m_RunEnvInfo.RecentOpenPath.IsEmpty())
	{
		Dlg.m_ofn.lpstrInitialDir = CFileTools::GetPathDirectory(m_RunEnvInfo.RecentOpenPath);
	}

	if (Dlg.DoModal() == IDOK)
	{
		if (LoadByXML(Dlg.GetPathName(), m_MainModule))
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


			m_RunEnvInfo.RecentOpenPath = (LPCTSTR)Dlg.GetPathName();
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
		SaveByXML(m_MainModule);
		for (size_t i = 0; i < m_ImportModuleList.size(); i++)
		{
			SaveByXML(m_ImportModuleList[i]);
		}
	}
}

void CCallInterfaceMakerDlg::OnBnClickedButtonSaveAs()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog Dlg(false, _T("*.json"), _T("*.json"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Json Files(*.json)|*,json|All Files(*.*)|*.*||"));

	if (Dlg.DoModal() == IDOK)
	{
		UpdateData(true);
		m_MainModule.ModuleDefineFileName = Dlg.GetPathName();
		SaveByXML(m_MainModule);
		for (size_t i = 0; i < m_ImportModuleList.size(); i++)
		{
			SaveByXML(m_ImportModuleList[i]);
		}

		m_RunEnvInfo.RecentOpenPath = Dlg.GetPathName();
	}
}
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		CString DefaultFolder = GetMainDlg()->GetRunEnv().RecentExportDir;
		if (!DefaultFolder.IsEmpty())
		{
			SendMessage(hwnd, BFFM_SETSELECTION,
				TRUE, (LPARAM)((LPCTSTR)DefaultFolder));
		}
	}
	return 0;
}

void CCallInterfaceMakerDlg::OnBnClickedButtonSaveExport()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);


	BROWSEINFO bi;
	TCHAR FilePath[MAX_PATH];
	LPITEMIDLIST pidl;

	ZeroMemory(&bi, sizeof(bi));

	//bi.pszDisplayName=FilePath;
	bi.lpszTitle = _T("请选择输出目录");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_SHAREABLE;
	bi.lpfn = BrowseCallbackProc;
	pidl = ::SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		::SHGetPathFromIDList(pidl, FilePath);
		IMalloc* pMalloc = NULL;
		::SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);
		pMalloc->Release();
		pMalloc = NULL;

		m_RunEnvInfo.RecentExportDir = FilePath;

		OnBnClickedButtonSave();
		SaveInterfaceSelections();
		SaveEnvByJson();
		MakeCurVarType(true);
		Export(FilePath);
		AfxMessageBox(_T("导出完毕"));

	}



}

void CCallInterfaceMakerDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	SaveConfigByJson();
	if (m_IsLoaded)
		SaveEnvByJson();
	CDialog::OnCancel();
}

void CCallInterfaceMakerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

bool CCallInterfaceMakerDlg::SaveByXML(MODULE_DEFINE_INFO& ModuleInfo)
{
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("utf-8");


	xml_node Interfaces = Doc.append_child(node_element, _T("Interfaces"));
	Interfaces.append_attribute(_T("Version"), _T("2.0.0.0"));
	Interfaces.append_attribute(_T("Name"), (LPCTSTR)ModuleInfo.Name);
	Interfaces.append_attribute(_T("ModuleID"), (UINT)ModuleInfo.ModuleID);
	Interfaces.append_attribute(_T("IDSeed"), (UINT)ModuleInfo.IDSeed);

	xml_node Imports = Interfaces.append_child(node_element, _T("Imports"));
	for (size_t i = 0; i < ModuleInfo.ImportList.size(); i++)
	{
		xml_node Import = Imports.append_child(node_element, _T("Import"));
		Import.append_attribute(_T("FileName"), ModuleInfo.ImportList[i]);
	}

	xml_node GlobalDefine = Interfaces.append_child(node_element, _T("GlobalDefine"));
	for (size_t i = 0; i < ModuleInfo.DataStructDefineList.size(); i++)
	{
		switch (ModuleInfo.DataStructDefineList[i]->ListType)
		{
		case DATA_STRUCT_CONST:
		{
			CONST_DEFINE_LIST* pConstList = (CONST_DEFINE_LIST*)ModuleInfo.DataStructDefineList[i];
			xml_node Consts = GlobalDefine.append_child(node_element, _T("Consts"));
			Consts.append_attribute(_T("Name"), pConstList->ListName);
			Consts.append_attribute(_T("IgnoreOnExport"), pConstList->IgnoreOnExport);
			SaveConstDefineByXML(Consts, pConstList->ConstList);
		}
		break;
		case DATA_STRUCT_ENUM:
		{
			ENUM_DEFINE_LIST* pEnumList = (ENUM_DEFINE_LIST*)ModuleInfo.DataStructDefineList[i];
			xml_node Enums = GlobalDefine.append_child(node_element, _T("Enums"));
			Enums.append_attribute(_T("Name"), pEnumList->ListName);
			Enums.append_attribute(_T("IgnoreOnExport"), pEnumList->IgnoreOnExport);
			SaveEnumDefineByXML(Enums, pEnumList->EnumList);
		}
		break;
		case DATA_STRUCT_STRUCT:
		{
			STRUCT_DEFINE_LIST* pStructList = (STRUCT_DEFINE_LIST*)ModuleInfo.DataStructDefineList[i];
			xml_node Structs = GlobalDefine.append_child(node_element, _T("Structs"));
			Structs.append_attribute(_T("Name"), pStructList->ListName);
			Structs.append_attribute(_T("IgnoreOnExport"), pStructList->IgnoreOnExport);
			SaveStructDefineByXML(Structs, pStructList->StructList);
		}
		break;
		}
	}

	xml_node DataObjectModifyFlags = GlobalDefine.append_child(node_element, _T("DataObjectModifyFlags"));
	SaveDataObjectModifyFlagByXML(DataObjectModifyFlags, ModuleInfo.DataObjectModifyFlags);

	SaveInterfacesByXML(Interfaces, ModuleInfo.InterfaceList);

	xml_node Prefix = Interfaces.append_child(node_element, _T("Prefix"));
	Prefix.append_attribute(_T("Content"), (LPCTSTR)ModuleInfo.InterfacePrefix);

	xml_node Postfix = Interfaces.append_child(node_element, _T("Postfix"));
	Postfix.append_attribute(_T("Content"), (LPCTSTR)ModuleInfo.InterfacePostFix);

	return Xml.SaveToFile(Xml.document(), ModuleInfo.ModuleDefineFileName, CP_UTF8);
}



bool CCallInterfaceMakerDlg::LoadByXML(LPCTSTR FileName, MODULE_DEFINE_INFO& ModuleInfo)
{
	xml_parser Parser;

	if (Parser.parse_file(FileName, pug::parse_trim_attribute))
	{
		ModuleInfo.Clear();
		ModuleInfo.ModuleDefineFileName = FileName;

		xml_node Interfaces = Parser.document();
		if (Interfaces.moveto_child(_T("Interfaces")))
		{

			ModuleInfo.Name = (LPCTSTR)Interfaces.attribute(_T("Name")).getvalue();
			ModuleInfo.Name.Trim();
			ModuleInfo.ModuleID = (UINT)Interfaces.attribute(_T("ModuleID"));
			ModuleInfo.IDSeed = (UINT)Interfaces.attribute(_T("IDSeed"));

			xml_node Imports = Interfaces;
			if (Imports.moveto_child(_T("Imports")))
			{
				for (UINT i = 0; i < Imports.children(); i++)
				{
					if (_tcsicmp(Imports.child(i).get_name(), _T("Import")) == 0)
					{
						CString ImportFileName = Imports.child(i).attribute(_T("FileName")).getvalue();
						if (!ImportFileName.IsEmpty())
							ModuleInfo.ImportList.push_back(ImportFileName);
					}
				}

			}

			xml_node GlobalDefine = Interfaces;
			if (GlobalDefine.moveto_child(_T("GlobalDefine")))
			{
				for (UINT i = 0; i < GlobalDefine.children(); i++)
				{
					if (_tcsicmp(GlobalDefine.child(i).get_name(), _T("Consts")) == 0)
					{
						CONST_DEFINE_LIST* pConstList = new CONST_DEFINE_LIST;
						pConstList->ListType = DATA_STRUCT_CONST;
						pConstList->ModuleID = ModuleInfo.ModuleID;
						pConstList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute(_T("Name")).getvalue();
						pConstList->IgnoreOnExport = GlobalDefine.child(i).attribute(_T("IgnoreOnExport"));
						LoadConstDefineByXML(GlobalDefine.child(i), pConstList->ConstList);
						ModuleInfo.DataStructDefineList.push_back(pConstList);
					}
					else if (_tcsicmp(GlobalDefine.child(i).get_name(), _T("Enums")) == 0)
					{
						ENUM_DEFINE_LIST* pEnumList = new ENUM_DEFINE_LIST;
						pEnumList->ListType = DATA_STRUCT_ENUM;
						pEnumList->ModuleID = ModuleInfo.ModuleID;
						pEnumList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute(_T("Name")).getvalue();
						pEnumList->IgnoreOnExport = GlobalDefine.child(i).attribute(_T("IgnoreOnExport"));
						LoadEnumDefineByXML(GlobalDefine.child(i), pEnumList->EnumList);
						ModuleInfo.DataStructDefineList.push_back(pEnumList);
					}
					else if (_tcsicmp(GlobalDefine.child(i).get_name(), _T("Structs")) == 0)
					{
						STRUCT_DEFINE_LIST* pStructList = new STRUCT_DEFINE_LIST;
						pStructList->ListType = DATA_STRUCT_STRUCT;
						pStructList->ModuleID = ModuleInfo.ModuleID;
						pStructList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute(_T("Name")).getvalue();
						pStructList->IgnoreOnExport = GlobalDefine.child(i).attribute(_T("IgnoreOnExport"));
						LoadStructDefineByXML(GlobalDefine.child(i), pStructList->StructList);
						ModuleInfo.DataStructDefineList.push_back(pStructList);
					}
					else if (_tcsicmp(GlobalDefine.child(i).get_name(), _T("DataObjectModifyFlags")) == 0)
					{
						LoadDataObjectModifyFlagByXML(GlobalDefine.child(i), ModuleInfo.DataObjectModifyFlags);
					}
				}
			}
			LoadInterfacesByXML(Interfaces, ModuleInfo.InterfaceList, ModuleInfo.ModuleID);

			xml_node Prefix = Interfaces;
			if (Prefix.moveto_child(_T("Prefix")))
			{
				ModuleInfo.InterfacePrefix = ((LPCTSTR)Prefix.attribute(_T("Content")).getvalue());
			}
			xml_node Postfix = Interfaces;
			if (Postfix.moveto_child(_T("Postfix")))
			{
				ModuleInfo.InterfacePostFix = ((LPCTSTR)Postfix.attribute(_T("Content")).getvalue());
			}

			//LoadEnv();
			return true;
		}
	}

	return false;
}
bool CCallInterfaceMakerDlg::SaveByJson(MODULE_DEFINE_INFO& ModuleInfo)
{
	rapidjson::Document Root;
	rapidjson::Document::AllocatorType& Alloc = Root.GetAllocator();

	Root.SetObject();

	rapidjson::Value Imports(rapidjson::kArrayType);
	for (CString& FileName : ModuleInfo.ImportList)
	{
		if (IsXMLFile(FileName))
		{
			CString Temp = CFileTools::GetPathWithoutExt(FileName);
			Temp += _T(".json");
			Imports.PushBack(rapidjson::Value((LPCTSTR)Temp, Temp.GetLength()), Alloc);
		}
		else
		{
			Imports.PushBack(rapidjson::Value((LPCTSTR)FileName, FileName.GetLength()), Alloc);
		}		
	}
	Root.AddMember(_T("Imports"), Imports, Alloc);


	rapidjson::Value DataStructs(rapidjson::kObjectType);
	for (BASE_DATA_STRUCT_DEFINE_LIST* pDefineList : ModuleInfo.DataStructDefineList)
	{
		switch (pDefineList->ListType)
		{
		case DATA_STRUCT_CONST:
			{
				CONST_DEFINE_LIST* pConstList = (CONST_DEFINE_LIST*)pDefineList;
				rapidjson::Value Consts(rapidjson::kObjectType);
				Consts.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)pConstList->ListName, pConstList->ListName.GetLength()), Alloc);
				Consts.AddMember(_T("IgnoreOnExport"), pConstList->IgnoreOnExport, Alloc);
				rapidjson::Value List(rapidjson::kArrayType);
				SaveConstDefineByJson(List, Alloc, pConstList->ConstList);
				Consts.AddMember(_T("List"), List, Alloc);
				DataStructs.AddMember(_T("Consts"), Consts, Alloc);
			}
			break;
		case DATA_STRUCT_ENUM:
		{
			ENUM_DEFINE_LIST* pEnumList = (ENUM_DEFINE_LIST*)pDefineList;
			rapidjson::Value Enums(rapidjson::kObjectType);
			Enums.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)pEnumList->ListName, pEnumList->ListName.GetLength()), Alloc);
			Enums.AddMember(_T("IgnoreOnExport"), pEnumList->IgnoreOnExport, Alloc);
			rapidjson::Value List(rapidjson::kArrayType);
			SaveEnumDefineByJson(List, Alloc, pEnumList->EnumList);
			Enums.AddMember(_T("List"), List, Alloc);
			DataStructs.AddMember(_T("Enums"), Enums, Alloc);
		}
		break;
		case DATA_STRUCT_STRUCT:
			{
				STRUCT_DEFINE_LIST* pStructList = (STRUCT_DEFINE_LIST*)pDefineList;
				rapidjson::Value Structs(rapidjson::kObjectType);
				Structs.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)pStructList->ListName, pStructList->ListName.GetLength()), Alloc);
				Structs.AddMember(_T("IgnoreOnExport"), pStructList->IgnoreOnExport, Alloc);
				rapidjson::Value List(rapidjson::kArrayType);
				SaveStructDefineByJson(List, Alloc, pStructList->StructList);
				Structs.AddMember(_T("List"), List, Alloc);
				DataStructs.AddMember(_T("Structs"), Structs, Alloc);
			}
			break;
		}
	}
	Root.AddMember(_T("DataStructs"), DataStructs, Alloc);

	rapidjson::Value DataObjectModifyFlags(rapidjson::kArrayType);
	SaveDataObjectModifyFlagByJson(DataObjectModifyFlags, Alloc, ModuleInfo.DataObjectModifyFlags);
	Root.AddMember(_T("DataObjectModifyFlags"), DataObjectModifyFlags, Alloc);

	rapidjson::Value Interfaces(rapidjson::kArrayType);
	SaveInterfacesByJson(Interfaces, Alloc, ModuleInfo.InterfaceList);
	Root.AddMember(_T("Interfaces"), Interfaces, Alloc);

	Root.AddMember(_T("InterfacePrefix"), rapidjson::Value((LPCTSTR)ModuleInfo.InterfacePrefix, ModuleInfo.InterfacePrefix.GetLength()), Alloc);
	Root.AddMember(_T("InterfacePostFix"), rapidjson::Value((LPCTSTR)ModuleInfo.InterfacePostFix, ModuleInfo.InterfacePostFix.GetLength()), Alloc);

#ifdef UNICODE
	rapidjson::GenericStringBuffer<rapidjson::UTF16<> > buffer;
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF16<> >, rapidjson::UTF16<>, rapidjson::UTF16<> > writer(buffer);
#else
	rapidjson::GenericStringBuffer<rapidjson::UTF8<> > buffer;
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<> >, rapidjson::UTF8<>, rapidjson::UTF8<> > writer(buffer);
#endif

	Root.Accept(writer);

	CStringFile JsonFile;

	JsonFile.SetSaveCodePage(CP_UTF8);
	JsonFile.LoadFromString(buffer.GetString(), -1, false);
	CEasyString FileName = ModuleInfo.ModuleDefineFileName;
	if (IsXMLFile(FileName))
	{
		FileName = CFileTools::GetPathWithoutExt(FileName);
		FileName += _T(".json");
	}
	if (JsonFile.SaveToFile(FileName))
	{
		return true;
	}
	else
	{
		CString Msg;
		Msg.Format(_T("保存文件失败%s"), FileName);
		MessageBox(Msg);
		return false;
	}
}
bool CCallInterfaceMakerDlg::LoadByJson(LPCTSTR FileName, MODULE_DEFINE_INFO& ModuleInfo)
{
	return true;
}
//bool CCallInterfaceMakerDlg::LoadOld(LPCTSTR FileName, INTERFANCE_DEFINES_OLD& ModuleInfo)
//{
//	xml_parser Parser;
//
//	if (Parser.parse_file(FileName, pug::parse_trim_attribute))
//	{
//		ModuleInfo.Clear();
//
//		xml_node Interfaces = Parser.document();
//		if (Interfaces.moveto_child(_T("Interfaces")))
//		{
//
//			ModuleInfo.Name = (LPCTSTR)Interfaces.attribute(_T("Name")).getvalue();
//			ModuleInfo.Name.Trim();
//			ModuleInfo.InterfaceIDSeed = (UINT)Interfaces.attribute(_T("IDSeed"));
//
//			xml_node GlobalDefine = Interfaces;
//			if (GlobalDefine.moveto_child(_T("GlobalDefine")))
//			{
//				for (UINT i = 0; i < GlobalDefine.children(); i++)
//				{
//					if (_tcsicmp(GlobalDefine.child(i).get_name(), _T("Consts")) == 0)
//					{
//						CONST_DEFINE_LIST* pConstList = new CONST_DEFINE_LIST;
//						pConstList->ListType = DATA_STRUCT_CONST;
//						pConstList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute(_T("Name")).getvalue();
//						pConstList->IgnoreOnExport = GlobalDefine.child(i).attribute(_T("IgnoreOnExport"));
//						LoadConstDefine(GlobalDefine.child(i), pConstList->ConstList);
//						ModuleInfo.DataStructDefines.DataStructDefineList.push_back(pConstList);
//					}
//					else if (_tcsicmp(GlobalDefine.child(i).get_name(), _T("Enums")) == 0)
//					{
//						ENUM_DEFINE_LIST* pEnumList = new ENUM_DEFINE_LIST;
//						pEnumList->ListType = DATA_STRUCT_ENUM;
//						pEnumList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute(_T("Name")).getvalue();
//						pEnumList->IgnoreOnExport = GlobalDefine.child(i).attribute(_T("IgnoreOnExport"));
//						LoadEnumDefine(GlobalDefine.child(i), pEnumList->EnumList);
//						ModuleInfo.DataStructDefines.DataStructDefineList.push_back(pEnumList);
//					}
//					else if (_tcsicmp(GlobalDefine.child(i).get_name(), _T("Structs")) == 0)
//					{
//						STRUCT_DEFINE_LIST_OLD* pStructList = new STRUCT_DEFINE_LIST_OLD;
//						pStructList->ListType = DATA_STRUCT_STRUCT;
//						pStructList->ListName = (LPCTSTR)GlobalDefine.child(i).attribute(_T("Name")).getvalue();
//						pStructList->IgnoreOnExport = GlobalDefine.child(i).attribute(_T("IgnoreOnExport"));
//						LoadStructDefineOld(GlobalDefine.child(i), pStructList->StructList);
//						ModuleInfo.DataStructDefines.DataStructDefineList.push_back(pStructList);
//					}
//					else if (_tcsicmp(GlobalDefine.child(i).get_name(), _T("DataObjectModifyFlags")) == 0)
//					{
//						LoadDataObjectModifyFlag(GlobalDefine.child(i), ModuleInfo.DataStructDefines.DataObjectModifyFlags);
//					}
//				}
//			}
//			LoadInterfacesOld(Interfaces, ModuleInfo.InterfaceList);
//
//			xml_node Prefix = Interfaces;
//			if (Prefix.moveto_child(_T("Prefix")))
//			{
//				ModuleInfo.InterfacePrefix = ((LPCTSTR)Prefix.attribute(_T("Content")).getvalue());
//			}
//			xml_node Postfix = Interfaces;
//			if (Postfix.moveto_child(_T("Postfix")))
//			{
//				ModuleInfo.InterfacePostFix = ((LPCTSTR)Postfix.attribute(_T("Content")).getvalue());
//			}
//
//			//LoadEnv();
//			return true;
//		}
//	}
//
//	return false;
//}

//bool CCallInterfaceMakerDlg::LoadOverVersion(LPCTSTR FileName, MODULE_DEFINE_INFO& ModuleInfo)
//{
//	UINT Version = GetFileVersion(FileName);
//	if (Version == 0)
//	{
//		INTERFANCE_DEFINES_OLD OldInterfaceDefines;
//		if (LoadOld(FileName, OldInterfaceDefines))
//		{
//			if (ConvertOldInterfaceDefines(OldInterfaceDefines, ModuleInfo))
//			{
//				ModuleInfo.ModuleDefineFileName = FileName;
//				OldInterfaceDefines.Clear();
//				return true;
//			}
//			else
//			{
//				OldInterfaceDefines.Clear();
//				ModuleInfo.Clear();
//				AfxMessageBox(_T("转换旧版本文件失败"));
//				return false;
//			}
//		}
//		else
//		{
//			OldInterfaceDefines.Clear();
//			CString Msg;
//			Msg.Format(_T("加载失败%s"), FileName);
//			AfxMessageBox(Msg);
//			return false;
//		}
//	}
//	else
//	{
//		if (Load(FileName, ModuleInfo))
//		{
//			return true;
//		}
//		else
//		{
//			ModuleInfo.Clear();
//			CString Msg;
//			Msg.Format(_T("加载失败%s"), FileName);
//			AfxMessageBox(Msg);
//			return false;
//		}
//	}
//}

UINT CCallInterfaceMakerDlg::GetFileVersion(LPCTSTR FileName)
{
	xml_parser Parser;

	if (Parser.parse_file(FileName, pug::parse_trim_attribute))
	{
		xml_node Interfaces = Parser.document();
		if (Interfaces.moveto_child(_T("Interfaces")))
		{

			if (Interfaces.has_attribute(_T("Version")))
			{
				return (UINT)Interfaces.attribute(_T("Version"));
			}
		}
	}
	return 0;
}

MODULE_DEFINE_INFO* CCallInterfaceMakerDlg::LoadModule(LPCTSTR szFileName)
{
	for (size_t i = 0; i < m_ImportModuleList.size(); i++)
	{
		if (_tcsicmp(m_ImportModuleList[i].ModuleDefineFileName, szFileName) == 0)
			return NULL;
	}
	MODULE_DEFINE_INFO ModuleDefine;
	if (LoadByXML(szFileName, ModuleDefine))
	{
		m_ImportModuleList.push_back(ModuleDefine);
		MODULE_DEFINE_INFO* pModuleIndo = m_ImportModuleList.end()._Ptr;
		for (size_t i = 0; i < ModuleDefine.ImportList.size(); i++)
		{
			CEasyString FileName = CFileTools::MakeFullPath(CFileTools::GetPathDirectory(ModuleDefine.ModuleDefineFileName) + DIR_SLASH + (LPCTSTR)ModuleDefine.ImportList[i]);
			LoadModule(FileName);
		}
		return pModuleIndo;
	}
	return NULL;
}

bool CCallInterfaceMakerDlg::SaveInterfacesByXML(xml_node& Interfaces, vector<CALLER_INTERFACE>& InterfaceList)
{
	for (size_t i = 0; i < InterfaceList.size(); i++)
	{
		CALLER_INTERFACE& InterfaceInfo = InterfaceList[i];
		xml_node Interface = Interfaces.append_child(node_element, _T("Interface"));

		Interface.append_attribute(_T("Name"), (LPCTSTR)InterfaceInfo.Name);
		Interface.append_attribute(_T("ID"), (UINT)InterfaceInfo.ID);
		Interface.append_attribute(_T("IDSeed"), (UINT)InterfaceInfo.IDSeed);
		Interface.append_attribute(_T("Description"), (LPCTSTR)InterfaceInfo.Description);

		//xml_node LocalDefine = Interface.append_child(node_element, _T("LocalDefine"));
		//xml_node LocalStructs = LocalDefine.append_child(node_element, _T("Structs"));

		for (size_t j = 0; j < InterfaceInfo.MethodList.size(); j++)
		{
			INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];
			xml_node Method = Interface.append_child(node_element, _T("Method"));

			Method.append_attribute(_T("Name"), (LPCTSTR)MethodInfo.Name);
			Method.append_attribute(_T("Type"), (UINT)MethodInfo.Type);
			Method.append_attribute(_T("ID"), (UINT)MethodInfo.ID);
			Method.append_attribute(_T("Flag"), (UINT)MethodInfo.Flag);
			Method.append_attribute(_T("ParamIDSeed"), (UINT)MethodInfo.ParamIDSeed);
			Method.append_attribute(_T("Description"), (LPCTSTR)MethodInfo.Description);
			for (size_t k = 0; k < MethodInfo.CallParamList.size(); k++)
			{
				METHOD_PARAM& ParamInfo = MethodInfo.CallParamList[k];

				xml_node Param = Method.append_child(node_element, _T("CallParam"));
				CString Temp;


				Param.append_attribute(_T("Name"), (LPCTSTR)ParamInfo.Name);
				Param.append_attribute(_T("Type"), (LPCTSTR)ParamInfo.Type);
				Param.append_attribute(_T("ID"), (UINT)ParamInfo.ID);
				Param.append_attribute(_T("Length"), (UINT)ParamInfo.Length);
				Param.append_attribute(_T("CanNull"), ParamInfo.CanNull);
				Param.append_attribute(_T("IsReference"), ParamInfo.IsReference);
				Param.append_attribute(_T("IsArray"), ParamInfo.IsArray);
				Param.append_attribute(_T("DefaultValue"), (LPCTSTR)ParamInfo.DefaultValue);
				Param.append_attribute(_T("Description"), (LPCTSTR)ParamInfo.Description);
				Param.append_attribute(_T("PackFlag"), (LPCTSTR)ParamInfo.PackFlag);

			}
			for (size_t k = 0; k < MethodInfo.AckParamList.size(); k++)
			{
				METHOD_PARAM& ParamInfo = MethodInfo.AckParamList[k];

				xml_node Param = Method.append_child(node_element, _T("AckParam"));
				CString Temp;


				Param.append_attribute(_T("Name"), (LPCTSTR)ParamInfo.Name);
				Param.append_attribute(_T("Type"), (LPCTSTR)ParamInfo.Type);
				Param.append_attribute(_T("ID"), (UINT)ParamInfo.ID);
				Param.append_attribute(_T("Length"), (UINT)ParamInfo.Length);
				Param.append_attribute(_T("CanNull"), ParamInfo.CanNull);
				Param.append_attribute(_T("IsReference"), ParamInfo.IsReference);
				Param.append_attribute(_T("IsArray"), ParamInfo.IsArray);
				Param.append_attribute(_T("DefaultValue"), (LPCTSTR)ParamInfo.DefaultValue);
				Param.append_attribute(_T("Description"), (LPCTSTR)ParamInfo.Description);
				Param.append_attribute(_T("PackFlag"), (LPCTSTR)ParamInfo.PackFlag);
			}
		}
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadInterfacesByXML(xml_node& Interfaces, vector<CALLER_INTERFACE>& InterfaceList, UINT ModuleID)
{
	for (UINT i = 0; i < Interfaces.children(); i++)
	{
		xml_node Interface = Interfaces.child(i);
		if (_tcsicmp(Interface.name(), _T("Interface")) == 0)
		{
			CALLER_INTERFACE InterfaceInfo;

			InterfaceInfo.Name = (LPCTSTR)Interface.attribute(_T("Name")).getvalue();
			InterfaceInfo.Name.Trim();
			InterfaceInfo.ModuleID = ModuleID;
			InterfaceInfo.ID = (UINT)Interface.attribute(_T("ID"));
			InterfaceInfo.IDSeed = (UINT)Interface.attribute(_T("IDSeed"));
			InterfaceInfo.Description = ((LPCTSTR)Interface.attribute(_T("Description")).getvalue());



			for (UINT j = 0; j < Interface.children(); j++)
			{
				xml_node Method = Interface.child(j);
				if (_tcsicmp(Method.name(), _T("Method")) == 0)
				{
					INTERFACE_METHOD MethodInfo;
					MethodInfo.Name = (LPCTSTR)Method.attribute(_T("Name")).getvalue();
					MethodInfo.Name.Trim();
					MethodInfo.Type = (UINT)Method.attribute(_T("Type"));
					MethodInfo.ID = (UINT)Method.attribute(_T("ID"));
					MethodInfo.Flag = (UINT)Method.attribute(_T("Flag"));
					MethodInfo.ParamIDSeed = (UINT)Method.attribute(_T("ParamIDSeed"));
					MethodInfo.Description = ((LPCTSTR)Method.attribute(_T("Description")).getvalue());

					for (UINT k = 0; k < Method.children(); k++)
					{
						xml_node Param = Method.child(k);
						if (_tcsicmp(Param.name(), _T("CallParam")) == 0)
						{

							METHOD_PARAM ParamInfo;

							ParamInfo.Name = (LPCTSTR)Param.attribute(_T("Name")).getvalue();
							ParamInfo.Name.Trim();

							ParamInfo.Type = ((LPCTSTR)Param.attribute(_T("Type")).getvalue());
							ParamInfo.ID = (UINT)Param.attribute(_T("ID"));
							ParamInfo.Length = (UINT)Param.attribute(_T("Length"));
							ParamInfo.CanNull = Param.attribute(_T("CanNull"));
							ParamInfo.IsReference = Param.attribute(_T("IsReference"));
							ParamInfo.IsArray = Param.attribute(_T("IsArray"));
							ParamInfo.DefaultValue = ((LPCTSTR)Param.attribute(_T("DefaultValue")).getvalue());
							ParamInfo.DefaultValue.Trim();
							ParamInfo.Description = ((LPCTSTR)Param.attribute(_T("Description")).getvalue());
							ParamInfo.PackFlag = ((LPCTSTR)Param.attribute(_T("PackFlag")).getvalue());


							MethodInfo.CallParamList.push_back(ParamInfo);
						}
						else if (_tcsicmp(Param.name(), _T("AckParam")) == 0)
						{

							METHOD_PARAM ParamInfo;

							ParamInfo.Name = (LPCTSTR)Param.attribute(_T("Name")).getvalue();
							ParamInfo.Name.Trim();

							ParamInfo.Type = ((LPCTSTR)Param.attribute(_T("Type")).getvalue());
							ParamInfo.ID = (UINT)Param.attribute(_T("ID"));
							ParamInfo.Length = (UINT)Param.attribute(_T("Length"));
							ParamInfo.CanNull = Param.attribute(_T("CanNull"));
							ParamInfo.IsReference = Param.attribute(_T("IsReference"));
							ParamInfo.IsArray = Param.attribute(_T("IsArray"));
							ParamInfo.DefaultValue = ((LPCTSTR)Param.attribute(_T("DefaultValue")).getvalue());
							ParamInfo.DefaultValue.Trim();
							ParamInfo.Description = ((LPCTSTR)Param.attribute(_T("Description")).getvalue());
							ParamInfo.PackFlag = ((LPCTSTR)Param.attribute(_T("PackFlag")).getvalue());

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

bool CCallInterfaceMakerDlg::SaveInterfacesByJson(rapidjson::Value& JsonNode, rapidjson::Document::AllocatorType& Alloc, vector<CALLER_INTERFACE>& InterfaceList)
{
	for (CALLER_INTERFACE& InterfaceInfo : InterfaceList)
	{
		rapidjson::Value Interface(rapidjson::kObjectType);

		Interface.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)InterfaceInfo.Name, InterfaceInfo.Name.GetLength()), Alloc);
		Interface.AddMember(_T("ID"), InterfaceInfo.ID, Alloc);
		Interface.AddMember(_T("IDSeed"), InterfaceInfo.IDSeed, Alloc);
		Interface.AddMember(_T("Description"), rapidjson::Value((LPCTSTR)InterfaceInfo.Description, InterfaceInfo.Description.GetLength()), Alloc);

		rapidjson::Value Methods(rapidjson::kArrayType);
		for (INTERFACE_METHOD& MethodInfo : InterfaceInfo.MethodList)
		{
			rapidjson::Value Method(rapidjson::kObjectType);

			Method.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)MethodInfo.Name, MethodInfo.Name.GetLength()), Alloc);
			Method.AddMember(_T("Type"), MethodInfo.Type, Alloc);
			Method.AddMember(_T("ID"), MethodInfo.ID, Alloc);
			Method.AddMember(_T("Flag"), MethodInfo.Flag, Alloc);
			Method.AddMember(_T("ParamIDSeed"), MethodInfo.ParamIDSeed, Alloc);
			Method.AddMember(_T("Description"), rapidjson::Value((LPCTSTR)MethodInfo.Description, MethodInfo.Description.GetLength()), Alloc);

			rapidjson::Value CallParams(rapidjson::kArrayType);
			for (METHOD_PARAM& ParamInfo : MethodInfo.CallParamList)
			{
				rapidjson::Value Param(rapidjson::kObjectType);
				SaveMethodParam(Param, Alloc, ParamInfo);
				CallParams.PushBack(Param, Alloc);
			}
			Method.AddMember(_T("CallParams"), CallParams, Alloc);

			rapidjson::Value AckParams(rapidjson::kArrayType);
			for (METHOD_PARAM& ParamInfo : MethodInfo.AckParamList)
			{
				rapidjson::Value Param(rapidjson::kObjectType);
				SaveMethodParam(Param, Alloc, ParamInfo);
				AckParams.PushBack(Param, Alloc);
			}
			Method.AddMember(_T("AckParams"), AckParams, Alloc);

			Methods.PushBack(Method, Alloc);
		}
		Interface.AddMember(_T("Methods"), Methods, Alloc);

		JsonNode.PushBack(Interface, Alloc);
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadInterfacesByJson(rapidjson::Value& JsonNode, vector<CALLER_INTERFACE>& InterfaceList, UINT ModuleID)
{
	if (JsonNode.IsArray())
	{
		for (size_t i = 0; i < JsonNode.Size(); i++)
		{
			rapidjson::Value& Interface = JsonNode[(rapidjson::SizeType)i];
			CALLER_INTERFACE InterfaceInfo;
			GetJsonValue(Interface[_T("Name")], InterfaceInfo.Name);
			GetJsonValue(Interface[_T("ID")], InterfaceInfo.ID);
			GetJsonValue(Interface[_T("IDSeed")], InterfaceInfo.IDSeed);
			GetJsonValue(Interface[_T("Description")], InterfaceInfo.Description);
			{
				rapidjson::Value& Methods = Interface[_T("Methods")];
				if (Methods.IsArray())
				{
					for (size_t j = 0; j < Methods.Size(); j++)
					{
						rapidjson::Value& Method = Methods[(rapidjson::SizeType)j];
						INTERFACE_METHOD MethodInfo;
						GetJsonValue(Method[_T("Name")], MethodInfo.Name);
						GetJsonValue(Method[_T("Type")], MethodInfo.Type);
						GetJsonValue(Method[_T("ID")], MethodInfo.ID);
						GetJsonValue(Method[_T("Flag")], MethodInfo.Flag);
						GetJsonValue(Method[_T("ParamIDSeed")], MethodInfo.ParamIDSeed);
						GetJsonValue(Method[_T("Description")], MethodInfo.Description);
						{
							rapidjson::Value& CallParams = Interface[_T("CallParams")];
							if (CallParams.IsArray())
							{
								for (size_t j = 0; j < CallParams.Size(); j++)
								{
									rapidjson::Value& Param = CallParams[(rapidjson::SizeType)j];
									METHOD_PARAM ParamInfo;
									LoadMethodParam(Param, ParamInfo);
									MethodInfo.CallParamList.push_back(ParamInfo);
								}
							}
						}
						{
							rapidjson::Value& AckParams = Interface[_T("AckParams")];
							if (AckParams.IsArray())
							{
								for (size_t j = 0; j < AckParams.Size(); j++)
								{
									rapidjson::Value& Param = AckParams[(rapidjson::SizeType)j];
									METHOD_PARAM ParamInfo;
									LoadMethodParam(Param, ParamInfo);
									MethodInfo.AckParamList.push_back(ParamInfo);
								}
							}
						}
						InterfaceInfo.MethodList.push_back(MethodInfo);
					}

				}
			}
			InterfaceList.push_back(InterfaceInfo);
		}
		return true;
	}
	return false;
}
//bool CCallInterfaceMakerDlg::LoadInterfacesOld(xml_node& Interfaces, vector<CALLER_INTERFACE_OLD>& InterfaceList)
//{
//	for (UINT i = 0; i < Interfaces.children(); i++)
//	{
//		xml_node Interface = Interfaces.child(i);
//		if (_tcsicmp(Interface.name(), _T("Interface")) == 0)
//		{
//			CALLER_INTERFACE_OLD InterfaceInfo;
//
//			InterfaceInfo.Name = (LPCTSTR)Interface.attribute(_T("Name")).getvalue();
//			InterfaceInfo.Name.Trim();
//			InterfaceInfo.Type = Interface.attribute(_T("Type"));
//			InterfaceInfo.ID = (UINT)Interface.attribute(_T("ID"));
//			InterfaceInfo.IDSeed = (UINT)Interface.attribute(_T("IDSeed"));
//			InterfaceInfo.Description = ((LPCTSTR)Interface.attribute(_T("Description")).getvalue());
//
//
//
//			for (UINT j = 0; j < Interface.children(); j++)
//			{
//				xml_node Method = Interface.child(j);
//				if (_tcsicmp(Method.name(), _T("Method")) == 0)
//				{
//					INTERFACE_METHOD_OLD MethodInfo;
//					MethodInfo.Name = (LPCTSTR)Method.attribute(_T("Name")).getvalue();
//					MethodInfo.Name.Trim();
//					MethodInfo.ID = (UINT)Method.attribute(_T("ID"));
//					MethodInfo.Flag = (UINT)Method.attribute(_T("Flag"));
//					MethodInfo.ParamIDSeed = (UINT)Method.attribute(_T("ParamIDSeed"));
//					MethodInfo.Description = ((LPCTSTR)Method.attribute(_T("Description")).getvalue());
//
//					for (UINT k = 0; k < Method.children(); k++)
//					{
//						xml_node Param = Method.child(k);
//						if (_tcsicmp(Param.name(), _T("Param")) == 0)
//						{
//
//							METHOD_PARAM ParamInfo;
//
//							ParamInfo.Name = (LPCTSTR)Param.attribute(_T("Name")).getvalue();
//							ParamInfo.Name.Trim();
//
//							ParamInfo.Type = ((LPCTSTR)Param.attribute(_T("Type")).getvalue());
//							ParamInfo.ID = (UINT)Param.attribute(_T("ID"));
//							ParamInfo.Length = (UINT)Param.attribute(_T("Length"));
//							ParamInfo.CanNull = Param.attribute(_T("CanNull"));
//							ParamInfo.IsReference = Param.attribute(_T("IsReference"));
//
//							ParamInfo.DefaultValue = ((LPCTSTR)Param.attribute(_T("DefaultValue")).getvalue());
//							ParamInfo.DefaultValue.Trim();
//
//
//							ParamInfo.Description = ((LPCTSTR)Param.attribute(_T("Description")).getvalue());
//
//
//							MethodInfo.ParamList.push_back(ParamInfo);
//						}
//
//					}
//					InterfaceInfo.MethodList.push_back(MethodInfo);
//				}
//			}
//			InterfaceList.push_back(InterfaceInfo);
//		}
//	}
//	return true;
//}

bool CCallInterfaceMakerDlg::SaveStructDefineByXML(xml_node& Structs, vector<STRUCT_DEFINE_INFO>& StructDefineList)
{
	for (size_t i = 0;i < StructDefineList.size();i++)
	{
		xml_node Struct = Structs.append_child(node_element, _T("Struct"));
		Struct.append_attribute(_T("Name"), (LPCTSTR)StructDefineList[i].Name);
		Struct.append_attribute(_T("ShortName"), (LPCTSTR)StructDefineList[i].ShortName);
		Struct.append_attribute(_T("ShowName"), (LPCTSTR)StructDefineList[i].ShowName);
		Struct.append_attribute(_T("BaseStruct"), (LPCTSTR)StructDefineList[i].BaseStruct);
		Struct.append_attribute(_T("Description"), (LPCTSTR)StructDefineList[i].Description);
		Struct.append_attribute(_T("IDSeed"), (UINT)StructDefineList[i].IDSeed);
		Struct.append_attribute(_T("Flag"), (UINT)StructDefineList[i].Flag);
		Struct.append_attribute(_T("ObjectID"), (UINT)StructDefineList[i].ObjectID);

		xml_node GenerateOperations = Struct.append_child(node_element, _T("GenerateOperations"));
		SaveGenerateOperations(GenerateOperations, StructDefineList[i].GenerateOperations);

		for (size_t j = 0;j < StructDefineList[i].MemberList.size();j++)
		{
			xml_node Member = Struct.append_child(node_element, _T("Member"));
			Member.append_attribute(_T("Name"), (LPCTSTR)StructDefineList[i].MemberList[j].Name);
			Member.append_attribute(_T("Type"), (LPCTSTR)StructDefineList[i].MemberList[j].Type);
			Member.append_attribute(_T("DBIndexType"), StructDefineList[i].MemberList[j].DBIndexType);
			Member.append_attribute(_T("Flag"), (UINT)StructDefineList[i].MemberList[j].Flag);
			Member.append_attribute(_T("IsArray"), (bool)StructDefineList[i].MemberList[j].IsArray);
			Member.append_attribute(_T("ArrayStartLength"), (UINT)StructDefineList[i].MemberList[j].ArrayStartLength);
			Member.append_attribute(_T("ArrayGrowLength"), (UINT)StructDefineList[i].MemberList[j].ArrayGrowLength);
			Member.append_attribute(_T("DBLength"), (LPCTSTR)StructDefineList[i].MemberList[j].DBLength);
			Member.append_attribute(_T("ShowName"), (LPCTSTR)StructDefineList[i].MemberList[j].ShowName);
			Member.append_attribute(_T("Description"), (LPCTSTR)StructDefineList[i].MemberList[j].Description);
			Member.append_attribute(_T("PackFlag"), (LPCTSTR)StructDefineList[i].MemberList[j].PackFlag);
			Member.append_attribute(_T("ID"), (UINT)StructDefineList[i].MemberList[j].ID);
			Member.append_attribute(_T("BindData"), (LPCTSTR)StructDefineList[i].MemberList[j].BindData);
			Member.append_attribute(_T("ExtendType"), (LPCTSTR)StructDefineList[i].MemberList[j].ExtendType);
		}
	}


	return true;
}

bool CCallInterfaceMakerDlg::LoadStructDefineByXML(xml_node& Structs, vector<STRUCT_DEFINE_INFO>& StructDefineList)
{
	for (UINT i = 0;i < Structs.children();i++)
	{
		xml_node Struct = Structs.child(i);
		if (_tcsicmp(Struct.name(), _T("Struct")) == 0)
		{
			STRUCT_DEFINE_INFO StructDefine;
			StructDefine.Name = (LPCTSTR)Struct.attribute(_T("Name")).getvalue();
			StructDefine.ShortName = (LPCTSTR)Struct.attribute(_T("ShortName")).getvalue();
			StructDefine.ShowName = (LPCTSTR)Struct.attribute(_T("ShowName")).getvalue();
			StructDefine.BaseStruct = (LPCTSTR)Struct.attribute(_T("BaseStruct")).getvalue();
			StructDefine.Description = ((LPCTSTR)Struct.attribute(_T("Description")).getvalue());
			StructDefine.IDSeed = (UINT)Struct.attribute(_T("IDSeed"));
			StructDefine.Flag = (UINT)Struct.attribute(_T("Flag"));
			StructDefine.ObjectID = (UINT)Struct.attribute(_T("ObjectID"));



			for (UINT j = 0;j < Struct.children();j++)
			{
				xml_node Member = Struct.child(j);
				if (_tcsicmp(Member.name(), _T("Member")) == 0)
				{
					STRUCT_MEMBER_INFO MemberInfo;
					MemberInfo.Name = (LPCTSTR)Member.attribute(_T("Name")).getvalue();
					MemberInfo.Type = ((LPCTSTR)Member.attribute(_T("Type")).getvalue());
					MemberInfo.DBIndexType = Member.attribute(_T("DBIndexType"));
					MemberInfo.Flag = (UINT)Member.attribute(_T("Flag"));
					MemberInfo.IsArray = (bool)Member.attribute(_T("IsArray"));
					MemberInfo.ArrayStartLength = (UINT)Member.attribute(_T("ArrayStartLength"));
					MemberInfo.ArrayGrowLength = (UINT)Member.attribute(_T("ArrayGrowLength"));
					MemberInfo.DBLength = (LPCTSTR)Member.attribute(_T("DBLength")).getvalue();
					MemberInfo.ShowName = ((LPCTSTR)Member.attribute(_T("ShowName")).getvalue());
					MemberInfo.Description = ((LPCTSTR)Member.attribute(_T("Description")).getvalue());
					MemberInfo.PackFlag = ((LPCTSTR)Member.attribute(_T("PackFlag")).getvalue());
					MemberInfo.ID = (UINT)Member.attribute(_T("ID"));
					MemberInfo.BindData = ((LPCTSTR)Member.attribute(_T("BindData")).getvalue());
					MemberInfo.ExtendType = ((LPCTSTR)Member.attribute(_T("ExtendType")).getvalue());
					StructDefine.MemberList.push_back(MemberInfo);
				}
				else if (_tcsicmp(Member.name(), _T("GenerateOperations")) == 0)
				{
					LoadGenerateOperations(Member, StructDefine.GenerateOperations);
				}
			}
			StructDefineList.push_back(StructDefine);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveStructDefineByJson(rapidjson::Value& JsonNode, rapidjson::Document::AllocatorType& Alloc, vector<STRUCT_DEFINE_INFO>& StructDefineList)
{
	for (STRUCT_DEFINE_INFO& StructInfo : StructDefineList)
	{
		rapidjson::Value Struct(rapidjson::kObjectType);

		Struct.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)StructInfo.Name, StructInfo.Name.GetLength()), Alloc);
		Struct.AddMember(_T("ShortName"), rapidjson::Value((LPCTSTR)StructInfo.ShortName, StructInfo.ShortName.GetLength()), Alloc);
		Struct.AddMember(_T("ShowName"), rapidjson::Value((LPCTSTR)StructInfo.ShowName, StructInfo.ShowName.GetLength()), Alloc);
		Struct.AddMember(_T("BaseStruct"), rapidjson::Value((LPCTSTR)StructInfo.BaseStruct, StructInfo.BaseStruct.GetLength()), Alloc);
		Struct.AddMember(_T("Description"), rapidjson::Value((LPCTSTR)StructInfo.Description, StructInfo.Description.GetLength()), Alloc);
		Struct.AddMember(_T("IDSeed"), StructInfo.IDSeed, Alloc);
		Struct.AddMember(_T("Flag"), StructInfo.Flag, Alloc);
		Struct.AddMember(_T("ObjectID"), StructInfo.ObjectID, Alloc);

		rapidjson::Value GenerateOperations(rapidjson::kObjectType);
		SaveGenerateOperations(GenerateOperations, Alloc, StructInfo.GenerateOperations);
		Struct.AddMember(_T("GenerateOperations"), GenerateOperations, Alloc);

		rapidjson::Value Members(rapidjson::kArrayType);
		for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
		{
			rapidjson::Value Member(rapidjson::kObjectType);
			Member.AddMember(_T("ID"), MemberInfo.ID, Alloc);
			Member.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)MemberInfo.Name, MemberInfo.Name.GetLength()), Alloc);
			Member.AddMember(_T("ShowName"), rapidjson::Value((LPCTSTR)MemberInfo.ShowName, MemberInfo.ShowName.GetLength()), Alloc);
			Member.AddMember(_T("Description"), rapidjson::Value((LPCTSTR)MemberInfo.Description, MemberInfo.Description.GetLength()), Alloc);
			Member.AddMember(_T("Type"), rapidjson::Value((LPCTSTR)MemberInfo.Type, MemberInfo.Type.GetLength()), Alloc);
			Member.AddMember(_T("Flag"), MemberInfo.Flag, Alloc);
			Member.AddMember(_T("IsArray"), MemberInfo.IsArray, Alloc);
			Member.AddMember(_T("ArrayStartLength"), MemberInfo.ArrayStartLength, Alloc);
			Member.AddMember(_T("ArrayGrowLength"), MemberInfo.ArrayGrowLength, Alloc);
			Member.AddMember(_T("DBIndexType"), MemberInfo.DBIndexType, Alloc);
			Member.AddMember(_T("DBLength"), rapidjson::Value((LPCTSTR)MemberInfo.DBLength, MemberInfo.DBLength.GetLength()), Alloc);
			Member.AddMember(_T("PackFlag"), rapidjson::Value((LPCTSTR)MemberInfo.PackFlag, MemberInfo.PackFlag.GetLength()), Alloc);
			Member.AddMember(_T("BindData"), rapidjson::Value((LPCTSTR)MemberInfo.BindData, MemberInfo.BindData.GetLength()), Alloc);
			Member.AddMember(_T("ExtendType"), rapidjson::Value((LPCTSTR)MemberInfo.ExtendType, MemberInfo.ExtendType.GetLength()), Alloc);
			Members.PushBack(Member, Alloc);
		}
		Struct.AddMember(_T("Members"), Members, Alloc);
		JsonNode.PushBack(Struct, Alloc);
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadStructDefineByJson(rapidjson::Value& JsonNode, vector<STRUCT_DEFINE_INFO>& StructDefineList)
{
	for (size_t i = 0;i < JsonNode.Size();i++)
	{
		rapidjson::Value& Struct = JsonNode[(rapidjson::SizeType)i];
		if (Struct.IsObject())
		{
			STRUCT_DEFINE_INFO StructInfo;
			GetJsonValue(Struct[_T("Name")], StructInfo.Name);
			GetJsonValue(Struct[_T("ShortName")], StructInfo.ShortName);
			GetJsonValue(Struct[_T("ShowName")], StructInfo.ShowName);
			GetJsonValue(Struct[_T("BaseStruct")], StructInfo.BaseStruct);
			GetJsonValue(Struct[_T("Description")], StructInfo.Description);
			GetJsonValue(Struct[_T("IDSeed")], StructInfo.IDSeed);
			GetJsonValue(Struct[_T("Flag")], StructInfo.Flag);
			GetJsonValue(Struct[_T("ObjectID")], StructInfo.ObjectID);

			rapidjson::Value& GenerateOperations = Struct[_T("GenerateOperations")];
			if (GenerateOperations.IsObject())
				LoadGenerateOperations(GenerateOperations, StructInfo.GenerateOperations);

			rapidjson::Value& Members = Struct[_T("Members")];
			for (size_t j = 0;j < Members.Size();j++)
			{
				rapidjson::Value& Member = Members[(rapidjson::SizeType)j];
				if (Member.IsObject())
				{
					STRUCT_MEMBER_INFO MemberInfo;
					GetJsonValue(Member[_T("ID")], MemberInfo.ID);
					GetJsonValue(Member[_T("Name")], MemberInfo.Name);
					GetJsonValue(Member[_T("ShowName")], MemberInfo.ShowName);
					GetJsonValue(Member[_T("Description")], MemberInfo.Description);
					GetJsonValue(Member[_T("Type")], MemberInfo.Type);
					GetJsonValue(Member[_T("Flag")], MemberInfo.Flag);
					GetJsonValue(Member[_T("IsArray")], MemberInfo.IsArray);
					GetJsonValue(Member[_T("ArrayStartLength")], MemberInfo.ArrayStartLength);
					GetJsonValue(Member[_T("ArrayGrowLength")], MemberInfo.ArrayGrowLength);
					GetJsonValue(Member[_T("DBIndexType")], MemberInfo.DBIndexType);
					GetJsonValue(Member[_T("DBLength")], MemberInfo.DBLength);
					GetJsonValue(Member[_T("PackFlag")], MemberInfo.PackFlag);
					GetJsonValue(Member[_T("BindData")], MemberInfo.BindData);
					GetJsonValue(Member[_T("ExtendType")], MemberInfo.ExtendType);
					StructInfo.MemberList.push_back(MemberInfo);
				}
			}
			StructDefineList.push_back(StructInfo);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveEnumDefineByXML(xml_node& Enums, vector<ENUM_DEFINE_INFO>& EnumDefineList)
{
	for (size_t i = 0;i < EnumDefineList.size();i++)
	{
		xml_node Enum = Enums.append_child(node_element, _T("Enum"));
		Enum.append_attribute(_T("Name"), (LPCTSTR)EnumDefineList[i].Name);
		Enum.append_attribute(_T("ShortName"), (LPCTSTR)EnumDefineList[i].ShortName);
		Enum.append_attribute(_T("ShowName"), (LPCTSTR)EnumDefineList[i].ShowName);
		Enum.append_attribute(_T("Flag"), (UINT)EnumDefineList[i].Flag);
		Enum.append_attribute(_T("Description"), (LPCTSTR)EnumDefineList[i].Description);


		for (size_t j = 0;j < EnumDefineList[i].MemberList.size();j++)
		{
			xml_node Member = Enum.append_child(node_element, _T("Member"));
			Member.append_attribute(_T("Name"), (LPCTSTR)EnumDefineList[i].MemberList[j].Name);
			Member.append_attribute(_T("Value"), (LPCTSTR)EnumDefineList[i].MemberList[j].Value);
			Member.append_attribute(_T("Flag"), EnumDefineList[i].MemberList[j].Flag);
			Member.append_attribute(_T("StrValue"), (LPCTSTR)EnumDefineList[i].MemberList[j].StrValue);
			Member.append_attribute(_T("Description"), (LPCTSTR)EnumDefineList[i].MemberList[j].Description);
			Member.append_attribute(_T("BindDataType"), (LPCTSTR)EnumDefineList[i].MemberList[j].BindDataType);

		}
	}


	return true;
}
bool CCallInterfaceMakerDlg::LoadEnumDefineByXML(xml_node& Enums, vector<ENUM_DEFINE_INFO>& EnumDefineList)
{
	for (UINT i = 0;i < Enums.children();i++)
	{
		xml_node Enum = Enums.child(i);
		if (_tcsicmp(Enum.name(), _T("Enum")) == 0)
		{
			ENUM_DEFINE_INFO EnumDefine;
			EnumDefine.Name = (LPCTSTR)Enum.attribute(_T("Name")).getvalue();
			EnumDefine.ShortName = (LPCTSTR)Enum.attribute(_T("ShortName")).getvalue();
			EnumDefine.ShowName = (LPCTSTR)Enum.attribute(_T("ShowName")).getvalue();
			EnumDefine.Flag = (UINT)Enum.attribute(_T("Flag"));
			EnumDefine.Description = ((LPCTSTR)Enum.attribute(_T("Description")).getvalue());




			for (UINT j = 0;j < Enum.children();j++)
			{
				xml_node Member = Enum.child(j);
				if (_tcsicmp(Member.name(), _T("Member")) == 0)
				{
					ENUM_MEMBER_INFO MemberInfo;
					MemberInfo.Name = (LPCTSTR)Member.attribute(_T("Name")).getvalue();
					MemberInfo.Value = ((LPCTSTR)Member.attribute(_T("Value")).getvalue());
					MemberInfo.Flag = (UINT)Member.attribute(_T("Flag"));
					MemberInfo.StrValue = ((LPCTSTR)Member.attribute(_T("StrValue")).getvalue());
					MemberInfo.Description = ((LPCTSTR)Member.attribute(_T("Description")).getvalue());
					MemberInfo.BindDataType = ((LPCTSTR)Member.attribute(_T("BindDataType")).getvalue());

					EnumDefine.MemberList.push_back(MemberInfo);
				}

			}
			EnumDefineList.push_back(EnumDefine);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveEnumDefineByJson(rapidjson::Value& JsonNode, rapidjson::Document::AllocatorType& Alloc, vector<ENUM_DEFINE_INFO>& EnumDefineList)
{
	for (ENUM_DEFINE_INFO& EnumDefine : EnumDefineList)
	{
		rapidjson::Value Enum(rapidjson::kObjectType);

		Enum.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)EnumDefine.Name, EnumDefine.Name.GetLength()), Alloc);
		Enum.AddMember(_T("ShortName"), rapidjson::Value((LPCTSTR)EnumDefine.ShortName, EnumDefine.ShortName.GetLength()), Alloc);
		Enum.AddMember(_T("ShowName"), rapidjson::Value((LPCTSTR)EnumDefine.ShowName, EnumDefine.ShowName.GetLength()), Alloc);
		Enum.AddMember(_T("Flag"), EnumDefine.Flag, Alloc);
		Enum.AddMember(_T("Description"), rapidjson::Value((LPCTSTR)EnumDefine.Description, EnumDefine.Description.GetLength()), Alloc);

		rapidjson::Value Members(rapidjson::kArrayType);
		for (ENUM_MEMBER_INFO& MemberInfo : EnumDefine.MemberList)
		{
			rapidjson::Value Member(rapidjson::kObjectType);
			Member.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)MemberInfo.Name, MemberInfo.Name.GetLength()), Alloc);
			Member.AddMember(_T("Value"), rapidjson::Value((LPCTSTR)MemberInfo.Value, MemberInfo.Value.GetLength()), Alloc);
			Member.AddMember(_T("StrValue"), rapidjson::Value((LPCTSTR)MemberInfo.StrValue, MemberInfo.StrValue.GetLength()), Alloc);
			Member.AddMember(_T("Description"), rapidjson::Value((LPCTSTR)MemberInfo.Description, MemberInfo.Description.GetLength()), Alloc);
			Member.AddMember(_T("BindDataType"), rapidjson::Value((LPCTSTR)MemberInfo.BindDataType, MemberInfo.BindDataType.GetLength()), Alloc);
			Member.AddMember(_T("Flag"), MemberInfo.Flag, Alloc);
			Members.PushBack(Member, Alloc);
		}
		Enum.AddMember(_T("Members"), Members, Alloc);
		JsonNode.PushBack(Enum, Alloc);
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadEnumDefineByJson(rapidjson::Value& JsonNode, vector<ENUM_DEFINE_INFO>& EnumDefineList)
{
	for (size_t i = 0;i < JsonNode.Size();i++)
	{
		rapidjson::Value& Enum = JsonNode[(rapidjson::SizeType)i];
		if (Enum.IsObject())
		{
			ENUM_DEFINE_INFO EnumDefine;
			GetJsonValue(Enum[_T("Name")], EnumDefine.Name);
			GetJsonValue(Enum[_T("ShortName")], EnumDefine.ShortName);
			GetJsonValue(Enum[_T("ShowName")], EnumDefine.ShowName);
			GetJsonValue(Enum[_T("Flag")], EnumDefine.Flag);
			GetJsonValue(Enum[_T("Description")], EnumDefine.Description);


			rapidjson::Value& Members = Enum[_T("Members")];
			for (size_t j = 0;j < Members.Size();j++)
			{
				rapidjson::Value& Member = Members[(rapidjson::SizeType)j];
				if (Member.IsObject())
				{
					ENUM_MEMBER_INFO MemberInfo;
					GetJsonValue(Member[_T("Name")], MemberInfo.Name);
					GetJsonValue(Member[_T("Value")], MemberInfo.Value);
					GetJsonValue(Member[_T("StrValue")], MemberInfo.StrValue);
					GetJsonValue(Member[_T("Description")], MemberInfo.Description);
					GetJsonValue(Member[_T("BindDataType")], MemberInfo.BindDataType);
					GetJsonValue(Member[_T("Flag")], MemberInfo.Flag);
					EnumDefine.MemberList.push_back(MemberInfo);
				}
			}
			EnumDefineList.push_back(EnumDefine);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveConstDefineByXML(xml_node& Consts, vector<CONST_DEFINE_INFO>& ConstDefineList)
{
	for (size_t i = 0;i < ConstDefineList.size();i++)
	{
		xml_node Const = Consts.append_child(node_element, _T("Const"));
		Const.append_attribute(_T("Name"), (LPCTSTR)ConstDefineList[i].Name);
		Const.append_attribute(_T("Type"), (LPCTSTR)ConstDefineList[i].Type);
		Const.append_attribute(_T("Value"), (LPCTSTR)ConstDefineList[i].Value);
		Const.append_attribute(_T("Description"), (LPCTSTR)ConstDefineList[i].Description);
	}


	return true;
}
bool CCallInterfaceMakerDlg::LoadConstDefineByXML(xml_node& Consts, vector<CONST_DEFINE_INFO>& ConstDefineList)
{
	for (UINT i = 0;i < Consts.children();i++)
	{
		xml_node Enum = Consts.child(i);
		if (_tcsicmp(Enum.name(), _T("Const")) == 0)
		{
			CONST_DEFINE_INFO ConstDefine;
			ConstDefine.Name = (LPCTSTR)Enum.attribute(_T("Name")).getvalue();
			ConstDefine.Type = (LPCTSTR)Enum.attribute(_T("Type")).getvalue();
			ConstDefine.Value = ((LPCTSTR)Enum.attribute(_T("Value")).getvalue());
			ConstDefine.Description = ((LPCTSTR)Enum.attribute(_T("Description")).getvalue());

			ConstDefineList.push_back(ConstDefine);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveConstDefineByJson(rapidjson::Value& JsonNode, rapidjson::Document::AllocatorType& Alloc, vector<CONST_DEFINE_INFO>& ConstDefineList)
{
	for (CONST_DEFINE_INFO& ConstDefine : ConstDefineList)
	{
		rapidjson::Value Const(rapidjson::kObjectType);
		Const.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)ConstDefine.Name, ConstDefine.Name.GetLength()), Alloc);
		Const.AddMember(_T("Type"), rapidjson::Value((LPCTSTR)ConstDefine.Type, ConstDefine.Type.GetLength()), Alloc);
		Const.AddMember(_T("Value"), rapidjson::Value((LPCTSTR)ConstDefine.Value, ConstDefine.Value.GetLength()), Alloc);
		Const.AddMember(_T("Description"), rapidjson::Value((LPCTSTR)ConstDefine.Description, ConstDefine.Description.GetLength()), Alloc);
		JsonNode.PushBack(Const, Alloc);
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadConstDefineByJson(rapidjson::Value& JsonNode, vector<CONST_DEFINE_INFO>& ConstDefineList)
{
	for (size_t i = 0;i < JsonNode.Size();i++)
	{
		rapidjson::Value& Const = JsonNode[(rapidjson::SizeType)i];
		if (Const.IsObject())
		{
			CONST_DEFINE_INFO ConstDefine;
			GetJsonValue(Const[_T("Name")], ConstDefine.Name);
			GetJsonValue(Const[_T("Type")], ConstDefine.Type);
			GetJsonValue(Const[_T("Value")], ConstDefine.Value);
			GetJsonValue(Const[_T("Description")], ConstDefine.Description);
			ConstDefineList.push_back(ConstDefine);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveDataObjectModifyFlagByXML(xml_node& ModifyFlags, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags)
{
	for (size_t i = 0;i < DataObjectModifyFlags.size();i++)
	{
		xml_node ModifyFlag = ModifyFlags.append_child(node_element, _T("ModifyFlag"));
		ModifyFlag.append_attribute(_T("Name"), (LPCTSTR)DataObjectModifyFlags[i].Name);
		for (size_t j = 0;j < DataObjectModifyFlags[i].ModifyFlag.size();j++)
		{
			xml_node Unit = ModifyFlag.append_child(node_element, _T("Unit"));
			Unit.append_attribute(_T("ClassName"), (LPCTSTR)DataObjectModifyFlags[i].ModifyFlag[j].ClassName);
			for (size_t k = 0;k < DataObjectModifyFlags[i].ModifyFlag[j].Members.size();k++)
			{
				xml_node Flag = Unit.append_child(node_element, _T("Flag"));
				Flag.append_attribute(_T("Name"), (LPCTSTR)DataObjectModifyFlags[i].ModifyFlag[j].Members[k]);
			}
		}
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadDataObjectModifyFlagByXML(xml_node& ModifyFlags, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags)
{
	for (UINT i = 0;i < ModifyFlags.children();i++)
	{
		xml_node ModifyFlag = ModifyFlags.child(i);
		if (_tcsicmp(ModifyFlag.name(), _T("ModifyFlag")) == 0)
		{
			DATA_OBJECT_MODIFY_FLAG MF;
			MF.Name = (LPCTSTR)ModifyFlag.attribute(_T("Name")).getvalue();
			for (UINT j = 0;j < ModifyFlag.children();j++)
			{
				xml_node Unit = ModifyFlag.child(j);
				if (_tcsicmp(Unit.name(), _T("Unit")) == 0)
				{
					MODIFY_FLAG_UNIT MFU;
					MFU.ClassName = (LPCTSTR)Unit.attribute(_T("ClassName")).getvalue();
					for (UINT k = 0;k < Unit.children();k++)
					{
						xml_node Flag = Unit.child(k);
						if (_tcsicmp(Flag.name(), _T("Flag")) == 0)
						{
							MFU.Members.push_back((LPCTSTR)Flag.attribute(_T("Name")).getvalue());
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

bool CCallInterfaceMakerDlg::SaveDataObjectModifyFlagByJson(rapidjson::Value& JsonNode, rapidjson::Document::AllocatorType& Alloc, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags)
{
	for (DATA_OBJECT_MODIFY_FLAG& ModifyFlagDefine : DataObjectModifyFlags)
	{
		rapidjson::Value ModifyFlag(rapidjson::kObjectType);
		ModifyFlag.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)ModifyFlagDefine.Name, ModifyFlagDefine.Name.GetLength()), Alloc);

		rapidjson::Value Units(rapidjson::kArrayType);
		for (MODIFY_FLAG_UNIT& UnitInfo : ModifyFlagDefine.ModifyFlag)
		{
			rapidjson::Value Unit(rapidjson::kObjectType);
			Unit.AddMember(_T("ClassName"), rapidjson::Value((LPCTSTR)UnitInfo.ClassName, UnitInfo.ClassName.GetLength()), Alloc);
			rapidjson::Value Flags(rapidjson::kArrayType);
			for (CString& Flag : UnitInfo.Members)
			{
				Flags.PushBack(rapidjson::Value((LPCTSTR)Flag, Flag.GetLength()), Alloc);
			}
			Unit.AddMember(_T("Flags"), Flags, Alloc);
			Units.PushBack(Unit, Alloc);
		}
		ModifyFlag.AddMember(_T("Units"), Units, Alloc);
		JsonNode.PushBack(ModifyFlag, Alloc);
	}
	return true;
}
bool CCallInterfaceMakerDlg::LoadDataObjectModifyFlagByJson(rapidjson::Value& JsonNode, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags)
{
	for (size_t i = 0;i < JsonNode.Size();i++)
	{
		rapidjson::Value& ModifyFlag = JsonNode[(rapidjson::SizeType)i];
		if (ModifyFlag.IsObject())
		{
			DATA_OBJECT_MODIFY_FLAG ModifyFlagDefine;
			GetJsonValue(ModifyFlag[_T("Name")], ModifyFlagDefine.Name);

			rapidjson::Value& Units = ModifyFlag[_T("Units")];
			for (size_t j = 0;j < Units.Size();j++)
			{
				rapidjson::Value& Unit = Units[(rapidjson::SizeType)j];
				if (Unit.IsObject())
				{
					MODIFY_FLAG_UNIT UnitInfo;
					GetJsonValue(Units[_T("ClassName")], UnitInfo.ClassName);

					rapidjson::Value& Flags = ModifyFlag[_T("Flags")];
					for (size_t k = 0;k < Flags.Size();k++)
					{
						rapidjson::Value& Flag = Flags[(rapidjson::SizeType)k];
						if (Flag.IsString())
						{
							UnitInfo.Members.push_back(Flag.GetString());
						}
					}
					ModifyFlagDefine.ModifyFlag.push_back(UnitInfo);
				}
			}
			DataObjectModifyFlags.push_back(ModifyFlagDefine);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::SaveGenerateOperations(xml_node& XmlNode, GENERATE_OPERATIONS_INFO& GenerateOperations)
{
	XmlNode.append_attribute(_T("PackOperation"), (LPCTSTR)GenerateOperations.PackOperation);
	XmlNode.append_attribute(_T("UnpackOperation"), (LPCTSTR)GenerateOperations.UnpackOperation);
	XmlNode.append_attribute(_T("SizeCaculateOperation"), (LPCTSTR)GenerateOperations.SizeCaculateOperation);
	XmlNode.append_attribute(_T("ToXMLOperation"), (LPCTSTR)GenerateOperations.ToXMLOperation);
	XmlNode.append_attribute(_T("FromXMLOperation"), (LPCTSTR)GenerateOperations.FromXMLOperation);
	XmlNode.append_attribute(_T("ToJsonOperation"), (LPCTSTR)GenerateOperations.ToJsonOperation);
	XmlNode.append_attribute(_T("FromJsonOperation"), (LPCTSTR)GenerateOperations.FromJsonOperation);
	XmlNode.append_attribute(_T("ReferenceDefine"), (LPCTSTR)GenerateOperations.ReferenceDefine);
	XmlNode.append_attribute(_T("ReferenceUse"), (LPCTSTR)GenerateOperations.ReferenceUse);
	XmlNode.append_attribute(_T("VariableDefine"), (LPCTSTR)GenerateOperations.VariableDefine);
	XmlNode.append_attribute(_T("InitOperation"), (LPCTSTR)GenerateOperations.InitOperation);
	XmlNode.append_attribute(_T("CloneOperation"), (LPCTSTR)GenerateOperations.CloneOperation);
	XmlNode.append_attribute(_T("GetMethodDeclare"), (LPCTSTR)GenerateOperations.GetMethodDeclare);
	XmlNode.append_attribute(_T("GetMethodDefine"), (LPCTSTR)GenerateOperations.GetMethodDefine);
	XmlNode.append_attribute(_T("SetMethodDeclare"), (LPCTSTR)GenerateOperations.SetMethodDeclare);
	XmlNode.append_attribute(_T("SetMethodDefine"), (LPCTSTR)GenerateOperations.SetMethodDefine);
	XmlNode.append_attribute(_T("IndexOperation"), (LPCTSTR)GenerateOperations.IndexOperation);
	XmlNode.append_attribute(_T("ConstIndexOperation"), (LPCTSTR)GenerateOperations.ConstIndexOperation);
	XmlNode.append_attribute(_T("ToStringOperation"), (LPCTSTR)GenerateOperations.ToStringOperation);
	XmlNode.append_attribute(_T("DBFieldDefineOperation"), (LPCTSTR)GenerateOperations.DBFieldDefineOperation);
	XmlNode.append_attribute(_T("DBInsertFormatOperation"), (LPCTSTR)GenerateOperations.DBInsertFormatOperation);
	XmlNode.append_attribute(_T("DBPutOperation"), (LPCTSTR)GenerateOperations.DBPutOperation);
	XmlNode.append_attribute(_T("DBGetOperation"), (LPCTSTR)GenerateOperations.DBGetOperation);
	XmlNode.append_attribute(_T("FileLogFillOperation"), (LPCTSTR)GenerateOperations.FileLogFillOperation);
	//XmlNode.append_attribute(_T("PropertyGridFetchOperation"), (LPCTSTR)GenerateOperations.PropertyGridFetchOperation);
	XmlNode.append_attribute(_T("AliLogSendOperation"), (LPCTSTR)GenerateOperations.AliLogSendOperation);
	XmlNode.append_attribute(_T("ToLuaOperation"), (LPCTSTR)GenerateOperations.ToLuaOperation);
	XmlNode.append_attribute(_T("FromLuaOperation"), (LPCTSTR)GenerateOperations.FromLuaOperation);
	XmlNode.append_attribute(_T("CreateXLSColumnOperation"), (LPCTSTR)GenerateOperations.CreateXLSColumnOperation);
	XmlNode.append_attribute(_T("CheckXLSColumnOperation"), (LPCTSTR)GenerateOperations.CheckXLSColumnOperation);
	XmlNode.append_attribute(_T("ToXLSOperation"), (LPCTSTR)GenerateOperations.ToXLSOperation);
	XmlNode.append_attribute(_T("FromXLSOperation"), (LPCTSTR)GenerateOperations.FromXLSOperation);
	XmlNode.append_attribute(_T("FormatSpecOperation"), (LPCTSTR)GenerateOperations.FormatSpecOperation);

	return true;
}

bool CCallInterfaceMakerDlg::LoadGenerateOperations(xml_node& XmlNode, GENERATE_OPERATIONS_INFO& GenerateOperations)
{
	GenerateOperations.PackOperation = ((LPCTSTR)XmlNode.attribute(_T("PackOperation")).getvalue());
	GenerateOperations.UnpackOperation = ((LPCTSTR)XmlNode.attribute(_T("UnpackOperation")).getvalue());
	GenerateOperations.SizeCaculateOperation = ((LPCTSTR)XmlNode.attribute(_T("SizeCaculateOperation")).getvalue());
	GenerateOperations.ToXMLOperation = ((LPCTSTR)XmlNode.attribute(_T("ToXMLOperation")).getvalue());
	GenerateOperations.FromXMLOperation = ((LPCTSTR)XmlNode.attribute(_T("FromXMLOperation")).getvalue());
	GenerateOperations.ToJsonOperation = ((LPCTSTR)XmlNode.attribute(_T("ToJsonOperation")).getvalue());
	GenerateOperations.FromJsonOperation = ((LPCTSTR)XmlNode.attribute(_T("FromJsonOperation")).getvalue());
	GenerateOperations.ReferenceDefine = ((LPCTSTR)XmlNode.attribute(_T("ReferenceDefine")).getvalue());
	GenerateOperations.ReferenceUse = ((LPCTSTR)XmlNode.attribute(_T("ReferenceUse")).getvalue());
	GenerateOperations.VariableDefine = ((LPCTSTR)XmlNode.attribute(_T("VariableDefine")).getvalue());
	GenerateOperations.InitOperation = ((LPCTSTR)XmlNode.attribute(_T("InitOperation")).getvalue());
	GenerateOperations.CloneOperation = ((LPCTSTR)XmlNode.attribute(_T("CloneOperation")).getvalue());
	GenerateOperations.GetMethodDeclare = ((LPCTSTR)XmlNode.attribute(_T("GetMethodDeclare")).getvalue());
	GenerateOperations.GetMethodDefine = ((LPCTSTR)XmlNode.attribute(_T("GetMethodDefine")).getvalue());
	GenerateOperations.SetMethodDeclare = ((LPCTSTR)XmlNode.attribute(_T("SetMethodDeclare")).getvalue());
	GenerateOperations.SetMethodDefine = ((LPCTSTR)XmlNode.attribute(_T("SetMethodDefine")).getvalue());
	GenerateOperations.IndexOperation = ((LPCTSTR)XmlNode.attribute(_T("IndexOperation")).getvalue());
	GenerateOperations.ConstIndexOperation = ((LPCTSTR)XmlNode.attribute(_T("ConstIndexOperation")).getvalue());
	GenerateOperations.ToStringOperation = ((LPCTSTR)XmlNode.attribute(_T("ToStringOperation")).getvalue());
	GenerateOperations.DBFieldDefineOperation = ((LPCTSTR)XmlNode.attribute(_T("DBFieldDefineOperation")).getvalue());
	GenerateOperations.DBInsertFormatOperation = ((LPCTSTR)XmlNode.attribute(_T("DBInsertFormatOperation")).getvalue());
	GenerateOperations.DBPutOperation = ((LPCTSTR)XmlNode.attribute(_T("DBPutOperation")).getvalue());
	GenerateOperations.DBGetOperation = ((LPCTSTR)XmlNode.attribute(_T("DBGetOperation")).getvalue());
	GenerateOperations.FileLogFillOperation = ((LPCTSTR)XmlNode.attribute(_T("FileLogFillOperation")).getvalue());
	GenerateOperations.AliLogSendOperation = ((LPCTSTR)XmlNode.attribute(_T("AliLogSendOperation")).getvalue());
	GenerateOperations.ToLuaOperation = ((LPCTSTR)XmlNode.attribute(_T("ToLuaOperation")).getvalue());
	GenerateOperations.FromLuaOperation = ((LPCTSTR)XmlNode.attribute(_T("FromLuaOperation")).getvalue());
	GenerateOperations.CreateXLSColumnOperation = ((LPCTSTR)XmlNode.attribute(_T("CreateXLSColumnOperation")).getvalue());
	GenerateOperations.CheckXLSColumnOperation = ((LPCTSTR)XmlNode.attribute(_T("CheckXLSColumnOperation")).getvalue());
	GenerateOperations.ToXLSOperation = ((LPCTSTR)XmlNode.attribute(_T("ToXLSOperation")).getvalue());
	GenerateOperations.FromXLSOperation = ((LPCTSTR)XmlNode.attribute(_T("FromXLSOperation")).getvalue());
	GenerateOperations.FormatSpecOperation = ((LPCTSTR)XmlNode.attribute(_T("FormatSpecOperation")).getvalue());

	return true;
}

bool CCallInterfaceMakerDlg::SaveGenerateOperations(rapidjson::Value& JsonNode, rapidjson::Document::AllocatorType& Alloc, GENERATE_OPERATIONS_INFO& GenerateOperations)
{
	JsonNode.AddMember(_T("PackOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.PackOperation, GenerateOperations.PackOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("UnpackOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.UnpackOperation, GenerateOperations.UnpackOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("SizeCaculateOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.SizeCaculateOperation, GenerateOperations.SizeCaculateOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("ToXMLOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.ToXMLOperation, GenerateOperations.ToXMLOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("FromXMLOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.FromXMLOperation, GenerateOperations.FromXMLOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("ToJsonOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.ToJsonOperation, GenerateOperations.ToJsonOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("FromJsonOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.FromJsonOperation, GenerateOperations.FromJsonOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("ReferenceDefine"), rapidjson::Value((LPCTSTR)GenerateOperations.ReferenceDefine, GenerateOperations.ReferenceDefine.GetLength()), Alloc);
	JsonNode.AddMember(_T("ReferenceUse"), rapidjson::Value((LPCTSTR)GenerateOperations.ReferenceUse, GenerateOperations.ReferenceUse.GetLength()), Alloc);
	JsonNode.AddMember(_T("VariableDefine"), rapidjson::Value((LPCTSTR)GenerateOperations.VariableDefine, GenerateOperations.VariableDefine.GetLength()), Alloc);
	JsonNode.AddMember(_T("InitOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.InitOperation, GenerateOperations.InitOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("CloneOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.CloneOperation, GenerateOperations.CloneOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("GetMethodDeclare"), rapidjson::Value((LPCTSTR)GenerateOperations.GetMethodDeclare, GenerateOperations.GetMethodDeclare.GetLength()), Alloc);
	JsonNode.AddMember(_T("GetMethodDefine"), rapidjson::Value((LPCTSTR)GenerateOperations.GetMethodDefine, GenerateOperations.GetMethodDefine.GetLength()), Alloc);
	JsonNode.AddMember(_T("SetMethodDeclare"), rapidjson::Value((LPCTSTR)GenerateOperations.SetMethodDeclare, GenerateOperations.SetMethodDeclare.GetLength()), Alloc);
	JsonNode.AddMember(_T("SetMethodDefine"), rapidjson::Value((LPCTSTR)GenerateOperations.SetMethodDefine, GenerateOperations.SetMethodDefine.GetLength()), Alloc);
	JsonNode.AddMember(_T("IndexOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.IndexOperation, GenerateOperations.IndexOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("ConstIndexOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.ConstIndexOperation, GenerateOperations.ConstIndexOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("ToStringOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.ToStringOperation, GenerateOperations.ToStringOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("DBFieldDefineOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.DBFieldDefineOperation, GenerateOperations.DBFieldDefineOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("DBInsertFormatOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.DBInsertFormatOperation, GenerateOperations.DBInsertFormatOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("DBPutOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.DBPutOperation, GenerateOperations.DBPutOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("DBGetOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.DBGetOperation, GenerateOperations.DBGetOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("FileLogFillOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.FileLogFillOperation, GenerateOperations.FileLogFillOperation.GetLength()), Alloc);
	//JsonNode.AddMember(_T("PropertyGridFetchOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.PropertyGridFetchOperation, GenerateOperations.PropertyGridFetchOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("AliLogSendOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.AliLogSendOperation, GenerateOperations.AliLogSendOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("ToLuaOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.ToLuaOperation, GenerateOperations.ToLuaOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("FromLuaOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.FromLuaOperation, GenerateOperations.FromLuaOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("CreateXLSColumnOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.CreateXLSColumnOperation, GenerateOperations.CreateXLSColumnOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("CheckXLSColumnOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.CheckXLSColumnOperation, GenerateOperations.CheckXLSColumnOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("ToXLSOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.ToXLSOperation, GenerateOperations.ToXLSOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("FromXLSOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.FromXLSOperation, GenerateOperations.FromXLSOperation.GetLength()), Alloc);
	JsonNode.AddMember(_T("FormatSpecOperation"), rapidjson::Value((LPCTSTR)GenerateOperations.FormatSpecOperation, GenerateOperations.FormatSpecOperation.GetLength()), Alloc);

	return true;
}
bool CCallInterfaceMakerDlg::LoadGenerateOperations(rapidjson::Value& JsonNode, GENERATE_OPERATIONS_INFO& GenerateOperations)
{
	GetJsonValue(JsonNode[_T("PackOperation")], GenerateOperations.PackOperation);
	GetJsonValue(JsonNode[_T("UnpackOperation")], GenerateOperations.UnpackOperation);
	GetJsonValue(JsonNode[_T("SizeCaculateOperation")], GenerateOperations.SizeCaculateOperation);
	GetJsonValue(JsonNode[_T("ToXMLOperation")], GenerateOperations.ToXMLOperation);
	GetJsonValue(JsonNode[_T("FromXMLOperation")], GenerateOperations.FromXMLOperation);
	GetJsonValue(JsonNode[_T("ToJsonOperation")], GenerateOperations.ToJsonOperation);
	GetJsonValue(JsonNode[_T("FromJsonOperation")], GenerateOperations.FromJsonOperation);
	GetJsonValue(JsonNode[_T("ReferenceDefine")], GenerateOperations.ReferenceDefine);
	GetJsonValue(JsonNode[_T("ReferenceUse")], GenerateOperations.ReferenceUse);
	GetJsonValue(JsonNode[_T("VariableDefine")], GenerateOperations.VariableDefine);
	GetJsonValue(JsonNode[_T("InitOperation")], GenerateOperations.InitOperation);
	GetJsonValue(JsonNode[_T("CloneOperation")], GenerateOperations.CloneOperation);
	GetJsonValue(JsonNode[_T("GetMethodDeclare")], GenerateOperations.GetMethodDeclare);
	GetJsonValue(JsonNode[_T("GetMethodDefine")], GenerateOperations.GetMethodDefine);
	GetJsonValue(JsonNode[_T("SetMethodDeclare")], GenerateOperations.SetMethodDeclare);
	GetJsonValue(JsonNode[_T("SetMethodDefine")], GenerateOperations.SetMethodDefine);
	GetJsonValue(JsonNode[_T("IndexOperation")], GenerateOperations.IndexOperation);
	GetJsonValue(JsonNode[_T("ConstIndexOperation")], GenerateOperations.ConstIndexOperation);
	GetJsonValue(JsonNode[_T("ToStringOperation")], GenerateOperations.ToStringOperation);
	GetJsonValue(JsonNode[_T("DBFieldDefineOperation")], GenerateOperations.DBFieldDefineOperation);
	GetJsonValue(JsonNode[_T("DBInsertFormatOperation")], GenerateOperations.DBInsertFormatOperation);
	GetJsonValue(JsonNode[_T("DBPutOperation")], GenerateOperations.DBPutOperation);
	GetJsonValue(JsonNode[_T("DBGetOperation")], GenerateOperations.DBGetOperation);
	GetJsonValue(JsonNode[_T("FileLogFillOperation")], GenerateOperations.FileLogFillOperation);
	GetJsonValue(JsonNode[_T("AliLogSendOperation")], GenerateOperations.AliLogSendOperation);
	GetJsonValue(JsonNode[_T("ToLuaOperation")], GenerateOperations.ToLuaOperation);
	GetJsonValue(JsonNode[_T("FromLuaOperation")], GenerateOperations.FromLuaOperation);
	GetJsonValue(JsonNode[_T("CreateXLSColumnOperation")], GenerateOperations.CreateXLSColumnOperation);
	GetJsonValue(JsonNode[_T("CheckXLSColumnOperation")], GenerateOperations.CheckXLSColumnOperation);
	GetJsonValue(JsonNode[_T("ToXLSOperation")], GenerateOperations.ToXLSOperation);
	GetJsonValue(JsonNode[_T("FromXLSOperation")], GenerateOperations.FromXLSOperation);
	GetJsonValue(JsonNode[_T("FormatSpecOperation")], GenerateOperations.FormatSpecOperation);
	return true;
}

bool CCallInterfaceMakerDlg::SaveMethodParam(rapidjson::Value& JsonNode, rapidjson::Document::AllocatorType& Alloc, METHOD_PARAM& ParamInfo)
{
	JsonNode.AddMember(_T("Name"), rapidjson::Value((LPCTSTR)ParamInfo.Name, ParamInfo.Name.GetLength()), Alloc);
	JsonNode.AddMember(_T("Type"), rapidjson::Value((LPCTSTR)ParamInfo.Type, ParamInfo.Type.GetLength()), Alloc);
	JsonNode.AddMember(_T("ID"), ParamInfo.ID, Alloc);
	JsonNode.AddMember(_T("Length"), ParamInfo.Length, Alloc);
	JsonNode.AddMember(_T("CanNull"), ParamInfo.CanNull, Alloc);
	JsonNode.AddMember(_T("IsReference"), ParamInfo.IsReference, Alloc);
	JsonNode.AddMember(_T("IsArray"), ParamInfo.IsArray, Alloc);
	JsonNode.AddMember(_T("DefaultValue"), rapidjson::Value((LPCTSTR)ParamInfo.DefaultValue, ParamInfo.DefaultValue.GetLength()), Alloc);
	JsonNode.AddMember(_T("Description"), rapidjson::Value((LPCTSTR)ParamInfo.Description, ParamInfo.Description.GetLength()), Alloc);
	JsonNode.AddMember(_T("PackFlag"), rapidjson::Value((LPCTSTR)ParamInfo.PackFlag, ParamInfo.PackFlag.GetLength()), Alloc);
	return true;
}
bool CCallInterfaceMakerDlg::LoadMethodParam(rapidjson::Value& JsonNode, METHOD_PARAM& ParamInfo)
{
	GetJsonValue(JsonNode[_T("Name")], ParamInfo.Name);
	GetJsonValue(JsonNode[_T("Type")], ParamInfo.Type);
	GetJsonValue(JsonNode[_T("ID")], ParamInfo.ID);
	GetJsonValue(JsonNode[_T("Length")], ParamInfo.Length);
	GetJsonValue(JsonNode[_T("CanNull")], ParamInfo.CanNull);
	GetJsonValue(JsonNode[_T("IsReference")], ParamInfo.IsReference);
	GetJsonValue(JsonNode[_T("IsArray")], ParamInfo.IsArray);
	GetJsonValue(JsonNode[_T("DefaultValue")], ParamInfo.DefaultValue);
	GetJsonValue(JsonNode[_T("Description")], ParamInfo.Description);
	GetJsonValue(JsonNode[_T("PackFlag")], ParamInfo.PackFlag);
	return true;
}

bool CCallInterfaceMakerDlg::LoadExportConfig(rapidjson::Value& JsonNode, EXPORT_CONFIG& ExportConfig)
{
	GetJsonValue(JsonNode[_T("IsExport")], ExportConfig.IsExport);
	GetJsonValue(JsonNode[_T("ExportExt")], ExportConfig.ExportExt);
	return true;
}

//CString CCallInterfaceMakerDlg::EncodeString(CString Str)
//{
//	Str.Replace(_T("&"),_T("&amp;"));
//	Str.Replace(_T("<"),_T("&lt;"));
//	Str.Replace(_T(">"),_T("&gt;"));
//	Str.Replace(_T("\")_T(",")&quot;");
//	Str.Replace(_T("\r"),_T("&rt;"));
//	Str.Replace(_T("\n"),_T("&nl;"));
//	Str.Replace(_T("	"),_T("&tab;"));
//	return Str;
//}
//CString CCallInterfaceMakerDlg::DecodeString(CString Str)
//{
//	Str.Replace(_T("&amp;"),_T("&"));
//	Str.Replace(_T("&lt;"),_T("<"));
//	Str.Replace(_T("&gt;"),_T(">"));
//	Str.Replace(_T("&quot;"),_T("\")");
//	Str.Replace(_T("&rt;"),_T("\r"));
//	Str.Replace(_T("&nl;"),_T("\n"));
//	Str.Replace(_T("&tab;"),_T("	"));
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
			Msg.Format(_T("无法打开文件%s"), OutputFileName);
			MessageBox(Msg);
		}
	}
}



bool CCallInterfaceMakerDlg::EndWith(CString& Str, CString EndStr)
{
	if (Str.GetLength() >= EndStr.GetLength())
	{
		return _tcscmp((LPCTSTR)Str + (Str.GetLength() - EndStr.GetLength()), EndStr) == 0;
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
		//AfxMessageBox(_T("结构依赖搜索层数过多，可能有循环依赖"));
		return;
	}
	if (!StructInfo.BaseStruct.IsEmpty())
	{
		TYPE_DEFINE* pType = FindVarType(StructInfo.BaseStruct);
		if (pType)
		{
			if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
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
		TYPE_DEFINE* pType = FindVarType(StructInfo.MemberList[i].Type);
		if (pType)
		{
			if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
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

void CCallInterfaceMakerDlg::GetDefineDepends(const vector<BASE_DATA_STRUCT_DEFINE_LIST*>& DefineList, const STRUCT_DEFINE_LIST* pStructDefine, vector<CString>& DependList, int Depth)
{
	if (Depth > 32)
	{
		//AfxMessageBox(_T("数据定义依赖搜索层数过多，可能有循环依赖"));
		return;
	}

	vector<CString> StructDependList;
	GetStructForeignDepends(pStructDefine->StructList, StructDependList);

	for (size_t i = 0; i < DefineList.size(); i++)
	{
		if (DefineList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			const STRUCT_DEFINE_LIST* pDefine = (const STRUCT_DEFINE_LIST*)DefineList[i];
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
			TYPE_DEFINE* pType = FindVarType(StructInfo.BaseStruct);
			if (pType)
			{
				if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
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
			TYPE_DEFINE* pType = FindVarType(StructInfo.MemberList[i].Type);
			if (pType)
			{
				if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
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



bool CCallInterfaceMakerDlg::HaveStructs(const STRUCT_DEFINE_LIST* pStructDefine, vector<CString>& StructNameList)
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

CString CCallInterfaceMakerDlg::MakeDefineName(const STRUCT_DEFINE_LIST* pStructDefine)
{
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(pStructDefine->ModuleID);
	if (pModuleInfo)
	{
		return pModuleInfo->Name + _T(":") + pStructDefine->ListName;
	}
	return pStructDefine->ListName;
}

void CCallInterfaceMakerDlg::SortModuleForExport(const vector<MODULE_DEFINE_INFO*>& SrcList, vector<MODULE_DEFINE_INFO*>& DestList)
{
	for (size_t i = 0; i < SrcList.size(); i++)
	{

		const MODULE_DEFINE_INFO* pModuleInfo = (const MODULE_DEFINE_INFO*)SrcList[i];
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
void CCallInterfaceMakerDlg::GetModuleDepends(const vector<MODULE_DEFINE_INFO*>& ModuleList, const MODULE_DEFINE_INFO* pModuleInfo, vector<CString>& DependList, int Depth)
{
	if (Depth > 32)
	{
		//AfxMessageBox(_T("数据定义依赖搜索层数过多，可能有循环依赖"));
		return;
	}
	vector<CString> DefineDependList;
	const vector<BASE_DATA_STRUCT_DEFINE_LIST*>& DefineList = pModuleInfo->DataStructDefineList;
	for (size_t i = 0; i < DefineList.size(); i++)
	{
		if (DefineList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			const STRUCT_DEFINE_LIST* pStructDefine = (const STRUCT_DEFINE_LIST*)DefineList[i];
			GetDefineDepends(m_AllDataStructList, pStructDefine, DefineDependList, 0);
		}
	}

	for (size_t i = 0; i < DefineList.size(); i++)
	{
		if (DefineList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			const STRUCT_DEFINE_LIST* pStructDefine = (const STRUCT_DEFINE_LIST*)DefineList[i];
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

void CCallInterfaceMakerDlg::AddUnique(vector<MODULE_DEFINE_INFO*>& ModuleList, MODULE_DEFINE_INFO* pModuleInfo)
{
	for (size_t i = 0; i < ModuleList.size(); i++)
	{
		if (ModuleList[i] == pModuleInfo)
			return;
	}
	ModuleList.push_back(pModuleInfo);
}

bool CCallInterfaceMakerDlg::HaveDefines(const MODULE_DEFINE_INFO* pModuleInfo, vector<CString>& DefineNameList)
{
	for (size_t i = 0; i < pModuleInfo->DataStructDefineList.size(); i++)
	{
		for (size_t j = 0; j < DefineNameList.size(); j++)
		{
			if (pModuleInfo->DataStructDefineList[i]->ListType == DATA_STRUCT_STRUCT)
			{
				const STRUCT_DEFINE_LIST* pStructDefine = (const STRUCT_DEFINE_LIST*)pModuleInfo->DataStructDefineList[i];
				if (MakeDefineName(pStructDefine) == DefineNameList[j])
					return true;
			}
		}
	}
	return false;
}

void CCallInterfaceMakerDlg::AddDependType(vector<DEPEND_INFO>& DependList, const TYPE_DEFINE* pType)
{
	DATA_STRUCT_TYPE ListType;
	if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
		ListType = DATA_STRUCT_STRUCT;
	else if (pType->Flag & TYPE_DEFINE_FLAG_ENUM)
		ListType = DATA_STRUCT_ENUM;
	else
		return;
	for (DEPEND_INFO& Info : DependList)
	{
		if (Info.ListType == ListType && Info.ListName == pType->SourceListName)
		{
			for (const TYPE_DEFINE* pInfo : Info.DependTypes)
			{
				if (pInfo->Name == pType->Name)
					return;
			}
			Info.DependTypes.push_back(pType);
			return;
		}
	}
	DEPEND_INFO Info;
	Info.ListType = ListType;
	Info.ListName = pType->SourceListName;
	Info.DependTypes.push_back(pType);
	DependList.push_back(Info);
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
		if (!EndWith(OldInterfaceInfo.Name, _T("Ack")))
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

				INTERFACE_METHOD_OLD* pAckMethodInfo = FindInterfaceMethod(OldModuleInfo, OldInterfaceInfo.Name, OldMethodInfo.Name, true);
				if (pAckMethodInfo)
				{
					MethodInfo.AckParamList = pAckMethodInfo->ParamList;
				}
				else
				{
					MethodInfo.Type = INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT;
					CString Msg;
					Msg.Format(_T("无法找到接口%s方法%s对应的回调方法"), OldInterfaceInfo.Name, OldMethodInfo.Name);
					PrintImportantLog(_T("ConvertWarnning"), _T("%s"), Msg);
					WarningCount++;
				}


			}

			CALLER_INTERFACE_OLD* pAckInterfaceInfo = FindInterface(OldModuleInfo, OldInterfaceInfo.Name, true);
			if (pAckInterfaceInfo)
			{
				for (size_t j = 0; j < pAckInterfaceInfo->MethodList.size(); j++)
				{
					INTERFACE_METHOD_OLD& OldMethodInfo = pAckInterfaceInfo->MethodList[j];
					if (EndWith(OldMethodInfo.Name, _T("Notify")))
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
						if (EndWith(MethodName, _T("Ack")))
						{
							MethodName = MethodName.Left(MethodName.GetLength() - 3);
						}

						INTERFACE_METHOD_OLD* pCallMethod = FindMethod(OldInterfaceInfo.MethodList, MethodName, false);
						if (pCallMethod == NULL)
						{
							CString Msg;
							Msg.Format(_T("无法找到接口%s回调方法%s对应的调用方法"), OldInterfaceInfo.Name, OldMethodInfo.Name);
							PrintImportantLog(_T("ConvertError"), _T("%s"), Msg);
							ErrorCount++;
						}
					}
				}
			}
			else
			{
				CString Msg;
				Msg.Format(_T("无法找到接口%s对应的回调接口"), OldInterfaceInfo.Name);
				PrintImportantLog(_T("ConvertError"), _T("%s"), Msg);
				ErrorCount++;
			}
			ModuleInfo.InterfaceList.push_back(InterfaceInfo);
		}

	}
	ModuleInfo.DataObjectModifyFlags = OldModuleInfo.DataStructDefines.DataObjectModifyFlags;
	ModuleInfo.DataStructDefineList.resize(OldModuleInfo.DataStructDefines.DataStructDefineList.size());
	for (size_t i = 0; i < OldModuleInfo.DataStructDefines.DataStructDefineList.size(); i++)
	{
		BASE_DATA_STRUCT_DEFINE_LIST* pBaseDefine = OldModuleInfo.DataStructDefines.DataStructDefineList[i];

		switch (pBaseDefine->ListType)
		{
		case DATA_STRUCT_CONST:
		case DATA_STRUCT_ENUM:
			ModuleInfo.DataStructDefineList[i] = pBaseDefine;
			OldModuleInfo.DataStructDefines.DataStructDefineList[i] = NULL;
			break;
		case DATA_STRUCT_STRUCT:
		{
			STRUCT_DEFINE_LIST_OLD* pOldStructList = (STRUCT_DEFINE_LIST_OLD*)pBaseDefine;
			STRUCT_DEFINE_LIST* pStructList = new STRUCT_DEFINE_LIST();
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
			Msg.Format(_T("数据结构[%s]的类型[%d]错误"), pBaseDefine->ListName, pBaseDefine->ListType);
			PrintImportantLog(_T("ConvertError"), _T("%s"), Msg);
			ErrorCount++;
		}
		}
	}
	if (WarningCount || ErrorCount)
	{
		CString Msg;
		Msg.Format(_T("从旧版转换过程中发现%d个警告,%d错误"), WarningCount, ErrorCount);
		AfxMessageBox(Msg);
	}

	return true;
}

INTERFACE_METHOD_OLD* CCallInterfaceMakerDlg::FindInterfaceMethod(INTERFANCE_DEFINES_OLD& OldInterfaceDefines, CString InterfaceName, LPCTSTR MethodName, bool FindAck)
{
	if (FindAck)
		InterfaceName += _T("Ack");

	for (size_t i = 0; i < OldInterfaceDefines.InterfaceList.size(); i++)
	{
		CALLER_INTERFACE_OLD& OldInterfaceInfo = OldInterfaceDefines.InterfaceList[i];
		if (OldInterfaceInfo.Name == InterfaceName)
		{
			INTERFACE_METHOD_OLD* pMethodInfo = FindMethod(OldInterfaceInfo.MethodList, MethodName, FindAck);
			if (pMethodInfo)
				return pMethodInfo;
		}
	}

	return NULL;
}

CALLER_INTERFACE_OLD* CCallInterfaceMakerDlg::FindInterface(INTERFANCE_DEFINES_OLD& OldInterfaceDefines, CString InterfaceName, bool FindAck)
{
	if (FindAck)
		InterfaceName += _T("Ack");
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

INTERFACE_METHOD_OLD* CCallInterfaceMakerDlg::FindMethod(vector<INTERFACE_METHOD_OLD>& OldMethodList, CString MethodName, bool FindAck)
{
	if (FindAck)
		MethodName += _T("Ack");
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
		AfxMessageBox(_T("数据定义排序错误"));
		return false;
	}

	if (m_InterfaceConfig.InterfaceHeaderExportConfig.IsExport)
	{
		CString FileName;

		FileName.Format(_T("%sInterfaces.%s"), (LPCTSTR)m_MainModule.Name, (LPCTSTR)m_InterfaceConfig.InterfaceHeaderExportConfig.ExportExt);
		ExportInterfaceHeader(m_AllInterfaceList, ExportDir, FileName);
	}

	if (m_InterfaceConfig.DataStructHeaderExportConfig.IsExport)
	{
		CString FileName;

		FileName.Format(_T("%sDataStructs.%s"), (LPCTSTR)m_MainModule.Name, (LPCTSTR)m_InterfaceConfig.DataStructHeaderExportConfig.ExportExt);
		ExportDataStructDefineHeader(ConstList, EnumList, StructList, ExportDir, FileName, m_MainModule.Name);

	}

	if (m_InterfaceConfig.EnumExportConfig.IsExport)
	{
		CString Temp;
		Temp.Format(_T("%sSystemEnums.%s"), (LPCTSTR)m_MainModule.Name, (LPCTSTR)m_InterfaceConfig.EnumExportConfig.ExportExt);
		ExportSystemEnums(ExportDir, Temp, m_AllModuleList);
	}

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
		CString LineSpace = GetLineSpace(Source, _T("<AllDefines>"));
		Source.Replace(_T("<SpaceName>"), m_MainModule.Name);
		CString AllDefines;
		IncSpace(m_OneFileExportCache.GetStr(), AllDefines, LineSpace);
		Source.Replace(_T("<AllDefines>"), AllDefines);


		CStringFile OutputFile;
		if (m_UTF8Export)
			OutputFile.SetSaveCodePage(CP_UTF8);
		CString OutputFileName;
		OutputFileName.Format(_T("%s\\%sAllDefines.%s"), ExportDir, (LPCTSTR)m_MainModule.Name, (LPCTSTR)m_InterfaceConfig.OneFileExportConfig.ExportExt);
		if (!OutputFile.SaveToFile(Source, OutputFileName))
		{
			CString Msg;
			Msg.Format(_T("无法打开文件%s"), OutputFileName);
			MessageBox(Msg);
		}
	}

	return true;
}

bool CCallInterfaceMakerDlg::ExportDataStructDefineHeader(vector<CONST_DEFINE_LIST*>& ConstList, vector<ENUM_DEFINE_LIST*>& EnumList, vector<STRUCT_DEFINE_LIST2*>& StructList,
	LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName)
{
	CString OutputFileName;

	OutputFileName = ExportDir;
	OutputFileName += _T("\\");
	OutputFileName += FileName;


	CString Source = m_InterfaceHeaderTemplate;

	CString Includes;
	CString Include;
	CString ImportFileName;

	Include = m_ImportDeclareTemplate;
	ImportFileName.Format(_T("%sSystemEnums.%s"), SpaceName, m_InterfaceConfig.EnumExportConfig.ExportExt);
	Include.Replace(_T("<ImportFileName>"), ImportFileName);
	Includes += Include;

	if (m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.IsExport)
	{
		Include = m_ImportDeclareTemplate;
		ImportFileName.Format(_T("DataObjectModifyFlags.%s"), m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt);
		Include.Replace(_T("<ImportFileName>"), ImportFileName);
		Includes += Include;
	}

	if (m_InterfaceConfig.ConstExportConfig.IsExport)
	{
		for (CONST_DEFINE_LIST* pList : ConstList)
		{
			if (pList->IgnoreOnExport && (!m_InterfaceConfig.ForceExportAll))
				continue;
			CString ModuleName = GetModuleName(pList->ModuleID);
			Include = m_ImportDeclareTemplate;
			ImportFileName.Format(_T("%s%sConsts.%s"), ModuleName, pList->ListName, m_InterfaceConfig.ConstExportConfig.ExportExt);
			Include.Replace(_T("<ImportFileName>"), ImportFileName);
			Includes += Include;
		}
	}

	if (m_InterfaceConfig.EnumExportConfig.IsExport)
	{
		for (ENUM_DEFINE_LIST* pList : EnumList)
		{
			if (pList->IgnoreOnExport && (!m_InterfaceConfig.ForceExportAll))
				continue;
			CString ModuleName = GetModuleName(pList->ModuleID);
			Include = m_ImportDeclareTemplate;
			ImportFileName.Format(_T("%s%sEnums.%s"), ModuleName, pList->ListName, m_InterfaceConfig.EnumExportConfig.ExportExt);
			Include.Replace(_T("<ImportFileName>"), ImportFileName);
			Includes += Include;
		}
	}

	if (m_InterfaceConfig.StructExportConfig.IsExport)
	{
		for (STRUCT_DEFINE_LIST2* pList : StructList)
		{
			if (pList->IgnoreOnExport && (!m_InterfaceConfig.ForceExportAll))
				continue;

			CString ModuleName = GetModuleName(pList->ModuleID);

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format(_T("%s%sStructs.%s"), ModuleName, pList->ListName, m_InterfaceConfig.StructExportConfig.ExportExt);
			Include.Replace(_T("<ImportFileName>"), ImportFileName);
			Includes += Include;

			for (STRUCT_DEFINE_INFO* pInfo : pList->StructList)
			{
				if (pInfo->Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				{
					Include = m_ImportDeclareTemplate;
					ImportFileName.Format(_T("%s.%s"), ClassNameToFileName(pInfo->Name),
						m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport ?
						m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt : m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
					Include.Replace(_T("<ImportFileName>"), ImportFileName);
					Includes += Include;
				}
			}
		}
	}


	Source.Replace(_T("<Prefix>"), _T(""));
	Source.Replace(_T("<Postfix>"), _T(""));

	Source.Replace(_T("<Includes>"), Includes);

	WriteStringToFile(OutputFileName, Source);

	return false;
}

bool CCallInterfaceMakerDlg::ExportInterfaceHeader(vector<CALLER_INTERFACE*>& InterfaceList, LPCTSTR ExportDir, LPCTSTR FileName)
{
	CString OutputFileName;

	OutputFileName = ExportDir;
	OutputFileName += _T("\\");
	OutputFileName += FileName;


	CString ProtocolFile = m_InterfaceHeaderTemplate;

	ProtocolFile.Replace(_T("<Prefix>"), _T(""));
	ProtocolFile.Replace(_T("<Postfix>"), _T(""));

	CString Includes;
	CString Include;
	CString ImportFileName;

	for (size_t i = 0; i < InterfaceList.size(); i++)
	{
		CALLER_INTERFACE* pInterfaceInfo = InterfaceList[i];
		if (IsListItemChecked(pInterfaceInfo->ModuleID, pInterfaceInfo->ID))
		{
			Include = m_ImportDeclareTemplate;
			ImportFileName.Format(_T("%sInterface.%s"), pInterfaceInfo->Name,
				m_InterfaceConfig.InterfaceExportConfig.ExportExt);
			Include.Replace(_T("<ImportFileName>"), ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format(_T("%sMsgCaller.%s"), pInterfaceInfo->Name,
				m_InterfaceConfig.CallHeaderExportConfig.ExportExt.IsEmpty() ?
				m_InterfaceConfig.CallSourceExportConfig.ExportExt : m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
			Include.Replace(_T("<ImportFileName>"), ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format(_T("%sMsgHandler.%s"), pInterfaceInfo->Name,
				m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt.IsEmpty() ?
				m_InterfaceConfig.HandlerSourceExportConfig.ExportExt : m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
			Include.Replace(_T("<ImportFileName>"), ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format(_T("%sAckInterface.%s"), pInterfaceInfo->Name,
				m_InterfaceConfig.InterfaceExportConfig.ExportExt);
			Include.Replace(_T("<ImportFileName>"), ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format(_T("%sAckMsgCaller.%s"), pInterfaceInfo->Name,
				m_InterfaceConfig.CallHeaderExportConfig.ExportExt.IsEmpty() ?
				m_InterfaceConfig.CallSourceExportConfig.ExportExt : m_InterfaceConfig.CallHeaderExportConfig.ExportExt);
			Include.Replace(_T("<ImportFileName>"), ImportFileName);
			Includes += Include;

			Include = m_ImportDeclareTemplate;
			ImportFileName.Format(_T("%sAckMsgHandler.%s"), pInterfaceInfo->Name,
				m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt.IsEmpty() ?
				m_InterfaceConfig.HandlerSourceExportConfig.ExportExt : m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);
			Include.Replace(_T("<ImportFileName>"), ImportFileName);
			Includes += Include;

			Includes += _T("\r\n");


		}
	}

	ProtocolFile.Replace(_T("<Includes>"), Includes);

	WriteStringToFile(OutputFileName, ProtocolFile);

	return false;
}

bool CCallInterfaceMakerDlg::ExportStructs(LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName, STRUCT_DEFINE_LIST2& StructList)
{
	CString OutputFileName;

	OutputFileName = ExportDir;
	OutputFileName += _T("\\");
	OutputFileName += FileName;


	CString DefineHeader = m_StructDefineHeaderTemplate;

	if (DefineHeader.Find(_T("<DependImports>")) >= 0)
	{
		CString LineSpace = GetLineSpace(DefineHeader, _T("<DependImports>"));
		CString Imports = MakeStructDependImports(StructList, SpaceName, LineSpace);
		DefineHeader.Replace(_T("<DependImports>"), Imports);
	}

	DefineHeader.Replace(_T("<SpaceName>"), SpaceName);


	CString LineSpace = GetLineSpace(DefineHeader, _T("<Structs>"));
	CString Structs = MakeStructDefines(StructList.StructList, SpaceName, LineSpace);
	DefineHeader.Replace(_T("<Structs>"), Structs);

	WriteStringToFile(OutputFileName, DefineHeader);

	return false;
}

bool CCallInterfaceMakerDlg::ExportSystemEnums(LPCTSTR ExportDir, LPCTSTR FileName, vector<MODULE_DEFINE_INFO*>& AllModuleList)
{
	vector<ENUM_DEFINE_INFO> SystemEnumList;

	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name = _T("MODULE_IDS");
	EnumInfo.MemberList.resize(AllModuleList.size());
	for (size_t i = 0; i < AllModuleList.size(); i++)
	{
		EnumInfo.MemberList[i].Name = _T("MODULE_ID_") + ClassNameToUpper(AllModuleList[i]->Name);
		EnumInfo.MemberList[i].Value.Format(_T("%d"), AllModuleList[i]->ModuleID);
	}
	SystemEnumList.push_back(EnumInfo);


	EnumInfo.Name = _T("GET_MODIFY_FLAG_TYPE");
	EnumInfo.MemberList.resize(3);
	EnumInfo.MemberList[0].Name = _T("GMFT_COPY");
	EnumInfo.MemberList[0].Value.Empty();
	EnumInfo.MemberList[1].Name = _T("GMFT_AND");
	EnumInfo.MemberList[1].Value.Empty();
	EnumInfo.MemberList[2].Name = _T("GMFT_OR");
	EnumInfo.MemberList[2].Value.Empty();
	SystemEnumList.push_back(EnumInfo);

	vector<STRUCT_DEFINE_INFO*> DataObjectList;
	GetDataObjectListSorted(m_AllDataStructList, DataObjectList);

	EnumInfo.MemberList.clear();
	MakeDataObjectSSTIDsEnum(DataObjectList, EnumInfo);
	SystemEnumList.push_back(EnumInfo);

	EnumInfo.MemberList.clear();
	MakeDataObjectFlagIndicesEnum(DataObjectList, EnumInfo);
	SystemEnumList.push_back(EnumInfo);

	for (size_t j = 0; j < AllModuleList.size(); j++)
	{
		if (AllModuleList[j]->InterfaceList.size())
		{
			EnumInfo.Name.Format(_T("%s_INTERFACES"), ClassNameToUpper(AllModuleList[j]->Name));
			EnumInfo.MemberList.resize(AllModuleList[j]->InterfaceList.size());
			for (size_t i = 0; i < AllModuleList[j]->InterfaceList.size(); i++)
			{
				EnumInfo.MemberList[i].Name.Format(_T("%s_INTERFACE_%s"), ClassNameToUpper(AllModuleList[j]->Name), ClassNameToUpper(AllModuleList[j]->InterfaceList[i].Name));
				EnumInfo.MemberList[i].Value.Format(_T("%d"), AllModuleList[j]->InterfaceList[i].ID);
			}
			SystemEnumList.push_back(EnumInfo);
		}
	}

	return ExportEnums(ExportDir, FileName, SystemEnumList, m_MainModule.Name, _T("System"), true);
}

bool CCallInterfaceMakerDlg::ExportEnums(LPCTSTR ExportDir, LPCTSTR FileName, vector<ENUM_DEFINE_INFO>& EnumList, LPCTSTR SpaceName, LPCTSTR ListName, bool IsSystem)
{
	CString OutputFileName;

	OutputFileName = ExportDir;
	OutputFileName += _T("\\");
	OutputFileName += FileName;

	CString EnumDefineHeader;

	if (IsSystem)
		EnumDefineHeader = m_SystemEnumDefineHeaderTemplate;
	else
		EnumDefineHeader = m_EnumDefineHeaderTemplate;

	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString ListNameUp = ClassNameToUpper(ListName);

	EnumDefineHeader.Replace(_T("<SpaceName>"), SpaceName);

	EnumDefineHeader.Replace(_T("<!SpaceName>"), SpaceNameUp);
	EnumDefineHeader.Replace(_T("<!ListName>"), ListNameUp);


	CString LineSpace = GetLineSpace(EnumDefineHeader, _T("<Enums>"));
	CString Enums = MakeEnumDefines(EnumList, SpaceName, LineSpace);
	EnumDefineHeader.Replace(_T("<Enums>"), Enums);

	CString EnumStrValuesName;
	EnumStrValuesName.Format(_T("%s_ENUM_STR_VALUES"), ListNameUp);
	EnumDefineHeader.Replace(_T("<EnumStrValuesName>"), EnumStrValuesName);

	//LineSpace = GetLineSpace(EnumDefineHeader, _T("<EnumStrValues>"));
	//CString EnumStrValues = MakeEnumStrValues(EnumList, SpaceName, LineSpace);
	//EnumDefineHeader.Replace(_T("<EnumStrValues>"), EnumStrValues);


	WriteStringToFile(OutputFileName, EnumDefineHeader);

	return false;
}

bool CCallInterfaceMakerDlg::ExportConsts(LPCTSTR ExportDir, LPCTSTR FileName, vector<CONST_DEFINE_INFO>& ConstList, LPCTSTR SpaceName, LPCTSTR ListName)
{
	CString OutputFileName;

	OutputFileName = ExportDir;
	OutputFileName += _T("\\");
	OutputFileName += FileName;

	CString ConstDefineHeader = m_ConstDefineHeaderTemplate;

	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString ListNameUp = ClassNameToUpper(ListName);

	ConstDefineHeader.Replace(_T("<SpaceName>"), SpaceName);

	ConstDefineHeader.Replace(_T("<!SpaceName>"), SpaceNameUp);
	ConstDefineHeader.Replace(_T("<!ListName>"), ListNameUp);

	CString LineSpace = GetLineSpace(ConstDefineHeader, _T("<Consts>"));
	CString Consts = MakeConstDefines(ConstList, SpaceName, LineSpace);
	ConstDefineHeader.Replace(_T("<Consts>"), Consts);

	WriteStringToFile(OutputFileName, ConstDefineHeader);

	return false;
}

bool CCallInterfaceMakerDlg::ExportDataStructDefines(vector<CONST_DEFINE_LIST*>& ConstList, vector<ENUM_DEFINE_LIST*>& EnumList, vector<STRUCT_DEFINE_LIST2*>& StructList,
	LPCTSTR ExportDir, LPCTSTR SpaceName)
{
	CString Temp;
	if (m_InterfaceConfig.ConstExportConfig.IsExport)
	{
		for (CONST_DEFINE_LIST* pList : ConstList)
		{
			if (pList->IgnoreOnExport && (!m_InterfaceConfig.ForceExportAll))
				continue;
			CString ModuleName = GetModuleName(pList->ModuleID);
			Temp.Format(_T("%s%sConsts.%s"), ModuleName, pList->ListName, m_InterfaceConfig.ConstExportConfig.ExportExt);
			ExportConsts(ExportDir, Temp, pList->ConstList, SpaceName, pList->ListName);
		}
	}
	if (m_InterfaceConfig.EnumExportConfig.IsExport)
	{
		for (ENUM_DEFINE_LIST* pList : EnumList)
		{
			if (pList->IgnoreOnExport && (!m_InterfaceConfig.ForceExportAll))
				continue;
			CString ModuleName = GetModuleName(pList->ModuleID);
			Temp.Format(_T("%s%sEnums.%s"), ModuleName, pList->ListName, m_InterfaceConfig.EnumExportConfig.ExportExt);
			ExportEnums(ExportDir, Temp, pList->EnumList, SpaceName, pList->ListName, false);
		}
	}
	if (m_InterfaceConfig.StructExportConfig.IsExport)
	{
		for (STRUCT_DEFINE_LIST2* pList : StructList)
		{
			if (pList->IgnoreOnExport && (!m_InterfaceConfig.ForceExportAll))
				continue;
			CString ModuleName = GetModuleName(pList->ModuleID);
			Temp.Format(_T("%s%sStructs.%s"), ModuleName, pList->ListName, m_InterfaceConfig.StructExportConfig.ExportExt);
			ExportStructs(ExportDir, Temp, SpaceName, *pList);
		}
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportInterfaces(vector<CALLER_INTERFACE*>& InterfaceList, LPCTSTR ExportDir, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName)
{
	for (size_t i = 0; i < InterfaceList.size(); i++)
	{
		CALLER_INTERFACE& InterfaceInfo = *(InterfaceList[i]);

		if (IsListItemChecked(InterfaceInfo.ModuleID, InterfaceInfo.ID))
		{
			if (m_InterfaceConfig.InterfaceExportConfig.IsExport)
			{
				CString OutputFileName;
				CString LineSpace;

				CString InterfaceName;
				if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
					InterfaceName = InterfaceInfo.Name + _T("Ack");
				else
					InterfaceName = InterfaceInfo.Name;

				OutputFileName.Format(_T("%s\\%sInterface.%s"), ExportDir, InterfaceName,
					m_InterfaceConfig.InterfaceExportConfig.ExportExt);

				CString Interface = m_InterfaceTemplate;

				if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL)
				{
					ENUM_DEFINE_INFO EnumInfo;

					EnumInfo.Name.Format(_T("%s_METHODS"), ClassNameToUpper(InterfaceName));

					EnumInfo.MemberList.resize(InterfaceInfo.MethodList.size());
					for (size_t j = 0; j < InterfaceInfo.MethodList.size(); j++)
					{
						EnumInfo.MemberList[j].Name.Format(_T("METHOD_%s"), ClassNameToUpper(InterfaceInfo.MethodList[j].Name));
						EnumInfo.MemberList[j].Value.Format(_T("%d"), InterfaceInfo.MethodList[j].ID);
					}

					LineSpace = GetLineSpace(Interface, _T("<MethodIDEnums>"));
					CString MethodIDEnums = MakeEnumDefine(EnumInfo, SpaceName, LineSpace);
					Interface.Replace(_T("<MethodIDEnums>"), MethodIDEnums);
				}
				else
				{
					Interface.Replace(_T("<MethodIDEnums>"), _T(""));
				}

				if (Interface.Find(_T("<DependImports>")) >= 0)
				{
					LineSpace = GetLineSpace(Interface, _T("<DependImports>"));
					CString Imports = MakeStructDependImports(InterfaceInfo, SpaceName, InterfaceName, LineSpace, ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL ? INTERFACE_DEPEND_MAKE_DECLARE : INTERFACE_DEPEND_MAKE_ACK_DECLARE);
					Interface.Replace(_T("<DependImports>"), Imports);
				}

				Interface.Replace(_T("<SpaceName>"), SpaceName);
				Interface.Replace(_T("<InterfaceName>"), InterfaceName);
				Interface.Replace(_T("<InterfaceDescription>"), InterfaceInfo.Description);
				Interface.Replace(_T("<@InterfaceName>"), InterfaceInfo.Name);
				LineSpace = GetLineSpace(Interface, _T("<Methods>"));
				CString Methods = MakeMethods(InterfaceInfo, SpaceName, ExportType, LineSpace, true);
				Interface.Replace(_T("<Methods>"), Methods);
				LineSpace = GetLineSpace(Interface, _T("<MethodSSTIDEnumDefine>"));
				CString SSTIDDefines = MakeMethodSSTIDEnumDefines(InterfaceInfo, ExportType, SpaceName, LineSpace);
				Interface.Replace(_T("<MethodSSTIDEnumDefine>"), SSTIDDefines);

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

	CString HeaderFileName, SourceFileName;

	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + _T("Ack");
	else
		InterfaceName = InterfaceInfo.Name;


	if (m_InterfaceConfig.CallHeaderExportConfig.IsExport)
	{
		HeaderFileName.Format(_T("%s\\%sMsgCaller.%s"), ExportDir, InterfaceName,
			m_InterfaceConfig.CallHeaderExportConfig.ExportExt);


		CString Header = m_MsgCallerHeaderTemplate;

		Header.Replace(_T("<SpaceName>"), SpaceName);
		Header.Replace(_T("<InterfaceName>"), InterfaceName);
		Header.Replace(_T("<@InterfaceName>"), InterfaceInfo.Name);
		CString LineSpace = GetLineSpace(Header, _T("<Methods>"));
		CString Methods = MakeMethods(InterfaceInfo, SpaceName, ExportType, LineSpace, false);
		Header.Replace(_T("<Methods>"), Methods);

		LineSpace = GetLineSpace(Header, _T("<PackMethods>"));
		Methods = MakeMsgPackMethods(InterfaceInfo.MethodList, ExportType, LineSpace);
		Header.Replace(_T("<PackMethods>"), Methods);

		WriteStringToFile(HeaderFileName, Header);
	}

	if (m_InterfaceConfig.CallSourceExportConfig.IsExport)
	{
		SourceFileName.Format(_T("%s\\%sMsgCaller.%s"), ExportDir, InterfaceName,
			m_InterfaceConfig.CallSourceExportConfig.ExportExt);

		CString Source = m_MsgCallerSourceTemplate;

		if (Source.Find(_T("<DependImports>")) >= 0)
		{
			CString LineSpace = GetLineSpace(Source, _T("<DependImports>"));
			CString Imports = MakeStructDependImports(InterfaceInfo, SpaceName, InterfaceName, LineSpace, ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL ? INTERFACE_DEPEND_MAKE_CALLER : INTERFACE_DEPEND_MAKE_ACK_CALLER);
			Source.Replace(_T("<DependImports>"), Imports);
		}

		MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(InterfaceInfo.ModuleID);
		if (pModuleInfo)
		{
			Source.Replace(_T("<MODULE_NAME>"), ClassNameToUpper(pModuleInfo->Name));
		}

		Source.Replace(_T("<SpaceName>"), SpaceName);
		Source.Replace(_T("<InterfaceName>"), InterfaceName);
		Source.Replace(_T("<@INTERFACE_NAME>"), ClassNameToUpper(InterfaceInfo.Name));
		Source.Replace(_T("<@InterfaceName>"), InterfaceInfo.Name);

		CString LineSpace = GetLineSpace(Source, _T("<Methods>"));
		CString Methods = MakeMethodsSource(InterfaceInfo, SpaceName, ExportType, LineSpace);
		Source.Replace(_T("<Methods>"), Methods);

		LineSpace = GetLineSpace(Source, _T("<PackMethods>"));
		Methods = MakeMsgPackMethodsSource(SpaceName, InterfaceInfo, ExportType, LineSpace);
		Source.Replace(_T("<PackMethods>"), Methods);

		WriteStringToFile(SourceFileName, Source);
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportMsgHandler(LPCTSTR ExportDir, LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType)
{
	CString HeaderFileName, SourceFileName;


	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + _T("Ack");
	else
		InterfaceName = InterfaceInfo.Name;

	if (m_InterfaceConfig.HandlerHeaderExportConfig.IsExport)
	{
		HeaderFileName.Format(_T("%s\\%sMsgHandler.%s"), ExportDir, InterfaceName,
			m_InterfaceConfig.HandlerHeaderExportConfig.ExportExt);

		CString Header = m_MsgHandlerHeaderTemplate;

		Header.Replace(_T("<SpaceName>"), SpaceName);
		Header.Replace(_T("<InterfaceName>"), InterfaceName);
		CString LineSpace = GetLineSpace(Header, _T("<Methods>"));
		CString Methods = MakeMsgHandlerMethods(InterfaceInfo, ExportType, LineSpace);
		Header.Replace(_T("<Methods>"), Methods);

		WriteStringToFile(HeaderFileName, Header);
	}

	if (m_InterfaceConfig.HandlerSourceExportConfig.IsExport)
	{
		SourceFileName.Format(_T("%s\\%sMsgHandler.%s"), ExportDir, InterfaceName,
			m_InterfaceConfig.HandlerSourceExportConfig.ExportExt);

		CString Source = m_MsgHandlerSourceTemplate;

		if (Source.Find(_T("<DependImports>")) >= 0)
		{
			CString LineSpace = GetLineSpace(Source, _T("<DependImports>"));
			CString Imports = MakeStructDependImports(InterfaceInfo, SpaceName, InterfaceName, LineSpace, ExportType == INTERFACE_METHOD_EXPORT_TYPE_CALL ? INTERFACE_DEPEND_MAKE_HANDLER : INTERFACE_DEPEND_MAKE_ACK_HANDLER);
			Source.Replace(_T("<DependImports>"), Imports);
		}

		MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(InterfaceInfo.ModuleID);
		if (pModuleInfo)
		{
			Source.Replace(_T("<MODULE_NAME>"), ClassNameToUpper(pModuleInfo->Name));
		}

		Source.Replace(_T("<SpaceName>"), SpaceName);
		Source.Replace(_T("<InterfaceName>"), InterfaceName);
		Source.Replace(_T("<@INTERFACE_NAME>"), ClassNameToUpper(InterfaceInfo.Name));
		Source.Replace(_T("<@InterfaceName>"), InterfaceInfo.Name);

		CString MethodCount;
		MethodCount.Format(_T("%d"), InterfaceInfo.MethodList.size());
		Source.Replace(_T("<MethodCount>"), MethodCount);

		CString LineSpace = GetLineSpace(Source, _T("<MsgMapInits>"));
		CString MsgMapInits = MakeMsgMapInits(InterfaceInfo, SpaceName, ExportType, LineSpace);
		Source.Replace(_T("<MsgMapInits>"), MsgMapInits);

		LineSpace = GetLineSpace(Source, _T("<Methods>"));
		CString Methods = MakeMsgHandlerMethodsSource(InterfaceInfo, ExportType, SpaceName, LineSpace);
		Source.Replace(_T("<Methods>"), Methods);

		WriteStringToFile(SourceFileName, Source);
	}
	return true;
}

bool CCallInterfaceMakerDlg::ExportDataObject(vector<STRUCT_DEFINE_LIST2*>& StructList, LPCTSTR ExportDir, LPCTSTR SpaceName)
{
	CString HeaderFileName, SourceFileName;

	for (STRUCT_DEFINE_LIST2* pList : StructList)
	{
		if (!pList->IgnoreOnExport && (!m_InterfaceConfig.ForceExportAll))
		{
			for (STRUCT_DEFINE_INFO* pInfo : pList->StructList)
			{
				if (pInfo->Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				{
					CString SSTIDEnumName;
					SSTIDEnumName.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(pInfo->Name));

					if (m_InterfaceConfig.DataObjectHeaderExportConfig.IsExport)
					{
						HeaderFileName.Format(_T("%s\\%s.%s"), ExportDir, ClassNameToFileName(pInfo->Name),
							m_InterfaceConfig.DataObjectHeaderExportConfig.ExportExt);

						CString Header = RestoreFileToTemplate(HeaderFileName, m_DataObjectDefineHeaderTemple);



						Header.Replace(_T("<SpaceName>"), SpaceName);

						CString LineSpace = GetLineSpace(m_MsgHandlerHeaderTemplate, _T("<Description>"));
						CString Description = ToComment(pInfo->Description, LineSpace);
						Header.Replace(_T("<Description>"), Description);


						Header.Replace(_T("<ClassName>"), pInfo->Name);

						if (pInfo->BaseStruct.IsEmpty())
						{
							RemoveBlock(Header, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
							Header.Replace(_T("<IfNotHaveBaseClass>"), _T(""));
							Header.Replace(_T("</IfNotHaveBaseClass>"), _T(""));
							Header.Replace(_T("<BaseClass>"), _T(""));

						}
						else
						{
							Header.Replace(_T("<IfHaveBaseClass>"), _T(""));
							Header.Replace(_T("</IfHaveBaseClass>"), _T(""));
							RemoveBlock(Header, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
							Header.Replace(_T("<BaseClass>"), pInfo->BaseStruct);
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_JSON_PROCESS)
						{
							LineSpace = GetLineSpace(Header, _T("<JsonProcess>"));
							CString Process = m_DataObjectJsonProcessHeaderTemplate;
							Process.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
							Header.Replace(_T("<JsonProcess>"), Process);
						}
						else
						{
							Header.Replace(_T("<JsonProcess>"), _T(""));
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_DB_PROCESS)
						{
							LineSpace = GetLineSpace(Header, _T("<DBProcess>"));
							CString Process = m_DataObjectDBProcessHeaderTemplate;
							Process.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
							Header.Replace(_T("<DBProcess>"), Process);
						}
						else
						{
							Header.Replace(_T("<DBProcess>"), _T(""));
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_LUA_PROCESS)
						{
							LineSpace = GetLineSpace(Header, _T("<LuaProcess>"));
							CString Process = m_DataObjectLuaProcessHeaderTemplate;
							Process.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
							Header.Replace(_T("<LuaProcess>"), Process);

							LineSpace = GetLineSpace(Header, _T("<LuaProcessSource>"));
							Process = MakeDataObjectLuaProcess(*pInfo, SSTIDEnumName, SpaceName, LineSpace);
							Header.Replace(_T("<LuaProcessSource>"), Process);
						}
						else
						{
							Header.Replace(_T("<LuaProcess>"), _T(""));
							Header.Replace(_T("<LuaProcessSource>"), _T(""));
						}

						if (Header.Find(_T("<Members>")) >= 0)
						{
							LineSpace = GetLineSpace(Header, _T("<Members>"));
							CString Members = MakeStructMembers(*pInfo, _T(";"), true, SpaceName, LineSpace);
							Header.Replace(_T("<Members>"), Members);
						}
						if (Header.Find(_T("<Members,>")) >= 0)
						{
							LineSpace = GetLineSpace(Header, _T("<Members,>"));
							CString Members = MakeStructMembers(*pInfo, _T(","), false, SpaceName, LineSpace);
							Header.Replace(_T("<Members,>"), Members);
						}

						LineSpace = GetLineSpace(Header, _T("<SSTIDEnumDefine>"));
						CString SSTIDEnum = MakeStructSSTIDEnumDefines(*pInfo, SpaceName, LineSpace);
						Header.Replace(_T("<SSTIDEnumDefine>"), SSTIDEnum);

						LineSpace = GetLineSpace(Header, _T("<ModifyFlagEnumDefine>"));
						CString ModifyFlagEnumDefine = MakeDataObjectModifyFlagEnumDefine(*pInfo, SpaceName, LineSpace);
						Header.Replace(_T("<ModifyFlagEnumDefine>"), ModifyFlagEnumDefine);

						LineSpace = GetLineSpace(Header, _T("<SetMethodsDeclare>"));
						CString SetMethodsDeclare = MakeDataObjectSetMethodsDeclare(*pInfo, LineSpace);
						Header.Replace(_T("<SetMethodsDeclare>"), SetMethodsDeclare);

						LineSpace = GetLineSpace(Header, _T("<GetMethodsDeclare>"));
						CString GetMethodsDeclare = MakeDataObjectGetMethodsDeclare(*pInfo, LineSpace);
						Header.Replace(_T("<GetMethodsDeclare>"), GetMethodsDeclare);


						LineSpace = GetLineSpace(Header, _T("<SetMethodsDefine>"));
						CString SetMethodsDefine = MakeDataObjectSetMethodsDefine(*pInfo, SpaceName, LineSpace);
						Header.Replace(_T("<SetMethodsDefine>"), SetMethodsDefine);

						LineSpace = GetLineSpace(Header, _T("<GetMethodsDefine>"));
						CString GetMethodsDefine = MakeDataObjectGetMethodsDefine(*pInfo, SpaceName, LineSpace);
						Header.Replace(_T("<GetMethodsDefine>"), GetMethodsDefine);

						Header.Replace(_T("<CLASS_FLAG_INDEX>"), _T("DATA_OBJECT_FLAG_") + ClassNameToUpper(pInfo->Name));

						WriteStringToFile(HeaderFileName, Header);
					}
					if (m_InterfaceConfig.DataObjectSourceExportConfig.IsExport)
					{
						SourceFileName.Format(_T("%s\\%s.%s"), ExportDir, ClassNameToFileName(pInfo->Name),
							m_InterfaceConfig.DataObjectSourceExportConfig.ExportExt);
						CString Source = RestoreFileToTemplate(SourceFileName, m_DataObjectDefineSourceTemple);

						Source.Replace(_T("<SpaceName>"), SpaceName);

						CString LineSpace;



						if (pInfo->Flag & STRUCT_FLAG_EXPORT_XML_PROCESS)
						{
							LineSpace = GetLineSpace(Source, _T("<XMLProcess>"));
							CString XMLProcess = MakeXMLProcess(*pInfo, SSTIDEnumName, SpaceName, LineSpace);
							Source.Replace(_T("<XMLProcess>"), XMLProcess);
						}
						else
						{
							Source.Replace(_T("<XMLProcess>"), _T(""));
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_JSON_PROCESS)
						{
							LineSpace = GetLineSpace(Source, _T("<JsonProcess>"));
							CString JsonProcess = MakeDataObjectJsonProcess(*pInfo, SSTIDEnumName, SpaceName, LineSpace);
							Source.Replace(_T("<JsonProcess>"), JsonProcess);
						}
						else
						{
							Source.Replace(_T("<JsonProcess>"), _T(""));
						}

						if (pInfo->Flag & STRUCT_FLAG_EXPORT_DB_PROCESS)
						{
							LineSpace = GetLineSpace(Source, _T("<DBProcess>"));
							CString Process = MakeDataObjectDBProcess(*pInfo, SSTIDEnumName, SpaceName, LineSpace);
							Source.Replace(_T("<DBProcess>"), Process);
						}
						else
						{
							Source.Replace(_T("<DBProcess>"), _T(""));
						}



						LineSpace = GetLineSpace(m_MsgHandlerHeaderTemplate, _T("<Description>"));
						CString Description = ToComment(pInfo->Description, LineSpace);
						Source.Replace(_T("<Description>"), Description);

						Source.Replace(_T("<SpaceName>"), SpaceName);

						if (pInfo->MemberList.size())
						{
							Source.Replace(_T("<IfHaveMember>"), _T(""));
							Source.Replace(_T("</IfHaveMember>"), _T(""));
						}
						else
						{
							RemoveBlock(Source, _T("<IfHaveMember>"), _T("</IfHaveMember>"));
						}

						if (pInfo->BaseStruct.IsEmpty())
						{
							RemoveBlock(Source, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
							Source.Replace(_T("<IfNotHaveBaseClass>"), _T(""));
							Source.Replace(_T("</IfNotHaveBaseClass>"), _T(""));
							Source.Replace(_T("<BaseClass>"), _T(""));

						}
						else
						{
							Source.Replace(_T("<IfHaveBaseClass>"), _T(""));
							Source.Replace(_T("</IfHaveBaseClass>"), _T(""));
							RemoveBlock(Source, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
							Source.Replace(_T("<BaseClass>"), pInfo->BaseStruct);
							Source.Replace(_T("<BaseClass>"), pInfo->BaseStruct);
						}

						Source.Replace(_T("<ClassName>"), pInfo->Name);

						CString FullModifyFlag;
						FullModifyFlag.Format(_T("DOMF_%s_FULL"), ClassNameToUpper(pInfo->Name));
						Source.Replace(_T("<FULL_MODIFY_FLAG>"), FullModifyFlag);

						if (Source.Find(_T("<Members>")) >= 0)
						{
							LineSpace = GetLineSpace(Source, _T("<Members>"));
							CString Members = MakeStructMembers(*pInfo, _T(";"), true, SpaceName, LineSpace);
							Source.Replace(_T("<Members>"), Members);
						}

						if (Source.Find(_T("<Members,>")) >= 0)
						{
							LineSpace = GetLineSpace(Source, _T("<Members,>"));
							CString Members = MakeStructMembers(*pInfo, _T(","), false, SpaceName, LineSpace);
							Source.Replace(_T("<Members,>"), Members);
						}

						if (Source.Find(_T("<DependImports>")) >= 0)
						{
							LineSpace = GetLineSpace(Source, _T("<DependImports>"));
							CString Imports = MakeStructDependImports(*pInfo, SpaceName, LineSpace);
							Source.Replace(_T("<DependImports>"), Imports);
						}

						LineSpace = GetLineSpace(Source, _T("<SSTIDEnumDefine>"));
						CString SSTIDEnum = MakeStructSSTIDEnumDefines(*pInfo, SpaceName, LineSpace);
						Source.Replace(_T("<SSTIDEnumDefine>"), SSTIDEnum);

						LineSpace = GetLineSpace(Source, _T("<ModifyFlagEnumDefine>"));
						CString ModifyFlagEnumDefine = MakeDataObjectModifyFlagEnumDefine(*pInfo, SpaceName, LineSpace);
						Source.Replace(_T("<ModifyFlagEnumDefine>"), ModifyFlagEnumDefine);

						LineSpace = GetLineSpace(Source, _T("<DataObjectMembersGetModifyFlag>"));
						CString DataObjectMembersGetModifyFlag = MakeDataObjectMembersGetModifyFlag(*pInfo, LineSpace);
						Source.Replace(_T("<DataObjectMembersGetModifyFlag>"), DataObjectMembersGetModifyFlag);

						LineSpace = GetLineSpace(Source, _T("<DataObjectMembersIsModified>"));
						CString DataObjectMembersIsModified = MakeDataDataObjectMembersIsModified(*pInfo, LineSpace);
						Source.Replace(_T("<DataObjectMembersIsModified>"), DataObjectMembersIsModified);

						LineSpace = GetLineSpace(Source, _T("<DataObjectMembersClearModifyFlag>"));
						CString DataObjectMembersClearModifyFlag = MakeDataDataObjectMembersClearModifyFlag(*pInfo, LineSpace);
						Source.Replace(_T("<DataObjectMembersClearModifyFlag>"), DataObjectMembersClearModifyFlag);

						LineSpace = GetLineSpace(Source, _T("<SetMethodsDefine>"));
						CString SetMethodsDefine = MakeDataObjectSetMethodsDefine(*pInfo, SpaceName, LineSpace);
						Source.Replace(_T("<SetMethodsDefine>"), SetMethodsDefine);

						LineSpace = GetLineSpace(Source, _T("<GetMethodsDefine>"));
						CString GetMethodsDefine = MakeDataObjectGetMethodsDefine(*pInfo, SpaceName, LineSpace);
						Source.Replace(_T("<GetMethodsDefine>"), GetMethodsDefine);

						Source.Replace(_T("<CLASS_FLAG_INDEX>"), _T("DATA_OBJECT_FLAG_") + ClassNameToUpper(pInfo->Name));


						LineSpace = GetLineSpace(Source, _T("<InitOperations>"));
						CString InitOperations = MakeInitOperations(*pInfo, LineSpace);
						Source.Replace(_T("<InitOperations>"), InitOperations);


						CString PacketName = m_InterfaceConfig.DefaultPacketName;
						CString PacketMemberName = m_InterfaceConfig.DefaultPacketMemberName;


						LineSpace = GetLineSpace(Source, _T("<PackOperations>"));
						CString PackOperations = MakePackOperations(*pInfo, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
						Source.Replace(_T("<PackOperations>"), PackOperations);

						LineSpace = GetLineSpace(Source, _T("<UnpackOperations>"));
						CString UnpackOperations = MakeUnpackOperations(*pInfo, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
						Source.Replace(_T("<UnpackOperations>"), UnpackOperations);

						LineSpace = GetLineSpace(Source, _T("<PacketSizes>"));
						CString PackSizes = MakePackSizes(*pInfo, SpaceName, LineSpace);
						Source.Replace(_T("<PacketSizes>"), PackSizes);

						//LineSpace = GetLineSpace(Source, _T("<ToXMLOperations>"));
						//CString ToXMLOperations = MakeToXMLOperations(StructInfo, SSTIDEnumName, LineSpace);
						//Source.Replace(_T("<ToXMLOperations>"), ToXMLOperations);

						//LineSpace = GetLineSpace(Source, _T("<FromXMLOperations>"));
						//CString FromXMLOperations = MakeFromXMLOperations(StructInfo, SSTIDEnumName, LineSpace);
						//Source.Replace(_T("<FromXMLOperations>"), FromXMLOperations);

						LineSpace = GetLineSpace(Source, _T("<CloneOperations>"));
						CString CloneOperations = MakeCloneOperations(*pInfo, m_InterfaceConfig.MemberVariablePrefix, _T("DataObject."), SpaceName, LineSpace);
						Source.Replace(_T("<CloneOperations>"), CloneOperations);

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

	vector<STRUCT_DEFINE_INFO*> DataObjectList;

	GetDataObjectListSorted(m_AllDataStructList, DataObjectList);

	if (m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.IsExport)
	{
		CString HeaderFileName;
		CString LineSpace;

		HeaderFileName.Format(_T("%s\\DataObjectModifyFlags.%s"), ExportDir,
			m_InterfaceConfig.DataObjectModifyFlagsHeaderExportConfig.ExportExt);

		CString Header = m_DataObjectModifyFlagsHeaderTemple;


		Header.Replace(_T("<SpaceName>"), SpaceName);

		LineSpace = GetLineSpace(Header, _T("<DataObjectModifyFlags>"));
		CString DataObjectModifyFlags = MakeDataObjectModifyFlagsHeader(DataObjectList, DataObjectModifyFlagList, SpaceName, LineSpace);
		Header.Replace(_T("<DataObjectModifyFlags>"), DataObjectModifyFlags);

		WriteStringToFile(HeaderFileName, Header);
	}


	if (m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.IsExport)
	{
		CString SourceFileName;
		CString LineSpace;

		SourceFileName.Format(_T("%s\\DataObjectModifyFlags.%s"), ExportDir,
			m_InterfaceConfig.DataObjectModifyFlagsSourceExportConfig.ExportExt);

		CString Source = m_DataObjectModifyFlagsSourceTemple;

		Source.Replace(_T("<SpaceName>"), SpaceName);

		LineSpace = GetLineSpace(Source, _T("<DataObjectModifyFlags>"));
		CString DataObjectModifyFlags = MakeDataObjectModifyFlagsSource(DataObjectList, DataObjectModifyFlagList, SpaceName, LineSpace);
		Source.Replace(_T("<DataObjectModifyFlags>"), DataObjectModifyFlags);

		if (Source.Find(_T("<DependImports>")) >= 0)
		{
			LineSpace = GetLineSpace(Source, _T("<DependImports>"));
			CString Imports = MakeAllDataObjectDependImports(DataObjectList, SpaceName, LineSpace);
			Source.Replace(_T("<DependImports>"), Imports);
		}

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
				MethodName = MethodInfo.Name + _T("Ack");
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
				InterfaceName = InterfaceInfo.Name + _T("Ack");
			else
				InterfaceName = InterfaceInfo.Name;

			Method.Replace(_T("<SpaceName>"), SpaceName);
			Method.Replace(_T("<InterfaceName>"), InterfaceName);
			Method.Replace(_T("<MethodName>"), MethodName);
			Method.Replace(_T("<MethodDescription>"), MethodInfo.Description);
			if (IsPure)
				Method.Replace(_T("<IsPure>"), m_MethodHeaderPureTailTemple);
			else
				Method.Replace(_T("<IsPure>"), _T(";"));
			CString Params = MakeParams(*pParamList, true, true);
			CString LineSpace = GetLineSpace(Method, _T("<ParamDescriptions>"));
			CString ParamDescriptions = MakeParamDescriptions(*pParamList, LineSpace);


			Method.Replace(_T("<Params>"), Params);
			Method.Replace(_T("<ParamDescriptions>"), ParamDescriptions);
			Methods += Method + _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMethodSSTIDEnumDefines(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString MethodIDEnumDefines;

	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = ClassNameToUpper(InterfaceInfo.Name + _T("Ack"));
	else
		InterfaceName = ClassNameToUpper(InterfaceInfo.Name);

	vector<INTERFACE_METHOD>& MethodList = InterfaceInfo.MethodList;
	for (size_t i = 0;i < MethodList.size();i++)
	{
		INTERFACE_METHOD& MethodInfo = MethodList[i];
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
				MethodName = MethodInfo.Name + _T("Ack");
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

			EnumInfo.Name.Format(_T("%s_%s_MEMBER_IDS"), InterfaceName, MethodName);
			EnumInfo.ShortName = EnumInfo.Name;

			EnumInfo.MemberList.resize(pParamList->size());



			for (size_t j = 0; j < pParamList->size(); j++)
			{
				CString ParamName;
				ParamName = ClassNameToUpper((*pParamList)[j].Name);
				EnumInfo.MemberList[j].Name.Format(_T("SST_%s_%s"), MethodName, ParamName);
				EnumInfo.MemberList[j].Value.Format(_T("%d"), (*pParamList)[j].ID);
			}

			MethodIDEnumDefines += MakeEnumDefine(EnumInfo, SpaceName, szLineSpace);
		}

	}

	return MethodIDEnumDefines;
}

CString CCallInterfaceMakerDlg::MakeStructSSTIDEnumDefines(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(StructInfo.Name));
	EnumInfo.ShortName = EnumInfo.Name;

	EnumInfo.MemberList.resize(StructInfo.MemberList.size());

	CString StructName;
	StructName = StructInfo.ShortName;

	if (!StructInfo.BaseStruct.IsEmpty())
	{
		if (StructInfo.ObjectID == 0)
		{
			CString Msg;
			Msg.Format(_T("结构%s有基类，但没有设置ObjectID，会导致打解包异常"), StructInfo.Name);
			AfxMessageBox(Msg);
		}
		else
		{
			vector<UINT> ObjectIDList;
			ObjectIDList.push_back(StructInfo.ObjectID);
			if (!CheckBaseStructObjectID(ObjectIDList, StructInfo.BaseStruct))
			{
				CString Msg;
				Msg.Format(_T("结构%s有基类中ObjectID有重复，会导致打解包异常"), StructInfo.Name);
				AfxMessageBox(Msg);
			}
		}
	}

	for (size_t j = 0;j < StructInfo.MemberList.size();j++)
	{
		CString MemberName;
		MemberName = ClassNameToUpper(StructInfo.MemberList[j].Name);
		EnumInfo.MemberList[j].Name.Format(_T("SST_%s_%s"), StructName, MemberName);
		EnumInfo.MemberList[j].Value.Format(_T("%d"), StructInfo.ObjectID + StructInfo.MemberList[j].ID);
	}

	return MakeEnumDefine(EnumInfo, SpaceName, szLineSpace);



}

CString CCallInterfaceMakerDlg::MakeMsgPackMethods(vector<INTERFACE_METHOD>& MethodList, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString Methods;
	for (size_t j = 0;j < MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo = MethodList[j];
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
				MethodName = MethodInfo.Name + _T("Ack");
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
			Method.Replace(_T("<MethodName>"), MethodName);
			CString Params = MakeParams(*pParamList, true, true);
			if (!Params.IsEmpty())
				Params = _T(",") + Params;
			Method.Replace(_T("<,Params>"), Params);
			Methods += Method + _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeParams(vector<METHOD_PARAM>& ParamList, bool IsDefine, bool IncludeDefaultValue)
{
	CString Params;
	for (size_t k = 0;k < ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo = ParamList[k];

		TYPE_DEFINE* pTypeInfo = FindVarType(ParamInfo.Type);
		if (pTypeInfo)
		{
			CString Param;

			if (IsDefine)
				Param = m_ParamDefineTemple;
			else
				Param = m_ParamTransTemple;



			if (ParamInfo.IsArray)
			{
				if (IsDefine)
				{
					CString ReferenceDefine = m_InterfaceConfig.ArrayDefineConfig.ReferenceDefine;
					ReferenceDefine.Replace(_T("<Type>"), pTypeInfo->CType);
					Param.Replace(_T("<ParamType>"), ReferenceDefine);
					Param.Replace(_T("<ParamName>"), ParamInfo.Name);
				}
				else
				{
					CString ReferenceUse = m_InterfaceConfig.ArrayDefineConfig.ReferenceUse;
					ReferenceUse.Replace(_T("<Variable>"), ParamInfo.Name);
					Param.Replace(_T("<ParamName>"), ReferenceUse);
				}
			}
			else if (ParamInfo.IsReference)
			{
				if (IsDefine)
				{
					CString ReferenceDefine = pTypeInfo->GenerateOperations.ReferenceDefine;
					ReferenceDefine.Replace(_T("<Type>"), pTypeInfo->CType);
					Param.Replace(_T("<ParamType>"), ReferenceDefine);
					Param.Replace(_T("<ParamName>"), ParamInfo.Name);
				}
				else
				{
					CString ReferenceUse = pTypeInfo->GenerateOperations.ReferenceUse;
					ReferenceUse.Replace(_T("<Variable>"), ParamInfo.Name);
					Param.Replace(_T("<ParamName>"), ReferenceUse);
				}
			}
			else
			{
				Param.Replace(_T("<ParamName>"), ParamInfo.Name);
				Param.Replace(_T("<ParamType>"), pTypeInfo->CType);
			}


			if (ParamInfo.DefaultValue.IsEmpty() || (!IncludeDefaultValue))
			{
				Param.Replace(_T("<ParamDefaultValue>"), _T(""));
			}
			else
			{
				CString DefauleValue;
				DefauleValue.Format(_T("= %s"), ParamInfo.DefaultValue);
				Param.Replace(_T("<ParamDefaultValue>"), DefauleValue);
			}

			if (k)
			{
				Params += _T(",");
			}
			Params += Param;
		}
	}
	return Params;
}

CString CCallInterfaceMakerDlg::MakeParamDescriptions(vector<METHOD_PARAM>& ParamList, LPCTSTR szLineSpace)
{
	CString ParamDescriptions;
	for (size_t k = 0;k < ParamList.size();k++)
	{
		METHOD_PARAM& ParamInfo = ParamList[k];

		TYPE_DEFINE* pTypeInfo = FindVarType(ParamInfo.Type);
		if (pTypeInfo)
		{

			CString ParamDescription = m_ParamDescriptionTemple;

			ParamDescription.Replace(_T("<ParamName>"), ParamInfo.Name);
			if (ParamInfo.IsArray)
			{
				CString ReferenceDefine = m_InterfaceConfig.ArrayDefineConfig.ReferenceDefine;
				ReferenceDefine.Replace(_T("<Type>"), pTypeInfo->CType);
				ParamDescription.Replace(_T("<ParamType>"), ReferenceDefine);
			}
			else if (ParamInfo.IsReference)
			{
				CString ReferenceDefine = pTypeInfo->GenerateOperations.ReferenceDefine;
				ReferenceDefine.Replace(_T("<Type>"), pTypeInfo->CType);
				ParamDescription.Replace(_T("<ParamType>"), ReferenceDefine);
			}
			else
			{
				ParamDescription.Replace(_T("<ParamType>"), pTypeInfo->CType);
			}

			ParamDescription.Replace(_T("<ParamDescript>"), ParamInfo.Description);

			ParamDescriptions += ParamDescription;
			if (k < ParamList.size() - 1)
			{
				ParamDescriptions += _T("\r\n");
			}
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	ParamDescriptions.Replace(_T("\r\n"), Space);
	return ParamDescriptions;
}

CString CCallInterfaceMakerDlg::MakeParamDefines(vector<METHOD_PARAM>& ParamList, LPCTSTR SpaceName, LPCTSTR szSplitChar, LPCTSTR szLineSpace)
{
	CString ParamDefines;

	int MaxLen = 0;
	for (size_t i = 0;i < ParamList.size();i++)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(ParamList[i].Type);
		if (pTypeInfo)
		{
			if (pTypeInfo->CType.GetLength() > MaxLen)
				MaxLen = pTypeInfo->CType.GetLength();
		}
	}
	int SpaceLen = (((int)ceil(MaxLen / 4.0f)) + 1) * 4;

	for (METHOD_PARAM& ParamInfo : ParamList)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(ParamInfo.Type);
		if (pTypeInfo)
		{
			CString Param;
			if (ParamInfo.IsArray)
				Param = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.VariableDefine, pTypeInfo);
			else
				Param = pTypeInfo->GenerateOperations.VariableDefine;
			Param.Replace(_T("<SpaceName>"), SpaceName);
			Param.Replace(_T("<Variable>"), ParamInfo.Name);
			Param.Replace(_T("<Type>"), pTypeInfo->CType);
			CString Space;
			int CurSpaceCount = (int)ceil((SpaceLen - pTypeInfo->CType.GetLength()) / 4.0f);
			while (CurSpaceCount)
			{
				Space += _T("	");
				CurSpaceCount--;
			}
			Param.Replace(_T("<Space>"), Space);

			Param = m_InterfaceConfig.LocalVariableDefinePrefix + Param;

			ParamDefines += Param + szSplitChar;
			ParamDefines += _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	ParamDefines.Replace(_T("\r\n"), Space);
	return ParamDefines;
}

CString CCallInterfaceMakerDlg::MakeMethodsSource(CALLER_INTERFACE& InterfaceInfo, LPCTSTR SpaceName, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + _T("Ack");
	else
		InterfaceName = InterfaceInfo.Name;

	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString InterfaceNameUp = ClassNameToUpper(InterfaceName);
	CString ModuleID;
	CString InterfaceIDEnumName;
	CString InterfaceID;
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(InterfaceInfo.ModuleID);
	if (pModuleInfo)
	{
		InterfaceIDEnumName.Format(_T("%s_INTERFACES"), ClassNameToUpper(pModuleInfo->Name));
		InterfaceID.Format(_T("%s_INTERFACE_%s"), ClassNameToUpper(pModuleInfo->Name), ClassNameToUpper(InterfaceInfo.Name));
		ModuleID = _T("MODULE_ID_") + ClassNameToUpper(pModuleInfo->Name);
	}

	CString MethodIDEnumName;
	MethodIDEnumName.Format(_T("%s_METHODS"), ClassNameToUpper(InterfaceInfo.Name));

	CString Methods;
	for (size_t j = 0;j < InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];
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
				MethodName = MethodInfo.Name + _T("Ack");
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

			Method.Replace(_T("<ModuleID>"), ModuleID);
			Method.Replace(_T("<InterfaceName>"), InterfaceName);
			Method.Replace(_T("<MethodName>"), MethodName);
			Method.Replace(_T("<SpaceName>"), SpaceName);
			Method.Replace(_T("<!SpaceName>"), SpaceNameUp);
			Method.Replace(_T("<!InterfaceName>"), InterfaceNameUp);
			Method.Replace(_T("<!MethodName>"), MethodNameUp);
			Method.Replace(_T("<@InterfaceName>"), InterfaceInfo.Name);
			CString Params = MakeParams(*pParamList, true, true);
			Method.Replace(_T("<Params>"), Params);
			Params = MakeParams(*pParamList, false, true);
			if (!Params.IsEmpty())
				Params = _T(",") + Params;
			Method.Replace(_T("<,Params>"), Params);
			CString LineSpace = GetLineSpace(Method, _T("<PacketSizes>"));
			CString PackSizes = MakePackSizes(MethodInfo, ExportType, LineSpace);
			Method.Replace(_T("<PacketSizes>"), PackSizes);

			Method.Replace(_T("<InterfaceIDEnumName>"), InterfaceIDEnumName);
			Method.Replace(_T("<InterfaceID>"), InterfaceID);
			CString MethodID;
			MethodID.Format(_T("METHOD_%s"), ClassNameToUpper(MethodInfo.Name));
			Method.Replace(_T("<MethodID>"), MethodID);
			Method.Replace(_T("<MethodIDEnumName>"), MethodIDEnumName);
			SelectBlock(Method, _T("IsAckMsg"), ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK);

			if (MethodInfo.Flag & INTERFACE_METHOD_FLAG_CAN_CACHE)
			{
				Method.Replace(_T("<IfMsgCanCache>"), _T(""));
				Method.Replace(_T("</IfMsgCanCache>"), _T(""));
			}
			else
			{
				RemoveBlock(Method, _T("<IfMsgCanCache>"), _T("</IfMsgCanCache>"));
			}

			if (MethodInfo.Flag & INTERFACE_METHOD_FLAG_NO_COMPRESS)
			{
				Method.Replace(_T("<IfMsgNoCompress>"), _T(""));
				Method.Replace(_T("</IfMsgNoCompress>"), _T(""));
			}
			else
			{
				RemoveBlock(Method, _T("<IfMsgNoCompress>"), _T("</IfMsgNoCompress>"));
			}

			Methods += Method + _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgPackMethodsSource(LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + _T("Ack");
	else
		InterfaceName = InterfaceInfo.Name;

	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString InterfaceNameUp = ClassNameToUpper(InterfaceName);

	CString Methods;
	for (size_t j = 0;j < InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];
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
				MethodName = MethodInfo.Name + _T("Ack");
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
			Method.Replace(_T("<InterfaceName>"), InterfaceName);
			Method.Replace(_T("<MethodName>"), MethodName);
			Method.Replace(_T("<SpaceName>"), SpaceName);
			Method.Replace(_T("<!SpaceName>"), SpaceNameUp);
			Method.Replace(_T("<!InterfaceName>"), InterfaceNameUp);
			Method.Replace(_T("<!MethodName>"), MethodNameUp);
			Method.Replace(_T("<@InterfaceName>"), InterfaceInfo.Name);
			CString Params = MakeParams(*pParamList, true, true);
			if (!Params.IsEmpty())
				Params = _T(",") + Params;
			Method.Replace(_T("<,Params>"), Params);
			CString LineSpace = GetLineSpace(Method, _T("<PackOperations>"));
			CString PacketName = m_InterfaceConfig.DefaultPacketName;
			CString PacketMemberName = m_InterfaceConfig.DefaultPacketMemberName;
			CString SSTIDEnumName;
			SSTIDEnumName.Format(_T("%s_%s_MEMBER_IDS"), ClassNameToUpper(InterfaceName), ClassNameToUpper(MethodName));
			CString PackOperations = MakePackOperations(MethodInfo, ExportType, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
			Method.Replace(_T("<PackOperations>"), PackOperations);

			Methods += Method + _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeInitOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString InitOperations;

	vector<METHOD_PARAM>* pParamList = NULL;
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
		for (METHOD_PARAM& ParamInfo : *pParamList)
		{
			if (ParamInfo.IsArray)
				continue;
			TYPE_DEFINE* pTypeInfo = FindVarType(ParamInfo.Type);
			if (pTypeInfo)
			{
				if (!pTypeInfo->GenerateOperations.InitOperation.IsEmpty())
				{
					CString Operation = pTypeInfo->GenerateOperations.InitOperation;
					Operation.Replace(_T("<Type>"), pTypeInfo->CType);
					Operation.Replace(_T("<Variable>"), ParamInfo.Name);

					InitOperations += Operation;
					InitOperations += _T("\r\n");
				}
			}
		}
		CString Space = _T("\r\n");
		Space += szLineSpace;
		InitOperations.Replace(_T("\r\n"), Space);
	}

	return InitOperations;
}

CString CCallInterfaceMakerDlg::MakePackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace)
{
	CString PackOperations;
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
			MethodName = MethodInfo.Name + _T("Ack");
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
		for (METHOD_PARAM& ParamInfo : *pParamList)
		{

			TYPE_DEFINE* pTypeInfo = FindVarType(ParamInfo.Type);
			if (pTypeInfo)
			{
				CString Operation = m_PackOperationUnitTemplate;
				CString LocalPacketName = PacketName;
				CString LocalPacketMemberName = PacketMemberName;
				if (ParamInfo.IsArray)
				{
					Operation = m_InterfaceConfig.ArrayDefineConfig.PackOperation;
					Operation.Replace(_T("<Packet>"), PacketName);
					Operation.Replace(_T("<PacketMember>"), PacketMemberName);
					CheckNameChange(Operation, LocalPacketName, _T("<ChangePacketName="));
					CheckNameChange(Operation, LocalPacketMemberName, _T("<ChangePacketMemberName="));
					Operation = ProcessArrayOperation(Operation, pTypeInfo);
				}

				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));


				Operation.Replace(_T("<Variable>"), ParamInfo.Name);

				CString SST_ID;
				CString ParentShortName = ClassNameToUpper(MethodName);
				SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(ParamInfo.Name));
				SST_ID.MakeUpper();
				Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
				Operation.Replace(_T("<SST_ID>"), SST_ID);

				if (ParamInfo.IsArray)
					Operation.Replace(_T("<OrginType>"), pTypeInfo->Name + _T("[]"));
				else
					Operation.Replace(_T("<OrginType>"), pTypeInfo->Name);

				CString LineSpace = GetLineSpace(Operation, _T("<PackOperation>"));
				UINT OperationFlag = PACK_OPERATION_FLAG_IN_INTERFACE | ((ParamInfo.IsArray) ? PACK_OPERATION_FLAG_IS_ARRAY : 0);
				CString PackOP = MakePackOperation(ParamInfo.Type,
					ParamInfo.Name,
					MethodName, ParentShortName,
					SSTIDEnumName, ParamInfo.PackFlag,
					LocalPacketName, LocalPacketMemberName,
					OperationFlag,
					SpaceName, LineSpace);

				Operation.Replace(_T("<PackOperation>"), PackOP);

				PackOperations += Operation;
				PackOperations += _T("\r\n");
			}
		}
		CString Space = _T("\r\n");
		Space += szLineSpace;
		PackOperations.Replace(_T("\r\n"), Space);
	}
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakeUnpackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace)
{
	CString PackOperations;
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
			MethodName = MethodInfo.Name + _T("Ack");
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
		for (size_t i = 0;i < pParamList->size();i++)
		{
			METHOD_PARAM& ParamInfo = (*pParamList)[i];
			TYPE_DEFINE* pTypeInfo = FindVarType(ParamInfo.Type);
			if (pTypeInfo)
			{
				CString Operation = m_UnpackOperationUnitTemplate;
				CString LocalPacketName = PacketName;
				CString LocalPacketMemberName = PacketMemberName;
				if (ParamInfo.IsArray)
				{
					Operation = m_InterfaceConfig.ArrayDefineConfig.UnpackOperation;

					CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
					VariableDefine.Replace(_T("<Type>"), pTypeInfo->CType);
					VariableDefine.Replace(_T("<Space>"), _T("	"));
					VariableDefine.Replace(_T("<Variable>"), _T("ArrayElement"));
					VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
					Operation.Replace(_T("<ArrayElementVariableDefine>"), VariableDefine);

					CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
					VariableInit.Replace(_T("<Variable>"), _T("ArrayElement"));
					Operation.Replace(_T("<ArrayElementVariableInit>"), VariableInit);

					Operation.Replace(_T("<Packet>"), PacketName);
					Operation.Replace(_T("<PacketMember>"), PacketMemberName);
					CheckNameChange(Operation, LocalPacketName, _T("<ChangePacketName="));
					CheckNameChange(Operation, LocalPacketMemberName, _T("<ChangePacketMemberName="));
					Operation = ProcessArrayOperation(Operation, pTypeInfo);
				}

				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				RemoveBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));


				CString SST_ID;

				CString ParentShortName = ClassNameToUpper(MethodName);
				SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(ParamInfo.Name));
				SST_ID.MakeUpper();


				Operation.Replace(_T("<SpaceName>"), SpaceName);
				Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
				Operation.Replace(_T("<SST_ID>"), SST_ID);

				Operation.Replace(_T("<Variable>"), ParamInfo.Name);


				CString LineSpace = GetLineSpace(Operation, _T("<PackOperation>"));
				UINT OperationFlag = PACK_OPERATION_FLAG_IN_INTERFACE | ((ParamInfo.IsArray) ? PACK_OPERATION_FLAG_IS_ARRAY : 0);
				CString UnpackOP = MakeUnpackOperation(ParamInfo.Type,
					ParamInfo.Name,
					MethodName, ParentShortName,
					SSTIDEnumName, ParamInfo.PackFlag,
					LocalPacketName, LocalPacketMemberName,
					OperationFlag,
					SpaceName, LineSpace);

				Operation.Replace(_T("<UnpackOperation>"), UnpackOP);

				if (i < pParamList->size() - 1)
				{
					Operation.Replace(_T("<IfNotLastItem>"), _T(""));
					Operation.Replace(_T("</IfNotLastItem>"), _T(""));
				}
				else
				{
					RemoveBlock(Operation, _T("<IfNotLastItem>"), _T("</IfNotLastItem>"));
				}

				PackOperations += Operation;
				PackOperations += _T("\r\n");
			}
		}
		CString Space = _T("\r\n");
		Space += szLineSpace;
		PackOperations.Replace(_T("\r\n"), Space);
	}
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakePackSizes(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString PackSizes;

	vector<METHOD_PARAM>* pParamList = NULL;
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
		for (METHOD_PARAM& ParamInfo : *pParamList)
		{

			TYPE_DEFINE* pTypeInfo = FindVarType(ParamInfo.Type);
			if (pTypeInfo)
			{
				CString PackSize = m_PackSizeTemple;
				CString Operation = pTypeInfo->GenerateOperations.SizeCaculateOperation;


				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));

				if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
				{
					RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
					Operation.Replace(_T("<IfDataObject>"), _T(""));
					Operation.Replace(_T("</IfDataObject>"), _T(""));
				}
				else
				{
					RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
					Operation.Replace(_T("<IfNotDataObject>"), _T(""));
					Operation.Replace(_T("</IfNotDataObject>"), _T(""));
				}

				if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
				{
					RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
					Operation.Replace(_T("<If64Bit>"), _T(""));
					Operation.Replace(_T("</If64Bit>"), _T(""));
				}
				else
				{
					RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
					Operation.Replace(_T("<IfNot64Bit>"), _T(""));
					Operation.Replace(_T("</IfNot64Bit>"), _T(""));
				}

				Operation.Replace(_T("<PackFlag>"), ParamInfo.PackFlag);

				if (ParamInfo.IsArray)
				{
					CString Var = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
					Var.Replace(_T("<Variable>"), ParamInfo.Name);
					Var.Replace(_T("<Index>"), _T("i"));
					Operation.Replace(_T("<Variable>"), Var);
					Operation.Replace(_T("<Type>"), pTypeInfo->CType);

					CString ArrayOP = m_InterfaceConfig.ArrayDefineConfig.SizeCaculateOperation;

					ArrayOP.Replace(_T("<Variable>"), ParamInfo.Name);
					ArrayOP.Replace(_T("<ArrayElementSize>"), Operation);
					RemoveBlock(ArrayOP, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
					ArrayOP.Replace(_T("<IfInArray>"), _T(""));
					ArrayOP.Replace(_T("</IfInArray>"), _T(""));

					PackSize = ProcessArrayOperation(ArrayOP, pTypeInfo);
				}
				else
				{
					Operation.Replace(_T("<Variable>"), ParamInfo.Name);
					Operation.Replace(_T("<Type>"), pTypeInfo->CType);
					RemoveBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
					Operation.Replace(_T("<IfNotInArray>"), _T(""));
					Operation.Replace(_T("</IfNotInArray>"), _T(""));
					PackSize.Replace(_T("<PackSize>"), Operation);
				}

				RemoveBlock(PackSize, _T("<IfCheckMF>"), _T("</IfCheckMF>"));


				PackSizes += PackSize;
				PackSizes += _T("\r\n");
			}
		}
		CString Space = _T("\r\n");
		Space += szLineSpace;
		PackSizes.Replace(_T("\r\n"), Space);
	}
	return PackSizes;
}

CString CCallInterfaceMakerDlg::MakeInitOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString InitOperations;

	for (size_t i = 0;i < StructInfo.MemberList.size();i++)
	{

		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			if (!pTypeInfo->GenerateOperations.InitOperation.IsEmpty())
			{
				CString Operation = pTypeInfo->GenerateOperations.InitOperation;
				if (StructInfo.MemberList[i].IsArray)
				{
					Operation = m_InterfaceConfig.ArrayDefineConfig.InitOperation;
					CString Temp;
					Temp.Format(_T("%u"), StructInfo.MemberList[i].ArrayStartLength);
					Operation.Replace(_T("<ArrayStartLength>"), Temp);
					Temp.Format(_T("%u"), StructInfo.MemberList[i].ArrayGrowLength);
					Operation.Replace(_T("<ArrayGrowLength>"), Temp);

					Operation = ProcessArrayOperation(Operation, pTypeInfo);
				}
				CString VarName = StructInfo.MemberList[i].Name;
				if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
					VarName = _T("m_") + VarName;
				VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
				Operation.Replace(_T("<Variable>"), VarName);
				Operation.Replace(_T("<Type>"), pTypeInfo->CType);

				InitOperations += Operation;
				InitOperations += _T("\r\n");
			}
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	InitOperations.Replace(_T("\r\n"), Space);
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
			Operation.Replace(_T("<Packet>"), PacketName);
			Operation.Replace(_T("<PacketMember>"), PacketMemberName);
			CheckNameChange(Operation, LocalPacketName, _T("<ChangePacketName="));
			CheckNameChange(Operation, LocalPacketMemberName, _T("<ChangePacketMemberName="));
			TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
			Operation = ProcessArrayOperation(Operation, pTypeInfo);
		}

		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			RetainBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				RetainBlock(Operation, _T("<IfNotCheckMF>"), _T("</IfNotCheckMF>"));
			}
			else
			{
				RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				RemoveBlock(Operation, _T("<IfNotCheckMF>"), _T("</IfNotCheckMF>"));
			}
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			RetainBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			RetainBlock(Operation, _T("<IfNotCheckMF>"), _T("</IfNotCheckMF>"));
		}

		CString VarName = StructInfo.MemberList[i].Name;
		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			VarName = _T("m_") + VarName;
		VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
		Operation.Replace(_T("<Variable>"), VarName);

		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			if (StructInfo.MemberList[i].IsArray)
				Operation.Replace(_T("<OrginType>"), pTypeInfo->Name + _T("[]"));
			else
				Operation.Replace(_T("<OrginType>"), pTypeInfo->Name);
		}


		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(StructInfo.MemberList[i].Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);

		CString LineSpace = GetLineSpace(Operation, _T("<PackOperation>"));
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

		Operation.Replace(_T("<PackOperation>"), PackOP);

		CString SST_ID;

		SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
		SST_ID.MakeUpper();


		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
		Operation.Replace(_T("<SST_ID>"), SST_ID);


		PackOperations += Operation;
		PackOperations += _T("\r\n");
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	PackOperations.Replace(_T("\r\n"), Space);
	return PackOperations;
}
CString CCallInterfaceMakerDlg::MakeUnpackOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace)
{
	CString PackOperations;

	for (size_t i = 0;i < StructInfo.MemberList.size();i++)
	{

		if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			CString Operation = m_UnpackOperationUnitTemplate;
			CString LocalPacketName = PacketName;
			CString LocalPacketMemberName = PacketMemberName;
			if (StructInfo.MemberList[i].IsArray)
			{
				Operation = m_InterfaceConfig.ArrayDefineConfig.UnpackOperation;

				CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
				VariableDefine.Replace(_T("<Type>"), pTypeInfo->CType);
				VariableDefine.Replace(_T("<Space>"), _T("	"));
				VariableDefine.Replace(_T("<Variable>"), _T("ArrayElement"));
				VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
				Operation.Replace(_T("<ArrayElementVariableDefine>"), VariableDefine);

				CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
				VariableInit.Replace(_T("<Variable>"), _T("ArrayElement"));
				Operation.Replace(_T("<ArrayElementVariableInit>"), VariableInit);

				Operation.Replace(_T("<Packet>"), PacketName);
				Operation.Replace(_T("<PacketMember>"), PacketMemberName);
				CheckNameChange(Operation, LocalPacketName, _T("<ChangePacketName="));
				CheckNameChange(Operation, LocalPacketMemberName, _T("<ChangePacketMemberName="));
				Operation = ProcessArrayOperation(Operation, pTypeInfo);
			}

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
				Operation.Replace(_T("<IfInDataObject>"), _T(""));
				Operation.Replace(_T("</IfInDataObject>"), _T(""));
				if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				else
				{
					RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				RetainBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
			}
			else
			{
				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				RemoveBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
			}

			CString SST_ID;

			SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
			SST_ID.MakeUpper();


			Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
			Operation.Replace(_T("<SST_ID>"), SST_ID);

			CString VarName = StructInfo.MemberList[i].Name;

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				VarName = _T("m_") + VarName;

			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
			Operation.Replace(_T("<Variable>"), VarName);



			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
			Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

			CString ModifyFlag = _T("MF_") + ClassNameToUpper(StructInfo.MemberList[i].Name);
			Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);
			Operation.Replace(_T("<SpaceName>"), SpaceName);

			CString LineSpace = GetLineSpace(Operation, _T("<UnpackOperation>"));
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

			Operation.Replace(_T("<UnpackOperation>"), UnpackOP);

			//if (i < StructInfo.MemberList.size() - 1)
			//{
			//	RemoveBlock(Operation, _T("<IfLastItem>"), _T("</IfLastItem>"));
			//}
			//else
			//{
			//	RetainBlock(Operation, _T("<IfLastItem>"), _T("</IfLastItem>"));
			//}

			PackOperations += Operation;
			PackOperations += _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	PackOperations.Replace(_T("\r\n"), Space);
	return PackOperations;
}

CString CCallInterfaceMakerDlg::MakePackSizes(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString PackSizes;

	for (size_t i = 0;i < StructInfo.MemberList.size();i++)
	{
		if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			CString PackSize = m_PackSizeTemple;
			CString Operation = pTypeInfo->GenerateOperations.SizeCaculateOperation;
			CString Variable;

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
				Operation.Replace(_T("<IfInDataObject>"), _T(""));
				Operation.Replace(_T("</IfInDataObject>"), _T(""));

				Variable = _T("m_") + StructInfo.MemberList[i].Name;

			}
			else
			{
				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));

				Variable = StructInfo.MemberList[i].Name;
			}

			Variable = m_InterfaceConfig.MemberVariablePrefix + Variable;

			if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
				Operation.Replace(_T("<IfDataObject>"), _T(""));
				Operation.Replace(_T("</IfDataObject>"), _T(""));
			}
			else
			{
				RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
				Operation.Replace(_T("<IfNotDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotDataObject>"), _T(""));
			}

			if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
			{
				RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
				Operation.Replace(_T("<If64Bit>"), _T(""));
				Operation.Replace(_T("</If64Bit>"), _T(""));
			}
			else
			{
				RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
				Operation.Replace(_T("<IfNot64Bit>"), _T(""));
				Operation.Replace(_T("</IfNot64Bit>"), _T(""));
			}

			Operation.Replace(_T("<PackFlag>"), StructInfo.MemberList[i].PackFlag);

			if (StructInfo.MemberList[i].IsArray)
			{
				CString Var = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
				Var.Replace(_T("<Variable>"), Variable);
				Var.Replace(_T("<Index>"), _T("i"));
				Operation.Replace(_T("<Variable>"), Var);
				Operation.Replace(_T("<Type>"), pTypeInfo->CType);

				CString ArrayOP = m_InterfaceConfig.ArrayDefineConfig.SizeCaculateOperation;

				ArrayOP.Replace(_T("<Variable>"), Variable);
				ArrayOP.Replace(_T("<ArrayElementSize>"), Operation);
				RemoveBlock(ArrayOP, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
				ArrayOP.Replace(_T("<IfInArray>"), _T(""));
				ArrayOP.Replace(_T("</IfInArray>"), _T(""));

				PackSize = ProcessArrayOperation(ArrayOP, pTypeInfo);
			}
			else
			{
				Operation.Replace(_T("<Variable>"), Variable);
				Operation.Replace(_T("<Type>"), pTypeInfo->CType);
				RemoveBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
				Operation.Replace(_T("<IfNotInArray>"), _T(""));
				Operation.Replace(_T("</IfNotInArray>"), _T(""));
				PackSize.Replace(_T("<PackSize>"), Operation);
			}



			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(PackSize, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				else
				{
					RetainBlock(PackSize, _T("<IfCheckMF>"), _T("</IfCheckMF>"));

					CString ModifyFlagEnumName;
					ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
					PackSize.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

					CString ModifyFlag = _T("MF_") + ClassNameToUpper(StructInfo.MemberList[i].Name);
					PackSize.Replace(_T("<ModifyFlag>"), ModifyFlag);

					PackSize.Replace(_T("<SpaceName>"), SpaceName);
				}
			}
			else
			{
				RemoveBlock(PackSize, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}


			PackSizes += PackSize;
			PackSizes += _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	PackSizes.Replace(_T("\r\n"), Space);
	return PackSizes;
}

CString CCallInterfaceMakerDlg::MakeXMLProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString XMLProcess = m_StructXMLProcessTemplate;

	CString LineSpace = GetLineSpace(XMLProcess, _T("<ToXMLOperations>"));
	CString ToXMLOperations = MakeToXMLOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	XMLProcess.Replace(_T("<ToXMLOperations>"), ToXMLOperations);

	LineSpace = GetLineSpace(XMLProcess, _T("<FromXMLOperations>"));
	CString FromXMLOperations = MakeFromXMLOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	XMLProcess.Replace(_T("<FromXMLOperations>"), FromXMLOperations);

	CString Space = _T("\r\n");
	Space += szLineSpace;
	XMLProcess.Replace(_T("\r\n"), Space);
	return XMLProcess;

}

CString CCallInterfaceMakerDlg::MakeToXMLOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation = m_ToXMLOperationUnitTemplate;
		if (StructInfo.MemberList[i].IsArray)
		{
			TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
			Operation = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.ToXMLOperation, pTypeInfo);
		}

		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
			if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}
			else
			{
				RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
			RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
		}

		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			Operation.Replace(_T("<Variable>"), _T("m_") + StructInfo.MemberList[i].Name);
		else
			Operation.Replace(_T("<Variable>"), StructInfo.MemberList[i].Name);

		Operation.Replace(_T("<VariableName>"), StructInfo.MemberList[i].Name);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(StructInfo.MemberList[i].Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);

		CString LineSpace = GetLineSpace(Operation, _T("<ToXMLOperation>"));
		CString PackOP = MakeToXMLOperation(StructInfo.MemberList[i].Type,
			StructInfo.MemberList[i].Name,
			StructInfo.Name, StructInfo.ShortName,
			SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
			(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0,
			(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
			StructInfo.MemberList[i].IsArray,
			SpaceName, LineSpace);

		Operation.Replace(_T("<ToXMLOperation>"), PackOP);

		CString SST_ID;

		SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
		SST_ID.MakeUpper();


		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
		Operation.Replace(_T("<SST_ID>"), SST_ID);


		Operations += Operation;
		Operations += _T("\r\n");
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeFromXMLOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{

		if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			CString Operation = m_FromXMLOperationUnitTemplate;
			if (StructInfo.MemberList[i].IsArray)
			{
				Operation = m_InterfaceConfig.ArrayDefineConfig.FromXMLOperation;

				CString Space;

				CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
				VariableDefine.Replace(_T("<Type>"), pTypeInfo->CType);
				VariableDefine.Replace(_T("<Space>"), _T("	"));
				VariableDefine.Replace(_T("<Variable>"), _T("ArrayElement"));
				VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
				Operation.Replace(_T("<ArrayElementVariableDefine>"), VariableDefine);

				CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
				VariableInit.Replace(_T("<Variable>"), _T("ArrayElement"));
				Operation.Replace(_T("<ArrayElementVariableInit>"), VariableInit);

				Operation = ProcessArrayOperation(Operation, pTypeInfo);
			}

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
				Operation.Replace(_T("<IfInDataObject>"), _T(""));
				Operation.Replace(_T("</IfInDataObject>"), _T(""));
				if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				else
				{
					RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
			}
			else
			{
				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}

			CString SST_ID;

			SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
			SST_ID.MakeUpper();


			Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
			Operation.Replace(_T("<SST_ID>"), SST_ID);

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				Operation.Replace(_T("<Variable>"), _T("m_") + StructInfo.MemberList[i].Name);
			else
				Operation.Replace(_T("<Variable>"), StructInfo.MemberList[i].Name);

			Operation.Replace(_T("<VariableName>"), StructInfo.MemberList[i].Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
			Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

			CString ModifyFlag = _T("MF_") + ClassNameToUpper(StructInfo.MemberList[i].Name);
			Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

			Operation.Replace(_T("<SpaceName>"), SpaceName);

			CString LineSpace = GetLineSpace(Operation, _T("<FromXMLOperation>"));
			CString UnpackOP = MakeFromXMLOperation(StructInfo.MemberList[i].Type,
				StructInfo.MemberList[i].Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
				(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0,
				(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
				StructInfo.MemberList[i].IsArray,
				SpaceName, LineSpace);

			Operation.Replace(_T("<FromXMLOperation>"), UnpackOP);

			Operations += Operation;
			Operations += _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}


CString CCallInterfaceMakerDlg::MakeJsonProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString JsonProcess = m_StructJsonProcessTemplate;

	CString LineSpace = GetLineSpace(JsonProcess, _T("<ToJsonOperations>"));
	CString ToJsonOperations = MakeToJsonOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	JsonProcess.Replace(_T("<ToJsonOperations>"), ToJsonOperations);

	LineSpace = GetLineSpace(JsonProcess, _T("<FromJsonOperations>"));
	CString FromJsonOperations = MakeFromJsonOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	JsonProcess.Replace(_T("<FromJsonOperations>"), FromJsonOperations);

	CString Space = _T("\r\n");
	Space += szLineSpace;
	JsonProcess.Replace(_T("\r\n"), Space);
	return JsonProcess;
}

CString CCallInterfaceMakerDlg::MakeDataObjectJsonProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString JsonProcess = m_DataObjectJsonProcessTemplate;

	if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
	{
		RemoveBlock(JsonProcess, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
		JsonProcess.Replace(_T("<IfInDataObject>"), _T(""));
		JsonProcess.Replace(_T("</IfInDataObject>"), _T(""));
	}
	else
	{
		RemoveBlock(JsonProcess, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
		JsonProcess.Replace(_T("<IfNotInDataObject>"), _T(""));
		JsonProcess.Replace(_T("</IfNotInDataObject>"), _T(""));
	}

	CString LineSpace = GetLineSpace(JsonProcess, _T("<ToJsonOperations>"));
	CString ToJsonOperations = MakeToJsonOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	JsonProcess.Replace(_T("<ToJsonOperations>"), ToJsonOperations);

	LineSpace = GetLineSpace(JsonProcess, _T("<FromJsonOperations>"));
	CString FromJsonOperations = MakeFromJsonOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	JsonProcess.Replace(_T("<FromJsonOperations>"), FromJsonOperations);

	CString Space = _T("\r\n");
	Space += szLineSpace;
	JsonProcess.Replace(_T("\r\n"), Space);
	return JsonProcess;
}
CString CCallInterfaceMakerDlg::MakeToJsonOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation = m_ToJsonOperationUnitTemplate;
		if (StructInfo.MemberList[i].IsArray)
		{
			TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
			Operation = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.ToJsonOperation, pTypeInfo);
		}

		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
			if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}
			else
			{
				RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
			RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
		}

		CString VarName = StructInfo.MemberList[i].Name;
		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			VarName = _T("m_") + VarName;
		VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
		Operation.Replace(_T("<Variable>"), VarName);

		Operation.Replace(_T("<VariableName>"), StructInfo.MemberList[i].Name);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(StructInfo.MemberList[i].Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);

		CString LineSpace = GetLineSpace(Operation, _T("<ToJsonOperation>"));
		CString PackOP = MakeToJsonOperation(StructInfo.MemberList[i].Type,
			StructInfo.MemberList[i].Name,
			StructInfo.Name, StructInfo.ShortName,
			SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
			(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0,
			(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
			StructInfo.MemberList[i].IsArray,
			SpaceName, LineSpace);

		Operation.Replace(_T("<ToJsonOperation>"), PackOP);

		CString SST_ID;

		SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
		SST_ID.MakeUpper();


		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
		Operation.Replace(_T("<SST_ID>"), SST_ID);


		Operations += Operation;
		Operations += _T("\r\n");
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeFromJsonOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{

		if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			CString Operation = m_FromJsonOperationUnitTemplate;
			if (StructInfo.MemberList[i].IsArray)
			{
				Operation = m_InterfaceConfig.ArrayDefineConfig.FromJsonOperation;

				CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
				VariableDefine.Replace(_T("<Type>"), pTypeInfo->CType);
				VariableDefine.Replace(_T("<Space>"), _T("	"));
				VariableDefine.Replace(_T("<Variable>"), _T("ArrayElement"));
				VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
				Operation.Replace(_T("<ArrayElementVariableDefine>"), VariableDefine);

				CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
				VariableInit.Replace(_T("<Variable>"), _T("ArrayElement"));
				Operation.Replace(_T("<ArrayElementVariableInit>"), VariableInit);

				Operation = ProcessArrayOperation(Operation, pTypeInfo);
			}

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
				Operation.Replace(_T("<IfInDataObject>"), _T(""));
				Operation.Replace(_T("</IfInDataObject>"), _T(""));
				if (StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				else
				{
					RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				RetainBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
			}
			else
			{
				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				RemoveBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
			}

			CString SST_ID;

			SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(StructInfo.MemberList[i].Name));
			SST_ID.MakeUpper();


			Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
			Operation.Replace(_T("<SST_ID>"), SST_ID);

			CString VarName = StructInfo.MemberList[i].Name;
			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				VarName = _T("m_") + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
			Operation.Replace(_T("<Variable>"), VarName);

			Operation.Replace(_T("<VariableName>"), StructInfo.MemberList[i].Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
			Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

			CString ModifyFlag = _T("MF_") + ClassNameToUpper(StructInfo.MemberList[i].Name);
			Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

			Operation.Replace(_T("<SpaceName>"), SpaceName);

			CString LineSpace = GetLineSpace(Operation, _T("<FromJsonOperation>"));
			CString UnpackOP = MakeFromJsonOperation(StructInfo.MemberList[i].Type,
				StructInfo.MemberList[i].Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName, StructInfo.MemberList[i].PackFlag,
				(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0,
				(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0 && ((StructInfo.MemberList[i].Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0),
				StructInfo.MemberList[i].IsArray,
				SpaceName, LineSpace);

			Operation.Replace(_T("<FromJsonOperation>"), UnpackOP);

			Operations += Operation;
			Operations += _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}

CString CCallInterfaceMakerDlg::MakePackOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, LPCTSTR PacketName, LPCTSTR PacketMemberName, UINT OperationFlag, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE* pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.PackOperation;

		//if (OperationFlag & PACK_OPERATION_FLAG_NEED_MF_CHECK)
		//{
		//	RemoveBlock(Operation, _T("<IfNotMFCheck>"), _T("</IfNotMFCheck>"));
		//	Operation.Replace(_T("<IfMFCheck>"), _T(""));
		//	Operation.Replace(_T("</IfMFCheck>"), _T(""));
		//}
		//else
		//{
		//	RemoveBlock(Operation, _T("<IfMFCheck>"), _T("</IfMFCheck>"));
		//	Operation.Replace(_T("<IfNotMFCheck>"), _T(""));
		//	Operation.Replace(_T("</IfNotMFCheck>"), _T(""));
		//}

		if (OperationFlag & PACK_OPERATION_FLAG_IN_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
		}
		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
		{
			RemoveBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
			Operation.Replace(_T("<IfNotInArray>"), _T(""));
			Operation.Replace(_T("</IfNotInArray>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
			Operation.Replace(_T("<IfNotInArray>"), _T(""));
			Operation.Replace(_T("</IfNotInArray>"), _T(""));
		}


		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
			Operation.Replace(_T("<IfDataObject>"), _T(""));
			Operation.Replace(_T("</IfDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
			Operation.Replace(_T("<IfNotDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
			Operation.Replace(_T("<If64Bit>"), _T(""));
			Operation.Replace(_T("</If64Bit>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
			Operation.Replace(_T("<IfNot64Bit>"), _T(""));
			Operation.Replace(_T("</IfNot64Bit>"), _T(""));
		}

		CString SST_ID;


		SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();



		//CString VarName=Name;
		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
		{
			CString ArrayVar = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
			ArrayVar.Replace(_T("<Index>"), _T("i"));
			Operation.Replace(_T("<Variable>"), ArrayVar);
			Operation = ProcessArrayOperation(Operation, pTypeInfo);
		}

		CString VarName = Name;
		if ((OperationFlag & PACK_OPERATION_FLAG_IN_INTERFACE) == 0)
		{
			if (OperationFlag & PACK_OPERATION_FLAG_IN_DATA_OBJECT)
				VarName = _T("m_") + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
		}

		Operation.Replace(_T("<Variable>"), VarName);

		Operation.Replace(_T("<VariableName>"), Name);
		Operation.Replace(_T("<Type>"), pTypeInfo->CType);
		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
			Operation.Replace(_T("<OrginType>"), pTypeInfo->Name + _T("[]"));
		else
			Operation.Replace(_T("<OrginType>"), pTypeInfo->Name);

		Operation.Replace(_T("<PackFlag>"), PackFlag);
		Operation.Replace(_T("<Packet>"), PacketName);
		Operation.Replace(_T("<PacketMember>"), PacketMemberName);

		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
		Operation.Replace(_T("<SST_ID>"), SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ParentName));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operation.Replace(_T("\r\n"), Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeUnpackOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, LPCTSTR PacketName, LPCTSTR PacketMemberName, UINT OperationFlag, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE* pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.UnpackOperation;

		if (OperationFlag & PACK_OPERATION_FLAG_IN_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
		}

		if (OperationFlag & PACK_OPERATION_FLAG_IN_INTERFACE)
		{
			RemoveBlock(Operation, _T("<IfNotInInterface>"), _T("</IfNotInInterface>"));
			Operation.Replace(_T("<IfInInterface>"), _T(""));
			Operation.Replace(_T("</IfInInterface>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInInterface>"), _T("</IfInInterface>"));
			Operation.Replace(_T("<IfNotInInterface>"), _T(""));
			Operation.Replace(_T("</IfNotInInterface>"), _T(""));
		}


		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
			Operation.Replace(_T("<IfDataObject>"), _T(""));
			Operation.Replace(_T("</IfDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
			Operation.Replace(_T("<IfNotDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
			Operation.Replace(_T("<If64Bit>"), _T(""));
			Operation.Replace(_T("</If64Bit>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
			Operation.Replace(_T("<IfNot64Bit>"), _T(""));
			Operation.Replace(_T("</IfNot64Bit>"), _T(""));
		}

		CString SST_ID;
		SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();


		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
		{
			Operation.Replace(_T("<Variable>"), _T("ArrayElement"));
		}

		CString VarName = Name;
		if ((OperationFlag & PACK_OPERATION_FLAG_IN_INTERFACE) == 0)
		{
			if (OperationFlag & PACK_OPERATION_FLAG_IN_DATA_OBJECT)
				VarName = _T("m_") + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
		}

		Operation.Replace(_T("<Variable>"), VarName);

		Operation.Replace(_T("<VariableName>"), Name);
		Operation.Replace(_T("<Type>"), pTypeInfo->CType);
		if (OperationFlag & PACK_OPERATION_FLAG_IS_ARRAY)
			Operation.Replace(_T("<OrginType>"), pTypeInfo->Name + _T("[]"));
		else
			Operation.Replace(_T("<OrginType>"), pTypeInfo->Name);

		Operation.Replace(_T("<PackFlag>"), PackFlag);
		Operation.Replace(_T("<Packet>"), PacketName);
		Operation.Replace(_T("<PacketMember>"), PacketMemberName);

		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
		Operation.Replace(_T("<SST_ID>"), SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ParentName));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operation.Replace(_T("\r\n"), Space);
	return Operation;
}
CString CCallInterfaceMakerDlg::MakeToXMLOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE* pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.ToXMLOperation;

		//if (IsMFCheck)
		//{
		//	RemoveBlock(Operation, _T("<IfNotMFCheck>"), _T("</IfNotMFCheck>"));
		//	Operation.Replace(_T("<IfMFCheck>"), _T(""));
		//	Operation.Replace(_T("</IfMFCheck>"), _T(""));
		//}
		//else
		//{
		//	RemoveBlock(Operation, _T("<IfMFCheck>"), _T("</IfMFCheck>"));
		//	Operation.Replace(_T("<IfNotMFCheck>"), _T(""));
		//	Operation.Replace(_T("</IfNotMFCheck>"), _T(""));
		//}

		if (IsInDataObject)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
			Operation.Replace(_T("<IfDataObject>"), _T(""));
			Operation.Replace(_T("</IfDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
			Operation.Replace(_T("<IfNotDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
			Operation.Replace(_T("<If64Bit>"), _T(""));
			Operation.Replace(_T("</If64Bit>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
			Operation.Replace(_T("<IfNot64Bit>"), _T(""));
			Operation.Replace(_T("</IfNot64Bit>"), _T(""));
		}

		CString SST_ID;


		SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();



		//CString VarName=Name;
		SelectBlock(Operation, _T("IsArray"), IsArray);
		if (IsArray)
		{
			CString ArrayVar = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
			ArrayVar.Replace(_T("<Index>"), _T("i"));
			Operation.Replace(_T("<Variable>"), ArrayVar);
			Operation = ProcessArrayOperation(Operation, pTypeInfo);
		}

		if (IsInDataObject)
			Operation.Replace(_T("<Variable>"), CString(_T("m_")) + Name);
		else
			Operation.Replace(_T("<Variable>"), Name);


		Operation.Replace(_T("<VariableName>"), Name);

		Operation.Replace(_T("<Type>"), pTypeInfo->CType);

		Operation.Replace(_T("<PackFlag>"), PackFlag);

		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
		Operation.Replace(_T("<SST_ID>"), SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ParentName));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operation.Replace(_T("\r\n"), Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeFromXMLOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE* pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.FromXMLOperation;

		if (IsMonitorUpdate)
		{
			RetainBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
		}

		if (IsInDataObject)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
			Operation.Replace(_T("<IfDataObject>"), _T(""));
			Operation.Replace(_T("</IfDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
			Operation.Replace(_T("<IfNotDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
			Operation.Replace(_T("<If64Bit>"), _T(""));
			Operation.Replace(_T("</If64Bit>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
			Operation.Replace(_T("<IfNot64Bit>"), _T(""));
			Operation.Replace(_T("</IfNot64Bit>"), _T(""));
		}

		CString SST_ID;
		SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();

		SelectBlock(Operation, _T("IsArray"), IsArray);
		if (IsArray)
		{
			Operation.Replace(_T("<Variable>"), _T("ArrayElement"));
		}

		if (IsInDataObject)
			Operation.Replace(_T("<Variable>"), CString(_T("m_")) + Name);
		else
			Operation.Replace(_T("<Variable>"), Name);


		Operation.Replace(_T("<VariableName>"), Name);
		Operation.Replace(_T("<Type>"), pTypeInfo->CType);

		Operation.Replace(_T("<PackFlag>"), PackFlag);

		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
		Operation.Replace(_T("<SST_ID>"), SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ParentName));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operation.Replace(_T("\r\n"), Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeToJsonOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE* pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.ToJsonOperation;

		//if (IsMFCheck)
		//{
		//	RemoveBlock(Operation, _T("<IfNotMFCheck>"), _T("</IfNotMFCheck>"));
		//	Operation.Replace(_T("<IfMFCheck>"), _T(""));
		//	Operation.Replace(_T("</IfMFCheck>"), _T(""));
		//}
		//else
		//{
		//	RemoveBlock(Operation, _T("<IfMFCheck>"), _T("</IfMFCheck>"));
		//	Operation.Replace(_T("<IfNotMFCheck>"), _T(""));
		//	Operation.Replace(_T("</IfNotMFCheck>"), _T(""));
		//}

		if (IsInDataObject)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
			Operation.Replace(_T("<IfDataObject>"), _T(""));
			Operation.Replace(_T("</IfDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
			Operation.Replace(_T("<IfNotDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
			Operation.Replace(_T("<If64Bit>"), _T(""));
			Operation.Replace(_T("</If64Bit>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
			Operation.Replace(_T("<IfNot64Bit>"), _T(""));
			Operation.Replace(_T("</IfNot64Bit>"), _T(""));
		}

		CString SST_ID;


		SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();



		//CString VarName=Name;
		SelectBlock(Operation, _T("IsArray"), IsArray);
		if (IsArray)
		{
			CString ArrayVar = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
			ArrayVar.Replace(_T("<Index>"), _T("i"));
			Operation.Replace(_T("<Variable>"), ArrayVar);
			Operation = ProcessArrayOperation(Operation, pTypeInfo);
		}

		CString VarName = Name;
		if (IsInDataObject)
			VarName = _T("m_") + VarName;
		VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;

		Operation.Replace(_T("<Variable>"), VarName);

		Operation.Replace(_T("<VariableName>"), Name);

		Operation.Replace(_T("<Type>"), pTypeInfo->CType);

		Operation.Replace(_T("<PackFlag>"), PackFlag);

		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
		Operation.Replace(_T("<SST_ID>"), SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ParentName));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operation.Replace(_T("\r\n"), Space);
	return Operation;
}
CString CCallInterfaceMakerDlg::MakeFromJsonOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE* pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.FromJsonOperation;

		if (IsMonitorUpdate)
		{
			RetainBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
		}

		if (IsInDataObject)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
			Operation.Replace(_T("<IfDataObject>"), _T(""));
			Operation.Replace(_T("</IfDataObject>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
			Operation.Replace(_T("<IfNotDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotDataObject>"), _T(""));
		}

		if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
		{
			RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
			Operation.Replace(_T("<If64Bit>"), _T(""));
			Operation.Replace(_T("</If64Bit>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
			Operation.Replace(_T("<IfNot64Bit>"), _T(""));
			Operation.Replace(_T("</IfNot64Bit>"), _T(""));
		}

		CString SST_ID;
		SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(Name));
		SST_ID.MakeUpper();

		SelectBlock(Operation, _T("IsArray"), IsArray);
		if (IsArray)
		{
			Operation.Replace(_T("<Variable>"), _T("ArrayElement"));
		}

		CString VarName = Name;
		if (IsInDataObject)
			VarName = _T("m_") + VarName;
		VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;

		Operation.Replace(_T("<Variable>"), VarName);

		Operation.Replace(_T("<VariableName>"), Name);
		Operation.Replace(_T("<Type>"), pTypeInfo->CType);

		Operation.Replace(_T("<PackFlag>"), PackFlag);

		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
		Operation.Replace(_T("<SST_ID>"), SST_ID);

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ParentName));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operation.Replace(_T("\r\n"), Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeDBProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString DBProcess = m_StructDBProcessTemplate;

	DBProcess.Replace(_T("<ClassName>"), StructInfo.Name);

	CString LineSpace = GetLineSpace(DBProcess, _T("<DBFieldDefineOperations>"));
	CString DBFieldDefineOperations = MakeDBFieldDefineOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace(_T("<DBFieldDefineOperations>"), DBFieldDefineOperations);

	LineSpace = GetLineSpace(DBProcess, _T("<DBIndexDefineOperations>"));
	CString DBIndexDefineOperations = MakeDBIndexDefineOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace(_T("<DBIndexDefineOperations>"), DBIndexDefineOperations);

	LineSpace = GetLineSpace(DBProcess, _T("<InsertFieldNames>"));
	CString InsertFieldNames = MakeDBFields(StructInfo, DB_INDEX_TYPE_NONE);
	DBProcess.Replace(_T("<InsertFieldNames>"), InsertFieldNames);

	LineSpace = GetLineSpace(DBProcess, _T("<InsertFieldFormats>"));
	CString InsertFieldFormats = MakeDBFieldFormats(StructInfo);
	DBProcess.Replace(_T("<InsertFieldFormats>"), InsertFieldFormats);

	LineSpace = GetLineSpace(DBProcess, _T("<DBPutParamBindOperations>"));
	CString DBPutParamBindOperations = MakeDBPutParamBindOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace(_T("<DBPutParamBindOperations>"), DBPutParamBindOperations);

	LineSpace = GetLineSpace(DBProcess, _T("<DBPutOperations>"));
	CString DBPutOperations;
	STRUCT_DEFINE_INFO* pStructInfo = &StructInfo;
	while (pStructInfo)
	{
		DBPutOperations = MakeDBPutOperations(*pStructInfo, SSTIDEnumName, SpaceName, LineSpace) + DBPutOperations;
		pStructInfo = GetStructDefineInfo(pStructInfo->BaseStruct);
	}
	DBProcess.Replace(_T("<DBPutOperations>"), DBPutOperations);

	CString Space = _T("\r\n");
	Space += szLineSpace;
	DBProcess.Replace(_T("\r\n"), Space);
	return DBProcess;
}
CString CCallInterfaceMakerDlg::MakeDataObjectDBProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString DBProcess = m_DataObjectDBProcessTemplate;

	CString LineSpace = GetLineSpace(DBProcess, _T("<DBFieldDefineOperations>"));
	CString DBFieldDefineOperations = MakeDBFieldDefineOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace(_T("<DBFieldDefineOperations>"), DBFieldDefineOperations);

	LineSpace = GetLineSpace(DBProcess, _T("<DBIndexDefineOperations>"));
	CString DBIndexDefineOperations = MakeDBIndexDefineOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace(_T("<DBIndexDefineOperations>"), DBIndexDefineOperations);

	LineSpace = GetLineSpace(DBProcess, _T("<InsertFieldNames>"));
	CString InsertFieldNames = MakeDBFields(StructInfo, DB_INDEX_TYPE_NONE);
	DBProcess.Replace(_T("<InsertFieldNames>"), InsertFieldNames);

	LineSpace = GetLineSpace(DBProcess, _T("<InsertFieldFormats>"));
	CString InsertFieldFormats = MakeDBFieldFormats(StructInfo);
	DBProcess.Replace(_T("<InsertFieldFormats>"), InsertFieldFormats);

	LineSpace = GetLineSpace(DBProcess, _T("<DBPutParamBindOperations>"));
	CString DBPutParamBindOperations = MakeDBPutParamBindOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace(_T("<DBPutParamBindOperations>"), DBPutParamBindOperations);

	LineSpace = GetLineSpace(DBProcess, _T("<DBPutOperations>"));
	CString DBPutOperations = MakeDBPutOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	DBProcess.Replace(_T("<DBPutOperations>"), DBPutOperations);

	CString Space = _T("\r\n");
	Space += szLineSpace;
	DBProcess.Replace(_T("\r\n"), Space);
	return DBProcess;
}
CString CCallInterfaceMakerDlg::MakeDBFieldDefineOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;

	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		CString Operation = m_DBFieldDefineUnitTemplate;

		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
			if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}
			else
			{
				RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
			RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
		}

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(MemberInfo.Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);

		CString LineSpace = GetLineSpace(Operation, _T("<DBFieldDefine>"));
		CString DBFieldDefine = MakeDBFieldDefineOperation(MemberInfo.Type,
			MemberInfo.Name,
			StructInfo.Name, StructInfo.ShortName,
			SSTIDEnumName,
			(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0,
			MemberInfo.IsArray,
			MemberInfo.DBLength,
			SpaceName, LineSpace);

		Operation.Replace(_T("<DBFieldDefine>"), DBFieldDefine);


		Operations += Operation;
		Operations += _T("\r\n");
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeDBIndexDefineOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;
	CString PrimaryKeyFields = MakeDBFields(StructInfo, DB_INDEX_TYPE_PRIMARY_KEY);
	bool IsPrimaryDefinded = false;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
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

		if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
		{
			RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
			Operation.Replace(_T("<IfInDataObject>"), _T(""));
			Operation.Replace(_T("</IfInDataObject>"), _T(""));
			if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
			{
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}
			else
			{
				RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}
		}
		else
		{
			RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
			Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
			Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
			RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
		}

		CString ModifyFlagEnumName;
		ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
		Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

		CString ModifyFlag = _T("MF_") + ClassNameToUpper(MemberInfo.Name);
		Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

		Operation.Replace(_T("<SpaceName>"), SpaceName);
		Operation.Replace(_T("<FieldNames>"), PrimaryKeyFields);
		Operation.Replace(_T("<FieldName>"), MemberInfo.Name);
		Operation.Replace(_T("<!FieldName>"), ClassNameToUpper(MemberInfo.Name));

		Operations += Operation;
		Operations += _T("\r\n");
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}

CString CCallInterfaceMakerDlg::MakeDBPutParamBindOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE* pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			if (((pTypeInfo->Flag & (TYPE_DEFINE_FLAG_DB_USE_PARAM_BIND | TYPE_DEFINE_FLAG_STRUCT)) == 0) && (!MemberInfo.IsArray))
				continue;

			CString Operation;

			if (MemberInfo.IsArray)
			{
				CString Var = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
				Var.Replace(_T("<Variable>"), MemberInfo.Name);
				Var.Replace(_T("<Index>"), _T("i"));
				CString SizeCaculateOperation = pTypeInfo->GenerateOperations.SizeCaculateOperation;
				SizeCaculateOperation.Replace(_T("<Variable>"), Var);
				SizeCaculateOperation.Replace(_T("<Type>"), pTypeInfo->CType);
				RemoveBlock(SizeCaculateOperation, _T("<IfInArray>"), _T("</IfInArray>"));
				SizeCaculateOperation.Replace(_T("<IfNotInArray>"), _T(""));
				SizeCaculateOperation.Replace(_T("</IfNotInArray>"), _T(""));
				CString PackOperation = pTypeInfo->GenerateOperations.PackOperation;
				PackOperation.Replace(_T("<Variable>"), Var);
				PackOperation.Replace(_T("<Type>"), pTypeInfo->CType);
				RemoveBlock(PackOperation, _T("<IfInArray>"), _T("</IfInArray>"));
				PackOperation.Replace(_T("<IfNotInArray>"), _T(""));
				PackOperation.Replace(_T("</IfNotInArray>"), _T(""));

				Operation = m_InterfaceConfig.ArrayDefineConfig.DBPutOperation;
				Operation.Replace(_T("<ArrayElementSize>"), SizeCaculateOperation);
				CString LineSpace = GetLineSpace(Operation, _T("<PackOperation>"));
				CString Space = _T("\r\n");
				Space += LineSpace;
				PackOperation.Replace(_T("\r\n"), Space);
				Operation.Replace(_T("<PackOperation>"), PackOperation);

				Operation = ProcessArrayOperation(Operation, pTypeInfo);

			}
			else
			{
				Operation = pTypeInfo->GenerateOperations.DBPutOperation;
			}

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
				Operation.Replace(_T("<IfInDataObject>"), _T(""));
				Operation.Replace(_T("</IfInDataObject>"), _T(""));
				if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				else
				{
					RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
			}
			else
			{
				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}

			if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
				Operation.Replace(_T("<IfDataObject>"), _T(""));
				Operation.Replace(_T("</IfDataObject>"), _T(""));
			}
			else
			{
				RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
				Operation.Replace(_T("<IfNotDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotDataObject>"), _T(""));
			}

			CString SST_ID;

			SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
			SST_ID.MakeUpper();


			Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
			Operation.Replace(_T("<SST_ID>"), SST_ID);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
			Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

			CString ModifyFlag = _T("MF_") + ClassNameToUpper(MemberInfo.Name);
			Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

			Operation.Replace(_T("<SpaceName>"), SpaceName);
			Operation.Replace(_T("<FieldName>"), MemberInfo.Name);
			Operation.Replace(_T("<!FieldName>"), ClassNameToUpper(MemberInfo.Name));

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				Operation.Replace(_T("<Variable>"), CString(_T("m_")) + MemberInfo.Name);
			else
				Operation.Replace(_T("<Variable>"), MemberInfo.Name);


			Operation.Replace(_T("<VariableName>"), MemberInfo.Name);

			Operation.Replace(_T("<Type>"), pTypeInfo->CType);

			CString FullModifyFlag;
			FullModifyFlag.Format(_T("DOMF_%s_FULL"), ClassNameToUpper(MemberInfo.Type));
			Operation.Replace(_T("<FULL_MODIFY_FLAG>"), FullModifyFlag);

			Operations += Operation;
			Operations += _T("\r\n");
			Operations += _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeDBPutOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operations;
	UINT Count = 0;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE* pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			if ((pTypeInfo->Flag & (TYPE_DEFINE_FLAG_DB_USE_PARAM_BIND | TYPE_DEFINE_FLAG_STRUCT)) || MemberInfo.IsArray)
				continue;

			CString Operation = pTypeInfo->GenerateOperations.DBPutOperation;

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
				Operation.Replace(_T("<IfInDataObject>"), _T(""));
				Operation.Replace(_T("</IfInDataObject>"), _T(""));
				if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				else
				{
					RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
			}
			else
			{
				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
			Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

			CString ModifyFlag = _T("MF_") + ClassNameToUpper(MemberInfo.Name);
			Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

			Operation.Replace(_T("<SpaceName>"), SpaceName);
			Operation.Replace(_T("<FieldName>"), MemberInfo.Name);
			Operation.Replace(_T("<!FieldName>"), ClassNameToUpper(MemberInfo.Name));

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				Operation.Replace(_T("<Variable>"), CString(_T("m_")) + MemberInfo.Name);
			else
				Operation.Replace(_T("<Variable>"), MemberInfo.Name);


			Operation.Replace(_T("<VariableName>"), MemberInfo.Name);

			Operation.Replace(_T("<Type>"), pTypeInfo->CType);

			CString FullModifyFlag;
			FullModifyFlag.Format(_T("DOMF_%s_FULL"), ClassNameToUpper(MemberInfo.Type));
			Operation.Replace(_T("<FULL_MODIFY_FLAG>"), FullModifyFlag);

			Operations += _T(",") + Operation;
			Operations += _T("\r\n");
			Count++;
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}

CString CCallInterfaceMakerDlg::MakeDBFields(STRUCT_DEFINE_INFO& StructInfo, BYTE DBIndexType)
{
	CString Fields;

	UINT Count = 0;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		if ((DBIndexType == DB_INDEX_TYPE_NONE) || (MemberInfo.DBIndexType == DBIndexType))
		{
			CString Operation = m_DBFieldNameTemplate;
			if (Count)
			{
				Operation.Replace(_T("<IfNotFirst>"), _T(""));
				Operation.Replace(_T("</IfNotFirst>"), _T(""));
			}
			else
			{
				RemoveBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
			}
			Operation.Replace(_T("<FieldName>"), MemberInfo.Name);
			Fields += Operation;
			Count++;
		}
	}
	return Fields;
}
CString CCallInterfaceMakerDlg::MakeDBFieldDefineOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, bool IsInDataObject, bool IsArray, LPCTSTR DBLength, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;

	TYPE_DEFINE* pTypeInfo = FindVarType(Type);
	if (pTypeInfo)
	{
		Operation = pTypeInfo->GenerateOperations.DBFieldDefineOperation;

		SelectBlock(Operation, _T("IsArray"), IsArray);

		Operation.Replace(_T("<FieldName>"), Name);
		Operation.Replace(_T("<DBLength>"), DBLength);

		CString FullModifyFlag;
		FullModifyFlag.Format(_T("DOMF_%s_FULL"), ClassNameToUpper(Name));
		Operation.Replace(_T("<FULL_MODIFY_FLAG>"), FullModifyFlag);

		Operation.Replace(_T("<VariableName>"), Name);
		Operation.Replace(_T("<Type>"), pTypeInfo->CType);
		Operation.Replace(_T("<SpaceName>"), SpaceName);
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operation.Replace(_T("\r\n"), Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeDBFieldFormats(STRUCT_DEFINE_INFO& StructInfo)
{
	CString Formats;

	UINT Count = 0;
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET)
			continue;

		TYPE_DEFINE* pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			if (Count)
				Formats += _T(", ") + pTypeInfo->GenerateOperations.DBInsertFormatOperation;
			else
				Formats += pTypeInfo->GenerateOperations.DBInsertFormatOperation;
			Count++;
		}
	}
	return Formats;
}

CString CCallInterfaceMakerDlg::MakeFileLogProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Process = m_StructFileLogProcessTemplate;

	if (StructInfo.BaseStruct.IsEmpty())
	{
		RemoveBlock(Process, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
		RetainBlock(Process, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
		Process.Replace(_T("<BaseClass>"), _T(""));

	}
	else
	{
		RetainBlock(Process, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
		RemoveBlock(Process, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
		Process.Replace(_T("<BaseClass>"), StructInfo.BaseStruct);
	}

	Process.Replace(_T("<ClassName>"), StructInfo.Name);
	int BlockLen = 0;
	CString Separator, Content;
	int StartPos = FindJoinContent(Process, _T("JoinWithStruct"), 0, BlockLen, Separator, Content);
	while (StartPos >= 0)
	{
		Content = MakeStructJoin(StructInfo, Content, Separator);
		Process.Delete(StartPos, BlockLen);
		Process.Insert(StartPos, Content);
		StartPos = FindJoinContent(Process, _T("JoinWithStruct"), StartPos + Content.GetLength(), BlockLen, Separator, Content);
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Process.Replace(_T("\r\n"), Space);
	return Process;
}

CString CCallInterfaceMakerDlg::MakeAliLogProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString DBProcess = m_StructAliLogProcessTemplate;

	DBProcess.Replace(_T("<ClassName>"), StructInfo.Name);

	UINT MemberCount = GetStructMemberCount(StructInfo);
	CString Temp;
	Temp.Format(_T("%u"), MemberCount);
	DBProcess.Replace(_T("<StructMemberCount>"), Temp);

	CString LineSpace = GetLineSpace(DBProcess, _T("<LogDataList>"));
	CString Operations = MakeAliLogDataList(StructInfo, LineSpace);
	DBProcess.Replace(_T("<LogDataList>"), Operations);

	CString Space = _T("\r\n");
	Space += szLineSpace;
	DBProcess.Replace(_T("\r\n"), Space);
	return DBProcess;
}

CString CCallInterfaceMakerDlg::MakeAliLogDataList(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Operations;

	if (!StructInfo.BaseStruct.IsEmpty())
	{
		STRUCT_DEFINE_INFO* pStructInfo = GetStructDefineInfo(StructInfo.BaseStruct);
		if (pStructInfo)
		{
			Operations += MakeAliLogDataList(*pStructInfo, szLineSpace);
		}
	}

	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if (!Operations.IsEmpty())
		{
			Operations += _T(",\r\n");
			Operations += szLineSpace;
		}

		TYPE_DEFINE* pTypeInfo = FindVarType(MemberInfo.Type);
		if (pTypeInfo)
		{
			CString Operation = pTypeInfo->GenerateOperations.AliLogSendOperation;
			Operation.Replace(_T("<VariableName>"), MemberInfo.Name);
			CString VarName;
			VarName = MemberInfo.Name;
			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				VarName = _T("m_") + VarName;
			}
			Operation.Replace(_T("<Variable>"), VarName);
			Operations += Operation;
		}
		else
		{
			Operations += _T("<ErrorType>");
		}
	}

	return Operations;
}

CString CCallInterfaceMakerDlg::MakeLuaProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Process = m_StructLuaProcessTemplate;

	Process.Replace(_T("<ClassName>"), StructInfo.Name);

	if (StructInfo.BaseStruct.IsEmpty())
	{
		RemoveBlock(Process, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
		RetainBlock(Process, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
		Process.Replace(_T("<BaseClass>"), _T(""));

	}
	else
	{
		RetainBlock(Process, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
		RemoveBlock(Process, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
		Process.Replace(_T("<BaseClass>"), StructInfo.BaseStruct);
	}


	CString LineSpace = GetLineSpace(Process, _T("<ToLuaTableOperations>"));
	CString Operations = MakeToLuaTableOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	Process.Replace(_T("<ToLuaTableOperations>"), Operations);

	LineSpace = GetLineSpace(Process, _T("<FromLuaTableOperations>"));
	Operations = MakeFromLuaTableOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	Process.Replace(_T("<FromLuaTableOperations>"), Operations);

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Process.Replace(_T("\r\n"), Space);
	return Process;
}

CString CCallInterfaceMakerDlg::MakeDataObjectLuaProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Process = m_DataObjectLuaProcessTemplate;

	Process.Replace(_T("<ClassName>"), StructInfo.Name);

	if (StructInfo.BaseStruct.IsEmpty())
	{
		RemoveBlock(Process, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
		RetainBlock(Process, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
		Process.Replace(_T("<BaseClass>"), _T(""));

	}
	else
	{
		RetainBlock(Process, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
		RemoveBlock(Process, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
		Process.Replace(_T("<BaseClass>"), StructInfo.BaseStruct);
	}

	CString LineSpace = GetLineSpace(Process, _T("<ToLuaTableOperations>"));
	CString Operations = MakeToLuaTableOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	Process.Replace(_T("<ToLuaTableOperations>"), Operations);

	LineSpace = GetLineSpace(Process, _T("<FromLuaTableOperations>"));
	Operations = MakeFromLuaTableOperations(StructInfo, SSTIDEnumName, SpaceName, LineSpace);
	Process.Replace(_T("<FromLuaTableOperations>"), Operations);

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Process.Replace(_T("\r\n"), Space);
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
					RemoveBlock(Operation, _T("<IfNotHaveKey>"), _T("</IfNotHaveKey>"));
					RetainBlock(Operation, _T("<IfHaveKey>"), _T("</IfHaveKey>"));
					Operation.Replace(_T("<StructKey>"), pKeyMember->Name);
				}
				else
				{
					RemoveBlock(Operation, _T("<IfHaveKey>"), _T("</IfHaveKey>"));
					RetainBlock(Operation, _T("<IfNotHaveKey>"), _T("</IfNotHaveKey>"));
				}
			}

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
				Operation.Replace(_T("<IfInDataObject>"), _T(""));
				Operation.Replace(_T("</IfInDataObject>"), _T(""));
				if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				else
				{
					RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
			}
			else
			{
				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
			}

			CString VarName = MemberInfo.Name;
			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				VarName = _T("m_") + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
			Operation.Replace(_T("<Variable>"), VarName);

			Operation.Replace(_T("<VariableName>"), MemberInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
			Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

			CString ModifyFlag = _T("MF_") + ClassNameToUpper(MemberInfo.Name);
			Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

			Operation.Replace(_T("<SpaceName>"), SpaceName);

			CString LineSpace = GetLineSpace(Operation, _T("<ToLuaOperation>"));
			CString PackOP = MakeToLuaTableOperation(pTypeInfo,
				MemberInfo.Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName,
				(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0,
				MemberInfo.IsArray,
				SpaceName, LineSpace);

			Operation.Replace(_T("<ToLuaOperation>"), PackOP);

			CString SST_ID;

			SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
			SST_ID.MakeUpper();


			Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
			Operation.Replace(_T("<SST_ID>"), SST_ID);

			//if (MemberInfo.IsArray)
			//{
			//	CString ArrayOperation = Operation;
			//	Operation = m_ToLuaTableOperationUnitTemplate;
			//	Operation.Replace(_T("<Variable>"), VarName);
			//	Operation.Replace(_T("<VariableName>"), MemberInfo.Name);
			//	Operation.Replace(_T("<ToLuaOperation>"), ArrayOperation);
			//}

			Operations += Operation;
			Operations += _T("\r\n");
		}

	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}

CString CCallInterfaceMakerDlg::MakeToLuaTableOperation(TYPE_DEFINE* pTypeInfo, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, bool IsInDataObject, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;


	Operation = pTypeInfo->GenerateOperations.ToLuaOperation;

	if (IsInDataObject)
	{
		RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
		Operation.Replace(_T("<IfInDataObject>"), _T(""));
		Operation.Replace(_T("</IfInDataObject>"), _T(""));
	}
	else
	{
		RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
		Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
		Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
	}

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
	{
		RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
		Operation.Replace(_T("<IfDataObject>"), _T(""));
		Operation.Replace(_T("</IfDataObject>"), _T(""));
	}
	else
	{
		RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
		Operation.Replace(_T("<IfNotDataObject>"), _T(""));
		Operation.Replace(_T("</IfNotDataObject>"), _T(""));
	}

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
	{
		RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
		Operation.Replace(_T("<If64Bit>"), _T(""));
		Operation.Replace(_T("</If64Bit>"), _T(""));
	}
	else
	{
		RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
		Operation.Replace(_T("<IfNot64Bit>"), _T(""));
		Operation.Replace(_T("</IfNot64Bit>"), _T(""));
	}



	CString SST_ID;


	SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(Name));
	SST_ID.MakeUpper();



	//CString VarName=Name;
	SelectBlock(Operation, _T("IsArray"), IsArray);
	if (IsArray)
	{
		CString ArrayVar = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
		ArrayVar.Replace(_T("<Index>"), _T("i"));
		Operation.Replace(_T("<Variable>"), ArrayVar);
		Operation = ProcessArrayOperation(Operation, pTypeInfo);
	}

	CString VarName = Name;
	if (IsInDataObject)
		VarName = _T("m_") + VarName;
	VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;

	Operation.Replace(_T("<Variable>"), VarName);

	Operation.Replace(_T("<VariableName>"), Name);

	Operation.Replace(_T("<Type>"), pTypeInfo->CType);

	Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
	Operation.Replace(_T("<SST_ID>"), SST_ID);

	CString ModifyFlagEnumName;
	ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ParentName));
	Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

	CString ModifyFlag = _T("MF_") + ClassNameToUpper(Name);
	Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

	Operation.Replace(_T("<SpaceName>"), SpaceName);


	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operation.Replace(_T("\r\n"), Space);
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
				VariableDefine.Replace(_T("<Type>"), pTypeInfo->CType);
				VariableDefine.Replace(_T("<Space>"), _T("	"));
				VariableDefine.Replace(_T("<Variable>"), _T("ArrayElement"));
				VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
				Operation.Replace(_T("<ArrayElementVariableDefine>"), VariableDefine);

				CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
				VariableInit.Replace(_T("<Variable>"), _T("ArrayElement"));
				Operation.Replace(_T("<ArrayElementVariableInit>"), VariableInit);

				STRUCT_MEMBER_INFO* pKeyMember = NULL;
				if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_STRUCT)
				{
					pKeyMember = GetStructKeyMember(pTypeInfo->Name);
				}
				if (pKeyMember)
				{
					RemoveBlock(Operation, _T("<IfNotHaveKey>"), _T("</IfNotHaveKey>"));
					RetainBlock(Operation, _T("<IfHaveKey>"), _T("</IfHaveKey>"));
					Operation.Replace(_T("<StructKey>"), pKeyMember->Name);
				}
				else
				{
					RemoveBlock(Operation, _T("<IfHaveKey>"), _T("</IfHaveKey>"));
					RetainBlock(Operation, _T("<IfNotHaveKey>"), _T("</IfNotHaveKey>"));
				}
			}

			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
				Operation.Replace(_T("<IfInDataObject>"), _T(""));
				Operation.Replace(_T("</IfInDataObject>"), _T(""));
				if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_MUST_PACK)
				{
					RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				else
				{
					RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				}
				RetainBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
			}
			else
			{
				RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
				Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
				Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
				RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
				RemoveBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
			}

			CString VarName = MemberInfo.Name;
			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
				VarName = _T("m_") + VarName;
			VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
			Operation.Replace(_T("<Variable>"), VarName);

			Operation.Replace(_T("<VariableName>"), MemberInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
			Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

			CString ModifyFlag = _T("MF_") + ClassNameToUpper(MemberInfo.Name);
			Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

			Operation.Replace(_T("<SpaceName>"), SpaceName);

			CString LineSpace = GetLineSpace(Operation, _T("<FromLuaOperation>"));
			CString PackOP = MakeFromLuaTableOperation(pTypeInfo,
				MemberInfo.Name,
				StructInfo.Name, StructInfo.ShortName,
				SSTIDEnumName,
				(StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT) != 0,
				MemberInfo.IsArray,
				SpaceName, LineSpace);

			Operation.Replace(_T("<FromLuaOperation>"), PackOP);

			CString SST_ID;

			SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
			SST_ID.MakeUpper();


			Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
			Operation.Replace(_T("<SST_ID>"), SST_ID);

			//if (MemberInfo.IsArray)
			//{
			//	CString ArrayOperation = Operation;
			//	Operation = m_FromLuaTableOperationUnitTemplate;
			//	Operation.Replace(_T("<Variable>"), VarName);
			//	Operation.Replace(_T("<VariableName>"), MemberInfo.Name);
			//	Operation.Replace(_T("<FromLuaOperation>"), ArrayOperation);
			//}

			Operations += Operation;
			Operations += _T("\r\n");
		}

	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operations.Replace(_T("\r\n"), Space);
	return Operations;
}
CString CCallInterfaceMakerDlg::MakeFromLuaTableOperation(TYPE_DEFINE* pTypeInfo, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, bool IsInDataObject, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Operation;


	Operation = pTypeInfo->GenerateOperations.FromLuaOperation;

	if (IsInDataObject)
	{
		RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
		Operation.Replace(_T("<IfInDataObject>"), _T(""));
		Operation.Replace(_T("</IfInDataObject>"), _T(""));
	}
	else
	{
		RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
		Operation.Replace(_T("<IfNotInDataObject>"), _T(""));
		Operation.Replace(_T("</IfNotInDataObject>"), _T(""));
	}

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
	{
		RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
		Operation.Replace(_T("<IfDataObject>"), _T(""));
		Operation.Replace(_T("</IfDataObject>"), _T(""));
	}
	else
	{
		RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
		Operation.Replace(_T("<IfNotDataObject>"), _T(""));
		Operation.Replace(_T("</IfNotDataObject>"), _T(""));
	}

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
	{
		RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
		Operation.Replace(_T("<If64Bit>"), _T(""));
		Operation.Replace(_T("</If64Bit>"), _T(""));
	}
	else
	{
		RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
		Operation.Replace(_T("<IfNot64Bit>"), _T(""));
		Operation.Replace(_T("</IfNot64Bit>"), _T(""));
	}



	CString SST_ID;


	SST_ID.Format(_T("SST_%s_%s"), ParentShortName, ClassNameToUpper(Name));
	SST_ID.MakeUpper();



	//CString VarName=Name;
	SelectBlock(Operation, _T("IsArray"), IsArray);
	if (IsArray)
	{
		Operation.Replace(_T("<Variable>"), _T("ArrayElement"));
	}

	CString VarName = Name;
	if (IsInDataObject)
		VarName = _T("m_") + VarName;
	VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;

	Operation.Replace(_T("<Variable>"), VarName);

	Operation.Replace(_T("<VariableName>"), Name);

	Operation.Replace(_T("<Type>"), pTypeInfo->CType);

	Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
	Operation.Replace(_T("<SST_ID>"), SST_ID);

	CString ModifyFlagEnumName;
	ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ParentName));
	Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

	CString ModifyFlag = _T("MF_") + ClassNameToUpper(Name);
	Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

	Operation.Replace(_T("<SpaceName>"), SpaceName);


	CString Space = _T("\r\n");
	Space += szLineSpace;
	Operation.Replace(_T("\r\n"), Space);
	return Operation;
}

CString CCallInterfaceMakerDlg::MakeMsgHandlerMethods(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString Methods;
	for (size_t j = 0;j < InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];

		CString MethodName = MethodInfo.Name;
		bool IsExport = true;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType != INTERFACE_METHOD_EXPORT_TYPE_CALL)
			{
				MethodName = MethodInfo.Name + _T("Ack");
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
			Method.Replace(_T("<MethodName>"), MethodName);
			Methods += Method + _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgHandlerMethodsSource(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + _T("Ack");
	else
		InterfaceName = InterfaceInfo.Name;

	CString Methods;
	for (size_t j = 0;j < InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];
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
				MethodName = MethodInfo.Name + _T("Ack");
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
				Method.Replace(_T("<IfHaveMember>"), _T(""));
				Method.Replace(_T("</IfHaveMember>"), _T(""));
			}
			else
			{
				RemoveBlock(Method, _T("<IfHaveMember>"), _T("</IfHaveMember>"));
			}

			Method.Replace(_T("<SpaceName>"), SpaceName);
			Method.Replace(_T("<InterfaceName>"), InterfaceName);
			Method.Replace(_T("<@InterfaceName>"), InterfaceInfo.Name);
			Method.Replace(_T("<MethodName>"), MethodName);
			CString Params = MakeParams(*pParamList, false, true);
			Method.Replace(_T("<Params>"), Params);
			CString LineSpace = GetLineSpace(Method, _T("<ParamDefines>"));
			CString ParamDefines = MakeParamDefines(*pParamList, SpaceName, _T(";"), LineSpace);
			Method.Replace(_T("<ParamDefines>"), ParamDefines);

			LineSpace = GetLineSpace(Method, _T("<ParamInitOperations>"));
			CString ParamInitOperations = MakeInitOperations(MethodInfo, ExportType, LineSpace);
			Method.Replace(_T("<ParamInitOperations>"), ParamInitOperations);

			CString PacketName = m_InterfaceConfig.DefaultPacketName;
			CString PacketMemberName = m_InterfaceConfig.DefaultPacketMemberName;

			LineSpace = GetLineSpace(Method, _T("<UnpackOperations>"));
			CString SSTIDEnumName;
			SSTIDEnumName.Format(_T("%s_%s_MEMBER_IDS"), ClassNameToUpper(InterfaceName), ClassNameToUpper(MethodName));
			CString UnpackOperations = MakeUnpackOperations(MethodInfo, ExportType, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
			Method.Replace(_T("<UnpackOperations>"), UnpackOperations);

			Methods += Method + _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeMsgMapInits(CALLER_INTERFACE& InterfaceInfo, LPCTSTR SpaceName, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace)
{
	CString InterfaceName;
	if (ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK)
		InterfaceName = InterfaceInfo.Name + _T("Ack");
	else
		InterfaceName = InterfaceInfo.Name;
	CString SpaceNameUp = ClassNameToUpper(SpaceName);
	CString InterfaceNameUp = ClassNameToUpper(InterfaceName);
	CString InterfaceIDEnumName;
	CString InterfaceID;
	CString ModuleID;
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(InterfaceInfo.ModuleID);
	if (pModuleInfo)
	{
		InterfaceIDEnumName.Format(_T("%s_INTERFACES"), ClassNameToUpper(pModuleInfo->Name));
		InterfaceID.Format(_T("%s_INTERFACE_%s"), ClassNameToUpper(pModuleInfo->Name), ClassNameToUpper(InterfaceInfo.Name));
		ModuleID = _T("MODULE_ID_") + ClassNameToUpper(pModuleInfo->Name);
	}




	CString MethodIDEnumName;
	MethodIDEnumName.Format(_T("%s_METHODS"), ClassNameToUpper(InterfaceInfo.Name));

	CString MapInits;
	for (size_t j = 0;j < InterfaceInfo.MethodList.size();j++)
	{
		INTERFACE_METHOD& MethodInfo = InterfaceInfo.MethodList[j];
		CString MapInit = m_MsgMapInitTemple;

		CString MethodName = MethodInfo.Name;
		bool IsExport = true;
		switch (MethodInfo.Type)
		{
		case INTERFACE_METHOD_TYPE_CALL:
			if (ExportType != INTERFACE_METHOD_EXPORT_TYPE_CALL)
			{
				MethodName = MethodInfo.Name + _T("Ack");
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

			MapInit.Replace(_T("<ModuleID>"), ModuleID);
			MapInit.Replace(_T("<InterfaceName>"), InterfaceName);
			MapInit.Replace(_T("<MethodName>"), MethodName);
			MapInit.Replace(_T("<SpaceName>"), SpaceName);
			MapInit.Replace(_T("<!SpaceName>"), SpaceNameUp);
			MapInit.Replace(_T("<!InterfaceName>"), InterfaceNameUp);
			MapInit.Replace(_T("<!MethodName>"), MethodNameUp);
			MapInit.Replace(_T("<@InterfaceName>"), InterfaceInfo.Name);

			MapInit.Replace(_T("<InterfaceIDEnumName>"), InterfaceIDEnumName);
			MapInit.Replace(_T("<InterfaceID>"), InterfaceID);
			CString MethodID;
			MethodID.Format(_T("METHOD_%s"), ClassNameToUpper(MethodInfo.Name));
			MapInit.Replace(_T("<MethodID>"), MethodID);
			MapInit.Replace(_T("<MethodIDEnumName>"), MethodIDEnumName);
			SelectBlock(MapInit, _T("IsAckMsg"), ExportType == INTERFACE_METHOD_EXPORT_TYPE_ACK);

			MapInits += MapInit + _T("\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	MapInits.Replace(_T("\r\n"), Space);
	return MapInits;
}

CString CCallInterfaceMakerDlg::MakeStructDefines(vector<STRUCT_DEFINE_INFO*>& StructDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString StructDefines;
	UINT StructCount = 0;
	for (size_t i = 0; i < StructDefineList.size(); i++)
	{
		STRUCT_DEFINE_INFO& StructDefine = *(StructDefineList[i]);
		if ((StructDefine.Flag & STRUCT_FLAG_IS_DATA_OBJECT) == 0)
		{
			CString Struct = m_StructDefineTemplate;

			Struct.Replace(_T("<SpaceName>"), SpaceName);


			CString LineSpace;
			bool NeedVTable = false;

			CString SSTIDEnumName;
			SSTIDEnumName.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(StructDefine.Name));

			if (StructDefine.Flag & STRUCT_FLAG_EXPORT_XML_PROCESS)
			{
				LineSpace = GetLineSpace(Struct, _T("<XMLProcess>"));
				CString XMLProcess = MakeXMLProcess(StructDefine, SSTIDEnumName, SpaceName, LineSpace);
				Struct.Replace(_T("<XMLProcess>"), XMLProcess);
			}
			else
			{
				Struct.Replace(_T("<XMLProcess>"), _T(""));
			}

			if (StructDefine.Flag & STRUCT_FLAG_EXPORT_JSON_PROCESS)
			{
				LineSpace = GetLineSpace(Struct, _T("<JsonProcess>"));
				CString JsonProcess = MakeJsonProcess(StructDefine, SSTIDEnumName, SpaceName, LineSpace);
				Struct.Replace(_T("<JsonProcess>"), JsonProcess);
			}
			else
			{
				Struct.Replace(_T("<JsonProcess>"), _T(""));
			}

			if (StructDefine.Flag & STRUCT_FLAG_EXPORT_DB_PROCESS)
			{
				LineSpace = GetLineSpace(Struct, _T("<DBProcess>"));
				CString Process = MakeDBProcess(StructDefine, SSTIDEnumName, SpaceName, LineSpace);
				Struct.Replace(_T("<DBProcess>"), Process);
				NeedVTable = true;
			}
			else
			{
				Struct.Replace(_T("<DBProcess>"), _T(""));
			}

			//if (Struct.Find(_T("<EditorProcess>")) >= 0)
			//{
			//	if (StructDefine.Flag & STRUCT_FLAG_EXPORT_EDITOR_PROCESS)
			//	{
			//		LineSpace = GetLineSpace(Struct, _T("<EditorProcess>"));
			//		CString Process = MakeStructEditorProcess(StructDefine, LineSpace);
			//		Struct.Replace(_T("<EditorProcess>"), Process);
			//	}
			//	else
			//	{
			//		Struct.Replace(_T("<EditorProcess>"), _T(""));
			//	}
			//}

			if (Struct.Find(_T("<FileLogProcess>")) >= 0)
			{
				if (StructDefine.Flag & STRUCT_FLAG_EXPORT_FILE_LOG_PROCESS)
				{
					LineSpace = GetLineSpace(Struct, _T("<FileLogProcess>"));
					CString Process = MakeFileLogProcess(StructDefine, SpaceName, LineSpace);
					Struct.Replace(_T("<FileLogProcess>"), Process);
					NeedVTable = true;
				}
				else
				{
					Struct.Replace(_T("<FileLogProcess>"), _T(""));
				}
			}

			if (Struct.Find(_T("<AliLogProcess>")) >= 0)
			{
				if (StructDefine.Flag & STRUCT_FLAG_EXPORT_ALI_LOG_PROCESS)
				{
					LineSpace = GetLineSpace(Struct, _T("<AliLogProcess>"));
					CString Process = MakeAliLogProcess(StructDefine, SpaceName, LineSpace);
					Struct.Replace(_T("<AliLogProcess>"), Process);
					NeedVTable = true;
				}
				else
				{
					Struct.Replace(_T("<AliLogProcess>"), _T(""));
				}
			}

			if (Struct.Find(_T("<LuaProcess>")) >= 0)
			{
				if (StructDefine.Flag & STRUCT_FLAG_EXPORT_LUA_PROCESS)
				{
					LineSpace = GetLineSpace(Struct, _T("<LuaProcess>"));
					CString Process = MakeLuaProcess(StructDefine, SSTIDEnumName, SpaceName, LineSpace);
					Struct.Replace(_T("<LuaProcess>"), Process);
					NeedVTable = true;
				}
				else
				{
					Struct.Replace(_T("<LuaProcess>"), _T(""));
				}
			}

			if (Struct.Find(_T("<XLSProcess>")) >= 0)
			{
				if (StructDefine.Flag & STRUCT_FLAG_EXPORT_XLS_PROCESS)
				{
					LineSpace = GetLineSpace(Struct, _T("<XLSProcess>"));
					CString Process = MakeStructXLSProcess(StructDefine, LineSpace);
					Struct.Replace(_T("<XLSProcess>"), Process);
				}
				else
				{
					Struct.Replace(_T("<XLSProcess>"), _T(""));
				}
			}

			if (NeedVTable)
				RetainBlock(Struct, _T("<NeedVTable>"), _T("</NeedVTable>"));
			else
				RemoveBlock(Struct, _T("<NeedVTable>"), _T("</NeedVTable>"));

			if (StructDefine.MemberList.size())
			{
				Struct.Replace(_T("<IfHaveMember>"), _T(""));
				Struct.Replace(_T("</IfHaveMember>"), _T(""));
			}
			else
			{
				RemoveBlock(Struct, _T("<IfHaveMember>"), _T("</IfHaveMember>"));
			}

			if (StructDefine.BaseStruct.IsEmpty())
			{
				RemoveBlock(Struct, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
				Struct.Replace(_T("<IfNotHaveBaseClass>"), _T(""));
				Struct.Replace(_T("</IfNotHaveBaseClass>"), _T(""));
				Struct.Replace(_T("<BaseClass>"), _T(""));

			}
			else
			{
				Struct.Replace(_T("<IfHaveBaseClass>"), _T(""));
				Struct.Replace(_T("</IfHaveBaseClass>"), _T(""));
				RemoveBlock(Struct, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
				Struct.Replace(_T("<BaseClass>"), StructDefine.BaseStruct);
			}

			LineSpace = GetLineSpace(Struct, _T("<Description>"));
			CString StructDescription = ToComment(StructDefine.Description, LineSpace);
			Struct.Replace(_T("<Description>"), StructDescription);
			Struct.Replace(_T("<OrginDescription>"), StructDefine.Description);
			Struct.Replace(_T("<StructName>"), StructDefine.Name);
			Struct.Replace(_T("<ShowName>"), StructDefine.ShowName);
			CString BaseStruct;
			if (!StructDefine.BaseStruct.IsEmpty())
			{
				BaseStruct = StructDefine.BaseStruct;
			}
			Struct.Replace(_T("<BaseStruct>"), BaseStruct);

			if (Struct.Find(_T("<Members>")) >= 0)
			{
				LineSpace = GetLineSpace(Struct, _T("<Members>"));
				CString Members = MakeStructMembers(StructDefine, _T(";"), true, SpaceName, LineSpace);
				Struct.Replace(_T("<Members>"), Members);
			}

			if (Struct.Find(_T("<Members,>")) >= 0)
			{
				LineSpace = GetLineSpace(Struct, _T("<Members,>"));
				CString Members = MakeStructMembers(StructDefine, _T(","), false, SpaceName, LineSpace);
				Struct.Replace(_T("<Members,>"), Members);
			}

			LineSpace = GetLineSpace(Struct, _T("<SSTIDEnumDefine>"));
			CString SSTIDEnum = MakeStructSSTIDEnumDefines(StructDefine, SpaceName, LineSpace);
			Struct.Replace(_T("<SSTIDEnumDefine>"), SSTIDEnum);

			LineSpace = GetLineSpace(Struct, _T("<InitOperations>"));
			CString InitOperations = MakeInitOperations(StructDefine, LineSpace);
			Struct.Replace(_T("<InitOperations>"), InitOperations);

			CString PacketName = m_InterfaceConfig.DefaultPacketName;
			CString PacketMemberName = m_InterfaceConfig.DefaultPacketMemberName;

			LineSpace = GetLineSpace(Struct, _T("<PackOperations>"));
			CString PackOperations = MakePackOperations(StructDefine, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
			Struct.Replace(_T("<PackOperations>"), PackOperations);

			LineSpace = GetLineSpace(Struct, _T("<UnpackOperations>"));
			CString UnpackOperations = MakeUnpackOperations(StructDefine, SSTIDEnumName, SpaceName, PacketName, PacketMemberName, LineSpace);
			Struct.Replace(_T("<UnpackOperations>"), UnpackOperations);

			LineSpace = GetLineSpace(Struct, _T("<PacketSizes>"));
			CString PackSizes = MakePackSizes(StructDefine, SpaceName, LineSpace);
			Struct.Replace(_T("<PacketSizes>"), PackSizes);

			//LineSpace = GetLineSpace(Struct, _T("<ToXMLOperations>"));
			//CString ToXMLOperations = MakeToXMLOperations(StructDefine, SSTIDEnumName, LineSpace);
			//Struct.Replace(_T("<ToXMLOperations>"), ToXMLOperations);

			//LineSpace = GetLineSpace(Struct, _T("<FromXMLOperations>"));
			//CString FromXMLOperations = MakeFromXMLOperations(StructDefine, SSTIDEnumName, LineSpace);
			//Struct.Replace(_T("<FromXMLOperations>"), FromXMLOperations);



			LineSpace = GetLineSpace(Struct, _T("<CloneFromOperations>"));
			CString CloneOperations = MakeCloneOperations(StructDefine, m_InterfaceConfig.MemberVariablePrefix, _T("Value."), SpaceName, LineSpace);
			Struct.Replace(_T("<CloneFromOperations>"), CloneOperations);

			//LineSpace = GetLineSpace(Struct, _T("<CloneToOperations>"));
			//CloneOperations = MakeCloneOperations(StructDefine, LineSpace, _T("CloneObject."), m_InterfaceConfig.MemberVariablePrefix, SpaceName);
			//Struct.Replace(_T("<CloneToOperations>"), CloneOperations);

			SelectBlock(Struct, _T("IsFirst"), StructCount == 0);
			StructCount++;

			StructDefines += Struct + _T("\r\n\r\n");
		}
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	StructDefines.Replace(_T("\r\n"), Space);
	return StructDefines;
}

CString CCallInterfaceMakerDlg::MakeStructMembers(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SplitChar, bool AllMemberAddSplitChar, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Members;
	int MaxLen = 0;
	for (size_t i = 0;i < StructInfo.MemberList.size();i++)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		if (pTypeInfo)
		{
			if (pTypeInfo->CType.GetLength() > MaxLen)
				MaxLen = pTypeInfo->CType.GetLength();
		}

	}
	CString MemberTemplate;
	if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
		MemberTemplate = m_DataObjectMemberTemplate;
	else
		MemberTemplate = m_StructMemberTemplate;

	int HeadLen = GetHeadLen(MemberTemplate, _T("<VariableDefine>"));
	MaxLen += HeadLen;
	int SpaceLen = (((int)ceil(MaxLen / 4.0f)) + 1) * 4 - HeadLen;

	for (size_t i = 0; i < StructInfo.MemberList.size(); i++)
	{
		CString Member = MemberTemplate;
		STRUCT_MEMBER_INFO& MemberInfo = StructInfo.MemberList[i];

		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[i].Type);
		CString Space;
		if (pTypeInfo)
		{
			CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;

			if (MemberInfo.IsArray)
			{
				VariableDefine = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.VariableDefine, pTypeInfo);
			}

			VariableDefine.Replace(_T("<Type>"), pTypeInfo->CType);
			int CurSpaceCount = (int)ceil((SpaceLen - pTypeInfo->CType.GetLength()) / 4.0f);
			while (CurSpaceCount)
			{
				Space += _T("	");
				CurSpaceCount--;
			}
			VariableDefine.Replace(_T("<Space>"), Space);
			VariableDefine.Replace(_T("<SpaceName>"), SpaceName);
			CString VarName;


			VarName = MemberInfo.Name;
			if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
			{
				VarName = _T("m_") + VarName;
			}
			VariableDefine.Replace(_T("<Variable>"), VarName);
			if (AllMemberAddSplitChar || i < StructInfo.MemberList.size() - 1)
				VariableDefine = VariableDefine + SplitChar;
			Member.Replace(_T("<VariableDefine>"), VariableDefine);
		}
		Member.Replace(_T("<MemberName>"), MemberInfo.Name);
		Member.Replace(_T("<OrginType>"), MemberInfo.Type);
		Member.Replace(_T("<ExtendType>"), MemberInfo.ExtendType);
		Member.Replace(_T("<ShowName>"), MemberInfo.ShowName);
		Member.Replace(_T("<BindData>"), MemberInfo.BindData);
		CString Description = MemberInfo.Description;
		if (!Description.IsEmpty())
		{
			Description = m_InterfaceConfig.CommentPrefix + Description;
			Description.Replace(_T("\r"), _T(""));
			Description.Replace(_T("\n"), _T(""));
		}
		Member.Replace(_T("<Description>"), Description);
		Member.Replace(_T("<OrginDescription>"), MemberInfo.Description);
		SelectBlock(Member, _T("IsArray"), MemberInfo.IsArray);
		SelectBlock(Member, _T("HideInEditorList"), MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_EDITOR_LIST);
		SelectBlock(Member, _T("HideInPropertyGrid"), MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_PROPERTY_GRID);
		SelectBlock(Member, _T("HideInXls"), MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_XLS);
		SelectBlock(Member, _T("IsKey"), MemberInfo.Flag & STRUCT_MEMBER_FLAG_IS_KEY);
		SelectBlock(Member, _T("IsLast"), i == (StructInfo.MemberList.size() - 1));

		Members += Member + _T("\r\n");
	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	Members.Replace(_T("\r\n"), Space);
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
			Source += Operation + _T("\r\n");
		}
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
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
		CString LineSpace = GetLineSpace(Operation, _T("<CloneOperation>"));
		RemoveBlock(SubOperation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
		RetainBlock(SubOperation, _T("<IfInArray>"), _T("</IfInArray>"));
		CString ArrayVar = m_InterfaceConfig.ArrayDefineConfig.ConstIndexOperation;
		ArrayVar.Replace(_T("<Index>"), _T("i"));
		SubOperation.Replace(_T("<Variable>"), ArrayVar);
		RemoveBlock(SubOperation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
		RemoveBlock(SubOperation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
		RemoveBlock(SubOperation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
		SubOperation.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
		Operation.Replace(_T("<CloneOperation>"), SubOperation);

		CString VariableDefine = pTypeInfo->GenerateOperations.VariableDefine;
		VariableDefine.Replace(_T("<Type>"), pTypeInfo->CType);
		VariableDefine.Replace(_T("<Space>"), _T("	"));
		VariableDefine.Replace(_T("<Variable>"), _T("ArrayElement"));
		VariableDefine = m_InterfaceConfig.LocalVariableDefinePrefix + VariableDefine;
		Operation.Replace(_T("<ArrayElementVariableDefine>"), VariableDefine);

		CString VariableInit = pTypeInfo->GenerateOperations.InitOperation;
		VariableInit.Replace(_T("<Variable>"), _T("ArrayElement"));
		Operation.Replace(_T("<ArrayElementVariableInit>"), VariableInit);
	}
	else
	{
		Operation = pTypeInfo->GenerateOperations.CloneOperation;
	}

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
	{
		RemoveBlock(Operation, _T("<IfNotDataObject>"), _T("</IfNotDataObject>"));
		Operation.Replace(_T("<IfDataObject>"), _T(""));
		Operation.Replace(_T("</IfDataObject>"), _T(""));
	}
	else
	{
		RemoveBlock(Operation, _T("<IfDataObject>"), _T("</IfDataObject>"));
		Operation.Replace(_T("<IfNotDataObject>"), _T(""));
		Operation.Replace(_T("</IfNotDataObject>"), _T(""));
	}

	if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
	{
		RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
		Operation.Replace(_T("<If64Bit>"), _T(""));
		Operation.Replace(_T("</If64Bit>"), _T(""));
	}
	else
	{
		RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
		Operation.Replace(_T("<IfNot64Bit>"), _T(""));
		Operation.Replace(_T("</IfNot64Bit>"), _T(""));
	}

	if (pTypeInfo->Flag & (TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
	{
		RemoveBlock(Operation, _T("<IfNotRefType>"), _T("</IfNotRefType>"));
		Operation.Replace(_T("<IfRefType>"), _T(""));
		Operation.Replace(_T("</IfRefType>"), _T(""));
	}
	else
	{
		RemoveBlock(Operation, _T("<IfRefType>"), _T("</IfRefType>"));
		Operation.Replace(_T("<IfNotRefType>"), _T(""));
		Operation.Replace(_T("</IfNotRefType>"), _T(""));
	}
	CString VarName = VariableName;
	if (InDataObject)
	{
		VarName = _T("m_") + VarName;
		RemoveBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
		RetainBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));

		if (BeCheckMF)
			RetainBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
		else
			RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));

		if (BeUpdateMF)
			RetainBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
		else
			RemoveBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
	}
	else
	{
		RemoveBlock(Operation, _T("<IfInDataObject>"), _T("</IfInDataObject>"));
		RetainBlock(Operation, _T("<IfNotInDataObject>"), _T("</IfNotInDataObject>"));
		RemoveBlock(Operation, _T("<IfCheckMF>"), _T("</IfCheckMF>"));
		RemoveBlock(Operation, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
	}

	Operation.Replace(_T("<LeftValue>"), szLeftValue);
	Operation.Replace(_T("<RightValue>"), szRightValue);

	Operation.Replace(_T("<Type>"), pTypeInfo->CType);
	Operation.Replace(_T("<VariableName>"), VariableName);

	Operation.Replace(_T("<PackFlag>"), PackFlag);

	Operation.Replace(_T("<Variable>"), VarName);
	Operation.Replace(_T("<ClassName>"), ClassName);

	CString ModifyFlagEnumName;
	ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ClassName));
	Operation.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

	CString ModifyFlag = _T("MF_") + ClassNameToUpper(VariableName);
	Operation.Replace(_T("<ModifyFlag>"), ModifyFlag);

	Operation.Replace(_T("<SpaceName>"), SpaceName);

	if (szLineSpace && (szLineSpace[0]))
	{
		CString Space = _T("\r\n");
		Space += szLineSpace;
		Operation.Replace(_T("\r\n"), Space);
	}
	return Operation;
}

bool CCallInterfaceMakerDlg::MakeDataObjectFlagIndicesEnum(vector<STRUCT_DEFINE_INFO*>& AllDataObjectList, ENUM_DEFINE_INFO& EnumInfo)
{

	EnumInfo.Name = _T("DATA_OBJECT_FLAGS");
	EnumInfo.ShortName = EnumInfo.Name;



	for (STRUCT_DEFINE_INFO* pStruct : AllDataObjectList)
	{
		ENUM_MEMBER_INFO MemberInfo;
		CString MemberName;
		MemberName = ClassNameToUpper(pStruct->Name);
		MemberInfo.Name.Format(_T("DATA_OBJECT_FLAG_%s"), MemberName);

		EnumInfo.MemberList.push_back(MemberInfo);
	}

	ENUM_MEMBER_INFO MemberInfo;
	MemberInfo.Name = _T("DATA_OBJECT_FLAG_MAX");
	EnumInfo.MemberList.push_back(MemberInfo);

	return true;
}

bool CCallInterfaceMakerDlg::MakeDataObjectSSTIDsEnum(vector<STRUCT_DEFINE_INFO*>& AllDataObjectList, ENUM_DEFINE_INFO& EnumInfo)
{
	EnumInfo.Name = _T("DATA_OBJECT_SSTIDS");
	EnumInfo.ShortName = EnumInfo.Name;

	for (STRUCT_DEFINE_INFO* pStruct : AllDataObjectList)
	{
		ENUM_MEMBER_INFO MemberInfo;
		CString MemberName;
		MemberName = ClassNameToUpper(pStruct->Name);
		MemberInfo.Name.Format(_T("SST_DO_%s"), MemberName);
		MemberInfo.Value.Format(_T("%d"), pStruct->ObjectID);

		EnumInfo.MemberList.push_back(MemberInfo);
	}
	return true;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagEnumDefine(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{

	ENUM_DEFINE_INFO EnumInfo;

	EnumInfo.Name.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
	EnumInfo.ShortName = EnumInfo.Name;
	EnumInfo.Flag |= ENUM_DEFINE_FLAG_IS_64BIT;

	EnumInfo.MemberList.resize(StructInfo.MemberList.size());

	CString EnumType = _T("UINT64");
	TYPE_DEFINE* pType = FindVarType(EnumType);
	if (pType)
		EnumType = pType->CType;

	//UINT64 MF_ALL=0;
	for (size_t j = 0; j < StructInfo.MemberList.size(); j++)
	{
		CString MemberName;
		MemberName = ClassNameToUpper(StructInfo.MemberList[j].Name);
		EnumInfo.MemberList[j].Name.Format(_T("MF_%s"), MemberName);
		EnumInfo.MemberList[j].Value.Format(_T("%d"), j);
		EnumInfo.MemberList[j].Flag = ENUM_MEMBER_FLAG_IS_BIT_MASK;
		//MF_ALL=(MF_ALL<<1)|1;
	}
	//EnumInfo.MemberList[StructInfo.MemberList.size()].Name=_T("MF_ALL");
	//EnumInfo.MemberList[StructInfo.MemberList.size()].Value.Format(_T("0x%llX"),MF_ALL);

	return MakeEnumDefine(EnumInfo, SpaceName, szLineSpace);
}

CString CCallInterfaceMakerDlg::MakeDataObjectMembersGetModifyFlag(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Source;

	for (size_t j = 0;j < StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[j].Type);
		if (pTypeInfo && (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT) &&
			((StructInfo.MemberList[j].Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0) &&
			(!StructInfo.MemberList[j].IsArray))
		{
			CString Temp = m_DataObjectGetModifyFlagDefineTemplate;

			CString VarName = m_InterfaceConfig.MemberVariablePrefix + _T("m_") + StructInfo.MemberList[j].Name;
			Temp.Replace(_T("<Variable>"), VarName);

			Source += Temp;
		}
	}


	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataDataObjectMembersIsModified(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Source;

	for (size_t j = 0;j < StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[j].Type);
		if (pTypeInfo && (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT) &&
			((StructInfo.MemberList[j].Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0) &&
			(!StructInfo.MemberList[j].IsArray))
		{
			CString Temp = m_DataObjectIsModifiedDefineTemplate;

			CString VarName = m_InterfaceConfig.MemberVariablePrefix + _T("m_") + StructInfo.MemberList[j].Name;
			Temp.Replace(_T("<Variable>"), VarName);

			Source += Temp;
		}
	}

	//if(Source.IsEmpty())
	//	Source=_T("||false");

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataDataObjectMembersClearModifyFlag(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Source;

	for (size_t j = 0;j < StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[j].Type);
		if (pTypeInfo && (pTypeInfo->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT) &&
			((StructInfo.MemberList[j].Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0) &&
			(!StructInfo.MemberList[j].IsArray))
		{
			CString Temp = m_DataObjectClearModifyFlagDefineTemplate;

			CString VarName = m_InterfaceConfig.MemberVariablePrefix + _T("m_") + StructInfo.MemberList[j].Name;
			Temp.Replace(_T("<Variable>"), VarName);

			Source += Temp;
		}
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}



CString CCallInterfaceMakerDlg::MakeDataObjectSetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Methods;

	for (size_t j = 0;j < StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[j].Type);
		if (pTypeInfo)
		{
			CString Method = pTypeInfo->GenerateOperations.SetMethodDeclare;
			if (StructInfo.MemberList[j].IsArray)
			{
				Method = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.SetMethodDeclare, pTypeInfo);
			}

			Method.Replace(_T("<Type>"), pTypeInfo->CType);
			Method.Replace(_T("<VariableName>"), StructInfo.MemberList[j].Name);

			Methods += Method + _T("\r\n");
		}
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
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
					PackFlag.Format(_T("DOMF_%s_FULL"), ClassNameToUpper(pTypeInfo->Name));
				CString LineSpace = GetLineSpace(Method, _T("<CloneOperation>"));
				CString CloneOperation = MakeCloneOperation(MemberInfo.Name, pTypeInfo, true, true, false,
					(MemberInfo.Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE) == 0, PackFlag, _T(""), _T("Value."), StructInfo.Name, SpaceName, LineSpace);
				Method.Replace(_T("<CloneOperation>"), CloneOperation);
			}

			if (MemberInfo.Flag & STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE)
			{
				RemoveBlock(Method, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
			}
			else
			{
				RetainBlock(Method, _T("<IfUpdateMF>"), _T("</IfUpdateMF>"));
			}

			if (pTypeInfo->Flag & (TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
			{
				RemoveBlock(Method, _T("<IfNotRefType>"), _T("</IfNotRefType>"));
				Method.Replace(_T("<IfRefType>"), _T(""));
				Method.Replace(_T("</IfRefType>"), _T(""));
			}
			else
			{
				RemoveBlock(Method, _T("<IfRefType>"), _T("</IfRefType>"));
				Method.Replace(_T("<IfNotRefType>"), _T(""));
				Method.Replace(_T("</IfNotRefType>"), _T(""));
			}

			if (pTypeInfo->Flag & TYPE_DEFINE_FLAG_64BIT)
			{
				RemoveBlock(Method, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
				Method.Replace(_T("<If64Bit>"), _T(""));
				Method.Replace(_T("</If64Bit>"), _T(""));
			}
			else
			{
				RemoveBlock(Method, _T("<If64Bit>"), _T("</If64Bit>"));
				Method.Replace(_T("<IfNot64Bit>"), _T(""));
				Method.Replace(_T("</IfNot64Bit>"), _T(""));
			}

			Method.Replace(_T("<Type>"), pTypeInfo->CType);
			Method.Replace(_T("<VariableName>"), MemberInfo.Name);

			CString VarName = m_InterfaceConfig.MemberVariablePrefix + _T("m_") + MemberInfo.Name;
			Method.Replace(_T("<Variable>"), VarName);
			Method.Replace(_T("<ClassName>"), StructInfo.Name);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(StructInfo.Name));
			Method.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

			CString ModifyFlag = _T("MF_") + ClassNameToUpper(MemberInfo.Name);
			Method.Replace(_T("<ModifyFlag>"), ModifyFlag);

			Method.Replace(_T("<SpaceName>"), SpaceName);



			Methods += Method + _T("\r\n");
		}
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectGetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Methods;

	for (size_t j = 0;j < StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[j].Type);
		if (pTypeInfo)
		{
			CString Method = pTypeInfo->GenerateOperations.GetMethodDeclare;
			if (StructInfo.MemberList[j].IsArray)
			{
				Method = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.GetMethodDeclare, pTypeInfo);
			}

			Method.Replace(_T("<Type>"), pTypeInfo->CType);
			Method.Replace(_T("<VariableName>"), StructInfo.MemberList[j].Name);
			CString ConstMethod = Method;
			Method.Replace(_T("<const>"), _T(""));
			Methods += Method + _T("\r\n");

			if (ConstMethod.Find(_T("<const>")) >= 0)
			{
				if (pTypeInfo->Flag & (TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
				{
					ConstMethod.Replace(_T("<const>"), _T("const"));
					Methods += ConstMethod + _T("\r\n");
				}
			}
		}
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectGetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Methods;

	for (size_t j = 0;j < StructInfo.MemberList.size();j++)
	{
		TYPE_DEFINE* pTypeInfo = FindVarType(StructInfo.MemberList[j].Type);
		if (pTypeInfo)
		{
			CString Method = pTypeInfo->GenerateOperations.GetMethodDefine;
			if (StructInfo.MemberList[j].IsArray)
			{
				Method = ProcessArrayOperation(m_InterfaceConfig.ArrayDefineConfig.GetMethodDefine, pTypeInfo);
			}

			Method.Replace(_T("<Type>"), pTypeInfo->CType);
			Method.Replace(_T("<VariableName>"), StructInfo.MemberList[j].Name);
			CString VarName = m_InterfaceConfig.MemberVariablePrefix + _T("m_") + StructInfo.MemberList[j].Name;
			Method.Replace(_T("<Variable>"), VarName);
			Method.Replace(_T("<ClassName>"), StructInfo.Name);
			Method.Replace(_T("<SpaceName>"), SpaceName);

			CString ConstMethod = Method;
			Method.Replace(_T("<const>"), _T(""));
			Methods += Method + _T("\r\n");

			if (ConstMethod.Find(_T("<const>")) >= 0)
			{
				if (pTypeInfo->Flag & (TYPE_DEFINE_FLAG_DATA_OBJECT | TYPE_DEFINE_FLAG_STRUCT | TYPE_DEFINE_FLAG_REF_TYPE))
				{
					ConstMethod.Replace(_T("<const>"), _T("const"));
					Methods += ConstMethod + _T("\r\n");
				}
			}
		}
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Methods.Replace(_T("\r\n"), Space);
	return Methods;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagsHeader(vector<STRUCT_DEFINE_INFO*>& AllDataObjectList, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Source;

	UINT ModifyFlagDefineSize = 0;

	for (STRUCT_DEFINE_INFO* pStruct : AllDataObjectList)
	{
		CString Define = m_DataObjectModifyFlagHeaderDefineTemple;

		CString Name;
		Name.Format(_T("DOMF_%s_FULL"), ClassNameToUpper(pStruct->Name));
		Define.Replace(_T("<FlagName>"), Name);

		Source += Define + _T("\r\n");
	}

	for (size_t i = 0; i < DataObjectModifyFlags.size(); i++)
	{

		CString Define = m_DataObjectModifyFlagHeaderDefineTemple;

		Define.Replace(_T("<FlagName>"), DataObjectModifyFlags[i].Name);
		Source += Define + _T("\r\n");

	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagsSource(vector<STRUCT_DEFINE_INFO*>& AllDataObjectList, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Source;

	UINT ModifyFlagDefineSize = 0;

	for (STRUCT_DEFINE_INFO* pStruct : AllDataObjectList)
	{
		CString Define = m_DataObjectModifyFlagDefineTemple;

		DATA_OBJECT_MODIFY_FLAG ModifyFlag = MakeDataObjectFullModifyFlag(*pStruct, AllDataObjectList);

		ModifyFlagDefineSize = ModifyFlag.ModifyFlag.size();

		ModifyFlag.Name.Format(_T("DOMF_%s_FULL"), ClassNameToUpper(pStruct->Name));
		Define.Replace(_T("<FlagName>"), ModifyFlag.Name);
		Define.Replace(_T("<SpaceName>"), SpaceName);

		CString Space = GetLineSpace(Define, _T("<FlagData>"));
		CString FlagData = MakeDataObjectModifyFlagData(ModifyFlag, SpaceName, Space);
		Define.Replace(_T("<FlagData>"), FlagData);

		Source += Define + _T("\r\n");
	}

	for (size_t i = 0; i < DataObjectModifyFlags.size(); i++)
	{

		CString Define = m_DataObjectModifyFlagDefineTemple;

		if (DataObjectModifyFlags[i].ModifyFlag.size() != ModifyFlagDefineSize)
		{
			CString Msg;
			Msg.Format(_T("[%s]的大小不正确,"), DataObjectModifyFlags[i].Name);
			AfxMessageBox(Msg);
		}

		Define.Replace(_T("<FlagName>"), DataObjectModifyFlags[i].Name);
		Define.Replace(_T("<SpaceName>"), SpaceName);

		CString Space = GetLineSpace(Define, _T("<FlagData>"));
		CString FlagData = MakeDataObjectModifyFlagData(DataObjectModifyFlags[i], SpaceName, Space);
		Define.Replace(_T("<FlagData>"), FlagData);

		Source += Define + _T("\r\n");

	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeDataObjectModifyFlagData(DATA_OBJECT_MODIFY_FLAG& ModifyFlag, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Source;
	CString Template = m_DataObjectModifyFlagUnitTemple;
	CString ObjSeparatorStr = GetBlock(Template, _T("<ObjSeparator>"), _T("</ObjSeparator>"));
	RemoveBlock(Template, _T("<ObjSeparator>"), _T("</ObjSeparator>"));
	CString FlagSeparatorStr = GetBlock(Template, _T("<FlagSeparator>"), _T("</FlagSeparator>"));
	RemoveBlock(Template, _T("<FlagSeparator>"), _T("</FlagSeparator>"));
	CString ZeroValue = GetBlock(Template, _T("<ZeroValue>"), _T("</ZeroValue>"));
	RemoveBlock(Template, _T("<ZeroValue>"), _T("</ZeroValue>"));
	CString ObjTemplate = GetBlock(Template, _T("<FlagUnit>"), _T("</FlagUnit>"));
	ReplaceBlock(Template, _T("<FlagUnit>"), _T("</FlagUnit>"), _T("<ObjFlag>"));
	for (size_t i = 0; i < ModifyFlag.ModifyFlag.size(); i++)
	{
		CString Member;
		for (size_t j = 0; j < ModifyFlag.ModifyFlag[i].Members.size(); j++)
		{

			CString FlagUnit = ObjTemplate;

			FlagUnit.Replace(_T("<SpaceName>"), SpaceName);
			FlagUnit.Replace(_T("<ClassName>"), ModifyFlag.ModifyFlag[i].ClassName);

			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(ModifyFlag.ModifyFlag[i].ClassName));
			FlagUnit.Replace(_T("<ModifyFlagEnumName>"), ModifyFlagEnumName);

			CString Flag;
			Flag.Format(_T("MF_%s"), ClassNameToUpper(ModifyFlag.ModifyFlag[i].Members[j]));
			FlagUnit.Replace(_T("<ModifyFlag>"), Flag);
			if (j)
				Member += FlagSeparatorStr;
			Member += FlagUnit;
		}
		if (i)
		{
			Source += ObjSeparatorStr + _T("\r\n");
		}
		if (Member.IsEmpty())
		{
			Source += ZeroValue;
		}
		else
		{
			CString Line = Template;
			Line.Replace(_T("<ObjFlag>"), Member);
			Source += Line;
		}
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeEnumDefine(ENUM_DEFINE_INFO& EnumInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Source;

	if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
		Source = m_EnumDefine64Template;
	else
		Source = m_EnumDefineTemplate;
	Source.Replace(_T("<EnumName>"), EnumInfo.Name);
	Source.Replace(_T("<ShowName>"), EnumInfo.ShowName);
	Source.Replace(_T("<SpaceName>"), SpaceName);

	if (EnumInfo.Description.IsEmpty())
		Source.Replace(_T("<Description>"), _T(""));
	else
		Source.Replace(_T("<Description>"), m_InterfaceConfig.CommentPrefix + EnumInfo.Description);
	Source.Replace(_T("<OrginDescription>"), EnumInfo.Description);

	if (EnumInfo.MemberList.size())
	{
		Source.Replace(_T("<LastEnumValue>"), EnumInfo.MemberList[EnumInfo.MemberList.size() - 1].Name);
	}

	CString MemberCount;

	MemberCount.Format(_T("%d"), GetEnumMemberCount(EnumInfo));

	Source.Replace(_T("<EnumMemberCount>"), MemberCount);

	if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_FLAG)
	{
		SelectBlock(Source, _T("IsFlagEnum"), true);
	}
	else
	{
		SelectBlock(Source, _T("IsFlagEnum"), false);
	}


	CString Space = GetLineSpace(Source, _T("<EnumMembers>"));
	CString EnumMembers;

	int Value = 0;
	for (size_t i = 0; i < EnumInfo.MemberList.size(); i++)
	{
		ENUM_MEMBER_INFO& MemberInfo = EnumInfo.MemberList[i];
		CString Member;
		if (MemberInfo.Flag & ENUM_MEMBER_FLAG_IS_BIT_MASK)
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

		Member.Replace(_T("<Name>"), MemberInfo.Name);
		Member.Replace(_T("<SpaceName>"), SpaceName);
		Member.Replace(_T("<EnumName>"), EnumInfo.Name);
		Member.Replace(_T("<BindType>"), MemberInfo.BindDataType);

		CString EnumValue = MemberInfo.Value;

		if (!EnumValue.IsEmpty())
			Value = _ttoi(EnumValue);

		if (MemberInfo.StrValue.IsEmpty())
			Member.Replace(_T("<EnumStrValue>"), MakeStringConst(MemberInfo.Name));
		else
			Member.Replace(_T("<EnumStrValue>"), MakeStringConst(MemberInfo.StrValue));

		if (EnumValue.IsEmpty())
		{
			RemoveBlock(EnumValue, _T("<IfHaveValue>"), _T("</IfHaveValue>"));
		}
		else
		{
			EnumValue.Replace(_T("<IfHaveValue>"), _T(""));
			EnumValue.Replace(_T("</IfHaveValue>"), _T(""));
		}

		//Member.Replace(_T("<Value>"), EnumValue);

		if (EnumValue.IsEmpty())
		{
			EnumValue.Format(_T("%d"), Value);
		}
		Member.Replace(_T("<Value>"), EnumValue);
		if (Member.Find(_T("<ParseredValue>")) >= 0)
			Member.Replace(_T("<ParseredValue>"), ParserEnumValue(EnumValue, MemberInfo.Flag & ENUM_MEMBER_FLAG_IS_BIT_MASK));
		Value++;

		CString Desc = MemberInfo.Description;
		if (!MemberInfo.BindDataType.IsEmpty())
			Desc += _T("，参考") + MemberInfo.BindDataType;


		if (Desc.IsEmpty())
			Member.Replace(_T("<Description>"), _T(""));
		else
			Member.Replace(_T("<Description>"), m_InterfaceConfig.CommentPrefix + Desc);
		Member.Replace(_T("<OrginDescription>"), MemberInfo.Description);

		SelectBlock(Member, _T("IsLast"), i == (EnumInfo.MemberList.size() - 1));
		SelectBlock(Member, _T("HideInPropertyGrid"), MemberInfo.Flag & ENUM_MEMBER_FLAG_HIDE_IN_PROPERTY_GRID);

		EnumMembers += Member + _T("\r\n");
	}
	CString LineSpace = GetLineSpace(Source, _T("<EnumMembers>"));
	EnumMembers.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
	Source.Replace(_T("<EnumMembers>"), EnumMembers);

	if (EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_ENUM_LIST)
	{
		RetainBlock(Source, _T("<IfExportEnumList>"), _T("</IfExportEnumList>"));
		CString Operatios;

		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		{
			if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			{
				CString Operation = m_EnumListUnitTemplate;
				if (!Operatios.IsEmpty())
				{
					RetainBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
				}
				else
				{
					RemoveBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
				}
				if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
				{
					RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
					Operation.Replace(_T("<If64Bit>"), _T(""));
					Operation.Replace(_T("</If64Bit>"), _T(""));
				}
				else
				{
					RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
					Operation.Replace(_T("<IfNot64Bit>"), _T(""));
					Operation.Replace(_T("</IfNot64Bit>"), _T(""));
				}
				Operation.Replace(_T("<EnumName>"), EnumInfo.Name);
				Operation.Replace(_T("<EnumValue>"), Info.Name);
				Operatios.Append(Operation);
			}
		}

		Source.Replace(_T("<EnumValues>"), Operatios);
	}
	else
	{
		RemoveBlock(Source, _T("<IfExportEnumList>"), _T("</IfExportEnumList>"));
	}

	if ((EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_STR_VALUE) && IsNaturalEnum(EnumInfo))
	{
		RetainBlock(Source, _T("<IfExportEnumStrList>"), _T("</IfExportEnumStrList>"));
		CString Operatios;

		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		{
			if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			{
				CString Operation = m_EnumStrValueUnitTemplate;
				if (!Operatios.IsEmpty())
				{
					RetainBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
				}
				else
				{
					RemoveBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
				}
				if (Info.StrValue.IsEmpty())
					Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.Name));
				else
					Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.StrValue));
				Operatios.Append(Operation);
			}
		}

		Source.Replace(_T("<EnumStrValues>"), Operatios);

		Operatios.Empty();
		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		{
			if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			{
				CString Operation = m_EnumStrValueUnitTemplate;
				if (!Operatios.IsEmpty())
				{
					RetainBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
				}
				else
				{
					RemoveBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
				}
				if (Info.StrValue.IsEmpty())
					Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.Name));
				else
					Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.Description));
				Operatios.Append(Operation);
			}
		}

		Source.Replace(_T("<EnumStrDescs>"), Operatios);
	}
	else
	{
		RemoveBlock(Source, _T("<IfExportEnumStrList>"), _T("</IfExportEnumStrList>"));
	}

	if (EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_STR_TRANS_FN)
	{
		RetainBlock(Source, _T("<IfExportEnumTransFn>"), _T("</IfExportEnumTransFn>"));

		CString Operations;
		CString LineSpace;

		if ((EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_STR_VALUE) && IsNaturalEnum(EnumInfo))
		{
			RemoveBlock(Source, _T("<NotHaveEnumStrArray>"), _T("</NotHaveEnumStrArray>"));
			RetainBlock(Source, _T("<HaveEnumStrArray>"), _T("</HaveEnumStrArray>"));
		}
		else
		{
			RemoveBlock(Source, _T("<HaveEnumStrArray>"), _T("</HaveEnumStrArray>"));
			RetainBlock(Source, _T("<NotHaveEnumStrArray>"), _T("</NotHaveEnumStrArray>"));

			Operations.Empty();
			LineSpace = GetLineSpace(Source, _T("<EnumToStrs>"));
			for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
			{
				if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
				{
					CString Operation = m_EnumToStrUnitTemplate;
					if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_FLAG)
					{
						SelectBlock(Operation, _T("IsFlagEnum"), true);
					}
					else
					{
						SelectBlock(Operation, _T("IsFlagEnum"), false);
					}
					if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
					{
						RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
						Operation.Replace(_T("<If64Bit>"), _T(""));
						Operation.Replace(_T("</If64Bit>"), _T(""));
					}
					else
					{
						RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
						Operation.Replace(_T("<IfNot64Bit>"), _T(""));
						Operation.Replace(_T("</IfNot64Bit>"), _T(""));
					}
					Operation.Replace(_T("<EnumName>"), EnumInfo.Name);
					Operation.Replace(_T("<EnumValue>"), Info.Name);
					if (Info.StrValue.IsEmpty())
						Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.Name));
					else
						Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.StrValue));
					if (!Operations.IsEmpty())
						Operations.Append(_T("\r\n"));
					Operations.Append(Operation);
				}
			}
			Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
			Source.Replace(_T("<EnumToStrs>"), Operations);

			Operations.Empty();
			LineSpace = GetLineSpace(Source, _T("<EnumToDescs>"));
			for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
			{
				if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
				{
					CString Operation = m_EnumToStrUnitTemplate;
					if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_FLAG)
					{
						SelectBlock(Operation, _T("IsFlagEnum"), true);
					}
					else
					{
						SelectBlock(Operation, _T("IsFlagEnum"), false);
					}
					if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
					{
						RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
						Operation.Replace(_T("<If64Bit>"), _T(""));
						Operation.Replace(_T("</If64Bit>"), _T(""));
					}
					else
					{
						RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
						Operation.Replace(_T("<IfNot64Bit>"), _T(""));
						Operation.Replace(_T("</IfNot64Bit>"), _T(""));
					}
					Operation.Replace(_T("<EnumName>"), EnumInfo.Name);
					Operation.Replace(_T("<EnumValue>"), Info.Name);
					if (Info.StrValue.IsEmpty())
						Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.Name));
					else
						Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.Description));
					if (!Operations.IsEmpty())
						Operations.Append(_T("\r\n"));
					Operations.Append(Operation);
				}
			}
			Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
			Source.Replace(_T("<EnumToDescs>"), Operations);
		}

		Operations.Empty();
		LineSpace = GetLineSpace(Source, _T("<StrToEnums>"));
		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
		{
			if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
			{
				CString Operation = m_StrToEnumUnitTemplate;
				if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_FLAG)
				{
					SelectBlock(Operation, _T("IsFlagEnum"), true);
				}
				else
				{
					SelectBlock(Operation, _T("IsFlagEnum"), false);
				}
				if (EnumInfo.Flag & ENUM_DEFINE_FLAG_IS_64BIT)
				{
					RemoveBlock(Operation, _T("<IfNot64Bit>"), _T("</IfNot64Bit>"));
					Operation.Replace(_T("<If64Bit>"), _T(""));
					Operation.Replace(_T("</If64Bit>"), _T(""));
				}
				else
				{
					RemoveBlock(Operation, _T("<If64Bit>"), _T("</If64Bit>"));
					Operation.Replace(_T("<IfNot64Bit>"), _T(""));
					Operation.Replace(_T("</IfNot64Bit>"), _T(""));
				}
				Operation.Replace(_T("<EnumName>"), EnumInfo.Name);
				Operation.Replace(_T("<EnumValue>"), Info.Name);
				if (Info.StrValue.IsEmpty())
					Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.Name));
				else
					Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.StrValue));
				if (!Operations.IsEmpty())
				{
					RetainBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
					Operations.Append(_T("\r\n"));
				}
				else
				{
					RemoveBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
				}

				Operations.Append(Operation);
			}
		}
		Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
		Source.Replace(_T("<StrToEnums>"), Operations);
	}
	else
	{
		RemoveBlock(Source, _T("<IfExportEnumTransFn>"), _T("</IfExportEnumTransFn>"));
	}

	//if ((EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_BIND_DATA_PROCESS))
	//{
	//	RetainBlock(Source, _T("<IfExportBindDataProcess>"), _T("</IfExportBindDataProcess>"));

	//	CString LineSpace;
	//	CString Operations;
	//	for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
	//	{
	//		if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
	//		{
	//			if (!Info.BindDataType.IsEmpty())
	//			{
	//				CString Operation = m_EnumBindDataFillUnit;
	//				Operation.Replace(_T("<EnumName>"), EnumInfo.Name);
	//				Operation.Replace(_T("<EnumValue>"), Info.Name);
	//				STRUCT_DEFINE_INFO* pStruct = GetStructDefineInfo(Info.BindDataType);
	//				if (pStruct)
	//				{
	//					SelectBlock(Operation, _T("HaveBindStruct"), true);
	//					CString Temp;
	//					Temp.Format(_T("%u"), pStruct->MemberList.size());
	//					Operation.Replace(_T("<BindDataLen>"), Temp);
	//					LineSpace = GetLineSpace(Operation, _T("<PropertyGridFillOperations>"));
	//					Temp = MakePropertyGridFillOperations(*pStruct, true, LineSpace);
	//					Operation.Replace(_T("<PropertyGridFillOperations>"), Temp);
	//				}
	//				else
	//				{
	//					SelectBlock(Operation, _T("HaveBindStruct"), false);
	//				}
	//				if (!Operations.IsEmpty())
	//					Operations.Append(_T("\r\n"));
	//				Operations.Append(Operation);
	//			}
	//		}
	//	}
	//	LineSpace = GetLineSpace(Source, _T("<BindDataFillOperations>"));
	//	Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
	//	Source.Replace(_T("<BindDataFillOperations>"), Operations);

	//	//Operations.Empty();
	//	//for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
	//	//{
	//	//	if ((Info.Flag & ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER) == 0)
	//	//	{
	//	//		if (!Info.BindDataType.IsEmpty())
	//	//		{
	//	//			CString Operation = m_EnumBindDataFetchUnit;
	//	//			Operation.Replace(_T("<EnumName>"), EnumInfo.Name);
	//	//			Operation.Replace(_T("<EnumValue>"), Info.Name);
	//	//			STRUCT_DEFINE_INFO* pStruct = GetStructDefineInfo(Info.BindDataType);
	//	//			if (pStruct)
	//	//			{
	//	//				SelectBlock(Operation, _T("HaveBindStruct"), true);
	//	//				CString Temp;
	//	//				Temp.Format(_T("%u"), pStruct->MemberList.size());
	//	//				Operation.Replace(_T("<BindDataLen>"), Temp);
	//	//				LineSpace = GetLineSpace(Operation, _T("<PropertyGridFetchOperations>"));
	//	//				Temp = MakePropertyGridFetchOperations(*pStruct, true, LineSpace);
	//	//				Operation.Replace(_T("<PropertyGridFetchOperations>"), Temp);
	//	//			}
	//	//			else
	//	//			{
	//	//				SelectBlock(Operation, _T("HaveBindStruct"), false);
	//	//			}
	//	//			if (!Operations.IsEmpty())
	//	//				Operations.Append(_T("\r\n"));
	//	//			Operations.Append(Operation);
	//	//		}
	//	//	}
	//	//}
	//	//LineSpace = GetLineSpace(Source, _T("<BindDataFetchOperations>"));
	//	//Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
	//	//Source.Replace(_T("<BindDataFetchOperations>"), Operations);
	//}
	//else
	//{
	//	RemoveBlock(Source, _T("<IfExportBindDataProcess>"), _T("</IfExportBindDataProcess>"));
	//}

	Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeEnumDefines(vector<ENUM_DEFINE_INFO>& EnumDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString EnumDefines;
	for (size_t i = 0; i < EnumDefineList.size(); i++)
	{
		CString Enum = MakeEnumDefine(EnumDefineList[i], SpaceName, _T(""));

		SelectBlock(Enum, _T("IsLast"), i == (EnumDefineList.size() - 1));

		EnumDefines += Enum + _T("\r\n\r\n");

	}
	CString Space = _T("\r\n");
	Space += szLineSpace;
	EnumDefines.Replace(_T("\r\n"), Space);
	return EnumDefines;
}

//CString CCallInterfaceMakerDlg::MakeEnumStrValue(ENUM_DEFINE_INFO& EnumInfo, LPCTSTR SpaceName,LPCTSTR szLineSpace)
//{
//	CString Source=m_EnumStrValueDefineTemplate;
//
//	Source.Replace(_T("<EnumName>"),EnumInfo.Name);
//	Source.Replace(_T("<SpaceName>"), SpaceName);
//
//	CString MemberCount;
//
//	MemberCount.Format(_T("%d"),EnumInfo.MemberList.size());
//
//	Source.Replace(_T("<EnumMemberCount>"),MemberCount);
//
//
//	CString StrValues;
//
//	for(size_t i=0;i<EnumInfo.MemberList.size();i++)
//	{
//		if(i)
//			StrValues+=_T(",");
//		if (EnumInfo.MemberList[i].StrValue.IsEmpty())
//			StrValues += MakeStringConst(EnumInfo.MemberList[i].Name);
//		else
//			StrValues += MakeStringConst(EnumInfo.MemberList[i].StrValue);
//	}	
//
//	Source.Replace(_T("<EnumStrValues>"),StrValues);
//
//	CString Space=_T("\r\n");
//	Space+=szLineSpace;
//	Source.Replace(_T("\r\n"),Space);
//	return Source;
//}

CString CCallInterfaceMakerDlg::MakeConstDefines(vector<CONST_DEFINE_INFO>& ConstDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Source;


	for (size_t i = 0;i < ConstDefineList.size();i++)
	{
		TYPE_DEFINE* pType = FindVarType(ConstDefineList[i].Type);
		CString Const;
		if (pType)
		{
			Const = m_ConstDefineTemplate;

			Const.Replace(_T("<Type>"), pType->CType);
			Const.Replace(_T("<SpaceName>"), SpaceName);
		}
		else
		{
			Const = m_MacroDefineTemplate;
		}

		if (ConstDefineList[i].Description.IsEmpty())
			Const.Replace(_T("<Description>"), _T(""));
		else
			Const.Replace(_T("<Description>"), _T("\\") + ConstDefineList[i].Description);
		Const.Replace(_T("<Name>"), ConstDefineList[i].Name);
		CString Value = ConstDefineList[i].Value;
		Value.Trim();
		if (Value.GetLength() && Value[0] == '"' && Value[Value.GetLength() - 1] == '"')
		{
			Value.Trim('"');
			Value = MakeStringConst(Value);
		}
		Const.Replace(_T("<Value>"), Value);

		Source += Const + _T("\r\n\r\n");

	}



	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeStringConst(LPCTSTR StrValue)
{
	CString Value = m_StringConstDefineTemplate;

	Value.Replace(_T("<StrValue>"), StrValue);

	return Value;
}

CString CCallInterfaceMakerDlg::MakeToStringExp(STRUCT_MEMBER_INFO& MemberInfo, TYPE_DEFINE* pType, bool IsInDataObject)
{
	CString ToStringOP = pType->GenerateOperations.ToStringOperation;
	SelectBlock(ToStringOP, _T("HaveStrTrans"), pType->Flag & TYPE_DEFINE_FLAG_ENUM_HAVE_STR_TRANS);
	SelectBlock(ToStringOP, _T("HaveExtendType"), !MemberInfo.ExtendType.IsEmpty());
	CString VarName = MemberInfo.Name;
	if (IsInDataObject)
		VarName = _T("m_") + VarName;
	VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
	ToStringOP.Replace(_T("<Variable>"), VarName);
	ToStringOP.Replace(_T("<Type>"), pType->CType);
	ToStringOP.Replace(_T("<ExtendType>"), MemberInfo.ExtendType);
	return ToStringOP;
}

//CString CCallInterfaceMakerDlg::MakeEnumStrValues(vector<ENUM_DEFINE_INFO>& EnumDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
//{
//	CString EnumDefines;
//	for(size_t i=0;i<EnumDefineList.size();i++)
//	{		
//		if ((EnumDefineList[i].Flag & ENUM_DEFINE_FLAG_EXPORT_STR_VALUE) && IsNaturalEnum(EnumDefineList[i]))
//		{
//			CString StrValues = MakeEnumStrValue(EnumDefineList[i], SpaceName, _T(""));
//
//			EnumDefines += StrValues + _T("\r\n\r\n");
//		}
//	}
//	CString Space=_T("\r\n");
//	Space+=szLineSpace;
//	EnumDefines.Replace(_T("\r\n"),Space);
//	return EnumDefines;
//}

//CString CCallInterfaceMakerDlg::MakeEnumStrTransFn(ENUM_DEFINE_INFO& EnumInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
//{
//	CString Source = m_EnumStrTransFnTemplate;
//
//	Source.Replace(_T("<EnumName>"), EnumInfo.Name);
//	Source.Replace(_T("<SpaceName>"), SpaceName);
//
//	if (EnumInfo.MemberList.size())
//	{
//		Source.Replace(_T("<LastEnumValue>"), EnumInfo.MemberList[EnumInfo.MemberList.size() - 1].Name);
//	}
//
//	CString MemberCount;
//
//	MemberCount.Format(_T("%d"), EnumInfo.MemberList.size());
//
//	Source.Replace(_T("<EnumMemberCount>"), MemberCount);
//
//	CString Operatios;
//	CString LineSpace;
//
//	if ((EnumInfo.Flag & ENUM_DEFINE_FLAG_EXPORT_STR_VALUE) && IsNaturalEnum(EnumInfo))
//	{
//		RemoveBlock(Source, _T("<NotHaveEnumStrArray>"), _T("</NotHaveEnumStrArray>"));
//		RetainBlock(Source, _T("<HaveEnumStrArray>"), _T("</HaveEnumStrArray>"));
//	}
//	else
//	{
//		RemoveBlock(Source, _T("<HaveEnumStrArray>"), _T("</HaveEnumStrArray>"));
//		RetainBlock(Source, _T("<NotHaveEnumStrArray>"), _T("</NotHaveEnumStrArray>"));
//
//		Operatios.Empty();
//		LineSpace = GetLineSpace(Source, _T("<EnumToStrs>"));
//		for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
//		{
//			CString Operation = m_EnumToStrUnitTemplate;
//			Operation.Replace(_T("<EnumName>"), EnumInfo.Name);
//			Operation.Replace(_T("<EnumValue>"), Info.Name);
//			if (Info.StrValue.IsEmpty())
//				Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.Name));
//			else
//				Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.StrValue));
//			if (!Operatios.IsEmpty())
//				Operatios.Append(_T("\r\n"));
//			Operatios.Append(Operation);
//		}
//		Operatios.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
//		Source.Replace(_T("<EnumToStrs>"), Operatios);
//	}
//
//	Operatios.Empty();
//	LineSpace = GetLineSpace(Source, _T("<StrToEnums>"));
//	for (ENUM_MEMBER_INFO& Info : EnumInfo.MemberList)
//	{
//		CString Operation = m_StrToEnumUnitTemplate;
//		Operation.Replace(_T("<EnumName>"), EnumInfo.Name);
//		Operation.Replace(_T("<EnumValue>"), Info.Name);
//		if (Info.StrValue.IsEmpty())
//			Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.Name));
//		else
//			Operation.Replace(_T("<EnumStrValue>"), MakeStringConst(Info.StrValue));
//		if (!Operatios.IsEmpty())
//		{
//			RetainBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
//			Operatios.Append(_T("\r\n"));
//		}			
//		else
//		{
//			RemoveBlock(Operation, _T("<IfNotFirst>"), _T("</IfNotFirst>"));
//		}
//			
//		Operatios.Append(Operation);
//	}
//	Operatios.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
//	Source.Replace(_T("<StrToEnums>"), Operatios);
//
//	CString Space = _T("\r\n");
//	Space += szLineSpace;
//	Source.Replace(_T("\r\n"), Space);
//	return Source;
//}
//CString CCallInterfaceMakerDlg::MakeEnumStrTransFns(vector<ENUM_DEFINE_INFO>& EnumDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
//{
//	CString EnumDefines;
//	for (size_t i = 0; i < EnumDefineList.size(); i++)
//	{
//		if (EnumDefineList[i].Flag & ENUM_DEFINE_FLAG_EXPORT_STR_TRANS_FN)
//		{
//			CString StrValues = MakeEnumStrTransFn(EnumDefineList[i], SpaceName, _T(""));
//
//			EnumDefines += StrValues;
//		}
//	}
//	CString Space = _T("\r\n");
//	Space += szLineSpace;
//	EnumDefines.Replace(_T("\r\n"), Space);
//	return EnumDefines;
//}

DATA_OBJECT_MODIFY_FLAG CCallInterfaceMakerDlg::MakeDataObjectFullModifyFlag(STRUCT_DEFINE_INFO& StructInfo, vector<STRUCT_DEFINE_INFO*>& DataObjectList)
{
	DATA_OBJECT_MODIFY_FLAG ModifyFlag;

	ModifyFlag.ModifyFlag.resize(DataObjectList.size());

	for (size_t i = 0;i < DataObjectList.size();i++)
	{
		if (DataObjectList[i]->Name == StructInfo.Name || HaveMemberByType(StructInfo, DataObjectList[i]->Name))
		{
			ModifyFlag.ModifyFlag[i].ClassName = DataObjectList[i]->Name;
			for (size_t j = 0;j < DataObjectList[i]->MemberList.size();j++)
			{
				if ((DataObjectList[i]->MemberList[j].Flag & STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET) == 0)
				{
					ModifyFlag.ModifyFlag[i].Members.push_back(DataObjectList[i]->MemberList[j].Name);
				}
			}
		}
	}

	return ModifyFlag;
}

//CString CCallInterfaceMakerDlg::MakeStructEditorProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
//{
//	CString Source = m_StructEditorProcessTemplate;
//
//	Source.Replace(_T("<ClassName>"), StructInfo.Name);
//
//	if (StructInfo.BaseStruct.IsEmpty())
//	{
//		RemoveBlock(Source, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
//		RetainBlock(Source, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
//	}
//	else
//	{
//		RemoveBlock(Source, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
//		RetainBlock(Source, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
//		Source.Replace(_T("<BaseClass>"), StructInfo.BaseStruct);
//	}
//
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(StructInfo.Name));
//
//	CString Operations;
//	CString LineSpace = GetLineSpace(Source, _T("<CreateListHeaderOperations>"));
//	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
//	{
//		if ((!MemberInfo.ShowName.IsEmpty()) && (!MemberInfo.IsArray) && ((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_EDITOR_LIST) == 0))
//		{
//			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
//			if (pType && ((pType->Flag & TYPE_DEFINE_FLAG_STRUCT) == 0))
//			{
//				CString Operation = m_CreateListHeaderUnitTemplate;
//				Operation.Replace(_T("<VarShowName>"), MakeStringConst(MemberInfo.ShowName));
//				if (!Operations.IsEmpty())
//					Operations.Append(_T("\r\n"));
//				Operations.Append(Operation);
//			}
//		}
//	}
//	Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
//	Source.Replace(_T("<CreateListHeaderOperations>"), Operations);
//
//	Operations.Empty();
//	LineSpace = GetLineSpace(Source, _T("<FillListItemOperations>"));
//	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
//	{
//		if ((!MemberInfo.ShowName.IsEmpty()) && (!MemberInfo.IsArray) && ((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_EDITOR_LIST) == 0))
//		{
//			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
//			if (pType && ((pType->Flag & TYPE_DEFINE_FLAG_STRUCT) == 0))
//			{
//				CString Operation = m_FillListItemUnitTemplate;
//
//				Operation.Replace(_T("<VarStrValue>"), MakeToStringExp(MemberInfo, pType, false));
//				if (!Operations.IsEmpty())
//					Operations.Append(_T("\r\n"));
//				Operations.Append(Operation);
//			}
//		}
//	}
//	Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
//	Source.Replace(_T("<FillListItemOperations>"), Operations);
//
//	CString Temp;
//	Temp.Format(_T("%u"), StructInfo.MemberList.size());
//	Source.Replace(_T("<BindDataLen>"), Temp);
//
//	LineSpace = GetLineSpace(Source, _T("<PropertyGridFillOperations>"));
//	Operations = MakePropertyGridFillOperations(StructInfo, false, LineSpace);
//	Source.Replace(_T("<PropertyGridFillOperations>"), Operations);
//
//	LineSpace = GetLineSpace(Source, _T("<PropertyGridBindDataFillOperations>"));
//	Operations = MakePropertyGridFillOperations(StructInfo, true, LineSpace);
//	Source.Replace(_T("<PropertyGridBindDataFillOperations>"), Operations);
//
//	//LineSpace = GetLineSpace(Source, _T("<PropertyGridFetchOperations>"));
//	//Operations = MakePropertyGridFetchOperations(StructInfo, false, LineSpace);
//	//Source.Replace(_T("<PropertyGridFetchOperations>"), Operations);
//
//	//LineSpace = GetLineSpace(Source, _T("<PropertyGridAddItemOperations>"));
//	//Operations = MakePropertyGridAddItemOperations(StructInfo, false, LineSpace);
//	//Source.Replace(_T("<PropertyGridAddItemOperations>"), Operations);
//
//	//LineSpace = GetLineSpace(Source, _T("<PropertyGridDelItemOperations>"));
//	//Operations = MakePropertyGridDelItemOperations(StructInfo, false, LineSpace);
//	//Source.Replace(_T("<PropertyGridDelItemOperations>"), Operations);
//
//	//LineSpace = GetLineSpace(Source, _T("<PropertyGridMoveItemOperations>"));
//	//Operations = MakePropertyGridMoveItemOperations(StructInfo, false, LineSpace);
//	//Source.Replace(_T("<PropertyGridMoveItemOperations>"), Operations);
//
//	CString Space = _T("\r\n");
//	Space += szLineSpace;
//	Source.Replace(_T("\r\n"), Space);
//	return Source;
//}
//
//CString CCallInterfaceMakerDlg::MakePropertyGridFillOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operations;
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(StructInfo.Name));
//
//	UINT Index = 0;
//	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
//	{
//		if (((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_PROPERTY_GRID) == 0) &&
//			((!MemberInfo.IsArray) || (IsBeBind(StructInfo, MemberInfo.Name) == NULL)))
//		{
//			CString Operation = MakePropertyGridFillOperation(StructInfo, MemberInfo, Index, SSTIDEnumName, ReplaceByBindData, _T(""));
//			Operations.Append(Operation);
//			Operations.Append(_T("\r\n"));
//		}
//		Index++;
//	}
//	CString LineSpace;
//	LineSpace.Format(_T("\r\n%s"), szLineSpace);
//	Operations.Replace(_T("\r\n"), LineSpace);
//	return Operations;
//}
//
//CString CCallInterfaceMakerDlg::MakePropertyGridFillOperation(STRUCT_DEFINE_INFO& StructInfo, STRUCT_MEMBER_INFO& MemberInfo, UINT MemberIndex, LPCTSTR SSTIDEnumName, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operation;
//	TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
//	if (pType)
//	{
//		if (MemberInfo.IsArray)
//		{
//			Operation = m_InterfaceConfig.ArrayDefineConfig.PropertyGridFillOperation;
//			CString SubOperation = pType->GenerateOperations.PropertyGridFillOperation;
//			CString LineSpace = GetLineSpace(Operation, _T("<PropertyGridFillOperation>"));
//			SubOperation.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
//			RemoveBlock(SubOperation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//			RetainBlock(SubOperation, _T("<IfInArray>"), _T("</IfInArray>"));
//			CString Var = m_InterfaceConfig.ArrayDefineConfig.IndexOperation;
//			Var.Replace(_T("<Variable>"), MemberInfo.Name);
//			Var.Replace(_T("<Index>"), _T("i"));
//			SubOperation.Replace(_T("<Variable>"), Var);
//			SubOperation.Replace(_T("<Index>"), _T("i"));
//			Operation.Replace(_T("<PropertyGridFillOperation>"), SubOperation);
//		}
//		else
//		{
//			Operation = pType->GenerateOperations.PropertyGridFillOperation;
//			RemoveBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//			RetainBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//		}
//		if (!Operation.IsEmpty())
//		{
//			if (pType->Flag & TYPE_DEFINE_FLAG_ENUM)
//			{
//				ENUM_DEFINE_INFO* pEnum = GetEnumDefine(pType->CType);
//				if (pEnum)
//				{
//					CString MemberCount;
//					MemberCount.Format(_T("%d"), GetEnumMemberCount(*pEnum));
//					Operation.Replace(_T("<EnumMemberCount>"), MemberCount);
//					if (pEnum->Flag & ENUM_DEFINE_FLAG_IS_FLAG)
//					{
//						SelectBlock(Operation, _T("IsFlagEnum"), true);
//					}
//					else
//					{
//						SelectBlock(Operation, _T("IsFlagEnum"), false);
//					}
//				}
//
//				if (IsBeBind(StructInfo, MemberInfo.Name))
//				{
//					SelectBlock(Operation, _T("BeBind"), true);
//				}
//				else
//				{
//					SelectBlock(Operation, _T("BeBind"), false);
//				}
//			}
//
//			if (!MemberInfo.BindData.IsEmpty())
//			{
//				SelectBlock(Operation, _T("HaveBindData"), true);
//				STRUCT_MEMBER_INFO* pBindData = NULL;
//				for (STRUCT_MEMBER_INFO& Info : StructInfo.MemberList)
//				{
//					if (Info.Name == MemberInfo.BindData)
//					{
//						pBindData = &Info;
//						break;
//					}
//				}
//				if (pBindData)
//				{
//					TYPE_DEFINE* pBindDataType = FindVarType(pBindData->Type);
//					if (pBindDataType)
//						Operation.Replace(_T("<BindParentType>"), pBindDataType->CType);
//				}
//			}
//			else
//			{
//				SelectBlock(Operation, _T("HaveBindData"), false);
//			}
//
//			Operation.Replace(_T("<VarShowName>"), MakeStringConst(MemberInfo.ShowName));
//			Operation.Replace(_T("<Description>"), MakeStringConst(MemberInfo.Description));
//			Operation.Replace(_T("<VarStrValue>"), MakeToStringExp(MemberInfo, pType, false));
//			Operation.Replace(_T("<Type>"), pType->CType);
//
//			SelectBlock(Operation, _T("HaveExtendType"), !MemberInfo.ExtendType.IsEmpty());
//			Operation.Replace(_T("<ExtendType>"), MemberInfo.ExtendType);
//
//			if (ReplaceByBindData)
//			{
//				SelectBlock(Operation, _T("InBindData"), true);
//			}
//			else
//			{
//				SelectBlock(Operation, _T("InBindData"), false);
//				CString VarName = MemberInfo.Name;
//				if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
//					VarName = _T("m_") + VarName;
//				VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
//				Operation.Replace(_T("<Variable>"), VarName);
//				Operation.Replace(_T("<VariableName>"), MemberInfo.Name);
//				CString SST_ID;
//				SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
//				SST_ID.MakeUpper();
//				Operation.Replace(_T("<StructName>"), StructInfo.Name);
//				Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
//				Operation.Replace(_T("<SST_ID>"), SST_ID);
//				Operation.Replace(_T("<Index>"), _T("-1"));
//			}
//		}
//	}
//	CString LineSpace;
//	LineSpace.Format(_T("\r\n%s"), szLineSpace);
//	Operation.Replace(_T("\r\n"), LineSpace);
//	return Operation;
//}

//CString CCallInterfaceMakerDlg::MakePropertyGridFetchOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operations;
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(StructInfo.Name));
//
//	UINT Index = 0;
//	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
//	{			
//		CString Operation = m_PropertyGridFetchUnitTemplate;
//
//		if (MemberInfo.IsArray)
//		{
//			RemoveBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//			RetainBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//		}
//		else
//		{
//			RemoveBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//			RetainBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//		}
//
//		if (ReplaceByBindData)
//		{
//			SelectBlock(Operation, _T("InBindData"), true);			
//		}
//		else
//		{
//			SelectBlock(Operation, _T("InBindData"), false);
//			Operation.Replace(_T("<Variable>"), MemberInfo.Name);
//		}
//		
//		STRUCT_MEMBER_INFO* pBindParent = NULL;
//		if (MemberInfo.IsArray)
//		{
//			pBindParent = IsBeBind(StructInfo, MemberInfo.Name);
//			if (pBindParent)
//			{
//				SelectBlock(Operation, _T("IsBindData"), true);
//				Operation.Replace(_T("<BindEnumName>"), pBindParent->Type);
//				Operation.Replace(_T("<BindEnumVariable>"), pBindParent->Name);
//			}
//			else
//			{
//				SelectBlock(Operation, _T("IsBindData"), false);
//			}
//		}
//		else
//		{
//			SelectBlock(Operation, _T("IsBindData"), false);
//		}
//
//		if (pBindParent == NULL)
//		{
//			CString LineSpace = GetLineSpace(Operation, _T("<PropertyGridFetchOperation>"));
//			CString FetchOperation = MakePropertyGridFetchOperation(StructInfo, MemberInfo, Index, ReplaceByBindData, LineSpace);
//			Operation.Replace(_T("<PropertyGridFetchOperation>"), FetchOperation);
//		}
//
//		CString SST_ID;
//		SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
//		SST_ID.MakeUpper();
//		Operation.Replace(_T("<StructName>"), StructInfo.Name);
//		Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
//		Operation.Replace(_T("<SST_ID>"), SST_ID);
//
//
//		Operations.Append(Operation);
//		Operations.Append(_T("\r\n"));
//			
//
//		Index++;
//	}
//	CString LineSpace;
//	LineSpace.Format(_T("\r\n%s"), szLineSpace);
//	Operations.Replace(_T("\r\n"), LineSpace);
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
//				SelectBlock(Operation, _T("InBindData"), true);
//			else
//				SelectBlock(Operation, _T("InBindData"), false);
//			CString SubOperation = pType->GenerateOperations.PropertyGridFetchOperation;
//			CString LineSpace = GetLineSpace(Operation, _T("<PropertyGridFetchOperation>"));
//			SubOperation.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
//			RemoveBlock(SubOperation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//			RetainBlock(SubOperation, _T("<IfInArray>"), _T("</IfInArray>"));
//			CString Var = m_InterfaceConfig.ArrayDefineConfig.IndexOperation;
//			Var.Replace(_T("<Variable>"), MemberInfo.Name);
//			Var.Replace(_T("<Index>"), _T("i"));
//			SubOperation.Replace(_T("<Variable>"), Var);
//			SubOperation.Replace(_T("<Index>"), _T("i"));
//			Operation.Replace(_T("<PropertyGridFetchOperation>"), SubOperation);
//		}
//		else
//		{
//			Operation = pType->GenerateOperations.PropertyGridFetchOperation;
//			RemoveBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//			RetainBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//		}
//		if (!Operation.IsEmpty())
//		{
//			if (pType->Flag & TYPE_DEFINE_FLAG_ENUM)
//			{
//				ENUM_DEFINE_INFO* pEnum = GetEnumDefine(pType->CType);
//				if (pEnum)
//				{
//					CString MemberCount;
//					MemberCount.Format(_T("%d"), GetEnumMemberCount(*pEnum));
//					Operation.Replace(_T("<EnumMemberCount>"), MemberCount);
//					if (pEnum->Flag & ENUM_DEFINE_FLAG_IS_FLAG)
//					{
//						SelectBlock(Operation, _T("IsFlagEnum"), true);
//					}
//					else
//					{
//						SelectBlock(Operation, _T("IsFlagEnum"), false);
//					}
//					if (!MemberInfo.BindData.IsEmpty())
//					{
//						SelectBlock(Operation, _T("HaveBindData"), true);						
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
//							Operation.Replace(_T("<BindData>"), pBindData->Name);
//							TYPE_DEFINE* pBindDataType = FindVarType(pBindData->Type);
//							if (pBindDataType)
//								Operation.Replace(_T("<BindDataType>"), pBindDataType->CType);
//						}
//						else
//						{
//							Operation.Replace(_T("<BindData>"), _T("BindData"));
//							Operation.Replace(_T("<BindDataType>"), _T("T"));
//						}
//					}
//					else
//					{
//						SelectBlock(Operation, _T("HaveBindData"), false);
//					}
//				}
//			}		
//			if (ReplaceByBindData)
//			{
//				SelectBlock(Operation, _T("InBindData"), true);				
//			}
//			else
//			{
//				SelectBlock(Operation, _T("InBindData"), false);				
//				Operation.Replace(_T("<Variable>"), MemberInfo.Name);
//			}
//			Operation.Replace(_T("<Type>"), pType->CType);			
//			Operation.Replace(_T("<VarShowName>"), MakeStringConst(MemberInfo.ShowName));
//			Operation.Replace(_T("<Description>"), MakeStringConst(MemberInfo.Description));
//			Operation.Replace(_T("<VarStrValue>"), MakeToStringExp(MemberInfo, pType));
//			Operation.Replace(_T("<Index>"), _T("-1"));
//
//			
//		}
//	}
//	CString LineSpace;
//	LineSpace.Format(_T("\r\n%s"), szLineSpace);
//	Operation.Replace(_T("\r\n"), LineSpace);
//	return Operation;
//}
//
//CString CCallInterfaceMakerDlg::MakePropertyGridAddItemOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operations;
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(StructInfo.Name));
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
//				RemoveBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//				RetainBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//			}
//			else
//			{
//				RemoveBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//				RetainBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//			}
//			if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
//			{
//				RetainBlock(Operation, _T("<IfIsStruct>"), _T("</IfIsStruct>"));
//			}
//			else
//			{
//				RemoveBlock(Operation, _T("<IfIsStruct>"), _T("</IfIsStruct>"));
//			}
//			Operation.Replace(_T("<Type>"), pType->CType);
//			Operation.Replace(_T("<VarShowName>"), MakeStringConst(MemberInfo.ShowName));
//			Operation.Replace(_T("<Description>"), MakeStringConst(MemberInfo.Description));
//			Operation.Replace(_T("<VarStrValue>"), MakeToStringExp(MemberInfo, pType));
//			Operation.Replace(_T("<Index>"), _T("-1"));
//
//			if (ReplaceByBindData)
//			{
//				SelectBlock(Operation, _T("InBindData"), true);				
//			}
//			else
//			{
//				SelectBlock(Operation, _T("InBindData"), false);
//				Operation.Replace(_T("<Variable>"), MemberInfo.Name);
//				CString SST_ID;
//				SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
//				SST_ID.MakeUpper();
//				Operation.Replace(_T("<StructName>"), StructInfo.Name);
//				Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
//				Operation.Replace(_T("<SST_ID>"), SST_ID);
//			}
//
//			CString LineSpace = GetLineSpace(Operation, _T("<PropertyGridRefreshOperation>"));
//			CString RefreshOperation = MakePropertyGridFillOperation(StructInfo, MemberInfo, 0, SSTIDEnumName, ReplaceByBindData, LineSpace);
//			Operation.Replace(_T("<PropertyGridRefreshOperation>"), RefreshOperation);
//
//			Operations.Append(Operation);
//			Operations.Append(_T("\r\n"));
//		}
//		Index++;
//	}
//	CString LineSpace;
//	LineSpace.Format(_T("\r\n%s"), szLineSpace);
//	Operations.Replace(_T("\r\n"), LineSpace);
//	return Operations;
//}
//
//CString CCallInterfaceMakerDlg::MakePropertyGridDelItemOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operations;
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(StructInfo.Name));
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
//				RemoveBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//				RetainBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//			}
//			else
//			{
//				RemoveBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//				RetainBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//			}
//			if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
//			{
//				RetainBlock(Operation, _T("<IfIsStruct>"), _T("</IfIsStruct>"));
//			}
//			else
//			{
//				RemoveBlock(Operation, _T("<IfIsStruct>"), _T("</IfIsStruct>"));
//			}
//			Operation.Replace(_T("<Type>"), pType->CType);
//			Operation.Replace(_T("<VarShowName>"), MakeStringConst(MemberInfo.ShowName));
//			Operation.Replace(_T("<Description>"), MakeStringConst(MemberInfo.Description));
//			Operation.Replace(_T("<VarStrValue>"), MakeToStringExp(MemberInfo, pType));
//			Operation.Replace(_T("<Index>"), _T("-1"));
//
//			if (ReplaceByBindData)
//			{
//				SelectBlock(Operation, _T("InBindData"), true);				
//			}
//			else
//			{
//				SelectBlock(Operation, _T("InBindData"), false);
//				Operation.Replace(_T("<Variable>"), MemberInfo.Name);
//				CString SST_ID;
//				SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
//				SST_ID.MakeUpper();
//				Operation.Replace(_T("<StructName>"), StructInfo.Name);
//				Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
//				Operation.Replace(_T("<SST_ID>"), SST_ID);
//			}
//
//			CString LineSpace = GetLineSpace(Operation, _T("<PropertyGridRefreshOperation>"));
//			CString RefreshOperation = MakePropertyGridFillOperation(StructInfo, MemberInfo, 0, SSTIDEnumName, ReplaceByBindData, LineSpace);
//			Operation.Replace(_T("<PropertyGridRefreshOperation>"), RefreshOperation);
//
//			Operations.Append(Operation);
//			Operations.Append(_T("\r\n"));
//		}
//		Index++;
//	}
//	CString LineSpace;
//	LineSpace.Format(_T("\r\n%s"), szLineSpace);
//	Operations.Replace(_T("\r\n"), LineSpace);
//	return Operations;
//}
//
//CString CCallInterfaceMakerDlg::MakePropertyGridMoveItemOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace)
//{
//	CString Operations;
//	CString SSTIDEnumName;
//	SSTIDEnumName.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(StructInfo.Name));
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
//				RemoveBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//				RetainBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//			}
//			else
//			{
//				RemoveBlock(Operation, _T("<IfInArray>"), _T("</IfInArray>"));
//				RetainBlock(Operation, _T("<IfNotInArray>"), _T("</IfNotInArray>"));
//			}
//			if (pType->Flag & TYPE_DEFINE_FLAG_STRUCT)
//			{
//				RetainBlock(Operation, _T("<IfIsStruct>"), _T("</IfIsStruct>"));
//			}
//			else
//			{
//				RemoveBlock(Operation, _T("<IfIsStruct>"), _T("</IfIsStruct>"));
//			}
//			Operation.Replace(_T("<Type>"), pType->CType);
//			Operation.Replace(_T("<VarShowName>"), MakeStringConst(MemberInfo.ShowName));
//			Operation.Replace(_T("<Description>"), MakeStringConst(MemberInfo.Description));
//			Operation.Replace(_T("<VarStrValue>"), MakeToStringExp(MemberInfo, pType));
//			Operation.Replace(_T("<Index>"), _T("-1"));
//
//			if (ReplaceByBindData)
//			{
//				SelectBlock(Operation, _T("InBindData"), true);				
//			}
//			else
//			{
//				SelectBlock(Operation, _T("InBindData"), false);
//				Operation.Replace(_T("<Variable>"), MemberInfo.Name);
//				CString SST_ID;
//				SST_ID.Format(_T("SST_%s_%s"), StructInfo.ShortName, ClassNameToUpper(MemberInfo.Name));
//				SST_ID.MakeUpper();
//				Operation.Replace(_T("<StructName>"), StructInfo.Name);
//				Operation.Replace(_T("<SST_NAME>"), SSTIDEnumName);
//				Operation.Replace(_T("<SST_ID>"), SST_ID);
//			}
//
//			CString LineSpace = GetLineSpace(Operation, _T("<PropertyGridRefreshOperation>"));
//			CString RefreshOperation = MakePropertyGridFillOperation(StructInfo, MemberInfo, 0, SSTIDEnumName, ReplaceByBindData, LineSpace);
//			Operation.Replace(_T("<PropertyGridRefreshOperation>"), RefreshOperation);
//
//			Operations.Append(Operation);
//			Operations.Append(_T("\r\n"));
//		}
//		Index++;
//	}
//	CString LineSpace;
//	LineSpace.Format(_T("\r\n%s"), szLineSpace);
//	Operations.Replace(_T("\r\n"), LineSpace);
//	return Operations;
//}

CString CCallInterfaceMakerDlg::MakeStructXLSProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace)
{
	CString Source = m_StructXLSProcessTemplate;

	Source.Replace(_T("<ClassName>"), StructInfo.Name);

	if (StructInfo.BaseStruct.IsEmpty())
	{
		RemoveBlock(Source, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
		RetainBlock(Source, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
	}
	else
	{
		RemoveBlock(Source, _T("<IfNotHaveBaseClass>"), _T("</IfNotHaveBaseClass>"));
		RetainBlock(Source, _T("<IfHaveBaseClass>"), _T("</IfHaveBaseClass>"));
		Source.Replace(_T("<BaseClass>"), StructInfo.BaseStruct);
	}

	CString SSTIDEnumName;
	SSTIDEnumName.Format(_T("%s_MEMBER_IDS"), ClassNameToUpper(StructInfo.Name));

	CString Operations;
	CString LineSpace = GetLineSpace(Source, _T("<CreateXLSColumnOperations>"));
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
				Operation.Replace(_T("<VariableName>"), MemberInfo.Name);
				Operation.Replace(_T("<Type>"), pType->CType);
				Operation.Replace(_T("<OrginType>"), pType->Name);
				Operation.Replace(_T("<Description>"), MakeStringConst(MemberInfo.Description));
				if (!Operations.IsEmpty())
					Operations.Append(_T("\r\n"));
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
	Source.Replace(_T("<CreateXLSColumnOperations>"), Operations);

	Operations.Empty();
	LineSpace = GetLineSpace(Source, _T("<CheckXLSColumnOperations>"));
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
				Operation.Replace(_T("<VariableName>"), MemberInfo.Name);
				if (!Operations.IsEmpty())
					Operations.Append(_T("\r\n"));
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
	Source.Replace(_T("<CheckXLSColumnOperations>"), Operations);

	Operations.Empty();
	LineSpace = GetLineSpace(Source, _T("<ToXLSOperations>"));
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		if ((!MemberInfo.ShowName.IsEmpty()) && ((MemberInfo.Flag & STRUCT_MEMBER_FLAG_HIDE_IN_XLS) == 0))
		{
			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
			if (pType)
			{
				CString Operation;
				if (MemberInfo.IsArray)
					Operation = m_InterfaceConfig.ArrayDefineConfig.ToXLSOperation;
				else
					Operation = pType->GenerateOperations.ToXLSOperation;
				Operation.Replace(_T("<Type>"), pType->CType);
				Operation.Replace(_T("<OrginType>"), pType->Name);
				CString VarName = MemberInfo.Name;
				if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
					VarName = _T("m_") + VarName;
				VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
				Operation.Replace(_T("<Variable>"), VarName);
				if (!Operations.IsEmpty())
					Operations.Append(_T("\r\n"));
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
	Source.Replace(_T("<ToXLSOperations>"), Operations);

	Operations.Empty();
	LineSpace = GetLineSpace(Source, _T("<FromXLSOperations>"));
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
				Operation.Replace(_T("<Type>"), pType->CType);
				Operation.Replace(_T("<OrginType>"), pType->Name);
				CString VarName = MemberInfo.Name;
				if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
					VarName = _T("m_") + VarName;
				VarName = m_InterfaceConfig.MemberVariablePrefix + VarName;
				Operation.Replace(_T("<Variable>"), VarName);
				Operation.Replace(_T("<VariableName>"), MemberInfo.Name);
				if (!Operations.IsEmpty())
					Operations.Append(_T("\r\n"));
				Operations.Append(Operation);
			}
		}
	}
	Operations.Replace(_T("\r\n"), _T("\r\n") + LineSpace);
	Source.Replace(_T("<FromXLSOperations>"), Operations);


	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeStructJoin(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR Template, LPCTSTR Separator)
{
	CString Source;
	for (UINT i = 0;i < StructInfo.MemberList.size();i++)
	{
		STRUCT_MEMBER_INFO& MemberInfo = StructInfo.MemberList[i];
		CString Operation = Template;
		TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
		if (pType && (!pType->GenerateOperations.FileLogFillOperation.IsEmpty()))
		{
			Operation.Replace(_T("<Variable>"), MemberInfo.Name);
			Operation.Replace(_T("<VariableName>"), MemberInfo.Name);
			Operation.Replace(_T("<FormatSpec>"), pType->GenerateOperations.FormatSpecOperation);
			CString Fill = pType->GenerateOperations.FileLogFillOperation;
			Fill.Replace(_T("<Variable>"), MemberInfo.Name);
			Operation.Replace(_T("<FileLogFillOperation>"), Fill);

			if (i != 0)
				Source += Separator;
			Source += Operation;
		}
	}
	return Source;
}

CString CCallInterfaceMakerDlg::MakeStructDependImports(STRUCT_DEFINE_LIST2& StructList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	vector<DEPEND_INFO> DependTypeList;
	vector<CString> DataObjectList;
	vector<CString> PackFlagList;
	vector<CString> MethodList;

	for (STRUCT_DEFINE_INFO* pStructInfo : StructList.StructList)
	{
		if (!pStructInfo->BaseStruct.IsEmpty())
		{
			TYPE_DEFINE* pType = FindVarType(pStructInfo->BaseStruct);
			if (pType && (pType->Flag & TYPE_DEFINE_FLAG_STRUCT) && (!pType->SourceListName.IsEmpty()) && (pType->SourceListName != StructList.ListName))
			{
				if (pType->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
					AddUnique(DataObjectList, pType->CType);
				else
					AddDependType(DependTypeList, pType);
			}
		}
		for (STRUCT_MEMBER_INFO& MemberInfo : pStructInfo->MemberList)
		{
			TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
			if (pType && (pType->Flag & TYPE_DEFINE_FLAG_ENUM))
				int err = 1;
			if (pType && (!pType->SourceListName.IsEmpty()) && (pType->SourceListName != StructList.ListName || (pType->Flag & TYPE_DEFINE_FLAG_ENUM)))
			{
				if (pType->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
					AddUnique(DataObjectList, pType->CType);
				else
					AddDependType(DependTypeList, pType);
			}
			if (!MemberInfo.PackFlag.IsEmpty())
				AddUnique(PackFlagList, MemberInfo.PackFlag);
		}
	}
	return MakeStructDependImports(DependTypeList, DataObjectList, PackFlagList, MethodList, SpaceName, _T(""), szLineSpace);
}
CString CCallInterfaceMakerDlg::MakeStructDependImports(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	vector<DEPEND_INFO> DependTypeList;
	vector<CString> DataObjectList;
	vector<CString> PackFlagList;
	vector<CString> MethodList;

	if (!StructInfo.BaseStruct.IsEmpty())
	{
		TYPE_DEFINE* pType = FindVarType(StructInfo.BaseStruct);
		if (pType && (pType->Flag & TYPE_DEFINE_FLAG_STRUCT) && (!pType->SourceListName.IsEmpty()))
		{
			if (pType->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
				AddUnique(DataObjectList, pType->CType);
			else
				AddDependType(DependTypeList, pType);
		}
	}
	for (STRUCT_MEMBER_INFO& MemberInfo : StructInfo.MemberList)
	{
		TYPE_DEFINE* pType = FindVarType(MemberInfo.Type);
		if (pType && (!pType->SourceListName.IsEmpty()))
		{
			if (pType->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
				AddUnique(DataObjectList, pType->CType);
			else
				AddDependType(DependTypeList, pType);
		}
		if (!MemberInfo.PackFlag.IsEmpty())
			AddUnique(PackFlagList, MemberInfo.PackFlag);
	}

	if (StructInfo.Flag & STRUCT_FLAG_IS_DATA_OBJECT)
	{
		CString FullModifyFlag;
		FullModifyFlag.Format(_T("DOMF_%s_FULL"), ClassNameToUpper(StructInfo.Name));
		AddUnique(PackFlagList, FullModifyFlag);
	}

	return MakeStructDependImports(DependTypeList, DataObjectList, PackFlagList, MethodList, SpaceName, _T(""), szLineSpace);
}
CString CCallInterfaceMakerDlg::MakeStructDependImports(CALLER_INTERFACE& InterfaceInfo, LPCTSTR SpaceName, LPCTSTR InterfaceName, LPCTSTR szLineSpace, INTERFACE_DEPEND_MAKE_TYPE MakeType)
{
	vector<DEPEND_INFO> DependTypeList;
	vector<CString> DataObjectList;
	vector<CString> PackFlagList;
	vector<CString> MethodList;

	for (INTERFACE_METHOD& Method : InterfaceInfo.MethodList)
	{
		if ((Method.Type == INTERFACE_METHOD_TYPE_NOTIFY && (MakeType == INTERFACE_DEPEND_MAKE_ACK_DECLARE || MakeType == INTERFACE_DEPEND_MAKE_ACK_CALLER || MakeType == INTERFACE_DEPEND_MAKE_ACK_HANDLER)) ||
			(Method.Type != INTERFACE_METHOD_TYPE_NOTIFY && (MakeType == INTERFACE_DEPEND_MAKE_DECLARE || MakeType == INTERFACE_DEPEND_MAKE_CALLER || MakeType == INTERFACE_DEPEND_MAKE_HANDLER)))
		{
			for (METHOD_PARAM& Param : Method.CallParamList)
			{
				TYPE_DEFINE* pType = FindVarType(Param.Type);
				if (pType && (!pType->SourceListName.IsEmpty()))
				{
					if (pType->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
						AddUnique(DataObjectList, pType->CType);
					else
						AddDependType(DependTypeList, pType);
				}
				if ((MakeType != INTERFACE_DEPEND_MAKE_DECLARE) && (!Param.PackFlag.IsEmpty()))
					AddUnique(PackFlagList, Param.PackFlag);
			}
			if (MakeType != INTERFACE_DEPEND_MAKE_DECLARE && MakeType != INTERFACE_DEPEND_MAKE_ACK_DECLARE)
				AddUnique(MethodList, Method.Name);
		}
		if ((Method.Type == INTERFACE_METHOD_TYPE_CALL) && (MakeType == INTERFACE_DEPEND_MAKE_ACK_DECLARE || MakeType == INTERFACE_DEPEND_MAKE_ACK_CALLER || MakeType == INTERFACE_DEPEND_MAKE_ACK_HANDLER))
		{
			for (METHOD_PARAM& Param : Method.AckParamList)
			{
				TYPE_DEFINE* pType = FindVarType(Param.Type);
				if (pType && (!pType->SourceListName.IsEmpty()))
				{
					if (pType->Flag & TYPE_DEFINE_FLAG_DATA_OBJECT)
						AddUnique(DataObjectList, pType->CType);
					else
						AddDependType(DependTypeList, pType);
				}
				if ((MakeType != INTERFACE_DEPEND_MAKE_ACK_DECLARE) && (!Param.PackFlag.IsEmpty()))
					AddUnique(PackFlagList, Param.PackFlag);
			}
			if (MakeType != INTERFACE_DEPEND_MAKE_ACK_DECLARE)
				AddUnique(MethodList, Method.Name + _T("Ack"));
		}
	}

	return MakeStructDependImports(DependTypeList, DataObjectList, PackFlagList, MethodList, SpaceName, InterfaceName, szLineSpace);
}
CString CCallInterfaceMakerDlg::MakeStructDependImports(vector<DEPEND_INFO>& DependTypeList, vector<CString> DataObjectList, vector<CString> PackFlagList, vector<CString> MethodList, LPCTSTR SpaceName, LPCTSTR InterfaceName, LPCTSTR szLineSpace)
{
	CString Source;
	for (DEPEND_INFO& Info : DependTypeList)
	{
		CString Operation = m_DependImportTemplate;
		BASE_DATA_STRUCT_DEFINE_LIST* pDefineList = GetDataDefineList(Info.ListType, Info.ListName);
		if (pDefineList)
		{
			int BlockLen = 0;
			CString Separator, Content;
			int StartPos = FindJoinContent(Operation, _T("JoinWithDepends"), 0, BlockLen, Separator, Content);
			if (StartPos >= 0)
			{
				CString JoinContent;
				for (size_t i = 0;i < Info.DependTypes.size();i++)
				{
					const TYPE_DEFINE* pType = Info.DependTypes[i];
					if (i)
						JoinContent += Separator;
					CString Temp = Content;
					Temp.Replace(_T("<Type>"), pType->CType);
					JoinContent += Temp;
				}
				Operation.Delete(StartPos, BlockLen);
				Operation.Insert(StartPos, JoinContent);
			}
			CString SourceFileName;
			CString ModuleName = GetModuleName(pDefineList->ModuleID);
			if (pDefineList->ListType == DATA_STRUCT_STRUCT)
				SourceFileName.Format(_T("%s%sStructs"), ModuleName, pDefineList->ListName);
			else
				SourceFileName.Format(_T("%s%sEnums"), ModuleName, pDefineList->ListName);
			Operation.Replace(_T("<SourceFileName>"), SourceFileName);
			Source += Operation + _T("\r\n");
		}
	}

	for (CString& Name : DataObjectList)
	{
		CString Operation = m_DependImportTemplate;
		int BlockLen = 0;
		CString Separator, Content;
		int StartPos = FindJoinContent(Operation, _T("JoinWithDepends"), 0, BlockLen, Separator, Content);
		if (StartPos >= 0)
		{
			Operation.Delete(StartPos, BlockLen);
			Operation.Insert(StartPos, Name);
		}
		Operation.Replace(_T("<SourceFileName>"), ClassNameToFileName(Name));
		Source += Operation + _T("\r\n");
	}
	if (PackFlagList.size())
	{
		CString Operation = m_DependImportTemplate;
		int BlockLen = 0;
		CString Separator, Content;
		int StartPos = FindJoinContent(Operation, _T("JoinWithDepends"), 0, BlockLen, Separator, Content);
		if (StartPos >= 0)
		{
			CString JoinContent;
			for (size_t i = 0;i < PackFlagList.size();i++)
			{
				if (i)
					JoinContent += Separator;
				CString Temp = Content;
				Temp.Replace(_T("<Type>"), PackFlagList[i]);
				JoinContent += Temp;
			}
			Operation.Delete(StartPos, BlockLen);
			Operation.Insert(StartPos, JoinContent);
		}
		Operation.Replace(_T("<SourceFileName>"), _T("DataObjectModifyFlags"));
		Source += Operation + _T("\r\n");
	}

	if (MethodList.size())
	{
		CString Operation = m_DependImportTemplate;
		int BlockLen = 0;
		CString Separator, Content;
		int StartPos = FindJoinContent(Operation, _T("JoinWithDepends"), 0, BlockLen, Separator, Content);
		if (StartPos >= 0)
		{
			CString JoinContent;
			for (size_t i = 0;i < MethodList.size();i++)
			{
				if (i)
					JoinContent += Separator;
				CString Temp = Content;
				CString EnumName;
				EnumName.Format(_T("%s_%s_MEMBER_IDS"), ClassNameToUpper(InterfaceName), ClassNameToUpper(MethodList[i]));
				Temp.Replace(_T("<Type>"), EnumName);
				JoinContent += Temp;
			}
			Operation.Delete(StartPos, BlockLen);
			Operation.Insert(StartPos, JoinContent);
		}
		CString FileName;
		FileName.Format(_T("%sInterface"), InterfaceName);
		Operation.Replace(_T("<SourceFileName>"), FileName);
		Source += Operation + _T("\r\n");
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

CString CCallInterfaceMakerDlg::MakeAllDataObjectDependImports(vector<STRUCT_DEFINE_INFO*>& AllDataObjectList, LPCTSTR SpaceName, LPCTSTR szLineSpace)
{
	CString Source;

	for (STRUCT_DEFINE_INFO* pStruct : AllDataObjectList)
	{
		CString Operation = m_DependImportTemplate;
		int BlockLen = 0;
		CString Separator, Content;
		int StartPos = FindJoinContent(Operation, _T("JoinWithDepends"), 0, BlockLen, Separator, Content);
		if (StartPos >= 0)
		{
			CString ModifyFlagEnumName;
			ModifyFlagEnumName.Format(_T("%s_MODIFY_FLAGS"), ClassNameToUpper(pStruct->Name));

			Operation.Delete(StartPos, BlockLen);
			Operation.Insert(StartPos, ModifyFlagEnumName);
		}
		Operation.Replace(_T("<SourceFileName>"), ClassNameToFileName(pStruct->Name));
		Source += Operation + _T("\r\n");
	}

	CString Space = _T("\r\n");
	Space += szLineSpace;
	Source.Replace(_T("\r\n"), Space);
	return Source;
}

bool CCallInterfaceMakerDlg::HaveMemberByType(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR Type, int Depth)
{
	if (StructInfo.BaseStruct == Type)
		return true;
	Depth--;
	if (Depth <= 0)
	{
		//AfxMessageBox(_T("结构递归过多，可能有循环嵌套"));
		return false;
	}

	STRUCT_DEFINE_INFO* pStructInfo = NULL;
	for (size_t i = 0;i < StructInfo.MemberList.size();i++)
	{
		if (StructInfo.MemberList[i].Type == Type)
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

STRUCT_DEFINE_INFO* CCallInterfaceMakerDlg::GetStructDefineInfo(LPCTSTR Type)
{
	for (size_t i = 0; i < m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == DATA_STRUCT_STRUCT)
		{
			STRUCT_DEFINE_LIST* pStructList = (STRUCT_DEFINE_LIST*)m_AllDataStructList[i];
			for (size_t j = 0;j < pStructList->StructList.size();j++)
			{
				if (pStructList->StructList[j].Name == Type)
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
void CCallInterfaceMakerDlg::GetDataObjectDefineList(vector<STRUCT_DEFINE_INFO*>& DataObjectList)
{
	GetDataObjectListSorted(m_AllDataStructList, DataObjectList);
}

MODULE_DEFINE_INFO* CCallInterfaceMakerDlg::GetModuleInfo(UINT ModuleID)
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

MODULE_DEFINE_INFO* CCallInterfaceMakerDlg::GetModuleInfoByName(LPCTSTR szModuleName)
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
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(ModuleID);
	if (pModuleInfo)
		return pModuleInfo->Name;
	return _T("未知");
}

CALLER_INTERFACE* CCallInterfaceMakerDlg::AddInterfaceInfo(CALLER_INTERFACE& InterfaceInfo)
{
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(InterfaceInfo.ModuleID);
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
CALLER_INTERFACE* CCallInterfaceMakerDlg::GetInterfaceInfoByID(UINT ModuleID, UINT InterfaceID)
{
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(ModuleID);
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
CALLER_INTERFACE* CCallInterfaceMakerDlg::GetInterfaceInfoByName(LPCTSTR ModuleName, LPCTSTR InterfaceName)
{
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfoByName(ModuleName);
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
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(ModuleID);
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

BASE_DATA_STRUCT_DEFINE_LIST* CCallInterfaceMakerDlg::NewDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name)
{
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(ModuleID);
	if (pModuleInfo)
	{
		BASE_DATA_STRUCT_DEFINE_LIST* pDataStructList = NULL;
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
BASE_DATA_STRUCT_DEFINE_LIST* CCallInterfaceMakerDlg::GetDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name)
{
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(ModuleID);
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
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(ModuleID);
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

bool CCallInterfaceMakerDlg::DataStructListMoveUp(BASE_DATA_STRUCT_DEFINE_LIST* pList)
{
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(pList->ModuleID);
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
bool CCallInterfaceMakerDlg::DataStructListMoveDown(BASE_DATA_STRUCT_DEFINE_LIST* pList)
{
	MODULE_DEFINE_INFO* pModuleInfo = GetModuleInfo(pList->ModuleID);
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

bool CCallInterfaceMakerDlg::DataStructListMoveToModule(BASE_DATA_STRUCT_DEFINE_LIST* pList, MODULE_DEFINE_INFO* pModuleInfo)
{
	MODULE_DEFINE_INFO* pOldModuleInfo = GetModuleInfo(pList->ModuleID);
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
	if (_tcsicmp(m_MainModule.ModuleDefineFileName, szFileName) == 0)
	{
		AfxMessageBox(_T("无法添加主模块作为引用模块"));
		return false;
	}
	MODULE_DEFINE_INFO* pModuleIndo = LoadModule(szFileName);
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

void CCallInterfaceMakerDlg::OnNMDblclkInterfaceList(NMHDR* pNMHDR, LRESULT* pResult)
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
	Dlg.m_Config = m_InterfaceConfig;
	if (Dlg.DoModal() == IDOK)
	{
		m_InterfaceConfig = Dlg.m_Config;
		SaveConfigByJson();
	}
}

CString CCallInterfaceMakerDlg::GetLineSpace(CString& TempleStr, LPCTSTR szWord)
{
	CString Space;

	int Pos = TempleStr.Find(szWord);
	if (Pos >= 0)
	{
		Space = TempleStr.Left(Pos);
		int SpaceLen = 0;
		for (int i = Space.GetLength() - 1;i >= 0;i--)
		{
			if (Space[i] != ' ' && Space[i] != '	')
				break;
			SpaceLen++;
		}
		Space = Space.Right(SpaceLen);
	}
	return Space;
}
int CCallInterfaceMakerDlg::GetHeadLen(CString& TempleStr, LPCTSTR szWord)
{
	CString Head;
	int HeadLen = 0;

	int Pos = TempleStr.Find(szWord);
	if (Pos >= 0)
	{
		Head = TempleStr.Left(Pos);
		for (int i = Head.GetLength() - 1;i >= 0;i--)
		{
			if (Head[i] == '\r' || Head[i] == '\n')
				break;
			HeadLen++;
		}
	}
	return HeadLen;
}
CString CCallInterfaceMakerDlg::ToComment(CString& Content, LPCTSTR szLineSpace)
{
	CString ToComment = m_InterfaceConfig.CommentPrefix + Content;
	CString Space = _T("\r\n//");
	Space += szLineSpace;
	ToComment.Replace(_T("\r\n"), Space);
	return ToComment;
}

CString CCallInterfaceMakerDlg::ClassNameToFileName(LPCTSTR szClassName)
{
	CString Output = szClassName;
	Output.Trim();
	if (Output.GetLength() >= 2)
	{
		if (Output[0] == 'C' && IsBigChar(Output[1]))
			Output.Delete(0);
	}
	return Output;
}

CString CCallInterfaceMakerDlg::ClassNameToUpper(LPCTSTR szClassName)
{
	CString Output = ClassNameToFileName(szClassName);
	for (int i = Output.GetLength() - 1;i > 0;i--)
	{
		if (IsBigChar(Output[i]) && i && (!IsBigChar(Output[i - 1])) && (Output[i - 1] != '_'))
		{
			Output.Insert(i, '_');
		}
	}
	Output.MakeUpper();
	return Output;
}

bool CCallInterfaceMakerDlg::IsBigChar(TCHAR c)
{
	return c >= 'A' && c <= 'Z';
}


void CCallInterfaceMakerDlg::OnBnClickedButtonSelectAllInterface()
{
	// TODO: 在此添加控件通知处理程序代码

	for (int i = 0;i < m_lvInterfaceList.GetItemCount();i++)
	{
		m_lvInterfaceList.SetCheck(i, m_SelectAll);
	}
	m_SelectAll = !m_SelectAll;
}



bool CCallInterfaceMakerDlg::RemoveBlock(CString& Template, LPCTSTR szBlockStart, LPCTSTR szBlockEnd)
{
	int StartBlockLen = _tcslen(szBlockStart);
	int EndBlockLen = _tcslen(szBlockEnd);
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
		StartMark.Format(_T("<%s>"), szBlockName);
		RemoveMark(Template, StartMark);
		StartMark.Format(_T("<!%s>"), szBlockName);
		EndMark.Format(_T("</%s>"), szBlockName);
		RemoveBlock(Template, StartMark, EndMark);
	}
	else
	{
		StartMark.Format(_T("<%s>"), szBlockName);
		EndMark.Format(_T("<!%s>"), szBlockName);
		RemoveBlock(Template, StartMark, EndMark);
		EndMark.Format(_T("</%s>"), szBlockName);
		RemoveMark(Template, EndMark);
	}
}
void CCallInterfaceMakerDlg::RemoveMark(CString& Template, LPCTSTR szMark)
{
	int BlockSize = _tcslen(szMark);
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
		if (NeedCutHeader)
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
		EndPos += _tcslen(szBlockEnd);
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
		StartPos += _tcslen(szBlockStart);
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

CString CCallInterfaceMakerDlg::RestoreFileToTemplate(LPCTSTR szFileName, const CString& szTemplate)
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

CString CCallInterfaceMakerDlg::RestoreToTemplate(LPCTSTR szInput, const CString& szTemplate)
{
	CString Output = szInput;
	int BlockIndex = 1;
	while (true)
	{
		CString BlockStart;
		CString BlockEnd;
		BlockStart.Format(_T("<GenerateArea%dStart>"), BlockIndex);
		BlockEnd.Format(_T("<GenerateArea%dEnd>"), BlockIndex);
		int TemplateStartPos = szTemplate.Find(BlockStart);
		int TemplateEndPos = szTemplate.Find(BlockEnd);
		int InputStartPos = Output.Find(BlockStart);
		int InputEndPos = Output.Find(BlockEnd);
		if (TemplateStartPos >= 0 && TemplateEndPos >= 0 && InputStartPos >= 0 && InputEndPos >= 0)
		{
			CString TemplateBlock = szTemplate.Mid(TemplateStartPos, TemplateEndPos - TemplateStartPos + BlockEnd.GetLength());
			Output.Delete(InputStartPos, InputEndPos - InputStartPos + BlockEnd.GetLength());
			Output.Insert(InputStartPos, TemplateBlock);
		}
		else
		{
			break;
		}
		BlockIndex++;
	}
	return Output;
}

void CCallInterfaceMakerDlg::GetDataStructPacketList(int Type, vector<CString>& NameList)
{
	for (size_t i = 0; i < m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == Type)
			NameList.push_back(m_AllDataStructList[i]->ListName);
	}
}

bool CCallInterfaceMakerDlg::MoveDataStruct(int Type, LPCTSTR SourcePacket, LPCTSTR TargetPacket, LPCTSTR Name)
{
	BASE_DATA_STRUCT_DEFINE_LIST* pSourcePacket = NULL;
	BASE_DATA_STRUCT_DEFINE_LIST* pTargetPacket = NULL;
	for (size_t i = 0; i < m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == Type && m_AllDataStructList[i]->ListName == SourcePacket)
			pSourcePacket = m_AllDataStructList[i];
		if (m_AllDataStructList[i]->ListType == Type && m_AllDataStructList[i]->ListName == TargetPacket)
			pTargetPacket = m_AllDataStructList[i];
		if (pSourcePacket && pTargetPacket)
			break;
	}
	if (pSourcePacket && pTargetPacket)
	{
		switch (Type)
		{
		case DATA_STRUCT_CONST:
		{
			CONST_DEFINE_LIST* pSourceList = (CONST_DEFINE_LIST*)pSourcePacket;
			CONST_DEFINE_LIST* pTargetList = (CONST_DEFINE_LIST*)pTargetPacket;
			for (size_t i = 0;i < pSourceList->ConstList.size();i++)
			{
				if (pSourceList->ConstList[i].Name == Name)
				{
					pTargetList->ConstList.push_back(pSourceList->ConstList[i]);
					pSourceList->ConstList.erase(pSourceList->ConstList.begin() + i);
					return true;
				}
			}
		}
		break;
		case DATA_STRUCT_ENUM:
		{
			ENUM_DEFINE_LIST* pSourceList = (ENUM_DEFINE_LIST*)pSourcePacket;
			ENUM_DEFINE_LIST* pTargetList = (ENUM_DEFINE_LIST*)pTargetPacket;
			for (size_t i = 0;i < pSourceList->EnumList.size();i++)
			{
				if (pSourceList->EnumList[i].Name == Name)
				{
					pTargetList->EnumList.push_back(pSourceList->EnumList[i]);
					pSourceList->EnumList.erase(pSourceList->EnumList.begin() + i);
					return true;
				}
			}
		}
		break;
		case DATA_STRUCT_STRUCT:
		{
			STRUCT_DEFINE_LIST* pSourceList = (STRUCT_DEFINE_LIST*)pSourcePacket;
			STRUCT_DEFINE_LIST* pTargetList = (STRUCT_DEFINE_LIST*)pTargetPacket;
			for (size_t i = 0;i < pSourceList->StructList.size();i++)
			{
				if (pSourceList->StructList[i].Name == Name)
				{
					pTargetList->StructList.push_back(pSourceList->StructList[i]);
					pSourceList->StructList.erase(pSourceList->StructList.begin() + i);
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
	if (AfxMessageBox(_T("是否要重排所有的ID，这将使接口和旧版本不兼容？"), MB_YESNO) == IDYES)
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
	const CALLER_INTERFACE* pInterfaceInfo1 = (const CALLER_INTERFACE*)p1;
	const CALLER_INTERFACE* pInterfaceInfo2 = (const CALLER_INTERFACE*)p2;
	return _tcscmp(pInterfaceInfo1->Name, pInterfaceInfo2->Name);
}

int CCallInterfaceMakerDlg::InterfaceMethodComp(LPCVOID p1, LPCVOID p2)
{
	const INTERFACE_METHOD* pMethodInfo1 = (const INTERFACE_METHOD*)p1;
	const INTERFACE_METHOD* pMethodInfo2 = (const INTERFACE_METHOD*)p2;
	return _tcscmp(pMethodInfo1->Name, pMethodInfo2->Name);
}

int CCallInterfaceMakerDlg::StructComp(LPCVOID p1, LPCVOID p2)
{
	const STRUCT_DEFINE_INFO* pInfo1 = (const STRUCT_DEFINE_INFO*)p1;
	const STRUCT_DEFINE_INFO* pInfo2 = (const STRUCT_DEFINE_INFO*)p2;
	return _tcscmp(pInfo1->Name, pInfo2->Name);
}

int CCallInterfaceMakerDlg::EnumComp(LPCVOID p1, LPCVOID p2)
{
	const ENUM_DEFINE_INFO* pInfo1 = (const ENUM_DEFINE_INFO*)p1;
	const ENUM_DEFINE_INFO* pInfo2 = (const ENUM_DEFINE_INFO*)p2;
	return _tcscmp(pInfo1->Name, pInfo2->Name);
}

int CCallInterfaceMakerDlg::ConstComp(LPCVOID p1, LPCVOID p2)
{
	const CONST_DEFINE_INFO* pInfo1 = (const CONST_DEFINE_INFO*)p1;
	const CONST_DEFINE_INFO* pInfo2 = (const CONST_DEFINE_INFO*)p2;
	return _tcscmp(pInfo1->Name, pInfo2->Name);
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

	for (size_t i = 0; i < m_AllInterfaceList.size(); i++)
	{
		CALLER_INTERFACE* pInterfaceInfo = m_AllInterfaceList[i];
		qsort(pInterfaceInfo->MethodList.begin()._Ptr, pInterfaceInfo->MethodList.size(), sizeof(INTERFACE_METHOD), InterfaceMethodComp);
	}

	for (size_t i = 0; i < m_AllDataStructList.size(); i++)
	{
		switch (m_AllDataStructList[i]->ListType)
		{
		case DATA_STRUCT_CONST:
		{
			CONST_DEFINE_LIST* pConstList = (CONST_DEFINE_LIST*)m_AllDataStructList[i];
			qsort(pConstList->ConstList.begin()._Ptr, pConstList->ConstList.size(), sizeof(CONST_DEFINE_INFO), ConstComp);
		}
		break;
		case DATA_STRUCT_ENUM:
		{
			ENUM_DEFINE_LIST* pEnumList = (ENUM_DEFINE_LIST*)m_AllDataStructList[i];
			qsort(pEnumList->EnumList.begin()._Ptr, pEnumList->EnumList.size(), sizeof(ENUM_DEFINE_INFO), EnumComp);
		}
		break;
		case DATA_STRUCT_STRUCT:
		{
			STRUCT_DEFINE_LIST* pStructList = (STRUCT_DEFINE_LIST*)m_AllDataStructList[i];
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
			STRUCT_DEFINE_LIST* pStructList = (STRUCT_DEFINE_LIST*)ModuleInfo.DataStructDefineList[i];
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
	STRUCT_DEFINE_INFO* pStructInfo = GetStructDefineInfo(szBaseStructName);
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
	if (pElementType)
	{
		if (pElementType->Flag & TYPE_DEFINE_FLAG_BASE_TYPE)
		{
			RemoveBlock(Operation, _T("<IfIsNotBaseType>"), _T("</IfIsNotBaseType>"));
			Operation.Replace(_T("<IfIsBaseType>"), _T(""));
			Operation.Replace(_T("</IfIsBaseType>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfIsBaseType>"), _T("</IfIsBaseType>"));
			Operation.Replace(_T("<IfIsNotBaseType>"), _T(""));
			Operation.Replace(_T("</IfIsNotBaseType>"), _T(""));
		}

		if ((pElementType->Flag & TYPE_DEFINE_FLAG_VARIABLE_LEN) || (pElementType->Flag & TYPE_DEFINE_FLAG_STRUCT))
		{
			RemoveBlock(Operation, _T("<IfIsFixLenType>"), _T("</IfIsFixLenType>"));
			Operation.Replace(_T("<IfIsVariableLenType>"), _T(""));
			Operation.Replace(_T("</IfIsVariableLenType>"), _T(""));
		}
		else
		{
			RemoveBlock(Operation, _T("<IfIsVariableLenType>"), _T("</IfIsVariableLenType>"));
			Operation.Replace(_T("<IfIsFixLenType>"), _T(""));
			Operation.Replace(_T("</IfIsFixLenType>"), _T(""));
		}

		Operation.Replace(_T("<ElementType>"), pElementType->CType);
		Operation.Replace(_T("<Index>"), _T("i"));
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
	int Pos = InputStr.Find(_T("\r\n"), 0);
	while (Pos >= 0)
	{
		ReplacePosList.Add(Pos);
		Pos = InputStr.Find(_T("\r\n"), Pos + 2);
	}
	if (ReplacePosList.GetCount())
	{
		const TCHAR* pSrc = (LPCTSTR)InputStr;
		TCHAR* pBuffer = OutputStr.GetBuffer(InputStr.GetLength() + Space.GetLength() * ReplacePosList.GetCount() + 1);
		int OutPos = 0;
		int StartPos = 0;
		for (int Pos : ReplacePosList)
		{
			_tcsncpy(pBuffer + OutPos, pSrc + StartPos, 2 + Pos - StartPos);
			OutPos += 2 + Pos - StartPos;
			StartPos += 2 + Pos - StartPos;
			_tcsncpy(pBuffer + OutPos, Space, Space.GetLength());
			OutPos += Space.GetLength();
		}
		if (StartPos < InputStr.GetLength() - 1)
		{
			_tcsncpy(pBuffer + OutPos, pSrc + StartPos, InputStr.GetLength() - StartPos - 1);
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
CString CCallInterfaceMakerDlg::ParserEnumValue(CString Value, bool IsBitMask)
{
	Value.Trim();
	Value.Remove('(');
	Value.Remove(')');
	if (Value[0] == '0' && (Value[1] == 'x' || Value[1] == 'X'))
	{
		UINT64 NumValue = _tcstoi64((LPCTSTR)Value + 2, NULL, 16);
		CString Result;
		Result.Format(_T("%llu"), NumValue);
		return Result;
	}
	int Pos = Value.Find(_T("<<"));
	if (Pos >= 0)
	{
		UINT64 NumValue = _tcstoi64(Value.Left(Pos), NULL, 10);
		UINT64 Shift = _tcstoi64(Value.Right(Value.GetLength() - Pos - 2), NULL, 10);
		NumValue = NumValue << Shift;
		CString Result;
		Result.Format(_T("%llu"), NumValue);
		return Result;
	}
	if (IsBitMask)
	{
		UINT64 NumValue = _tcstoi64(Value, NULL, 10);
		NumValue = 1 << NumValue;
		CString Result;
		Result.Format(_T("%llu"), NumValue);
		return Result;
	}
	return Value;
}

int CCallInterfaceMakerDlg::FindJoinContent(CString& Template, LPCTSTR Type, int StartPos, int& BlockLen, CString& Separator, CString& Content)
{
	CString Target;
	Target.Format(_T("<%s"), Type);
	size_t TypeLen = _tcslen(Type);
	StartPos = Template.Find(Target, StartPos);
	if (StartPos >= 0)
	{
		int HeadEndPos = Template.Find(_T(">"), StartPos + TypeLen + 1);
		if (HeadEndPos >= 0)
		{
			Target.Format(_T("</%s>"), Type);
			int EndPos = Template.Find(Target, HeadEndPos + 1);
			if (EndPos >= 0 && EndPos > HeadEndPos + 1)
			{
				int SeparatorPos = Template.Find(_T("Separator("), StartPos + TypeLen + 1);
				if (SeparatorPos >= 0)
				{
					SeparatorPos += 10;
					int SeparatorEndPos = Template.Find(_T(")"), SeparatorPos);
					if (SeparatorEndPos >= 0 && SeparatorEndPos < HeadEndPos)
					{
						Separator = Template.Mid(SeparatorPos, SeparatorEndPos - SeparatorPos);
						Content = Template.Mid(HeadEndPos + 1, EndPos - HeadEndPos - 1);
						BlockLen = EndPos + TypeLen + 3 - StartPos;
						return StartPos;
					}
				}
			}
		}
		StartPos = -1;
	}
	return StartPos;
}

BASE_DATA_STRUCT_DEFINE_LIST* CCallInterfaceMakerDlg::GetDataDefineList(DATA_STRUCT_TYPE Type, LPCTSTR Name)
{
	for (size_t i = 0; i < m_AllDataStructList.size(); i++)
	{
		if (m_AllDataStructList[i]->ListType == Type && m_AllDataStructList[i]->ListName == Name)
		{
			return m_AllDataStructList[i];
		}
	}
	return NULL;
}