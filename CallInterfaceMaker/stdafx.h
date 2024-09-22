/****************************************************************************/
/*                                                                          */
/*      文件名:    stdafx.h                                                 */
/*      创建日期:  2012年04月22日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#define CONFIG_FILE_NAME_XML										_T("Config.xml")
#define ENV_FILE_NAME_XML											_T("Env.xml")
#define TYPE_DEF_FILE_NAME_XML										_T("TypeDef.xml")

#define CONFIG_FILE_NAME_JSON										_T("Config.json")
#define ENV_FILE_NAME_JSON											_T("Env.json")
#define TYPE_DEF_FILE_NAME_JSON										_T("TypeDef.json")

#define INTERFACE_HEADER_TEMPLE_FILE_NAME							_T("InterfaceHeader.tmpl")
#define INTERFACE_TEMPLE_FILE_NAME									_T("Interface.tmpl")

#define PARAM_DEFINE_TEMPLE_FILE_NAME								_T("ParamDefine.tmpl")
#define PARAM_TRANS_TEMPLE_FILE_NAME								_T("ParamTrans.tmpl")
#define PARAM_DESCRIPTION_TEMPLE_FILE_NAME							_T("ParamDescript.tmpl")

#define PACK_SIZE_TEMPLE_FILE_NAME									_T("PackSize.tmpl")

#define METHOD_HEADER_TEMPLE_FILE_NAME								_T("MethodHeader.tmpl")
#define METHOD_HEADER_PURE_TAIL_TEMPLE_FILE_NAME					_T("MethodHeaderPureTail.tmpl")
#define METHOD_CALLER_SOURCE_TEMPLE_FILE_NAME						_T("MethodCallerSource.tmpl")
#define MSG_PACK_METHOD_HEADER_TEMPLE_FILE_NAME 					_T("MsgPackMethodHeader.tmpl")
#define MSG_PACK_METHOD_SOURCE_TEMPLE_FILE_NAME 					_T("MsgPackMethodSource.tmpl")
#define MSG_HANDLER_METHOD_HEADER_TEMPLE_FILE_NAME 					_T("MsgHandlerMethodHeader.tmpl")
#define MSG_HANDLER_METHOD_SOURCE_TEMPLE_FILE_NAME 					_T("MsgHandlerMethodSource.tmpl")

#define MSG_MAP_INIT_TEMPLE_FILE_NAME								_T("MsgMapInit.tmpl")


#define MSG_CALLER_HEADER_TEMPLE_FILE_NAME							_T("MsgCallerHeader.tmpl")
#define MSG_CALLER_SOURCE_TEMPLE_FILE_NAME							_T("MsgCallerSource.tmpl")


#define MSG_HANDLER_HEADER_TEMPLE_FILE_NAME							_T("MsgHandlerHeader.tmpl")
#define MSG_HANDLER_SOURCE_TEMPLE_FILE_NAME							_T("MsgHandlerSource.tmpl")

#define DATA_OBJECT_DEFINE_HEADER_TEMPLE_FILE_NAME					_T("DataObjectDefineHeader.tmpl")
#define DATA_OBJECT_DEFINE_SOURCE_TEMPLE_FILE_NAME					_T("DataObjectDefineSource.tmpl")

#define STRUCT_DEFINE_HEADER_TEMPLE_FILE_NAME						_T("StructDefineHeader.tmpl")
#define ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME							_T("EnumDefineHeader.tmpl")
#define SYSTEM_ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME					_T("SystemEnumDefineHeader.tmpl")
#define STRUCT_DEFINE_TEMPLE_FILE_NAME								_T("StructDefine.tmpl")
#define STRUCT_MEMBER_TEMPLE_FILE_NAME								_T("StructMember.tmpl")
#define DATA_OBJECT_MEMBER_TEMPLE_FILE_NAME							_T("DataObjectMember.tmpl")

#define DATA_OBJECT_MODIFY_FLAGS_HEADER_TEMPLE_FILE_NAME			_T("DataObjectModifyFlagsHeader.tmpl")
#define DATA_OBJECT_MODIFY_FLAGS_SOURCE_TEMPLE_FILE_NAME			_T("DataObjectModifyFlagsSource.tmpl")
#define DATA_OBJECT_MODIFY_FLAG_HEADER_DEFINE_TEMPLE_FILE_NAME		_T("DataObjectModifyFlagHeaderDefine.tmpl")
#define DATA_OBJECT_MODIFY_FLAG_DEFINE_TEMPLE_FILE_NAME				_T("DataObjectModifyFlagDefine.tmpl")
#define DATA_OBJECT_MODIFY_FLAG_UNIT_TEMPLE_FILE_NAME				_T("DataObjectModifyFlagUnit.tmpl")

#define PACK_OPERATION_UNIT_TEMPLE_FILE_NAME						_T("PackOperationUnit.tmpl")
#define UNPACK_OPERATION_UNIT_TEMPLE_FILE_NAME						_T("UnpackOperationUnit.tmpl")

#define TO_XML_OPERATION_UNIT_TEMPLE_FILE_NAME						_T("ToXMLOperationUnit.tmpl")
#define FROM_XML_OPERATION_UNIT_TEMPLE_FILE_NAME					_T("FromXMLOperationUnit.tmpl")
#define STRUCT_XML_PROCESS_TEMPLE_FILE_NAME							_T("StructXMLProcess.tmpl")

#define TO_JSON_OPERATION_UNIT_TEMPLE_FILE_NAME						_T("ToJsonOperationUnit.tmpl")
#define FROM_JSON_OPERATION_UNIT_TEMPLE_FILE_NAME					_T("FromJsonOperationUnit.tmpl")
#define STRUCT_JSON_PROCESS_TEMPLE_FILE_NAME						_T("StructJsonProcess.tmpl")
#define DATA_OBJECT_JSON_PROCESS_TEMPLE_FILE_NAME					_T("DataObjectJsonProcess.tmpl")
#define DATA_OBJECT_JSON_PROCESS_HEADER_TEMPLE_FILE_NAME			_T("DataObjectJsonProcessHeader.tmpl")

#define STRUCT_DB_PROCESS_TEMPLE_FILE_NAME							_T("StructDBProcess.tmpl")
#define DATA_OBJECT_DB_PROCESS_TEMPLE_FILE_NAME						_T("DataObjectDBProcess.tmpl")
#define DATA_OBJECT_DB_PROCESS_HEADER_TEMPLE_FILE_NAME				_T("DataObjectDBProcessHeader.tmpl")
#define DB_FIELD_NAME_TEMPLE_FILE_NAME								_T("DBFieldName.tmpl")
#define DB_FIELD_DEFINE_UNIT_TEMPLE_FILE_NAME						_T("DBFieldDefineUnit.tmpl")
#define DB_KEY_DEFINE_UNIT_TEMPLE_FILE_NAME							_T("DBKeyDefineUnit.tmpl")
#define DB_PRIMARY_KEY_DEFINE_UNIT_TEMPLE_FILE_NAME					_T("DBPrimaryKeyDefineUnit.tmpl")
#define DB_UNIQUE_KEY_DEFINE_UNIT_TEMPLE_FILE_NAME					_T("DBUniqueKeyDefineUnit.tmpl")

#define ENUM_DFINE_TEMPLE_FILE_NAME									_T("EnumDefine.tmpl")
#define ENUM_DFINE_64_TEMPLE_FILE_NAME								_T("EnumDefine64.tmpl")
#define ENUM_MEMBER_DFINE_TEMPLE_FILE_NAME							_T("EnumMemberDefine.tmpl")
#define ENUM_MEMBER_DFINE_64_TEMPLE_FILE_NAME						_T("EnumMemberDefine64.tmpl")
#define ENUM_BIT_MASK_MEMBER_DFINE_TEMPLE_FILE_NAME					_T("EnumBitMaskMemberDefine.tmpl")
#define ENUM_BIT_MASK_MEMBER_DFINE_64_TEMPLE_FILE_NAME				_T("EnumBitMaskMemberDefine64.tmpl")
#define ENUM_STR_VALUE_UNIT_TEMPLE_FILE_NAME						_T("EnumStrValueUnit.tmpl")
#define ENUM_LIST_UNIT_TEMPLE_FILE_NAME								_T("EnumListUnit.tmpl")
#define ENUM_TO_STR_UNIT_TEMPLE_FILE_NAME							_T("EnumToStrUnit.tmpl")
#define STR_TO_ENUM_UNIT_TEMPLE_FILE_NAME							_T("StrToEnum.tmpl")
#define ENUM_BIND_DATA_FILL_UNIT_TEMPLE_FILE_NAME					_T("EnumBindDataFillUnit.tmpl")
//#define ENUM_BIND_DATA_FETCH_UNIT_TEMPLE_FILE_NAME					_T("EnumBindDataFetchUnit.tmpl")

#define CONST_DFINE_TEMPLE_FILE_NAME								_T("ConstDefine.tmpl")
#define MACRO_DFINE_TEMPLE_FILE_NAME								_T("MacroDefine.tmpl")
#define CONST_DFINE_HEADER_TEMPLE_FILE_NAME							_T("ConstDefineHeader.tmpl")

#define STRING_CONST_DFINE_TEMPLE_FILE_NAME							_T("StringConstDefine.tmpl")

#define IMPORT_DECLARE_TEMPLE_FILE_NAME								_T("ImportDeclare.tmpl")

#define DATA_OBJECT_CLEAR_MODIFY_FLAG_DEFINE_TEMPLE_FILE_NAME		_T("DataObjectClearModifyFlagDefine.tmpl")
#define DATA_OBJECT_GET_MODIFY_FLAG_DEFINE_TEMPLE_FILE_NAME			_T("DataObjectGetModifyFlagDefine.tmpl")
#define DATA_OBJECT_IS_MODIFIED_DEFINE_TEMPLE_FILE_NAME				_T("DataObjectIsModifiedDefine.tmpl")

#define STRUCT_EDITOR_PROCESS_TEMPLE_FILE_NAME						_T("StructEditorProcess.tmpl")
#define CREATE_LIST_HEADER_UNIT_TEMPLE_FILE_NAME					_T("CreateListHeaderUnit.tmpl")
#define FILL_LIST_ITEM_UNIT_TEMPLE_FILE_NAME						_T("FillListItemUnit.tmpl")
#define PROPERTY_GRID_FETCH_UNIT_TEMPLE_FILE_NAME					_T("PropertyGridFetchUnit.tmpl")
#define PROPERTY_GRID_ADD_ITEM_UNIT_TEMPLE_FILE_NAME				_T("PropertyGridAddItemUnit.tmpl")
#define PROPERTY_GRID_DEL_ITEM_UNIT_TEMPLE_FILE_NAME				_T("PropertyGridDelItemUnit.tmpl")
#define PROPERTY_GRID_MOVE_ITEM_UNIT_TEMPLE_FILE_NAME				_T("PropertyGridMoveItemUnit.tmpl")

#define STRUCT_FILE_LOG_PROCESS_TEMPLE_FILE_NAME					_T("StructFileLogProcess.tmpl")
#define STRUCT_ALI_LOG_PROCESS_TEMPLE_FILE_NAME						_T("StructAliLogProcess.tmpl")

#define STRUCT_LUA_PROCESS_TEMPLE_FILE_NAME							_T("StructLuaProcess.tmpl")
#define DATA_OBJECT_LUA_PROCESS_TEMPLE_FILE_NAME					_T("DataObjectLuaProcess.tmpl")
#define DATA_OBJECT_LUA_PROCESS_HEADER_TEMPLE_FILE_NAME				_T("DataObjectLuaProcessHeader.tmpl")
#define TO_LUA_TABLE_OPERATION_UNIT_TEMPLE_FILE_NAME				_T("ToLuaTableOperationUnit.tmpl")
#define FROM_LUA_TABLE_OPERATION_UNIT_TEMPLE_FILE_NAME				_T("FromLuaTableOperationUnit.tmpl")

#define STRUCT_XLS_PROCESS_TEMPLE_FILE_NAME							_T("StructXLSProcess.tmpl")

#define DEPEND_IMPORT_TEMPLE_FILE_NAME								_T("DependImport.tmpl")


#define ALL_DEFINES_TEMPLE_FILE_NAME								_T("AllDefines.tmpl")

#define USE_CRT_DETAIL_NEW

#include "..\..\Libs\Utils\Utils.h"
#include "..\..\Libs\MFCExtensionLib\MFCExtensionLib.h"

#include "resource.h"

#include <vector>
#include <fstream>

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#undef new
#endif

#include "../../Libs/rapidjson/document.h"
#include "../../Libs/rapidjson/stringbuffer.h"
#include "../../Libs/rapidjson/prettywriter.h"



#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#define new NEWNEW
#endif

#include "Tools.h"

using namespace std;

enum INTERFACE_METHOD_TYPE
{
	INTERFACE_METHOD_TYPE_CALL,
	INTERFACE_METHOD_TYPE_NOTIFY,
	INTERFACE_METHOD_TYPE_CALL_WITHOUT_RESULT,
	INTERFACE_METHOD_TYPE_MAX,
};

enum INTERFACE_METHOD_EXPORT_TYPE
{
	INTERFACE_METHOD_EXPORT_TYPE_CALL,
	INTERFACE_METHOD_EXPORT_TYPE_ACK
};

enum INTERFACE_DEPEND_MAKE_TYPE
{
	INTERFACE_DEPEND_MAKE_DECLARE,
	INTERFACE_DEPEND_MAKE_ACK_DECLARE,
	INTERFACE_DEPEND_MAKE_CALLER,
	INTERFACE_DEPEND_MAKE_ACK_CALLER,
	INTERFACE_DEPEND_MAKE_HANDLER,
	INTERFACE_DEPEND_MAKE_ACK_HANDLER,
};

enum INTERFACE_METHOD_PARAM_TYPE
{
	INTERFACE_METHOD_PARAM_TYPE_CALL,
	INTERFACE_METHOD_PARAM_TYPE_ACK,
};

const LPCTSTR g_szINTERFACE_METHOD_TYPE[] = { _T("调用方法"), _T("通知方法") ,_T("无返回调用")};

enum STRUCT_FLAG
{
	STRUCT_FLAG_IS_DATA_OBJECT = 1,
	STRUCT_FLAG_DECLARE_LATER = (1<<1),
	STRUCT_FLAG_EXPORT_XML_PROCESS = (1<<2),
	STRUCT_FLAG_EXPORT_JSON_PROCESS = (1 << 3),
	STRUCT_FLAG_EXPORT_DB_PROCESS = (1 << 4),
	STRUCT_FLAG_EXPORT_FILE_LOG_PROCESS = (1 << 5),
	STRUCT_FLAG_EXPORT_ALI_LOG_PROCESS = (1 << 6),
	STRUCT_FLAG_EXPORT_LUA_PROCESS = (1 << 7),
	STRUCT_FLAG_EXPORT_XLS_PROCESS = (1 << 8),
};

enum STRUCT_MEMBER_FLAG
{
	STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE=1,
	STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET=(1<<1),
	STRUCT_MEMBER_FLAG_MERGE_TO_PARENT=(1<<2),
	STRUCT_MEMBER_FLAG_MUST_PACK=(1<<3),
	STRUCT_MEMBER_FLAG_HIDE_IN_EDITOR_LIST = (1 << 4),
	STRUCT_MEMBER_FLAG_HIDE_IN_PROPERTY_GRID = (1 << 5),
	STRUCT_MEMBER_FLAG_IGNORE_IN_LUA_PROCESS = (1 << 6),
	STRUCT_MEMBER_FLAG_HIDE_IN_XLS = (1 << 7),
	STRUCT_MEMBER_FLAG_IS_KEY = (1 << 8),
};

enum TYPE_DEFINE_FLAG
{
	TYPE_DEFINE_FLAG_STRUCT = 1,
	TYPE_DEFINE_FLAG_DATA_OBJECT = (1 << 1),
	TYPE_DEFINE_FLAG_REF_TYPE = (1 << 2),
	TYPE_DEFINE_FLAG_ENUM = (1 << 3),
	TYPE_DEFINE_FLAG_64BIT = (1 << 4),
	TYPE_DEFINE_FLAG_DB_USE_PARAM_BIND = (1 << 5),
	TYPE_DEFINE_FLAG_BASE_TYPE = (1 << 6),
	TYPE_DEFINE_FLAG_VARIABLE_LEN = (1 << 7),
	TYPE_DEFINE_FLAG_SYSTEM_TYPE = (1 << 8),
	TYPE_DEFINE_FLAG_ENUM_HAVE_STR_TRANS = (1 << 9),
};


enum ENUM_DEFINE_FLAG
{
	ENUM_DEFINE_FLAG_IS_64BIT=1,
	ENUM_DEFINE_FLAG_EXPORT_STR_VALUE=(1<<1),
	ENUM_DEFINE_FLAG_EXPORT_STR_TRANS_FN = (1 << 3),
	ENUM_DEFINE_FLAG_EXPORT_ENUM_LIST = (1 << 4),
	ENUM_DEFINE_FLAG_IS_FLAG = (1 << 5),
};

enum ENUM_MEMBER_FLAG
{
	ENUM_MEMBER_FLAG_NOT_EXPORT_OTHER = 1,
	ENUM_MEMBER_FLAG_IS_BIT_MASK = (1 << 2),
	ENUM_MEMBER_FLAG_HIDE_IN_PROPERTY_GRID = (1 << 3),
};

enum DATA_STRUCT_TYPE
{
	DATA_STRUCT_CONST,
	DATA_STRUCT_ENUM,
	DATA_STRUCT_STRUCT,
	DATA_STRUCT_MAX,
};

enum INTERFACE_METHOD_FLAG
{
	INTERFACE_METHOD_FLAG_CAN_CACHE = 1,
	INTERFACE_METHOD_FLAG_NO_COMPRESS = (1 << 1),
};

enum DB_INDEX_TYPE
{
	DB_INDEX_TYPE_NONE,
	DB_INDEX_TYPE_KEY,
	DB_INDEX_TYPE_PRIMARY_KEY,
	DB_INDEX_TYPE_UNIQUE_KEY,
	DB_INDEX_TYPE_MAX,
};

enum PACK_OPERATION_FLAG
{
	PACK_OPERATION_FLAG_IN_INTERFACE = 1,
	PACK_OPERATION_FLAG_IN_DATA_OBJECT = 1 << 1,
	PACK_OPERATION_FLAG_NEED_MF_CHECK = 1 << 2,
	PACK_OPERATION_FLAG_IS_ARRAY = 1 << 3,
};

const LPCTSTR g_szDATA_STRUCT_TYPE[DATA_STRUCT_MAX]={_T("常量"),_T("枚举"),_T("结构")};

const LPCTSTR g_szDB_INDEX_TYPE[DB_INDEX_TYPE_MAX] = { _T("无"),_T("索引"),_T("主键"),_T("唯一索引") };

struct MODIFY_FLAG_UNIT
{
	CString			ClassName;
	vector<CString>	Members;
};

struct DATA_OBJECT_MODIFY_FLAG
{
	CString						Name;
	vector<MODIFY_FLAG_UNIT>	ModifyFlag;
};

struct GENERATE_OPERATIONS_INFO
{
	CString						PackOperation;
	CString						UnpackOperation;
	CString						SizeCaculateOperation;
	CString						ToXMLOperation;
	CString						FromXMLOperation;
	CString						ToJsonOperation;
	CString						FromJsonOperation;
	CString						ReferenceDefine;
	CString						ReferenceUse;
	CString						VariableDefine;
	CString						InitOperation;
	CString						CloneOperation;
	CString						GetMethodDeclare;
	CString						GetMethodDefine;
	CString						SetMethodDeclare;
	CString						SetMethodDefine;
	CString						IndexOperation;
	CString						ConstIndexOperation;
	CString						ToStringOperation;
	CString						DBFieldDefineOperation;
	CString						DBInsertFormatOperation;
	CString						DBPutOperation;
	CString						DBGetOperation;
	CString						FileLogFillOperation;
	CString						AliLogSendOperation;
	CString						ToLuaOperation;
	CString						FromLuaOperation;
	CString						CreateXLSColumnOperation;
	CString						CheckXLSColumnOperation;
	CString						ToXLSOperation;
	CString						FromXLSOperation;
	CString						FormatSpecOperation;
};

class BASE_DATA_STRUCT_DEFINE_LIST
{
public:
	int			ListType;
	CString		ListName;
	UINT		ModuleID;
	bool		IgnoreOnExport;
	BASE_DATA_STRUCT_DEFINE_LIST()
	{
		ListType = 0;
		ModuleID = 0;
		IgnoreOnExport = false;
	}
	virtual ~BASE_DATA_STRUCT_DEFINE_LIST()
	{

	}
};

struct STRUCT_MEMBER_INFO
{
	CString	Name;
	CString	Type;
	BYTE	DBIndexType;
	UINT	Flag;
	bool	IsArray;
	UINT	ArrayStartLength;
	UINT	ArrayGrowLength;
	CString	DBLength;
	CString ShowName;
	CString	Description;
	CString	PackFlag;
	UINT	ID;
	CString	BindData;
	CString ExtendType;
	STRUCT_MEMBER_INFO()
	{
		Flag = 0;
		DBIndexType = DB_INDEX_TYPE_NONE;
		IsArray = false;
		ArrayStartLength = 0;
		ArrayGrowLength = 8;
		ID = 1;
	}
};

struct STRUCT_DEFINE_INFO_OLD
{
	CString						Name;
	CString						ShortName;
	CString						BaseStruct;
	vector<STRUCT_MEMBER_INFO>	MemberList;
	CString						Description;
	bool						IsDataObject;
	bool						DeclareLater;
	UINT						ObjectID;
	UINT						IDSeed;
	GENERATE_OPERATIONS_INFO	GenerateOperations;
	STRUCT_DEFINE_INFO_OLD()
	{
		IsDataObject = false;
		DeclareLater = false;
		ObjectID = 0;
		IDSeed = 1;
	}
};

struct STRUCT_DEFINE_INFO
{
	CString						Name;
	CString						ShortName;
	CString						ShowName;
	CString						BaseStruct;
	vector<STRUCT_MEMBER_INFO>	MemberList;
	CString						Description;
	UINT						Flag;
	UINT						ObjectID;
	UINT						IDSeed;	
	GENERATE_OPERATIONS_INFO	GenerateOperations;	
	STRUCT_DEFINE_INFO()
	{
		Flag = 0;
		ObjectID = 0;
		IDSeed = 1;		
	}
};

class STRUCT_DEFINE_LIST:public BASE_DATA_STRUCT_DEFINE_LIST
{	
public:
	vector<STRUCT_DEFINE_INFO>		StructList;	
	virtual ~STRUCT_DEFINE_LIST()
	{

	}
};

class STRUCT_DEFINE_LIST2 :public BASE_DATA_STRUCT_DEFINE_LIST
{
public:
	vector<STRUCT_DEFINE_INFO*>		StructList;
	virtual ~STRUCT_DEFINE_LIST2()
	{

	}
};

class STRUCT_DEFINE_LIST_OLD :public BASE_DATA_STRUCT_DEFINE_LIST
{
public:
	vector<STRUCT_DEFINE_INFO_OLD>		StructList;
	virtual ~STRUCT_DEFINE_LIST_OLD()
	{

	}
};

struct ENUM_MEMBER_INFO
{
	CString	Name;	
	CString Value;
	CString StrValue;
	CString	Description;
	UINT	Flag;	
	CString	BindDataType;
	ENUM_MEMBER_INFO()
	{
		Flag = 0;
	}
};

struct ENUM_DEFINE_INFO
{
	CString						Name;
	CString						ShortName;	
	CString						ShowName;
	UINT						Flag;
	vector<ENUM_MEMBER_INFO>	MemberList;
	CString						Description;
	ENUM_DEFINE_INFO()
	{
		Flag=0;
	}
};

class ENUM_DEFINE_LIST :public BASE_DATA_STRUCT_DEFINE_LIST
{
public:
	vector<ENUM_DEFINE_INFO>		EnumList;
	virtual ~ENUM_DEFINE_LIST()
	{

	}
};

struct CONST_DEFINE_INFO
{
	CString 	Name;
	CString 	Type;
	CString		Value;
	CString		Description;
};

class CONST_DEFINE_LIST :public BASE_DATA_STRUCT_DEFINE_LIST
{
public:
	vector<CONST_DEFINE_INFO>		ConstList;
	virtual ~CONST_DEFINE_LIST()
	{

	}
};

struct TYPE_DEFINE
{
	CString 					Name;
	CString 					CType;
	CString						SourceListName;
	UINT						Flag;
	GENERATE_OPERATIONS_INFO	GenerateOperations;
	
	TYPE_DEFINE()
	{
		Flag=0;
	}
};

struct DEPEND_INFO
{
	DATA_STRUCT_TYPE			ListType;
	CString						ListName;
	vector<const TYPE_DEFINE*>	DependTypes;
};


struct DATA_STRUCT_DEFINES
{
	vector<BASE_DATA_STRUCT_DEFINE_LIST *>	DataStructDefineList;	
	vector<DATA_OBJECT_MODIFY_FLAG>			DataObjectModifyFlags;	
	void Clear()
	{
		for (size_t i = 0; i < DataStructDefineList.size(); i++)
		{
			SAFE_DELETE(DataStructDefineList[i]);
		}
		DataStructDefineList.clear();		
		DataObjectModifyFlags.clear();
	}
};

struct METHOD_PARAM
{
	CString		Name;
	UINT		ID;
	CString		Type;
	int			Length;
	CString		DefaultValue;
	bool		CanNull;
	bool		IsReference;
	bool		IsArray;
	CString		Description;
	CString		PackFlag;
};

struct INTERFACE_METHOD_OLD
{
	CString					Name;
	vector<METHOD_PARAM>	ParamList;	
	UINT					ID;
	UINT					Flag;
	UINT					ParamIDSeed;
	CString					Description;
	INTERFACE_METHOD_OLD()
	{
		ID=0;
		ParamIDSeed=1;
	}
};

struct CALLER_INTERFACE_OLD
{
	CString							Name;
	vector<INTERFACE_METHOD_OLD>	MethodList;
	int								Type;
	UINT							ID;
	UINT							IDSeed;
	CString							Description;
	CALLER_INTERFACE_OLD()
	{
		Type = 0;
		ID = 0;
		IDSeed = 1;
	}
};

struct INTERFACE_METHOD
{
	CString					Name;
	int						Type;
	vector<METHOD_PARAM>	CallParamList;
	vector<METHOD_PARAM>	AckParamList;
	UINT					ID;
	UINT					Flag;
	UINT					ParamIDSeed;
	CString					Description;
	INTERFACE_METHOD()
	{
		Type = INTERFACE_METHOD_TYPE_CALL;
		ID = 0;
		Flag = 0;
		ParamIDSeed = 1;
	}
};

struct CALLER_INTERFACE
{
	CString						Name;
	UINT						ModuleID;
	vector<INTERFACE_METHOD>	MethodList;
	UINT						ID;
	UINT						IDSeed;
	CString						Description;
	CALLER_INTERFACE()
	{
		ModuleID = 0;
		ID = 0;
		IDSeed = 1;
	}
};

struct MODULE_DEFINE_INFO
{
	CString									Name;
	UINT									ModuleID;
	UINT									IDSeed;
	vector<CALLER_INTERFACE>				InterfaceList;
	vector<BASE_DATA_STRUCT_DEFINE_LIST *>	DataStructDefineList;
	vector<DATA_OBJECT_MODIFY_FLAG>			DataObjectModifyFlags;
	CString									InterfacePrefix;
	CString									InterfacePostFix;
	vector<CString>							ImportList;
	CString									ModuleDefineFileName;
	MODULE_DEFINE_INFO()
	{
		Clear();
	}
	void Clear()
	{
		Name.Empty();
		ModuleID = 1;
		IDSeed = 1;
		InterfaceList.clear();
		for (size_t i = 0; i < DataStructDefineList.size(); i++)
		{
			SAFE_DELETE(DataStructDefineList[i]);
		}
		DataStructDefineList.clear();
		DataObjectModifyFlags.clear();
		InterfacePrefix.Empty();
		InterfacePostFix.Empty();
		ImportList.clear();
		ModuleDefineFileName.Empty();
	}
};

struct INTERFANCE_DEFINES_OLD
{
	CString									Name;
	UINT									InterfaceIDSeed;
	vector<CALLER_INTERFACE_OLD>			InterfaceList;
	DATA_STRUCT_DEFINES						DataStructDefines;
	CString									InterfacePrefix;
	CString									InterfacePostFix;
	INTERFANCE_DEFINES_OLD()
	{
		Clear();
	}
	void Clear()
	{
		Name.Empty();
		InterfaceIDSeed = 1;
		InterfaceList.clear();
		DataStructDefines.Clear();
		InterfacePrefix.Empty();
		InterfacePostFix.Empty();
	}
};

struct EXPORT_CONFIG
{
	bool		IsExport;
	CString		ExportExt;
};

struct INTERFACE_CONFIG
{	
	EXPORT_CONFIG				OneFileExportConfig;
	EXPORT_CONFIG				InterfaceHeaderExportConfig;
	EXPORT_CONFIG				StructExportConfig;
	EXPORT_CONFIG				EnumExportConfig;
	EXPORT_CONFIG				ConstExportConfig;
	EXPORT_CONFIG				DataStructHeaderExportConfig;
	EXPORT_CONFIG				InterfaceExportConfig;
	EXPORT_CONFIG				CallHeaderExportConfig;
	EXPORT_CONFIG				CallSourceExportConfig;
	EXPORT_CONFIG				HandlerHeaderExportConfig;
	EXPORT_CONFIG				HandlerSourceExportConfig;
	EXPORT_CONFIG				DataObjectHeaderExportConfig;
	EXPORT_CONFIG				DataObjectSourceExportConfig;
	EXPORT_CONFIG				DataObjectModifyFlagsHeaderExportConfig;
	EXPORT_CONFIG				DataObjectModifyFlagsSourceExportConfig;
	GENERATE_OPERATIONS_INFO	DefaultStructGenerateOperations;
	GENERATE_OPERATIONS_INFO	DefaultEnumGenerateOperations;
	GENERATE_OPERATIONS_INFO	DefaultEnum64GenerateOperations;
	GENERATE_OPERATIONS_INFO	ArrayDefineConfig;
	CString						CommentPrefix;
	CString						LocalVariableDefinePrefix;
	CString						MemberVariablePrefix;
	CString						DefaultPacketName;
	CString						DefaultPacketMemberName;
	CString						PrecompileHeaderFileName;
	bool						SupportBigInt;
	bool						ForceExportAll;
	INTERFACE_CONFIG()
	{
		OneFileExportConfig.IsExport = false;
		OneFileExportConfig.ExportExt = _T("h");
		InterfaceHeaderExportConfig.IsExport = true;
		InterfaceHeaderExportConfig.ExportExt = _T("h");
		StructExportConfig.IsExport = true;
		StructExportConfig.ExportExt = _T("h");
		EnumExportConfig.IsExport = true;
		EnumExportConfig.ExportExt = _T("h");
		ConstExportConfig.IsExport = true;
		ConstExportConfig.ExportExt = _T("h");
		DataStructHeaderExportConfig.IsExport = true;
		DataStructHeaderExportConfig.ExportExt = _T("h");
		InterfaceExportConfig.IsExport = true;
		InterfaceExportConfig.ExportExt = _T("h");
		CallHeaderExportConfig.IsExport = true;
		CallHeaderExportConfig.ExportExt = _T("h");
		CallSourceExportConfig.IsExport = true;
		CallSourceExportConfig.ExportExt = _T("cpp");
		HandlerHeaderExportConfig.IsExport = true;
		HandlerHeaderExportConfig.ExportExt = _T("h");
		HandlerSourceExportConfig.IsExport = true;
		HandlerSourceExportConfig.ExportExt = _T("cpp");
		DataObjectHeaderExportConfig.IsExport = true;
		DataObjectHeaderExportConfig.ExportExt = _T("h");
		DataObjectSourceExportConfig.IsExport = true;
		DataObjectSourceExportConfig.ExportExt = _T("cpp");
		DataObjectModifyFlagsHeaderExportConfig.IsExport = true;
		DataObjectModifyFlagsHeaderExportConfig.ExportExt = _T("h");
		DataObjectModifyFlagsSourceExportConfig.IsExport = true;
		DataObjectModifyFlagsSourceExportConfig.ExportExt = _T("cpp");		
		CommentPrefix = _T("//");
		LocalVariableDefinePrefix = "";
		MemberVariablePrefix = "";
		DefaultPacketName = _T("Packet");
		DefaultPacketMemberName = _T("PacketMember");
		PrecompileHeaderFileName = _T("stdafx.h");
		SupportBigInt = true;
		ForceExportAll = false;
	}
};

struct RUN_ENV_INFO
{
	CString				RecentOpenPath;
	CString				RecentExportDir;
	vector<CString>		SelectedInterfaces;
};


void TabInitOperations(CTabCtrl& tbOperations);
void TabShowOperations(CTabCtrl& tbOperations, CEdit& edOperation, GENERATE_OPERATIONS_INFO& Operation);
void TabFetchOperations(CTabCtrl& tbOperations, CEdit& edOperation, GENERATE_OPERATIONS_INFO& Operation);


#include "DlgGenerateOperationsEditor.h"

#include "DlgTypeEditor.h"
#include "DlgConfig.h"
#include "CallInterfaceMakerDlg.h"

#include "CallInterfaceMakerDlg.h"
#include "CallInterfaceMaker.h"

#include "DlgParamEditor.h"
#include "DlgMethodEditor.h"
#include "DlgInterfaceEdtor.h"


#include "DlgTextEditor.h"

#include "DlgDataObjectModifyFlagEditor.h"
#include "DlgDataObjectModifyFlagManager.h"

#include "DlgStructMemberEditor.h"
#include "DlgStructEditor.h"
#include "DlgStructList.h"

#include "DlgEnumEditor.h"
#include "DlgEnumList.h"

#include "DlgConstList.h"

#include "DlgDataStructDefine.h"

#include "DlgListSelector.h"

#include "DlgImportEditor.h"

#include "DlgModuleSelector.h"



#include <afxcontrolbars.h>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


