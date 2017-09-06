/*********************************************************************************
*  Author         : orbit                                                        *
*  Description    : ����ļ�������ģ����CCtrlColor������һ��ATL��Ƕ���࣬ͨ��Ƕ  *
                    �봰�ڵ���Ϣ����ȡ������Ϣ��ͨ��������ص���Ϣ���ƴ��ڵĻ�   *
                    ����Ϊ������ؼ�������ɫ���Ի��򱳾���ɫ�ȵȣ�����ͨ��ģ��� *
                    ��������Щ��ɫֵ                                             *
*  Date           : July 21, 2002                                                *
*  Last-modified  : Nov 6, 2002                                                  *
*  Contact us     : inte2000@163.com,support@winmsg.com                          *
*  Web Page       : http://www.winmsg.com/cn/orbit.htm                           *
*                   http://blog.csdn.net/orbit/          (���ڴ���ĸ���)        *
*********************************************************************************/
#ifndef __CTRLCOLOR_H__
#define __CTRLCOLOR_H__

//template <class T, COLORREF t_crTextColor = RGB(64,64,255), COLORREF t_crBkgnd = RGB(222,222,222)>
//##ModelId=43538E41030D
template <class T, COLORREF t_crTextColor = RGB(0,0,255), COLORREF t_crBkgnd = RGB(215,241,215)>
class CCtrlColor
{
public:
	//##ModelId=43538E410311
	CCtrlColor() { m_brBkgnd = CreateSolidBrush(t_crBkgnd);}
	//##ModelId=43538E41031C
	~CCtrlColor() { DeleteObject(m_brBkgnd);}
 
	BEGIN_MSG_MAP(CCtrlColor)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
	END_MSG_MAP()

	//##ModelId=43538E41031D
	LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//T*   pT = static_cast<T*>(this);
		HDC hDC = (HDC)wParam;
		HWND hWnd = (HWND)lParam;

		if(uMsg == WM_CTLCOLORDLG)
		{
			bHandled = TRUE;
			return (LRESULT)m_brBkgnd;
		}
		else if(uMsg == WM_CTLCOLORSTATIC)
		{
			SetBkMode(hDC,TRANSPARENT);
			SetTextColor(hDC,t_crTextColor);
			bHandled = TRUE;
			return (LRESULT)m_brBkgnd;
		}
		else
			return 0;
	}
 
protected:
	//##ModelId=43538E41032C
	HBRUSH m_brBkgnd;
};

#endif //__CTRLCOLOR_H__
