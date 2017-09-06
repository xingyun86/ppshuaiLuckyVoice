/*********************************************************************************
*  Author         : orbit                                                        *
*  Description    : 这个文件定义了模板类CButtonHelp，这个模板类帮助对话框实现按  *
                    钮控件的子类化，可以通过模板参数指定按钮的子类化实现类       *
*  Date           : Sep 13, 2003                                                 *
*  Last-modified  : Oct 19, 2003                                                 *
*  Contact us     : inte2000@163.com,support@winmsg.com                          *
*  Web Page       : http://www.winmsg.com/cn/orbit.htm                           *
*                   http://blog.csdn.net/orbit/          (关于代码的更新)        *
*********************************************************************************/
#ifndef __BUTTONHELP_H__
#define __BUTTONHELP_H__

//##ModelId=43538E4103A9
template <class T, class t_ButtonClass>
class CButtonHelp
{
public:
	//##ModelId=43538E4103AC
	CButtonHelp()
	{
		m_arButtons.RemoveAll();
	}

	//##ModelId=43538E4103B9
	~CButtonHelp() 
	{
		int nSize = m_arButtons.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			t_ButtonClass *p = m_arButtons[i];
			ATLASSERT(p);
			delete p;
		}

		m_arButtons.RemoveAll();
	}
 
	//##ModelId=43538E4103BA
	BOOL SubclassAllButtons()
	{
		T*   pT = static_cast<T*>(this);
		return EnumChildWindows(pT->m_hWnd,EnumChildProc,(LPARAM)this);
	}

	//##ModelId=43538E4103BB
	BOOL SubclassButton(HWND hBtnWnd)
	{
		if(FindButtons(hBtnWnd) == -1)//not find
		{
			t_ButtonClass *pBtn = new t_ButtonClass();
			if(pBtn != NULL)
			{
				pBtn->SubclassWindow(hBtnWnd);
				m_arButtons.Add(pBtn);
				return TRUE;
			}
		}

		return FALSE;
	}

	//##ModelId=43538E4103BD
	BOOL UnSubclassButton(HWND hBtnWnd)
	{
		int nidx = FindButtons(hBtnWnd);
		if(nidx != -1)
		{
			t_ButtonClass *pBtn = m_arButtons[nidx];
			pBtn->UnsubclassWindow(hBtnWnd);
			delete pBtn;
			m_arButtons.RemoveAt(nidx);
		}

		return TRUE;
	}

	//##ModelId=43538E4103C9
	int FindButtons(HWND hBtnWnd)
	{
		int nRtn = -1;
		int nSize = m_arButtons.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			t_ButtonClass *p = m_arButtons[i];
			ATLASSERT(p);
			if(p->m_hWnd == hBtnWnd)//find the subclass items
			{
				nRtn = i;
				break;
			}
		}

		return nRtn;
	}

	//##ModelId=43538E4103CB
	static BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam)
	{
	//	CButtonHelp *pThis = (CButtonHelp *)lParam;
		CButtonHelp<T,t_ButtonClass>* pThis = (CButtonHelp<T,t_ButtonClass> *)lParam;
		TCHAR szClass[64];
		GetClassName(hwnd,szClass,64);
		if(lstrcmpi(szClass,_T("Button")) == 0)
		{
			LONG style = GetWindowLong(hwnd,GWL_STYLE);
			BOOL bRtn = (style & BS_CHECKBOX) == BS_CHECKBOX;
			bRtn = bRtn || (style & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX;
			bRtn = bRtn || (style & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON;
			bRtn = bRtn || (style & BS_GROUPBOX) == BS_GROUPBOX;
			bRtn = bRtn || (style & BS_RADIOBUTTON) == BS_RADIOBUTTON;
			if(!bRtn)//only pushbutton
			{
				return pThis->SubclassButton(hwnd);
			}
		}

		return TRUE;
	}

protected:
	//##ModelId=43538E420002
		CSimpleArray<t_ButtonClass *> m_arButtons;
};


#endif //__BUTTONHELP_H__
