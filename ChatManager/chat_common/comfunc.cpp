#include "../stdafx.h"
#include "comfunc.h"
#include "comdef.h"
#include "md5/md5.h"
#include "utils/tstring.h"
#include "log/winlog.h"
#include "utils/convert.h"
#include "http_load/http_unit.h"
#include <algorithm>

//字符串到长整形的互换
DWORD StringToDWORD(string str)
{
	return (DWORD)atoi(str.c_str());
}

bool IsNumber(string lpStr)
{
	if (lpStr.empty())
	{
		return false;
	}

	char ch;

	for (int i = 0; i < lpStr.size(); i++)
	{
		ch = lpStr.at(i);
		if ((ch < '0') || (ch > '9'))
		{
			return false;
		}
	}
	return true;
}

//获得本机的地址
char * GetLocalHost(char * szAddress)
{
	if (gethostname(szAddress, 128) == SOCKET_ERROR)
	{
	int nError = GetLastError();
	return NULL;
	}
	else
		return szAddress;
}

string GetCurrentPath()
{
	char szMyDllFileName[MAX_1024_LEN];
	string str;

	memset(szMyDllFileName, 0, 1024);
	::GetModuleFileNameA(NULL, szMyDllFileName, MAX_1024_LEN);
	str = szMyDllFileName;

	int pos = str.find_last_of('\\');

	if (pos > -1)
	{
		str = str.substr(0, pos + 1);
	}
	return str;
}

bool LoadIniString(char *sApp, char *sKey, char *sReturn, int len, char *sFile, char *sDefault)
{
	if (sApp == NULL || sKey == NULL || sFile == NULL)
		return false;

	char sDefault1[1024], strString[1024];
	bool brtn = false;

	if (sDefault == NULL)
	{
		strcpy(sDefault1, sReturn);
	}
	else
	{
		strcpy(sDefault1, sDefault);
	}
	strcpy(strString, sDefault1);

	if (GetPrivateProfileStringA(sApp, sKey, sDefault1, strString, len, sFile))
	{
		if (strlen(strString) >= 0)
		{
			strcpy(sReturn, strString);
			brtn = true;
		}
	}

	return brtn;

}

bool LoadIniInt(char *sApp, char *sKey, int &nReturn, char *sFile, char *sDefault)
{
	if (sApp == NULL || sKey == NULL || sFile == NULL)
		return false;

	bool brtn = false;
	char sDefault1[256], strString[256];
	if (sDefault == NULL)
	{
		sprintf(sDefault1, "%d", nReturn);
	}
	else
	{
		sprintf(sDefault1, sDefault);
	}
	strcpy(strString, sDefault1);

	if (GetPrivateProfileStringA(sApp, sKey, sDefault1, strString, 256, sFile))
	{
		if (strlen(strString) > 0)
		{
			nReturn = atol(strString);
			brtn = true;
		}
	}

	return brtn;
}

//获取某几位的值
unsigned long GetMutiByte(unsigned int value, int index, int bytenum)
{
	int byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	unsigned int rtn = value >> index;  //右移
	rtn &= byte;  //1111

	return rtn;
}

unsigned short GetMutiByte(unsigned short value, int index, int bytenum)
{
	int byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	unsigned long rtn = value >> index;  //右移
	rtn &= byte;  //1111

	return (unsigned short)rtn;
}

unsigned char GetMutiByte(unsigned char value, int index, int bytenum)
{
	int byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	unsigned long rtn = value >> index;  //右移
	rtn &= byte;  //1111

	return (unsigned char)rtn;
}

//设置多位的值
unsigned int SetMutiByte(unsigned int &source, int index, int bytenum, unsigned int value)
{
	// 先取出要设置的位，并移到指定的位置
	int byte;

	byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	value &= byte;
	value = value << index;   //左移

	//指定的位全为1 和全为0的情况
	int rtn0, rtn1;
	rtn1 = byte << index;   //左移
	rtn0 = ~rtn1;

	//首先将指定的位置为0
	source &= rtn0;

	//设置指定位
	source |= value;

	return source;
}


unsigned short SetMutiByte(unsigned short &source, int index, int bytenum, unsigned short value)
{
	// 先取出要设置的位，并移到指定的位置
	int byte;

	byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	value &= byte;
	value = value << index;   //左移

	//指定的位全为1 和全为0的情况
	int rtn0, rtn1;
	rtn1 = byte << index;   //左移
	rtn0 = ~rtn1;

	//首先将指定的位置为0
	source &= rtn0;

	//设置指定位
	source |= value;

	return source & 0xffff;
}

unsigned char SetMutiByte(unsigned char &source, int index, int bytenum, unsigned char value)
{
	// 先取出要设置的位，并移到指定的位置
	int byte;

	byte = (int)(pow(2, bytenum) - 1); //结构为二进制 1111 (2**4 - 1)
	value &= byte;
	value = value << index;   //左移

	//指定的位全为1 和全为0的情况
	int rtn0, rtn1;
	rtn1 = byte << index;   //左移
	rtn0 = ~rtn1;

	//首先将指定的位置为0
	source &= rtn0;

	//设置指定位
	source |= value;

	return source & 0xff;
}

unsigned short SendOnePack(SOCKET socket, char *sbuff, TCP_PACK_HEADER tcppackhead, int &nError)
{
	if (tcppackhead.len > PACKNORMALLEN)
	{
		nError = -1;
		return -1;
	}

	nError = 0;
	char data[PACKMAXLEN + sizeof(TCP_PACK_HEADER)] = { 0 };

	int ret, idx, tcpheadlen;
	int nLeft, nSend;

	tcpheadlen = sizeof(TCP_PACK_HEADER);
	*(TCP_PACK_HEADER *)data = tcppackhead;
	memcpy((data + tcpheadlen), sbuff, tcppackhead.len);

	nLeft = tcpheadlen + tcppackhead.len;
	idx = 0;
	nSend = 0;
	while (nLeft > 0)
	{
		ret = send(socket, (char *)(data + idx), nLeft, 0);
		if (ret == SOCKET_ERROR)
		{
			nError = GetLastError();
			if (nError == WSAEWOULDBLOCK)
			{
				Sleep(500);
				continue;
			}
			goto RETURN;
		}
		idx += ret;
		nSend += ret;
		nLeft -= ret;
	}

RETURN:
	return nSend;
}

unsigned short SendOnePack(SOCKET socket, char *data, int len, int &nError, unsigned short cmd,
	unsigned long senduid, unsigned long recvuid, unsigned long sendsock, unsigned long recvsock,
	unsigned short seq)
{
	TCP_PACK_HEADER tcppackhead;

	memset((char *)(&tcppackhead), 0, sizeof(tcppackhead));


	tcppackhead.len = len;


	return SendOnePack(socket, data, tcppackhead, nError);
}

int SendAllBuff(SOCKET socket, const char *sbuff, int len, int &nError)
{
	nError = 0;

	int ret, idx;
	int nLeft, nSend;

	nLeft = len;
	idx = 0;
	nSend = 0;
	while (nLeft > 0)
	{
		ret = send(socket, (sbuff + idx), nLeft, 0);
		if (ret == SOCKET_ERROR)
		{
			nError = GetLastError();
			closesocket(socket);
		goto RETURN;
		}
		idx += ret;
		nSend += ret;
		nLeft -= ret;
	}

RETURN:
	return nSend;
}

void ConvertMsg(char *msg, int buflen)
{
	wstring s = convertstring((const char *)msg);
	ConvertWidecharToChar(s.c_str(), -1, msg, buflen, false);
}


void ConvertWidecharToChar(const WCHAR *pFrom, int len, char *pTo, int buflen, bool butf8)
{
	if (pFrom == NULL || pTo == NULL)
		return;

	if (len == 0)
	{
		strcpy(pTo, "");
		return;
	}

	if (butf8)
	{
		len = ::WideCharToMultiByte(CP_UTF8, 0, pFrom, len, pTo, buflen, NULL, NULL);
	}
	else
	{
		len = ::WideCharToMultiByte(CP_ACP, 0, pFrom, len, pTo, buflen, NULL, NULL);
	}

	if (len > 0)
		pTo[len] = '\0';
	else
	{
		DWORD dd = GetLastError();
	}

	return;
}

WxObj* ParseWxJsonMsg(const char* msg)
{
	Json::Value jv;
	if (!ParseJson(msg, jv))
	{
		return NULL;
	}

	string MsgType = GetStrFromJson(jv, "msgtype");
	if (MsgType.empty())
	{
		return NULL;
	}

	WxObj  *pwxobj = NULL;
	if ("text" == MsgType)
	{
		pwxobj = new WxMsgText(MsgType);
	}
	else if ("image" == MsgType)
	{
		pwxobj = new WxMsgImage(MsgType);
	}
	else if ("voice" == MsgType)
	{
		pwxobj = new WxMsgVoice(MsgType);
	}
	else if ("video" == MsgType || "shortvideo" == MsgType)
	{
		pwxobj = new WxMsgVideo(MsgType);
	}
	else if ("location" == MsgType)
	{
		pwxobj = new WxMsgLocation(MsgType);
	}
	else if ("link" == MsgType)
	{
		pwxobj = new WxMsgLink(MsgType);
	}
	else if ("news" == MsgType)
	{
		pwxobj = new WxMsgNews(MsgType);
	}
	else if ("event" == MsgType)
	{
		string szEvent = GetStrFromJson(jv, "event");

		if ("subscribe" == szEvent || "unsubscribe" == szEvent || "SCAN" == szEvent)
		{
			pwxobj = new WxEventSubscribe(MsgType);
		}
		else if ("CLICK" == szEvent || "VIEW" == szEvent)
		{
			pwxobj = new WxEventMenu(MsgType);
		}
		else if ("LOCATION" == szEvent)
		{
			pwxobj = new WxEventLocation(MsgType);

		}
		else if ("MASSSENDJOBFINISH" == szEvent)
		{
			pwxobj = new WxEventMasssendjobfinish(MsgType);
		}
	}
	else if ("userinfo" == MsgType)
	{
		pwxobj = new WxUserInfo();
	}
	else if ("wxactoken" == MsgType)
	{
		pwxobj = new WxAccessTokenInfo();
	}

	if (pwxobj != NULL)
	{
		pwxobj->ParseFromJson(jv);
	}

	return pwxobj;
}

string GetTimeByMDAndHMS(unsigned long ntime)
{
	string stime = "";
	char ctime[MAX_256_LEN];
	time_t tt;
	if (ntime == 0)
		tt = time(NULL);
	else
		tt = (time_t)(ntime);

	struct tm * ttm = localtime(&tt);
	if (ttm != NULL)
	{
		sprintf(ctime, "%d-%d %d:%d:%d", ttm->tm_mon + 1, ttm->tm_mday, ttm->tm_hour, ttm->tm_min, ttm->tm_sec);
		stime = ctime;
	}

	return stime.c_str();
}

char *GetContentBetweenString(const char *str, const char *sstart, const char * send, char *content)
{
	if (str == NULL || sstart == NULL || send == NULL || content == NULL)
		return NULL;

	strcpy(content, "");
	char *p1, *p2;

	p1 = (char*)strstr(str, sstart);
	if (p1 == NULL)
		return NULL;

	p2 = strstr(p1, send);
	if (p2 == NULL)
		strcpy(content, p1);
	else
	{
		int len0 = strlen(sstart);
		int len = p2 - p1 - len0;

		if (len > 0)
		{
			memcpy(content, p1 + len0, len);
			content[len] = '\0';
		}
	}

	return content;
}

bool GetByte(unsigned int value, int index)
{
	unsigned int  rtn = value >> index;  //右移
	rtn &= 0x0001;

	if (rtn == 0)
		return false;
	else
		return true;
}

unsigned int SetByte(unsigned int &source, int index, unsigned char ucvalue)
{
	//先将要设置的位取出并移到指定的位置上
	unsigned long value = (unsigned long)ucvalue;
	value &= 0x0001;
	value = value << index;   //左移

	int rtn0, rtn1;
	rtn1 = 0x0001;
	rtn1 = rtn1 << index;   //左移
	rtn0 = ~rtn1;

	//首先将设置值的指定的位置为设为0
	source &= rtn0;

	//设置指定位
	source |= value;

	return source;
}

string FullPath(string extPath)
{
	extPath = GetCurrentPath()+ extPath;
	return extPath;
}

unsigned long GetTimeLong()
{
	unsigned long ntime = 0;
	time_t tt = time(NULL);
	struct tm * stTime = localtime(&tt);
	if (stTime != NULL)
	{
		ntime = (long)_mkgmtime(stTime);  //转换成了unsigned long类型
	}
	return ntime;	
}

string GetMd5Str(const string str)
{
	unsigned char kmd[32];
	char strReturn[256];
	sprintf(strReturn, "%s", str.c_str());
	struct MD5Context md5c;
	MD5Init(&md5c);
	MD5Update(&md5c, (unsigned char *)strReturn, strlen(strReturn));
	MD5Final((unsigned char *)kmd, &md5c);

	dec2hex(kmd, strReturn);
	strReturn[32] = '\0';
	return strReturn;
}

string DWORDToString(unsigned long num)
{
	if (num == 0)
		return "0";

	char str[256];

	_itoa(num, str, 10);
	return str;
}

bool IsXMLCommandStringExist(char *pBuff, char *cmditem)
{
	if (pBuff == NULL || cmditem == NULL)
		return false;

	char sitem1[100], sitem2[100];
	char *p1, *p2;
	int len1, len2;
	sprintf(sitem1, "<%s>", cmditem);
	sprintf(sitem2, "</%s>", cmditem);
	len1 = strlen(sitem1);
	len2 = strlen(sitem2);

	p1 = strstr(pBuff, sitem1);
	p2 = strstr(pBuff, sitem2);

	return (p1 == pBuff && p2 != NULL && p2 > p1);
}

char *GetXMLCommandString(char *pBuff, char *cmd, char *cmditem, int maxlen)
{
	if (pBuff == NULL || cmd == NULL || cmditem == NULL)
		return "";

	char sitem1[100], sitem2[100];
	char *p1, *p2;
	int len1, len2, len;
	sprintf(sitem1, "<%s>", cmditem);
	sprintf(sitem2, "</%s>", cmditem);
	len1 = strlen(sitem1);
	len2 = strlen(sitem2);

	p1 = strstr(pBuff, sitem1);
	p2 = strstr(pBuff, sitem2);

	sprintf(cmd, "");
	if (p1 != NULL && p2 != NULL)
	{
		len = p2 - p1 - len1;
		if (len > 0)
		{
			if (len > maxlen)
				len = maxlen;

			memcpy(cmd, p1 + len1, len);
			cmd[len] = '\0';
		}
	}

	return cmd;

}

int GetXMLCommandInt(char *pBuff, char *cmditem)
{
	char cmd[50];

	GetXMLCommandString(pBuff, cmd, cmditem, 48);

	return atol(cmd);
}

bool ParseSearchURLHostAndVar(string strurl, string &strHost, string &strVar)
{
	return false;
}

string GetTimeByYMDAndHMS(unsigned long ntime)
{
	string stime = "";
	char ctime[MAX_256_LEN];
	time_t tt;
	if (ntime == 0)
		tt = time(NULL);
	else
		tt = (time_t)(ntime);

	struct tm * ttm = localtime(&tt);
	if (ttm != NULL)
	{
		sprintf(ctime, "%d-%d-%d %d:%d:%d", ttm->tm_year + 1900, ttm->tm_mon + 1, ttm->tm_mday, ttm->tm_hour, ttm->tm_min, ttm->tm_sec);
		stime = ctime;
	}

	return stime;
}

int GetApplyTypeID(string stype)
{
	if (stype.compare("APPLY_ASK") == 0)
		return APPLY_ASK;
	else if (stype.compare("APPLY_OPEN") == 0)
		return APPLY_OPEN;
	else if (stype.compare("MYGETNOTE") == 0)
		return MYGETNOTE;
	else
		return APPLY_UNKNOWN;
}

string GetApplyTypeString(int type)
{
	switch (type)
	{
	case APPLY_ASK:
		return "APPLY_ASK";
	case APPLY_OPEN:
		return "APPLY_OPEN";
	case MYGETNOTE:
		return "GETNOTE";
	default:
		return "";
	}
}

std::string GetTimeString()
{
	char stime[MAX_256_LEN];
	time_t tt = time(NULL);
	struct tm * ttm = localtime(&tt);
	if (ttm != NULL)
	{
		sprintf(stime, "%d%d%d%d%d", ttm->tm_mon + 1, ttm->tm_mday, ttm->tm_hour, ttm->tm_min, ttm->tm_sec);
	}
	return stime;
}

unsigned long GetTimeLongByDHMS()
{
	unsigned long ntime = 0;
	time_t tt = time(NULL);
	struct tm * ttm = localtime(&tt);
	if (ttm != NULL)
	{
		char stime[MAX_256_LEN];
		sprintf(stime, "%d%d%d%d%d", ttm->tm_mon + 1, ttm->tm_mday, ttm->tm_hour, ttm->tm_min, ttm->tm_sec);
		ntime = atol(stime);
	}
	return ntime;
}

int SendFileToWebUserFunc(string &strUpLink, string strPath, unsigned long CurUserId, unsigned long sendUserId)
{
	int nRetCode = 404;
	string body, urlfile, strhttpHeader;
	char sBuff[MAX_1024_LEN] = { 0 };

	string strText, strLower;
	int pos = strPath.find_last_of('.');
	if (pos != -1)
	{
		int nExtLen = strPath.length() - pos - 1;
		strLower = strText = strPath.substr(pos + 1, nExtLen);
	}
	else
		return -1;//文件名后缀没有

	transform(strLower.begin(), strLower.end(), strLower.begin(), ::tolower);
	if (("gif") == strLower || ("jpg") == strLower || ("jpeg") == strLower || ("png") == strLower ||
		("bmp") == strLower || ("zip") == strLower || ("rar") == strLower || ("txt") == strLower ||
		("xls") == strLower || ("swf") == strLower || ("ppt") == strLower || ("wps") == strLower ||
		("doc") == strLower || ("docx") == strLower || ("xlsx") == strLower || ("pptx") == strLower || ("pdf") == strLower)
	{
		sprintf(sBuff, ("%u:%u:%s"), CurUserId, sendUserId, strText.c_str());
		CConvert convert;
		ConvertMsg(sBuff, sizeof(sBuff)-1);
		strhttpHeader = "Authorization: BASIC " + convert.enBase64(sBuff) + "\r\n";
		strhttpHeader += "Content-Type: text/html\r\n";

		sprintf(sBuff, "%s", strUpLink.c_str());
		ConvertMsg(sBuff, sizeof(sBuff)-1);
		nRetCode = HttpDownloadFile(sBuff, body, urlfile, strPath,strhttpHeader);

		if (nRetCode / 100 != 2)
		{
			g_WriteLog.WriteLog(C_LOG_ERROR, "SendFileToWebUserFunc-UploadFailed，errCode：%d", nRetCode);
			return -2;//上传失败
		}
	}
	else
	{
		return -1;//文件格式不对
	}

	int position = urlfile.find_last_of("\r");
	strUpLink = urlfile.substr(0, position);
	return 1;
}


//字符串替换 当前字符  将srcStr 替 换为 destStr
void StringReplace(string &curStr, string srcStr, string destStr)
{
	string::size_type pos = 0;
	string::size_type src = srcStr.size();
	string::size_type dest = destStr.size();
	while ((pos = curStr.find(srcStr, pos)) != string::npos)
	{
		curStr.replace(pos, src, destStr);
		pos += dest;
	}
}

void ReplaceFaceId(string& msg)
{
	string str = msg;
	string url;
	char getStr[32] = { 0 };

	while (1)
	{
		url = "<IMG alt=\"\" src=\"face.gif\">";
		int pos = str.find("[");


		if (pos == -1)//没有找到 插入的图片 
			break;


		int end = str.find("]", pos);
		if (end == -1)
			break;
		string idStr = str.substr(pos + 1, end - pos - 1);
		string reStr = str.substr(pos, end - pos + 1);

		string facePath = GetCurrentPath();
		facePath += ("\\SkinRes\\Face\\");
		facePath += idStr;

		StringReplace(url, "face.gif", facePath);
		StringReplace(str, reStr, url);
		StringReplace(str, "\\", "/");
	}

	msg = str;

}