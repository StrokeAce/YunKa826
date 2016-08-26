#include "stdafx.h"
#include "operation_tips.h"
#include <ShlObj.h>
//#include "IChatManager.h"

COperationTips::COperationTips()
{
}


COperationTips::~COperationTips()
{
}

void COperationTips::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == L"close_button")
		{
			::ShowWindow(m_hWnd,SW_HIDE);
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
		
	}
}

LRESULT COperationTips::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
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

LRESULT COperationTips::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	m_PaintManager.AddPreMessageFilter(this);
	CDialogBuilder builder;

	CControlUI* pRoot = builder.Create(L"operation_tips.xml", (UINT)0, 0, &m_PaintManager);

	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	return 0;
}

LRESULT COperationTips::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT COperationTips::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT COperationTips::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT COperationTips::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT COperationTips::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT COperationTips::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT COperationTips::OnMenuHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT COperationTips::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void COperationTips::ShowWnd(int type, CPoint startPoint, CPoint endPoint, CDuiString tips)
{
	if (type == SW_MINIMIZE || type == SW_MAXIMIZE)
	{
		::MoveWindow(m_hWnd, startPoint.x, startPoint.y, endPoint.x - startPoint.x, endPoint.y - startPoint.y, true);
	}
	else if (type == SW_SHOW)
	{
		::MoveWindow(m_hWnd, startPoint.x, startPoint.y, endPoint.x - startPoint.x, endPoint.y - startPoint.y, true);
		::ShowWindow(m_hWnd, type);
		COptionUI * pCheck;
		pCheck = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("tips")));
		if (pCheck && !tips.IsEmpty())
		{
			pCheck->SetText(tips);
		}
	}
	else
	{
		::ShowWindow(m_hWnd, type);
	}
}

LRESULT COperationTips::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	return 0;
}

LPCTSTR COperationTips::GetWindowClassName() const
{
	return _T("UIOperationTipsDlg");
}

void COperationTips::MoveWnd(CPoint startPoint)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	::MoveWindow(m_hWnd, startPoint.x, startPoint.y, rc.right, rc.bottom, false);
}

