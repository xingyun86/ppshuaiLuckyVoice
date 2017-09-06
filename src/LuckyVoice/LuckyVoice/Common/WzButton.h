/*********************************************************************************
*  Author         : orbit                                                        *
*  Description    : 这个文件定义CWzButtonImpl类，这是一个支持OwnerDraw的按钮类， *
*  Date           : Sep 15, 2003                                                 *
*  Last-modified  : Nov 21, 2003                                                 *
*  Contact us     : inte2000@163.com,support@winmsg.com                          *
*  Web Page       : http://www.winmsg.com/cn/orbit.htm                           *
*                   http://blog.csdn.net/orbit/          (关于代码的更新)        *
*********************************************************************************/
#ifndef __WZBUTTON_H__
#define __WZBUTTON_H__

//##ModelId=43538E3F038E
class CWzButtonImpl : public CWindowImpl<CWzButtonImpl, CButton>,public COwnerDraw<CWzButtonImpl>
{
public:
// Construction
public:
	//##ModelId=43538E3F03AB
  CWzButtonImpl();
	//##ModelId=43538E3F03AC
	~CWzButtonImpl();
	
	//##ModelId=43538E3F03B9
	BOOL SubclassWindow(HWND hWnd);
	//##ModelId=43538E3F03BB
	void Init();
	//##ModelId=43538E3F03C8
	void InitDefaultColor();
	//##ModelId=43538E3F03C9
	void SetFlat(BOOL bState, BOOL bRepaint = FALSE);
	//##ModelId=43538E3F03D8
	BOOL GetFlat();
	//##ModelId=43538E3F03D9
	void SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint = FALSE);
	//##ModelId=43538E400000
	BOOL GetFlatFocus();
	//##ModelId=43538E400001
	void DrawBorder(BOOL bEnable);
	//##ModelId=43538E400003
	void SetActiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	//##ModelId=43538E400011
	void SetInactiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	//##ModelId=43538E40001F
	void SetTextInactiveColor(COLORREF crNew, BOOL bRepaint = FALSE);
	//##ModelId=43538E400022
	void SetTextActiveColor(COLORREF crNew, BOOL bRepaint = FALSE);
	//##ModelId=43538E40002F
	void SetBorderColor(COLORREF crNew, BOOL bRepaint = FALSE);
	//##ModelId=43538E400032
	void SetButtonCursor(HCURSOR hCursor);

  BEGIN_MSG_MAP(CWzButtonImpl)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
 		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
   CHAIN_MSG_MAP_ALT(COwnerDraw<CWzButtonImpl>, 1)
    DEFAULT_REFLECTION_HANDLER()
  END_MSG_MAP()
	
	//##ModelId=43538E40003F
	LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//##ModelId=43538E40004E
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 1;	// no need for the background
	}

	//##ModelId=43538E400053
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//##ModelId=43538E400061
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//##ModelId=43538E400070
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//##ModelId=43538E40007F
  void DrawItem (LPDRAWITEMSTRUCT lpdis);

protected:
	//##ModelId=43538E400081
	BYTE m_bDefaultBtn;
	//##ModelId=43538E40008C
	BYTE m_MouseOnButton;
	//##ModelId=43538E40008D
	BYTE m_bAlwaysTrack;
	//##ModelId=43538E40008E
	BYTE m_bIsFlat;			// Is a flat button?
	//##ModelId=43538E40009C
	BYTE m_bDrawBorder;		// Draw border?
	//##ModelId=43538E40009D
	BYTE m_bDrawFlatFocus;	// Draw focus rectangle for flat button?
	//##ModelId=43538E4000AB
	HCURSOR m_hBtnCursor;
	
	//##ModelId=43538E4000AC
	COLORREF m_crInactiveBg;
	//##ModelId=43538E4000AD
	COLORREF m_crActiveBg;
	//##ModelId=43538E4000BB
	COLORREF m_crText;
	//##ModelId=43538E4000BC
	COLORREF m_crTextHilight;
	//##ModelId=43538E4000BD
	COLORREF m_crBorder;
};


//##ModelId=43538E3F03D8
inline BOOL CWzButtonImpl::GetFlat()
{ return m_bIsFlat;}

//##ModelId=43538E400001
inline void CWzButtonImpl::DrawBorder(BOOL bEnable)
{ m_bDrawBorder = bEnable; }

//##ModelId=43538E400000
inline BOOL CWzButtonImpl::GetFlatFocus()
{	return m_bDrawFlatFocus; }

//##ModelId=43538E400032
inline void CWzButtonImpl::SetButtonCursor(HCURSOR hCursor)
{ m_hBtnCursor = hCursor;}

#endif //__WZBUTTON_H__
