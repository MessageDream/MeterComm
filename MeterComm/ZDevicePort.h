#pragma once
#include "ZSerialPort.h"
typedef bool (*CheckCall)(CString str);

class ZDevicePort :public ZSerialPort
{
private:
	unsigned int m_nOnceBytesSend;
	unsigned int m_nOnceBytesRecv;
	unsigned int m_nTimeOutSend;
	unsigned int m_nTimeOutRecv;
	bool m_bIsStopSend;
	bool m_bIsStopRecv;
	HANDLE m_hEvtStop;
	HANDLE m_hEvtExitComm;
	CString m_strPort;
public:
	ZDevicePort(void);
	virtual ~ZDevicePort(void);
	virtual bool OpenPort(const CString & in_strPort);
	virtual void ClosePort(void);
	virtual int SendToPort(const CString & in_strSendData);
	int RecvFromPort(CString & out_strRecvData,CheckCall checkcall);
	void SetOnceBytesSend(const unsigned int & in_nBytes);
	void SetOnceBytesRecv(const unsigned int & in_nBytes);
	void SetTimeOutSend(const unsigned int & in_nTimeOut);
	void SetTimeOutRecv(const unsigned int & in_nTimeOut);
	void StopComm(void);
	CString GetPort(void);
	enum ERRORINFO
	{
		ERROR_SEND_TIMEOUT = 10,//���ͳ�ʱ
		ERROR_SEND_STOP,//����ֹͣ
		ERROR_RECV_TIMEOUT,//���ճ�ʱ
		ERROR_RECV_STOP//����ֹͣ
	};
};

