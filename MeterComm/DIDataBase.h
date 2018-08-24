#pragma once
#include "zlistctrl.h"
#include "zbutton.h"

// CDIDataBase �Ի���

class CDIDataBase : public CDialogEx
{
	DECLARE_DYNAMIC(CDIDataBase)

public:
	CDIDataBase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDIDataBase();

// �Ի�������
	enum { IDD = IDD_DIDATABASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	ZListCtrl m_listItem;
	ZButton m_btnAdd;
	ZButton m_btnDelete;
	ZButton m_btnOK;
	ZButton m_btnCancel;
	bool m_bIsInitFinish;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	void InitList(void);
	void InsertList(void);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void InitButton(void);
	void SetBtnColor(ZButton & btn,const CString & strTip);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(UINT nID,const CRect & rcParent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
