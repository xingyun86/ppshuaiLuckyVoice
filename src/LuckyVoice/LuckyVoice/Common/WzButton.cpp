/*********************************************************************************
*  Author         : orbit                                                        *
*  Description    : 这个文件定义CWzButtonImpl类，这是一个支持OwnerDraw的按钮类， *
*  Date           : Sep 15, 2003                                                 *
*  Last-modified  : Nov 21, 2003                                                 *
*  Contact us     : inte2000@163.com,support@winmsg.com                          *
*  Web Page       : http://www.winmsg.com/cn/orbit.htm                           *
*                   http://blog.csdn.net/orbit/          (关于代码的更新)        *
*********************************************************************************/
#include "stdafx.h"
#include "WzButton.h"

//##ModelId=43538E3F03AB
CWzButtonImpl::CWzButtonImpl()
{
  m_MouseOnButton = 0;
	m_bIsFlat = 1;
	m_bDrawBorder = 1; 
	m_bDrawFlatFocus = 1;
	m_bAlwaysTrack = 1;
	m_hBtnCursor = NULL;

	InitDefaultColor();
}

//##ModelId=43538E3F03AC
CWzButtonImpl::~CWzButtonImpl()
{
}

//##ModelId=43538E3F03B9
BOOL CWzButtonImpl::SubclassWindow(HWND hWnd)
{
	ATLASSERT(m_hWnd == NULL);
	ATLASSERT(::IsWindow(hWnd));
	BOOL bRet = CWindowImpl<CWzButtonImpl, CButton>::SubclassWindow(hWnd);
	if(bRet)
	{
		Init();
	}
	return bRet;
}

//##ModelId=43538E3F03BB
void CWzButtonImpl::Init()
{
	UINT nBS = GetButtonStyle();
	if(nBS & BS_DEFPUSHBUTTON)
		m_bDefaultBtn = 1;
	else
		m_bDefaultBtn = 0;
  SetButtonStyle(nBS | BS_OWNERDRAW);
}

//##ModelId=43538E3F03C8
void CWzButtonImpl::InitDefaultColor()
{
//	m_crInactiveBg = RGB(222,222,222);
	m_crInactiveBg = RGB(215,241,215);
	m_crActiveBg = RGB(43,200,213);
	m_crText = RGB(0,0,255);
	m_crTextHilight = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crBorder = RGB(254,128,62);
}

//##ModelId=43538E3F03C9
void CWzButtonImpl::SetFlat(BOOL bState, BOOL bRepaint)
{ 
	m_bIsFlat = bState; 
	if(bRepaint) 
		Invalidate();
}

//##ModelId=43538E3F03D9
void CWzButtonImpl::SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint)
{
	m_bDrawFlatFocus = bDrawFlatFocus;
	// Repaint the button
	if(bRepaint) 
		Invalidate();
} // End of SetFlatFocus

//##ModelId=43538E400011
void CWzButtonImpl::SetInactiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crInactiveBg = crNew; 
	if(bRepaint) 
		Invalidate();
}

//##ModelId=43538E400003
void CWzButtonImpl::SetActiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crActiveBg = crNew; 
	if (bRepaint) 
		Invalidate();
}

//##ModelId=43538E40001F
void CWzButtonImpl::SetTextInactiveColor(COLORREF crNew, BOOL bRepaint)
{
	m_crText = crNew; 
	if (bRepaint) 
		Invalidate();
}

//##ModelId=43538E400022
void CWzButtonImpl::SetTextActiveColor(COLORREF crNew, BOOL bRepaint)
{
	m_crTextHilight = crNew; 
	if (bRepaint) 
		Invalidate();
}

//##ModelId=43538E40002F
void CWzButtonImpl::SetBorderColor(COLORREF crNew, BOOL bRepaint)
{
	m_crBorder = crNew; 
	if (bRepaint) 
		Invalidate();
}

//##ModelId=43538E40003F
LRESULT CWzButtonImpl::OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(m_MouseOnButton == 1)
	{
		ReleaseCapture();
		Invalidate();
	}

	return 0;
}

//##ModelId=43538E400053
LRESULT CWzButtonImpl::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lr = CWindowImpl<CWzButtonImpl, CButton>::DefWindowProc(uMsg,wParam,lParam);
	bHandled = TRUE;

	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	UINT nFlags = wParam;

 // HWND hWnd;  // Finestra attiva
  HWND hParent; // Finestra che contiene il bottone

  // If the mouse enter the button with the left button pressed then do nothing
  if (nFlags & MK_LBUTTON && m_MouseOnButton == 0) 
		return lr;

  // If our button is not flat then do nothing
  if (m_bIsFlat == 0) 
		return lr;

//hWnd = ::GetActiveWindow();
	hParent = ::GetParent(m_hWnd);

	if ((::GetCapture() != m_hWnd) &&	(hParent != NULL)) 
	{
		m_MouseOnButton = 1;
		//SetFocus();	// Thanks Ralph!
		SetCapture();
		Invalidate();
	}
	else
  {
    RECT rc;
    GetClientRect(&rc);
    if (!PtInRect(&rc,point))
    {
      // Redraw only if mouse goes out
      if (m_MouseOnButton == 1)
      {
        m_MouseOnButton = 0;
        Invalidate();
      }
      // If user is NOT pressing left button then release capture!
      if (!(nFlags & MK_LBUTTON)) 
				ReleaseCapture();
    }
  }

	return lr;
}

//##ModelId=43538E400061
LRESULT CWzButtonImpl::OnSetCursor(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// If a cursor was specified then use it!
	if (m_hBtnCursor != NULL)
	{
		::SetCursor(m_hBtnCursor);
		bHandled = TRUE;
		return TRUE;
	}

	return 0;
}

//##ModelId=43538E400070
LRESULT CWzButtonImpl::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // If our button is not flat then do nothing
  if (m_bIsFlat == 0) 
		return 0;

//  if (m_MouseOnButton == 1)
 // {
    m_MouseOnButton = 0;
    Invalidate();
 // }

	return 0;
}

//##ModelId=43538E40007F
void CWzButtonImpl::DrawItem (LPDRAWITEMSTRUCT lpdis)
{
  HPEN hOldPen;
  BOOL bIsPressed  = (lpdis->itemState & ODS_SELECTED);
  BOOL bIsFocused  = ::GetFocus() == m_hWnd;//(lpdis->itemState & ODS_FOCUS) == ODS_FOCUS;
  BOOL bIsDisabled = (lpdis->itemState & ODS_DISABLED);
//	BOOL bIsDefault = (lpDIS->itemState & ODS_DEFAULT);
  HDC hDC = lpdis->hDC;
	RECT itemRect = lpdis->rcItem;

	SetBkMode(hDC,TRANSPARENT);

	if (m_bIsFlat == 0)
	{
		if (bIsFocused || m_bDefaultBtn == 1)
		{
			HBRUSH hbr = CreateSolidBrush(RGB(0,0,0));  
			FrameRect(hDC,&itemRect, hbr);//draw a black color thin line around button
			DeleteObject(hbr);
			InflateRect(&itemRect,-1,-1);
		}
	}

  // Prepare draw... paint button's area with background color
	COLORREF crBkgnd,crText,crFrame;
	if(bIsDisabled)
	{
		crFrame = GetSysColor(COLOR_INACTIVEBORDER);
		crText = GetSysColor(COLOR_GRAYTEXT);
		crBkgnd = m_crInactiveBg;
	}
	else
	{
		if ((m_MouseOnButton == 1) || (bIsPressed))
		{
			crBkgnd = m_crActiveBg;
			crText = m_crTextHilight;
		}
		else
		{
			crBkgnd = m_crInactiveBg;
			crText = m_crText;
		}
		crFrame = m_crBorder;
	}
	//draw background 
	SetBkColor(hDC, crBkgnd);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &itemRect, NULL, 0, NULL);

  // Draw pressed button
  if (bIsPressed)
  {
		HPEN penBtnHiLight = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DHILIGHT));//COLOR_3DLIGHT)); // Bianco
		HPEN penBtnShadow = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DSHADOW));   // Grigio scuro

		// draw darkline of left-top conner
		hOldPen = (HPEN)SelectObject(hDC,penBtnShadow);
		MoveToEx(hDC,itemRect.left, itemRect.bottom-1,NULL);
		LineTo(hDC,itemRect.left, itemRect.top);
		LineTo(hDC,itemRect.right, itemRect.top);
		// draw hilight line of right-bottom conner
		SelectObject(hDC,penBtnHiLight);
		MoveToEx(hDC,itemRect.left, itemRect.bottom-1,NULL);
		LineTo(hDC,itemRect.right-1, itemRect.bottom-1);
		LineTo(hDC,itemRect.right-1, itemRect.top-1);
		//release resource
		SelectObject(hDC,hOldPen);
		DeleteObject(penBtnShadow);
		DeleteObject(penBtnHiLight);
  }
  else // ...else draw non pressed button
  {
    if((m_bIsFlat == 0) || (m_bIsFlat == 1 && m_MouseOnButton == 1))
    {
			HPEN pen3DLight = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DHILIGHT));//COLOR_3DLIGHT));       // Light gray
			HPEN penBtnShadow = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DSHADOW));   // Dark gray
      // White line
      hOldPen = (HPEN)SelectObject(hDC,pen3DLight);
      MoveToEx(hDC,itemRect.left, itemRect.bottom-1,NULL);
      LineTo(hDC,itemRect.left, itemRect.top);
      LineTo(hDC,itemRect.right, itemRect.top);
      // Dark gray line
      SelectObject(hDC,penBtnShadow);
      MoveToEx(hDC,itemRect.left, itemRect.bottom-1,NULL);
      LineTo(hDC,itemRect.right-1, itemRect.bottom-1);
      LineTo(hDC,itemRect.right-1, itemRect.top-1);
			//
			SelectObject(hDC,hOldPen);
			DeleteObject(pen3DLight); 
			DeleteObject(penBtnShadow);
    }
		else
		{
			if(m_bDrawBorder == 1)
			{
				HPEN penBorder = CreatePen(PS_SOLID, 1, crFrame);
				hOldPen = (HPEN)SelectObject(hDC,penBorder);
				SelectObject(hDC,GetStockObject(NULL_BRUSH));
				Rectangle(hDC,itemRect.left,itemRect.top,itemRect.right,itemRect.bottom);
				SelectObject(hDC,hOldPen);
				DeleteObject(penBorder);
			}
		}
  }

  // Read the button title
  TCHAR sTitle[64];
  GetWindowText(sTitle,64);

  RECT captionRect = lpdis->rcItem;

  // Write the button title (if any)
  if (lstrlen(sTitle) != 0)
  {
    if(bIsPressed)
      OffsetRect(&captionRect,1, 1);
    
		SetTextColor(hDC,crText);
		RECT centerRect = captionRect;
		DrawText(hDC,sTitle, -1, &captionRect, DT_SINGLELINE|DT_CALCRECT);
		int xoff = ((centerRect.right - centerRect.left) - (captionRect.right - captionRect.left)) / 2;
		int yoff = ((centerRect.bottom - centerRect.top) - (captionRect.bottom - captionRect.top)) / 2;
		OffsetRect(&captionRect,xoff, yoff);
		COLORREF oc = SetBkColor(hDC,crBkgnd);
		DrawText(hDC,sTitle, -1, &captionRect, DT_SINGLELINE);
		SetTextColor(hDC,oc);
  }

	if((m_bIsFlat == 0) || ((m_bIsFlat == 1) && (m_bDrawFlatFocus == 1)))
	{
		// Draw the focus rect
		if (bIsFocused)
		{
			RECT focusRect = itemRect;
			InflateRect(&focusRect,-4, -3);
//			DrawFocusRect(hDC,&focusRect);
			HPEN penFocus = CreatePen(PS_SOLID, 0, RGB(128,128,128));
			HPEN hOldPen = (HPEN)SelectObject(hDC,penFocus);
      
			MoveToEx(hDC,focusRect.left + 4, focusRect.top,NULL);
      LineTo(hDC,focusRect.left, focusRect.top);
      LineTo(hDC,focusRect.left, focusRect.top + 4);

			MoveToEx(hDC,focusRect.right - 4, focusRect.top,NULL);
      LineTo(hDC,focusRect.right, focusRect.top);
      LineTo(hDC,focusRect.right, focusRect.top + 4);

			MoveToEx(hDC,focusRect.left, focusRect.bottom - 4,NULL);
      LineTo(hDC,focusRect.left, focusRect.bottom);
      LineTo(hDC,focusRect.left + 4, focusRect.bottom);

			MoveToEx(hDC,focusRect.right - 4, focusRect.bottom,NULL);
      LineTo(hDC,focusRect.right, focusRect.bottom);
      LineTo(hDC,focusRect.right, focusRect.bottom - 4);

			SelectObject(hDC,hOldPen);
			DeleteObject(penFocus);
		}
	}
}
