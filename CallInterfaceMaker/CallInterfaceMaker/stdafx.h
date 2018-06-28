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

#define CONFIG_FILE_NAME									"Config.xml"
#define ENV_FILE_NAME										"Env.xml"
#define TYPE_DEF_FILE_NAME									"TypeDef.xml"

#define INTERFACE_HEADER_TEMPLE_FILE_NAME					"InterfaceHeader.tmpl"
#define INTERFACE_TEMPLE_FILE_NAME							"Interface.tmpl"

#define PARAM_TEMPLE_FILE_NAME								"Param.tmpl"
#define PARAM_DESCRIPTION_TEMPLE_FILE_NAME					"ParamDescript.tmpl"

#define PACK_SIZE_TEMPLE_FILE_NAME							"PackSize.tmpl"

#define METHOD_HEADER_TEMPLE_FILE_NAME						"MethodHeader.tmpl"
#define METHOD_HEADER_PURE_TAIL_TEMPLE_FILE_NAME			"MethodHeaderPureTail.tmpl"
#define METHOD_CALLER_SOURCE_TEMPLE_FILE_NAME				"MethodCallerSource.tmpl"
#define MSG_PACK_METHOD_HEADER_TEMPLE_FILE_NAME 			"MsgPackMethodHeader.tmpl"
#define MSG_PACK_METHOD_SOURCE_TEMPLE_FILE_NAME 			"MsgPackMethodSource.tmpl"
#define MSG_HANDLER_METHOD_HEADER_TEMPLE_FILE_NAME 			"MsgHandlerMethodHeader.tmpl"
#define MSG_HANDLER_METHOD_SOURCE_TEMPLE_FILE_NAME 			"MsgHandlerMethodSource.tmpl"

#define MSG_MAP_INIT_TEMPLE_FILE_NAME						"MsgMapInit.tmpl"


#define MSG_CALLER_HEADER_TEMPLE_FILE_NAME					"MsgCallerHeader.tmpl"
#define MSG_CALLER_SOURCE_TEMPLE_FILE_NAME					"MsgCallerSource.tmpl"


#define MSG_HANDLER_HEADER_TEMPLE_FILE_NAME					"MsgHandlerHeader.tmpl"
#define MSG_HANDLER_SOURCE_TEMPLE_FILE_NAME					"MsgHandlerSource.tmpl"

#define DATA_OBJECT_DEFINE_HEADER_TEMPLE_FILE_NAME			"DataObjectDefineHeader.tmpl"
#define DATA_OBJECT_DEFINE_SOURCE_TEMPLE_FILE_NAME			"DataObjectDefineSource.tmpl"

#define STRUCT_DEFINE_HEADER_TEMPLE_FILE_NAME				"StructDefineHeader.tmpl"
#define ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME					"EnumDefineHeader.tmpl"
#define SYSTEM_ENUM_DEFINE_HEADER_TEMPLE_FILE_NAME			"SystemEnumDefineHeader.tmpl"
#define STRUCT_DEFINE_TEMPLE_FILE_NAME						"StructDefine.tmpl"
#define STRUCT_MEMBER_TEMPLE_FILE_NAME						"StructMember.tmpl"
#define DATA_OBJECT_MEMBER_TEMPLE_FILE_NAME					"DataObjectMember.tmpl"

#define DATA_OBJECT_MODIFY_FLAGS_TEMPLE_FILE_NAME			"DataObjectModifyFlags.tmpl"
#define DATA_OBJECT_MODIFY_FLAG_DEFINE_TEMPLE_FILE_NAME		"DataObjectModifyFlagDefine.tmpl"
#define DATA_OBJECT_MODIFY_FLAG_UNIT_TEMPLE_FILE_NAME		"DataObjectModifyFlagUnit.tmpl"

#define PACK_OPERATION_UNIT_TEMPLE_FILE_NAME				"PackOperationUnit.tmpl"
#define UNPACK_OPERATION_UNIT_TEMPLE_FILE_NAME				"UnpackOperationUnit.tmpl"

#define TO_XML_OPERATION_UNIT_TEMPLE_FILE_NAME				"ToXMLOperationUnit.tmpl"
#define FROM_XML_OPERATION_UNIT_TEMPLE_FILE_NAME			"FromXMLOperationUnit.tmpl"
#define STRUCT_XML_PROCESS_TEMPLE_FILE_NAME					"StructXMLProcess.tmpl"

#define TO_JSON_OPERATION_UNIT_TEMPLE_FILE_NAME				"ToJsonOperationUnit.tmpl"
#define FROM_JSON_OPERATION_UNIT_TEMPLE_FILE_NAME			"FromJsonOperationUnit.tmpl"
#define STRUCT_JSON_PROCESS_TEMPLE_FILE_NAME				"StructJsonProcess.tmpl"

#define ENUM_DFINE_TEMPLE_FILE_NAME							"EnumDefine.tmpl"
#define ENUM_MEMBER_DFINE_TEMPLE_FILE_NAME					"EnumMemberDefine.tmpl"
#define ENUM_STR_VALUE_DFINE_TEMPLE_FILE_NAME				"EnumStrValueDefine.tmpl"

#define CONST_DFINE_TEMPLE_FILE_NAME						"ConstDefine.tmpl"
#define MACRO_DFINE_TEMPLE_FILE_NAME						"MacroDefine.tmpl"
#define CONST_DFINE_HEADER_TEMPLE_FILE_NAME					"ConstDefineHeader.tmpl"

#define STRING_CONST_DFINE_TEMPLE_FILE_NAME					"StringConstDefine.tmpl"

#define USE_CRT_DETAIL_NEW

#include "..\..\Libs\Utils\Utils.h"

#include "resource.h"

#include <vector>
#include <fstream>

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

enum INTERFACE_METHOD_PARAM_TYPE
{
	INTERFACE_METHOD_PARAM_TYPE_CALL,
	INTERFACE_METHOD_PARAM_TYPE_ACK,
};

const LPCTSTR g_szINTERFACE_METHOD_TYPE[] = { "调用方法", "通知方法" ,"无返回调用"};

enum STRUCT_FLAG
{
	STRUCT_FLAG_IS_DATA_OBJECT = 1,
	STRUCT_FLAG_DECLARE_LATER = (1<<1),
	STRUCT_FLAG_EXPORT_XML_PROCESS = (1<<2),
	STRUCT_FLAG_EXPORT_JSON_PROCESS = (1 << 3),
};

enum STRUCT_MEMBER_FLAG
{
	STRUCT_MEMBER_FLAG_NOT_MONITOR_UPDATE=1,
	STRUCT_MEMBER_FLAG_EXCLUDE_IN_PACKET=(1<<1),
	STRUCT_MEMBER_FLAG_MERGE_TO_PARENT=(1<<2),
	STRUCT_MEMBER_FLAG_MUST_PACK=(1<<3),
};

enum TYPE_DEFINE_FLAG
{
	TYPE_DEFINE_FLAG_STRUCT=1,
	TYPE_DEFINE_FLAG_DATA_OBJECT=(1<<1),
	TYPE_DEFINE_FLAG_REF_TYPE = (1 << 2),
	TYPE_DEFINE_FLAG_ENUM = (1 << 3),
	TYPE_DEFINE_FLAG_64BIT = (1 << 4),
};


enum ENUM_DEFINE_FLAG
{
	ENUM_DEFINE_FLAG_IS_64BIT=1,
	ENUM_DEFINE_FLAG_EXPORT_STR_VALUE=(1<<1),
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
	INTERFACE_METHOD_FLAG_CAN_CACHE=1,
};

const LPCTSTR g_szDATA_STRUCT_TYPE[DATA_STRUCT_MAX]={"常量","枚举","结构"};

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
	UINT	Flag;
	BOOL	IsArray;
	UINT	ArrayStartLength;
	UINT	ArrayGrowLength;
	CString	Description;
	UINT	ID;
	STRUCT_MEMBER_INFO()
	{
		Flag=0;
		IsArray=false;
		ArrayStartLength=16;
		ArrayGrowLength=8;
		ID=1;
	}
};

struct STRUCT_DEFINE_INFO_OLD
{
	CString						Name;
	CString						ShortName;
	CString						BaseStruct;
	vector<STRUCT_MEMBER_INFO>	MemberList;
	CString						Description;
	BOOL						IsDataObject;
	BOOL						DeclareLater;
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
		ObjectID=0;
		IDSeed=1;
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
	
};

struct ENUM_DEFINE_INFO
{
	CString						Name;
	CString						ShortName;	
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
	UINT						Flag;
	GENERATE_OPERATIONS_INFO	GenerateOperations;
	
	TYPE_DEFINE()
	{
		Flag=0;
	}
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
	CString		Description;
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
	BOOL		IsExport;
	CString		ExportExt;
};

struct INTERFACE_CONFIG
{
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
	EXPORT_CONFIG				DataObjectModifyFlagsExportConfig;
	GENERATE_OPERATIONS_INFO	DefaultStructGenerateOperations;	
	GENERATE_OPERATIONS_INFO	DefaultEnumGenerateOperations;
	GENERATE_OPERATIONS_INFO	ArrayDefineConfig;
	INTERFACE_CONFIG()
	{
		InterfaceHeaderExportConfig.IsExport=true;
		InterfaceHeaderExportConfig.ExportExt="h";
		StructExportConfig.IsExport=true;
		StructExportConfig.ExportExt="h";
		EnumExportConfig.IsExport=true;
		EnumExportConfig.ExportExt="h";
		ConstExportConfig.IsExport=true;
		ConstExportConfig.ExportExt="h";
		DataStructHeaderExportConfig.IsExport=true;
		DataStructHeaderExportConfig.ExportExt="h";
		InterfaceExportConfig.IsExport=true;
		InterfaceExportConfig.ExportExt="h";
		CallHeaderExportConfig.IsExport=true;
		CallHeaderExportConfig.ExportExt="h";
		CallSourceExportConfig.IsExport=true;
		CallSourceExportConfig.ExportExt="cpp";
		HandlerHeaderExportConfig.IsExport=true;
		HandlerHeaderExportConfig.ExportExt="h";
		HandlerSourceExportConfig.IsExport=true;
		HandlerSourceExportConfig.ExportExt="cpp";
		DataObjectHeaderExportConfig.IsExport=true;
		DataObjectHeaderExportConfig.ExportExt="h";
		DataObjectSourceExportConfig.IsExport=true;
		DataObjectSourceExportConfig.ExportExt="cpp";
		DataObjectModifyFlagsExportConfig.IsExport=true;
		DataObjectModifyFlagsExportConfig.ExportExt="h";
		DefaultStructGenerateOperations.PackOperation=
			"<IfMFCheck>if(Flag&<ModifyFlag>)</IfMFCheck>\r\n"
			"{\r\n"
			"	UINT BufferSize;\r\n"
			"	void * pBuffer=Packet.PrepareMember(BufferSize);\r\n"
			"	CSmartStruct SubPacket(pBuffer,BufferSize,true);\r\n"
			"	if(!<Variable>.MakePacket(SubPacket)) FailCount++;\r\n"
			"	Packet.FinishMember(<SST_ID>,SubPacket.GetDataLen());\r\n"
			"}\r\n";			
			"</IfNotMFCheck>\r\n";
		DefaultStructGenerateOperations.UnpackOperation=
			"case <SST_ID>:\r\n"
			"	<IfMonitorUpdate>if(Flag&<ModifyFlag>)</IfMonitorUpdate>\r\n"
			"	{\r\n"
			"		<Variable>.ParsePacket(Value);\r\n"
			"		<IfMonitorUpdate>UpdateFlag|=<ModifyFlag>;</IfMonitorUpdate>\r\n"
			"	}\r\n"
			"	break;";
		DefaultStructGenerateOperations.SizeCaculateOperation=			
			"CSmartStruct::GetStructMemberSize(<Variable>.GetSmartStructSize()+CSmartStruct::GetEmptyStructSize())";
		DefaultStructGenerateOperations.ReferenceDefine="<Type>&";
		DefaultStructGenerateOperations.ReferenceUse="<Variable>";
		DefaultStructGenerateOperations.VariableDefine="<Type><Space><Variable>;";
		DefaultStructGenerateOperations.InitOperation="<Variable>.Clear();";

		ArrayDefineConfig.PackOperation="";
		ArrayDefineConfig.UnpackOperation="";
		ArrayDefineConfig.SizeCaculateOperation="";
		ArrayDefineConfig.ReferenceDefine="<Type>&";
		ArrayDefineConfig.ReferenceUse="<Variable>";
		ArrayDefineConfig.VariableDefine="CEasyArray<<Type>><Space><Variable>;";
		ArrayDefineConfig.InitOperation="<Variable>.Clear();";
		ArrayDefineConfig.CloneOperation="";
		ArrayDefineConfig.GetMethodDeclare="";
		ArrayDefineConfig.GetMethodDefine="";
		ArrayDefineConfig.SetMethodDeclare="";
		ArrayDefineConfig.SetMethodDefine="";
	}
};

struct RUN_ENV_INFO
{
	CString				RecentDataDir;
	CString				RecentExportDir;
	vector<CString>		SelectedInterfaces;
};





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


