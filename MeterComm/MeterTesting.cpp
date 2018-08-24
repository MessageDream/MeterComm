// MeterTesting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MeterComm.h"
#include "MeterTesting.h"
#include "afxdialogex.h"
#include <locale.h>
#include "ShowResultOfMT.h"

// CMeterTesting �Ի���
const int nMaxItem=36;

IMPLEMENT_DYNAMIC(CMeterTesting, CDialogEx)

CMeterTesting::CMeterTesting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeterTesting::IDD, pParent)
	, m_nIndexOfItem(-1)
	, m_bIsInitFinish(false)
{

}

CMeterTesting::~CMeterTesting()
{
	int nSize=m_vec_listItem.size();
	for(int i=0;i<nSize;++i)
	{
		if(m_vec_listItem[i]!=NULL)
		{
			delete m_vec_listItem[i];
			m_vec_listItem[i]=NULL;
		}
	}
}

void CMeterTesting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ITEMCLASS, m_treeItemClass);
	DDX_Control(pDX, IDC_CHECK_SELECTALL, m_checkSelectAll);
	DDX_Control(pDX, IDC_COMBO_PORTSELECTED, m_comboPortSelected);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_BUTTON_UNSELECT, m_btnUnselect);
	DDX_Control(pDX, IDC_BUTTON_READMETER, m_btnReadMeter);
	DDX_Control(pDX, IDC_BUTTON_SETMETER, m_btnSetMeter);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CMeterTesting, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ITEMCLASS, &CMeterTesting::OnTvnSelchangedTreeItemclass)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, &CMeterTesting::OnBnClickedCheckSelectall)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CMeterTesting::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECT, &CMeterTesting::OnBnClickedButtonUnselect)
	ON_BN_CLICKED(IDC_BUTTON_READMETER, &CMeterTesting::OnBnClickedButtonReadmeter)
	ON_BN_CLICKED(IDC_BUTTON_SETMETER, &CMeterTesting::OnBnClickedButtonSetmeter)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMeterTesting::OnBnClickedButtonStop)
	ON_WM_SIZE()
	ON_MESSAGE(WM_MSGRECVPRO, &CMeterTesting::OnMsgrecvpro)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_NOTIFY_RANGE(NM_DBLCLK, IDC_LIST_START, IDC_LIST_START+nMaxItem, &CMeterTesting::OnNMDblclkListItem)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CMeterTesting ��Ϣ�������


BOOL CMeterTesting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	g_sz_p_wnd[WND_METERTEST]=this;
	InitTree();
	InsertCombo();
	m_bIsInitFinish=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMeterTesting::InitTree(void)
{
	CTreeCtrl & tree=m_treeItemClass;
	HTREEITEM hRoot,hFirst,hSecond; 
	int nIndex=0;
    hRoot = tree.InsertItem(_T("����������"));    
	tree.SetItemData(hRoot,nIndex++); 
	m_vec_strClass.push_back(_T(""));
	hFirst = tree.InsertItem(_T("��������"), hRoot, TVI_LAST);   
    tree.SetItemData(hFirst,nIndex++); 
	m_vec_strClass.push_back(_T("0"));
	hSecond = tree.InsertItem(_T("��ǰ"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("0,0"));
	CString strTemp;
	for(int i=0;i<12;++i)
	{
		strTemp.Format(_T("��%02d������"),i+1);
		hSecond = tree.InsertItem(strTemp, hFirst, TVI_LAST);   
		tree.SetItemData(hSecond,nIndex++);
		strTemp.Format(_T("%d"),i+1);
		m_vec_strClass.push_back(_T("0,")+strTemp);
	}
	hFirst = tree.InsertItem(_T("��������"), hRoot, TVI_LAST);   
    tree.SetItemData(hFirst,nIndex++); 
	m_vec_strClass.push_back(_T("1"));
	hFirst = tree.InsertItem(_T("��������"), hRoot, TVI_LAST);   
    tree.SetItemData(hFirst,nIndex++); 
	m_vec_strClass.push_back(_T("2"));
	hFirst = tree.InsertItem(_T("�¼���¼"), hRoot, TVI_LAST);     
    tree.SetItemData(hFirst,nIndex++); 
	m_vec_strClass.push_back(_T("3"));
	hSecond = tree.InsertItem(_T("�����¼"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("3,0"));
	hSecond = tree.InsertItem(_T("��̼�¼"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("3,1"));
	hSecond = tree.InsertItem(_T("��������¼"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("3,2"));
	hSecond = tree.InsertItem(_T("Уʱ��¼"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("3,3"));
	hSecond = tree.InsertItem(_T("����Ǽ�¼"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("3,4"));
	hSecond = tree.InsertItem(_T("��բ��¼"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("3,5"));
	hSecond = tree.InsertItem(_T("��բ��¼"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("3,6"));
	hFirst = tree.InsertItem(_T("�α�������"), hRoot, TVI_LAST);   
    tree.SetItemData(hFirst,nIndex++); 
	m_vec_strClass.push_back(_T("4"));
	hSecond = tree.InsertItem(_T("��������"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++); 
	m_vec_strClass.push_back(_T("4,0"));
	hSecond = tree.InsertItem(_T("ʱ��ʱ��"), hFirst, TVI_LAST);     
    tree.SetItemData(hSecond,nIndex++); 
	m_vec_strClass.push_back(_T("4,1"));
	hSecond = tree.InsertItem(_T("��ʾ��"), hFirst, TVI_LAST);     
    tree.SetItemData(hSecond,nIndex++); 
	m_vec_strClass.push_back(_T("4,3"));
	hFirst = tree.InsertItem(_T("��������"), hRoot, TVI_LAST);     
    tree.SetItemData(hFirst,nIndex++); 
	m_vec_strClass.push_back(_T("5"));
	hSecond = tree.InsertItem(_T("��ʱ�������ݿ�"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("5,0"));
	hSecond = tree.InsertItem(_T("˲ʱ�������ݿ�"), hFirst, TVI_LAST);   
    tree.SetItemData(hSecond,nIndex++);
	m_vec_strClass.push_back(_T("5,1"));
	ExpandTree(tree,tree.GetRootItem()); 
	for(int i=0;i<nIndex;++i)
		m_vec_listItem.push_back(NULL);
}

void CMeterTesting::ExpandTree(CTreeCtrl & tree,HTREEITEM hTreeItem)
{
	if(!tree.ItemHasChildren(hTreeItem))
	{
		return;
	}
	HTREEITEM hNextItem = tree.GetChildItem(hTreeItem);
	while (hNextItem != NULL)
	{
		ExpandTree(tree,hNextItem);
		hNextItem = tree.GetNextItem(hNextItem, TVGN_NEXT);
	}
	tree.Expand(hTreeItem,TVE_EXPAND);
}

void CMeterTesting::OnTvnSelchangedTreeItemclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	HTREEITEM hItem = m_treeItemClass.GetSelectedItem();
	if(hItem)
	{
		int nIndex=m_treeItemClass.GetItemData(hItem);
		if(m_vec_listItem[nIndex]==NULL)
		{
			m_vec_listItem[nIndex]=new CListCtrl();
			CRect rcTemp;
			m_vec_listItem[nIndex]->Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_ALIGNLEFT|WS_BORDER|WS_TABSTOP,rcTemp,this,IDC_LIST_START+nIndex);
			m_vec_listItem[nIndex]->SetExtendedStyle(m_vec_listItem[nIndex]->GetExtendedStyle()|LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
			m_vec_listItem[nIndex]->InsertColumn(0, _T("���"), LVCFMT_LEFT,0, 0); 
			m_vec_listItem[nIndex]->InsertColumn(1, _T("���ݱ�ʶ"), LVCFMT_LEFT, 0,1); 
			m_vec_listItem[nIndex]->InsertColumn(2, _T("��������"), LVCFMT_LEFT,0,2); 
			m_vec_listItem[nIndex]->InsertColumn(3, _T("ͨѶ���"), LVCFMT_LEFT,0,3); 
			m_vec_listItem[nIndex]->InsertColumn(4, _T("�������"), LVCFMT_LEFT,0,4); 
			m_vec_listItem[nIndex]->SetTextColor(RGB(192,64,0));
			InsertList(nIndex);
		}
		if(m_nIndexOfItem!=-1)
			m_vec_listItem[m_nIndexOfItem]->ShowWindow(SW_HIDE);
		OperatorList(nIndex);
		m_vec_listItem[nIndex]->ShowWindow(SW_SHOW);
		m_nIndexOfItem=nIndex;
	}
}

void CMeterTesting::InsertList(const int & nIndex)
{
	std::vector<CString> vec_strData;
	StrSplit_Z(m_vec_strClass[nIndex],vec_strData,_T(','),m_vec_strClass[nIndex].Right(1)==_T(','));
	ZSqlite3 zsql;
	CString strPath=GetExeCatalogPath_Z()+_T("\\res\\DataItem.di");
	CStdioFile f;
	if(!f.Open(strPath, CFile::modeRead))
	{
		AfxMessageBox(_T("���ݿ��ļ���ʧ!"));
		return;
	}
	f.Close();
	if(!zsql.OpenDB(strPath))
	{
		AfxMessageBox(_T("���ݿ�򿪳���!"));
		return;
	}
	CString strSQL,strError;
	if(vec_strData.size()==1&&vec_strData[0].IsEmpty())
		strSQL.Format(_T("SELECT DataItem,DataName,IsRead,IsWrite FROM DataItem645 ORDER BY FstClass,SecClass"));
	else if(vec_strData.size()==1&&!vec_strData[0].IsEmpty())
		strSQL.Format(_T("SELECT DataItem,DataName,IsRead,IsWrite FROM DataItem645 WHERE FstClass=%d ORDER BY FstClass,SecClass"),_ttoi(vec_strData[0]));
	else
		strSQL.Format(_T("SELECT DataItem,DataName,IsRead,IsWrite FROM DataItem645 WHERE FstClass=%d AND SecClass=%d ORDER BY FstClass,SecClass"),_ttoi(vec_strData[0]),_ttoi(vec_strData[1]));
	std::vector<std::vector <CString>> vec2_strData;
	zsql.GetTable(strSQL,vec2_strData,&strError);
	zsql.CloseDB();
	int nRow=vec2_strData.size();
	CString strRow;
	if(nRow)
	{
		int nColumn=vec2_strData[0].size();
		if(nColumn==4)
		{
			for(int i=0;i<nRow-1;++i)
			{
				strRow.Format(_T("%d"),i+1);
				m_vec_listItem[nIndex]->InsertItem(i,strRow);
				m_vec_listItem[nIndex]->SetItemText(i,1,vec2_strData[i+1][0]);
				m_vec_listItem[nIndex]->SetItemText(i,2,vec2_strData[i+1][1]);
			}
		}
	}
}

void CMeterTesting::OnBnClickedCheckSelectall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_nIndexOfItem==-1)
		return;
	int nCount=m_vec_listItem[m_nIndexOfItem]->GetItemCount();
	for(int i=0;i<nCount;++i)
	{
		m_vec_listItem[m_nIndexOfItem]->SetCheck(i,m_checkSelectAll.GetCheck());
	}
}


void CMeterTesting::OnBnClickedButtonSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_nIndexOfItem==-1)
		return;
	Select(TRUE);
}


void CMeterTesting::OnBnClickedButtonUnselect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_nIndexOfItem==-1)
		return;
	Select(FALSE);
}

void CMeterTesting::Select(const BOOL & nIsSelect)
{
	int nCount,nStart,nEnd;
	nCount=m_vec_listItem[m_nIndexOfItem]->GetItemCount();
	nStart=GetDlgItemInt(IDC_EDIT_STARTINDEX);
	nEnd=GetDlgItemInt(IDC_EDIT_ENDINDEX);
	--nStart;
	nStart=nStart>0?nStart:0;
	nEnd=nEnd>nCount?nCount:nEnd;
	for(int i=nStart;i<nEnd;++i)
	{
		m_vec_listItem[m_nIndexOfItem]->SetCheck(i,nIsSelect);
	}
}

void CMeterTesting::OnBnClickedButtonReadmeter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_v.g_strFuncCode=_T("11");
	if(!g_nTotalPort)
	{
		AfxMessageBox(_T("����δ��!"));
		return;
	}
	if(m_nIndexOfItem==-1)
	{
		AfxMessageBox(_T("��Ŀδѡ��!"));
		return;
	}
	int nPortIndex=m_comboPortSelected.GetCurSel();
	if(nPortIndex!=CB_ERR)
	{
		int nCount=m_vec_listItem[m_nIndexOfItem]->GetItemCount();
		if(GetFirstRow()<nCount)
		{
			SetBlank();
			g_vec_md[nPortIndex].m_nProtocolType = CMainData::PROTOCOL_DLT645;
			g_vec_md[nPortIndex].m_p_protl=g_vec_md[nPortIndex].m_p_dlt645;
			g_vec_md[nPortIndex].m_pf_checkcall=ZDLT645_2007::Check645_2007;
			g_vec_md[nPortIndex].m_p_WndOfSource=this;
			g_vec_md[nPortIndex].m_nRow=GetFirstRow();
			g_vec_md[nPortIndex].m_nRowMax=nCount;
			SetEvent(g_vec_md[nPortIndex].m_hEvtThread);
			for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
			{
				if(g_sz_p_wnd[i])
					g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,FALSE,MSGUSER_ENABLEBTN);
			}
			g_bIsStop=false;
		}
		else
			AfxMessageBox(_T("�޲�����!"));
	}
}


void CMeterTesting::OnBnClickedButtonSetmeter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_v.g_strFuncCode=_T("14");
	if(!g_nTotalPort)
	{
		AfxMessageBox(_T("����δ��!"));
		return;
	}
	if(m_nIndexOfItem==-1)
	{
		AfxMessageBox(_T("��Ŀδѡ��!"));
		return;
	}
	int nPortIndex=m_comboPortSelected.GetCurSel();
	if(nPortIndex!=CB_ERR)
	{
		int nCount=m_vec_listItem[m_nIndexOfItem]->GetItemCount();
		if(GetFirstRow()<nCount)
		{
			g_vec_md[nPortIndex].m_nProtocolType = CMainData::PROTOCOL_DLT645;
			g_vec_md[nPortIndex].m_p_protl=g_vec_md[nPortIndex].m_p_dlt645;
			g_vec_md[nPortIndex].m_pf_checkcall=ZDLT645_2007::Check645_2007;
			g_vec_md[nPortIndex].m_p_WndOfSource=this;
			g_vec_md[nPortIndex].m_nRow=GetFirstRow();
			g_vec_md[nPortIndex].m_nRowMax=nCount;
			SetEvent(g_vec_md[nPortIndex].m_hEvtThread);
			for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
			{
				if(g_sz_p_wnd[i])
					g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,FALSE,MSGUSER_ENABLEBTN);
			}
			g_bIsStop=false;
		}
		else
			AfxMessageBox(_T("�޲�����!"));
	}
}


void CMeterTesting::OnBnClickedButtonStop()
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

void CMeterTesting::SetBlank(void)
{
	int nCount=m_vec_listItem[m_nIndexOfItem]->GetItemCount();
	for(int i=0;i<nCount;++i)
	{
		if(m_vec_listItem[m_nIndexOfItem]->GetCheck(i))
		{
			m_vec_listItem[m_nIndexOfItem]->SetItemText(i,3,_T(""));
			m_vec_listItem[m_nIndexOfItem]->SetItemText(i,4,_T(""));
		}
	}
}

int CMeterTesting::GetFirstRow(void)
{
	int nCount=m_vec_listItem[m_nIndexOfItem]->GetItemCount();
	int i=0;
	for(;i<nCount;++i)
	{
		if(m_vec_listItem[m_nIndexOfItem]->GetCheck(i))
			return i;
	}
	return i;
}

void CMeterTesting::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_bIsInitFinish&&nType!=SIZE_MINIMIZED)
	{
		CRect rc;
		GetClientRect(&rc);
		ChangeSize(IDCANCEL,rc); 
		ChangeSize(IDOK,rc); 
		ChangeSize(IDC_TREE_ITEMCLASS,rc);
		ChangeSize(IDC_BUTTON_STOP,rc); 
		ChangeSize(IDC_BUTTON_SETMETER,rc); 
		ChangeSize(IDC_BUTTON_READMETER,rc); 
		ChangeSize(IDC_COMBO_PORTSELECTED,rc); 
		ChangeSize(IDC_BUTTON_UNSELECT,rc); 
		ChangeSize(IDC_STATIC_SPLIT,rc); 
		ChangeSize(IDC_BUTTON_SELECT,rc); 
		ChangeSize(IDC_EDIT_ENDINDEX,rc); 
		ChangeSize(IDC_STATIC_TO,rc); 
		ChangeSize(IDC_EDIT_STARTINDEX,rc); 
		ChangeSize(IDC_CHECK_SELECTALL,rc); 
		if(m_nIndexOfItem!=-1)
			OperatorList(m_nIndexOfItem);  
		InitButton();
		Invalidate();
	}
}

void CMeterTesting::ChangeSize(UINT nID,const CRect & rcParent)
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
		case IDCANCEL:
			{
				rc.right=rcParent.right-10;
				rc.left=rc.right-nWidth;
				rc.bottom=rcParent.bottom-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDOK:
			{
				rc.right=rcLast.left-10;
				rc.left=rc.right-nWidth;
				rc.bottom=rcParent.bottom-10;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_TREE_ITEMCLASS:
			{
				rc.bottom=rcLast.top-10;
			}
			break;
		case IDC_STATIC_TO:
			{
				rc.bottom=rcParent.bottom-15;
				rc.top=rc.bottom-nHeight;
			}
			break;
		case IDC_STATIC_SPLIT:
			{
				rc.bottom=rcParent.bottom-15;
				rc.top=rc.bottom-nHeight;
			}
			break;
		default:
			{
				rc.bottom=rcParent.bottom-10;
				rc.top=rc.bottom-nHeight;
			}
		}
		rcLast=rc;
		pWnd->MoveWindow(rc,FALSE);
	}
}

void CMeterTesting::OperatorList(const int & nIndex)
{
	CRect rc,rcList;
	GetClientRect(&rc);
	m_treeItemClass.GetWindowRect(&rcList);
	ScreenToClient(&rcList);
	rcList.left=rcList.right+10;
	rcList.right=rc.right-10;
	m_vec_listItem[nIndex]->MoveWindow(rcList);
	m_vec_listItem[nIndex]->SetColumnWidth(0,rcList.Width()*2/18);   
	m_vec_listItem[nIndex]->SetColumnWidth(1,rcList.Width()*2/18); 
	m_vec_listItem[nIndex]->SetColumnWidth(2,rcList.Width()*5/18);   
	m_vec_listItem[nIndex]->SetColumnWidth(3,rcList.Width()*4/18); 
	m_vec_listItem[nIndex]->SetColumnWidth(4,rcList.Width()*4/18); 
}


afx_msg LRESULT CMeterTesting::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case MSGUSER_GETSENDDATA:
		{
			CMainData * p_md=(CMainData *)wParam;
			CString strDataItem,strTgtData;
			g_v.g_strDeviceAddr=_T("");
			g_v.g_strDataItem=m_vec_listItem[m_nIndexOfItem]->GetItemText(p_md->m_nRow,1);
			g_v.g_strTgtData=m_vec_listItem[m_nIndexOfItem]->GetItemText(p_md->m_nRow,3);
			p_md->m_p_dlt645->Set15Param(g_v.Get645Param());
			m_vec_listItem[m_nIndexOfItem]->EnsureVisible(p_md->m_nRow, FALSE);
		}
		break;
	case MSGUSER_ENCODE:
		{
			CMainData * p_md=(CMainData *)wParam;
			CString strText;
			switch(p_md->m_nRtn)
			{
			case 0:
				break;
			default:
				strText=p_md->m_strError;
			}
			if(p_md->m_nRtn)
			{
				m_vec_listItem[m_nIndexOfItem]->SetItemText(p_md->m_nRow,3,strText);
				m_vec_listItem[m_nIndexOfItem]->EnsureVisible(p_md->m_nRow, FALSE);
			}
		}
		break;
	case MSGUSER_UPDATESENDDATA:
		{
			CMainData * p_md=(CMainData *)wParam;
			CString strText;
			switch(p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
				break;
			case ZDevicePort::ERROR_INVALIDPORT:
				strText=_T("����δ��!");
				break;
			case ZDevicePort::ERROR_SEND_TIMEOUT:
				strText=_T("���ڷ��ͳ�ʱ!");
				break;
			case ZDevicePort::ERROR_SEND_STOP:
				strText=_T("������ֹͣ!");
				break;
			default:
				strText=_T("���ڷ���ʧ��!");
			}
			if(p_md->m_nRtn)
			{
				m_vec_listItem[m_nIndexOfItem]->SetItemText(p_md->m_nRow,3,strText);
				m_vec_listItem[m_nIndexOfItem]->EnsureVisible(p_md->m_nRow, FALSE);
			}
		}
		break;
	case MSGUSER_UPDATERECVDATA:
		{
			CMainData * p_md=(CMainData *)wParam;
			CString strText;
			switch(p_md->m_nRtn)
			{
			case ZDevicePort::ERROR_OK:
				{
					strText=p_md->m_strDataValue;
					if(strText.Right(3)==_T("91H"))
						strText=strText.Left(strText.Find(_T('@')));
				}
				break;
			case ZDevicePort::ERROR_INVALIDPORT:
				strText=_T("����δ��!");
				break;
			case ZDevicePort::ERROR_RECV_TIMEOUT:
				strText=_T("���ڽ��ճ�ʱ!");
				break;
			case ZDevicePort::ERROR_RECV_STOP:
				strText=_T("������ֹͣ!");
				break;
			default:
				strText=_T("���ڽ���ʧ��!");
			}
			m_vec_listItem[m_nIndexOfItem]->SetItemText(p_md->m_nRow,3,strText);
			m_vec_listItem[m_nIndexOfItem]->EnsureVisible(p_md->m_nRow, FALSE);
			if(p_md->m_nRtn==0)
			{
				CString strTextDecode=ExecResult(m_vec_listItem[m_nIndexOfItem]->GetItemText(p_md->m_nRow,1),strText);
				m_vec_listItem[m_nIndexOfItem]->SetItemText(p_md->m_nRow,4,strTextDecode);
			}
		}
		break;
	case MSGUSER_ENDONCELOOP:
		{
			CMainData * p_md=(CMainData *)wParam;
			if(!g_bIsStop)
			{
				int nRowNext=p_md->m_nRow+1;
				if(nRowNext<p_md->m_nRowMax)
				{
					int i=nRowNext;
					for(;i<p_md->m_nRowMax;++i)
					{
						if(m_vec_listItem[m_nIndexOfItem]->GetCheck(i))
							break;
					}
					p_md->m_nRow=i-1;
					if(i==p_md->m_nRowMax)
					{
						for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
						{
							if(g_sz_p_wnd[i])
								g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,TRUE,MSGUSER_ENABLEBTN);
						}
					}
				}
				else
				{
					for(int i=0;i<sizeof(g_sz_p_wnd)/sizeof(CWnd *);++i)
					{
						if(g_sz_p_wnd[i])
							g_sz_p_wnd[i]->PostMessage(WM_MSGRECVPRO,TRUE,MSGUSER_ENABLEBTN);
					}
				}
			}
		}
		break;
	case MSGUSER_PORTOPEN:
	case MSGUSER_PORTCLOSE:
		{
			InsertCombo();
		}
		break;
	case MSGUSER_ENABLEBTN:
		{
			EnableBtn(wParam);
		}
		break;
	}
	return 0;
}
void CMeterTesting::InsertCombo(void)
{
	m_comboPortSelected.ResetContent();
	CString str;
	for(int i=g_nFstPort;i<g_nFstPort+g_nTotalPort;++i)
	{
		str.Format(_T("COM%d"),i);
		m_comboPortSelected.AddString(str);
	}
	if(g_nTotalPort)
		m_comboPortSelected.SetCurSel(0);
}

void CMeterTesting::EnableBtn(const BOOL & bEnable)
{
	m_treeItemClass.EnableWindow(bEnable);
	m_comboPortSelected.EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_READMETER)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_SETMETER)->EnableWindow(bEnable);
}


BOOL CMeterTesting::OnEraseBkgnd(CDC* pDC)
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
	return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH CMeterTesting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	static CBrush brush;
	if(brush.m_hObject==NULL)
		brush.CreateSolidBrush(RGB(230,248,226)); 
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		{
			if(pWnd->GetDlgCtrlID()!=IDC_TREE_ITEMCLASS)
			{
				if(pWnd->GetDlgCtrlID()!=IDC_CHECK_SELECTALL)
				{
					pDC->SetBkMode(TRANSPARENT);
					return (HBRUSH)GetStockObject(NULL_BRUSH);
				}
				else
					return brush;
			}
		}
		break;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CMeterTesting::InitButton(void)
{
	SetBtnColor(m_btnSelect,_T(""));
	SetBtnColor(m_btnUnselect,_T(""));
	SetBtnColor(m_btnReadMeter,_T(""));
	SetBtnColor(m_btnSetMeter,_T(""));
	SetBtnColor(m_btnStop,_T(""));
	SetBtnColor(m_btnOK,_T(""));
	SetBtnColor(m_btnCancel,_T(""));
	m_btnSelect.SetFlat(TRUE);
	m_btnUnselect.SetFlat(TRUE);
	m_btnSelect.DrawBorder(TRUE);
	m_btnUnselect.DrawBorder(TRUE);
}

void CMeterTesting::SetBtnColor(ZButton & btn,const CString & strTip)
{
	//btn.DrawTransparent(TRUE);
	btn.SetColor(ZButton::COLOR_BK_IN,RGB(230,248,226));
	btn.SetColor(ZButton::COLOR_BK_OUT,RGB(230,248,226));
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

void CMeterTesting::OnNMDblclkListItem(UINT nID, NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if(pNMItemActivate->iItem!=-1)  
	{   
		static CString strInfo,strText;
		strInfo=m_vec_listItem[m_nIndexOfItem]->GetItemText(pNMItemActivate->iItem,1);
		strInfo+=_T("___");
		strInfo+=m_vec_listItem[m_nIndexOfItem]->GetItemText(pNMItemActivate->iItem,2);
		strText=m_vec_listItem[m_nIndexOfItem]->GetItemText(pNMItemActivate->iItem,3);
		strText+=_T("\r\n   ----------------------------------------------------------------------------------\r\n");
		strText+=m_vec_listItem[m_nIndexOfItem]->GetItemText(pNMItemActivate->iItem,4);
		CShowResultOfMT showresult;
		showresult.SetParam(strInfo,strText);
		showresult.DoModal();
	}
}

CString CMeterTesting::ExecResult(const CString & strDataItem,const CString &strData)
{
	if(strData==_T("ErrorData-D1H")||strData==_T("ErrorData-D4H"))
		return CString(_T(""));
	ZSqlite3 zsql;
	CString strPath=GetExeCatalogPath_Z()+_T("\\res\\DataItem.di");
	CStdioFile f;
	if(!f.Open(strPath, CFile::modeRead))
		return CString(_T(""));
	f.Close();
	if(!zsql.OpenDB(strPath))
		return CString(_T(""));
	CString strSQL,strError;
	strSQL.Format(_T("SELECT FstClass,SecClass,DataFormat,DataLen,Unit,IsASCII FROM DataItem645 WHERE DataItem='%s'"),strDataItem);
	std::vector<std::vector <CString>> vec2_strData;
	zsql.GetTable(strSQL,vec2_strData,&strError);
	int nRow=vec2_strData.size();
	int nFstClass,nSecClass;
	if(nRow==2)
	{
		int nColumn=vec2_strData[0].size();
		if(nColumn==6)
		{
			nFstClass=_ttoi(vec2_strData[1][0]);
			nSecClass=_ttoi(vec2_strData[1][1]);
		}
	}
	else
		return CString(_T(""));
	CString strRtn,strTemp;
	int nLength=strData.GetLength();
	switch(nFstClass)
	{
	case 0:
		{
		}
		break;
	case 1:
		{
		}
		break;
	case 2:
		{
		}
		break;
	case 3:
		{
			switch(nSecClass)
			{
			case 0:
				{
					if(strDataItem==_T("03110000"))
						break;
					strRtn=_T("����ʱ�̣�")+Str2TimeFormat(strData.Right(12))+_T("\r\n");
					nLength-=12;
					strRtn+=_T("����ʱ�̣�")+Str2TimeFormat(strData.Mid(nLength-12,12))+_T("\r\n");
				}
				break;
			case 1:
				{
					if(strDataItem==_T("03300000"))
						break;
					strRtn=_T("����ʱ�̣�")+Str2TimeFormat(strData.Right(12))+_T("\r\n");
					nLength-=12;
					strRtn+=_T("�����ߴ��룺")+strData.Mid(nLength-8,8)+_T("\r\n");
					nLength-=8;
					for(int i=0;i<10;++i)
					{
						strTemp.Format(_T("��̵���%d�����ݱ�ʶ:"),i+1);
						strRtn+=strTemp+strData.Mid(nLength-8,8)+_T("\r\n");
						nLength-=8;
					}
				}
				break;
			case 2:
				{
					if(strDataItem==_T("03300100"))
						break;
					strRtn=_T("����ʱ�̣�")+Str2TimeFormat(strData.Right(12))+_T("\r\n");
					nLength-=12;
					strRtn+=_T("�����ߴ��룺")+strData.Mid(nLength-8,8)+_T("\r\n");
					nLength-=8;
					const CString strText[]=
					{
						_T("�������ǰ�����й��ܵ��ܣ�"),
						_T("�������ǰ�����й��ܵ��ܣ�"),
						_T("�������ǰ��һ�����޹��ܵ��ܣ�"),
						_T("�������ǰ�ڶ������޹��ܵ��ܣ�"),
						_T("�������ǰ���������޹��ܵ��ܣ�"),
						_T("�������ǰ���������޹��ܵ��ܣ�"),
						_T("�������ǰA�������й����ܣ�"),
						_T("�������ǰA�෴���й����ܣ�"),
						_T("�������ǰA���һ�����޹����ܣ�"),
						_T("�������ǰA��ڶ������޹����ܣ�"),
						_T("�������ǰA����������޹����ܣ�"),
						_T("�������ǰA����������޹����ܣ�"),
						_T("�������ǰB�������й����ܣ�"),
						_T("�������ǰB�෴���й����ܣ�"),
						_T("�������ǰB���һ�����޹����ܣ�"),
						_T("�������ǰB��ڶ������޹����ܣ�"),
						_T("�������ǰB����������޹����ܣ�"),
						_T("�������ǰB����������޹����ܣ�"),
						_T("�������ǰC�������й����ܣ�"),
						_T("�������ǰC�෴���й����ܣ�"),
						_T("�������ǰC���һ�����޹����ܣ�"),
						_T("�������ǰC��ڶ������޹����ܣ�"),
						_T("�������ǰC����������޹����ܣ�"),
						_T("�������ǰC����������޹����ܣ�")
					};
					for(int i=0;i<24;++i)
					{
						strRtn+=strText[i]+strData.Mid(nLength-8,6)+_T(".")+strData.Mid(nLength-2,2);
						if(strText[i].Find(_T("�й�"))>=0)
							strRtn+=_T(" kWh\r\n");
						else
							strRtn+=_T(" kvarh\r\n");
						nLength-=8;
					}
				}
				break;
			case 3:
				{
					if(strDataItem==_T("03300400"))
						break;
					strRtn=_T("�����ߴ��룺")+strData.Right(8)+_T("\r\n");
					nLength-=8;
					strRtn+=_T("Уʱǰʱ�䣺")+Str2TimeFormat(strData.Mid(nLength-12,12))+_T("\r\n");
					nLength-=12;
					strRtn+=_T("Уʱ��ʱ�䣺")+Str2TimeFormat(strData.Mid(nLength-12,12))+_T("\r\n");
				}
				break;
			case 4:
				{
					if(strDataItem==_T("03300D00"))
						break;
					strRtn=_T("����ʱ�̣�")+Str2TimeFormat(strData.Right(12))+_T("\r\n");
					nLength-=12;
					strRtn+=_T("����ʱ�̣�")+Str2TimeFormat(strData.Mid(nLength-12,12))+_T("\r\n");
					nLength-=12;
					const CString strText[]=
					{
						_T("�����ǰ�����й��ܵ��ܣ�"),
						_T("�����ǰ�����й��ܵ��ܣ�"),
						_T("�����ǰ��һ�����޹��ܵ��ܣ�"),
						_T("�����ǰ�ڶ������޹��ܵ��ܣ�"),
						_T("�����ǰ���������޹��ܵ��ܣ�"),
						_T("�����ǰ���������޹��ܵ��ܣ�"),
						_T("����Ǻ������й��ܵ��ܣ�"),
						_T("����Ǻ����й��ܵ��ܣ�"),
						_T("����Ǻ��һ�����޹��ܵ��ܣ�"),
						_T("����Ǻ�ڶ������޹��ܵ��ܣ�"),
						_T("����Ǻ���������޹��ܵ��ܣ�"),
						_T("����Ǻ���������޹��ܵ��ܣ�")
					};
					for(int i=0;i<12;++i)
					{
						strRtn+=strText[i]+strData.Mid(nLength-8,6)+_T(".")+strData.Mid(nLength-2,2);
						if(strText[i].Find(_T("�й�"))>=0)
							strRtn+=_T(" kWh\r\n");
						else
							strRtn+=_T(" kvarh\r\n");
						nLength-=8;
					}
				}
				break;
			case 5:
				{
					if(strDataItem==_T("1D000001"))
						break;
					strRtn=_T("����ʱ�̣�")+Str2TimeFormat(strData.Right(12))+_T("\r\n");
					nLength-=12;
					strRtn+=_T("�����ߴ��룺")+strData.Mid(nLength-8,8)+_T("\r\n");
					nLength-=8;
					const CString strText[]=
					{
						_T("��բʱ�����й��ܵ��ܣ�"),
						_T("��բʱ�����й��ܵ��ܣ�"),
						_T("��բʱ��һ�����޹��ܵ��ܣ�"),
						_T("��բʱ�ڶ������޹��ܵ��ܣ�"),
						_T("��բʱ���������޹��ܵ��ܣ�"),
						_T("��բʱ���������޹��ܵ��ܣ�")
					};
					for(int i=0;i<6;++i)
					{
						strRtn+=strText[i]+strData.Mid(nLength-8,6)+_T(".")+strData.Mid(nLength-2,2);
						if(strText[i].Find(_T("�й�"))>=0)
							strRtn+=_T(" kWh\r\n");
						else
							strRtn+=_T(" kvarh\r\n");
						nLength-=8;
					}
				}
				break;
			case 6:
				{
					if(strDataItem==_T("1E000001"))
						break;
					strRtn=_T("����ʱ�̣�")+Str2TimeFormat(strData.Right(12))+_T("\r\n");
					nLength-=12;
					strRtn+=_T("�����ߴ��룺")+strData.Mid(nLength-8,8)+_T("\r\n");
					nLength-=8;
					const CString strText[]=
					{
						_T("��բʱ�����й��ܵ��ܣ�"),
						_T("��բʱ�����й��ܵ��ܣ�"),
						_T("��բʱ��һ�����޹��ܵ��ܣ�"),
						_T("��բʱ�ڶ������޹��ܵ��ܣ�"),
						_T("��բʱ���������޹��ܵ��ܣ�"),
						_T("��բʱ���������޹��ܵ��ܣ�")
					};
					for(int i=0;i<6;++i)
					{
						strRtn+=strText[i]+strData.Mid(nLength-8,6)+_T(".")+strData.Mid(nLength-2,2);
						if(strText[i].Find(_T("�й�"))>=0)
							strRtn+=_T(" kWh\r\n");
						else
							strRtn+=_T(" kvarh\r\n");
						nLength-=8;
					}
				}
				break;
			}
		}
		break;
	case 4:
		{
			switch(nSecClass)
			{
			case 0:
				{
					if(_ttoi(vec2_strData[1][5]))
					{
						int nSize=strData.GetLength();
						int nTemp=0;
						for(int i=0;i<nSize;i=i+2)
						{
							nTemp=_tcstol(strData.Mid(i,2),NULL,16);
							strTemp.Format(_T("%c"),nTemp);
							strRtn+=strTemp;
						}
					}
				}
				break;
			case 1:
				{
					if(strDataItem==_T("04010000")||strDataItem==_T("04020000"))
						break;
					int nSize=nLength/6;
					for(int i=0;i<nSize;++i)
					{
						strTemp.Format(_T("��%dʱ����ʼʱ�估���ʺ�:"),i+1);
						strRtn+=strTemp+strData.Mid(nLength-6,2)+_T(":")+strData.Mid(nLength-4,2)+_T("  ")+strData.Mid(nLength-2,2)+_T("���ʺ�\r\n");
						nLength-=6;
					}
				}
				break;
			case 3:
				{
					strSQL.Format(_T("SELECT DataName FROM DataItem645 WHERE DataItem='%s'"),strData.Right(8));
					std::vector<std::vector <CString>> vec2_strDisplay;
					zsql.GetTable(strSQL,vec2_strDisplay,&strError);
					if(vec2_strDisplay.size())
					{
						strRtn=vec2_strDisplay[1][0];
					}
				}
				break;
			}
		}
		break;
	case 5:
		{
			CString strTime=Str2TimeFormat(strData.Right(10)+_T("00"));
			strTime=strTime.Mid(0,strTime.GetLength()-3);
			strRtn=_T("����ʱ�䣺")+strTime+_T("\r\n");
			nLength-=10;
			nLength-=2;
			const CString strText[]=
			{
				_T("�����й��ܵ��ܣ�"),
				_T("�����й�����1���ܣ�"),
				_T("�����й�����2���ܣ�"),
				_T("�����й�����3���ܣ�"),
				_T("�����й�����4���ܣ�"),
				_T("�����й��ܵ��ܣ�"),
				_T("�����й�����1���ܣ�"),
				_T("�����й�����2���ܣ�"),
				_T("�����й�����3���ܣ�"),
				_T("�����й�����4���ܣ�"),
			};
			for(int j=0;j<2;++j)
			{
				for(int i=0;i<5;++i)
				{
					strRtn+=strText[i]+strData.Mid(nLength-8,6)+_T(".")+strData.Mid(nLength-2,2);
					if(strText[i].Find(_T("�й�"))>=0)
						strRtn+=_T(" kWh\r\n");
					else
						strRtn+=_T(" kvarh\r\n");
					nLength-=8;
				}
				nLength-=2;
			}
		}
		break;
	case 6:
		{
		}
		break;
	case 7:
		{
		}
		break;
	case 8:
		{
		}
		break;
	case 9:
		{
		}
		break;
	}
	if(strRtn.IsEmpty())
	{
		strRtn=strData;
		DataChange(strRtn,vec2_strData[1][2],vec2_strData[1][4]);
	}
	return strRtn;
}

void CMeterTesting::DataChange(CString & str,const CString & strFormat,const CString & strUnit)
{
	int nLen=strFormat.GetLength();
	for(int i=0;i<nLen;++i)
	{
		if(strFormat[i]==_T('.')||strFormat[i]==_T('/'))
			str.Insert(i,strFormat[i]);
	}
	std::vector<CString> vec_strData;
	StrSplit_Z(str,vec_strData,_T('/'),str.Right(1)==_T('/'));
	int nSize=vec_strData.size();
	str.Empty();
	for(int i=nSize-1;i>=0;--i)
	{
		str+=vec_strData[i]+_T(" ")+strUnit+_T(" \r\n");
	}
}

CString CMeterTesting::Str2TimeFormat(const CString & str)
{
	CString strRtn;
	strRtn=str.Mid(0,2)+_T("-")+str.Mid(2,2)+_T("-")+str.Mid(4,2)+_T(" ")+str.Mid(6,2)+_T(":")+str.Mid(8,2)+_T(":")+str.Mid(10,2);
	return strRtn;
}


void CMeterTesting::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ˴������Ϣ����������
}
