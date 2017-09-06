/*********************************************************************************
*  Author         : orbit                                                        *
*  Description    : ����ļ�������ģ����CBitmapBkgnd������һ��ATL��Ƕ���࣬ͨ��  *
                    Ƕ�봰�ڵ���Ϣ����ȡ������Ϣ��ͨ������WM_ERASEBKGND��Ϣ����  *
                    ���ڵı���������Ϊ������ͨ��ģ������ñ���λͼ����ԴID       *
*  Date           : Sep 6, 2005                                                  *
*  Last-modified  : Nov 19, 2005                                                 *
*  Contact us     : inte2000@163.com,support@winmsg.com                          *
*  Web Page       : http://www.winmsg.com/cn/orbit.htm                           *
*                   http://blog.csdn.net/orbit/          (���ڴ���ĸ���)        *
*********************************************************************************/
#ifndef __BITMAPBKGND_H__
#define __BITMAPBKGND_H__

template <class T, UINT uBitmapID>
class CBitmapBkgnd : public CMessageMap
{
public:
	CBitmapBkgnd() { m_Bitmap.LoadBitmap(uBitmapID); }
	~CBitmapBkgnd() { m_Bitmap.DeleteObject(); }

	BEGIN_MSG_MAP(CPaintBkgnd)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hDC = (HDC)wParam;
		HWND hWnd = (HWND)lParam;

		if (uMsg == WM_CTLCOLORBTN || uMsg == WM_CTLCOLORSTATIC)
		{
			//SetTextColor(hDC, RGB(255, 0, 255));
			SetBkMode(hDC, TRANSPARENT);
			bHandled = TRUE;
			return (LRESULT)GetStockObject(LTGRAY_BRUSH);
		}

		//SetTextColor(hDC, RGB(255, 0, 255));
		SetBkMode(hDC, TRANSPARENT);

		return 0;
	}
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T*   pT = static_cast<T*>(this);
		HDC  hDC = (HDC) wParam;
		RECT rcClient;
		pT->GetClientRect ( &rcClient );

		BITMAP bm;
		m_Bitmap.GetBitmap(&bm);
		CDC memDC;
		memDC.CreateCompatibleDC(hDC);
		HBITMAP hOldBmp = memDC.SelectBitmap(m_Bitmap);
		StretchBlt(hDC,0,0,rcClient.right - rcClient.left,rcClient.bottom - rcClient.top,memDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
		memDC.SelectBitmap(hOldBmp);
		memDC.DeleteDC();

		return TRUE;    // we painted the background
	}

protected:
	CBitmap m_Bitmap;
};

#endif //__BITMAPBKGND_H__