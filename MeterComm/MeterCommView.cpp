
// MeterCommView.cpp : CMeterCommView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MeterComm.h"
#endif

//#include "MeterCommDoc.h"
#include "MeterCommView.h"
#include "ComPortSetting.h"
#include "ProduceTesting.h"
#include "ParamSetting.h"
#include "NetWorkSetting.h"
#include "DataItem.h"
#include "MeterTesting.h"
#include "SoftwareUpdate.h"
#include "DIDataBase.h"
#include "CCardTesting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMeterCommView


IMPLEMENT_DYNCREATE(CMeterCommView, CFormView)

BEGIN_MESSAGE_MAP(CMeterCommView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_GETADDR, &CMeterCommView::OnBnClickedButtonGetaddr)
	ON_BN_CLICKED(IDC_BUTTON_GETDATAITEM, &CMeterCommView::OnBnClickedButtonGetdataitem)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMeterCommView::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_GETESAMID, &CMeterCommView::OnBnClickedButtonGetesamid)
	ON_BN_CLICKED(IDC_BUTTON_CLEARDATA, &CMeterCommView::OnBnClickedButtonCleardata)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMeterCommView::OnBnClickedButtonStop)
	ON_COMMAND(ID_SETTING_COMPORT, &CMeterCommView::OnSettingComport)
	ON_COMMAND(ID_SETTING_PARAMETER, &CMeterCommView::OnSettingParameter)
	ON_COMMAND(ID_SETTING_NETWORK, &CMeterCommView::OnSettingNetwork)
	ON_COMMAND(ID_TESTING_PRODUCE, &CMeterCommView::OnTestingProduce)
	ON_COMMAND(ID_TESTING_METER, &CMeterCommView::OnTestingMeter)
	ON_COMMAND(ID_TESTING_CARD, &CMeterCommView::OnTestingCard)
	ON_BN_CLICKED(IDC_BUTTON_GETIDPARAM, &CMeterCommView::OnBnClickedButtonGetidparam)
	ON_WM_SIZE()
	ON_MESSAGE(WM_MSGRECVPRO, &CMeterCommView::OnMsgrecvpro)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_CLICK, IDC_LIST_DISPLAY, &CMeterCommView::OnNMClickListDisplay)
	ON_COMMAND(ID_HELPING_UPDATE, &CMeterCommView::OnHelpingUpdate)
	ON_COMMAND(ID_SETTING_DIDATABASE, &CMeterCommView::OnSettingDidatabase)
	ON_CBN_SELCHANGE(IDC_COMBO_PROTOCOL, &CMeterCommView::OnCbnSelchangeComboProtocol)
END_MESSAGE_MAP()

// CMeterCommView ����/����

CMeterCommView::CMeterCommView()
	: CFormView(CMeterCommView::IDD)
	, m_nListCount(0)
	, m_p_comportset(NULL)
	, m_p_protest(NULL)
	, m_p_metertest(NULL)
	, m_hEvtExitCheckUpdate(NULL)
	, m_hEvtExitUpdateDIDB(NULL)
	, m_bIsInitFinish(false)
	, m_nDllSel(1)
{
	// TODO: �ڴ˴���ӹ������
	ZDLT645_2007::CallExDll();
	ZDLT698_45::CallExDll();
	ReadGlobalVariable();
}

CMeterCommView::~CMeterCommView()
{
	WriteGlobalVariable();
	WaitForSingleObject(m_hEvtExitCheckUpdate, INFINITE);
	WaitForSingleObject(m_hEvtExitUpdateDIDB, INFINITE);
	if(m_hEvtExitCheckUpdate!=NULL)
	{
		CloseHandle(m_hEvtExitCheckUpdate);
		m_hEvtExitCheckUpdate=NULL;
	}
	if(m_hEvtExitUpdateDIDB!=NULL)
	{
		CloseHandle(m_hEvtExitUpdateDIDB);
		m_hEvtExitUpdateDIDB=NULL;
	}
	if(m_p_comportset!=NULL)
	{
		delete m_p_comportset;
		m_p_comportset=NULL;
	}
	if(m_p_protest!=NULL)
	{
		delete m_p_protest;
		m_p_protest=NULL;
	}
	if(m_p_metertest!=NULL)
	{
		delete m_p_metertest;
		m_p_metertest=NULL;
	}
	ZDLT698_45::UncallExDll();
	ZDLT645_2007::UncallExDll();
}

void CMeterCommView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DISPLAY, m_listDisplay);
	DDX_Control(pDX, IDC_BUTTON_GETADDR, m_btnGetAddr);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_btnSend);
	DDX_Control(pDX, IDC_BUTTON_GETESAMID, m_btnGetEsamID);
	DDX_Control(pDX, IDC_COMBO_FUNCCODE, m_comboFuncCode);
	DDX_Control(pDX, IDC_BUTTON_GETDATAITEM, m_btnGetDataItem);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTTON_GETIDPARAM, m_btnGetMeterNO);
	DDX_Control(pDX, IDC_BUTTON_CLEARDATA, m_btnClearData);
	DDX_Control(pDX, IDC_COMBO_PROTOCOL, m_comboProtocol);
}

BOOL CMeterCommView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

void CMeterCommView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	//��Ӷ����ʼ��
	g_sz_p_wnd[WND_METERCOMM]=this;
	InitOperatorArea();
	InitDisplayArea();
	InitButton();
	AddClipSiblings();
	SetDlgItemText(IDC_EDIT_SENDTIMES,_T("1"));
	m_hEvtExitCheckUpdate =CreateEvent(NULL,TRUE,TRUE,NULL);
	while(AfxBeginThread(CheckSoftwareUpdateThreadFunc,(LPVOID)this)==NULL);
	m_hEvtExitUpdateDIDB =CreateEvent(NULL,TRUE,TRUE,NULL);
	while(AfxBeginThread(UpdateDIDBThreadFunc,(LPVOID)this)==NULL);
	m_bIsInitFinish=true;
}

// CMeterCommView ���

#ifdef _DEBUG
void CMeterCommView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMeterCommView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMeterCommDoc* CMeterCommView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMeterCommDoc)));
	return (CMeterCommDoc*)m_pDocument;
}
#endif //_DEBUG


// CMeterCommView ��Ϣ�������

void CMeterCommView::OnBnClickedButtonGetaddr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!g_nTotalPort)
	{
		UpdateEdit(false,_T("����δ��!"),NULL);
		return;
	}
	switch(g_v.g_nProtocolType%10)
	{
	case CMainData::PROTOCOL_DLT645:
		{
			g_v.g_strDeviceAddr=_T("AAAAAAAAAAAA");
			g_v.g_strFuncCode=_T("13");
		}
		break;
	case CMainData::PROTOCOL_DLT698:
		{
			g_v.g_strDeviceAddr=_T("AAAAAAAAAAAA");
			g_v.g_strFuncCode=_T("0501");
			g_v.g_strDataItem=_T("40010200");
		}
		break;
	default:
		return;
	}
	Start();
}


void CMeterCommView::OnBnClickedButtonGetdataitem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataItem dataitem;
	if(IDOK==dataitem.DoModal())
	{
		SetDlgItemText(IDC_EDIT_DATAITEM,dataitem.GetDataItem());
	}
}


void CMeterCommView::OnBnClickedButtonSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!g_nTotalPort)
	{
		UpdateEdit(false,_T("����δ��!"),NULL);
		return;
	}
	GetDlgItemText(IDC_EDIT_DEVICEADDR,g_v.g_strDeviceAddr);
	GetDlgItemText(IDC_COMBO_FUNCCODE,g_v.g_strFuncCode);
	if(g_v.g_strFuncCode.Find(_T('H'))!=-1)
		g_v.g_strFuncCode=g_v.g_strFuncCode.Left(g_v.g_strFuncCode.Find(_T('H')));
	GetDlgItemText(IDC_EDIT_DATAITEM,g_v.g_strDataItem);
	GetDlgItemText(IDC_EDIT_TARGETDATA,g_v.g_strTgtData);
	Start();
}

void CMeterCommView::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_bIsStop=true;
	for(int i=0;i<g_nTotalPort;++i)
	{
		ResetEvent(g_vec_md[i].m_hEvtThread);
		g_vec_md[i].m_p_zdp->StopComm();
	}
	for(int i=0;i<g_nTotalPort;++i)
	{
		WaitForSingleObject(g_vec_md[i].m_hEvtStop, INFINITE);
	}
	if(g_nIsUsingMD)
	{
		ResetEvent(g_mdMeterDevice.m_hEvtThread);
		g_mdMeterDevice.m_p_zdp->StopComm();
		WaitForSingleObject(g_mdMeterDevice.m_hEvtStop, INFINITE);
	}
	for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
	{
		if(g_sz_p_wnd[i])
			g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,TRUE,MSGUSER_ENABLEBTN);
	}
}

void CMeterCommView::OnBnClickedButtonGetidparam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!g_nTotalPort)
	{
		UpdateEdit(false,_T("����δ��!"),NULL);
		return;
	}
	switch(g_v.g_nProtocolType%10)
	{
	case CMainData::PROTOCOL_DLT645:
		{
			g_v.g_strDeviceAddr=_T("");
			g_v.g_strFuncCode=_T("11");
			g_v.g_strDataItem=_T("04000402");
		}
		break;
	case CMainData::PROTOCOL_DLT698:
		{
			g_v.g_strDeviceAddr=_T("");
			g_v.g_strFuncCode=_T("0502");
			g_v.g_strDataItem=_T("40010200#F1000400#40020200#F1000200#F1000701");
		}
		break;
	default:
		return;
	}
	Start();
}

void CMeterCommView::OnBnClickedButtonGetesamid()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!g_nTotalPort)
	{
		UpdateEdit(false,_T("����δ��!"),NULL);
		return;
	}
	switch(g_v.g_nProtocolType%10)
	{
	case CMainData::PROTOCOL_DLT645:
		{
			g_v.g_strDeviceAddr=_T("");
			g_v.g_strFuncCode=_T("03");
			g_v.g_strDataItem=_T("070000FF");
		}
		break;
	case CMainData::PROTOCOL_DLT698:
		{
			g_v.g_strDeviceAddr=_T("");
			g_v.g_strFuncCode=_T("02");
			g_v.g_strTgtData=_T("0014FFFFFFFFC0000000FFFEC400000000000000000000000000020002000107D000001C2002");
		}
		break;
	default:
		return;
	}
	Start();
}


void CMeterCommView::OnBnClickedButtonCleardata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listDisplay.DeleteAllItems();
	m_nListCount=0;
}


void CMeterCommView::Start(void)
{
	for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
	{
		if(g_sz_p_wnd[i])
			g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,FALSE,MSGUSER_ENABLEBTN);
	}
	int nTimes=GetDlgItemInt(IDC_EDIT_SENDTIMES);
	for(int i=0;i<g_nTotalPort;++i)
	{
		g_vec_md[i].m_nProtocolType=g_v.g_nProtocolType;
		switch(g_vec_md[i].m_nProtocolType%10)
		{
		case CMainData::PROTOCOL_DLT645:
			{
				g_vec_md[i].m_p_protl=g_vec_md[i].m_p_dlt645;
				g_vec_md[i].m_pf_checkcall=ZDLT645_2007::Check645_2007;
			}
			break;
		case CMainData::PROTOCOL_RMT:
			{
				g_vec_md[i].m_p_protl=g_vec_md[i].m_p_rmtProtl;
				g_vec_md[i].m_pf_checkcall=ZRMTProtocol::CheckRMTProtocol;
			}
			break;
		case CMainData::PROTOCOL_DLT698:
			{
				g_vec_md[i].m_p_protl=g_vec_md[i].m_p_dlt698;
				g_vec_md[i].m_pf_checkcall=ZDLT698_45::Check698_45;
			}
			break;
		}
		g_vec_md[i].m_p_WndOfSource=this;
		g_vec_md[i].m_nRow=0;
		g_vec_md[i].m_nRowMax=nTimes;
		SetEvent(g_vec_md[i].m_hEvtThread);
	}
	g_bIsStop=false;
}

void CMeterCommView::InitOperatorArea(void)
{
	CString sz_strProtocol[]={_T("DL/T645-2007"),_T("RMTProtocol"),_T("DL/T698.45")};
	for(int i=0;i<sizeof(sz_strProtocol)/sizeof(CString);++i)
		m_comboProtocol.AddString(sz_strProtocol[i]);
	m_comboProtocol.SetCurSel(CMainData::PROTOCOL_DLT645);//Ԥ��645Э��
	InitFuncCode(CMainData::PROTOCOL_DLT645);
	GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE); 
}

void CMeterCommView::InitDisplayArea(void)
{
	CRect rc;     
	m_listDisplay.GetClientRect(&rc);     
	m_listDisplay.SetExtendedStyle(m_listDisplay.GetExtendedStyle()|LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);     
	m_listDisplay.InsertColumn(0, _T("��־"), LVCFMT_LEFT, rc.Width()*6/40, 0);   
	m_listDisplay.InsertColumn(1, _T("Э��"), LVCFMT_LEFT, rc.Width()*6/40, 1);   
	m_listDisplay.InsertColumn(2, _T("����֡"), LVCFMT_LEFT, rc.Width()*21/40, 2);
	m_listDisplay.InsertColumn(3, _T("ʱ��"), LVCFMT_LEFT, rc.Width()*5/40, 3);
}

UINT CMeterCommView::PortCommThreadFunc(LPVOID lpParam)
{
	CMainData * p_md=(CMainData *)lpParam;
	CWnd * & p_MainDlg = p_md->m_p_MainDlg;
	CWnd * & p_SourceDlg=p_md->m_p_WndOfSource;
	std::vector<CString> vec_strData;
	ResetEvent(p_md->m_hEvtExit);
	while(p_md->m_bIsThreadRun)
	{
		WaitForSingleObject(p_md->m_hEvtThread, INFINITE);
		ResetEvent(p_md->m_hEvtStop);
		if(!(p_md->m_bIsThreadRun))
			break;
		if(p_md->m_nRow>=p_md->m_nRowMax)
		{
			SetEvent(p_md->m_hEvtStop);
			goto end;
		}
		p_md->m_nFlagGetSendData = FLAG_NONE;
		SendMessageTimeout(p_SourceDlg->m_hWnd,WM_MSGRECVPRO,(WPARAM)p_md,MSGUSER_GETSENDDATA,SMTO_BLOCK,500,NULL); 
		switch(p_md->m_nFlagGetSendData)
		{
		case FLAG_THREADWAIT:
		{
			DWORD dwStart = 0, dwSpan = 0;
			dwStart = ::GetTickCount();
			while (dwSpan < p_md->m_dwThreadTimeWait)
			{
				DWORD dwRtn = WaitForSingleObject(p_md->m_hEvtThread, 10);
				if (dwRtn != WAIT_TIMEOUT)
					Sleep(10);
				else
					break;
				if (!(p_md->m_bIsThreadRun))
					break;
				dwSpan = ::GetTickCount() - dwStart;
			}
			SetEvent(p_md->m_hEvtStop);
			goto end;
		}
		break;
		}
		if(!p_md->m_p_protl)
			goto end;
	encode:
		p_md->m_nRtn = p_md->m_p_protl->DataEncode(p_md->m_strDataSend, p_md->m_strError);
		SetEvent(p_md->m_hEvtStop);
		SendMessageTimeout(p_SourceDlg->m_hWnd,WM_MSGRECVPRO,(WPARAM)p_md,MSGUSER_ENCODE,SMTO_BLOCK,500,NULL); 
		if(p_md->m_nRtn)
			goto end;
		ZUtil::StrSplit(p_md->m_strDataSend,vec_strData,_T('#'), p_md->m_strDataSend.Right(1)==_T('#'));
		int nSize = vec_strData.size();
		for (int i = 0; i < nSize; ++i)
		{
			p_md->m_strDataSend = vec_strData[i];
		send:
			p_md->m_p_zdp->PurgePort();
			p_md->m_nRtn = p_md->m_p_zdp->SendToPort(p_md->m_strDataSend);
			if (!(p_md->m_bIsThreadRun))
				break;
			if (!p_md->m_nRtn)
				p_md->m_p_protl->DataDecode(p_md->m_strDataSend, p_md->m_strDataValue);
			SendMessageTimeout(p_SourceDlg->m_hWnd, WM_MSGRECVPRO, (WPARAM)p_md, MSGUSER_UPDATESENDDATA, SMTO_BLOCK, 500, NULL);
			if (p_SourceDlg != p_MainDlg)
				SendMessageTimeout(p_MainDlg->m_hWnd, WM_MSGRECVPRO, (WPARAM)p_md, MSGUSER_TRANSENDDATA, SMTO_BLOCK, 500, NULL);
			if (p_md->m_nRtn)
				goto end;
		recv:
			p_md->m_nFlagUpdateRecvData = FLAG_NONE;
			p_md->m_nRtn = p_md->m_p_zdp->RecvFromPort(p_md->m_strDataRecv, p_md->m_pf_checkcall);
			if (!(p_md->m_bIsThreadRun))
				break;
			if (!p_md->m_nRtn)
				p_md->m_p_protl->DataDecode(p_md->m_strDataRecv, p_md->m_strDataValue);
			SendMessageTimeout(p_SourceDlg->m_hWnd, WM_MSGRECVPRO, (WPARAM)p_md, MSGUSER_UPDATERECVDATA, SMTO_BLOCK, 500, NULL);
			if (p_SourceDlg != p_MainDlg)
				SendMessageTimeout(p_MainDlg->m_hWnd, WM_MSGRECVPRO, (WPARAM)p_md, MSGUSER_TRANRECVDATA, SMTO_BLOCK, 500, NULL);
			switch (p_md->m_nFlagUpdateRecvData)
			{
			case FLAG_GOTOENCODE:goto encode; break;
			case FLAG_GOTOSEND:goto send; break;
			case FLAG_GOTORECV:goto recv; break;
			}
		}
end:
		SendMessageTimeout(p_SourceDlg->m_hWnd,WM_MSGRECVPRO,(WPARAM)p_md,MSGUSER_ENDONCELOOP,SMTO_BLOCK,500,NULL);  
		++(p_md->m_nRow);
		if(p_md->m_nRow>=p_md->m_nRowMax)
			ResetEvent(p_md->m_hEvtThread);
	}
	SetEvent(p_md->m_hEvtExit);
	return 0;
}


afx_msg LRESULT CMeterCommView::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_GETSENDDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		switch (p_md->m_nProtocolType % 10)
		{
		case CMainData::PROTOCOL_DLT645:
		{
			p_md->m_p_dlt645->Set15Param(g_v.Get645Param());
		}
		break;
		case CMainData::PROTOCOL_RMT:
		{
			p_md->m_p_rmtProtl->InitParam(g_v.g_strFuncCode, g_v.g_strTgtData);
		}
		break;
		case CMainData::PROTOCOL_DLT698:
		{
			p_md->m_p_dlt698->Set16Param(g_v.Get698Param());
		}
		break;
		}
	}
	break;
	case MSGUSER_ENCODE:
	{
		CMainData * p_md = (CMainData *)wParam;
		switch (p_md->m_nRtn)
		{
		case 0:
		{
			SetDlgItemText(IDC_EDIT_LENGTH, p_md->m_p_protl->GetLength());
			SetDlgItemText(IDC_EDIT_SENDDATA, DataFormat(p_md->m_strDataSend));
		}
		break;
		default:
		{
			SetDlgItemText(IDC_EDIT_DETAIL, p_md->m_strError);
			SetDlgItemText(IDC_EDIT_CONTECT, p_md->m_strError);
		}
		}
	}
	break;
	case MSGUSER_UPDATESENDDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		CString strText;
		bool bRight = false;
		switch (p_md->m_nRtn)
		{
		case ZDevicePort::ERROR_OK:
		{
			UpdateList(_T("���͡�") + p_md->m_p_zdp->GetPort(), p_md->m_strDataSend, p_md);
			strText = p_md->m_strDataSend;
			bRight = true;
		}
		break;
		case ZDevicePort::ERROR_INVALIDPORT:
			strText = _T("����δ��!");
			break;
		case ZDevicePort::ERROR_SEND_TIMEOUT:
			strText = _T("���ڷ��ͳ�ʱ!");
			break;
		case ZDevicePort::ERROR_SEND_STOP:
			strText = _T("������ֹͣ!");
			break;
		default:
			strText = _T("���ڷ���ʧ��!");
		}
		UpdateEdit(bRight, strText, p_md);
	}
	break;
	case MSGUSER_TRANSENDDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		if (!p_md->m_nRtn)
			UpdateList(_T("���͡�") + p_md->m_p_zdp->GetPort(), p_md->m_strDataSend, p_md);
	}
	break;
	case MSGUSER_UPDATERECVDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		switch (p_md->m_nProtocolType % 10)
		{
		case CMainData::PROTOCOL_DLT645:
		{
			CString strText;
			bool bRight = false;
			switch (p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
			{
				UpdateList(_T("���ա�") + p_md->m_p_zdp->GetPort(), p_md->m_strDataRecv, p_md);
				strText = p_md->m_strDataRecv;
				bRight = true;
			}
			break;
			case ZDevicePort::ERROR_INVALIDPORT:
				strText = _T("����δ��!");
				break;
			case ZDevicePort::ERROR_RECV_TIMEOUT:
				strText = _T("���ڽ��ճ�ʱ!");
				break;
			case ZDevicePort::ERROR_RECV_STOP:
				strText = _T("������ֹͣ!");
				break;
			default:
				strText = _T("���ڽ���ʧ��!");
			}
			UpdateEdit(bRight, strText, p_md);
		}
		break;
		case CMainData::PROTOCOL_RMT:
		{
			CString strText;
			static DWORD dwNowTick = 0, dwEndTick = 0;
			bool bRight = false;
			switch (p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
			{
				UpdateList(_T("���ա�") + p_md->m_p_zdp->GetPort(), p_md->m_strDataRecv, p_md);
				CString strOrderType = p_md->m_p_rmtProtl->GetOrderType();
				if ((strOrderType == _T("20") || strOrderType == _T("33")) && p_md->m_strDataValue.Right(3) == _T("FFH"))
				{
					if (p_md->m_strDataValue.Left(4) == _T("0000"))
					{
						p_md->m_nFlagUpdateRecvData = FLAG_GOTORECV;
						dwNowTick = ::GetTickCount();
						if (strOrderType == _T("20"))
							dwEndTick = dwNowTick + DWORD(p_md->m_p_rmtProtl->GetMeterErrorWaitTime()*1000.0f);
						else
							dwEndTick = dwNowTick + DWORD(p_md->m_p_rmtProtl->GetClockErrorWaitTime()*1000.0f);
					}
					strText = p_md->m_strDataRecv;
					bRight = true;
				}
				else if ((strOrderType == _T("20") || strOrderType == _T("33")) && (p_md->m_strDataValue.Right(3) == _T("A0H") || p_md->m_strDataValue.Right(3) == _T("B1H")))
				{
					dwNowTick = ::GetTickCount();
					if (dwNowTick < dwEndTick)
					{
						p_md->m_nFlagUpdateRecvData = FLAG_GOTORECV;
						strText = p_md->m_strDataRecv;
						bRight = true;
					}
					else
					{
						strText = _T("���ڽ��ճ�ʱ!");
					}
				}
				else
				{
					strText = p_md->m_strDataRecv;
					bRight = true;
				}
			}
			break;
			case ZDevicePort::ERROR_INVALIDPORT:
				strText = _T("����δ��!");
				break;
			case ZDevicePort::ERROR_RECV_TIMEOUT:
			{
				CString strOrderType = p_md->m_p_rmtProtl->GetOrderType();
				dwNowTick = ::GetTickCount();
				if ((strOrderType == _T("20") || strOrderType == _T("33")) && dwNowTick < dwEndTick)
					p_md->m_nFlagUpdateRecvData = FLAG_GOTORECV;
				else
					strText = _T("���ڽ��ճ�ʱ!");
			}
			break;
			case ZDevicePort::ERROR_RECV_STOP:
				strText = _T("������ֹͣ!");
				break;
			default:
				strText = _T("���ڽ���ʧ��!");
			}
			UpdateEdit(bRight, strText, p_md);
		}
		break;
		case CMainData::PROTOCOL_DLT698:
		{
			CString strText;
			bool bRight = false;
			switch (p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
			{
				if (p_md->m_p_dlt698->IsSplitRecv())
					p_md->m_nFlagUpdateRecvData = FLAG_GOTOENCODE;
				UpdateList(_T("���ա�") + p_md->m_p_zdp->GetPort(), p_md->m_strDataRecv, p_md);
				strText = p_md->m_strDataRecv;
				bRight = true;
			}
			break;
			case ZDevicePort::ERROR_INVALIDPORT:
				strText = _T("����δ��!");
				break;
			case ZDevicePort::ERROR_RECV_TIMEOUT:
				strText = _T("���ڽ��ճ�ʱ!");
				break;
			case ZDevicePort::ERROR_RECV_STOP:
				strText = _T("������ֹͣ!");
				break;
			default:
				strText = _T("���ڽ���ʧ��!");
			}
			UpdateEdit(bRight, strText, p_md);
		}
		break;
		}
	}
	break;
	case MSGUSER_TRANRECVDATA:
	{
		CMainData * p_md = (CMainData *)wParam;
		if (!p_md->m_nRtn)
			UpdateList(_T("���ա�") + p_md->m_p_zdp->GetPort(), p_md->m_strDataRecv, p_md);
	}
	break;
	case MSGUSER_ENDONCELOOP:
	{
		CMainData * p_md = (CMainData *)wParam;
		static int nPortAdd = 0;
		if (g_bIsStop)
			nPortAdd = 0;
		else
		{
			int nRowNext = p_md->m_nRow + 1;
			if (nRowNext >= p_md->m_nRowMax)
			{
				++nPortAdd;
				if (nPortAdd == g_nTotalPort)
				{
					nPortAdd = 0;
					for (int i = 0; i < sizeof(g_sz_p_wnd) / sizeof(CWnd *); ++i)
					{
						if (g_sz_p_wnd[i])
							g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO, TRUE, MSGUSER_ENABLEBTN);
					}
				}
			}
		}
	}
	break;
	case MSGUSER_ENABLEBTN:
	{
		EnableBtn(wParam);
	}
	break;
	case MSGUSER_GETSERVERINFO:
	{
		CGlobalVariable * p_gvariable = (CGlobalVariable *)wParam;
		*p_gvariable = g_v;
	}
	break;
	case MSGUSER_ERROFGETSERVERINFO:
	{
		//������
	}
	break;
	case MSGUSER_ISNEEDUPDATE:
	{
		CString * p_str = (CString *)wParam;
		std::vector<CString> vec_strDatas;
		ZUtil::StrSplit(*p_str, vec_strDatas, _T('*'), p_str->Right(1) == _T('*'));
		if (vec_strDatas[0] != g_strVersion)
		{
			OnHelpingUpdate();
		}
	}
	break;
	case MSGUSER_UPDATEDIDB:
	{
		if (wParam)
		{
			::DeleteFile(ZUtil::GetExeCatalogPath() + _T("\\res\\DataItem.ditemp"));
		}
		else
		{
			::DeleteFile(ZUtil::GetExeCatalogPath() + _T("\\res\\DataItem.di"));
			CFile::Rename(ZUtil::GetExeCatalogPath() + _T("\\res\\DataItem.ditemp"), ZUtil::GetExeCatalogPath() + _T("\\res\\DataItem.di"));
		}
	}
	break;
	}
	return 0;
}


void CMeterCommView::UpdateList(const CString & strFlag, const CString & strData,CMainData * p_md)
{
	static CString strTime;
	static SYSTEMTIME stTime;
	::GetLocalTime(&stTime);
	strTime.Format(_T("%02d:%02d:%02d.%03d"),stTime.wHour,stTime.wMinute,stTime.wSecond,stTime.wMilliseconds);
	m_listDisplay.SetRedraw(FALSE);
	m_listDisplay.InsertItem(m_nListCount,strFlag); 
	switch(p_md->m_nProtocolType%10)
	{
	case CMainData::PROTOCOL_DLT645:
		{
			m_listDisplay.SetItemText(m_nListCount, 1,_T("DL/T645-2007")); 
		}
		break;
	case CMainData::PROTOCOL_RMT:
		{
			m_listDisplay.SetItemText(m_nListCount, 1,_T("RMTProtocol")); 
		}
		break;
	case CMainData::PROTOCOL_DLT698:
		{
			m_listDisplay.SetItemText(m_nListCount, 1,_T("DL/T698.45")); 
		}
		break;
	}	
	m_listDisplay.SetItemText(m_nListCount,2,DataFormat(strData)); 
	m_listDisplay.SetItemText(m_nListCount,3,strTime); 
	m_listDisplay.EnsureVisible(m_nListCount, FALSE);
	++m_nListCount;
	m_listDisplay.SetRedraw(TRUE);
}

void CMeterCommView::UpdateEdit(const bool & bRight,const CString & strData,CMainData * p_md)
{
	CString & strContect=p_md->m_strDataValue;
	if(bRight)
	{
		SetDlgItemText(IDC_EDIT_DETAIL,DataFormat(strData));
		SetDlgItemText(IDC_EDIT_CONTECT,strContect);
		switch (p_md->m_nProtocolType % 10)
		{
		case CMainData::PROTOCOL_DLT645:
		{
			CString strExt= strContect.Right(3);
			if ((strExt == _T("91H") || strExt == _T("94H")) && p_md->m_p_dlt645->GetDataItem() == _T("04000401"))
			{
				SetDlgItemText(IDC_EDIT_DEVICEADDR, p_md->m_p_dlt645->GetMeterAddr());
			}
			if (strExt == _T("93H") || strExt == _T("95H"))
			{
				SetDlgItemText(IDC_EDIT_DEVICEADDR, p_md->m_p_dlt645->GetMeterAddr());
			}
		}
		break;
		case CMainData::PROTOCOL_DLT698:
		{
			CString strReqResCode = strContect.Right(3);
			strReqResCode = strReqResCode.Left(2);
			if ((strReqResCode == _T("85") || strReqResCode == _T("86") || strReqResCode == _T("90")) && strContect.Find(_T("obj=\"40010200\"")) != -1)
				SetDlgItemText(IDC_EDIT_DEVICEADDR, p_md->m_p_dlt698->GetServerAddr());
		}
		break;
		}
	}
	else
	{
		SetDlgItemText(IDC_EDIT_DETAIL,strData);
		SetDlgItemText(IDC_EDIT_CONTECT,strData);
	}
}

CString CMeterCommView::DataFormat(const CString & str)
{
	CString strByte,strTemp;
	int nSize=str.GetLength();
	for(int i=0;i<nSize;i=i+2)
	{
		strByte=str.Mid(i,2);
		strTemp+=strByte+_T(" ");
	}
	return strTemp;
}

void CMeterCommView::OnSettingComport()
{
	// TODO: �ڴ���������������
	if(m_p_comportset==NULL)
	{
		m_p_comportset = new CComPortSetting(); 
		m_p_comportset->Create(IDD_COMPORTSETTING, this); 
	}
	m_p_comportset->ShowWindow(SW_SHOW);
}


void CMeterCommView::OnSettingParameter()
{
	// TODO: �ڴ���������������
	CParamSetting paramset;
	paramset.SetParam(g_v.g_strGrade, g_v.g_strPsWd, g_v.g_strOperaCode);
	if (IDOK == paramset.DoModal())
	{
		paramset.GetParam(g_v.g_strGrade, g_v.g_strPsWd, g_v.g_strOperaCode);
	}
}


void CMeterCommView::OnSettingNetwork()
{
	// TODO: �ڴ���������������
	CNetWorkSetting nwsetting;
	nwsetting.SetParam(g_v.g_strServerIP, g_v.g_strServerPort, g_v.g_strServerTimeOut, g_v.g_strNetCptIP, g_v.g_strNetCptPort, g_v.g_strNetCptTimeOut, m_nDllSel, g_v.g_nIsRemote);
	if (IDOK == nwsetting.DoModal())
	{
		nwsetting.GetParam(g_v.g_strServerIP, g_v.g_strServerPort, g_v.g_strServerTimeOut, g_v.g_strNetCptIP, g_v.g_strNetCptPort, g_v.g_strNetCptTimeOut, m_nDllSel, g_v.g_nIsRemote);
		switch (m_nDllSel)
		{
		case 0:g_v.g_nDllType = 1; break;
		case 1:g_v.g_nDllType = 2; break;
		case 2:g_v.g_nDllType = 3; break;
		case 3:g_v.g_nDllType = 4; break;
		case 4:g_v.g_nDllType = 5; break;
		case 5:g_v.g_nDllType = 1; break;
		}
	}
}


void CMeterCommView::OnTestingProduce()
{
	// TODO: �ڴ���������������
	if(m_p_protest==NULL)
	{
		m_p_protest = new CProduceTesting(); 
		m_p_protest->Create(IDD_PRODUCETESTING, this); 
		m_p_protest->ModifyStyle(0,WS_CLIPSIBLINGS);
	}
	CRect rc;
	GetClientRect(&rc);
	m_p_protest->BringWindowToTop();
	m_p_protest->MoveWindow(rc,FALSE);
	m_p_protest->ShowWindow(SW_SHOW);
}


void CMeterCommView::OnTestingMeter()
{
	// TODO: �ڴ���������������
	if(m_p_metertest==NULL)
	{
		m_p_metertest = new CMeterTesting(); 
		m_p_metertest->Create(IDD_METERTESTING, this); 
		m_p_metertest->ModifyStyle(0,WS_CLIPSIBLINGS);
	}
	CRect rc;
	GetClientRect(&rc);
	m_p_metertest->BringWindowToTop();
	m_p_metertest->MoveWindow(rc,FALSE);
	m_p_metertest->ShowWindow(SW_SHOW);
}

void CMeterCommView::OnTestingCard()
{
	// TODO: �ڴ���������������
	CCardTesting cardtesting;
	cardtesting.DoModal();
}

void CMeterCommView::AddClipSiblings(void)
{
	CWnd * p_wndChild=NULL;
	p_wndChild=GetWindow(GW_CHILD);
	while(p_wndChild!=NULL)
	{
		p_wndChild->ModifyStyle(0,WS_CLIPSIBLINGS);
		p_wndChild=p_wndChild->GetNextWindow(GW_HWNDNEXT);
	}
	GetDlgItem(IDC_STATIC_OPERATORAREA)->SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	GetDlgItem(IDC_STATIC_DISPLAYAREA)->SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

void CMeterCommView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_bIsInitFinish&&nType!=SIZE_MINIMIZED)
	{
		CRect rc;
		GetClientRect(&rc);
		ChangeSize(IDC_STATIC_DISPLAYAREA,rc); 
		ChangeSize(IDC_EDIT_CONTECT,rc);
		ChangeSize(IDC_EDIT_DETAIL,rc);
		ChangeSize(IDC_LIST_DISPLAY,rc); 
		ChangeSize(IDC_STATIC_OPERATORAREA,rc);
		ChangeSize(IDC_BUTTON_CLEARDATA,rc);
		ChangeSize(IDC_BUTTON_GETESAMID,rc);
		ChangeSize(IDC_BUTTON_GETIDPARAM,rc);
		ChangeSize(IDC_BUTTON_STOP,rc);
		ChangeSize(IDC_BUTTON_SEND,rc);
		ChangeSize(IDC_EDIT_SENDDATA,rc);
		ChangeSize(IDC_EDIT_SENDTIMES,rc);
		ChangeSize(IDC_STATIC_SENDTIMES,rc);
		ChangeSize(IDC_EDIT_TARGETDATA,rc);
		ChangeSize(IDC_STATIC_DETAIL,rc);
		ChangeSize(IDC_STATIC_CONTECT,rc);
		if(m_p_protest!=NULL)
			m_p_protest->MoveWindow(rc,FALSE);
		if(m_p_metertest!=NULL)
			m_p_metertest->MoveWindow(rc,FALSE);
		CRect rcDisplay;     
		m_listDisplay.GetClientRect(&rcDisplay);     
		m_listDisplay.SetColumnWidth(0,rcDisplay.Width()*6/40);   
		m_listDisplay.SetColumnWidth(1,rcDisplay.Width()*6/40);   
		m_listDisplay.SetColumnWidth(2,rcDisplay.Width()*21/40);
		m_listDisplay.SetColumnWidth(3,rcDisplay.Width()*5/40);
		InitButton();
		Invalidate();
	}
}

void CMeterCommView::ChangeSize(UINT nID,const CRect & rcParent)
{
	CWnd *pWnd=GetDlgItem(nID); 
	if(pWnd!=NULL)
	{
		static CRect rcLast;
		CRect rc; 
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		int nHeight=rc.Height();
		int nWidth=rc.Width();
		switch(nID)
		{
		case IDC_STATIC_DISPLAYAREA:
			{
				rc.right=rcParent.right-10;
				rc.bottom=rcParent.bottom-10;
			}
			break;
		case IDC_EDIT_CONTECT:
			{
				rc.right=rcLast.right-10;
				rc.bottom=rcLast.bottom-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_EDIT_DETAIL:
			{
				rc.right=rcLast.right;
				rc.bottom=rcLast.top-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_LIST_DISPLAY:
			{
				rc.right=rcLast.right;
				rc.bottom=rcLast.top-10;
			}
			break;
		case IDC_STATIC_OPERATORAREA:
			{
				rc.right=rcParent.right-10;
			}
			break;
		case IDC_BUTTON_CLEARDATA:
			{
				rc.right=rcLast.right-10;
				rc.left=rc.right-nWidth;
			}
			break;
		case IDC_BUTTON_GETESAMID:
		case IDC_BUTTON_GETIDPARAM:
		case IDC_BUTTON_STOP:
		case IDC_BUTTON_SEND:
			{
				rc.right=rcLast.left-10;
				rc.left=rc.right-nWidth;
			}
			break;
		case IDC_EDIT_SENDDATA:
			{
				rc.right=rcLast.left-10;
			}
			break;
		case IDC_EDIT_SENDTIMES:
			{
				rc.right=rcParent.right-20;
				rc.left=rc.right-nWidth; 
			}
			break;
		case IDC_STATIC_SENDTIMES:
			{
				rc.right=rcLast.left-5;
				rc.left=rc.right-nWidth;
			}
			break;
		case IDC_EDIT_TARGETDATA:
			{
				rc.right=rcParent.right-20;
			}
			break;
		case IDC_STATIC_DETAIL:
			{
				CRect rcTemp;
				GetDlgItem(IDC_EDIT_DETAIL)->GetWindowRect(&rcTemp);
				ScreenToClient(&rcTemp);
				rc.top=rcTemp.top;
				rc.bottom=rc.top+nHeight;
			}
			break;
		case IDC_STATIC_CONTECT:
			{
				CRect rcTemp;
				GetDlgItem(IDC_EDIT_CONTECT)->GetWindowRect(&rcTemp);
				ScreenToClient(&rcTemp);
				rc.top=rcTemp.top;
				rc.bottom=rc.top+nHeight;
			}
			break;
		}
		rcLast=rc;
		pWnd->MoveWindow(rc,FALSE);
	}
}

void CMeterCommView::EnableBtn(const BOOL & bEnable)
{
	GetDlgItem(IDC_BUTTON_GETADDR)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_GETIDPARAM)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_GETESAMID)->EnableWindow(bEnable);
}

BOOL CMeterCommView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_BKGND);
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap * p_bmpOld=dcMem.SelectObject(&bmp);
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	CRect rc; 
	GetClientRect(&rc);
	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);
	dcMem.SelectObject(p_bmpOld);
	return TRUE;
	return CFormView::OnEraseBkgnd(pDC);
}


HBRUSH CMeterCommView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	static CBrush brush;
	if(brush.m_hObject==NULL)
		brush.CreateSolidBrush(RGB(230,248,226)); 
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		{
			if(pWnd->GetDlgCtrlID()!=IDC_EDIT_LENGTH)
			{
				pDC->SetBkMode(TRANSPARENT);
				return (HBRUSH)GetStockObject(NULL_BRUSH);
			}
			else
				return brush;
		}
		break;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CMeterCommView::InitButton(void)
{
	SetBtnColor(m_btnGetAddr,_T(""));
	SetBtnColor(m_btnSend,_T(""));
	SetBtnColor(m_btnGetEsamID,_T(""));
	SetBtnColor(m_btnGetDataItem,_T(""));
	SetBtnColor(m_btnStop,_T(""));
	SetBtnColor(m_btnGetMeterNO,_T(""));
	SetBtnColor(m_btnClearData,_T(""));
}

void CMeterCommView::SetBtnColor(ZButton & btn,const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN,RGB(230,248,226));
	btn.SetColor(ZButton::COLOR_BK_OUT,RGB(230,248,226));
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

void CMeterCommView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: �ڴ˴������Ϣ����������
	CMenu menu;
	if(menu.LoadMenu(IDR_MENU_MAINWND))
	{
		CMenu* pPopup=menu.GetSubMenu(0);  
		if(pPopup) 
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	}
}


void CMeterCommView::OnNMClickListDisplay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if(pNMItemActivate->iItem!=-1)   
    {    
		CString strType,strData,strContect;
		strType = m_listDisplay.GetItemText(pNMItemActivate->iItem,1);   
		strData = m_listDisplay.GetItemText(pNMItemActivate->iItem,2); 
		SetDlgItemText(IDC_EDIT_DETAIL,strData);
		if(ZDLT645_2007::Check645_2007(strData))
		{
			ZFac645 dlt645;
			dlt645.DataDecode(strData,strContect);
		}
		else if(ZRMTProtocol::CheckRMTProtocol(strData))
		{
			ZRMTProtocol rmtProtl;
			rmtProtl.DataDecode(strData,strContect);
		}
		else if(ZDLT698_45::Check698_45(strData))
		{
			ZDLT698_45 dlt698;
			dlt698.DataDecode(strData,strContect);
		}
		SetDlgItemText(IDC_EDIT_CONTECT,strContect);
	} 
}

UINT CMeterCommView::CheckSoftwareUpdateThreadFunc(LPVOID lpParam)
{
	CMeterCommView * m_p_MainDlg=(CMeterCommView *)lpParam;
	ResetEvent(m_p_MainDlg->m_hEvtExitCheckUpdate);
	CString strDataIn,strDataOut,strHead,strPath;
	strDataIn=g_strVersion;
	strHead.Format(_T("[%06d02]"),strDataIn.GetLength()+10);
	strDataIn=strHead+strDataIn;
	if(!SocketCommunication(strDataIn,strDataOut))
	{
		if(m_p_MainDlg->m_hWnd)
			m_p_MainDlg->PostMessage(WM_MSGRECVPRO,0,MSGUSER_ERROFGETSERVERINFO); 
	}
	else
	{
		if(strDataOut.Mid(0,1)==_T("[")&&strDataOut.Mid(9,1)==_T("]")&&_ttoi(strDataOut.Mid(1,6))==strDataOut.GetLength()&&strDataOut.Mid(7,2)==_T("02"))
		{
			strDataOut.Delete(0,10);
			SendMessageTimeout(m_p_MainDlg->m_hWnd,WM_MSGRECVPRO,(WPARAM)&strDataOut,MSGUSER_ISNEEDUPDATE,SMTO_BLOCK,500,NULL); 
		}
		else
		{
			if(m_p_MainDlg->m_hWnd)
				m_p_MainDlg->PostMessage(WM_MSGRECVPRO,0,MSGUSER_ERROFGETSERVERINFO); 
		}
	}
	SetEvent(m_p_MainDlg->m_hEvtExitCheckUpdate);
	return 0;
}

bool CMeterCommView::SocketCommunication(const CString & strDataIn,CString & strDataOut)
{
	ZStringSocket zSock;
	int nRtn=zSock.InitSocket();
	if(nRtn)
	{
		zSock.CloseSocket();
		return false;
	}
	CGlobalVariable gvariable;
	SendMessageTimeout(g_sz_p_wnd[WND_METERCOMM]->m_hWnd,WM_MSGRECVPRO,(WPARAM)&gvariable,MSGUSER_GETSERVERINFO,SMTO_BLOCK,500,NULL); 
	CString & strIP=gvariable.g_strServerIP;
	CString & strPort=gvariable.g_strServerPort;
	CString & strTimeOut=gvariable.g_strServerTimeOut;
	zSock.SetTimeOut(ZSocket::TIMEOUT_CONT,_ttoi(strTimeOut));
	zSock.SetTimeOut(ZSocket::TIMEOUT_SEND,_ttoi(strTimeOut));
	zSock.SetTimeOut(ZSocket::TIMEOUT_RECV,_ttoi(strTimeOut));
	nRtn=zSock.Connect(strIP,strPort);
	if(nRtn)
	{
		zSock.CloseSocket();
		return false;
	}
	nRtn=zSock.StringSend(strDataIn);
	if(nRtn)
	{
		zSock.CloseSocket();
		return false;
	}
	nRtn=zSock.StringRecv(strDataOut);
	if(nRtn)
	{
		zSock.CloseSocket();
		return false;
	}
	zSock.CloseSocket();
	return true;
}

void CMeterCommView::OnHelpingUpdate()
{
	// TODO: �ڴ���������������
	CSoftwareUpdate swupdate;
	swupdate.DoModal();
}


void CMeterCommView::OnSettingDidatabase()
{
	// TODO: �ڴ���������������
	CDIDataBase didb;
	didb.DoModal();
}

UINT CMeterCommView::UpdateDIDBThreadFunc(LPVOID lpParam)
{
	CMeterCommView * m_p_MainDlg=(CMeterCommView *)lpParam;
	ResetEvent(m_p_MainDlg->m_hEvtExitUpdateDIDB);
	CString strDataIn,strHead,strPath;
	::CreateDirectory(ZUtil::GetExeCatalogPath()+_T("\\res"), NULL);
	strDataIn=_T("DataItem.di");
	strHead.Format(_T("[%06d05]"),strDataIn.GetLength()+10);
	strDataIn=strHead+strDataIn;
	int nRtn=0;
	ZFileSocket sock;
	sock.InitSocket();
	CGlobalVariable gvariable;
	SendMessageTimeout(g_sz_p_wnd[WND_METERCOMM]->m_hWnd,WM_MSGRECVPRO,(WPARAM)&gvariable,MSGUSER_GETSERVERINFO,SMTO_BLOCK,500,NULL); 
	CString & strIP=gvariable.g_strServerIP;
	CString & strPort=gvariable.g_strServerPort;
	CString & strTimeOut=gvariable.g_strServerTimeOut;
	nRtn=sock.Connect(strIP,strPort);
	if(nRtn)
	{
		sock.CloseSocket();
		SetEvent(m_p_MainDlg->m_hEvtExitUpdateDIDB);
		return 0;
	}
	nRtn=sock.StringSend(strDataIn);
	if(nRtn)
	{
		sock.CloseSocket();
		SetEvent(m_p_MainDlg->m_hEvtExitUpdateDIDB);
		return 0;
	}
	strPath=ZUtil::GetExeCatalogPath()+_T("\\res\\DataItem.ditemp");
	sock.SetFilePath(strPath);
	ULONGLONG llLength=0;
	CString strLength,strFinish, strCurTimes;
	nRtn=sock.StringRecv(strLength);
	if(nRtn)
		goto end;
	llLength=_ttoi64(strLength);
	sock.SetFileLength(llLength);
	strCurTimes.Format(_T("%lld"), sock.GetCurTimes());
	nRtn=sock.StringSend(strCurTimes);
	if(nRtn)
		goto end;
	nRtn=sock.FileRecv();
	if(nRtn)
		goto end;
end:
	if(m_p_MainDlg->m_hWnd)
		m_p_MainDlg->PostMessage(WM_MSGRECVPRO,(WPARAM)nRtn,MSGUSER_UPDATEDIDB); 
	sock.StringRecv(strFinish);
	sock.CloseSocket();
	SetEvent(m_p_MainDlg->m_hEvtExitUpdateDIDB);
	return 0;
}

void CMeterCommView::OnCbnSelchangeComboProtocol()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel=m_comboProtocol.GetCurSel();
	if(nSel!=CB_ERR)
	{
		g_v.g_nProtocolType=nSel;
		InitFuncCode(nSel);
	}
}


void CMeterCommView::InitFuncCode(int nProtocol)
{
	m_comboFuncCode.ResetContent();
	switch(nProtocol)
	{
	case CMainData::PROTOCOL_DLT645:
		{
			CString sz_strFuncCode[]={_T("11H-������"),_T("14H-д����"),_T("1AH-�������"),_T("1BH-�¼�����"),_T("1CH-����բ������������"),_T("03H-ESAM���"),_T("08H-�㲥Уʱ")};
			for(int i=0;i<sizeof(sz_strFuncCode)/sizeof(CString);++i)
				m_comboFuncCode.AddString(sz_strFuncCode[i]);
			m_comboFuncCode.SetCurSel(0);
		}
		break;
	case CMainData::PROTOCOL_RMT:
		{
			CString sz_strFuncCode[]={_T("10H-̨���������"),_T("23H-����"),_T("20H-�������춨"),_T("33H-�ռ�ʱ���춨"),_T("22H-ж��ֹͣ")};
			for(int i=0;i<sizeof(sz_strFuncCode)/sizeof(CString);++i)
				m_comboFuncCode.AddString(sz_strFuncCode[i]);
			m_comboFuncCode.SetCurSel(0);
		}
		break;
	case CMainData::PROTOCOL_DLT698:
		{
			CString sz_strFuncCode[]={_T("0501H-��ȡ����01"),_T("0502H-��ȡ����02"),_T("0601H-��������01"),_T("0602H-��������02"),_T("0701H-��������01"),_T("0702H-��������02"),_T("02H-����Ӧ����������"),_T("03H-�Ͽ�Ӧ����������")};
			for(int i=0;i<sizeof(sz_strFuncCode)/sizeof(CString);++i)
				m_comboFuncCode.AddString(sz_strFuncCode[i]);
			m_comboFuncCode.SetCurSel(0);
		}
		break;
	}
}


void CMeterCommView::ReadGlobalVariable()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	CString * p_sz_strValue[] = { &g_v.g_strServerIP ,&g_v.g_strServerPort ,&g_v.g_strServerTimeOut, &g_v.g_strNetCptIP ,&g_v.g_strNetCptPort ,&g_v.g_strNetCptTimeOut };
	if (InitGlobalVariableDB())
	{
		ZSqlite3 zsql;
		CString strPath = ZUtil::GetExeCatalogPath() + _T("\\res\\Data.db3");
		if (!zsql.OpenDB(strPath))
			return;
		CString  strSQL, strError;
		strSQL = _T("SELECT Value FROM Setting ORDER BY ID");
		std::vector<std::vector <CString>> vec2_strData;
		zsql.GetTable(strSQL, vec2_strData, &strError);
		if (vec2_strData.size() == 9)
		{
			for (int i = 0; i < 6; ++i)
			{
				*p_sz_strValue[i] = vec2_strData[i + 1][0];
			}
			m_nDllSel = _ttoi(vec2_strData[6 + 1][0]);
			switch (m_nDllSel)
			{
			case 0:g_v.g_nDllType = 1; break;
			case 1:g_v.g_nDllType = 2; break;
			case 2:g_v.g_nDllType = 3; break;
			case 3:g_v.g_nDllType = 4; break;
			case 4:g_v.g_nDllType = 5; break;
			case 5:g_v.g_nDllType = 1; break;
			}
			g_v.g_nIsRemote = _ttoi(vec2_strData[7 + 1][0]);
		}
	}
}


void CMeterCommView::WriteGlobalVariable()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	CString * p_sz_strValue[] = { &g_v.g_strServerIP ,&g_v.g_strServerPort ,&g_v.g_strServerTimeOut, &g_v.g_strNetCptIP ,&g_v.g_strNetCptPort ,&g_v.g_strNetCptTimeOut };
	if (InitGlobalVariableDB())
	{
		ZSqlite3 zsql;
		CString strPath = ZUtil::GetExeCatalogPath() + _T("\\res\\Data.db3");
		if (!zsql.OpenDB(strPath))
			return;
		CString  strSQL, strError;
		for (int i = 0; i < 6; ++i)
		{
			strSQL.Format(_T("UPDATE Setting SET Value='%s' WHERE ID=%d"), *p_sz_strValue[i], i);
			zsql.ExecSQL(strSQL, &strError);
		}
		strSQL.Format(_T("UPDATE Setting SET Value='%d' WHERE ID=%d"), m_nDllSel, 6);
		zsql.ExecSQL(strSQL, &strError);
		strSQL.Format(_T("UPDATE Setting SET Value='%d' WHERE ID=%d"), g_v.g_nIsRemote, 7);
		zsql.ExecSQL(strSQL, &strError);
	}
}


BOOL CMeterCommView::InitGlobalVariableDB()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	ZSqlite3 zsql;
	CString strPath = ZUtil::GetExeCatalogPath() + _T("\\res\\Data.db3");
	if (!zsql.OpenDB(strPath))
		return FALSE;
	const CString sz_strIDInfo[] =
	{
		_T("ServerIP"),
		_T("ServerPort"),
		_T("ServerTimeOut"),
		_T("NetCptIP"),
		_T("NetCptPort"),
		_T("NetCptTimeOut"),
		_T("DllSel"),
		_T("IsRemote")
	};
	const CString sz_strValue[] =
	{
		_T("zaxai.nat123.net"),
		_T("46960"),
		_T("5000"),
		_T("10.0.17.99"),
		_T("6666"),
		_T("5000"),
		_T("1"),
		_T("0")
	};
	CString strSql;
	strSql = _T("CREATE TABLE Setting (ID int PRIMARY KEY NOT NULL,IDInfo nvarchar(100) NOT NULL,Value nvarchar(1000) NOT NULL)");
	if (zsql.ExecSQL(strSql) == 0)
	{
		for (int i = 0; i < sizeof(sz_strIDInfo) / sizeof(CString); ++i)
		{
			strSql.Format(_T("INSERT INTO Setting VALUES ( %d ,'%s','%s')"), i, sz_strIDInfo[i], sz_strValue[i]);
			if (zsql.ExecSQL(strSql))
			{
				zsql.ExecSQL(_T("DROP TABLE Setting"));
				return FALSE;
			}
		}
	}
	return TRUE;
}
