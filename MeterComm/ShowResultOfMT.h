#pragma once


// CShowResultOfMT �Ի���

class CShowResultOfMT : public CDialogEx
{
	DECLARE_DYNAMIC(CShowResultOfMT)

public:
	CShowResultOfMT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowResultOfMT();

// �Ի�������
	enum { IDD = IDD_SHOWRESULTOFMT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_strInfo;
	CString m_strText;
public:
	virtual BOOL OnInitDialog();
	void SetParam(const CString & strInfo, const CString & strText);
};
