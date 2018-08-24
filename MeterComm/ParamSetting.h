#pragma once

// CParamSetting �Ի���

class CParamSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CParamSetting)

public:
	CParamSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParamSetting();

// �Ի�������
	enum { IDD = IDD_PARAMSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_strGrade;
	CString m_strPsWd;
	CString m_strOperaCode;
	ZButton m_btnOK;
	ZButton m_btnCancel;
public:
	virtual BOOL OnInitDialog();
	void SetParam(const CString & strGrade, const CString & strPsWd, const CString & strOperaCode);
	void GetParam(CString & strGrade, CString & strPsWd, CString & strOperaCode);
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
