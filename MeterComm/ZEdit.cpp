// ZEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZEdit.h"
#include "ZListCtrl.h"

// ZEdit

IMPLEMENT_DYNAMIC(ZEdit, CEdit)

ZEdit::ZEdit()
{

}

ZEdit::~ZEdit()
{

}


BEGIN_MESSAGE_MAP(ZEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// ZEdit ��Ϣ�������




void ZEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: �ڴ˴������Ϣ����������
	GetParent()->PostMessage(WM_EDITKILLFOCUS,0,0);
}
