/****************************************************************************/
/*                                                                          */
/*      文件名:    stdafx.cpp                                               */
/*      创建日期:  2011年10月07日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// stdafx.cpp : 只包括标准包含文件的源文件
// CallInterfaceMaker.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


void TabInitOperations(CTabCtrl& tbOperations)
{
	tbOperations.InsertItem(0, _T("长度计算操作"));
	tbOperations.InsertItem(1, _T("打包操作"));
	tbOperations.InsertItem(2, _T("解包操作"));
	tbOperations.InsertItem(3, _T("ToXML操作"));
	tbOperations.InsertItem(4, _T("FromXML操作"));
	tbOperations.InsertItem(5, _T("ToJson操作"));
	tbOperations.InsertItem(6, _T("FromJson操作"));
	tbOperations.InsertItem(7, _T("引用定义方式"));
	tbOperations.InsertItem(8, _T("引用使用方式"));
	tbOperations.InsertItem(9, _T("变量定义"));
	tbOperations.InsertItem(10, _T("初始化操作"));
	tbOperations.InsertItem(11, _T("复制操作"));
	tbOperations.InsertItem(12, _T("Get方法声明"));
	tbOperations.InsertItem(13, _T("Get方法实现"));
	tbOperations.InsertItem(14, _T("Set方法声明"));
	tbOperations.InsertItem(15, _T("Set方法实现"));
	tbOperations.InsertItem(16, _T("索引操作"));
	tbOperations.InsertItem(17, _T("只读索引操作"));
	tbOperations.InsertItem(18, _T("字符串化操作"));
	tbOperations.InsertItem(19, _T("数据库字段定义"));
	tbOperations.InsertItem(20, _T("数据库格式化符"));
	tbOperations.InsertItem(21, _T("数据库写入操作"));
	tbOperations.InsertItem(22, _T("数据库读取操作"));
	tbOperations.InsertItem(23, _T("文件日志填充操作"));
	//tbOperations.InsertItem(24, _T("PropertyGrid获取操作"));
	tbOperations.InsertItem(24, _T("阿里日志发送操作"));
	tbOperations.InsertItem(25, _T("ToLua操作"));
	tbOperations.InsertItem(26, _T("FromLua操作"));
	tbOperations.InsertItem(27, _T("创建XLS表头操作"));
	tbOperations.InsertItem(28, _T("检查XLS表头操作"));
	tbOperations.InsertItem(29, _T("ToXLS操作"));
	tbOperations.InsertItem(30, _T("FromXLS操作"));
	tbOperations.InsertItem(31, _T("格式化占位符"));
}
void TabShowOperations(CTabCtrl& tbOperations, CEdit& edOperation, GENERATE_OPERATIONS_INFO& Operation)
{
	switch (tbOperations.GetCurSel())
	{
	case 0:
		edOperation.SetWindowText(Operation.SizeCaculateOperation);
		break;
	case 1:
		edOperation.SetWindowText(Operation.PackOperation);
		break;
	case 2:
		edOperation.SetWindowText(Operation.UnpackOperation);
		break;
	case 3:
		edOperation.SetWindowText(Operation.ToXMLOperation);
		break;
	case 4:
		edOperation.SetWindowText(Operation.FromXMLOperation);
		break;
	case 5:
		edOperation.SetWindowText(Operation.ToJsonOperation);
		break;
	case 6:
		edOperation.SetWindowText(Operation.FromJsonOperation);
		break;
	case 7:
		edOperation.SetWindowText(Operation.ReferenceDefine);
		break;
	case 8:
		edOperation.SetWindowText(Operation.ReferenceUse);
		break;
	case 9:
		edOperation.SetWindowText(Operation.VariableDefine);
		break;
	case 10:
		edOperation.SetWindowText(Operation.InitOperation);
		break;
	case 11:
		edOperation.SetWindowText(Operation.CloneOperation);
		break;
	case 12:
		edOperation.SetWindowText(Operation.GetMethodDeclare);
		break;
	case 13:
		edOperation.SetWindowText(Operation.GetMethodDefine);
		break;
	case 14:
		edOperation.SetWindowText(Operation.SetMethodDeclare);
		break;
	case 15:
		edOperation.SetWindowText(Operation.SetMethodDefine);
		break;
	case 16:
		edOperation.SetWindowText(Operation.IndexOperation);
		break;
	case 17:
		edOperation.SetWindowText(Operation.ConstIndexOperation);
		break;
	case 18:
		edOperation.SetWindowText(Operation.ToStringOperation);
		break;
	case 19:
		edOperation.SetWindowText(Operation.DBFieldDefineOperation);
		break;
	case 20:
		edOperation.SetWindowText(Operation.DBInsertFormatOperation);
		break;
	case 21:
		edOperation.SetWindowText(Operation.DBPutOperation);
		break;
	case 22:
		edOperation.SetWindowText(Operation.DBGetOperation);
		break;
	case 23:
		edOperation.SetWindowText(Operation.FileLogFillOperation);
		break;
	//case 24:
	//	edOperation.SetWindowText(Operation.PropertyGridFetchOperation);
	//	break;
	case 24:
		edOperation.SetWindowText(Operation.AliLogSendOperation);
		break;
	case 25:
		edOperation.SetWindowText(Operation.ToLuaOperation);
		break;
	case 26:
		edOperation.SetWindowText(Operation.FromLuaOperation);
		break;
	case 27:
		edOperation.SetWindowText(Operation.CreateXLSColumnOperation);
		break;
	case 28:
		edOperation.SetWindowText(Operation.CheckXLSColumnOperation);
		break;
	case 29:
		edOperation.SetWindowText(Operation.ToXLSOperation);
		break;
	case 30:
		edOperation.SetWindowText(Operation.FromXLSOperation);
		break;
	case 31:
		edOperation.SetWindowText(Operation.FormatSpecOperation);
		break;
	}
}
void TabFetchOperations(CTabCtrl& tbOperations, CEdit& edOperation, GENERATE_OPERATIONS_INFO& Operation)
{
	switch (tbOperations.GetCurSel())
	{
	case 0:
		edOperation.GetWindowText(Operation.SizeCaculateOperation);
		break;
	case 1:
		edOperation.GetWindowText(Operation.PackOperation);
		break;
	case 2:
		edOperation.GetWindowText(Operation.UnpackOperation);
		break;
	case 3:
		edOperation.GetWindowText(Operation.ToXMLOperation);
		break;
	case 4:
		edOperation.GetWindowText(Operation.FromXMLOperation);
		break;
	case 5:
		edOperation.GetWindowText(Operation.ToJsonOperation);
		break;
	case 6:
		edOperation.GetWindowText(Operation.FromJsonOperation);
		break;
	case 7:
		edOperation.GetWindowText(Operation.ReferenceDefine);
		break;
	case 8:
		edOperation.GetWindowText(Operation.ReferenceUse);
		break;
	case 9:
		edOperation.GetWindowText(Operation.VariableDefine);
		break;
	case 10:
		edOperation.GetWindowText(Operation.InitOperation);
		break;
	case 11:
		edOperation.GetWindowText(Operation.CloneOperation);
		break;
	case 12:
		edOperation.GetWindowText(Operation.GetMethodDeclare);
		break;
	case 13:
		edOperation.GetWindowText(Operation.GetMethodDefine);
		break;
	case 14:
		edOperation.GetWindowText(Operation.SetMethodDeclare);
		break;
	case 15:
		edOperation.GetWindowText(Operation.SetMethodDefine);
		break;
	case 16:
		edOperation.GetWindowText(Operation.IndexOperation);
		break;
	case 17:
		edOperation.GetWindowText(Operation.ConstIndexOperation);
		break;
	case 18:
		edOperation.GetWindowText(Operation.ToStringOperation);
		break;
	case 19:
		edOperation.GetWindowText(Operation.DBFieldDefineOperation);
		break;
	case 20:
		edOperation.GetWindowText(Operation.DBInsertFormatOperation);
		break;
	case 21:
		edOperation.GetWindowText(Operation.DBPutOperation);
		break;
	case 22:
		edOperation.GetWindowText(Operation.DBGetOperation);
		break;
	case 23:
		edOperation.GetWindowText(Operation.FileLogFillOperation);
		break;
	//case 24:
	//	edOperation.GetWindowText(Operation.PropertyGridFetchOperation);
	//	break;
	case 24:
		edOperation.GetWindowText(Operation.AliLogSendOperation);
		break;
	case 25:
		edOperation.GetWindowText(Operation.ToLuaOperation);
		break;
	case 26:
		edOperation.GetWindowText(Operation.FromLuaOperation);
		break;
	case 27:
		edOperation.GetWindowText(Operation.CreateXLSColumnOperation);
		break;
	case 28:
		edOperation.GetWindowText(Operation.CheckXLSColumnOperation);
		break;
	case 29:
		edOperation.GetWindowText(Operation.ToXLSOperation);
		break;
	case 30:
		edOperation.GetWindowText(Operation.FromXLSOperation);
		break;
	case 31:
		edOperation.GetWindowText(Operation.FormatSpecOperation);
		break;
	}
}