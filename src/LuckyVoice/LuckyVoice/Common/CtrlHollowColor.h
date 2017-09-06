/*********************************************************************************
*  Author         : orbit                                                        *
*  Description    : ����ļ�������ģ����CCtrlHollowColor������һ��ATL��Ƕ���࣬  *
                    ͨ��Ƕ�봰�ڵ���Ϣ����ȡ������Ϣ��ͨ��������ص���Ϣ���ƴ��� *
                    �Ŀؼ�������ɫ�����ڱ�����ɫ�ȵȣ�����ͨ��ģ������ñ�����ɫ *               *
*  Date           : Sep 6, 2005                                                  *
*  Last-modified  : Nov 18, 2005                                                 *
*  Contact us     : inte2000@163.com,support@winmsg.com                          *
*  Web Page       : http://www.winmsg.com/cn/orbit.htm                           *
*                   http://blog.csdn.net/orbit/          (���ڴ���ĸ���)        *
*********************************************************************************/
#ifndef __CTRHOLLOWLCOLOR_H__
#define __CTRHOLLOWLCOLOR_H__

template <class T, COLORREF t_crTextColor = RGB(0,0,255), COLORREF t_crBkgnd = RGB(215,241,215)>
class CCtrlHollowColor
{
public:
	CCtrlHollowColor() 
	{ 
		m_HollowBrush = AtlGetStockBrush(HOLLOW_BRUSH);
	}
	
	~CCtrlHollowColor() 
	{ 
		DeleteObject(m_HollowBrush);
	}
 
	BEGIN_MSG_MAP(CCtrlHollowColor)
		MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
	END_MSG_MAP()

	LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//T*   pT = static_cast<T*>(this);
		HDC hDC = (HDC)wParam;
		HWND hWnd = (HWND)lParam;

		if(uMsg == WM_CTLCOLORBTN || uMsg == WM_CTLCOLORSTATIC)
		{
			SetTextColor(hDC,t_crTextColor);
			SetBkMode(hDC,TRANSPARENT);
			bHandled = TRUE;
			return (LRESULT)m_HollowBrush;
		}
		
		SetTextColor(hDC,t_crTextColor);
		SetBkMode(hDC,TRANSPARENT);
		return 0;
	}
 
protected:
	HBRUSH m_HollowBrush;
};

#endif //__CTRHOLLOWLCOLOR_H__