#ifndef _MAIN_FRAME_H_
#define _MAIN_FRAME_H

#pragma once
#include <map>
#include "small_menu.h"
#include "IChatManager.h"
#include "cef_browser/client_handler.h"
#include <log/winlog.h>
#include "rich_edit_util.h"
#include "IImageOle.h"
#include "face_list.h"
#include "face_sel_dlg.h"
#include "user_list.h"
#include "show_big_image_dlg.h"
#include "system_settings.h"
#include "operation_tips.h"
#include "ui_common/markup.h"
#include "commctrl.h"
#include "WndShadow.h"


#pragma comment(lib,"comctl32.lib")

#define MID_MANAGER_BUTTON_NUM    8
#define MAX_PATH_LENGTH           512

#define MAX_SHOW_LIST_LENGH                 56



typedef struct SHORT_ANSWER_DATA
{
	char title[256];
	char value[10240];

}SHORT_ANSWER_DATA;



typedef struct SHORT_ANSWER_STRUCT
{

	char key[256];
	vector<SHORT_ANSWER_DATA> m_value;

}SHORT_ANSWER_STRUCT;






typedef struct CONTROL_ATTR
{
	int centerFrameWitdh;
	int showMsgWidth;

}CONTROL_ATTR;


// 将HWND显示到CControlUI上面
class CWndUI : public CControlUI
{
public:
	CWndUI() : m_hWnd(NULL){}

	virtual void SetVisible(bool bVisible = true)
	{
		__super::SetVisible(bVisible);
		::ShowWindow(m_hWnd, bVisible);
		//SetBkImage(_T("E:\\WeiBo_3\\client\\cppprj\\common\\media\\bin\\debug\\SkinRes\\videobg.bmp"));
	}

	virtual void SetInternVisible(bool bVisible = true)
	{
		__super::SetInternVisible(bVisible);
		::ShowWindow(m_hWnd, bVisible);
	}

	void SetPos(RECT rc)
	{
		__super::SetPos(rc);
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL Attach(HWND hWndNew)
	{
		if (!::IsWindow(hWndNew))
		{
			return FALSE;
		}

		m_hWnd = hWndNew;
		return TRUE;
	}

	HWND Detach()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

	HWND GetHWND()
	{
		return m_hWnd;
	}

public:
	HWND m_hWnd;
};


typedef struct ManagerButtonStruct
{
	CButtonUI *m_pManagerBtn;
	int   m_buttonState;
	WCHAR hotImage[MAX_PATH_LENGTH];
	WCHAR pushedImage[MAX_PATH_LENGTH];
	WCHAR normalImage[MAX_PATH_LENGTH];

}ManagerButtonStruct;


class CMainFrame : public WindowImplBase, public IHandlerMsgs
{
public:

	CMainFrame(IChatManager * manager);
	~CMainFrame();

public:

	LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);



	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnWndTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	DWORD GetBkColor();
	void SetBkColor(DWORD dwBackColor);

	//virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged(TNotifyUI &msg);
	virtual void OnItemClick(TNotifyUI &msg);
	virtual void OnItemRbClick(TNotifyUI &msg);
	virtual void OnTimer(TNotifyUI &msg);
	virtual void OnHeaderClick(TNotifyUI& msg);
	virtual void OnMouseEnter(TNotifyUI& msg);
	virtual void OnItemSelect(TNotifyUI &msg);


	void OnItemActive(TNotifyUI &msg);

public:    //主界面消息回调
	// 收到坐席列表
	virtual void RecvShareListCount(int len);
	
	// 收到一个坐席用户的信息,用来初始化坐席列表
	virtual void RecvUserInfo(CUserObject* pWebUser);
	
	// 收到一个会话消息
	virtual void RecvChatInfo(CWebUserObject* pWebUser, CUserObject* pUser);
	
	// 收到更新用户的在线状态
	virtual void RecvUserStatus(CUserObject* pUser);
	
	// 坐席上线消息
	virtual void RecvOnline(IBaseObject* pObj);
	
	// 坐席下线消息
	virtual void RecvOffline(IBaseObject* pObj, bool bShow = true);
	
	virtual void RecvAcceptChat(CWebUserObject* pWebUser, CUserObject* pUser);
	
	virtual void RecvCloseChat(CWebUserObject* pWebUser);
	
	virtual void RecvReleaseChat(CWebUserObject* pWebUser);
	
	virtual void RecvMsg(IBaseObject* pObj, MSG_FROM_TYPE msgFrom, const char* msgId, MSG_TYPE msgType, MSG_DATA_TYPE msgDataType, const char* msgContent,
		const char* msgTime, CUserObject* pAssistUser, WxMsgBase* msgContentWx);
	
	virtual void ResultRecvMsg(const char* msgId, bool bSuccess, const char* url, unsigned long msgFromUserId,
		unsigned long assistUserId, const char* filePath, MSG_FROM_TYPE msgFromType, MSG_DATA_TYPE msgDataType);
	
	virtual void ResultSendMsg(const char* msgId, bool bSuccess = true, unsigned long userId = 0, MSG_RECV_TYPE recvUserType = MSG_RECV_WX,
		MSG_DATA_TYPE msgDataType = MSG_DATA_TYPE_IMAGE, const char* msg = "");
	
	virtual void ResultScreenCapture(const char* imagePath);
	
	virtual void RecvInviteUser(CWebUserObject* pWebUser, unsigned long uid);
	
	virtual void ResultInviteUser(CWebUserObject* pWebUser, unsigned long uid, RESULT_STATUS status);
	
	virtual void RecvTransferUser(CWebUserObject* pWebUser);
	
	virtual void ResultTransferUser(CWebUserObject* pWebUser, CUserObject* pUser, RESULT_STATUS status);
	
	virtual void RecvOnlineUsers(const char* pUsersXml);
	
	virtual void RecvWebUserInfo(CWebUserObject* pWebUser, WEBUSER_INFO_NOTIFY_TYPE type);
	
	virtual void RecvWebUserInInvite(CWebUserObject* pWebUser, CUserObject* pInviteUser);
	
	virtual void ResultInviteWebUser(CWebUserObject* pWebUser, bool bAgree);
	
	virtual void RecvQuickReply(const char* quickReply);
	
	virtual void PopTrayTips(const char* strPopTips, const char* strTitle = "");

	virtual void ShowMainWnd();
	virtual void OnCreateShadow(HWND hwnd);


public:
	//自己定义的操作函数
	void OnBtnFont(TNotifyUI& msg);
	void OnBtnFace(TNotifyUI& msg);
	void OnBtnScreen(TNotifyUI& msg);
	void OnBtnVoice(TNotifyUI& msg);
	void OnManagerButtonEvent(TNotifyUI& msg);
	void OnBtnSendFile(TNotifyUI& msg);
	void OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnBtnSendMessage(TNotifyUI& msg);

	BOOL _RichEdit_InsertFace(CRichEditUI * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex);

//操作树列表的操作
	void SendMsgToGetList();
	void AddHostUserList(UserListUI * ptr, CUserObject *user);
	void AddHostUserList(UserListUI * ptr, CUserObject *user, int pos);
	void AddOnlineVisitor(UserListUI * ptr, CUserObject *user, int index);
	void AddMyselfToList(UserListUI * ptr, CUserObject *user);
	UserListUI::Node* CMainFrame::GetOneUserNode(unsigned long id);
	void CMainFrame::DeleteOneUserNode(unsigned long id);
	void CMainFrame::OnItemClickEvent(unsigned long id,int type);

	void CMainFrame::HostUserOnlineAndOffline(CUserObject* pUser, bool type);
	void CMainFrame::VisitorUserOnlineAndOffline(CWebUserObject* pWebUser, bool type);
	void CMainFrame::FindVisitorFromOnlineNode(CWebUserObject* pWebUser);
	BOOL CMainFrame::CheckItemForOnlineVisitor(UserListUI::Node *curNode);
	void CMainFrame::ShowBigImage(string url, MSG_DATA_TYPE msgDataType);

	void CMainFrame::AcceptChat();
	void CMainFrame::RefuseChat();

	int CMainFrame::CheckIdForInvoteMyselfOrOther(unsigned long id);
	int CMainFrame::ReleaseChatIdForInvoteMyselfOrOther(unsigned long id);

	//判定当前的用户id 处于那种状态底下
	TREENODEENUM  CMainFrame::CheckIdForNodeType(unsigned long id);
	VISITOR_TYPE  CMainFrame::CheckIdForTalkType(unsigned long id);
	//0 为不可能 1为可用

	void CMainFrame::SetManagerButtonState(int i, int type);
	void CMainFrame::CreateSmallTaskIcon(WCHAR *name);
	void CMainFrame::OnCancel();

	int CMainFrame::ParseGroup(CMarkupXml &xml, int id, int curitemid);
	int CMainFrame::ParseGroupItem(CMarkupXml &xml, KEYWORDGROUP_INFO *pKeyWordGroupInfo, char *sKey, int type, int curitemid);

	KEYWORDGROUP_INFO *CMainFrame::AddKeyWordGroupInfo(unsigned long id, int sort, unsigned long compid, unsigned long uid, unsigned long parentid, unsigned char type, char *name);
	KEYWORD_INFO *CMainFrame::AddKeyWordInfo(unsigned long id, int sort, unsigned long compid,unsigned long uid, unsigned long groupid, unsigned char type,char *name, char *memo);

	void CMainFrame::DoRightShortAnswerList(string str);
	void CMainFrame::InsertInviteUserid(unsigned long webUserid, unsigned long id);
	unsigned long  CMainFrame::GetInviteUserid(unsigned long webUserid);


//接入聊天 相关处理
	void CMainFrame::OnMenuEvent(CDuiString controlName);
	void CMainFrame::OnCtrlVEvent();
	bool CMainFrame::SaveBitmapToFile(HBITMAP hbitmap, BITMAP bitmap, string lpFileName);

	void CMainFrame::UpdateTopCenterButtonState(unsigned long id);
	void CMainFrame::OnSelectUser(unsigned long id);
	void CMainFrame::OnActiveUser(unsigned long id,string sid);

	//void CMainFrame::ReplaceFaceId(string &msg);
	void CMainFrame::ShowMySelfSendMsg(string strMsg, MSG_DATA_TYPE msgType, string msgId);
	void CMainFrame::MoveAndRestoreMsgWnd(int type);
	void CMainFrame::InitLibcef(void);
	void CMainFrame::LoadBrowser(char* url);
	void CMainFrame::ShowRightOptionFrameView(unsigned long id,string sid);
	void CMainFrame::ShowClearMsg();
	void CMainFrame::ChangeShowUserMsgWnd(unsigned long id);
	void CMainFrame::SetHandler();
	MSG_RECV_TYPE  CMainFrame::GetSendUserType(unsigned long id);
	void CMainFrame::CheckIdForUerOrWebuser(unsigned long id,string sid, CWebUserObject **pWebUser, CUserObject **pUser);
	string CMainFrame::CreateClientInfoHtml(WxUserInfo* pWxUser);
	void CMainFrame::MoveAndRestoreRightFrameControl(int type); //0 max 1 retore
	void CMainFrame::InitRightTalkList();
	void CMainFrame::OnBtnSelectSendType(TNotifyUI& msg);
	void JsCallMFC(WPARAM wParam, LPARAM lParam);
	void CMainFrame::ShowWebBrowser(char *url);
	void CMainFrame::HideWebBrowser();

	CODE_RECORD_AUDIO StartRecordAudio();
	void CancelRecordAudio();
	void ShowOperationTips(CDuiString strTips); // 在聊天记录下方显示横条，做提示栏用
	void ShowRecordVoice(string strTips); // 在聊天记录下方显示横条，做提示栏用

	CDuiString CMainFrame::GetShowTimeString(unsigned long time);
protected:

	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnCloseBtn(TNotifyUI& msg);
	void OnMaxBtn(TNotifyUI& msg);
	void OnRestoreBtn(TNotifyUI& msg);
	void OnMinBtn(TNotifyUI& msg);


	bool ParseGroupUser(CMarkupXml &xml, CGroupObject *pGroupOb, char *sGroupKey, char *sUserKey);


public:
	IChatManager* m_manager;
    int	m_currentNumber;
	map<unsigned long, UserListUI::Node*> m_onlineNodeMap; //在线坐席
	map<unsigned long, UserListUI::Node*> m_offlineNodeMap; //离线坐席
	UserListUI::Node* pOnlineNode ;   //在线访客 
	UserListUI::Node* pWaitForAccept;  //等待应答
	UserListUI::Node* pMySelfeNode;    //自己的node
	map<unsigned long, UserListUI::Node*> m_waitVizitorMap;  //等待列表
	map<unsigned long, UserListUI::Node*> m_allVisitorNodeMap;  //所有访客列表
	list<unsigned long>m_invoteOtherList;     //存自己主动邀请协助的  id
	list<unsigned long>m_invoteMyselfList;

	map<unsigned long, unsigned long > m_allVisitorUserMap;   //所有访客的归属列表
	list<unsigned long >m_acceptingsUserList;                 //邀请列表
	list<unsigned long >m_transferUserList;                 //邀请列表

	vector<SHORT_ANSWER_STRUCT> m_savedShortAnswer;
	vector<SHORT_ANSWER_STRUCT> m_pushWebUrl;
	SHORT_ANSWER_STRUCT m_answerData;

	UserListUI::Node* m_pLastOfflineNode;
	UserListUI::Node* m_pLastOnlineNode;

	map<string, UserListUI::Node*> m_visitorOnlineNode; //在线访客 

	list<CUserObject* > m_upUser;
	unsigned int m_recordWaitNumber;

	CONTROL_ATTR m_centerChatInfo;

	RECT m_rightRectWnd;
	RECT m_rightRectMax;

	string m_defaultUrlInfo;
	string		m_audioPath;			// 正在录音文件的路径
	bool		m_bRecording;			// 正在录音

	CDuiString m_defalutButtonImage;

private:

	CSmallMenu m_frameSmallMenu;

	CEditUI *m_pRightCommonWordEdit, *m_pRightCommonTypeEdit , *m_pRightCommonFindEdit;
	CComboUI *m_pRightCommonWordCombo, *m_pRightCommonTypeCombo , *m_pRightCommonFindCombo;


	RECT m_mainCenterAndRightRect;
	CControlUI *m_MainCenterRightWND;
	CButtonUI * m_pFontBtn, *m_pFaceBtn, *m_pScreenBtn, *pSendMsgBtn,*m_pVoiceBtn;
	CFaceSelDlg m_faceSelDlg;
	CFaceList  m_faceList;


	ManagerButtonStruct m_pManagerBtn[MID_MANAGER_BUTTON_NUM];

	//CRichEditUI2    *m_pSendEdit;
	CRichEditUI* m_pSendEdit;


	HandlerInfo m_pListMsgHandler; // 消息列表
	HandlerInfo m_pWebURLHandler; // 访客历史 访客来电 访客留言 客户管理 统计分析 
	HandlerInfo m_pVisitorRelatedHandler;;


	//HandlerInfo m_pShowImageHandler; // 消息列表

	CDuiString m_sendMsgString;
	HWND m_hMainWnd;

	UserListUI* pUserList;
	UserListUI* m_pTalkList;

	unsigned long m_curSelectId;


	string m_facePathUrl;
	int  m_curSelectOptionBtn;
	unsigned long m_savedImageIndex;

	int m_userListCount;
	int m_recordListCount;

	unsigned long m_selectUserId;


	int  m_topWndType; //当前选择的哪种方式  进入在线列表选择 加以区分




	UserListUI::Node*  m_curClickItemNode;
	string m_curSavedSid;
	unsigned long m_savedClickId;
	map<unsigned long, unsigned long >m_recvUserObjMap;

	CShowBigImageDlg *pShowImgDlg;
	CSystemSettings *m_pSystSettingsDlg;
	COperationTips* m_pOperTipsDlg;

	bool   m_wndShow;	 
	int   m_startTimer;

	CWndShadow m_WndShadow;



};


#endif