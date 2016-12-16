#pragma once

#include "login.h"
#include "IChatManager.h"
#include "socket.h"

typedef map<string/*thirdId*/, string/*公众号token*/> MapWxTokens; // 公众号的thirdid和token一一对应
typedef map<unsigned long, CUserObject*> MapUsers; // 保存坐席用户
typedef map<string, CWebUserObject*> MapWebUsers; // 保存访客

class CChatVisitor;

class CChatManager : public IBaseReceive , public IChatManager
{
public:
	CChatManager();
	~CChatManager();

	// 设置接收登录消息的接口
	void SetHandlerLogin(IHandlerLgoin* handlerLogin);

	// 设置接收登录后通信消息的接口
	void SetHandlerMsgs(IHandlerMsgs* handlerMsgs);

public:

	// 获取上一次登录信息,调用者不用关心析构
	ListLoginedInfo GetPreLoginInfo();

	// 开始登录
	bool StartLogin(const char* loginName, const char* password, bool isAutoLogin, bool isKeepPwd);

	// 发送在线状态的消息
	int SendTo_UpdateOnlineStatus(unsigned short status);

	// 发送获取好友对象列表的消息
	int SendTo_GetShareList();

	// 发送获取会话列表的消息
	int SendTo_GetListChatInfo();

	// 发送获取所有用户的信息的消息
	int SendTo_GetAllUserInfo();

	int StartLoginVisitor();

	// 发送获取某个坐席信息的消息
	int SendGetUserInfo(unsigned long uid);

	// 发送获取某个会话信息的消息
	int SendGetWebUserChatInfo(unsigned short gpid, unsigned long adminid, char *chatid);

	//************************************
	// Method:    SendTo_Msg
	// Qualifier: 发送一条消息
	// Parameter: userId 接收消息的对象的id
	// Parameter: userType 接收对象的类型，坐席或访客
	// Parameter: msgId	消息id
	// Parameter: msgDataType 数据类型
	// Parameter: msg 数据内容
	//************************************
	int SendTo_Msg(unsigned long userId, MSG_RECV_TYPE userType, const char* msgId, MSG_DATA_TYPE msgDataType, const char* msg);

	// 重新接收一条消息
	int ReRecv_Msg(const char* url, MSG_FROM_TYPE msgFromUserType, const char* msgId, MSG_DATA_TYPE nMsgDataType,
		unsigned long msgFromUserId, unsigned long assistUserId, unsigned long time);

	// 发起接受访客会话
	int SendTo_AcceptChat(unsigned long webuserid);

	// 发起释放访客会话
	int SendTo_ReleaseChat(unsigned long webuserid);

	//************************************
	// Method:    SendTo_CloseChat
	// Qualifier: 发起关闭会话
	// Parameter: webuserid 会话中的访客的id
	// Parameter: ntype 会话关闭的原因，例如：CHATCLOSE_USER
	//************************************
	void SendTo_CloseChat(unsigned long webuserid, int ntype);

	//************************************
	// Method:    SendTo_InviteWebUser
	// Qualifier: 发起邀请访客参与会话
	// Parameter: pWebUser 会话中的访客
	// Parameter: type 会话中的访客
	// Parameter: strText 会话中的访客
	//************************************
	int SendTo_InviteWebUser(CWebUserObject *pWebUser, int type, const char* strText);

	//************************************
	// Method:    SendTo_InviteUser
	// Qualifier: 发起邀请其他坐席会话协助
	// Parameter: pWebUser 会话中的访客
	// Parameter: pUser 邀请的坐席
	//************************************
	int SendTo_InviteUser(CWebUserObject* pWebUser, unsigned long uid);

	//************************************
	// Method:    SendTo_InviteUserResult
	// Qualifier: 发送是否接受该坐席的邀请协助
	// Parameter: pWebUser 会话中访客
	// Parameter: pUser 邀请的坐席
	// Parameter: bAccept 是否同意
	//************************************
	int SendTo_InviteUserResult(CWebUserObject* pWebUser, unsigned long uid, bool bAccept);

	// 发起会话转接到其他坐席的请求
	int SendTo_TransferRequestUser(CWebUserObject* pWebUser, unsigned long uid);

	//************************************
	// Method:    SendTo_InviteUserResult
	// Qualifier: 发送是否接受该坐席的会话转接
	// Parameter: pWebUser 会话中访客
	// Parameter: pUser 邀请的坐席
	// Parameter: bAccept 是否同意
	//************************************
	int SendTo_TransferUserResult(CWebUserObject* pWebUser, bool bAccept);

	int SendTo_GetOnlineUser();

	void SendTo_GetQuickReply(unsigned long uin);

	// 发送获取某个访客信息的消息
	int SendToGetWorkBill(unsigned long webuserid, const char *chatid, char *szMsg = "", unsigned int chatkefuid = 0);

	int SendToTransferUser(CUserObject *pAcceptUser, CWebUserObject *pWebUser, unsigned long acceptuin = 0);

	int SendToRefuseChat(CWebUserObject *pWebUser, string strReason = "NO");

	// 获取上一次错误信息
	const char* GetLastError();

	// 截图
	void ScreenCapture(HWND hWnd);

	// 二次重启会话
	void RestartSession(LPARAM lParam);

	void FormatRequestUrl(string &strUrl,string strMsg);

	bool CChatManager::RepickChatCon(string url, string& strRet, unsigned long &uin, string &strErrMsg);
	
	// 退出，程序退出时调用
	void Exit();

	CSysConfigFile* GetSystemConfig();

	CUserObject* GetSelfInfo();

	CCustomList* GetEarlyChatList();

	INIT_CONF* GetConfig();

	const char* GetVip();

	const char* GetAuthToken();

private:
	friend class CLogin;
	friend class CMySocket;

	/***************     继承接口的函数实现    *****************/

	virtual void OnReceive(void* wParam, void* lParam);

	virtual void OnReceiveEvent(int wParam, int lParam);


	/***************     配置文件操作函数     *****************/

	bool ReadSystemConfig();

	void SetSystemConfigByInitconfig();

	void SolveUserdefineFilter(FILTER_USERDEFINE &filter, char *sfilter);

	bool LoadINIResource();


	/***************     消息包处理函数      *****************/

	int RecvSrvConfLogon(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvRepUserinfo(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvDenyLogon(PACK_HEADER packhead, char *pRecvBuff, int len, int &errtype);

	int RecvSrvConfLogOff(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvStatusFrdOnline(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvStatusFrdOffline(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvStatusUserForm(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatShareList(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvComSendMsg(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatCreateChat(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatChatMsg(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatChatMsgAck(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatAcceptChat(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatTransQuest(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatTransFailed(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvInviteRequest(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvInviteResult(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatRelease(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatCMDError(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatCloseChat(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatListChat(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvFloatChatInfo(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvEventAnnouncement(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvUpdateSucc(PACK_HEADER packhead, char *pRecvBuff, int len);

	int RecvSrvUpdateFail(PACK_HEADER packhead, char *pRecvBuff, int len);

	//用户在异地登陆
	int RecvSrvDown(PACK_HEADER packhead, char *pRecvBuff, int len);

	//转移临时用户成功失败
	int RecvRepTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len);

	//接收转移会话的用户处理
	int RecvTransferClient(PACK_HEADER packhead, char *pRecvBuff, int len);

	void RecvComSendNormalChatidHisMsg(unsigned long senduid, unsigned long recvuid, COM_SEND_MSG& RecvInfo);

	int RecvComTransRequest(unsigned long senduid, COM_SEND_MSG& RecvInfo);

	int RecvComTransAnswer(unsigned long senduid, COM_SEND_MSG& RecvInfo);

	void RecvComSendWorkBillMsg(unsigned long senduid, unsigned long recvuid, char *msg, char* mobile);


	/***************     坐席和访客信息管理函数      *****************/

	int UnPack(CPackInfo *pPackInfo, char *buff, int len);

	int SendPackTo(CPackInfo *pPackInfo, unsigned long recvuid = 0, unsigned long recvsock = 0, HWND hWnd = NULL);

	void SetLoginProgress(int percent);

public:
	CUserObject* GetMySelfUserInfo();

	CUserObject* AddUserObject(unsigned long id, char *sid, char *name, unsigned char status, unsigned long fd);

	CUserObject* GetUserObjectByUid(unsigned long id);

	CWebUserObject *GetWebUserObjectBySid(char *sid);

	CWebUserObject *GetWebUserObjectByUid(unsigned long uid);

	CWebUserObject *AddWebUserObject(char *sid, char *thirdid, char *name, char *scriptflag, char *url,
		USER_STATUS status, unsigned char floatauth);


	unsigned short GetPackSeq();	

	// 定时发送心跳包保活
	void TimerSolveAck();

	// 定时连接登录visit服务器
	void TimerSolveLoginToVisitorServer();

	// 定时处理请求的超时情况
	void TimerSolveRequestTimerOut();

	int SendPing();

	void SetOfflineStatus();

	void CloseAllSocket();

	void SetAllUserOffline();

	void DeleteAllSrvInfo();

	void DeleteAllUserInfo();

	void DeleteAllWebUserInfo();

	/***************     类内使用的发送到服务端的消息      *****************/

	int SendGetSelfInfo(unsigned long id, char *strid, unsigned short cmd, unsigned short cmdtype = 0, unsigned short type = 0);

	int SendAckEx(unsigned short seq, unsigned long uid = 0, unsigned long ip = 0, unsigned short port = 0);

	int SendAutoRespMsg(CWebUserObject *pWebUser, const char *msg, BOOL bClearTimer = true);

	//开始接受会话消息
	int SendStartRecvFloatMsg(unsigned short gpid, unsigned long adminid, char *chatid, unsigned short sLastMsgid);

	// 获取userinfo和token
	void SendGetWxUserInfoAndToken(CWebUserObject* pWebUser);

	// 获取微信用户信息
	int SendGetWxUserInfo(unsigned long webuserid, const char *chatid);

	// 获取微信公众号token
	int SendGetWxToken(unsigned long webuserid, const char *chatid);

	//************************************
	// Method:    SendMsg
	// Qualifier: 发送消息包
	// Parameter: pUser 接收消息的对象
	// Parameter: msg 消息具体内容
	// Parameter: bak 备份内容
	// Parameter: sfont 对微信用户的媒体消息用"JSON=WX"，其他的都用"HTML"
	//************************************
	int SendMsg(IBaseObject* pUser, const char *msg, int bak = 0, char *sfont = "HTML");

	// 发送老规则的消息
	int SendComMsg(unsigned long recvuid, char *visitorid, const char *msg, char * chatid, char* thirdid, int bak = 0, char *sfontinfo = "");

	// 发送新规则的消息，主要针对微信用户
	int SendFloatMsg(CWebUserObject *pWebUser, const char *msg, char *sfont);

	int SendGetChatHisMsg(unsigned long webuserid, const char *chatid);//获取非等待应答会话的会话历史消息

	//************************************
	// Method:    SendCloseChat
	// Qualifier: 发起关闭会话
	// Parameter: pWebUser 会话中的访客
	// Parameter: ntype 会话关闭的原因，例如：CHATCLOSE_USER
	//************************************
	int SendCloseChat(CWebUserObject *pWebUser, int ntype);

	int SendLoginOff();

	void ClearDirectory(string dir);

	// 微信消息的解析
	WxMsgBase* ParseWxMsg(CWebUserObject* pWebUser, char* msg, CUserObject* pAssistUser, unsigned long time);

	bool ParseTextMsg(CWebUserObject* pWebUser, string content, CUserObject* pAssistUser, unsigned long time);

	const char* GetMsgId();

	string GetFileId();

	void SaveEarlyMsg(MSG_INFO *pMsgInfo);

	void SolveWebUserEarlyMsg(CWebUserObject *pWebUser);

	CWebUserObject *ChangeWebUserSid(CWebUserObject *pWebUser, char *sid, char *thirdid);

	void GetInviteChatSysMsg(char* msg, CUserObject *pInviteUser, CWebUserObject *pWebUser, int type, CUserObject *pAcceptUser = NULL);

	void GetStopChatSysMsg(char* msg, CWebUserObject *pWebUser, int type = 0, CUserObject *pSendUser = NULL);

	// 定时器事件处理函数
	static void CALLBACK TimerProc(int timeId, LPVOID pThis); 

	void LoginSuccess();

	CWebUserObject *GetWebUserObjectByScriptFlag(char *scriptflag);	

	void UpLoadFile(unsigned long userId, MSG_RECV_TYPE userType, string msgId, string filePath,
					MSG_DATA_TYPE = MSG_DATA_TYPE_IMAGE);

	void DownLoadFile(IBaseObject* pUser, MSG_DATA_TYPE nMsgDataType, string url, CUserObject *pAssistUser, unsigned long time,string msgId);

	void AfterUpload(unsigned long userId, MSG_RECV_TYPE userType, string msgId, string mediaID = "",
					MSG_DATA_TYPE = MSG_DATA_TYPE_IMAGE, string fileId = "", string filePath = "",
					string wxToken = "");

	// 从表情转换到字符串
	void TransferFaceToStr(string& msg, MSG_RECV_TYPE recvType);

	// 从字符串替换到表情
	void TransferStrToFace(string& msg);

	// 从字符串替换到老版的表情
	void TransferStrToOldFace(string& msg);

	// 替换表情到服务器地址字符串，用于发送给web用户
	void TransferFaceToServeStr(string& msg);

	int GetFaceIndex(const char * faceStr);

	string GetFaceStr(int id, MSG_RECV_TYPE recvType);

	void Amr2Wav(string filePath);

	//************************************
	// Method:    AddMsgToList
	// Qualifier: 添加一条消息
	// Parameter: pObj 消息所属的用户
	// Parameter: msgFrom 消息发送者的用户类型
	// Parameter: recvType 消息接收者的用户类型
	// Parameter: msgId 消息id
	// Parameter: msgType 消息类型
	// Parameter: msgDataType 消息数据类型
	// Parameter: msgContent 消息内容
	// Parameter: msgTime 消息时间
	// Parameter: pAssistUser 协助对象
	// Parameter: msgContentWx 微信消息体
	// Parameter: bSave	消息是否保存
	// Parameter: bNotify 消息是否通知界面
	// Parameter: bAgain 消息内容是否需要再次处理
	//************************************
	void AddMsgToList(IBaseObject* pObj, MSG_FROM_TYPE msgFrom, MSG_RECV_TYPE recvType, string msgId, 
					MSG_TYPE msgType, MSG_DATA_TYPE msgDataType, string msgContent="",
					unsigned long msgTime = 0, CUserObject* pAssistUser = NULL, 
					WxMsgBase* msgContentWx = NULL,bool bSave = true,
					bool bNotify = true,bool bAgain = true);

	string ReplaceToken(string srcStr, string replaceStr);

	void AddToken(WxUserInfo* userInfo,string token);

	static DWORD WINAPI GetVisitorInfoThread(void *arg);

	void GetVisitorInfo();

	static DWORD WINAPI GetOnlineUserThread(void *arg);

	void GetOnlineUser();

	bool TokenIsDifferent(string oldToken, string newToken);

	static UINT WINAPI UpLoadFileToWxServerThread(void * pUpLoadInfo);

	static UINT WINAPI UpLoadFileToServerThread(void * pUpLoadInfo);

	static UINT WINAPI DownLoadFileFromServerThread(void * para);

	static DWORD WINAPI SendFileThread(void *arg);

	static DWORD WINAPI GetQuickReplyThread(void *arg);

	bool SendFileToUser(IBaseObject* pUser, string strPathFile, string msgId, MSG_RECV_TYPE userType);

	void SolveAlertInfo(ALERT_TYPE type, string strPopTips);

	void ReplaceHeadImage(CWebUserObject* pWebUser);

public:
	int						m_nOnLineStatus;		// 是否在线,对于im服务器而言
	int						m_nOnLineStatusEx;		// 是否在线,对于visit服务器而言
	bool					m_bExit;				// 程序是否退出
	CUserObject				m_userInfo;				// 登录用户的信息
	CSysConfigFile*			m_sysConfig;			// 用户设置文件类
	INIT_CONF				m_initConfig;			// 软件配置文件类
	CLogin*					m_login;				// 登录处理类	
	IHandlerLgoin*			m_handlerLogin;			// 登录消息接收接口
	IHandlerMsgs*			m_handlerMsgs;			// 通信消息接收接口	
	CMySocket				m_socket;				// 消息连接
	CChatVisitor*			m_vistor;				// 访客接待处理对象
	string					m_sLogin;				// 登录名
	string					m_password;				// 登录密码
	string					m_server;				// 服务器ip地址
	string					m_vip;					// 服务器java系统访问地址
	int						m_port;					// 服务器端口
	string					m_lastError;			// 上一次错误信息
	bool					m_bAutoLogin;			// 是否自动登录
	bool					m_bKeepPwd;				// 是否记住密码
	bool					m_bLoginSuccess;		// 是否已登录			
	unsigned short			m_usSrvRand;			// 服务器的随机数
	unsigned short			m_usCltRand;			// 本次运行的随机数
	MapUsers				m_mapUsers;				// 协助对象的存储集合
	MapWebUsers				m_mapWebUsers;			// 访客的存储集合
	CUserObject				m_commUserInfo;			// 公用用户
	CTimerManager*			m_timers;				// 定时器管理类
	int						m_nMyInfoIsGet;			// 是否我的信息已经获取到了
	int						m_nSendPing;			// 心跳包发送的次数
	int						m_nSendPingFail;		// 心跳包发送失败次数 
	int						m_nLoginToVisitor;		// 尝试登录visit服务器的次数	 
	unsigned long			m_nNextInviteWebuserUid;// 邀请的访客信息
	unsigned long			m_nNextInviteUid;		// 主动邀请的用户
	MapWxTokens				m_mapTokens;			// 公众号token存储集合
	MMutex					m_idLock;				// 生成消息id的锁
	int						m_nMsgId;				// 消息id，自增
	char					m_sMsgId[MAX_64_LEN];	// 消息id，自增
	unsigned long			m_fileId;				// 文件id，自增
	list<MSG_INFO*>			m_listEarlyMsg;			// 保存还未初始化访客对象之前收到的消息
	int						m_nClientIndex;			// 访客的序列号，自增
	HMODULE					m_hScreenDll;			// 截图句柄
	HANDLE					m_hGetOnlineUserThread;	// 获取在线坐席的线程
	CGroupObject			m_groupUser;			// 获取的在线坐席的分组集合
	HANDLE					m_sendFileThreadHandle;	// 发送文件线程句柄
	unsigned short			m_packSeq;				// 包序列号,自增
	HANDLE					m_hQuickReplyHandle;	// 辅助应答线程句柄
	bool					m_bFrameInit;			// 界面是否初始化完成
	CCustomList*			m_listEarlyChat;		// 保存初始化完成之前收到的会话
};

