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
	tbOperations.InsertItem(0, "���ȼ������");
	tbOperations.InsertItem(1, "�������");
	tbOperations.InsertItem(2, "�������");
	tbOperations.InsertItem(3, "ToXML����");
	tbOperations.InsertItem(4, "FromXML����");
	tbOperations.InsertItem(5, "ToJson����");
	tbOperations.InsertItem(6, "FromJson����");
	tbOperations.InsertItem(7, "���ö��巽ʽ");
	tbOperations.InsertItem(8, "����ʹ�÷�ʽ");
	tbOperations.InsertItem(9, "��������");
	tbOperations.InsertItem(10, "��ʼ������");
	tbOperations.InsertItem(11, "���Ʋ���");
	tbOperations.InsertItem(12, "Get��������");
	tbOperations.InsertItem(13, "Get����ʵ��");
	tbOperations.InsertItem(14, "Set��������");
	tbOperations.InsertItem(15, "Set����ʵ��");
	tbOperations.InsertItem(16, "��������");
	tbOperations.InsertItem(17, "ֻ����������");
	tbOperations.InsertItem(18, "�ַ���������");
	tbOperations.InsertItem(19, "���ݿ��ֶζ���");
	tbOperations.InsertItem(20, "���ݿ��ʽ����");
	tbOperations.InsertItem(21, "���ݿ�д�����");
	tbOperations.InsertItem(22, "���ݿ��ȡ����");
	tbOperations.InsertItem(23, "PropertyGrid������");
	//tbOperations.InsertItem(24, "PropertyGrid��ȡ����");
	tbOperations.InsertItem(24, "��־���Ͳ���");
	tbOperations.InsertItem(25, "ToLua����");
	tbOperations.InsertItem(26, "FromLua����");
	tbOperations.InsertItem(27, "����XLS��ͷ����");
	tbOperations.InsertItem(28, "���XLS��ͷ����");
	tbOperations.InsertItem(29, "ToXLS����");
	tbOperations.InsertItem(30, "FromXLS����");
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
		edOperation.SetWindowText(Operation.PropertyGridFillOperation);
		break;
	//case 24:
	//	edOperation.SetWindowText(Operation.PropertyGridFetchOperation);
	//	break;
	case 24:
		edOperation.SetWindowText(Operation.LogSendOperation);
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
		edOperation.GetWindowText(Operation.PropertyGridFillOperation);
		break;
	//case 24:
	//	edOperation.GetWindowText(Operation.PropertyGridFetchOperation);
	//	break;
	case 24:
		edOperation.GetWindowText(Operation.LogSendOperation);
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
	}
}