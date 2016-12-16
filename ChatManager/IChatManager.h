#ifndef _ICHAT_MANAGER_H_
#define _ICHAT_MANAGER_H_

#include "chat_common/comobject.h"
#include "chat_common/comstruct.h"
#include "chat_common/comenum.h"

// ��¼��Ϣ�Ļص��ӿ�
class IHandlerLgoin
{
public:
	// ��¼�Ľ���,percent=100ʱ��ʾ��¼�ɹ�
	virtual void LoginProgress(int percent) = 0;
};

// ��¼��ͨ����Ϣ�Ļص��ӿ�
class IHandlerMsgs
{
public:
	// �յ���ϯ�б�
	virtual void RecvShareListCount(int len) = 0;

	// �յ�һ����ϯ�û�����Ϣ,������ʼ����ϯ�б�
	virtual void RecvUserInfo(CUserObject* pWebUser) = 0;

	// �յ�һ���Ự��Ϣ
	virtual void RecvChatInfo(CWebUserObject* pWebUser, CUserObject* pUser = NULL) = 0;

	// �յ������û�������״̬
	virtual void RecvUserStatus(CUserObject* pUser) = 0;

	// ��ϯ������Ϣ
	virtual void RecvOnline(IBaseObject* pObj) = 0;

	// ��ϯ������Ϣ
	virtual void RecvOffline(IBaseObject* pObj, bool bShow = true) = 0;

	//************************************
	// Method:    RecvAcceptChat
	// Qualifier: ��ϯ���ܻỰ��֪ͨ��Ϣ
	// Parameter: pUser ���ܻỰ����ϯ����
	// Parameter: pWebUser �����ܵķÿͶ���
	//************************************
	virtual void RecvAcceptChat(CWebUserObject* pWebUser, CUserObject* pUser) = 0;

	//************************************
	// Method:    RecvCloseChat
	// Qualifier: �Ự�ر�֪ͨ��Ϣ
	// Parameter: pWebUser �Ự�ر���صķÿ�
	//************************************
	virtual void RecvCloseChat(CWebUserObject* pWebUser) = 0;

	//************************************
	// Method:    RecvReleaseChat
	// Qualifier: �ͷŻỰ
	// Parameter: pWebUser ���ͷŵķÿ�
	//************************************
	virtual void RecvReleaseChat(CWebUserObject* pWebUser) = 0;

	//************************************
	// Method:    RecvMsg
	// Qualifier: �յ�һ����Ϣ
	// Parameter: pObj ����Ķ���������ϯ��web�ÿ͡�΢�ŷÿ�
	// Parameter: msgFrom ��Ϣ�ķ��������ͣ�������ϯ���ÿ�(΢�Ż�web)��Э������(��һ����ϯ)
	// Parameter: msgId ��Ϣ��Ψһid
	// Parameter: msgType ��Ϣ���ͣ�Ԥ֪��Ϣ(��Ҫ���web�ÿ�)����ͨ��Ϣ
	// Parameter: msgDataType ��Ϣ���������ͣ�����(��������)��������ͼƬ�����ꡢ��Ƶ��
	// Parameter: msgContent ��Ϣ�ľ�������
	// Parameter: msgTime �յ���Ϣ��ʱ��
	// Parameter: pAssistUser Э�����󣬵���ϢΪЭ��������ʱ����Ҫ�ò���
	// Parameter: msgContentWx ΢����Ϣ���������ֵ�΢����Ϣʱ����Ҫ�ò���
	//************************************
	virtual void RecvMsg(IBaseObject* pObj, MSG_FROM_TYPE msgFrom, const char* msgId, MSG_TYPE msgType, MSG_DATA_TYPE msgDataType, const char* msgContent,
		const char* msgTime = NULL, CUserObject* pAssistUser = NULL, WxMsgBase* msgContentWx = NULL) = 0;

	//************************************
	// Method:    ResultRecvMsg
	// Qualifier: ����һ��������ͼƬ��ý���ļ���Ϣ�Ľ��
	// Parameter: msgId ��Ϣid
	// Parameter: bSuccess �Ƿ���ճɹ�
	// Parameter: url �ļ�������url
	// Parameter: msgFromUserId ��Ϣ�����ߵ�id
	// Parameter: assistUserId Э�������id
	// Parameter: filePath �ļ����ر���·��������ʧ��ʱΪ��
	// Parameter: msgFromType ��Ϣ����������
	// Parameter: msgDataType ��Ϣ����������
	//************************************
	virtual void ResultRecvMsg(const char* msgId, bool bSuccess, const char* url, unsigned long msgFromUserId,
		unsigned long assistUserId, const char* filePath, MSG_FROM_TYPE msgFromType, MSG_DATA_TYPE msgDataType) = 0;

	//************************************
	// Method:    ResultSendMsg
	// Qualifier: ����һ����Ϣ�Ľ��
	// Parameter: msgId ��Ϣid
	// Parameter: bSuccess �Ƿ��ͳɹ�
	// Parameter: userId ��Ϣ�����ߵ�id
	// Parameter: userType ��Ϣ�����ߵ�����
	// Parameter: msgDataType ��Ϣ����������
	// Parameter: msg ��Ϣ����
	//************************************
	virtual void ResultSendMsg(const char* msgId, bool bSuccess = true, unsigned long userId = 0,
		MSG_RECV_TYPE userType = MSG_RECV_WX, MSG_DATA_TYPE msgDataType = MSG_DATA_TYPE_IMAGE, const char* msg = "") = 0;

	//************************************
	// Method:    ResultScreenCapture
	// Qualifier: ��ͼ�Ľ��
	// Parameter: imagePath ��ͼ�ı���λ�ã�����Ϊ��
	//************************************
	virtual void ResultScreenCapture(const char* imagePath) = 0;

	//************************************
	// Method:    RecvInviteUser
	// Qualifier: �յ�����Э��������
	// Parameter: pWebUser ����Э���Ự������ÿ�
	// Parameter: pUser �������Э����
	//************************************
	virtual void RecvInviteUser(CWebUserObject* pWebUser, unsigned long uid) = 0;

	//************************************
	// Method:    ResultInviteUser
	// Qualifier: ����Э���Ľ��
	// Parameter: pWebUser ����Э���Ự������ÿ�
	// Parameter: pUser �������Э����
	// Parameter: status ����Э����״̬
	//************************************
	virtual void ResultInviteUser(CWebUserObject* pWebUser, unsigned long uid, RESULT_STATUS status) = 0;

	//************************************
	// Method:    RecvTransferUser
	// Qualifier: �յ�����ת�ӵ�����
	// Parameter: pWebUser ת�ӻỰ�е�����ÿ�
	// Parameter: pUser �������ת����
	//************************************
	virtual void RecvTransferUser(CWebUserObject* pWebUser) = 0;

	// �յ�������ϯ��Ϣ�б�
	virtual void RecvOnlineUsers(const char* pUsersXml) = 0;

	// �յ��ÿ���Ϣ���µ��¼�
	virtual void RecvWebUserInfo(CWebUserObject* pWebUser, WEBUSER_INFO_NOTIFY_TYPE type) = 0;

	// �յ���ϯ�������е���Ϣ
	virtual void RecvWebUserInInvite(CWebUserObject* pWebUser, CUserObject* pInviteUser) = 0;

	// �յ���������web�ÿͻỰ�Ľ��
	virtual void ResultInviteWebUser(CWebUserObject* pWebUser, bool bAgree) = 0;

	// �յ�����Ӧ�����������
	virtual void RecvQuickReply(const char* quickReply) = 0;

	virtual void PopTrayTips(const char* strPopTips, const char* strTitle = "") = 0;

	virtual void ShowMainWnd() = 0;
};

class IChatManager
{
public:
	// ���ý��յ�¼��Ϣ�Ľӿ�
	virtual void SetHandlerLogin(IHandlerLgoin* handlerLogin) = 0;

	// ���ý��յ�¼��ͨ����Ϣ�Ľӿ�
	virtual void SetHandlerMsgs(IHandlerMsgs* handlerMsgs) = 0;

	// ��ȡ��һ�ε�¼��Ϣ,�����߲��ù�������
	virtual ListLoginedInfo GetPreLoginInfo() = 0;

	// ��ʼ��¼,�˴�����ֵ��true�Ļ�ֻ��ʾ��ǰ��¼�����ɹ���������¼�ɹ����ص������Ĳ���ֵ
	virtual bool StartLogin(const char* loginName, const char* password, bool isAutoLogin, bool isKeepPwd) = 0;

	// ��������״̬����Ϣ
	virtual  int SendTo_UpdateOnlineStatus(unsigned short status) = 0;

	// ���ͻ�ȡ���Ѷ����б����Ϣ
	virtual int SendTo_GetShareList() = 0;

	// ���ͻ�ȡ�Ự�б����Ϣ
	virtual int SendTo_GetListChatInfo() = 0;

	// ���ͻ�ȡ�����û�����Ϣ����Ϣ
	virtual int SendTo_GetAllUserInfo() = 0;

	// ��¼visitor������
	virtual int StartLoginVisitor() = 0;

	// ���ͻ�ȡĳ����ϯ��Ϣ����Ϣ
	virtual int SendGetUserInfo(unsigned long uid) = 0;

	// ���ͻ�ȡĳ���Ự��Ϣ����Ϣ
	virtual int SendGetWebUserChatInfo(unsigned short gpid, unsigned long adminid, char *chatid) = 0;

	//************************************
	// Method:    SendTo_Msg
	// Qualifier: ����һ����Ϣ
	// Parameter: userId ������Ϣ�Ķ����id
	// Parameter: userType ���ն�������ͣ���ϯ��ÿ�
	// Parameter: msgId	��Ϣid
	// Parameter: msgDataType ��������
	// Parameter: msg ��������
	//************************************
	virtual int SendTo_Msg(unsigned long userId, MSG_RECV_TYPE userType, const char* msgId, MSG_DATA_TYPE msgDataType, const char* msg) = 0;

	// ���½���һ����Ϣ
	virtual int ReRecv_Msg(const char* url, MSG_FROM_TYPE msgFromUserType, const char* msgId, MSG_DATA_TYPE nMsgDataType,
		unsigned long msgFromUserId, unsigned long assistUserId, unsigned long time) = 0;

	// ������ܷÿͻỰ
	virtual int SendTo_AcceptChat(unsigned long webuserid) = 0;

	// �����ͷŷÿͻỰ
	virtual int SendTo_ReleaseChat(unsigned long webuserid) = 0;

	//************************************
	// Method:    SendTo_CloseChat
	// Qualifier: ����رջỰ
	// Parameter: webuserid �Ự�еķÿ͵�id
	// Parameter: ntype �Ự�رյ�ԭ�����磺CHATCLOSE_USER
	//************************************
	virtual void SendTo_CloseChat(unsigned long webuserid, int ntype) = 0;

	//************************************
	// Method:    SendTo_InviteWebUser
	// Qualifier: ��������ÿͲ���Ự
	// Parameter: pWebUser �Ự�еķÿ�
	// Parameter: type �Ự�еķÿ�
	// Parameter: strText �Ự�еķÿ�
	//************************************
	virtual int SendTo_InviteWebUser(CWebUserObject *pWebUser, int type, const char* strText) = 0;

	//************************************
	// Method:    SendTo_InviteUser
	// Qualifier: ��������������ϯ�ỰЭ��
	// Parameter: pWebUser �Ự�еķÿ�
	// Parameter: pUser �������ϯ
	//************************************
	virtual int SendTo_InviteUser(CWebUserObject* pWebUser, unsigned long uid) = 0;

	//************************************
	// Method:    SendTo_InviteUserResult
	// Qualifier: �����Ƿ���ܸ���ϯ������Э��
	// Parameter: pWebUser �Ự�зÿ�
	// Parameter: pUser �������ϯ
	// Parameter: bAccept �Ƿ�ͬ��
	//************************************
	virtual int SendTo_InviteUserResult(CWebUserObject* pWebUser, unsigned long uid, bool bAccept) = 0;

	// ����Ựת�ӵ�������ϯ������
	virtual int SendTo_TransferRequestUser(CWebUserObject* pWebUser, unsigned long uid) = 0;

	//************************************
	// Method:    SendTo_InviteUserResult
	// Qualifier: �����Ƿ���ܸ���ϯ�ĻỰת��
	// Parameter: pWebUser �Ự�зÿ�
	// Parameter: pUser �������ϯ
	// Parameter: bAccept �Ƿ�ͬ��
	//************************************
	virtual int SendTo_TransferUserResult(CWebUserObject* pWebUser, bool bAccept) = 0;

	virtual int SendTo_GetOnlineUser() = 0;

	virtual void SendTo_GetQuickReply(unsigned long uin) = 0;

	// ��ȡ��һ�δ�����Ϣ
	virtual const char* GetLastError() = 0;

	// ��ͼ
	virtual void ScreenCapture(HWND hWnd) = 0;

	// ���������Ự
	virtual void RestartSession(LPARAM lParam) = 0;

	// �˳��������˳�ʱ����
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
