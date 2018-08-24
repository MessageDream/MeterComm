// ShowResultOfMT.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MeterComm.h"
#include "ShowResultOfMT.h"
#include "afxdialogex.h"


// CShowResultOfMT �Ի���

IMPLEMENT_DYNAMIC(CShowResultOfMT, CDialogEx)

CShowResultOfMT::CShowResultOfMT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowResultOfMT::IDD, pParent)
	, m_strInfo(_T(""))
	, m_strText(_T(""))
{

}

CShowResultOfMT::~CShowResultOfMT()
{
}

void CShowResultOfMT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowResultOfMT, CDialogEx)
END_MESSAGE_MAP()


// CShowResultOfMT ��Ϣ�������


BOOL CShowResultOfMT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemText(IDC_STATIC_INFO,m_strInfo);
	SetDlgItemText(IDC_EDIT_RESULT,m_strText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CShowResultOfMT::SetParam(const CString & strInfo, const CString & strText)
{
	m_strInfo=strInfo;
	m_strText=strText;
}
