/****************************************************************************/
/*                                                                          */
/*      �ļ���:    stdafx.cpp                                               */
/*      ��������:  2011��10��07��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// CallInterfaceMaker.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


void TabInitOperations(CTabCtrl& tbOperations)
{
	tbOperations.InsertItem(0, _T("���ȼ������"));
	tbOperations.InsertItem(1, _T("�������"));
	tbOperations.InsertItem(2, _T("�������"));
	tbOperations.InsertItem(3, _T("ToXML����"));
	tbOperations.InsertItem(4, _T("FromXML����"));
	tbOperations.InsertItem(5, _T("ToJson����"));
	tbOperations.InsertItem(6, _T("FromJson����"));
	tbOperations.InsertItem(7, _T("���ö��巽ʽ"));
	tbOperations.InsertItem(8, _T("����ʹ�÷�ʽ"));
	tbOperations.InsertItem(9, _T("��������"));
	tbOperations.InsertItem(10, _T("��ʼ������"));
	tbOperations.InsertItem(11, _T("���Ʋ���"));
	tbOperations.InsertItem(12, _T("Get��������"));
	tbOperations.InsertItem(13, _T("Get����ʵ��"));
	tbOperations.InsertItem(14, _T("Set��������"));
	tbOperations.InsertItem(15, _T("Set����ʵ��"));
	tbOperations.InsertItem(16, _T("��������"));
	tbOperations.InsertItem(17, _T("ֻ����������"));
	tbOperations.InsertItem(18, _T("�ַ���������"));
	tbOperations.InsertItem(19, _T("���ݿ��ֶζ���"));
	tbOperations.InsertItem(20, _T("���ݿ��ʽ����"));
	tbOperations.InsertItem(21, _T("���ݿ�д�����"));
	tbOperations.InsertItem(22, _T("���ݿ��ȡ����"));
	tbOperations.InsertItem(23, _T("�ļ���־������"));
	//tbOperations.InsertItem(24, _T("PropertyGrid��ȡ����"));
	tbOperations.InsertItem(24, _T("������־���Ͳ���"));
	tbOperations.InsertItem(25, _T("ToLua����"));
	tbOperations.InsertItem(26, _T("FromLua����"));
	tbOperations.InsertItem(27, _T("����XLS��ͷ����"));
	tbOperations.InsertItem(28, _T("���XLS��ͷ����"));
	tbOperations.InsertItem(29, _T("ToXLS����"));
	tbOperations.InsertItem(30, _T("FromXLS����"));
	tbOperations.InsertItem(31, _T("��ʽ��ռλ��"));
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