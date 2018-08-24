#pragma once
#include "afxwin.h"

// CSoftwareUpdate �Ի���

class CSoftwareUpdate : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftwareUpdate)

public:
	CSoftwareUpdate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSoftwareUpdate();

// �Ի�������
	enum { IDD = IDD_SOFTWAREUPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hEvtExitCheckUpdate;
	ZButton m_btnOK;
	ZButton m_btnCancel;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	static UINT CheckSoftwareUpdateThreadFunc(LPVOID lpParam);
	static bool SocketCommunication(const CString & strDataIn,CString & strDataOut);
	afx_msg void OnBnClickedOk();
	void SoftWareUpdate(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
