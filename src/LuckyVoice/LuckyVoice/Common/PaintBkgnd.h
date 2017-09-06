/*********************************************************************************
*  Author         : orbit                                                        *
*  Description    : ����ļ�������ģ����CPaintBkgnd������һ��ATL��Ƕ���࣬ͨ��   *
                    Ƕ�봰�ڵ���Ϣ����ȡ������Ϣ��ͨ������WM_ERASEBKGND��Ϣ����  *
                    ���ڵı���������Ϊ������ͨ��ģ������ñ�����ɫ               *
*  Date           : Sep 6, 2005                                                  *
*  Last-modified  : Nov 18, 2005                                                 *
*  Contact us     : inte2000@163.com,support@winmsg.com                          *
*  Web Page       : http://www.winmsg.com/cn/orbit.htm                           *
*                   http://blog.csdn.net/orbit/          (���ڴ���ĸ���)        *
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