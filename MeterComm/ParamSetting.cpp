// ParamSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MeterComm.h"
#include "ParamSetting.h"
#include "afxdialogex.h"


// CParamSetting �Ի���

IMPLEMENT_DYNAMIC(CParamSetting, CDialogEx)

CParamSetting::CParamSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParamSetting::IDD, pParent)
	, m_strGrade(_T("02"))
	, m_strPsWd(_T("123456"))
	, m_strOperaCode(_T("00000000"))
{

}

CParamSetting::~CParamSetting()
{
}

void CParamSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CParamSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CParamSetting::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CParamSetting ��Ϣ�������


BOOL CParamSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemText(IDC_EDIT_GRADE,m_strGrade);
	SetDlgItemText(IDC_EDIT_PSWD,m_strPsWd);
	SetDlgItemText(IDC_EDIT_OPERATORCODE,m_strOperaCode);
	InitButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CParamSetting::SetParam(const CString & strGrade, const CString & strPsWd, const CString & strOperaCode)
{
	m_strGrade=strGrade;
	m_strPsWd=strPsWd;
	m_strOperaCode=strOperaCode;
}

void CParamSetting::GetParam(CString & strGrade, CString & strPsWd, CString & strOperaCode)
{
	strGrade=m_strGrade;
	strPsWd=m_strPsWd;
	strOperaCode=m_strOperaCode;
}

void CParamSetting::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strZero(_T('0'),8);
	GetDlgItemText(IDC_EDIT_GRADE,m_strGrade);
	if(m_strGrade.GetLength()<2)
		m_strGrade=strZero.Left(2-m_strGrade.GetLength())+m_strGrade;
	m_strGrade=m_strGrade.Left(2);
	GetDlgItemText(IDC_EDIT_PSWD,m_strPsWd);
	if(m_strPsWd.GetLength()<6)
		m_strPsWd=strZero.Left(6-m_strPsWd.GetLength())+m_strPsWd;
	m_strPsWd=m_strPsWd.Left(6);
	GetDlgItemText(IDC_EDIT_OPERATORCODE,m_strOperaCode);
	if(m_strOperaCode.GetLength()<8)
		m_strOperaCode=strZero.Left(8-m_strOperaCode.GetLength())+m_strOperaCode;
	m_strOperaCode=m_strOperaCode.Left(8);
	CDialogEx::OnOK();
}


BOOL CParamSetting::OnEraseBkgnd(CDC* pDC)
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

void CParamSetting::InitButton(void)
{
	SetBtnColor(m_btnOK,_T(""));
	SetBtnColor(m_btnCancel,_T(""));
}

void CParamSetting::SetBtnColor(ZButton & btn,const CString & strTip)
{
	btn.DrawTransparent(TRUE);
	btn.DrawBorder(FALSE);
	btn.SetFlat(FALSE);
	btn.SetTooltipText(strTip);
}

HBRUSH CParamSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
