// DataItem.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MeterComm.h"
#include "DataItem.h"
#include "afxdialogex.h"

// CDataItem �Ի���

IMPLEMENT_DYNAMIC(CDataItem, CDialogEx)

CDataItem::CDataItem(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataItem::IDD, pParent)
	, m_strDataItem(_T(""))
	, m_bIsInitFinish(false)
{

}

CDataItem::~CDataItem()
{
}

void CDataItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ITEMCLASS, m_treeItemClass);
	DDX_Control(pDX, IDC_LIST_ITEM, m_listItem);
	DDX_Control(pDX, IDC_LIST_SEARCH, m_listSearch);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_btnSearch);
}


BEGIN_MESSAGE_MAP(CDataItem, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDataItem::OnBnClickedButtonSearch)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ITEMCLASS, &CDataItem::OnTvnSelchangedTreeItemclass)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ITEM, &CDataItem::OnNMClickListItem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ITEM, &CDataItem::OnNMDblclkListItem)
	ON_BN_CLICKED(IDOK, &CDataItem::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SEARCH, &CDataItem::OnNMClickListSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SEARCH, &CDataItem::OnNMDblclkListSearch)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDataItem ��Ϣ�������


BOOL CDataItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitTree();
	InitList();
	InitButton();
	m_bIsInitFinish=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDataItem::OnBnClickedButtonSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listSearch.DeleteAllItems();
	CString strSearch;
	GetDlgItemText(IDC_EDIT_SEARCH,strSearch);
	strSearch.MakeUpper();
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
	strSQL.Format(_T("SELECT DataItem,DataName FROM DataItem645 WHERE DataItem LIKE '%%%s%%' OR DataName LIKE '%%%s%%' ORDER BY FstClass,SecClass"),strSearch,strSearch);
	std::vector<std::vector <CString>> vec2_strData;
	zsql.GetTable(strSQL,vec2_strData,&strError);
	zsql.CloseDB();
	int nRow=vec2_strData.size();
	if(nRow)
	{
		int nColumn=vec2_strData[0].size();
		for(int i=0;i<nRow-1;++i)
		{
			m_listSearch.InsertItem(i,vec2_strData[i+1][0]);
			m_listSearch.SetItemText(i,1,vec2_strData[i+1][1]);
		}
	}
}


void CDataItem::InitTree(void)
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
}

void CDataItem::ExpandTree(CTreeCtrl & tree,HTREEITEM hTreeItem)
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

void CDataItem::InitList(void)
{
	CRect rc;     
	m_listItem.GetClientRect(&rc);     
	m_listItem.SetExtendedStyle(m_listItem.GetExtendedStyle()|LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);     
	m_listItem.InsertColumn(0, _T("���ݱ�ʶ"), LVCFMT_LEFT, rc.Width()*4/15, 0);   
	m_listItem.InsertColumn(1, _T("��������"), LVCFMT_LEFT, rc.Width()*6/15, 1);  
	m_listItem.InsertColumn(2, _T("�Ƿ�ɶ�"), LVCFMT_LEFT, rc.Width()*2/15, 2);  
	m_listItem.InsertColumn(3, _T("�Ƿ��д"), LVCFMT_LEFT, rc.Width()*2/15, 3);  
	m_listItem.SetTextColor(RGB(192,64,0));
	m_listSearch.GetClientRect(&rc);     
	m_listSearch.SetExtendedStyle(m_listSearch.GetExtendedStyle()|LVS_EX_DOUBLEBUFFER|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);     
	m_listSearch.InsertColumn(0, _T("���ݱ�ʶ"), LVCFMT_LEFT, rc.Width()*4/11, 0);   
	m_listSearch.InsertColumn(1, _T("��������"), LVCFMT_LEFT, rc.Width()*6/11, 1); 
	m_listSearch.SetTextColor(RGB(192,64,0));
}


void CDataItem::OnTvnSelchangedTreeItemclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	HTREEITEM hItem = m_treeItemClass.GetSelectedItem();
	if(hItem)
	{
		int nIndex=m_treeItemClass.GetItemData(hItem);
		InsertList(nIndex);
	}
}

void CDataItem::InsertList(const int & nIndex)
{
	m_listItem.DeleteAllItems();
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
	if(nRow)
	{
		int nColumn=vec2_strData[0].size();
		if(nColumn==4)
		{
			for(int i=0;i<nRow-1;++i)
			{
				m_listItem.InsertItem(i,vec2_strData[i+1][0]);
				m_listItem.SetItemText(i,1,vec2_strData[i+1][1]);
				m_listItem.SetItemText(i,2,vec2_strData[i+1][2]);
				m_listItem.SetItemText(i,3,vec2_strData[i+1][3]);
			}
		}
	}
}

void CDataItem::OnNMClickListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CString strDataItem;
    if(pNMItemActivate->iItem!=-1)   
    {    
        strDataItem = m_listItem.GetItemText(pNMItemActivate->iItem,0);   
        SetDlgItemText(IDC_EDIT_SELECT,strDataItem);   
    }   
}


void CDataItem::OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CString strDataItem;
    if(pNMItemActivate->iItem!=-1)   
    {    
        strDataItem = m_listItem.GetItemText(pNMItemActivate->iItem,0);   
        SetDlgItemText(IDC_EDIT_SELECT,strDataItem); 
		OnBnClickedOk();
    }   
}


void CDataItem::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_EDIT_SELECT,m_strDataItem);
	CDialogEx::OnOK();
}


CString CDataItem::GetDataItem(void)
{
	return m_strDataItem;
}


void CDataItem::OnNMClickListSearch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CString strDataItem;
    if(pNMItemActivate->iItem!=-1)   
    {    
        strDataItem = m_listSearch.GetItemText(pNMItemActivate->iItem,0);   
        SetDlgItemText(IDC_EDIT_SELECT,strDataItem);   
    } 
}


void CDataItem::OnNMDblclkListSearch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CString strDataItem;
    if(pNMItemActivate->iItem!=-1)   
    {    
        strDataItem = m_listSearch.GetItemText(pNMItemActivate->iItem,0);   
        SetDlgItemText(IDC_EDIT_SELECT,strDataItem); 
		OnBnClickedOk();
    } 
}


BOOL CDataItem::OnEraseBkgnd(CDC* pDC)
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

void CDataItem::InitButton(void)
{
	SetBtnColor(m_btnSearch,_T(""));
	SetBtnColor(m_btnOK,_T(""));
	SetBtnColor(m_btnCancel,_T(""));
}

void CDataItem::SetBtnColor(ZButton & btn,const CString & strTip)
{
	btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

void CDataItem::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_bIsInitFinish&&nType!=SIZE_MINIMIZED)
	{
		CRect rc;
		GetClientRect(&rc);
		ChangeSize(IDCANCEL,rc); 
		ChangeSize(IDC_STATIC_SEARCH,rc); 
		ChangeSize(IDC_LIST_SEARCH,rc); 
		ChangeSize(IDC_BUTTON_SEARCH,rc); 
		ChangeSize(IDC_EDIT_SEARCH,rc); 
		ChangeSize(IDC_LIST_ITEM,rc); 
		ChangeSize(IDC_TREE_ITEMCLASS,rc); 
		CRect rcItem;     
		m_listItem.GetClientRect(&rcItem);      
		m_listItem.SetColumnWidth(0,rcItem.Width()*4/15);   
		m_listItem.SetColumnWidth(1,rcItem.Width()*6/15);  
		m_listItem.SetColumnWidth(2,rcItem.Width()*2/15);   
		m_listItem.SetColumnWidth(3,rcItem.Width()*2/15);  
		InitButton();
		Invalidate();
	}
}

void CDataItem::ChangeSize(UINT nID,const CRect & rcParent)
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
		case IDC_STATIC_SEARCH:
			{
				rc.right=rcParent.right-10;
				rc.left=rc.right-nWidth;
				rc.bottom=rcLast.top-10;
			}
			break;
		case IDC_LIST_SEARCH:
			{
				rc.right=rcLast.right-10;
				rc.left=rc.right-nWidth;
				rc.bottom=rcLast.bottom-10;
			}
			break;
		case IDC_BUTTON_SEARCH:
			{
				rc.right=rcLast.right;
				rc.left=rc.right-nWidth;
			}
			break;
		case IDC_EDIT_SEARCH:
			{
				rc.right=rcLast.left-10;
				rc.left=rc.right-nWidth;
			}
			break;
		case IDC_LIST_ITEM:
			{
				CRect rcGroup;
				GetDlgItem(IDC_STATIC_SEARCH)->GetWindowRect(&rcGroup);
				ScreenToClient(&rcGroup);
				rc.right=rcGroup.left-10;
				rc.bottom=rcGroup.bottom-10;
			}
			break;
		case IDC_TREE_ITEMCLASS:
			{
				rc.bottom=rcLast.bottom;
			}
			break;
		}
		rcLast=rc;
		pWnd->MoveWindow(rc,FALSE);
	}
}

HBRUSH CDataItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr =(HBRUSH)GetStockObject(NULL_BRUSH);
		}
		break;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
