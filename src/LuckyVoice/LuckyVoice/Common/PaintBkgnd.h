/*********************************************************************************
*  Author         : orbit                                                        *
*  Description    : 这个文件定义了模板类CPaintBkgnd，这是一个ATL的嵌入类，通过   *
                    嵌入窗口的消息链获取窗口消息，通过处理WM_ERASEBKGND消息定制  *
                    窗口的背景绘制行为，可以通过模板参设置背景颜色               *
*  Date           : Sep 6, 2005                                                  *
*  Last-modified  : Nov 18, 2005                                                 *
*  Contact us     : inte2000@163.com,support@winmsg.com                          *
*  Web Page       : http://www.winmsg.com/cn/orbit.htm                           *
*                   http://blog.csdn.net/orbit/          (关于代码的更新)        *
*********************************************************************************/
#ifndef __PAINTBKGND_H__
#define __PAINTBKGND_H__

template <class T, COLORREF t_crBrushColor>
class CPaintBkgnd : public CMessageMap
{
public:
	CPaintBkgnd() { m_hbrBkgnd = CreateSolidBrush(t_crBrushColor); }
	~CPaintBkgnd() { DeleteObject ( m_hbrBkgnd ); }

	BEGIN_MSG_MAP(CPaintBkgnd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()
 
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T*   pT = static_cast<T*>(this);
		HDC  dc = (HDC) wParam;
		RECT rcClient;
		pT->GetClientRect ( &rcClient );
		FillRect ( dc, &rcClient, m_hbrBkgnd );
		return TRUE; 
	}

protected:
	HBRUSH m_hbrBkgnd;
};


#endif //__PAINTBKGND_H__