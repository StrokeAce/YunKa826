#ifndef _ICHAT_MANAGER_H_
#define _ICHAT_MANAGER_H_

#include "chat_common/comobject.h"
#include "chat_common/comstruct.h"
#include "chat_common/comenum.h"

// 登录消息的回调接口
class IHandlerLgoin
{
public:
	// 登录的进度,percent=100时表示登录成功
	virtual void LoginProgress(int percent) = 0;
};

// 登录后通信消息的回调接口
class IHandlerMsgs
{
public:
	// 收到坐席列表
	virtual void RecvShareListCount(int len) = 0;

	// 收到一个坐席用户的信息,用来初始化坐席列表
	virtual void RecvUserInfo(CUserObject* pWebUser) = 0;

	// 收到一个会话消息
	virtual void RecvChatInfo(CWebUserObject* pWebUser, CUserObject* pUser = NULL) = 0;

	// 收到更新用户的在线状态
	virtual void RecvUserStatus(CUserObject* pUser) = 0;

	// 坐席上线消息
	virtual void RecvOnline(IBaseObject* pObj) = 0;

	// 坐席下线消息
	virtual void RecvOffline(IBaseObject* pObj, bool bShow = true) = 0;

	//************************************
	// Method:    RecvAcceptChat
	// Qualifier: 坐席接受会话的通知消息
	// Parameter: pUser 接受会话的坐席对象
	// Parameter: pWebUser 被接受的访客对象
	//************************************
	virtual void RecvAcceptChat(CWebUserObject* pWebUser, CUserObject* pUser) = 0;

	//************************************
	// Method:    RecvCloseChat
	// Qualifier: 会话关闭通知消息
	// Parameter: pWebUser 会话关闭相关的访客
	//************************************
	virtual void RecvCloseChat(CWebUserObject* pWebUser) = 0;

	//************************************
	// Method:    RecvReleaseChat
	// Qualifier: 释放会话
	// Parameter: pWebUser 被释放的访客
	//************************************
	virtual void RecvReleaseChat(CWebUserObject* pWebUser) = 0;

	//************************************
	// Method:    RecvMsg
	// Qualifier: 收到一条消息
	// Parameter: pObj 聊天的对象，其他坐席、web访客、微信访客
	// Parameter: msgFrom 消息的发送者类型，其他坐席、访客(微信或web)、协助对象(另一个坐席)
	// Parameter: msgId 消息的唯一id
	// Parameter: msgType 消息类型，预知消息(主要针对web访客)、普通消息
	// Parameter: msgDataType 消息的数据类型，文字(包括表情)、语音、图片、坐标、视频等
	// Parameter: msgContent 消息的具体内容
	// Parameter: msgTime 收到消息的时间
	// Parameter: pAssistUser 协助对象，当消息为协助对象发来时，需要该参数
	// Parameter: msgContentWx 微信消息，当非文字的微信消息时，需要该参数
	//************************************
	virtual void RecvMsg(IBaseObject* pObj, MSG_FROM_TYPE msgFrom, const char* msgId, MSG_TYPE msgType, MSG_DATA_TYPE msgDataType, const char* msgContent,
		const char* msgTime = NULL, CUserObject* pAssistUser = NULL, WxMsgBase* msgContentWx = NULL) = 0;

	//************************************
	// Method:    ResultRecvMsg
	// Qualifier: 接收一条语音或图片等媒体文件消息的结果
	// Parameter: msgId 消息id
	// Parameter: bSuccess 是否接收成功
	// Parameter: url 文件的下载url
	// Parameter: msgFromUserId 消息发送者的id
	// Parameter: assistUserId 协助对象的id
	// Parameter: filePath 文件本地保存路径，接收失败时为空
	// Parameter: msgFromType 消息发送者类型
	// Parameter: msgDataType 消息的数据类型
	//************************************
	virtual void ResultRecvMsg(const char* msgId, bool bSuccess, const char* url, unsigned long msgFromUserId,
		unsigned long assistUserId, const char* filePath, MSG_FROM_TYPE msgFromType, MSG_DATA_TYPE msgDataType) = 0;

	//************************************
	// Method:    ResultSendMsg
	// Qualifier: 发送一条消息的结果
	// Parameter: msgId 消息id
	// Parameter: bSuccess 是否发送成功
	// Parameter: userId 消息接收者的id
	// Parameter: userType 消息接收者的类型
	// Parameter: msgDataType 消息的数据类型
	// Parameter: msg 消息内容
	//************************************
	virtual void ResultSendMsg(const char* msgId, bool bSuccess = true, unsigned long userId = 0,
		MSG_RECV_TYPE userType = MSG_RECV_WX, MSG_DATA_TYPE msgDataType = MSG_DATA_TYPE_IMAGE, const char* msg = "") = 0;

	//************************************
	// Method:    ResultScreenCapture
	// Qualifier: 截图的结果
	// Parameter: imagePath 截图的保存位置，可能为空
	//************************************
	virtual void ResultScreenCapture(const char* imagePath) = 0;

	//************************************
	// Method:    RecvInviteUser
	// Qualifier: 收到邀请协助的请求
	// Parameter: pWebUser 邀请协助会话的聊天访客
	// Parameter: pUser 被邀请的协助者
	//************************************
	virtual void RecvInviteUser(CWebUserObject* pWebUser, unsigned long uid) = 0;

	//************************************
	// Method:    ResultInviteUser
	// Qualifier: 邀请协助的结果
	// Parameter: pWebUser 邀请协助会话的聊天访客
	// Parameter: pUser 被邀请的协助者
	// Parameter: status 邀请协助的状态
	//************************************
	virtual void ResultInviteUser(CWebUserObject* pWebUser, unsigned long uid, RESULT_STATUS status) = 0;

	//************************************
	// Method:    RecvTransferUser
	// Qualifier: 收到邀请转接的请求
	// Parameter: pWebUser 转接会话中的聊天访客
	// Parameter: pUser 被邀请的转接者
	//************************************
	virtual void RecvTransferUser(CWebUserObject* pWebUser) = 0;

	// 收到在线坐席信息列表
	virtual void RecvOnlineUsers(const char* pUsersXml) = 0;

	// 收到访客信息更新的事件
	virtual void RecvWebUserInfo(CWebUserObject* pWebUser, WEBUSER_INFO_NOTIFY_TYPE type) = 0;

	// 收到坐席在邀请中的消息
	virtual void RecvWebUserInInvite(CWebUserObject* pWebUser, CUserObject* pInviteUser) = 0;

	// 收到邀请邀请web访客会话的结果
	virtual void ResultInviteWebUser(CWebUserObject* pWebUser, bool bAgree) = 0;

	// 收到辅助应答的配置内容
	virtual void RecvQuickReply(const char* quickReply) = 0;

	virtual void PopTrayTips(const char* strPopTips, const char* strTitle = "") = 0;

	virtual void ShowMainWnd() = 0;
};

class IChatManager
{
public:
	// 设置接收登录消息的接口
	virtual void SetHandlerLogin(IHandlerLgoin* handlerLogin) = 0;

	// 设置接收登录后通信消息的接口
	virtual void SetHandlerMsgs(IHandlerMsgs* handlerMsgs) = 0;

	// 获取上一次登录信息,调用者不用关心析构
	virtual ListLoginedInfo GetPreLoginInfo() = 0;

	// 开始登录,此处返回值是true的话只表示当前登录操作成功，真正登录成功看回调函数的参数值
	virtual bool StartLogin(const char* loginName, const char* password, bool isAutoLogin, bool isKeepPwd) = 0;

	// 发送在线状态的消息
	virtual  int SendTo_UpdateOnlineStatus(unsigned short status) = 0;

	// 发送获取好友对象列表的消息
	virtual int SendTo_GetShareList() = 0;

	// 发送获取会话列表的消息
	virtual int SendTo_GetListChatInfo() = 0;

	// 发送获取所有用户的信息的消息
	virtual int SendTo_GetAllUserInfo() = 0;

	// 登录visitor服务器
	virtual int StartLoginVisitor() = 0;

	// 发送获取某个坐席信息的消息
	virtual int SendGetUserInfo(unsigned long uid) = 0;

	// 发送获取某个会话信息的消息
	virtual int SendGetWebUserChatInfo(unsigned short gpid, unsigned long adminid, char *chatid) = 0;

	//************************************
	// Method:    SendTo_Msg
	// Qualifier: 发送一条消息
	// Parameter: userId 接收消息的对象的id
	// Parameter: userType 接收对象的类型，坐席或访客
	// Parameter: msgId	消息id
	// Parameter: msgDataType 数据类型
	// Parameter: msg 数据内容
	//************************************
	virtual int SendTo_Msg(unsigned long userId, MSG_RECV_TYPE userType, const char* msgId, MSG_DATA_TYPE msgDataType, const char* msg) = 0;

	// 重新接收一条消息
	virtual int ReRecv_Msg(const char* url, MSG_FROM_TYPE msgFromUserType, const char* msgId, MSG_DATA_TYPE nMsgDataType,
		unsigned long msgFromUserId, unsigned long assistUserId, unsigned long time) = 0;

	// 发起接受访客会话
	virtual int SendTo_AcceptChat(unsigned long webuserid) = 0;

	// 发起释放访客会话
	virtual int SendTo_ReleaseChat(unsigned long webuserid) = 0;

	//************************************
	// Method:    SendTo_CloseChat
	// Qualifier: 发起关闭会话
	// Parameter: webuserid 会话中的访客的id
	// Parameter: ntype 会话关闭的原因，例如：CHATCLOSE_USER
	//************************************
	virtual void SendTo_CloseChat(unsigned long webuserid, int ntype) = 0;

	//************************************
	// Method:    SendTo_InviteWebUser
	// Qualifier: 发起邀请访客参与会话
	// Parameter: pWebUser 会话中的访客
	// Parameter: type 会话中的访客
	// Parameter: strText 会话中的访客
	//************************************
	virtual int SendTo_InviteWebUser(CWebUserObject *pWebUser, int type, const char* strText) = 0;

	//************************************
	// Method:    SendTo_InviteUser
	// Qualifier: 发起邀请其他坐席会话协助
	// Parameter: pWebUser 会话中的访客
	// Parameter: pUser 邀请的坐席
	//************************************
	virtual int SendTo_InviteUser(CWebUserObject* pWebUser, unsigned long uid) = 0;

	//************************************
	// Method:    SendTo_InviteUserResult
	// Qualifier: 发送是否接受该坐席的邀请协助
	// Parameter: pWebUser 会话中访客
	// Parameter: pUser 邀请的坐席
	// Parameter: bAccept 是否同意
	//************************************
	virtual int SendTo_InviteUserResult(CWebUserObject* pWebUser, unsigned long uid, bool bAccept) = 0;

	// 发起会话转接到其他坐席的请求
	virtual int SendTo_TransferRequestUser(CWebUserObject* pWebUser, unsigned long uid) = 0;

	//************************************
	// Method:    SendTo_InviteUserResult
	// Qualifier: 发送是否接受该坐席的会话转接
	// Parameter: pWebUser 会话中访客
	// Parameter: pUser 邀请的坐席
	// Parameter: bAccept 是否同意
	//************************************
	virtual int SendTo_TransferUserResult(CWebUserObject* pWebUser, bool bAccept) = 0;

	virtual int SendTo_GetOnlineUser() = 0;

	virtual void SendTo_GetQuickReply(unsigned long uin) = 0;

	// 获取上一次错误信息
	virtual const char* GetLastError() = 0;

	// 截图
	virtual void ScreenCapture(HWND hWnd) = 0;

	// 二次重启会话
	virtual void RestartSession(LPARAM lParam) = 0;

	// 退出，程序退出时调用
	virtual void Exit() = 0;

	virtual CSysConfigFile* GetSystemConfig() = 0;

	virtual CUserObject* GetSelfInfo() = 0;

	virtual const char* GetMsgId() = 0;

	virtual CUserObject *GetUserObjectByUid(unsigned long id) = 0;

	virtual CCustomList* GetEarlyChatList() = 0;

	virtual INIT_CONF* GetConfig() = 0;

	virtual const char* GetVip() = 0;

	virtual const char* GetAuthToken() = 0;

	virtual CWebUserObject *GetWebUserObjectBySid(char *sid) = 0;

	virtual CWebUserObject *GetWebUserObjectByUid(unsigned long uid) = 0;



};

class DLL_EXPORT CManagerFactory
{
private:
	CManagerFactory();
	
	static CManagerFactory* mpr_factory;
	IChatManager* mpr_manager;

public:
	~CManagerFactory();
	static CManagerFactory* GetInstance();

	IChatManager* GetManager();

};

#endif
