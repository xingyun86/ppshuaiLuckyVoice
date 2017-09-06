// LuckyVoice.cpp : main source file for LuckyVoice.exe
//

#include "stdafx.h"

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	int nResult = 0;
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	//COM初始化
	if (SUCCEEDED(::CoInitialize(NULL)))
	{
		CMainDlg dlgMain;

		if (dlgMain.Create(NULL))
		{
			dlgMain.ShowWindow(nCmdShow);

			nResult = theLoop.Run();

			//释放析构
			::CoUninitialize();
		}
		else
		{
			ATLTRACE(_T("Main dialog creation failed!Error(0x%X)\n"), GetLastError());
		}
	}
	else
	{
		ATLTRACE(_T("CoInitialize() failed!Error(0x%X)\n"), GetLastError());
	}

	_Module.RemoveMessageLoop();

	return nResult;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
