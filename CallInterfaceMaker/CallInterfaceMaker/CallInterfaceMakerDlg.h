/****************************************************************************/
/*                                                                          */
/*      文件名:    CallInterfaceMakerDlg.h                                  */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// CallInterfaceMakerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CCallInterfaceMakerDlg 对话框
class CCallInterfaceMakerDlg : public CDialog
{
	// 构造
protected:
	CListCtrl								m_lvInterfaceList;
	BOOL									m_UTF8Export;

	vector<TYPE_DEFINE>						m_BaseTypeList;

	MODULE_DEFINE_INFO						m_MainModule;
	vector<MODULE_DEFINE_INFO>				m_ImportModuleList;

	vector<MODULE_DEFINE_INFO*>			m_AllModuleList;
	vector<CALLER_INTERFACE*>				m_AllInterfaceList;
	vector<BASE_DATA_STRUCT_DEFINE_LIST*>	m_AllDataStructList;

	CStringBuilder							m_OneFileExportCache;

	INTERFACE_CONFIG						m_InterfaceConfig;
	RUN_ENV_INFO							m_RunEnvInfo;
	CString									m_TypeDefFileName;

	CString									m_AllDefinesTemplate;

	CString									m_InterfaceHeaderTemplate;
	CString									m_InterfaceTemplate;

	CString									m_ParamDefineTemple;
	CString									m_ParamTransTemple;
	CString									m_ParamDescriptionTemple;

	CString									m_PackSizeTemple;

	CString									m_MethodHeaderTemple;
	CString									m_MethodHeaderPureTailTemple;
	CString									m_MethodCallerSourceTemple;
	CString									m_MsgPackMethodHeaderTemple;
	CString									m_MsgPackMethodSourceTemple;
	CString									m_MsgHandlerMethodHeaderTemple;
	CString									m_MsgHandlerMethodSourceTemple;
	CString									m_DataObjectDefineHeaderTemple;
	CString									m_DataObjectDefineSourceTemple;

	CString									m_MsgMapInitTemple;

	CString									m_MsgCallerHeaderTemplate;
	CString									m_MsgCallerSourceTemplate;

	CString									m_MsgHandlerHeaderTemplate;
	CString									m_MsgHandlerSourceTemplate;

	CString									m_StructDefineHeaderTemplate;
	CString									m_EnumDefineHeaderTemplate;
	CString									m_SystemEnumDefineHeaderTemplate;
	CString									m_StructDefineTemplate;
	CString									m_StructMemberTemplate;
	CString									m_DataObjectMemberTemplate;
	CString									m_DataObjectModifyFlagsHeaderTemple;
	CString									m_DataObjectModifyFlagsSourceTemple;
	CString									m_DataObjectModifyFlagHeaderDefineTemple;
	CString									m_DataObjectModifyFlagDefineTemple;
	CString									m_DataObjectModifyFlagUnitTemple;
	CString									m_PackOperationUnitTemplate;
	CString									m_UnpackOperationUnitTemplate;

	CString									m_ToXMLOperationUnitTemplate;
	CString									m_FromXMLOperationUnitTemplate;
	CString									m_StructXMLProcessTemplate;

	CString									m_ToJsonOperationUnitTemplate;
	CString									m_FromJsonOperationUnitTemplate;
	CString									m_StructJsonProcessTemplate;
	CString									m_DataObjectJsonProcessTemplate;
	CString									m_DataObjectJsonProcessHeaderTemplate;

	CString									m_StructDBProcessTemplate;
	CString									m_DataObjectDBProcessTemplate;
	CString									m_DataObjectDBProcessHeaderTemplate;

	CString									m_DBFieldNameTemplate;
	CString									m_DBFieldDefineUnitTemplate;
	CString									m_DBKeyDefineUnitTemplate;
	CString									m_DBPrimaryKeyDefineUnitTemplate;
	CString									m_DBUniqueKeyDefineUnitTemplate;

	CString									m_EnumDefineTemplate;
	CString									m_EnumDefine64Template;
	CString									m_EnumMemberDefineTemplate;
	CString									m_EnumMemberDefine64Template;
	CString									m_EnumBitMaskMemberDefineTemplate;
	CString									m_EnumBitMaskMemberDefine64Template;
	CString									m_EnumStrValueUnitTemplate;
	CString									m_EnumListUnitTemplate;
	CString									m_EnumToStrUnitTemplate;
	CString									m_StrToEnumUnitTemplate;
	CString									m_EnumBindDataFillUnit;
	//CString									m_EnumBindDataFetchUnit;

	CString									m_ConstDefineTemplate;
	CString									m_MacroDefineTemplate;
	CString									m_ConstDefineHeaderTemplate;

	CString									m_StringConstDefineTemplate;

	CString									m_ImportDeclareTemplate;

	CString									m_DataObjectClearModifyFlagDefineTemplate;
	CString									m_DataObjectGetModifyFlagDefineTemplate;
	CString									m_DataObjectIsModifiedDefineTemplate;

	CString									m_StructEditorProcessTemplate;
	CString									m_CreateListHeaderUnitTemplate;
	CString									m_FillListItemUnitTemplate;
	//CString									m_PropertyGridFetchUnitTemplate;
	//CString									m_PropertyGridAddItemUnitTemplate;
	//CString									m_PropertyGridDelItemUnitTemplate;
	//CString									m_PropertyGridMoveItemUnitTemplate;

	CString									m_StructLogProcessTemplate;

	CString									m_StructLuaProcessTemplate;
	CString									m_DataObjectLuaProcessTemplate;
	CString									m_DataObjectLuaProcessHeaderTemplate;
	CString									m_ToLuaTableOperationUnitTemplate;
	CString									m_FromLuaTableOperationUnitTemplate;

	CString									m_StructXLSProcessTemplate;

	vector<TYPE_DEFINE>						m_CurVarTypeList;

	bool									m_SelectAll;
	bool									m_IsLoaded;
public:
	CCallInterfaceMakerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CCallInterfaceMakerDlg();

	// 对话框数据
	enum { IDD = IDD_CALLINTERFACEMAKER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:

	afx_msg void OnBnClickedButtonAddInterface();
	afx_msg void OnBnClickedButtonEditInterface();
	afx_msg void OnBnClickedButtonDelInterface();
	afx_msg void OnBnClickedButtonMoveInterface();
	//afx_msg void OnBnClickedButtonEditPrefix();
	//afx_msg void OnBnClickedButtonEditPostfix();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonSaveAs();
	afx_msg void OnBnClickedButtonSaveExport();
	afx_msg void OnNMDblclkInterfaceList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonSelectAllInterface();
	afx_msg void OnBnClickedButtonDataStructDef();
	afx_msg void OnBnClickedButtonArrangeIds();
	afx_msg void OnBnClickedButtonSort();
	afx_msg void OnBnClickedButtonSortAll();
	afx_msg void OnBnClickedButtonImportEdit();


	void AddListItem(CALLER_INTERFACE& InterfaceInfo);
	void EditListItem(int Item, CALLER_INTERFACE& InterfaceInfo);
	bool IsListItemChecked(UINT ModuleID, UINT InterfaceID);
	void FillListItem();
	bool LoadTypeDef();
	bool LoadTemple(LPCTSTR FileName, CString& TemplateStr);
	bool LoadTemples();
	bool LoadConfig();
	bool LoadEnv();
	bool SaveEnv();
public:
	bool SaveTypeDef();
	bool SaveConfig();
	void RefreshDataCache();
	void RefreshInterfaceCache();
	void RefreshDataStructCache();
	void RefreshModuleCache();
	void MakeCurVarType(bool IncludeDataObject);
	UINT GetVarTypeCount();
	TYPE_DEFINE* GetVarType(UINT Index);
	TYPE_DEFINE* FindVarType(LPCTSTR TypeName);
	ENUM_DEFINE_INFO* GetEnumDefine(LPCTSTR EnumName);
	void GetDataObjectDefineList(vector<STRUCT_DEFINE_INFO*>& DataObjectList);

	MODULE_DEFINE_INFO* GetModuleInfo(UINT ModuleID);
	MODULE_DEFINE_INFO* GetModuleInfoByName(LPCTSTR szModuleName);
	LPCTSTR GetModuleName(UINT ModuleID);

	CALLER_INTERFACE* AddInterfaceInfo(CALLER_INTERFACE& InterfaceInfo);
	CALLER_INTERFACE* GetInterfaceInfoByID(UINT ModuleID, UINT InterfaceID);
	CALLER_INTERFACE* GetInterfaceInfoByName(LPCTSTR ModuleName, LPCTSTR InterfaceName);
	bool DeleteInterfaceInfoByID(UINT ModuleID, UINT InterfaceID);

	BASE_DATA_STRUCT_DEFINE_LIST* NewDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name);
	BASE_DATA_STRUCT_DEFINE_LIST* GetDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name);
	bool DeleteDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name);
	bool DataStructListMoveUp(BASE_DATA_STRUCT_DEFINE_LIST* pList);
	bool DataStructListMoveDown(BASE_DATA_STRUCT_DEFINE_LIST* pList);
	bool DataStructListMoveToModule(BASE_DATA_STRUCT_DEFINE_LIST* pList, MODULE_DEFINE_INFO* pModuleInfo);

	vector<MODULE_DEFINE_INFO*>& GetModuleList()
	{
		return m_AllModuleList;
	}

	vector<MODULE_DEFINE_INFO>& GetImportModuleList()
	{
		return m_ImportModuleList;
	}

	UINT GetMainModuleID()
	{
		return m_MainModule.ModuleID;
	}

	bool AddModule(LPCTSTR szFileName);
	bool DeleteModule(UINT ModuleID);

	vector<CALLER_INTERFACE*>& GetInterfaceList()
	{
		return m_AllInterfaceList;
	}


	INTERFACE_CONFIG& GetConfig()
	{
		return m_InterfaceConfig;
	}
	RUN_ENV_INFO& GetRunEnv()
	{
		return m_RunEnvInfo;
	}
	vector<TYPE_DEFINE>& GetBaseTypeList()
	{
		return m_BaseTypeList;
	}

	void GetDataStructPacketList(int Type, vector<CString>& NameList);

	bool MoveDataStruct(int Type, LPCTSTR SourcePacket, LPCTSTR TargetPacket, LPCTSTR Name);

	bool Save(MODULE_DEFINE_INFO& ModuleInfo);
	bool Load(LPCTSTR FileName, MODULE_DEFINE_INFO& ModuleInfo);
	//bool LoadOld(LPCTSTR FileName, INTERFANCE_DEFINES_OLD& ModuleInfo);
	//bool LoadOverVersion(LPCTSTR FileName, MODULE_DEFINE_INFO& ModuleInfo);

protected:
	virtual void OnCancel();
	virtual void OnOK();


	UINT GetFileVersion(LPCTSTR FileName);

	MODULE_DEFINE_INFO* LoadModule(LPCTSTR szFileName);

	bool SaveInterfaces(xml_node& Interfaces, vector<CALLER_INTERFACE>& InterfaceList);
	bool LoadInterfaces(xml_node& Interfaces, vector<CALLER_INTERFACE>& InterfaceList, UINT ModuleID);
	//bool LoadInterfacesOld(xml_node& Interfaces, vector<CALLER_INTERFACE_OLD>& InterfaceList);



	bool SaveStructDefine(xml_node& Structs, vector<STRUCT_DEFINE_INFO>& StructDefineList);
	bool LoadStructDefine(xml_node& Structs, vector<STRUCT_DEFINE_INFO>& StructDefineList);
	bool LoadStructDefineOld(xml_node& Structs, vector<STRUCT_DEFINE_INFO_OLD>& StructDefineList);

	bool SaveEnumDefine(xml_node& Enums, vector<ENUM_DEFINE_INFO>& EnumDefineList);
	bool LoadEnumDefine(xml_node& Enums, vector<ENUM_DEFINE_INFO>& EnumDefineList);

	bool SaveConstDefine(xml_node& Consts, vector<CONST_DEFINE_INFO>& ConstDefineList);
	bool LoadConstDefine(xml_node& Consts, vector<CONST_DEFINE_INFO>& ConstDefineList);

	bool SaveDataObjectModifyFlag(xml_node& ModifyFlags, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags);
	bool LoadDataObjectModifyFlag(xml_node& ModifyFlags, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags);

	bool SaveGenerateOperations(xml_node& XmlNode, GENERATE_OPERATIONS_INFO& GenerateOperations);
	bool LoadGenerateOperations(xml_node& XmlNode, GENERATE_OPERATIONS_INFO& GenerateOperations);

	//CString EncodeString(CString Str);
	//CString DecodeString(CString Str);

	bool ConvertOldInterfaceDefines(INTERFANCE_DEFINES_OLD& OldModuleInfo, MODULE_DEFINE_INFO& ModuleInfo);
	INTERFACE_METHOD_OLD* FindInterfaceMethod(INTERFANCE_DEFINES_OLD& OldInterfaceDefines, CString InterfaceName, LPCTSTR MethodName, bool FindAck);
	CALLER_INTERFACE_OLD* FindInterface(INTERFANCE_DEFINES_OLD& OldInterfaceDefines, CString InterfaceName, bool FindAck);
	INTERFACE_METHOD_OLD* FindMethod(vector<INTERFACE_METHOD_OLD>& OldMethodList, CString MethodName, bool FindAck);

	bool Export(LPCTSTR ExportDir);
	bool ExportDataStructDefineHeader(vector<CONST_DEFINE_LIST*>& ConstList, vector<ENUM_DEFINE_LIST*>& EnumList, vector<STRUCT_DEFINE_LIST2*>& StructList,
		LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName);
	bool ExportInterfaceHeader(vector<CALLER_INTERFACE*>& InterfaceList, LPCTSTR ExportDir, LPCTSTR FileName);
	bool ExportDataStructDefines(vector<CONST_DEFINE_LIST*>& ConstList, vector<ENUM_DEFINE_LIST*>& EnumList, vector<STRUCT_DEFINE_LIST2*>& StructList,
		LPCTSTR ExportDir, LPCTSTR SpaceName);
	bool ExportDataObject(vector<STRUCT_DEFINE_LIST2*>& StructList, LPCTSTR ExportDir, LPCTSTR SpaceName);
	bool ExportInterfaces(vector<CALLER_INTERFACE*>& InterfaceList, LPCTSTR ExportDir, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName);

	bool ExportStructs(LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName, vector<STRUCT_DEFINE_INFO*>& StructList);
	bool ExportSystemEnums(LPCTSTR ExportDir, LPCTSTR FileName, vector<MODULE_DEFINE_INFO*>& AllModuleList);
	bool ExportEnums(LPCTSTR ExportDir, LPCTSTR FileName, vector<ENUM_DEFINE_INFO>& EnumList, LPCTSTR SpaceName, LPCTSTR ListName, bool IsSystem);
	bool ExportConsts(LPCTSTR ExportDir, LPCTSTR FileName, vector<CONST_DEFINE_INFO>& ConstList, LPCTSTR SpaceName, LPCTSTR ListName);

	bool ExportDataObjectModifyFlags(vector<DATA_OBJECT_MODIFY_FLAG> DataObjectModifyFlagList, LPCTSTR ExportDir, LPCTSTR SpaceName);



	bool ExportMsgCaller(LPCTSTR ExportDir, LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType);
	bool ExportMsgHandler(LPCTSTR ExportDir, LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType);


	CString MakeMethods(CALLER_INTERFACE& InterfaceInfo, LPCTSTR SpaceName, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace, bool IsPure);
	CString MakeMethodSSTIDEnumDefines(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeStructSSTIDEnumDefines(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeMsgPackMethods(vector<INTERFACE_METHOD>& MethodList, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);
	CString MakeParams(vector<METHOD_PARAM>& ParamList, bool IsDefine, bool IncludeDefaultValue);
	CString MakeParamDescriptions(vector<METHOD_PARAM>& ParamList, LPCTSTR szLineSpace);
	CString MakeParamDefines(vector<METHOD_PARAM>& ParamList, LPCTSTR SpaceName, LPCTSTR szSplitChar, LPCTSTR szLineSpace);

	CString MakeMethodsSource(CALLER_INTERFACE& InterfaceInfo, LPCTSTR SpaceName, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);
	CString MakeMsgPackMethodsSource(LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);

	CString MakeInitOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);
	CString MakePackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace);
	CString MakeUnpackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace);
	CString MakePackSizes(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);

	CString MakeInitOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace);
	CString MakePackOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace);
	CString MakeUnpackOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR PacketName, LPCTSTR PacketMemberName, LPCTSTR szLineSpace);
	CString MakePackSizes(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	CString MakeXMLProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeToXMLOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeFromXMLOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	CString MakeJsonProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDataObjectJsonProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeToJsonOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeFromJsonOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	CString MakePackOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, LPCTSTR PacketName, LPCTSTR PacketMemberName, UINT OperationFlag, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeUnpackOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, LPCTSTR PacketName, LPCTSTR PacketMemberName, UINT OperationFlag, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeToXMLOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeFromXMLOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeToJsonOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeFromJsonOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, LPCTSTR PackFlag, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace);


	CString MakeDBProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDataObjectDBProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDBFieldDefineOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDBIndexDefineOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDBPutParamBindOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDBPutOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	CString MakeDBFields(STRUCT_DEFINE_INFO& StructInfo, BYTE DBIndexType);
	CString MakeDBFieldDefineOperation(LPCTSTR Type, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, bool IsInDataObject, bool IsArray, LPCTSTR DBLength, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDBFieldFormats(STRUCT_DEFINE_INFO& StructInfo);

	CString MakeLogProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeLogDataList(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace);

	CString MakeLuaProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDataObjectLuaProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeToLuaTableOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeToLuaTableOperation(TYPE_DEFINE* pTypeInfo, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, bool IsInDataObject, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeFromLuaTableOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SSTIDEnumName, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeFromLuaTableOperation(TYPE_DEFINE* pTypeInfo, LPCTSTR Name, LPCTSTR ParentName, LPCTSTR ParentShortName, LPCTSTR SSTIDEnumName, bool IsInDataObject, bool IsArray, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	CString MakeMsgHandlerMethods(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);
	CString MakeMsgHandlerMethodsSource(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	CString MakeMsgMapInits(CALLER_INTERFACE& InterfaceInfo, LPCTSTR SpaceName, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);

	CString MakeStructDefines(vector<STRUCT_DEFINE_INFO*>& StructDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeStructMembers(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SplitChar, bool AllMemberAddSplitChar, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	CString MakeCloneOperations(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLeftValue, LPCTSTR szRightValue, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeCloneOperation(LPCTSTR VariableName, TYPE_DEFINE* pTypeInfo, bool IsArray, bool InDataObject, bool BeCheckMF, bool BeUpdateMF, LPCTSTR PackFlag,
		LPCTSTR szLeftValue, LPCTSTR szRightValue, LPCTSTR ClassName, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	bool MakeDataObjectFlagIndicesEnum(vector<BASE_DATA_STRUCT_DEFINE_LIST*>& AllDataStructList, ENUM_DEFINE_INFO& EnumInfo);
	bool MakeDataObjectSSTIDsEnum(vector<BASE_DATA_STRUCT_DEFINE_LIST*>& DataStructList, ENUM_DEFINE_INFO& EnumInfo);
	CString MakeDataObjectModifyFlagEnumDefine(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDataObjectMembersGetModifyFlag(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace);
	CString MakeDataDataObjectMembersIsModified(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace);
	CString MakeDataDataObjectMembersClearModifyFlag(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace);

	CString MakeDataObjectSetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace);
	CString MakeDataObjectSetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDataObjectGetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace);
	CString MakeDataObjectGetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	CString MakeDataObjectModifyFlagsHeader(vector<BASE_DATA_STRUCT_DEFINE_LIST*>& AllDataStructList, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDataObjectModifyFlagsSource(vector<BASE_DATA_STRUCT_DEFINE_LIST*>& AllDataStructList, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeDataObjectModifyFlagData(DATA_OBJECT_MODIFY_FLAG& ModifyFlag, LPCTSTR SpaceName, LPCTSTR szLineSpace);


	CString MakeEnumDefine(ENUM_DEFINE_INFO& EnumInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeEnumDefines(vector<ENUM_DEFINE_INFO>& EnumDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	//CString MakeEnumStrValue(ENUM_DEFINE_INFO& EnumInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	//CString MakeEnumStrValues(vector<ENUM_DEFINE_INFO>& EnumDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	//CString MakeEnumStrTransFn(ENUM_DEFINE_INFO& EnumInfo, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	//CString MakeEnumStrTransFns(vector<ENUM_DEFINE_INFO>& EnumDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace);

	CString MakeConstDefines(vector<CONST_DEFINE_INFO>& ConstDefineList, LPCTSTR SpaceName, LPCTSTR szLineSpace);
	CString MakeStringConst(LPCTSTR StrValue);
	CString MakeToStringExp(STRUCT_MEMBER_INFO& MemberInfo, TYPE_DEFINE* pType, bool IsInDataObject);


	DATA_OBJECT_MODIFY_FLAG MakeDataObjectFullModifyFlag(STRUCT_DEFINE_INFO& StructInfo, vector<STRUCT_DEFINE_INFO*>& DataObjectList);


	CString MakeStructEditorProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace);
	CString MakePropertyGridFillOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace);
	CString MakePropertyGridFillOperation(STRUCT_DEFINE_INFO& StructInfo, STRUCT_MEMBER_INFO& MemberInfo, UINT MemberIndex, LPCTSTR SSTIDEnumName, bool ReplaceByBindData, LPCTSTR szLineSpace);
	//CString MakePropertyGridFetchOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace);
	//CString MakePropertyGridFetchOperation(STRUCT_DEFINE_INFO& StructInfo, STRUCT_MEMBER_INFO& MemberInfo, UINT MemberIndex, bool ReplaceByBindData, LPCTSTR szLineSpace);
	//CString MakePropertyGridAddItemOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace);
	//CString MakePropertyGridDelItemOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace);
	//CString MakePropertyGridMoveItemOperations(STRUCT_DEFINE_INFO& StructInfo, bool ReplaceByBindData, LPCTSTR szLineSpace);


	CString MakeStructXLSProcess(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR szLineSpace);

	bool HaveMemberByType(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR Type, int Depth = 32);
	STRUCT_DEFINE_INFO* GetStructDefineInfo(LPCTSTR Type);


	CString GetLineSpace(CString& TempleStr, LPCTSTR szWord);
	int GetHeadLen(CString& TempleStr, LPCTSTR szWord);
	CString ToComment(CString& Content, LPCTSTR szLineSpace);

	CString ClassNameToFileName(LPCTSTR szClassName);
	CString ClassNameToUpper(LPCTSTR szClassName);

	bool IsBigChar(TCHAR c);

	bool RemoveBlock(CString& Template, LPCTSTR szBlockStart, LPCTSTR szBlockEnd);
	void RetainBlock(CString& Template, LPCTSTR szBlockStart, LPCTSTR szBlockEnd);
	void SelectBlock(CString& Template, LPCTSTR szBlockName, bool SelectWitch);
	void RemoveMark(CString& Template, LPCTSTR szMark);
	void ReplaceBlock(CString& Template, LPCTSTR szBlockStart, LPCTSTR szBlockEnd, LPCTSTR ReplaceStr);
	CString GetBlock(const CString& Template, LPCTSTR szBlockStart, LPCTSTR szBlockEnd);
	int IsEmptyPrefix(CString& Template, int Pos);
	int IsEmptySuffix(CString& Template, int Pos);

	CString RestoreFileToTemplate(LPCTSTR szFileName, const CString& szTemplate);
	CString RestoreToTemplate(LPCTSTR szInput, const CString& szTemplate);

	void WriteStringToFile(LPCTSTR OutputFileName, CString& Str);

	bool EndWith(CString& Str, CString EndStr);

	void SortStructForExport(vector<STRUCT_DEFINE_INFO>& SrcStructList, vector<STRUCT_DEFINE_INFO*>& DestStructList);
	void GetStructDepends(const vector<STRUCT_DEFINE_INFO>& StructList, const STRUCT_DEFINE_INFO& StructInfo, vector<CString>& DependList, int Depth);
	void AddUnique(vector<STRUCT_DEFINE_INFO*>& StructList, STRUCT_DEFINE_INFO* pStructInfo);

	void SortDefinesForExport(vector<BASE_DATA_STRUCT_DEFINE_LIST*>& SrcList,
		vector<CONST_DEFINE_LIST*>& ConstList, vector<ENUM_DEFINE_LIST*>& EnumList, vector<STRUCT_DEFINE_LIST2*>& StructList);
	void GetDefineDepends(const vector<BASE_DATA_STRUCT_DEFINE_LIST*>& DefineList, const STRUCT_DEFINE_LIST* pStructDefine, vector<CString>& DependList, int Depth);
	void GetStructForeignDepends(const vector<STRUCT_DEFINE_INFO>& StructList, vector<CString>& DependList);
	void AddUnique(vector<CString>& NameList, const CString& Name);
	template<typename T>
	void AddUnique(vector<T*>& DefineList, BASE_DATA_STRUCT_DEFINE_LIST* pDefine);
	bool HaveStructs(const STRUCT_DEFINE_LIST* pStructDefine, vector<CString>& StructNameList);
	CString MakeDefineName(const STRUCT_DEFINE_LIST* pStructDefine);

	void SortModuleForExport(const vector<MODULE_DEFINE_INFO*>& SrcList, vector<MODULE_DEFINE_INFO*>& DestList);
	void GetModuleDepends(const vector<MODULE_DEFINE_INFO*>& ModuleList, const MODULE_DEFINE_INFO* pModuleInfo, vector<CString>& DependList, int Depth);
	void AddUnique(vector<MODULE_DEFINE_INFO*>& ModuleList, MODULE_DEFINE_INFO* pModuleInfo);
	bool HaveDefines(const MODULE_DEFINE_INFO* pModuleInfo, vector<CString>& DefineNameList);

	void SaveInterfaceSelections();
	void RestoreInterfaceSelections();

	void FetchInterfaceToCache(MODULE_DEFINE_INFO& InterfaceDefines);
	void FetchDataStructToCache(MODULE_DEFINE_INFO& InterfaceDefines);

	void ArrangeIds(MODULE_DEFINE_INFO& ModuleInfo);
	bool CheckBaseStructObjectID(vector<UINT>& ObjectIDList, LPCTSTR szBaseStructName);
	CString ProcessArrayOperation(CString Operation, TYPE_DEFINE* pElementType);
	bool CheckNameChange(CString& Operation, CString& Name, LPCTSTR szChangeToken);
	void IncSpace(const CEasyString& InputStr, CString& OutputStr, const CString& Space);
	bool IsNaturalEnum(ENUM_DEFINE_INFO& EnumInfo);
	UINT GetEnumMemberCount(ENUM_DEFINE_INFO& EnumInfo);
	UINT GetStructMemberCount(STRUCT_DEFINE_INFO& StructInfo);
	STRUCT_MEMBER_INFO* IsBeBind(STRUCT_DEFINE_INFO& StructInfo, LPCTSTR MemberName);
	STRUCT_MEMBER_INFO* GetStructKeyMember(LPCTSTR StructName);
	CString ParserEnumValue(CString Value, bool IsBitMask);
public:
	void GetDataObjectListSorted(vector<BASE_DATA_STRUCT_DEFINE_LIST*>& DataStructList, vector<STRUCT_DEFINE_INFO*>& DataObjectList);
	static int InterfaceInfoComp(LPCVOID p1, LPCVOID p2);
	static int InterfaceMethodComp(LPCVOID p1, LPCVOID p2);
	static int StructComp(LPCVOID p1, LPCVOID p2);
	static int EnumComp(LPCVOID p1, LPCVOID p2);
	static int ConstComp(LPCVOID p1, LPCVOID p2);
	static int DataObjectComp(LPCVOID p1, LPCVOID p2);
	static int ModifyFlagComp(LPCVOID p1, LPCVOID p2);




};


inline UINT CCallInterfaceMakerDlg::GetVarTypeCount()
{
	return m_CurVarTypeList.size();
}
inline TYPE_DEFINE* CCallInterfaceMakerDlg::GetVarType(UINT Index)
{
	if (Index < m_CurVarTypeList.size())
	{
		return &m_CurVarTypeList.at(Index);
	}
	return NULL;
}

inline TYPE_DEFINE* CCallInterfaceMakerDlg::FindVarType(LPCTSTR TypeName)
{
	for (UINT i = 0;i < m_CurVarTypeList.size();i++)
	{
		if (m_CurVarTypeList[i].Name == TypeName)
		{
			return &m_CurVarTypeList[i];
		}
	}
	return NULL;
}
template<typename T>
inline void CCallInterfaceMakerDlg::AddUnique(vector<T*>& DefineList, BASE_DATA_STRUCT_DEFINE_LIST* pDefine)
{
	for (size_t i = 0; i < DefineList.size(); i++)
	{
		if (DefineList[i]->ListType == pDefine->ListType && DefineList[i] == pDefine)
			return;
	}
	T* pObj = dynamic_cast<T*>(pDefine);
	if (pObj)
		DefineList.push_back(pObj);
}