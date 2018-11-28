#pragma once
#include "ZProtocol.h"
class ZRMTProtocol:public ZProtocol
{
private:
	CString m_strOrderType;
	CString m_strLength;
	CString m_strTgtData;
	float m_fUb;//���ѹ
	float m_fIb;//�����
	float m_fIMax;//������
	float m_fPulse;//�й�����
	int m_nMode;//����ģʽ
	int m_nPhase;//�������
	float m_fPF;//��������
	float m_fUcurPer;//��ǰ��ѹ%
	float m_fIcurPer;//��ǰ����%
	int m_nTurns;//Ȧ��
	int m_nStabTime;//�ȶ�ʱ��
	int m_nClockErrorSpan;//�ռ�ʱ�����
public:
	ZRMTProtocol(void);
	virtual ~ZRMTProtocol(void);
	virtual int DataEncode(CString & strData,CString & strError);
	virtual bool DataDecode(CString strData,CString & strValue);
	virtual CString GetLength(void);
	static bool CheckRMTProtocol(CString str);
	static bool CStringToBYTEArrBy2(const CString & in_str,BYTE *out_pArray,const int & in_nMaxArrayLen);
	int Type10Pro(CString & strData);
	bool VecHexStr2VecFloat2Str(const std::vector<CString> & vec_strData,std::vector<float> & vec_fData,const int & nChangeCount,CString & strOut);
	int Type11Pro(CString & strData);
	void Type15Pro(CString & strData);
	int Type20Pro(CString & strData);
	int Type23Pro(CString & strData);
	CString HexStr2Float2Str(const CString & strSrc);
	float HexString2Float(const CString & strSrc);
	void InitParam(const CString & strOrderType,const CString & strTgtData);
	float GetMeterErrorWaitTime(void);
	float GetClockErrorWaitTime(void);
	CString GetOrderType(void);
};

