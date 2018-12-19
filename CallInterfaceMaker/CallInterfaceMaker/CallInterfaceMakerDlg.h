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

	vector<MODULE_DEFINE_INFO *>			m_AllModuleList;
	vector<CALLER_INTERFACE *>				m_AllInterfaceList;
	vector<BASE_DATA_STRUCT_DEFINE_LIST *>	m_AllDataStructList;
	
	
	INTERFACE_CONFIG						m_InterfaceConfig;
	RUN_ENV_INFO							m_RunEnvInfo;
	CString									m_TypeDefFileName;

	CString									m_InterfaceHeaderTemplate;
	CString									m_InterfaceTemplate;

	CString									m_ParamTemple;
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
	CString									m_DataObjectModifyFlagsTemple;
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
	CString									m_DataObjectStructJsonProcessHeaderTemplate;


	CString									m_EnumDefineTemplate;
	CString									m_EnumMemberDefineTemplate;
	CString									m_EnumStrValueDefineTemplate;

	CString									m_ConstDefineTemplate;
	CString									m_MacroDefineTemplate;
	CString									m_ConstDefineHeaderTemplate;

	CString									m_StringConstDefineTemplate;

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
	afx_msg void OnNMDblclkInterfaceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonSelectAllInterface();
	afx_msg void OnBnClickedButtonDataStructDef();
	afx_msg void OnBnClickedButtonArrangeIds();
	afx_msg void OnBnClickedButtonSort();
	afx_msg void OnBnClickedButtonSortAll();
	afx_msg void OnBnClickedButtonImportEdit();
	

	void AddListItem(CALLER_INTERFACE& InterfaceInfo);
	void EditListItem(int Item,CALLER_INTERFACE& InterfaceInfo);
	bool IsListItemChecked(UINT ModuleID, UINT InterfaceID);
	void FillListItem();
	bool LoadTypeDef();	
	bool LoadTemples();
	bool LoadConfig();
	bool SaveConfig();
	bool LoadEnv();
	bool SaveEnv();
public:
	bool SaveTypeDef();
	void RefreshDataCache();
	void RefreshInterfaceCache();
	void RefreshDataStructCache();
	void RefreshModuleCache();
	void MakeCurVarType(bool IncludeDataObject);
	UINT GetVarTypeCount();
	TYPE_DEFINE * GetVarType(UINT Index);
	TYPE_DEFINE * FindVarType(LPCTSTR TypeName);
	void GetDataObjectDefineList(vector<STRUCT_DEFINE_INFO *>& DataObjectList);

	MODULE_DEFINE_INFO * GetModuleInfo(UINT ModuleID);
	MODULE_DEFINE_INFO * GetModuleInfoByName(LPCTSTR szModuleName);
	LPCTSTR GetModuleName(UINT ModuleID);

	CALLER_INTERFACE * AddInterfaceInfo(CALLER_INTERFACE& InterfaceInfo);
	CALLER_INTERFACE * GetInterfaceInfoByID(UINT ModuleID, UINT InterfaceID);
	CALLER_INTERFACE * GetInterfaceInfoByName(LPCTSTR ModuleName, LPCTSTR InterfaceName);
	bool DeleteInterfaceInfoByID(UINT ModuleID, UINT InterfaceID);

	BASE_DATA_STRUCT_DEFINE_LIST * NewDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name);
	BASE_DATA_STRUCT_DEFINE_LIST * GetDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name);
	bool DeleteDataStructList(UINT ModuleID, DATA_STRUCT_TYPE Type, LPCTSTR Name);
	bool DataStructListMoveUp(BASE_DATA_STRUCT_DEFINE_LIST * pList);
	bool DataStructListMoveDown(BASE_DATA_STRUCT_DEFINE_LIST * pList);
	bool DataStructListMoveToModule(BASE_DATA_STRUCT_DEFINE_LIST * pList, MODULE_DEFINE_INFO * pModuleInfo);

	vector<MODULE_DEFINE_INFO *>& GetModuleList()
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

	vector<CALLER_INTERFACE *>& GetInterfaceList()
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

	void GetDataStructPacketList(int Type,vector<CString>& NameList);

	bool MoveDataStruct(int Type,CString SourcePacket,CString TargetPacket,CString Name);

	bool Save(MODULE_DEFINE_INFO& ModuleInfo);
	bool Load(LPCTSTR FileName, MODULE_DEFINE_INFO& ModuleInfo);
	bool LoadOld(LPCTSTR FileName, INTERFANCE_DEFINES_OLD& ModuleInfo);
	bool LoadOverVersion(LPCTSTR FileName, MODULE_DEFINE_INFO& ModuleInfo);
	
protected:
	virtual void OnCancel();
	virtual void OnOK();

	
	UINT GetFileVersion(LPCTSTR FileName);

	MODULE_DEFINE_INFO * LoadModule(LPCTSTR szFileName);

	bool SaveInterfaces(xml_node& Interfaces, vector<CALLER_INTERFACE>& InterfaceList);
	bool LoadInterfaces(xml_node& Interfaces, vector<CALLER_INTERFACE>& InterfaceList, UINT ModuleID);
	bool LoadInterfacesOld(xml_node& Interfaces, vector<CALLER_INTERFACE_OLD>& InterfaceList);



	bool SaveStructDefine(xml_node& Structs,vector<STRUCT_DEFINE_INFO>& StructDefineList);
	bool LoadStructDefine(xml_node& Structs,vector<STRUCT_DEFINE_INFO>& StructDefineList);
	bool LoadStructDefineOld(xml_node& Structs, vector<STRUCT_DEFINE_INFO_OLD>& StructDefineList);

	bool SaveEnumDefine(xml_node& Enums,vector<ENUM_DEFINE_INFO>& EnumDefineList);
	bool LoadEnumDefine(xml_node& Enums,vector<ENUM_DEFINE_INFO>& EnumDefineList);

	bool SaveConstDefine(xml_node& Consts,vector<CONST_DEFINE_INFO>& ConstDefineList);
	bool LoadConstDefine(xml_node& Consts,vector<CONST_DEFINE_INFO>& ConstDefineList);

	bool SaveDataObjectModifyFlag(xml_node& ModifyFlags,vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags);
	bool LoadDataObjectModifyFlag(xml_node& ModifyFlags,vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags);

	bool SaveGenerateOperations(xml_node& XmlNode,GENERATE_OPERATIONS_INFO& GenerateOperations);
	bool LoadGenerateOperations(xml_node& XmlNode,GENERATE_OPERATIONS_INFO& GenerateOperations);

	//CString EncodeString(CString Str);
	//CString DecodeString(CString Str);

	bool ConvertOldInterfaceDefines(INTERFANCE_DEFINES_OLD& OldModuleInfo, MODULE_DEFINE_INFO& ModuleInfo);
	INTERFACE_METHOD_OLD * FindInterfaceMethod(INTERFANCE_DEFINES_OLD& OldInterfaceDefines, CString InterfaceName, CString MethodName, bool FindAck);
	CALLER_INTERFACE_OLD * FindInterface(INTERFANCE_DEFINES_OLD& OldInterfaceDefines, CString InterfaceName, bool FindAck);
	INTERFACE_METHOD_OLD * FindMethod(vector<INTERFACE_METHOD_OLD>& OldMethodList, CString MethodName, bool FindAck);

	bool Export(LPCTSTR ExportDir);
	bool ExportDataStructDefineHeader(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName);
	bool ExportInterfaceHeader(vector<CALLER_INTERFACE *>& InterfaceList, LPCTSTR ExportDir, LPCTSTR FileName);
	bool ExportDataStructDefines(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, LPCTSTR ExportDir, LPCTSTR SpaceName);
	bool ExportDataObject(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, LPCTSTR ExportDir, LPCTSTR SpaceName);
	bool ExportInterfaces(vector<CALLER_INTERFACE *>& InterfaceList, LPCTSTR ExportDir, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR SpaceName);

	bool ExportStructs(LPCTSTR ExportDir, LPCTSTR FileName, LPCTSTR SpaceName, vector<STRUCT_DEFINE_INFO>& StructList);
	bool ExportSystemEnums(LPCTSTR ExportDir, LPCTSTR FileName, vector<MODULE_DEFINE_INFO *>& AllModuleList);
	bool ExportEnums(LPCTSTR ExportDir, LPCTSTR FileName, vector<ENUM_DEFINE_INFO>& EnumList, LPCTSTR SpaceName, LPCTSTR ListName, bool IsSystem);
	bool ExportConsts(LPCTSTR ExportDir, LPCTSTR FileName, vector<CONST_DEFINE_INFO>& ConstList, LPCTSTR SpaceName, LPCTSTR ListName);

	bool ExportDataObjectModifyFlags(vector<DATA_OBJECT_MODIFY_FLAG> DataObjectModifyFlagList, LPCTSTR ExportDir, LPCTSTR SpaceName);
	
	

	bool ExportMsgCaller(LPCTSTR ExportDir, LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType);
	bool ExportMsgHandler(LPCTSTR ExportDir, LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType);
	
	

	

	CString MakeMethods(vector<INTERFACE_METHOD> MethodList, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace, bool IsPure);
	CString MakeMethodSSTIDEnumDefines(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);
	CString MakeStructSSTIDEnumDefines(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeMsgPackMethods(vector<INTERFACE_METHOD>& MethodList, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);
	CString MakeParams(vector<METHOD_PARAM>& ParamList,bool IsDefine,bool IncludeDefaultValue);
	CString MakeParamDescriptions(vector<METHOD_PARAM>& ParamList,LPCTSTR szLineSpace);
	CString MakeParamDefines(vector<METHOD_PARAM>& ParamList,LPCTSTR szLineSpace);

	CString MakeMethodsSource(LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);
	CString MakeMsgPackMethodsSource(LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);

	CString MakeInitOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);
	CString MakePackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, CString SSTIDEnumName, LPCTSTR szLineSpace);
	CString MakeUnpackOperations(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, CString SSTIDEnumName, LPCTSTR szLineSpace);
	CString MakePackSizes(INTERFACE_METHOD& MethodInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);

	CString MakeInitOperations(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakePackOperations(STRUCT_DEFINE_INFO& StructInfo,CString SSTIDEnumName,LPCTSTR szLineSpace);
	CString MakeUnpackOperations(STRUCT_DEFINE_INFO& StructInfo,CString SSTIDEnumName,LPCTSTR szLineSpace);
	CString MakePackSizes(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeXMLProcess(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace);
	CString MakeToXMLOperations(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace);
	CString MakeFromXMLOperations(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace);
	CString MakeJsonProcess(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace);
	CString MakeDataObjectJsonProcess(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace);
	CString MakeToJsonOperations(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace);
	CString MakeFromJsonOperations(STRUCT_DEFINE_INFO& StructInfo, CString SSTIDEnumName, LPCTSTR szLineSpace);

	CString MakePackOperation(CString Type,CString Name,CString ParentName,CString ParentShortName,CString SSTIDEnumName,bool IsInDataObject,bool IsMFCheck,bool IsArray,LPCTSTR szLineSpace);
	CString MakeUnpackOperation(CString Type,CString Name,CString ParentName,CString ParentShortName,CString SSTIDEnumName,bool IsInDataObject,bool IsMonitorUpdate,bool IsArray,LPCTSTR szLineSpace);
	CString MakeToXMLOperation(CString Type, CString Name, CString ParentName, CString ParentShortName, CString SSTIDEnumName, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR szLineSpace);
	CString MakeFromXMLOperation(CString Type, CString Name, CString ParentName, CString ParentShortName, CString SSTIDEnumName, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR szLineSpace);
	CString MakeToJsonOperation(CString Type, CString Name, CString ParentName, CString ParentShortName, CString SSTIDEnumName, bool IsInDataObject, bool IsMFCheck, bool IsArray, LPCTSTR szLineSpace);
	CString MakeFromJsonOperation(CString Type, CString Name, CString ParentName, CString ParentShortName, CString SSTIDEnumName, bool IsInDataObject, bool IsMonitorUpdate, bool IsArray, LPCTSTR szLineSpace);


	CString MakeMsgHandlerMethods(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);
	CString MakeMsgHandlerMethodsSource(CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);

	CString MakeMsgMapInits(LPCTSTR SpaceName, CALLER_INTERFACE& InterfaceInfo, INTERFACE_METHOD_EXPORT_TYPE ExportType, LPCTSTR szLineSpace);

	CString MakeStructDefines(vector<STRUCT_DEFINE_INFO>& StructDefineList,LPCTSTR szLineSpace);
	CString MakeStructMembers(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);

	CString MakeCloneOperations(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace,LPCTSTR szLeftValue,LPCTSTR szRightValue);

	bool MakeDataObjectFlagIndicesEnum(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& AllDataStructList, ENUM_DEFINE_INFO& EnumInfo);
	bool MakeDataObjectSSTIDsEnum(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, ENUM_DEFINE_INFO& EnumInfo);
	CString MakeDataObjectModifyFlagEnumDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataObjectMembersGetModifyFlag(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataDataObjectMembersIsModified(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataDataObjectMembersClearModifyFlag(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	
	CString MakeDataObjectSetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataObjectSetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataObjectGetMethodsDeclare(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);
	CString MakeDataObjectGetMethodsDefine(STRUCT_DEFINE_INFO& StructInfo,LPCTSTR szLineSpace);

	CString MakeDataObjectModifyFlags(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& AllDataStructList, vector<DATA_OBJECT_MODIFY_FLAG>& DataObjectModifyFlags, LPCTSTR szLineSpace);
	CString MakeDataObjectModifyFlagData(DATA_OBJECT_MODIFY_FLAG& ModifyFlag,LPCTSTR szLineSpace);


	CString MakeEnumDefine(ENUM_DEFINE_INFO& EnumInfo,LPCTSTR szLineSpace);
	CString MakeEnumDefines(vector<ENUM_DEFINE_INFO>& EnumDefineList,LPCTSTR szLineSpace);

	CString MakeEnumStrValue(ENUM_DEFINE_INFO& EnumInfo,LPCTSTR szLineSpace);
	CString MakeEnumStrValues(vector<ENUM_DEFINE_INFO>& EnumDefineList,LPCTSTR szLineSpace);

	CString MakeConstDefines(vector<CONST_DEFINE_INFO>& ConstDefineList,LPCTSTR szLineSpace);

	CString MakeStringConst(LPCTSTR StrValue);


	DATA_OBJECT_MODIFY_FLAG MakeDataObjectFullModifyFlag(STRUCT_DEFINE_INFO& StructInfo,vector<STRUCT_DEFINE_INFO *>& DataObjectList);

	bool HaveMemberByType(STRUCT_DEFINE_INFO& StructInfo, CString Type, int Depth = 32);
	STRUCT_DEFINE_INFO * GetStructDefineInfo(CString Type);
	

	CString GetLineSpace(CString& TempleStr,LPCTSTR szWord);
	int GetHeadLen(CString& TempleStr,LPCTSTR szWord);
	CString ToComment(CString& Content,LPCTSTR szLineSpace);

	CString ClassNameToFileName(LPCTSTR szClassName);
	CString ClassNameToUpper(LPCTSTR szClassName);

	bool IsBigChar(TCHAR c);

	void RemoveBlock(CString& Template,LPCTSTR szBlockStart,LPCTSTR szBlockEnd);

	CString RestoreFileToTemplate(LPCTSTR szFileName,const CString& szTemplate);
	CString RestoreToTemplate(LPCTSTR szInput,const CString& szTemplate);

	void WriteStringToFile(CFile& OutputFile, CString& Str);

	bool EndWith(CString& Str, CString EndStr);

	void SortStructForExport(const vector<STRUCT_DEFINE_INFO>& SrcStructList, vector<STRUCT_DEFINE_INFO>& DestStructList);
	void GetStructDepends(const vector<STRUCT_DEFINE_INFO>& StructList, const STRUCT_DEFINE_INFO& StructInfo, vector<CString>& DependList, int Depth);
	void AddUnique(vector<STRUCT_DEFINE_INFO>& StructList, const STRUCT_DEFINE_INFO& StructInfo);

	void SortDefinesForExport(const vector<BASE_DATA_STRUCT_DEFINE_LIST *>& SrcList, vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DestList);
	void GetDefineDepends(const vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DefineList, const STRUCT_DEFINE_LIST * pStructDefine, vector<CString>& DependList, int Depth);
	void GetStructForeignDepends(const vector<STRUCT_DEFINE_INFO>& StructList, vector<CString>& DependList);
	void AddUnique(vector<CString>& NameList, const CString& Name);
	void AddUnique(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DefineList, BASE_DATA_STRUCT_DEFINE_LIST * pDefine);
	bool HaveStructs(const STRUCT_DEFINE_LIST * pStructDefine, vector<CString>& StructNameList);
	CString MakeDefineName(const STRUCT_DEFINE_LIST * pStructDefine);

	void SortModuleForExport(const vector<MODULE_DEFINE_INFO *>& SrcList, vector<MODULE_DEFINE_INFO *>& DestList);
	void GetModuleDepends(const vector<MODULE_DEFINE_INFO *>& ModuleList, const MODULE_DEFINE_INFO * pModuleInfo, vector<CString>& DependList, int Depth);
	void AddUnique(vector<MODULE_DEFINE_INFO *>& ModuleList, MODULE_DEFINE_INFO * pModuleInfo);
	bool HaveDefines(const MODULE_DEFINE_INFO * pModuleInfo, vector<CString>& DefineNameList);

	void SaveInterfaceSelections();
	void RestoreInterfaceSelections();

	void FetchInterfaceToCache(MODULE_DEFINE_INFO& InterfaceDefines);
	void FetchDataStructToCache(MODULE_DEFINE_INFO& InterfaceDefines);

	void ArrangeIds(MODULE_DEFINE_INFO& ModuleInfo);
	bool CheckBaseStructObjectID(vector<UINT>& ObjectIDList, LPCTSTR szBaseStructName);
public:
	void GetDataObjectListSorted(vector<BASE_DATA_STRUCT_DEFINE_LIST *>& DataStructList, vector<STRUCT_DEFINE_INFO *>& DataObjectList);
	static int InterfaceInfoComp(LPCVOID p1, LPCVOID p2);
	static int InterfaceMethodComp(LPCVOID p1, LPCVOID p2);
	static int StructComp(LPCVOID p1, LPCVOID p2);
	static int EnumComp(LPCVOID p1, LPCVOID p2);
	static int ConstComp(LPCVOID p1, LPCVOID p2);
	static int DataObjectComp(LPCVOID p1, LPCVOID p2);
	
	
	
	
};


inline UINT CCallInterfaceMakerDlg::GetVarTypeCount()
{
	return m_CurVarTypeList.size();
}
inline TYPE_DEFINE * CCallInterfaceMakerDlg::GetVarType(UINT Index)
{
	if(Index<m_CurVarTypeList.size())
	{
		return &m_CurVarTypeList.at(Index);
	}
	return NULL;
}

inline TYPE_DEFINE * CCallInterfaceMakerDlg::FindVarType(LPCTSTR TypeName)
{
	for(UINT i=0;i<m_CurVarTypeList.size();i++)
	{
		if(m_CurVarTypeList[i].Name==TypeName)
		{
			return &m_CurVarTypeList[i];
		}
	}
	return NULL;
}