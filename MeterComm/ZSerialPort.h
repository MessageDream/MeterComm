#pragma once
class ZSerialPort
{
private:
	HANDLE m_hCom;
	OVERLAPPED m_ol;
	ZSerialPort(const ZSerialPort & zsp);
	ZSerialPort & operator = (const ZSerialPort & zsp);
public:
	ZSerialPort(void);
	virtual ~ZSerialPort(void);
	virtual bool OpenPort(CString in_strPort);//ʹ���첽����
	virtual void ClosePort(void);
	int GetPortState(DCB & out_dcb);
	int SetPortState(DCB & in_dcb);
	int SetPortState(const DWORD & in_dwBaudRate,const BYTE & in_ucParity,const BYTE & in_ucByteSize,const BYTE & in_ucStopBits);
	int SetPortState(const CString & in_strBaudRate,const CString & in_strParity,const CString & in_strByteSize,const CString & in_strStopBits);
	int SetBufferSize(const DWORD & in_dwInBuffer, const DWORD & in_dwOutBuffer);
	int GetPortTimeOut(COMMTIMEOUTS  & out_Timeout);
	int SetPortTimeOut(COMMTIMEOUTS  & in_Timeout);
	int SetPortTimeOut(const DWORD & in_dwReadIntTmout,const DWORD & in_dwReadTolTmoutMul,const DWORD & in_dwReadTolTmoutCst,const DWORD & in_dwWriteTolTmoutMul,const DWORD & in_dwWriteTolTmoutCst);
	int SetPortTimeOut(const CString & in_strReadIntTmout,const CString & in_strReadTolTmoutMul,const CString & in_strReadTolTmoutCst,const CString & in_strWriteTolTmoutMul,const CString & in_strWriteTolTmoutCst);
	int PurgePort(void);
	int SendToPort(const BYTE * in_pSendData,const DWORD & in_dwSendLen,DWORD * out_p_dwActSendLen=NULL);
	virtual int SendToPort(const CString & in_strSendData);
	bool CStringToBYTEArrBy2(const CString & in_str,BYTE *out_pArray,const int & in_nMaxOutBuffer);
	int RecvFromPort(BYTE * out_pRecvData,const int & in_nMaxOutBuffer,DWORD & out_dwActOutBuffer);
	int RecvFromPort(CString & out_strRecvData,const int & in_nMaxOutBuffer);
	void BYTEArrToCStringAsHex(const BYTE *in_pArray,const DWORD & in_dwArrayLen,CString & out_str);
	int ClearPortError(DWORD & dwInBytesExist,DWORD & dwOutBytesExist);
	DWORD GetErrorInfo(void);
	enum ERRORINFO
	{
		ERROR_OK=0,//�ɹ�
		ERROR_INVALIDPORT,//��Ч�˿�
		ERROR_PORTSTATE,//�����˿�DCB����
		ERROR_BUFFERSIZE,//��������������
		ERROR_TIMEOUTS,//������ʱ����
		ERROR_PURGE,//��ջ���������
		ERROR_SEND,//���ͳ���
		ERROR_RECV,//���ճ���
		ERROR_OTHER,//��������
		ERROR_BUFFERSTATE//�鿴������״̬����
	};
};

