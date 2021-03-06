#include "StdAfx.h"
#include "ZDLT698_45.h"

HMODULE ZDLT698_45::m_hDll17SGCC=NULL;

const TCHAR tcSplitChar = _T('#');
const TCHAR tcSplitError = _T('$');
const TCHAR tcSplitNetData = _T('*');

ZDLT698_45::ZDLT698_45(void)
	: m_strLength(_T(""))
	, m_strCtrlArea(_T("43"))
	, m_strAddrFlag(_T(""))
	, m_strServerAddr(_T("AAAAAAAAAAAA"))
	, m_strClientAddr(_T("11"))
	, m_strHeadCS(_T(""))
	, m_strSplitFlag(_T(""))
	, m_strReqResCode(_T(""))
	, m_strReqResChoice(_T(""))
	, m_nPIID(0)
	, m_strPIID(_T(""))
	, m_strObjNum(_T(""))
	, m_strObject(_T(""))
	, m_strTgtData(_T(""))
	, m_strTimeTag(_T(""))
	, m_strFrameCS(_T(""))
	, m_strGrade(_T("02"))
	, m_strPsWd(_T("123456"))
	, m_strOperaCode(_T("00000000"))
	, m_nKeyState(0)
	, m_strDiv(_T(""))
	, m_strMeterNo(_T(""))
	, m_strESAMNO(_T(""))
	, m_strASCTR(_T(""))
	, m_strFLG(_T("01"))
	, m_strRand1(_T(""))
	, m_strSessionData1(_T(""))
	, m_strSign1(_T(""))
	, m_strSessionData2(_T(""))
	, m_strSign2(_T(""))
	, m_strSessionKey(_T(""))
	, m_nOperateMode(1)
	, m_strSID(_T(""))
	, m_strAttachData(_T(""))
	, m_strMAC(_T(""))
	, m_nDllType(0)
	, m_nIsRemote(FALSE)
	, m_strServerIP(_T("zaxai.nat123.net"))
	, m_strServerPort(_T("44535"))
	, m_strServerTimeOut(_T("5000"))
	, m_strNetCptIP(_T("10.0.17.107"))
	, m_strNetCptPort(_T("6666")) 
	, m_strNetCptTimeOut(_T("3000"))
	, m_bIsSplitSend(false)
	, m_bIsSplitRecv(false)
	, m_strSplitAPDU(_T(""))
{
}


ZDLT698_45::~ZDLT698_45(void)
{
}


int ZDLT698_45::CallExDll(void)
{
	CString strPath=ZUtil::GetExeCatalogPath();
	m_hDll17SGCC= LoadLibraryEx(strPath+_T("\\exdll\\SocketApi.dll"), NULL, 0);
	if(m_hDll17SGCC==NULL)  
		return 1;
	return 0;
}


void ZDLT698_45::UncallExDll(void)
{
	if(m_hDll17SGCC!=NULL)
		FreeLibrary(m_hDll17SGCC);
}


int ZDLT698_45::DataEncode(CString & strData,CString & strError)
{
	m_strServerAddr.MakeUpper();
	m_strReqResCode.MakeUpper();
	m_strObject.MakeUpper();
	m_strTgtData.MakeUpper();
	CString str68(_T("68"));
	CString str16(_T("16"));
	CString strFLength,strFCtrlArea,strFAddrFlag,strFServerAddr,strFClientAddr,strFHeadCS,strFReqResCode,strFReqResChoice;
	CString strFAPDU,strFPIID,strFObjNum,strFObject,strFTargetData,strFSessionData1,strFSign1,strFSID,strFAttachData,strFMAC,strFTimeTag,strFFrameCS,strHeadArea,strDataArea;
	strFCtrlArea=m_strCtrlArea;
	if(m_strServerAddr.GetLength()%2)
		m_strServerAddr+=_T("F");
	strFServerAddr=ReverseCStringBy2(m_strServerAddr);
	strFClientAddr=ReverseCStringBy2(m_strClientAddr);
	m_strAddrFlag.Format(_T("%X"),m_strServerAddr.GetLength()/2-1);
	if(m_strServerAddr.Find(_T('A'))!=-1)
		m_strAddrFlag=_T("4")+m_strAddrFlag.Right(1);
	else
		m_strAddrFlag=_T("0")+m_strAddrFlag.Right(1);
	strFAddrFlag=m_strAddrFlag;
	strFReqResCode=m_strReqResCode;
	strFReqResChoice=m_strReqResChoice;
	m_strPIID.Format(_T("%02X"), m_nPIID++ %63);
	strFPIID=m_strPIID;
	strFTimeTag=m_strTimeTag;
	if (m_strCtrlArea == _T("43"))
	{
		if (m_strReqResCode == _T("05"))
		{
			if (m_strReqResChoice == _T("01"))
			{
				strFObject = m_strObject;
				strFObject = strFObject.Left(8);
				m_vec_strObject.clear();
				m_vec_strObject.push_back(strFObject);
				strFAPDU = strFReqResCode + strFReqResChoice + strFPIID + strFObject + strFTimeTag;
			}
			else if (m_strReqResChoice == _T("02"))
			{
				strFObject = m_strObject;
				std::vector<CString> vec_strObj;
				ZUtil::StrSplit(strFObject, vec_strObj, tcSplitChar, strFObject.Right(1) == tcSplitChar);
				int nNum = vec_strObj.size();
				strFObjNum = m_strObjNum = ExecLength(nNum);
				strFAPDU = strFReqResCode + strFReqResChoice + strFPIID + strFObjNum;
				m_vec_strObject.clear();
				for (int i = 0; i < nNum; ++i)
				{
					m_vec_strObject.push_back(vec_strObj[i]);
					strFAPDU += vec_strObj[i];
				}
				strFAPDU += strFTimeTag;
			}
			else if (m_strReqResChoice == _T("03"))
			{
				strFObject = m_strObject;
				strFObject = strFObject.Left(8);
				strFTargetData = m_strTgtData;
				m_vec_strObject.clear();
				m_vec_strTgtData.clear();
				m_vec_strObject.push_back(strFObject);
				m_vec_strTgtData.push_back(strFTargetData);
				strFAPDU = strFReqResCode + strFReqResChoice + strFPIID + strFObject + strFTargetData + strFTimeTag;
			}
			else if (m_strReqResChoice == _T("04"))
			{
				strFObject = m_strObject;
				strFTargetData = m_strTgtData;
				std::vector<CString> vec_strObj, vec_strTgt;
				ZUtil::StrSplit(strFObject, vec_strObj, tcSplitChar, strFObject.Right(1) == tcSplitChar);
				ZUtil::StrSplit(strFTargetData, vec_strTgt, tcSplitChar, strFTargetData.Right(1) == tcSplitChar);
				int nNum = vec_strObj.size() > vec_strTgt.size() ? vec_strTgt.size() : vec_strObj.size();
				strFObjNum = m_strObjNum = ExecLength(nNum);
				strFAPDU = strFReqResCode + strFReqResChoice + strFPIID + strFObjNum;
				m_vec_strObject.clear();
				m_vec_strTgtData.clear();
				for (int i = 0; i < nNum; ++i)
				{
					m_vec_strObject.push_back(vec_strObj[i]);
					m_vec_strTgtData.push_back(vec_strTgt[i]);
					strFAPDU += vec_strObj[i] + vec_strTgt[i];
				}
				strFAPDU += strFTimeTag;
			}
			if (m_strGrade == _T("02"))
			{
				m_nOperateMode = 1;
				goto pro10;
			}
			else if (m_strGrade == _T("03"))
			{
				m_nOperateMode = 2;
				goto pro10;
			}
			else if (m_strGrade == _T("04"))
			{
				m_nOperateMode = 3;
				goto pro10;
			}
		}
		else if (m_strReqResCode == _T("06"))
		{
			if (m_strReqResChoice == _T("01"))
			{
				strFObject = m_strObject;
				strFObject = strFObject.Left(8);
				strFTargetData = m_strTgtData;
				m_vec_strObject.clear();
				m_vec_strTgtData.clear();
				m_vec_strObject.push_back(strFObject);
				m_vec_strTgtData.push_back(strFTargetData);
				strFAPDU = strFReqResCode + strFReqResChoice + strFPIID + strFObject + strFTargetData + strFTimeTag;
			}
			else if (m_strReqResChoice == _T("02"))
			{
				strFObject = m_strObject;
				strFTargetData = m_strTgtData;
				std::vector<CString> vec_strObj, vec_strTgt;
				ZUtil::StrSplit(strFObject, vec_strObj, tcSplitChar, strFObject.Right(1) == tcSplitChar);
				ZUtil::StrSplit(strFTargetData, vec_strTgt, tcSplitChar, strFTargetData.Right(1) == tcSplitChar);
				int nNum = vec_strObj.size() > vec_strTgt.size() ? vec_strTgt.size() : vec_strObj.size();
				strFObjNum = m_strObjNum = ExecLength(nNum);
				strFAPDU = strFReqResCode + strFReqResChoice + strFPIID + strFObjNum;
				m_vec_strObject.clear();
				m_vec_strTgtData.clear();
				for (int i = 0; i < nNum; ++i)
				{
					m_vec_strObject.push_back(vec_strObj[i]);
					m_vec_strTgtData.push_back(vec_strTgt[i]);
					strFAPDU += vec_strObj[i] + vec_strTgt[i];
				}
				strFAPDU += strFTimeTag;
			}
			if (m_strGrade == _T("02"))
			{
				m_nOperateMode = 1;
				goto pro10;
			}
			else if (m_strGrade == _T("03"))
			{
				m_nOperateMode = 2;
				goto pro10;
			}
			else if (m_strGrade == _T("04"))
			{
				m_nOperateMode = 3;
				goto pro10;
			}
		}
		else if (m_strReqResCode == _T("07"))
		{
			if (m_nIsRemote)
				return GetEncodeRemoteData(strData, strError);
			if (m_strReqResChoice == _T("01"))
			{
				strFObject = m_strObject;
				strFObject = strFObject.Left(8);
				strFTargetData = m_strTgtData;
				m_vec_strObject.clear();
				m_vec_strTgtData.clear();
				m_vec_strObject.push_back(strFObject);
				m_vec_strTgtData.push_back(strFTargetData);
				int nRtn = ExecESAMData(strFObject, strFTargetData, strError);
				if (nRtn)
					return nRtn;
				strFAPDU = strFReqResCode + strFReqResChoice + strFPIID + strFObject + strFTargetData + strFTimeTag;
			}
			else if (m_strReqResChoice == _T("02"))
			{
				strFObject = m_strObject;
				strFTargetData = m_strTgtData;
				std::vector<CString> vec_strObj, vec_strTgt;
				ZUtil::StrSplit(strFObject, vec_strObj, tcSplitChar, strFObject.Right(1) == tcSplitChar);
				ZUtil::StrSplit(strFTargetData, vec_strTgt, tcSplitChar, strFTargetData.Right(1) == tcSplitChar);
				int nNum = vec_strObj.size() > vec_strTgt.size() ? vec_strTgt.size() : vec_strObj.size();
				strFObjNum = m_strObjNum = ExecLength(nNum);
				strFAPDU = strFReqResCode + strFReqResChoice + strFPIID + strFObjNum;
				m_vec_strObject.clear();
				m_vec_strTgtData.clear();
				for (int i = 0; i < nNum; ++i)
				{
					m_vec_strObject.push_back(vec_strObj[i]);
					m_vec_strTgtData.push_back(vec_strTgt[i]);
					int nRtn = ExecESAMData(vec_strObj[i], vec_strTgt[i], strError);
					if (nRtn)
						return nRtn;
					strFAPDU += vec_strObj[i] + vec_strTgt[i];
				}
				strFAPDU += strFTimeTag;
			}
			if (m_strGrade == _T("02"))
			{
				m_nOperateMode = 1;
				goto pro10;
			}
			else if (m_strGrade == _T("03"))
			{
				m_nOperateMode = 2;
				goto pro10;
			}
			else if (m_strGrade == _T("04"))
			{
				m_nOperateMode = 3;
				goto pro10;
			}
		}
		else if (m_strReqResCode == _T("02"))
		{
			if (m_nIsRemote)
				return GetEncodeRemoteData(strData, strError);
			switch (m_nDllType)
			{
			case 1:
			{
				int nRtn = ConnectDevice17SGCC();
				if (nRtn)
				{
					strError = _T("error:0101") + CString(tcSplitError) + ErrorInfo17SGCC(nRtn);
					return 101;
				}
				nRtn = Set_MeterType17SGCC(16);
				if (nRtn)
				{
					strError = _T("error:0102") + CString(tcSplitError) + ErrorInfo17SGCC(nRtn);
					return 102;
				}
				nRtn = Obj_Meter_Formal_InitSession17SGCC();
				if (nRtn)
				{
					strError = _T("error:0103") + CString(tcSplitError) + ErrorInfo17SGCC(nRtn);
					return 103;
				}
			}
			break;
			}
			strFTargetData = m_strTgtData;
			strFSessionData1.Format(_T("%02X"), m_strSessionData1.GetLength() / 2);
			strFSessionData1 += m_strSessionData1;
			strFSign1.Format(_T("%02X"), m_strSign1.GetLength() / 2);
			strFSign1 += m_strSign1;
			strFAPDU = strFReqResCode + strFPIID + strFTargetData + strFSessionData1 + strFSign1 + strFTimeTag;
		}
		else if (m_strReqResCode == _T("03"))
		{
			strFAPDU = strFReqResCode + strFPIID + strFTimeTag;
		}
		else if (m_strReqResCode == _T("10"))
		{
		pro10:
			if (m_nIsRemote)
				return GetEncodeRemoteData(strData, strError);
			CString strSunAPDU, strFSunAPDU;
			strSunAPDU = strFAPDU;
			switch (m_nDllType)
			{
			case 1:
			{
				int nRtn = Obj_Meter_Formal_GetMeterSetData17SGCC(strSunAPDU);
				if (nRtn)
				{
					strError = _T("error:0104") + CString(tcSplitError) + ErrorInfo17SGCC(nRtn);
					return 104;
				}
			}
			break;
			}
			strFSunAPDU = ExecLength(strSunAPDU.GetLength()/2);
			strFSunAPDU += strSunAPDU;
			strFSID = m_strSID;
			strFAttachData.Format(_T("%02X"), m_strAttachData.GetLength() / 2);
			strFAttachData += m_strAttachData;
			strFMAC.Format(_T("%02X"), m_strMAC.GetLength() / 2);
			strFMAC += m_strMAC;
			switch (m_nOperateMode)
			{
			case 1:
				strFAPDU = _T("1000") + strFSunAPDU + _T("00") + strFSID + strFAttachData + strFMAC;
				break;
			case 2:
				strFAPDU = _T("1001") + strFSunAPDU + _T("03") + strFSID + strFAttachData + strFMAC;
				break;
			case 3:
				strFAPDU = _T("1001") + strFSunAPDU + _T("00") + strFSID + strFAttachData + strFMAC;
				break;
			}
		}
		else
		{
			strError = _T("error:0093") + CString(tcSplitError) + _T("请求码错误!");
			return 93;
		}
		CString strNonAPDU;
		strNonAPDU = strFCtrlArea + strFAddrFlag + strFServerAddr + strFClientAddr;
		int nLimitAPDU = 512 - (strNonAPDU.GetLength() / 2 + 8);
		int nAPDU = strFAPDU.GetLength() / 2;
		if (nAPDU <= nLimitAPDU)
		{
			m_bIsSplitSend = false;
			strFCtrlArea = m_strCtrlArea = _T("43");
			strDataArea = strFCtrlArea + strFAddrFlag + strFServerAddr + strFClientAddr + strFAPDU;
			m_strLength.Format(_T("%04X"), strDataArea.GetLength() / 2 + 6);
			strFLength = ReverseCStringBy2(m_strLength);
			strHeadArea = strFLength + strFCtrlArea + strFAddrFlag + strFServerAddr + strFClientAddr;
			m_strHeadCS = GetPPPFCS16(strHeadArea);
			strFHeadCS = ReverseCStringBy2(m_strHeadCS);
			strDataArea = strHeadArea + strFHeadCS + strFAPDU;
			m_strFrameCS = GetPPPFCS16(strDataArea);
			strFFrameCS = ReverseCStringBy2(m_strFrameCS);
			strData = _T("FEFEFEFE") + str68 + strDataArea + strFFrameCS + str16;
		}
		else
		{
			m_bIsSplitSend = true;
			strFCtrlArea = m_strCtrlArea = _T("63");
			strData.Empty();
			CString strSplitFAPDU, strFSplitFlag;
			int nSplitAPDU = nLimitAPDU - 2;
			int nSize = nAPDU / nSplitAPDU;
			if (nAPDU%nSplitAPDU)
				++nSize;
			for (int i = 0; i < nSize; ++i)
			{
				if (i == 0)
					m_strSplitFlag.Format(_T("%02X00"), i);
				else if (i == nSize - 1)
					m_strSplitFlag.Format(_T("%02X40"), i);
				else
					m_strSplitFlag.Format(_T("%02XC0"), i);
				strFSplitFlag = m_strSplitFlag;
				strSplitFAPDU = strFAPDU.Mid(i*nSplitAPDU * 2, nSplitAPDU * 2);
				strDataArea = strFCtrlArea + strFAddrFlag + strFServerAddr + strFClientAddr + strFSplitFlag + strSplitFAPDU;
				m_strLength.Format(_T("%04X"), strDataArea.GetLength() / 2 + 6);
				strFLength = ReverseCStringBy2(m_strLength);
				strHeadArea = strFLength + strFCtrlArea + strFAddrFlag + strFServerAddr + strFClientAddr;
				m_strHeadCS = GetPPPFCS16(strHeadArea);
				strFHeadCS = ReverseCStringBy2(m_strHeadCS);
				strDataArea = strHeadArea + strFHeadCS + +strFSplitFlag + strSplitFAPDU;
				m_strFrameCS = GetPPPFCS16(strDataArea);
				strFFrameCS = ReverseCStringBy2(m_strFrameCS);
				strData += _T("FEFEFEFE") + str68 + strDataArea + strFFrameCS + str16 + CString(_T('#'));
			}
		}
	}
	else if (m_strCtrlArea == _T("63"))
	{
		CString strFSplitFlag;
		strFSplitFlag = m_strSplitFlag;
		strDataArea = strFCtrlArea + strFAddrFlag + strFServerAddr + strFClientAddr + strFSplitFlag;
		m_strLength.Format(_T("%04X"), strDataArea.GetLength() / 2 + 6);
		strFLength = ReverseCStringBy2(m_strLength);
		strHeadArea = strFLength + strFCtrlArea + strFAddrFlag + strFServerAddr + strFClientAddr;
		m_strHeadCS = GetPPPFCS16(strHeadArea);
		strFHeadCS = ReverseCStringBy2(m_strHeadCS);
		strDataArea = strHeadArea + strFHeadCS + strFSplitFlag;
		m_strFrameCS = GetPPPFCS16(strDataArea);
		strFFrameCS = ReverseCStringBy2(m_strFrameCS);
		strData = _T("FEFEFEFE") + str68 + strDataArea + strFFrameCS + str16;
	}
	return 0;
}


bool ZDLT698_45::DataDecode(CString strData,CString & strValue)
{
	strData.Remove(_T(' '));
	strData=strData.Mid(strData.Find(_T("68")));
	if(!Check698_45(strData))
		return false;
	strValue.Empty();
	BYTE cLenServerAddr=BYTE(_tcstol(strData.Mid(8,2),NULL,16));
	cLenServerAddr<<=4;
	cLenServerAddr>>=4;
	cLenServerAddr+=1;
	CString strCtrlArea, strSplitFlag,strReqResCode,strReqResChoice,strExt,strSrc;
	strCtrlArea = strData.Mid(2 * (3), 2);
	if (strCtrlArea == _T("43") || strCtrlArea == _T("C3"))
	{
		m_bIsSplitRecv = false;
		strSrc = strData.Mid(2 * (8 + cLenServerAddr));
	pro90:
		strReqResCode = strSrc.Left(2);
		strSrc.Delete(0, 2);
		strExt = _T("-") + strReqResCode + _T("H") + strExt;
		if (strReqResCode == _T("05"))
		{
			strReqResChoice = strSrc.Left(2);
			strSrc.Delete(0, 2);
			strSrc.Delete(0, 2);//删除PIID
			if (strReqResChoice == _T("01"))
			{
				CString strObject = strSrc.Left(8);
				strSrc.Delete(0, 8);
				strValue += _T("<O obj=\"") + strObject + _T("\">");
				strValue += _T("</O>");
				strValue += strExt;
			}
			else if (strReqResChoice == _T("02"))
			{
				int nObjNum = ReverseExecLength(strSrc);
				CString strObjNum;
				strObjNum.Format(_T("%d"), nObjNum);
				strValue += _T("<N num=\"") + strObjNum + _T("\">");
				for (int i = 0; i < nObjNum; ++i)
				{
					CString strObject = strSrc.Left(8);
					strSrc.Delete(0, 8);
					strValue += _T("<O obj=\"") + strObject + _T("\">");
					strValue += _T("</O>");
				}
				strValue += _T("</N>");
				strValue += strExt;
			}
			else if (strReqResChoice == _T("03"))
			{
				CString strObject = strSrc.Left(8);
				strSrc.Delete(0, 8);
				strValue += _T("<O obj=\"") + strObject + _T("\">");
				ExecRSD_Tag(strSrc, strValue);
				ExecRCSD_Tag(strSrc, strValue);
				strValue += _T("</O>");
				strValue += strExt;
			}
			else if (strReqResChoice == _T("04"))
			{
				int nObjNum = ReverseExecLength(strSrc);
				CString strObjNum;
				strObjNum.Format(_T("%d"), nObjNum);
				strValue += _T("<N num=\"") + strObjNum + _T("\">");
				for (int i = 0; i < nObjNum; ++i)
				{
					CString strObject = strSrc.Left(8);
					strSrc.Delete(0, 8);
					strValue += _T("<O obj=\"") + strObject + _T("\">");
					ExecRSD_Tag(strSrc, strValue);
					ExecRCSD_Tag(strSrc, strValue);
					strValue += _T("</O>");
				}
				strValue += _T("</N>");
				strValue += strExt;
			}
		}
		else if (strReqResCode == _T("85"))
		{
			strReqResChoice = strSrc.Left(2);
			strSrc.Delete(0, 2);
			strSrc.Delete(0, 2);//删除PIID
			if (strReqResChoice == _T("01"))
			{
				ExecGetResultNormal(strSrc, strValue);
				UpdateVariables85H(strValue);
				strValue += strExt;
			}
			else if (strReqResChoice == _T("02"))
			{
				int nObjNum = ReverseExecLength(strSrc);
				CString strObjNum;
				strObjNum.Format(_T("%d"), nObjNum);
				strValue += _T("<N num=\"") + strObjNum + _T("\">");
				for (int i = 0; i < nObjNum; ++i)
					ExecGetResultNormal(strSrc, strValue);
				strValue += _T("</N>");
				UpdateVariables85H(strValue);
				strValue += strExt;
			}
			else if (strReqResChoice == _T("03"))
			{
				ExecGetResultRecord(strSrc, strValue);
				UpdateVariables85H(strValue);
				strValue += strExt;
			}
			else if (strReqResChoice == _T("04"))
			{
				int nObjNum = ReverseExecLength(strSrc);
				CString strObjNum;
				strObjNum.Format(_T("%d"), nObjNum);
				strValue += _T("<N num=\"") + strObjNum + _T("\">");
				for (int i = 0; i < nObjNum; ++i)
					ExecGetResultRecord(strSrc, strValue);
				strValue += _T("</N>");
				UpdateVariables85H(strValue);
				strValue += strExt;
			}
		}
		else if (strReqResCode == _T("06"))
		{
			strReqResChoice = strSrc.Left(2);
			strSrc.Delete(0, 2);
			strSrc.Delete(0, 2);//删除PIID
			if (strReqResChoice == _T("01"))
			{
				CString strObject = strSrc.Left(8);
				strSrc.Delete(0, 8);
				strValue += _T("<O obj=\"") + strObject + _T("\">");
				ExecData_Tag(strSrc, strValue);
				strValue += _T("</O>");
				strValue += strExt;
			}
			else if (strReqResChoice == _T("02"))
			{
				int nObjNum = ReverseExecLength(strSrc);
				CString strObjNum;
				strObjNum.Format(_T("%d"), nObjNum);
				strValue += _T("<N num=\"") + strObjNum + _T("\">");
				for (int i = 0; i < nObjNum; ++i)
				{
					CString strObject = strSrc.Left(8);
					strSrc.Delete(0, 8);
					strValue += _T("<O obj=\"") + strObject + _T("\">");
					ExecData_Tag(strSrc, strValue);
					strValue += _T("</O>");
				}
				strValue += _T("</N>");
				strValue += strExt;
			}
		}
		else if (strReqResCode == _T("86"))
		{
			strReqResChoice = strSrc.Left(2);
			strSrc.Delete(0, 2);
			strSrc.Delete(0, 2);//删除PIID
			if (strReqResChoice == _T("01"))
			{
				ExecSetResultNormal(strSrc, strValue);
				UpdateVariables86H(strValue);
				strValue += strExt;
			}
			if (strReqResChoice == _T("02"))
			{
				int nObjNum = ReverseExecLength(strSrc);
				CString strObjNum;
				strObjNum.Format(_T("%d"), nObjNum);
				strValue += _T("<N num=\"") + strObjNum + _T("\">");
				for (int i = 0; i < nObjNum; ++i)
					ExecSetResultNormal(strSrc, strValue);
				strValue += _T("</N>");
				UpdateVariables86H(strValue);
				strValue += strExt;
			}
		}
		else if (strReqResCode == _T("07"))
		{
			strReqResChoice = strSrc.Left(2);
			strSrc.Delete(0, 2);
			strSrc.Delete(0, 2);//删除PIID
			if (strReqResChoice == _T("01"))
			{
				CString strObject = strSrc.Left(8);
				strSrc.Delete(0, 8);
				strValue += _T("<O obj=\"") + strObject + _T("\">");
				ExecData_Tag(strSrc, strValue);
				strValue += _T("</O>");
				strValue += strExt;
			}
			else if (strReqResChoice == _T("02"))
			{
				int nObjNum = ReverseExecLength(strSrc);
				CString strObjNum;
				strObjNum.Format(_T("%d"), nObjNum);
				strValue += _T("<N num=\"") + strObjNum + _T("\">");
				for (int i = 0; i < nObjNum; ++i)
				{
					CString strObject = strSrc.Left(8);
					strSrc.Delete(0, 8);
					strValue += _T("<O obj=\"") + strObject + _T("\">");
					ExecData_Tag(strSrc, strValue);
					strValue += _T("</O>");
				}
				strValue += _T("</N>");
				strValue += strExt;
			}
		}
		else if (strReqResCode == _T("87"))
		{
			strReqResChoice = strSrc.Left(2);
			strSrc.Delete(0, 2);
			strSrc.Delete(0, 2);//删除PIID
			if (strReqResChoice == _T("01"))
			{
				ExecActionResultNormal(strSrc, strValue);
				UpdateVariables87H(strValue);
				strValue += strExt;
			}
			if (strReqResChoice == _T("02"))
			{
				int nObjNum = ReverseExecLength(strSrc);
				CString strObjNum;
				strObjNum.Format(_T("%d"), nObjNum);
				strValue += _T("<N num=\"") + strObjNum + _T("\">");
				for (int i = 0; i < nObjNum; ++i)
					ExecActionResultNormal(strSrc, strValue);
				strValue += _T("</N>");
				UpdateVariables87H(strValue);
				strValue += strExt;
			}
		}
		else if (strReqResCode == _T("02"))
		{
			strSrc = strData.Mid(2 * (47 + cLenServerAddr));
			strValue += strSrc.Left(2);
			strSrc.Delete(0, 2);
			strValue += strExt;
		}
		else if (strReqResCode == _T("82"))
		{
			if (m_nIsRemote)
				return GetDecodeRemoteData(strData, strValue);
			strSrc = strData.Mid(2 * (79 + cLenServerAddr));
			int nConntRslt = _tcstol(strSrc.Left(2), NULL, 16);
			strSrc.Delete(0, 2);
			strValue += GetConnectResult(nConntRslt);
			int nHasSecurityData = _tcstol(strSrc.Left(2), NULL, 16);
			strSrc.Delete(0, 2);
			if (nHasSecurityData)
			{
				int nLength = _tcstol(strSrc.Left(2), NULL, 16);
				strSrc.Delete(0, 2);
				m_strSessionData2 = strSrc.Left(nLength * 2);
				strSrc.Delete(0, nLength * 2);
				nLength = _tcstol(strSrc.Left(2), NULL, 16);
				strSrc.Delete(0, 2);
				m_strSign2 = strSrc.Left(nLength * 2);
				strSrc.Delete(0, nLength * 2);
				switch (m_nDllType)
				{
				case 1:
				{
					Obj_Meter_Formal_VerifySession17SGCC();
				}
				break;
				}
			}
			strValue += strExt;
		}
		else if (strReqResCode == _T("03"))
		{
			strValue += _T("断开应用连接") + strExt;
		}
		else if (strReqResCode == _T("83"))
		{
			strSrc.Delete(0, 2);//删除PIID
			int nResult = _tcstol(strSrc.Left(2), NULL, 16);
			strSrc.Delete(0, 2);
			if (nResult)
				strValue += _T("断开应用连接失败");
			else
				strValue += _T("断开应用连接成功");
			strValue += strExt;
		}
		else if (strReqResCode == _T("10"))
		{
			strValue += strSrc.Left(2) + CString(tcSplitChar);
			strSrc.Delete(0, 2);
			int nLength = ReverseExecLength(strSrc);
			strSrc.Delete(0, nLength * 2);
			strValue += strSrc.Left(2);
			strValue += strExt;
		}
		else if (strReqResCode == _T("90"))
		{
			if (m_nIsRemote)
				return GetDecodeRemoteData(strData, strValue);
			int nAPDUChoice = _tcstol(strSrc.Left(2), NULL, 16);
			strSrc.Delete(0, 2);
			switch (nAPDUChoice)
			{
			case 0:
			{
				int nLength = ReverseExecLength(strSrc);
				CString strSunAPDU = strSrc.Left(nLength * 2);
				strSrc.Delete(0, nLength * 2);
				strSrc = strSunAPDU;
				goto pro90;
			}
			break;
			case 1:
			{
				int nLength = ReverseExecLength(strSrc);
				CString strSunAPDU = strSrc.Left(nLength * 2);
				strSrc.Delete(0, nLength * 2);
				int nOPTIONAL = _tcstol(strSrc.Left(2), NULL, 16);
				strSrc.Delete(0, 2);
				if (nOPTIONAL)
				{
					int nSunChoice = _tcstol(strSrc.Left(2), NULL, 16);
					strSrc.Delete(0, 2);
					if (nSunChoice == 0)
					{
						nLength = _tcstol(strSrc.Left(2), NULL, 16);
						strSrc.Delete(0, 2);
						m_strMAC = strSrc.Left(nLength * 2);
						strSrc.Delete(0, nLength * 2);
					}
				}
				else
					m_strMAC.Empty();
				switch (m_nDllType)
				{
				case 1:
				{
					int nRtn = Obj_Meter_Formal_VerifyMeterData17SGCC(strSunAPDU);
					if (!nRtn)
					{
						strSrc = strSunAPDU;
						goto pro90;
					}
					else
						strValue = ErrorInfo17SGCC(nRtn) + strExt;
				}
				break;
				}
			}
			break;
			case 2:
			{
				ExecDAR_Tag(strSrc, strValue);
				strValue += strExt;
			}
			break;
			}
		}
		else
		{
			strValue = _T("ErrorData") + strExt;
		}
	}
	else if (strCtrlArea == _T("63"))
	{
		strSplitFlag = strData.Mid(2 * (8 + cLenServerAddr), 4);
		strValue += strSplitFlag + CString(tcSplitChar) + _T("SplitFrame");
	}
	else if (strCtrlArea == _T("E3"))
	{
		strSplitFlag = strData.Mid(2 * (8 + cLenServerAddr), 4);
		if (strSplitFlag.Right(2) == _T("00"))
		{
			m_bIsSplitRecv = true;
			m_strSplitAPDU.Empty();
			m_strSplitAPDU += strData.Mid(2 * (10 + cLenServerAddr));
			m_strSplitAPDU = m_strSplitAPDU.Left(m_strSplitAPDU.GetLength() - 6);
			m_strCtrlArea = _T("63");
			m_strSplitFlag = strSplitFlag.Left(2) + _T("80");
		}
		else if (strSplitFlag.Right(2) == _T("C0"))
		{
			m_strSplitAPDU += strData.Mid(2 * (10 + cLenServerAddr));
			m_strSplitAPDU = m_strSplitAPDU.Left(m_strSplitAPDU.GetLength() - 6);
			m_strCtrlArea = _T("63");
			m_strSplitFlag = strSplitFlag.Left(2) + _T("80");
		}
		else if (strSplitFlag.Right(2) == _T("40"))
		{
			m_bIsSplitRecv = false;
			m_strSplitAPDU += strData.Mid(2 * (10 + cLenServerAddr));
			m_strSplitAPDU = m_strSplitAPDU.Left(m_strSplitAPDU.GetLength() - 6);
			strSrc = m_strSplitAPDU;
			CString strDataArea, strLength, strHeadArea, strHeadCS, strFrameCS;
			strDataArea = _T("C3") + m_strAddrFlag + ReverseCStringBy2(m_strServerAddr) + m_strClientAddr + m_strSplitAPDU;
			strLength.Format(_T("%04X"), strDataArea.GetLength() / 2 + 6);
			strLength = ReverseCStringBy2(strLength);
			strHeadArea = strLength + _T("C3") + m_strAddrFlag + ReverseCStringBy2(m_strServerAddr) + m_strClientAddr;
			strHeadCS = GetPPPFCS16(strHeadArea);
			strHeadCS = ReverseCStringBy2(strHeadCS);
			strDataArea = strHeadArea + strHeadCS + m_strSplitAPDU;
			strFrameCS = GetPPPFCS16(strDataArea);
			strFrameCS = ReverseCStringBy2(strFrameCS);
			strData = _T("FEFEFEFE68") + strDataArea + strFrameCS + _T("16");
			goto pro90;
		}
		strValue += strSplitFlag + CString(tcSplitChar) + _T("SplitFrame");
	}
	return true;
}


CString ZDLT698_45::GetLength(void)
{
	return m_strLength;
}


bool ZDLT698_45::Check698_45(CString str)
{
	str.Remove(_T(' '));
	str=str.Mid(str.Find(_T("68")));
	BYTE sz_cData[70000]={0};
	if(CStringToBYTEArrBy2(str,sz_cData,sizeof(sz_cData))==false)
		return false;
	UINT16 nLength,nHCS,nFCS;
	nLength=sz_cData[2];
	nLength=(nLength<<8)+sz_cData[1];
	BYTE cLenServerAddr=sz_cData[4];
	cLenServerAddr<<=4;
	cLenServerAddr>>=4;
	cLenServerAddr+=1;
	nHCS=sz_cData[7+cLenServerAddr];
	nHCS=(nHCS<<8)+sz_cData[6+cLenServerAddr];
	nFCS=sz_cData[nLength];
	nFCS=(nFCS<<8)+sz_cData[nLength-1];
	if(sz_cData[0]==0x68&&sz_cData[nLength+1]==0x16&&nLength+2==str.GetLength()/2&&nHCS==GetPPPFCS16(sz_cData+1,5+cLenServerAddr)&&nFCS==GetPPPFCS16(sz_cData+1,nLength-2))
		return true;
	else
		return false;
}


bool ZDLT698_45::Set16Param(const CString & strParam)
{
	std::vector<CString> vec_strDatas;
	ZUtil::StrSplit(strParam,vec_strDatas,_T(','),strParam.Right(1)==_T(','));
	if(vec_strDatas.size()!=16)
		return false;
	int i=0;
	m_strCtrlArea = vec_strDatas[i++];
	if(!vec_strDatas[i].IsEmpty())
		m_strServerAddr=vec_strDatas[i++];
	else
		i++;
	m_strReqResCode=vec_strDatas[i++];
	m_strReqResChoice=m_strReqResCode.Mid(2,2);
	m_strReqResCode=m_strReqResCode.Mid(0,2);
	m_strObject=vec_strDatas[i++];
	m_strTgtData=vec_strDatas[i++];
	int nIndexAt=m_strTgtData.ReverseFind(_T('@'));
	if(nIndexAt!=-1)
	{
		m_strTimeTag=m_strTgtData.Mid(nIndexAt+1);
		m_strTgtData=m_strTgtData.Left(nIndexAt);
	}
	else
		m_strTimeTag=_T("00");
	m_strGrade=vec_strDatas[i++];
	m_strPsWd=vec_strDatas[i++];
	m_strOperaCode=vec_strDatas[i++];
	m_strServerIP=vec_strDatas[i++];
	m_strServerPort=vec_strDatas[i++];
	m_strServerTimeOut=vec_strDatas[i++];
	m_strNetCptIP=vec_strDatas[i++];
	m_strNetCptPort=vec_strDatas[i++];
	m_strNetCptTimeOut=vec_strDatas[i++];
	m_nDllType = _ttoi(vec_strDatas[i++]);
	m_nIsRemote = _ttoi(vec_strDatas[i++]);
	return true;
}


CString ZDLT698_45::ReverseCStringBy2(CString str)
{
	str.MakeUpper();
	str.MakeReverse();
	int i;
	int nSize=str.GetLength();
	if(nSize%2)
		--nSize;
	if(nSize<2)
		return CString(_T(""));
	TCHAR tcTemp;
	TCHAR *p_tc=new TCHAR[nSize+1];
	for(i=0;i<nSize;++i)
	{
		p_tc[i]=str[i];
	}
	p_tc[nSize]=_T('\0');
	for(i=0;i<nSize;i=i+2)
	{
		tcTemp=p_tc[i];
		p_tc[i]=p_tc[i+1];
		p_tc[i+1]=tcTemp;
	}
	str=p_tc;
	delete []p_tc;
	return str;
}


CString ZDLT698_45::GetPPPFCS16(const CString & str)
{
	CString strFCS;
	BYTE sz_cData[8192]={0};
	if(CStringToBYTEArrBy2(str,sz_cData,sizeof(sz_cData)))
	{
		strFCS.Format(_T("%04X"),GetPPPFCS16(sz_cData,str.GetLength()/2));
	}
	return strFCS;
}


UINT16 ZDLT698_45::GetPPPFCS16(BYTE * p_c,int nLen)
{
	const int P=0x8408;
	UINT16 nFCS=0xFFFF;
	UINT16 sz_nFCSTab[256]={0};
	for(int i=0;i<256;++i)
	{
		UINT16 & v=sz_nFCSTab[i];
		v=i;
		for(int j=8;j--;)
			v=v&1?(v>>1)^P:v>>1;
	}
	while (nLen--)
	{
		nFCS=(nFCS>>8)^sz_nFCSTab[(nFCS^*p_c++)&0xFF];
	}
	return (~nFCS);
}


bool ZDLT698_45::CStringToBYTEArrBy2(const CString & in_str,BYTE *out_pArray,const int & in_nMaxArrayLen)
{
	CString strByte;
	int i=0,j=0,nLen;
	nLen=in_str.GetLength()/2;
	if(nLen>=in_nMaxArrayLen)
		return false;
	for(;i<nLen;++i,j=j+2)
	{
		strByte=in_str.Mid(j,2);
		out_pArray[i]=BYTE(_tcstol(strByte,NULL ,16));
	}
	out_pArray[nLen]='\0';
	return true;
}


void ZDLT698_45::ExecBool_Integer(CString & strSrc, CString & strValue)
{
	int nLength = 1;
	int nTemp = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString strTemp;
	strTemp.Format(_T("%d"), nTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecBitString(CString & strSrc, CString & strValue)
{
	int nLength = ReverseExecLength(strSrc);
	nLength /= 8;
	strValue += strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
}


void ZDLT698_45::ExecDoubleLong(CString & strSrc, CString & strValue)
{
	int nLength = 4;
	int nTemp = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString strTemp;
	strTemp.Format(_T("%d"), nTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecDoubleLongUnsigned(CString & strSrc, CString & strValue)
{
	int nLength = 4;
	unsigned int nTemp = _tcstoul(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString strTemp;
	strTemp.Format(_T("%u"), nTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecOctetString_MAC_RN(CString & strSrc, CString & strValue)
{
	int nLength = ReverseExecLength(strSrc);
	strValue += strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
}


void ZDLT698_45::ExecVisibleString(CString & strSrc, CString & strValue)
{
	int nLength = ReverseExecLength(strSrc);
	CString strTemp = strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
	strTemp = HexStringToAsciiString(strTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecUTF8String(CString & strSrc, CString & strValue)
{
	int nLength = ReverseExecLength(strSrc);
	CString strTemp = strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
	strTemp = HexStringToUTF8String(strTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecLong(CString & strSrc, CString & strValue)
{
	int nLength = 2;
	int nTemp = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString strTemp;
	strTemp.Format(_T("%d"), nTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecUnsigned_Enum(CString & strSrc, CString & strValue)
{
	int nLength = 1;
	unsigned int nTemp = _tcstoul(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString strTemp;
	strTemp.Format(_T("%u"), nTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecLongUnsigned(CString & strSrc, CString & strValue)
{
	int nLength = 2;
	unsigned int nTemp = _tcstoul(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString strTemp;
	strTemp.Format(_T("%u"), nTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecLong64(CString & strSrc, CString & strValue)
{
	int nLength = 8;
	LONGLONG nTemp = _tcstoi64(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString strTemp;
	strTemp.Format(_T("%lld"), nTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecLong64Unsigned(CString & strSrc, CString & strValue)
{
	int nLength = 8;
	ULONGLONG nTemp = _tcstoui64(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString strTemp;
	strTemp.Format(_T("%llu"), nTemp);
	strValue += strTemp;
}


void ZDLT698_45::ExecFloat32(CString & strSrc, CString & strValue)
{
	int nLength = 4;
	CString strTemp = strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
	float f = HexString2Float(strTemp);
	strTemp.Format(_T("%f"), f);
	strValue += strTemp;
}


void ZDLT698_45::ExecFloat64(CString & strSrc, CString & strValue)
{
	int nLength = 8;
	CString strTemp = strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
	double d = HexString2Double(strTemp);
	strTemp.Format(_T("%lf"), d);
	strValue += strTemp;
}


void ZDLT698_45::ExecDateTime(CString & strSrc, CString & strValue)
{
	int nLength = 4;
	CString strDate = HexStringToDateString(strSrc.Left(nLength * 2));
	strSrc.Delete(0, nLength * 2);
	nLength = 1;
	int nWeek = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	nLength = 3;
	CString strTime = HexStringToTimeString(strSrc.Left(nLength * 2));
	strSrc.Delete(0, nLength * 2);
	nLength = 2;
	int nMillSec = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString str;
	str.Format(_T("%s%02d%s%04d"), strDate, nWeek, strTime, nMillSec);
	strValue += str;
}


void ZDLT698_45::ExecDate(CString & strSrc, CString & strValue)
{
	int nLength = 4;
	CString strDate = HexStringToDateString(strSrc.Left(nLength * 2));
	strSrc.Delete(0, nLength * 2);
	nLength = 1;
	int nWeek = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	CString str;
	str.Format(_T("%s%02d"), strDate, nWeek);
	strValue += str;
}


void ZDLT698_45::ExecTime(CString & strSrc, CString & strValue)
{
	int nLength = 3;
	CString strTime = HexStringToTimeString(strSrc.Left(nLength * 2));
	strSrc.Delete(0, nLength * 2);
	strValue += strTime;
}


void ZDLT698_45::ExecDateTimeS(CString & strSrc, CString & strValue)
{
	int nLength = 4;
	CString strDate = HexStringToDateString(strSrc.Left(nLength * 2));
	strSrc.Delete(0, nLength * 2);
	nLength = 3;
	CString strTime = HexStringToTimeString(strSrc.Left(nLength * 2));
	strSrc.Delete(0, nLength * 2);
	CString str;
	str.Format(_T("%s%s"), strDate, strTime);
	strValue += str;
}


void ZDLT698_45::ExecOI(CString & strSrc, CString & strValue)
{
	int nLength = 2;
	strValue += strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
}


void ZDLT698_45::ExecOAD_OMD(CString & strSrc, CString & strValue)
{
	int nLength = 4;
	strValue += strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
}


void ZDLT698_45::ExecROAD(CString & strSrc, CString & strValue)
{
	ExecOAD_OMD(strSrc, strValue);
	int nLength = 1;
	int nNumSequenceOfOAD = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	while (nNumSequenceOfOAD--)
		ExecOAD_OMD(strSrc, strValue);
}


void ZDLT698_45::ExecTI(CString & strSrc, CString & strValue)
{
	int nLength = 3;
	strValue += strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
}


void ZDLT698_45::ExecTSA(CString & strSrc, CString & strValue)
{
	int nLength = 6;
	strValue += strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
}


void ZDLT698_45::ExecRegion(CString & strSrc, CString & strValue)
{
}


void ZDLT698_45::ExecScalerUnit(CString & strSrc, CString & strValue)
{
	ExecBool_Integer(strSrc, strValue);
	strValue += _T(" ");
	ExecUnsigned_Enum(strSrc, strValue);
}


void ZDLT698_45::ExecRSD(CString & strSrc, CString & strValue)
{
	int nRSDChoice = _tcstol(strSrc.Left(2), NULL, 16);
	strSrc.Delete(0, 2);
	switch (nRSDChoice)
	{
	case 0:break;
	case 1:
	{
		ExecOAD_OMD(strSrc, strValue);
		ExecData(strSrc, strValue);
	}
	break;
	case 2:
	{
		ExecOAD_OMD(strSrc, strValue);
		int i = 3;
		while (i--)
			ExecData(strSrc, strValue);
	}
	break;
	case 9:
		ExecUnsigned_Enum(strSrc, strValue);
		break;
	}
}


void ZDLT698_45::ExecCSD(CString & strSrc, CString & strValue)
{
	int nLength = 1;
	int nCSDChoice = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	switch (nCSDChoice)
	{
	case 0:
		ExecOAD_OMD(strSrc, strValue);
		break;
	case 1:
		ExecROAD(strSrc, strValue);
		break;
	}
}


void ZDLT698_45::ExecMS(CString & strSrc, CString & strValue)
{
}


void ZDLT698_45::ExecSID(CString & strSrc, CString & strValue)
{
	ExecDoubleLongUnsigned(strSrc, strValue);
	strValue += _T(" ");
	ExecOctetString_MAC_RN(strSrc, strValue);
}


void ZDLT698_45::ExecSIDMAC(CString & strSrc, CString & strValue)
{
	ExecSID(strSrc, strValue);
	strValue += _T(" ");
	ExecOctetString_MAC_RN(strSrc, strValue);
}


void ZDLT698_45::ExecCOMDCB(CString & strSrc, CString & strValue)
{
	int nLength = 5;
	strValue += strSrc.Left(nLength * 2);
	strSrc.Delete(0, nLength * 2);
}


void ZDLT698_45::ExecRCSD(CString & strSrc, CString & strValue)
{
	int nLength = 1;
	int nNumSequenceOfCSD = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	while (nNumSequenceOfCSD--)
		ExecCSD(strSrc, strValue);
}


void ZDLT698_45::ExecData(CString & strSrc, CString & strValue)
{
	int nType = _tcstol(strSrc.Left(2), NULL, 16);
	strSrc.Delete(0, 2);
	switch (nType)
	{
	case 0:
		break;
	case 1:
	case 2:
	{
		int nSize = ReverseExecLength(strSrc);
		for (int i = 0; i < nSize; ++i)
		{
			ExecData(strSrc, strValue);
		}
	}
	break;
	case 3:
		ExecBool_Integer(strSrc, strValue);
		break;
	case 4:
		ExecBitString(strSrc, strValue);
		break;
	case 5:
		ExecDoubleLong(strSrc, strValue);
		break;
	case 6:
		ExecDoubleLongUnsigned(strSrc, strValue);
		break;
	case 9:
		ExecOctetString_MAC_RN(strSrc, strValue);
		break;
	case 10:
		ExecVisibleString(strSrc, strValue);
		break;
	case 12:
		ExecUTF8String(strSrc, strValue);
		break;
	case 15:
		ExecBool_Integer(strSrc, strValue);
		break;
	case 16:
		ExecLong(strSrc, strValue);
		break;
	case 17:
		ExecUnsigned_Enum(strSrc, strValue);
		break;
	case 18:
		ExecLongUnsigned(strSrc, strValue);
		break;
	case 20:
		ExecLong64(strSrc, strValue);
		break;
	case 21:
		ExecLong64Unsigned(strSrc, strValue);
		break;
	case 22:
		ExecUnsigned_Enum(strSrc, strValue);
		break;
	case 23:
		ExecFloat32(strSrc, strValue);
		break;
	case 24:
		ExecFloat64(strSrc, strValue);
		break;
	case 25:
		ExecDateTime(strSrc, strValue);
		break;
	case 26:
		ExecDate(strSrc, strValue);
		break;
	case 27:
		ExecTime(strSrc, strValue);
		break;
	case 28:
		ExecDateTimeS(strSrc, strValue);
		break;
	case 80:
		ExecOI(strSrc, strValue);
		break;
	case 81:
		ExecOAD_OMD(strSrc, strValue);
		break;
	case 82:
		ExecROAD(strSrc, strValue);
		break;
	case 83:
		ExecOAD_OMD(strSrc, strValue);
		break;
	case 84:
		ExecTI(strSrc, strValue);
		break;
	case 85:
		ExecTSA(strSrc, strValue);
		break;
	case 86:
		ExecOctetString_MAC_RN(strSrc, strValue);
		break;
	case 87:
		ExecOctetString_MAC_RN(strSrc, strValue);
		break;
	case 88:
		ExecRegion(strSrc, strValue);
		break;
	case 89:
		ExecScalerUnit(strSrc, strValue);
		break;
	case 90:
		ExecRSD(strSrc, strValue);
		break;
	case 91:
		ExecCSD(strSrc, strValue);
		break;
	case 92:
		ExecMS(strSrc, strValue);
		break;
	case 93:
		ExecSID(strSrc, strValue);
		break;
	case 94:
		ExecSIDMAC(strSrc, strValue);
		break;
	case 95:
		ExecCOMDCB(strSrc, strValue);
		break;
	case 96:
		ExecRCSD(strSrc, strValue);
		break;
	}
}


void ZDLT698_45::ExecOAD_OMD_Tag(CString & strSrc, CString & strValue)
{
	strValue += _T("<O obj=\"");
	ExecOAD_OMD(strSrc, strValue);
	strValue += _T("\" />");
}


void ZDLT698_45::ExecROAD_Tag(CString & strSrc, CString & strValue)
{
	strValue += _T("<Road>");
	ExecOAD_OMD_Tag(strSrc, strValue);
	int nLength = 1;
	int nNumSequenceOfOAD = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	while (nNumSequenceOfOAD--)
		ExecOAD_OMD_Tag(strSrc, strValue);
	strValue += _T("</Road>");
}


void ZDLT698_45::ExecRSD_Tag(CString & strSrc, CString & strValue)
{
	int nRSDChoice = _tcstol(strSrc.Left(2), NULL, 16);
	strSrc.Delete(0, 2);
	CString strChoice;
	strChoice.Format(_T("%d"), nRSDChoice);
	strValue += _T("<Rsd choice=\"") + strChoice + _T("\">");
	switch (nRSDChoice)
	{
	case 0:break;
	case 1:
	{
		ExecOAD_OMD_Tag(strSrc, strValue);
		ExecData_Tag(strSrc, strValue);
	}
	break;
	case 2:
	{
		ExecOAD_OMD_Tag(strSrc, strValue);
		int i = 3;
		while (i--)
			ExecData_Tag(strSrc, strValue);
	}
	break;
	case 9:
		ExecUnsigned_Enum(strSrc, strValue);
		break;
	}
	strValue += _T("</Rsd>");
}


void ZDLT698_45::ExecCSD_Tag(CString & strSrc, CString & strValue)
{
	int nLength = 1;
	int nCSDChoice = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	strValue += _T("<Csd>");
	switch (nCSDChoice)
	{
	case 0:
		ExecOAD_OMD(strSrc, strValue);
		break;
	case 1:
		ExecROAD(strSrc, strValue);
		break;
	}
	strValue += _T("</Csd>");
}


void ZDLT698_45::ExecRCSD_Tag(CString & strSrc, CString & strValue)
{
	int nLength = 1;
	int nNumSequenceOfCSD = _tcstol(strSrc.Left(nLength * 2), NULL, 16);
	strSrc.Delete(0, nLength * 2);
	strValue += _T("<Rcsd>");
	while (nNumSequenceOfCSD--)
		ExecCSD_Tag(strSrc, strValue);
	strValue += _T("</Rcsd>");
}


void ZDLT698_45::ExecData_Tag(CString & strSrc, CString & strValue)
{
	int nType = _tcstol(strSrc.Left(2), NULL, 16);
	CString strType;
	strType.Format(_T("%d"), nType);
	strValue += _T("<T type=\"") + strType + _T("\">");
	if (nType==1||nType==2)
	{
		strSrc.Delete(0, 2);
		int nSize = ReverseExecLength(strSrc);
		for (int i = 0; i < nSize; ++i)
		{
			ExecData_Tag(strSrc, strValue);
		}
	}
	else
		ExecData(strSrc, strValue);
	strValue += _T("</T>");
}


void ZDLT698_45::ExecDAR_Tag(CString & strSrc, CString & strValue)
{
	CString strDar;
	int nIndex = _tcstol(strSrc.Left(2), NULL, 16);
	strSrc.Delete(0, 2);
	CString strIndex;
	strIndex.Format(_T("%d"), nIndex);
	strDar = GetDAR(nIndex);
	strValue += _T("<D dar=\"") + strIndex + _T("\">") + strDar + _T("</D>");
}


void ZDLT698_45::ExecGetResultNormal(CString & strSrc, CString & strValue)
{
	CString strObject = strSrc.Left(8);
	strSrc.Delete(0, 8);
	strValue += _T("<O obj=\"") + strObject + _T("\">");
	int nResultChoice = _tcstol(strSrc.Left(2), NULL, 16);
	strSrc.Delete(0, 2);
	switch (nResultChoice)
	{
	case 0:
		ExecDAR_Tag(strSrc, strValue);
		break;
	case 1:
		ExecData_Tag(strSrc, strValue);
		break;
	}
	strValue += _T("</O>");
}


void ZDLT698_45::ExecGetResultRecord(CString & strSrc, CString & strValue)
{
	CString strObject = strSrc.Left(8);
	strSrc.Delete(0, 8);
	strValue += _T("<O obj=\"") + strObject + _T("\">");
	int nNumSequenceOfCSD = _tcstol(strSrc.Left(2), NULL, 16);
	ExecRCSD_Tag(strSrc, strValue);
	int nResultChoice = _tcstol(strSrc.Left(2), NULL, 16);
	strSrc.Delete(0, 2);
	switch (nResultChoice)
	{
	case 0:
		ExecDAR_Tag(strSrc, strValue);
		break;
	case 1:
	{
		int nNumSequenceOfRecord = _tcstol(strSrc.Left(2), NULL, 16);
		strSrc.Delete(0, 2);
		while (nNumSequenceOfRecord--)
		{
			int nNum = nNumSequenceOfCSD;
			while (nNum--)
				ExecData_Tag(strSrc, strValue);
		}
	}
	break;
	}
	strValue += _T("</O>");
}


void ZDLT698_45::ExecSetResultNormal(CString & strSrc, CString & strValue)
{
	CString strObject=strSrc.Left(8);
	strSrc.Delete(0,8);
	strValue+=_T("<O obj=\"")+strObject+_T("\">");
	ExecDAR_Tag(strSrc, strValue);
	strValue+=_T("</O>");
}


void ZDLT698_45::ExecActionResultNormal(CString & strSrc, CString & strValue)
{
	CString strObject=strSrc.Left(8);
	strSrc.Delete(0,8);
	strValue+=_T("<O obj=\"")+strObject+_T("\">");
	ExecDAR_Tag(strSrc, strValue);
	int nDataOptional = _tcstol(strSrc.Left(2), NULL, 16);
	strSrc.Delete(0, 2);
	switch (nDataOptional)
	{
	case 0:break;
	case 1:
	{
		ExecData_Tag(strSrc, strValue);
	}
	break;
	}
	strValue+=_T("</O>");
}


CString ZDLT698_45::HexStringToAsciiString(const CString & strHex)
{
	CString strAscii;
	int nSize=strHex.GetLength()/2;
	char * p_c=new char[nSize+1];
	for(int i=0;i<nSize;++i)
		p_c[i]=(char)_tcstol(strHex.Mid(2*i,2),NULL ,16);
	p_c[nSize]='\0';
	strAscii=p_c;
	delete []p_c;
	return strAscii;
}


CString ZDLT698_45::HexStringToUTF8String(const CString & strHex)
{
	CString strUTF8;
	int nSize=strHex.GetLength()/2;
	char * p_c=new char[nSize+1];
	for(int i=0;i<nSize;++i)
		p_c[i]=(char)_tcstol(strHex.Mid(2*i,2),NULL ,16);
	p_c[nSize]='\0';
	ZUtil::UTF8toW(p_c,strUTF8.GetBuffer(nSize),nSize);
	strUTF8.ReleaseBuffer();
	delete []p_c;
	return strUTF8;
}


float ZDLT698_45::HexString2Float(const CString & strSrc)
{
	BYTE sz_cData[8]={0};
	float * p_fData=(float *)sz_cData;
	CStringToBYTEArrBy2(strSrc,sz_cData,sizeof(sz_cData));
	return p_fData[0];
}


double ZDLT698_45::HexString2Double(const CString & strSrc)
{
	BYTE sz_cData[16]={0};
	double * p_dData=(double *)sz_cData;
	CStringToBYTEArrBy2(strSrc,sz_cData,sizeof(sz_cData));
	return p_dData[0];
}


CString ZDLT698_45::HexStringToDateString(CString strHex)
{
	CString strDate;
	int nLength;
	nLength=2;
	int nYear=_tcstol(strHex.Left(nLength*2),NULL,16);
	strHex.Delete(0,nLength*2);
	nLength=1;
	int nMonth=_tcstol(strHex.Left(nLength*2),NULL,16);
	strHex.Delete(0,nLength*2);
	nLength=1;
	int nDay=_tcstol(strHex.Left(nLength*2),NULL,16);
	strHex.Delete(0,nLength*2);
	strDate.Format(_T("%04d%02d%02d"),nYear,nMonth,nDay);
	return strDate;
}


CString ZDLT698_45::HexStringToTimeString(CString strHex)
{
	CString strTime;
	int nLength;
	nLength=1;
	int nHour=_tcstol(strHex.Left(nLength*2),NULL,16);
	strHex.Delete(0,nLength*2);
	nLength=1;
	int nMin=_tcstol(strHex.Left(nLength*2),NULL,16);
	strHex.Delete(0,nLength*2);
	nLength=1;
	int nSec=_tcstol(strHex.Left(nLength*2),NULL,16);
	strHex.Delete(0,nLength*2);
	strTime.Format(_T("%02d%02d%02d"),nHour,nMin,nSec);
	return strTime;
}


CString ZDLT698_45::GetDAR(unsigned int nResult)
{
	CString strDAR;
	const CString sz_strDAR[]=
	{
		_T("成功"),_T("硬件失效"),_T("暂时失效"),_T("拒绝读写"),_T("对象未定义"),_T("对象接口类不符合"),_T("对象不存在"),
		_T("类型不匹配"),_T("越界"),_T("数据块不可用"),_T("分帧传输已取消"),_T("不处于分帧传输状态"),_T("块写取消"),_T("不存在块写状态"),
		_T("数据块序号无效"),_T("密码错/未授权"),_T("通信速率不能更改"),_T("年时区数超"),_T("日时段数超"),_T("费率数超"),
		_T("安全认证不匹配"),_T("重复充值"),_T("ESAM验证失败"),_T("安全认证失败"),_T("客户编号不匹配"),_T("充值次数错误"),
		_T("购电超囤积"),_T("地址异常"),_T("对称解密错误"),_T("非对称解密错误"),_T("签名错误"),_T("电能表挂起"),_T("时间标签无效"),
		_T("请求超时"),_T("ESAM的P1P2不正确"),_T("ESAM的LC错误")
	};
	strDAR=nResult<sizeof(sz_strDAR)/sizeof(CString)?sz_strDAR[nResult]:_T("其它");
	return strDAR;
}


CString ZDLT698_45::ErrorInfo17SGCC(const int & nError)
{
	CString strErrorInfo;
	strErrorInfo.Format(_T("%d%c"), nError, tcSplitError);
	switch(nError)
	{
	case -1: strErrorInfo+=_T("DLL调用出错");break;
	case 45: strErrorInfo+=_T("密码机密钥错");break;
	case 48: strErrorInfo+=_T("无设备或设备无效");break;
	case 56: strErrorInfo+=_T("创建socket句柄失败");break;
	case 57: strErrorInfo+=_T("连接服务器失败");break;
	case 64: strErrorInfo+=_T("客户端发送数据失败");break;
	case 65: strErrorInfo+=_T("客户端接收数据失败");break;
	case 100: strErrorInfo+=_T("打开设备失败");break;
	case 160: strErrorInfo+=_T("连接密码机失败");break;
	case 161: strErrorInfo+=_T("操作权限不够");break;
	case 162: strErrorInfo+=_T("USBKey不是操作员");break;
	case 163: strErrorInfo+=_T("服务器发送数据失败");break;
	case 164: strErrorInfo+=_T("服务端接收报文失败");break;
	case 165: strErrorInfo+=_T("密码机加密数据失败");break;
	case 166: strErrorInfo+=_T("密码机导出密钥失败");break;
	case 167: strErrorInfo+=_T("密码机计算MAC 失败");break;
	case 168: strErrorInfo+=_T("服务器已断开连");break;
	case 169: strErrorInfo+=_T("数据无效");break;
	case 170: strErrorInfo+=_T("密码机收发报文错误");break;
	case 171: strErrorInfo+=_T("密码机故障");break;
	case 172: strErrorInfo+=_T("数据库出错");break;
	case 1100: strErrorInfo+=_T("系统认证错误");break;
	case 1107: strErrorInfo+=_T("USBKey权限不正确");break;
	default:
		{	
			if(nError>=700&&nError<=712)
				strErrorInfo+=_T("客户端导出密钥失败");
			else if(nError>=800&&nError<=810)
				strErrorInfo+=_T("计算MAC失败");
			else if(nError>=900&&nError<=910)
				strErrorInfo+=_T("加密数据失败");
			else if(nError>=1000&&nError<=1010)
				strErrorInfo+=_T("数据长度错");
		}
		break;
	}
	return strErrorInfo;
}


int ZDLT698_45::ConnectDevice17SGCC(void)
{
	typedef int (_stdcall * ConnectDevice)(char * PutIP,char * PutPort,char * PutCtime);
	ConnectDevice cd;
	int nRtn=-1;
	cd=(ConnectDevice)GetProcAddress(m_hDll17SGCC,"ConnectDevice");
	if(cd)
	{
		char sz_cIP[100]={0},sz_cPort[100]={0},sz_cCtime[100]={0};
		ZUtil::WtoA(m_strNetCptIP,sz_cIP,sizeof(sz_cIP));
		ZUtil::WtoA(m_strNetCptPort,sz_cPort,sizeof(sz_cPort));
		CString strNetCptTimeOut;
		strNetCptTimeOut.Format(_T("%d"),_ttoi(m_strNetCptTimeOut)/1000);
		ZUtil::WtoA(strNetCptTimeOut,sz_cCtime,sizeof(sz_cCtime));
		nRtn=cd(sz_cIP,sz_cPort,sz_cCtime);
	}
	return nRtn;
}


int ZDLT698_45::Set_MeterType17SGCC(int nMeterType)
{
	typedef int (_stdcall * Set_MeterType)(int MeterType);
	Set_MeterType smt;
	int nRtn=-1;
	smt=(Set_MeterType)GetProcAddress(m_hDll17SGCC,"Set_MeterType");
	if(smt)
	{
		nRtn=smt(nMeterType);
	}
	return nRtn;
}


int ZDLT698_45::Obj_Meter_Formal_InitSession17SGCC(void)
{
	typedef int (_stdcall * Obj_Meter_Formal_InitSession)(int iKeyState,char * cDiv,char * cASCTR,char * cFLG,char * cOutRandHost,char * cOutSessionInit,char * cOutSign);
	Obj_Meter_Formal_InitSession omfi;
	int nRtn=-1;
	omfi=(Obj_Meter_Formal_InitSession)GetProcAddress(m_hDll17SGCC,"Obj_Meter_Formal_InitSession");
	if(omfi)
	{
		char sz_cDiv[100]={0},sz_cASCTR[100]={0},sz_cFLG[100]={0},sz_cOutRandHost[100]={0},sz_cOutSessionInit[100]={0},sz_cOutSign[100]={0};
		ZUtil::WtoA(m_strDiv,sz_cDiv,sizeof(sz_cDiv));
		ZUtil::WtoA(m_strASCTR,sz_cASCTR,sizeof(sz_cASCTR));
		ZUtil::WtoA(m_strFLG,sz_cFLG,sizeof(sz_cFLG));
		nRtn=omfi(m_nKeyState,sz_cDiv,sz_cASCTR,sz_cFLG,sz_cOutRandHost,sz_cOutSessionInit,sz_cOutSign);
		if(!nRtn)
		{
			m_strRand1=sz_cOutRandHost;
			m_strSessionData1=sz_cOutSessionInit;
			m_strSign1=sz_cOutSign;
		}
	}
	return nRtn;
}


int ZDLT698_45::Obj_Meter_Formal_VerifySession17SGCC(void)
{
	typedef int (_stdcall * Obj_Meter_Formal_VerifySession)(int iKeyState,char * cDiv,char * cRandHost,char * cSessionData,char * cSign,char * cOutSessionKey);
	Obj_Meter_Formal_VerifySession omfv;
	int nRtn=-1;
	omfv=(Obj_Meter_Formal_VerifySession)GetProcAddress(m_hDll17SGCC,"Obj_Meter_Formal_VerifySession");
	if(omfv)
	{
		char sz_cDiv[100]={0},sz_cRandHost[100]={0},sz_cSessionData[100]={0},sz_cSign[100]={0},sz_cOutSessionKey[500]={0};
		ZUtil::WtoA(m_strDiv,sz_cDiv,sizeof(sz_cDiv));
		ZUtil::WtoA(m_strRand1,sz_cRandHost,sizeof(sz_cRandHost));
		ZUtil::WtoA(m_strSessionData2,sz_cSessionData,sizeof(sz_cSessionData));
		ZUtil::WtoA(m_strSign2,sz_cSign,sizeof(sz_cSign));
		nRtn=omfv(m_nKeyState,sz_cDiv,sz_cRandHost,sz_cSessionData,sz_cSign,sz_cOutSessionKey);
		if(!nRtn)
		{
			m_strSessionKey=sz_cOutSessionKey;
		}
	}
	return nRtn;
}


int ZDLT698_45::Obj_Meter_Formal_GetSessionData17SGCC(CString & strData,int nTaskType)
{
	typedef int (_stdcall * Obj_Meter_Formal_GetSessionData)(int iOperateMode,char * cESAMNO,char * cSessionKey,int cTaskType,char * cTaskData,char * cOutSID,char * cOutAttachData,char * cOutData,char * cOutMAC);
	Obj_Meter_Formal_GetSessionData omfg;
	int nRtn=-1;
	omfg=(Obj_Meter_Formal_GetSessionData)GetProcAddress(m_hDll17SGCC,"Obj_Meter_Formal_GetSessionData");
	if(omfg)
	{
		char sz_cESAMNO[100]={0},sz_cSessionKey[500]={0},sz_cTaskData[500]={0},sz_cOutSID[500]={0},sz_cOutAttachData[500]={0},sz_cOutData[500]={0},sz_cOutMAC[500]={0};
		ZUtil::WtoA(m_strESAMNO,sz_cESAMNO,sizeof(sz_cESAMNO));
		ZUtil::WtoA(m_strSessionKey,sz_cSessionKey,sizeof(sz_cSessionKey));
		ZUtil::WtoA(strData,sz_cTaskData,sizeof(sz_cTaskData));
		nRtn=omfg(m_nOperateMode,sz_cESAMNO,sz_cSessionKey,nTaskType,sz_cTaskData,sz_cOutSID,sz_cOutAttachData,sz_cOutData,sz_cOutMAC);
		if(!nRtn)
		{
			m_strSID=sz_cOutSID;
			m_strAttachData=sz_cOutAttachData;
			strData=sz_cOutData;
			m_strMAC=sz_cOutMAC;
		}
	}
	return nRtn;
}


int ZDLT698_45::Obj_Meter_Formal_GetMeterSetData17SGCC(CString & strData)
{
	typedef int (_stdcall * Obj_Meter_Formal_GetMeterSetData)(int iOperateMode,char * cESAMNO,char * cSessionKey,char * cTaskData,char * cOutSID,char * cOutAttachData,char * cOutData,char * cOutMAC);
	Obj_Meter_Formal_GetMeterSetData omfg;
	int nRtn=-1;
	omfg=(Obj_Meter_Formal_GetMeterSetData)GetProcAddress(m_hDll17SGCC,"Obj_Meter_Formal_GetMeterSetData");
	if(omfg)
	{
		char sz_cESAMNO[100]={0},sz_cSessionKey[500]={0},sz_cTaskData[8192]={0},sz_cOutSID[500]={0},sz_cOutAttachData[500]={0},sz_cOutData[8192]={0},sz_cOutMAC[500]={0};
		ZUtil::WtoA(m_strESAMNO,sz_cESAMNO,sizeof(sz_cESAMNO));
		ZUtil::WtoA(m_strSessionKey,sz_cSessionKey,sizeof(sz_cSessionKey));
		ZUtil::WtoA(strData,sz_cTaskData,sizeof(sz_cTaskData));
		nRtn=omfg(m_nOperateMode,sz_cESAMNO,sz_cSessionKey,sz_cTaskData,sz_cOutSID,sz_cOutAttachData,sz_cOutData,sz_cOutMAC);
		if(!nRtn)
		{
			m_strSID=sz_cOutSID;
			m_strAttachData=sz_cOutAttachData;
			strData=sz_cOutData;
			m_strMAC=sz_cOutMAC;
		}
	}
	return nRtn;
}


int ZDLT698_45::Obj_Meter_Formal_VerifyMeterData17SGCC(CString & strData)
{
	typedef int (_stdcall * Obj_Meter_Formal_VerifyMeterData)(int iKeyState,int iOperateMode,char * cESAMNO,char * cSessionKey,char * cTaskData,char * cMAC,char * cOutData);
	Obj_Meter_Formal_VerifyMeterData omfv;
	int nRtn=-1;
	omfv=(Obj_Meter_Formal_VerifyMeterData)GetProcAddress(m_hDll17SGCC,"Obj_Meter_Formal_VerifyMeterData");
	if(omfv)
	{
		char sz_cESAMNO[100]={0},sz_cSessionKey[500]={0},sz_cTaskData[8192]={0},sz_cMAC[500]={0},sz_cOutData[8192]={0};
		ZUtil::WtoA(m_strESAMNO,sz_cESAMNO,sizeof(sz_cESAMNO));
		ZUtil::WtoA(m_strSessionKey,sz_cSessionKey,sizeof(sz_cSessionKey));
		ZUtil::WtoA(strData,sz_cTaskData,sizeof(sz_cTaskData));
		ZUtil::WtoA(m_strMAC,sz_cMAC,sizeof(sz_cMAC));
		nRtn=omfv(m_nKeyState,m_nOperateMode,sz_cESAMNO,sz_cSessionKey,sz_cTaskData,sz_cMAC,sz_cOutData);
		if(!nRtn)
		{
			strData=sz_cOutData;
		}
	}
	return nRtn;
}


int ZDLT698_45::Obj_Meter_Formal_SetESAMData17SGCC(CString & strData)
{
	typedef int (_stdcall * Obj_Meter_Formal_SetESAMData)(int iKeyState,int iOperateMode,char * cESAMNO,char * cSessionKey,char * cMeterNo,char * cESAMRand,char * cData,char * cOutSID,char * cOutAttachData,char * cOutData,char * cOutMAC);
	Obj_Meter_Formal_SetESAMData omfs;
	int nRtn=-1;
	omfs=(Obj_Meter_Formal_SetESAMData)GetProcAddress(m_hDll17SGCC,"Obj_Meter_Formal_SetESAMData");
	if(omfs)
	{
		char sz_cESAMNO[100]={0},sz_cSessionKey[500]={0},sz_cMeterNo[500]={0},sz_cESAMRand[500]={0},sz_cData[8192]={0},sz_cOutSID[500]={0},sz_cOutAttachData[500]={0},sz_cOutData[8192]={0},sz_cOutMAC[500]={0};
		ZUtil::WtoA(m_strESAMNO,sz_cESAMNO,sizeof(sz_cESAMNO));
		ZUtil::WtoA(m_strSessionKey,sz_cSessionKey,sizeof(sz_cSessionKey));
		ZUtil::WtoA(m_strMeterNo,sz_cMeterNo,sizeof(sz_cMeterNo));
		ZUtil::WtoA(m_strRand1,sz_cESAMRand,sizeof(sz_cESAMRand));
		ZUtil::WtoA(strData,sz_cData,sizeof(sz_cData));
		nRtn=omfs(m_nKeyState,m_nOperateMode,sz_cESAMNO,sz_cSessionKey,sz_cMeterNo,sz_cESAMRand,sz_cData,sz_cOutSID,sz_cOutAttachData,sz_cOutData,sz_cOutMAC);
		if(!nRtn)
		{
			m_strSID=sz_cOutSID;
			m_strAttachData=sz_cOutAttachData;
			//strData=sz_cOutData;//适用于40020200 401C0200 401D0200，取入参
			m_strMAC=sz_cOutMAC;
		}
	}
	return nRtn;
}


int ZDLT698_45::Obj_Meter_Formal_GetESAMData17SGCC(CString & strData)
{
	typedef int(_stdcall * Obj_Meter_Formal_GetESAMData)(int iKeyState, int iOperateMode, char * cMeterNo, char * cOAD, char * cOutRandHost, char * cOutSID, char * cOutAttachData);
	Obj_Meter_Formal_GetESAMData omfg;
	int nRtn = -1;
	omfg = (Obj_Meter_Formal_GetESAMData)GetProcAddress(m_hDll17SGCC, "Obj_Meter_Formal_GetESAMData");
	if (omfg)
	{
		char sz_cMeterNo[500] = { 0 }, sz_cOAD[8192] = { 0 }, sz_cOutRndHost[500] = { 0 }, sz_cOutSID[500] = { 0 }, sz_cOutAttachData[500] = { 0 };
		ZUtil::WtoA(m_strMeterNo, sz_cMeterNo, sizeof(sz_cMeterNo));
		ZUtil::WtoA(strData, sz_cOAD, sizeof(sz_cOAD));
		nRtn = omfg(m_nKeyState, m_nOperateMode, sz_cMeterNo, sz_cOAD, sz_cOutRndHost, sz_cOutSID, sz_cOutAttachData);
		if (!nRtn)
		{
			m_strSID = sz_cOutSID;
			m_strAttachData = sz_cOutAttachData;
		}
	}
	return nRtn;
}


int ZDLT698_45::Obj_Meter_Formal_GetTrmKeyData17SGCC(CString & strData)
{
	typedef int(_stdcall * Obj_Meter_Formal_GetTrmKeyData)(int iKeyState, char * cESAMNO, char * cSessionKey, char * cMeterNo, char * cKeyType, char * cOutSID, char * cOutAttachData, char * cOutTrmKeyData, char * cOutMAC);
	Obj_Meter_Formal_GetTrmKeyData omfg;
	int nRtn = -1;
	omfg = (Obj_Meter_Formal_GetTrmKeyData)GetProcAddress(m_hDll17SGCC, "Obj_Meter_Formal_GetTrmKeyData");
	if (omfg)
	{
		char sz_cESAMNO[100] = { 0 }, sz_cSessionKey[500] = { 0 }, sz_cMeterNo[500] = { 0 }, cKeyType[500] = { 0 }, sz_cOutSID[500] = { 0 }, sz_cOutAttachData[500] = { 0 }, sz_cOutTrmKeyData[8192] = { 0 }, sz_cOutMAC[500] = { 0 };
		ZUtil::WtoA(m_strESAMNO, sz_cESAMNO, sizeof(sz_cESAMNO));
		ZUtil::WtoA(m_strSessionKey, sz_cSessionKey, sizeof(sz_cSessionKey));
		ZUtil::WtoA(m_strMeterNo, sz_cMeterNo, sizeof(sz_cMeterNo));
		ZUtil::WtoA(_T("00"), cKeyType, sizeof(cKeyType));
		nRtn = omfg(_ttoi(strData), sz_cESAMNO, sz_cSessionKey, sz_cMeterNo, cKeyType, sz_cOutSID, sz_cOutAttachData, sz_cOutTrmKeyData, sz_cOutMAC);
		if (!nRtn)
		{
			m_strSID = sz_cOutSID;
			m_strAttachData = sz_cOutAttachData;
			strData = sz_cOutTrmKeyData;
			m_strMAC = sz_cOutMAC;
		}
	}
	return nRtn;
}


void ZDLT698_45::GetXmlObjValue(const CString & strXml, std::vector<CString> & vec_strObj, std::vector<CString> & vec_strTValue, std::vector<CString> & vec_strDValue)
{
	vec_strObj.clear();
	vec_strTValue.clear();
	vec_strDValue.clear();
	CMarkup xml;
	if (xml.SetDoc(strXml))
	{
		GetXmlObjValue(xml, vec_strObj, vec_strTValue, vec_strDValue);
	}
}


void ZDLT698_45::GetXmlObjValue(CMarkup & xml, std::vector<CString> & vec_strObj, std::vector<CString> & vec_strTValue, std::vector<CString> & vec_strDValue)
{
	if (xml.FindElem(_T("N")))
		xml.IntoElem();
	while (xml.FindElem(_T("O")))
	{
		CString strAttribObj = xml.GetAttrib(_T("obj"));
		vec_strObj.push_back(strAttribObj);
		xml.IntoElem();
		CString strTValue, strDValue;
		GetXmlValue(xml, strTValue, strDValue);
		strTValue.Delete(strTValue.GetLength() - 1, 1);
		strDValue.Delete(strDValue.GetLength() - 1, 1);
		vec_strTValue.push_back(strTValue);
		vec_strDValue.push_back(strDValue);
		xml.OutOfElem();
	}
}


void ZDLT698_45::GetXmlValue(CMarkup & xml, CString & strTValue, CString & strDValue)
{
	while (xml.FindElem())
	{
		CString strTagName = xml.GetTagName();
		if (strTagName == _T("T"))
		{
			CString strAttribType = xml.GetAttrib(_T("type"));
			if (strAttribType == _T("1") || strAttribType == _T("2"))
			{
				xml.IntoElem();
				GetXmlValue(xml, strTValue, strDValue);
				xml.OutOfElem();
			}
			else
				strTValue += xml.GetData() + CString(tcSplitChar);
		}
		else if (strTagName == _T("D"))
			strDValue += xml.GetData() + CString(tcSplitChar);
	}
}


void ZDLT698_45::GetXmlValue(const CString & strXml, CString & strTValue, CString & strDValue)
{
	CMarkup xml;
	if (xml.SetDoc(strXml))
	{
		GetXmlValue(xml, strTValue, strDValue);
	}
}


void ZDLT698_45::UpdateVariables85H(const CString & strValue)
{
	std::vector<CString> vec_strObj,vec_strTValue, vec_strDValue;
	GetXmlObjValue(strValue,vec_strObj,vec_strTValue, vec_strDValue);
	int nSize=vec_strObj.size();
	for(int i=0;i<nSize;++i)
	{
		if(vec_strObj[i]==_T("40010200"))
		{
			m_strServerAddr=vec_strTValue[i];
		}
		if(vec_strObj[i]==_T("F1000400"))
		{
			if(vec_strTValue[i].Left(9)==_T("7FFFFFFFF"))
				m_nKeyState=1;
			else
				m_nKeyState=0;
		}
		if(vec_strObj[i]==_T("40020200"))
		{
			m_strMeterNo=_T("0000")+ vec_strTValue[i];
			if(m_nKeyState==1)
				m_strDiv=_T("0000")+ vec_strTValue[i];
		}
		if(vec_strObj[i]==_T("F1000200"))
		{
			m_strESAMNO= vec_strTValue[i];
			if(m_nKeyState==0)
				m_strDiv= vec_strTValue[i];
		}
		if(vec_strObj[i]==_T("F1000701"))
		{
			m_strASCTR.Format(_T("%08X"),_ttoi(vec_strTValue[i])+1);
		}
		if(vec_strObj[i]==_T("F1000700"))
		{
			m_strASCTR.Format(_T("%08X"),_ttoi(vec_strTValue[i])+1);
		}
	}
}


void ZDLT698_45::UpdateVariables86H(const CString & strValue)
{
	std::vector<CString> vec_strObj, vec_strTValue, vec_strDValue;
	GetXmlObjValue(strValue, vec_strObj, vec_strTValue, vec_strDValue);
	int nSize=vec_strObj.size();
	for(int i=0;i<nSize;++i)
	{
		if(vec_strObj[i]==_T("40010200")&&m_vec_strObject.size()==nSize&&vec_strObj[i]==m_vec_strObject[i]&& vec_strDValue[i]==_T("成功"))
		{
			m_strServerAddr=m_vec_strTgtData[i].Right(12);
		}
		if(vec_strObj[i]==_T("40020200")&&m_vec_strObject.size()==nSize&&vec_strObj[i]==m_vec_strObject[i]&& vec_strDValue[i]==_T("成功"))
		{
			m_strMeterNo=_T("0000")+m_vec_strTgtData[i].Right(12);
			if(m_nKeyState==1)
				m_strDiv=_T("0000")+m_vec_strTgtData[i].Right(12);
		}
	}
}


void ZDLT698_45::UpdateVariables87H(const CString & strValue)
{
	std::vector<CString> vec_strObj, vec_strTValue, vec_strDValue;
	GetXmlObjValue(strValue, vec_strObj, vec_strTValue, vec_strDValue);
	int nSize=vec_strObj.size();
	for(int i=0;i<nSize;++i)
	{
		if(vec_strObj[i]==_T("F1000400")&&m_vec_strObject.size()==nSize&&vec_strObj[i]==m_vec_strObject[i]&&m_vec_strTgtData[i].Left(8)==_T("40020200")&& vec_strDValue[i]==_T("成功"))
		{
			m_strMeterNo=_T("0000")+m_vec_strTgtData[i].Right(12);
			if(m_nKeyState==1)
				m_strDiv=_T("0000")+m_vec_strTgtData[i].Right(12);
		}
	}
}


CString ZDLT698_45::GetServerAddr(void)
{
	return m_strServerAddr;
}


CString ZDLT698_45::GetConnectResult(unsigned int nResult)
{
	CString strConntRslt;
	const CString sz_strConntRslt[]=
	{
		_T("允许建立应用连接"),_T("密码错误"),_T("对称解密错误"),_T("非对称解密错误"),_T("签名错误"),_T("协议版本不匹配 ")
	};
	strConntRslt=nResult<sizeof(sz_strConntRslt)/sizeof(CString)?sz_strConntRslt[nResult]:_T("其他错误");
	return strConntRslt;
}


int ZDLT698_45::ExecESAMData(const CString & strFObject, CString & strFTargetData,CString & strError)
{
	CString strLength,strFSID,strFAttachData,strFMAC;
	if (strFObject == _T("F1000300"))
	{
		m_nOperateMode = 1;
		switch (m_nDllType)
		{
		case 1:
		{
			int nRtn = Obj_Meter_Formal_GetESAMData17SGCC(strFTargetData);
			if (nRtn)
			{
				strError = _T("error:0108") + CString(tcSplitError) + ErrorInfo17SGCC(nRtn);
				return 108;
			}
		}
		break;
		}
		strFSID = m_strSID;
		strFAttachData.Format(_T("%02X"), m_strAttachData.GetLength() / 2);
		strFAttachData += m_strAttachData;
		strFTargetData = _T("5D") + strFSID + strFAttachData;
	}
	if(strFObject==_T("F1000400"))
	{
		m_nOperateMode=1;
		switch (m_nDllType)
		{
		case 1:
		{
			int nRtn = Obj_Meter_Formal_SetESAMData17SGCC(strFTargetData);
			if (nRtn)
			{
				strError = _T("error:0105") + CString(tcSplitError) + ErrorInfo17SGCC(nRtn);
				return 105;
			}
		}
		break;
		}
		strLength = ExecLength(strFTargetData.GetLength() / 2);
		strFSID=m_strSID;
		strFAttachData.Format(_T("%02X"),m_strAttachData.GetLength()/2);
		strFAttachData+=m_strAttachData;
		strFMAC.Format(_T("%02X"),m_strMAC.GetLength()/2);
		strFMAC+=m_strMAC;
		strFTargetData=_T("020209")+strLength+strFTargetData+_T("5E")+strFSID+strFAttachData+strFMAC;
	}
	if (strFObject == _T("F1000700"))
	{
		switch (m_nDllType)
		{
		case 1:
		{
			int nRtn = Obj_Meter_Formal_GetTrmKeyData17SGCC(strFTargetData);
			if (nRtn)
			{
				strError = _T("error:0107") + CString(tcSplitError) + ErrorInfo17SGCC(nRtn);
				return 107;
			}
		}
		break;
		}
		strLength=ExecLength(strFTargetData.GetLength() / 2);
		strFSID = m_strSID;
		strFAttachData.Format(_T("%02X"), m_strAttachData.GetLength() / 2);
		strFAttachData += m_strAttachData;
		strFMAC.Format(_T("%02X"), m_strMAC.GetLength() / 2);
		strFMAC += m_strMAC;
		strFTargetData = _T("020209") + strLength + strFTargetData + _T("5E") + strFSID + strFAttachData + strFMAC;
	}
	if(strFObject==_T("F1000900"))
	{
		m_nOperateMode=2;
		switch (m_nDllType)
		{
		case 1:
		{
			int nRtn = Obj_Meter_Formal_GetSessionData17SGCC(strFTargetData, 2);
			if (nRtn)
			{
				strError = _T("error:0106") + CString(tcSplitError) + ErrorInfo17SGCC(nRtn);
				return 106;
			}
		}
		break;
		}
		strLength = ExecLength(strFTargetData.GetLength() / 2);
		strFSID=m_strSID;
		strFAttachData.Format(_T("%02X"),m_strAttachData.GetLength()/2);
		strFAttachData+=m_strAttachData;
		strFTargetData=_T("020209")+strLength+strFTargetData+_T("5D")+strFSID+strFAttachData;
	}
	return 0;
}


CString ZDLT698_45::GetTheObject(void)
{
	return m_strObject;
}


int ZDLT698_45::GetEncodeRemoteData(CString & strData, CString & strError)
{
	int nRtn = 0;
	CString strDataIn, strDataOut, strHead;
	strDataIn = GetParamByCstring();
	strHead.Format(_T("[%06d12]"), strDataIn.GetLength() + 10);
	strDataIn = strHead + strDataIn;
	if (!SocketCommunication(strDataIn, strDataOut))
	{
		strError = _T("error:1094") + CString(tcSplitError) + _T("远程数据获取失败!");
		return 1094;
	}
	if (strDataOut.Mid(0, 1) == _T("[") && strDataOut.Mid(9, 1) == _T("]") && _ttoi(strDataOut.Mid(1, 6)) == strDataOut.GetLength() && strDataOut.Mid(7, 2) == _T("12"))
	{
		strDataOut.Delete(0, 10);
		std::vector<CString> vec_strData;
		ZUtil::StrSplit(strDataOut, vec_strData, tcSplitNetData, strDataOut.Right(1) == tcSplitNetData);
		if (vec_strData.size() == 2)
		{
			nRtn = _ttoi(vec_strData[0]);
			if (nRtn)
			{
				strError = vec_strData[1];
			}
			else
			{
				SetParamByCstring(vec_strData[1].Left(vec_strData[1].ReverseFind(_T(','))));
				strData = vec_strData[1].Mid(vec_strData[1].ReverseFind(_T(',')) + 1);
			}
			return nRtn;
		}
	}
	strError = _T("error:1095") + CString(tcSplitError) + _T("远程数据格式错误!");
	return 1095;
}


bool ZDLT698_45::GetDecodeRemoteData(const CString  & strData,CString & strValue)
{
	bool bRtn = false;
	CString strDataIn, strDataOut, strHead;
	strDataIn = GetParamByCstring()+strData;
	strHead.Format(_T("[%06d13]"), strDataIn.GetLength() + 10);
	strDataIn = strHead + strDataIn;
	if (!SocketCommunication(strDataIn, strDataOut))
	{
		return false;
	}
	if (strDataOut.Mid(0, 1) == _T("[") && strDataOut.Mid(9, 1) == _T("]") && _ttoi(strDataOut.Mid(1, 6)) == strDataOut.GetLength() && strDataOut.Mid(7, 2) == _T("13"))
	{
		strDataOut.Delete(0, 10);
		std::vector<CString> vec_strData;
		ZUtil::StrSplit(strDataOut, vec_strData, tcSplitNetData, strDataOut.Right(1) == tcSplitNetData);
		if (vec_strData.size() == 2)
		{
			bRtn = _ttoi(vec_strData[0]);
			if (bRtn)
			{
				SetParamByCstring(vec_strData[1].Left(vec_strData[1].ReverseFind(_T(','))));
				strValue = vec_strData[1].Mid(vec_strData[1].ReverseFind(_T(',')) + 1);
			}
			return bRtn;
		}
	}
	return false;
}


bool ZDLT698_45::SocketCommunication(const CString & strDataIn, CString & strDataOut)
{
	ZStringSocket zSock;
	int nRtn = ZSocket::ERROR_OK;
	zSock.SetTimeOut(ZSocket::TIMEOUT_CONT, _ttoi(m_strServerTimeOut));
	zSock.SetTimeOut(ZSocket::TIMEOUT_SEND, _ttoi(m_strServerTimeOut));
	zSock.SetTimeOut(ZSocket::TIMEOUT_RECV, _ttoi(m_strServerTimeOut));
	nRtn = zSock.Connect(m_strServerIP, m_strServerPort);
	if (nRtn)
		return false;
	nRtn = zSock.StringSend(strDataIn);
	if (nRtn)
		return false;
	nRtn = zSock.StringRecv(strDataOut);
	if (nRtn)
		return false;
	return true;
}


CString ZDLT698_45::GetParamByCstring(void)
{
	CString strRtn, strDllType, strKeyState, strOperateMode;
	CString str(_T(","));
	strDllType.Format(_T("%d"), m_nDllType);
	strKeyState.Format(_T("%d"), m_nKeyState);
	strOperateMode.Format(_T("%d"), m_nOperateMode);
	strRtn = m_strCtrlArea + str + m_strServerAddr + str + m_strReqResCode + str + m_strReqResChoice + str + m_strPIID + str + m_strObjNum + str + m_strObject + str + m_strTgtData + str + m_strTimeTag + str
		+ m_strGrade + str + m_strPsWd + str + m_strOperaCode + str + strKeyState + str + m_strDiv + str + m_strMeterNo + str + m_strESAMNO + str + m_strASCTR + str
		+ m_strFLG + str + m_strRand1 + str + m_strSessionData1 + str + m_strSign1 + str + m_strSessionData2 + str+ m_strSign2+str+ m_strSessionKey+str+ strOperateMode+str
		+ m_strSID+str+ m_strAttachData+str+ m_strMAC+str+ strDllType + str;
	return strRtn;
}


bool ZDLT698_45::SetParamByCstring(const CString & strParam)
{
	std::vector<CString> vec_strDatas;
	ZUtil::StrSplit(strParam, vec_strDatas, _T(','), strParam.Right(1) == _T(','));
	if (vec_strDatas.size() != 29)
		return false;
	int i = 0;
	m_strCtrlArea= vec_strDatas[i++];
	m_strServerAddr = vec_strDatas[i++];
	m_strReqResCode = vec_strDatas[i++];
	m_strReqResChoice = vec_strDatas[i++];
	m_strPIID = vec_strDatas[i++];
	m_strObjNum = vec_strDatas[i++];
	m_strObject = vec_strDatas[i++];
	m_strTgtData = vec_strDatas[i++];
	std::vector<CString> vec_strObj, vec_strTgt;
	ZUtil::StrSplit(m_strObject, vec_strObj, tcSplitChar, m_strObject.Right(1) == tcSplitChar);
	ZUtil::StrSplit(m_strTgtData, vec_strTgt, tcSplitChar, m_strTgtData.Right(1) == tcSplitChar);
	int nNum = vec_strObj.size()>vec_strTgt.size() ? vec_strTgt.size() : vec_strObj.size();
	m_vec_strObject.clear();
	m_vec_strTgtData.clear();
	for (int i = 0; i<nNum; ++i)
	{
		m_vec_strObject.push_back(vec_strObj[i]);
		m_vec_strTgtData.push_back(vec_strTgt[i]);
	}
	m_strTimeTag = vec_strDatas[i++];
	m_strGrade = vec_strDatas[i++];
	m_strPsWd = vec_strDatas[i++];
	m_strOperaCode = vec_strDatas[i++];
	m_nKeyState= _ttoi(vec_strDatas[i++]);
	m_strDiv = vec_strDatas[i++];
	m_strMeterNo = vec_strDatas[i++];
	m_strESAMNO = vec_strDatas[i++];
	m_strASCTR = vec_strDatas[i++];
	m_strFLG = vec_strDatas[i++];
	m_strRand1 = vec_strDatas[i++];
	m_strSessionData1 = vec_strDatas[i++];
	m_strSign1 = vec_strDatas[i++];
	m_strSessionData2 = vec_strDatas[i++];
	m_strSign2 = vec_strDatas[i++];
	m_strSessionKey = vec_strDatas[i++];
	m_nOperateMode = _ttoi(vec_strDatas[i++]);
	m_strSID = vec_strDatas[i++];
	m_strAttachData = vec_strDatas[i++];
	m_strMAC = vec_strDatas[i++];
	m_nDllType = _ttoi(vec_strDatas[i++]);
	return true;
}


void ZDLT698_45::ChooseDll(const int & nDllType)
{
	m_nDllType = nDllType;
}


int ZDLT698_45::GetDllChoose(void)
{
	return m_nDllType;
}


void ZDLT698_45::ChooseMode(const BOOL & nIsRemote)
{
	m_nIsRemote = nIsRemote;
}


BOOL ZDLT698_45::GetModeChoose(void)
{
	return m_nIsRemote;
}


void ZDLT698_45::SetServerInfo(const CString & strIP, const CString & strPort, const CString & strTimeOut)
{
	m_strServerIP = strIP;
	m_strServerPort = strPort;
	m_strServerTimeOut = strTimeOut;
}


void ZDLT698_45::GetServerInfo(CString & strIP, CString & strPort, CString & strTimeOut)
{
	strIP = m_strServerIP;
	strPort = m_strServerPort;
	strTimeOut = m_strServerTimeOut;
}


void ZDLT698_45::SetNetCptInfo(const CString & strIP, const CString & strPort, const CString & strTimeOut)
{
	m_strNetCptIP = strIP;
	m_strNetCptPort = strPort;
	m_strNetCptTimeOut = strTimeOut;
}


void ZDLT698_45::GetNetCptInfo(CString & strIP, CString & strPort, CString & strTimeOut)
{
	strIP = m_strNetCptIP;
	strPort = m_strNetCptPort;
	strTimeOut = m_strNetCptTimeOut;
}


CString ZDLT698_45::GetReqResCode(void)
{
	return m_strReqResCode;
}


void ZDLT698_45::SetReqResCode(const CString & strReqResCode)
{
	m_strReqResCode = strReqResCode;
}


CString ZDLT698_45::ExecLength(const int & nLength)
{
	CString strLength;
	if (nLength < 0x80)
		strLength.Format(_T("%02X"), nLength);
	else
	{
		int n = 1;
		int nLen = nLength;
		while (nLen / 0xFF)
		{
			++n;
			nLen %= 0xFF;
		}
		CString strHead;
		strHead.Format(_T("%02X"), 0x80 | n);
		strLength.Format(_T("%X"), nLength);
		if (strLength.GetLength() % 2)
			strLength = _T("0") + strLength;
		strLength = strHead + strLength;
	}
	return strLength;
}


int ZDLT698_45::ReverseExecLength(CString & strData)
{
	int nLength;
	int n = _tcstol(strData.Left(2), NULL, 16);
	strData.Delete(0, 2);
	if (n & 0x80)
	{
		nLength = _tcstol(strData.Left(2 * (n ^ 0x80)), NULL, 16);
		strData.Delete(0, 2 * (n ^ 0x80));
	}
	else
		nLength = n;
	return nLength;
}


bool ZDLT698_45::IsSplitSend(void)
{
	return m_bIsSplitSend;
}


bool ZDLT698_45::IsSplitRecv(void)
{
	return m_bIsSplitRecv;
}