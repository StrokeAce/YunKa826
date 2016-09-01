#include "../stdafx.h"
#include "system_settings.h"
#include "ui_common/common_utility.h"
#include <ShlObj.h>

CSystemSettings::CSystemSettings()
{
}

CSystemSettings::~CSystemSettings()
{
}

int CALLBACK CSystemSettings::BrowseCallbackProc(HWND hwnd, UINT msg, LPARAM lp, LPARAM pData)
{
	switch (msg)
	{
		case BFFM_INITIALIZED:
			::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
			break;
	}

	return 0;
}

void CSystemSettings::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		OnPrepare(msg);
	}

	if (msg.sType == DUI_MSGTYPE_CLICK)
	{ 
		if (msg.pSender->GetName() == L"close_button")
		{
			SaveSystemSettings();
			Close();
		}
		else if (msg.pSender->GetName() == L"reset_botton")
		{
			ResetAlertSettings();
		}
		else if (msg.pSender->GetName().Find(L"find_file_1") >= 0)
		{
			SelectSoundFile(1);
		}
		else if (msg.pSender->GetName().Find(L"find_file_2") >= 0)
		{
			SelectSoundFile(2);
		}
		else if (msg.pSender->GetName().Find(L"find_file_3") >= 0)
		{
			SelectSoundFile(3);
		}
		else if (msg.pSender->GetName().Find(L"find_file_4") >= 0)
		{
			SelectSoundFile(4);
		}
		else if (msg.pSender->GetName().Find(L"find_file_5") >= 0)
		{
			SelectSoundFile(5);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{

	}
	else if (msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{

	}
	else if (msg.sType = DUI_MSGTYPE_SELECTCHANGED)
	{
		OnSelectChanged(msg);
	}
}

LRESULT CSystemSettings::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN) 
	{
		if (wParam == VK_RETURN) 
		{
			return true;
		}
		else if (wParam == VK_ESCAPE) 
		{
			return true;
		}
	}

	return false;
}

LRESULT CSystemSettings::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	m_PaintManager.AddPreMessageFilter(this);
	CDialogBuilder builder;

	CControlUI* pRoot = builder.Create(L"systemsettings\\system_settings.xml", (UINT)0, 0, &m_PaintManager);

	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	return 0;
}

LRESULT CSystemSettings::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_MY_MESSAGE_NOTIFYICON:    HandleCustomMessage(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;


	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled)
		return lRes;

	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CSystemSettings::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CSystemSettings::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CSystemSettings::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CSystemSettings::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CSystemSettings::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CSystemSettings::OnMenuHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CSystemSettings::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_HIDE_IMAGE_WND_MSG)
	{
		::ShowWindow(m_hWnd, SW_HIDE);
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		OnMouseMove(uMsg, wParam, lParam);
		int a = 10;
	}
		
	return 0;
}

void CSystemSettings::OnPrepare(TNotifyUI& msg)
{
	COptionUI * pCheck;
	CEditUI* pEdit;
	TCHAR fileName[MAX_1024_LEN];
	string filePath;

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_visit_1")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[0]->bTray);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_visit_2")));
	if (pCheck)  pCheck->Selected(m_sysConfig->m_cAlertInfoList[0]->bShowwnd);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_visit_3")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[0]->bSound); 
	pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("sound_file_1")));
	if (pEdit)
	{
		filePath = m_sysConfig->m_cAlertInfoList[0]->soundfilename;
		int pos = filePath.find_last_of("\\");
		if (pos > 0)
		{
			filePath = filePath.substr(pos + 1, filePath.length() - pos - 1);
		}
		ANSIToUnicode(filePath.c_str(), fileName);
		pEdit->SetText(fileName);
	}

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_chat_1")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[1]->bTray);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_chat_2")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[1]->bShowwnd);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_chat_3")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[1]->bSound);
	pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("sound_file_2")));
	if (pEdit)
	{
		filePath = m_sysConfig->m_cAlertInfoList[1]->soundfilename;
		int pos = filePath.find_last_of("\\");
		if (pos > 0)
		{
			filePath = filePath.substr(pos + 1, filePath.length() - pos - 1);
		}
		ANSIToUnicode(filePath.c_str(), fileName);
		pEdit->SetText(fileName);
	}

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_msg_1")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[2]->bTray);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_msg_2")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[2]->bShowwnd);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_msg_3")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[2]->bSound);
	pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("sound_file_3")));
	if (pEdit)
	{
		filePath = m_sysConfig->m_cAlertInfoList[2]->soundfilename;
		int pos = filePath.find_last_of("\\");
		if (pos > 0)
		{
			filePath = filePath.substr(pos + 1, filePath.length() - pos - 1);
		}
		ANSIToUnicode(filePath.c_str(), fileName);
		pEdit->SetText(fileName);
	}

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_transfer_1")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[3]->bTray);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_transfer_2")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[3]->bShowwnd);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_transfer_3")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[3]->bSound);
	pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("sound_file_4")));
	if (pEdit)
	{
		filePath = m_sysConfig->m_cAlertInfoList[3]->soundfilename;
		int pos = filePath.find_last_of("\\");
		if (pos > 0)
		{
			filePath = filePath.substr(pos + 1, filePath.length() - pos - 1);
		}
		ANSIToUnicode(filePath.c_str(), fileName);
		pEdit->SetText(fileName);
	}

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_other_1")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[4]->bTray);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_other_2")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[4]->bShowwnd);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_other_3")));
	if (pCheck) pCheck->Selected(m_sysConfig->m_cAlertInfoList[4]->bSound);
	pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("sound_file_5")));
	if (pEdit)
	{
		filePath = m_sysConfig->m_cAlertInfoList[4]->soundfilename;
		int pos = filePath.find_last_of("\\");
		if (pos > 0)
		{
			filePath = filePath.substr(pos + 1, filePath.length() - pos - 1);
		}
		ANSIToUnicode(filePath.c_str(), fileName);
		pEdit->SetText(fileName);
	}
}

LPCTSTR CSystemSettings::GetWindowClassName() const
{
	return _T("UIShowSystemSettingDlg");
}

UINT CSystemSettings::GetClassStyle() const
{
	return UI_CLASSSTYLE_DIALOG;
}

void CSystemSettings::ShowWnd(int type)
{
	::ShowWindow(m_hWnd, type);
}

void CSystemSettings::OnSelectChanged(TNotifyUI &msg)
{
	WCHAR OptionBtnName[MAX_64_LEN] = { 0 };
	CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("left_tab")));
	if (pTabControl != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			swprintf_s(OptionBtnName, _T("top_tab_button_%d"), i + 1);

			if (_tcsicmp(msg.pSender->GetName(), OptionBtnName) == 0)
			{
				if (pTabControl && pTabControl->GetCurSel() != i)
				{
					pTabControl->SelectItem(i);
					break;
				}
			}
		}
	}

	pTabControl = NULL;
	pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("right_frame_1")));
	if (pTabControl != NULL)
	{
		for (int i = 0; i < 5; i++)
		{
			swprintf_s(OptionBtnName, _T("left_tab_1_button_%d"), i + 1);

			if (_tcsicmp(msg.pSender->GetName(), OptionBtnName) == 0)
			{
				if (pTabControl && pTabControl->GetCurSel() != i)
				{
					pTabControl->SelectItem(i);
					break;
				}
			}
		}
	}
}

LRESULT CSystemSettings::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	return 0;
}

void CSystemSettings::SaveSystemSettings()
{
	COptionUI * pCheck;

	m_sysConfig->m_cAlertInfoList[0]->type = ALERT_NEW_VISIT;
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_visit_1")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[0]->bTray = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_visit_2")));
	if (pCheck)  m_sysConfig->m_cAlertInfoList[0]->bShowwnd = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_visit_3")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[0]->bSound = pCheck->IsSelected();

	m_sysConfig->m_cAlertInfoList[1]->type = ALERT_NEW_CHAT;
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_chat_1")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[1]->bTray = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_chat_2")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[1]->bShowwnd = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_chat_3")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[1]->bSound = pCheck->IsSelected();

	m_sysConfig->m_cAlertInfoList[2]->type = ALERT_NEW_MSG;
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_msg_1")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[2]->bTray = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_msg_2")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[2]->bShowwnd = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_msg_3")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[2]->bSound = pCheck->IsSelected();

	m_sysConfig->m_cAlertInfoList[3]->type = ALERT_NEW_TRANSFER;
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_transfer_1")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[3]->bTray = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_transfer_2")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[3]->bShowwnd = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_transfer_3")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[3]->bSound = pCheck->IsSelected();

	m_sysConfig->m_cAlertInfoList[4]->type = ALERT_NEW_OTHER;
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_other_1")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[4]->bTray = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_other_2")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[4]->bShowwnd = pCheck->IsSelected();
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_other_3")));
	if (pCheck) m_sysConfig->m_cAlertInfoList[4]->bSound = pCheck->IsSelected();
}

void CSystemSettings::ResetAlertSettings()
{
	COptionUI * pCheck;

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_visit_1")));
	if (pCheck) pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_visit_2")));
	if (pCheck)  pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_visit_3")));
	if (pCheck) pCheck->Selected(true);

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_chat_1")));
	if (pCheck) pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_chat_2")));
	if (pCheck) pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_chat_3")));
	if (pCheck) pCheck->Selected(true);

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_msg_1")));
	if (pCheck) pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_msg_2")));
	if (pCheck) pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_msg_3")));
	if (pCheck) pCheck->Selected(true);

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_transfer_1")));
	if (pCheck) pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_transfer_2")));
	if (pCheck) pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_transfer_3")));
	if (pCheck) pCheck->Selected(true);

	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_other_1")));
	if (pCheck) pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_other_2")));
	if (pCheck) pCheck->Selected(false);
	pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("new_other_3")));
	if (pCheck) pCheck->Selected(false);
}

void CSystemSettings::SelectSoundFile(int index)
{
	TCHAR pszPath[MAX_1024_LEN];
	string filePath = "C:\\";
	if (m_sysConfig && m_sysConfig->m_cAlertInfoList[0])
	{
		filePath = m_sysConfig->m_cAlertInfoList[0]->soundfilename;
		int pos = filePath.find_last_of("\\");
		filePath = filePath.substr(0, pos);
	}

	ANSIToUnicode(filePath.c_str(), pszPath);

	BROWSEINFO bi;
	bi.hwndOwner = m_PaintManager.GetPaintWindow();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT("ÇëÑ¡ÔñÉùÒôÎÄ¼þ");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_BROWSEINCLUDEFILES;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)pszPath;
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL) return;

	if (SHGetPathFromIDList(pidl, pszPath))
	{
		wstring wfilePath = pszPath;

		if ((int)wfilePath.find(_T(".wav")) > 0 || (int)wfilePath.find(_T(".mp3")) > 0)
		{
			CEditUI* pEdit;
			TCHAR controlName[MAX_64_LEN];
			wsprintf(controlName, _T("sound_file_%d"), index);
			pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(controlName));
			if (pEdit)
			{
				int pos = wfilePath.find_last_of(_T("\\"));
				if (pos > 0)
				{
					wfilePath = wfilePath.substr(pos + 1, wfilePath.length() - pos - 1);
				}
				pEdit->SetText(wfilePath.c_str());
				if (m_sysConfig && m_sysConfig->m_cAlertInfoList[index - 1])
				{
					char cfileName[MAX_1024_LEN];
					UnicodeToANSI(pszPath, cfileName);
					strcpy(m_sysConfig->m_cAlertInfoList[index - 1]->soundfilename, cfileName);
				}
			}
		}		
	}
}

