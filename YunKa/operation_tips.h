#ifndef _OPERATION_TIPS_H_
#define _OPERATION_TIPS_H_

#pragma once

class COperationTips : public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
public:
	COperationTips();
	~COperationTips();

	LPCTSTR GetWindowClassName() const;

	virtual void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnMenuHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void ShowWnd(int type, CPoint startPoint, CPoint endPoint, CDuiString tips=_T(""));
	void MoveWnd(CPoint startPoint);

private:
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CPaintManagerUI m_PaintManager;
};

#endif