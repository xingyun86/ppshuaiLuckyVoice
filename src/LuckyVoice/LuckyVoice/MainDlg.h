// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common\WzButton.h"
#include "Common\ButtonHelp.h"
#include "Common\BitmapBkgnd.h"
#include "Common\CtrlHollowColor.h"
#include "Common\PaintBkgnd.h"

#include <sapi.h>
#include <sphelper.h>

//sapi.lib在SDK的lib目录,必需正确配置
#pragma comment(lib,"sapi.lib")   
#pragma comment(lib, "winmm.lib")

#define SP_EVENT_WM_NOTIFY	WM_USER + 1

typedef struct tagVoiceConfig {
	USHORT uVolume;//音量(0 ~ 100)
	long lRate;//音调(-10 ~ 10)
	long lVoice;//声音类型
}VOICECONFIG, *PVOICECONFIG;

class CMainDlg : public CDialogImpl<CMainDlg>, 
				public CUpdateUI<CMainDlg>,
				public CMessageFilter, public CIdleHandler,
				//public CCtrlHollowColor<CMainDlg, RGB(64, 128, 64)>, 
				public CBitmapBkgnd<CMainDlg, IDB_BITMAP_BACKGROUND>,
				//public CPaintBkgnd<CMainDlg, RGB(192, 192, 0)>,
				public CButtonHelp<CMainDlg, CWzButtonImpl>
{
public:
	CMainDlg() 
	{
	}
	~CMainDlg() 
	{
	}

	enum { IDD = IDD_MAINDLG };
	enum SPEAKERVOICE_TYPE { SVT_DEFAULT = 0, SVT_FEMALE = 1, SVT_MALE = 2, SVT_OTHERS, };

	//这里使用typedef是有必要的，因为CHAIN_MSG_MAP宏会将“,”解释成模板参数之间的分隔符
	//typedef CCtrlHollowColor<CMainDlg, RGB(64, 128, 64)> CCtrlColoBase;
	typedef CBitmapBkgnd<CMainDlg, IDB_BITMAP_BACKGROUND> CBmpBkgndBase;
	//typedef CPaintBkgnd<CMainDlg, RGB(192, 192, 0)> CPaintBkgndBase;
	
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateChildWindows();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(SP_EVENT_WM_NOTIFY, OnSpEventNotify)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDC_BUTTON_WAVETOTEXT, OnWaveToText)
		COMMAND_ID_HANDLER(IDC_BUTTON_SPEAKTOTEXT, OnSpeakToText)
		COMMAND_ID_HANDLER(IDC_BUTTON_PLAYTHETEXT, OnPlayTheText)
		COMMAND_ID_HANDLER(IDC_BUTTON_TEXTTOWAVE, OnTextToWave)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		//CHAIN_MSG_MAP(CCtrlColoBase)
		CHAIN_MSG_MAP(CBmpBkgndBase)
		//CHAIN_MSG_MAP(CPaintBkgndBase)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		SubclassAllButtons();

		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		SetWindowText(_T("文本语音转换工具"));
		SetDlgItemText(IDC_BUTTON_SPEAKTOTEXT, _T("声音转文本"));
		SetDlgItemText(IDC_BUTTON_WAVETOTEXT, _T("语音转文本"));
		SetDlgItemText(IDC_BUTTON_PLAYTHETEXT, _T("播放文本"));	
		SetDlgItemText(IDC_BUTTON_TEXTTOWAVE, _T("文本转语音"));
		SetDlgItemText(IDCANCEL, _T("退出"));
		SetDlgItemText(ID_APP_ABOUT, _T("关于"));

		m_InputTextCtrl = GetDlgItem(IDC_EDIT_TEXT);
		m_VolumeCtrl = GetDlgItem(IDC_SLIDER_VOLUME);
		m_Voice = GetDlgItem(IDC_COMBO_VOICE);

		//m_VolumeCtrl.SetWindowLong(GWL_STYLE, m_VolumeCtrl.GetWindowLong(GWL_STYLE) | TBS_AUTOTICKS | WS_TABSTOP);
		::SetWindowLong(GetDlgItem(IDC_SLIDER_VOLUME), GWL_STYLE, ::GetWindowLong(GetDlgItem(IDC_SLIDER_VOLUME), GWL_STYLE) | TBS_AUTOTICKS | WS_TABSTOP);

		m_VolumeCtrl.ModifyStyle(0, m_VolumeCtrl.GetStyle());
		SetBkMode(m_VolumeCtrl.GetDC(), TRANSPARENT);
		
		SetBkColor(m_VolumeCtrl.GetDC(), RGB(120, 0, 0));
		m_VolumeCtrl.SetRange(0, 100);
		m_VolumeCtrl.SetTicFreq(10);
		m_VolumeCtrl.SetPos(60);
		
		m_Voice.InsertString(0, _T("默认"));
		m_Voice.InsertString(1, _T("女声"));
		m_Voice.InsertString(2, _T("男声"));
		m_Voice.SetCurSel(0);

		UIAddChildWindowContainer(m_hWnd);
		
		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
	
	LRESULT OnSpEventNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		HandleEvent(IDC_EDIT_TEXT, m_pSpRecoContext);
		return 0;
	}

	LRESULT OnSpeakToText(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		LaunchRecognition(this->m_hWnd);
		return 0;
	}
	
	LRESULT OnWaveToText(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CString strText(_T(""));
		_TCHAR tOpenFileName[MAX_PATH] = { 0 };
		if (SelectOpenFile(tOpenFileName))
		{
			HRESULT hResult = WaveFileToText(strText, tOpenFileName);
			if (FAILED(hResult))
			{
				ShowMsgs(_T("转换提示"), _T("语音文件转换文本失败!错误原因:%s"), ParseError(hResult));
			}
			else
			{
				SetDlgItemText(IDC_EDIT_TEXT, strText);
			}
		}
		return 0;
	}
	LRESULT OnPlayTheText(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code
		HRESULT hResult = S_FALSE;
		CString strInputText(_T(""));

		m_InputTextCtrl.GetWindowText(strInputText);
		
		m_VoiceConfig.uVolume = m_VolumeCtrl.GetPos();
		m_VoiceConfig.lRate = 0;
		m_VoiceConfig.lVoice = m_Voice.GetCurSel();

		hResult = PlayInputText(strInputText, &m_VoiceConfig);

		//CloseDialog(wID);
		return 0;
	}

	LRESULT OnTextToWave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//MessageBox(_T("功能正在开发中，后续需要收费!"), _T("转换提示"), MB_ICONINFORMATION);

		_TCHAR szOuputFileName[MAX_PATH] = { 0 };
		CString strInputText(_T(""));
		
		m_InputTextCtrl.GetWindowText(strInputText);
		if (strInputText.GetLength() <= 0 || !SelectSaveFile(szOuputFileName))
		{
			MessageBox(_T("输入文本为空或未设定保存文件!"), _T("转换提示"), MB_ICONINFORMATION);
			return FALSE;
		}

		SaveTextToWaveFile(szOuputFileName, strInputText);
		
		MessageBox(_T("文本转换语音文件完成!"), _T("转换提示"), MB_ICONINFORMATION);
		
		return TRUE;
	}
	
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
	
	void ShowMsgs(const _TCHAR * ptTitle, const _TCHAR * ptFormat, ...)
	{
		va_list arglist = 0;
		CString strText(_T(""));

		va_start(arglist, ptFormat);
		strText.FormatV(ptFormat, arglist);
		va_end(arglist);

		::MessageBox(NULL, strText, ptTitle, MB_ICONINFORMATION);
	}

	BOOL SelectSaveFile(_TCHAR(&tFileName)[MAX_PATH])
	{
		BOOL bResult = FALSE;
		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFilter = _T("Audio Files (*.WAV)\0*.WAV\0All Files (*.*)\0*.*\0\0");
		ofn.lpstrFile = tFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK |	OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
		bResult = GetSaveFileName(&ofn);
		if (bResult == FALSE)
		{
			//dwError = CommDlgExtendedError();
			//return bResult;
		}
		return bResult;
	}
	BOOL SelectOpenFile(_TCHAR(&tFileName)[MAX_PATH])
	{
		BOOL bResult = FALSE;
		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFilter = _T("Audio Files (*.WAV)\0*.WAV\0All Files (*.*)\0*.*\0\0");
		ofn.lpstrFile = tFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
		bResult = GetOpenFileName(&ofn);
		if (bResult == FALSE)
		{
			//dwError = CommDlgExtendedError();
			//return bResult;
		}
		return bResult;
	}
	HRESULT PlayInputText(CString strInputText, VOICECONFIG * pVoiceConfig)
	{
		HRESULT hResult = S_FALSE;
		_TCHAR szRequiredAttribs[MAXBYTE] = { 0 };
		CComPtr<ISpVoice> pSpVoice = NULL;
		CComPtr<ISpObjectToken> pSpObjectToken = NULL;
		
		//获取ISpVoice接口：
		hResult = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pSpVoice);
		if (SUCCEEDED(hResult))
		{
			//pVoice->SetInterest(SPFEI(SPEI_WORD_BOUNDARY), SPFEI(SPEI_WORD_BOUNDARY));
			//pVoice->SetNotifyWindowMessage(this->m_hWnd, WM_USER, 0, 0);

			//把音量设为100，音量范围是0～100
			hResult = pSpVoice->SetVolume(pVoiceConfig->uVolume);

			//标志音调（-10～10）
			hResult = pSpVoice->SetRate(pVoiceConfig->lRate);

			//设置发音
			//_TCHAR szRequiredAttribs[MAXBYTE] = _T("vendor=microsoft;gender=male;");
			//_TCHAR szRequiredAttribs[MAXBYTE] = _T("vendor=microsoft;gender=female;");
			//_TCHAR szRequiredAttribs[MAXBYTE] = _T("vendor=microsoft;vendorpreferred;");
			switch (pVoiceConfig->lVoice)
			{
			case SVT_DEFAULT:
				_sntprintf(szRequiredAttribs, sizeof(szRequiredAttribs) - 1, _T("vendor=microsoft;vendorpreferred;"));
				break;
			case SVT_FEMALE:
				_sntprintf(szRequiredAttribs, sizeof(szRequiredAttribs) - 1, _T("vendor=microsoft;gender=female;"));
				break;
			case SVT_MALE:
				_sntprintf(szRequiredAttribs, sizeof(szRequiredAttribs) - 1, _T("vendor=microsoft;gender=male;"));
				break;
			default:
				_sntprintf(szRequiredAttribs, sizeof(szRequiredAttribs) - 1, _T("vendor=microsoft;vendorpreferred;"));
				break;
			}

			hResult = SpFindBestToken(SPCAT_VOICES, szRequiredAttribs, _T(""), &pSpObjectToken);
			if (SUCCEEDED(hResult))
			{
				pSpVoice->SetVoice(pSpObjectToken);
			}
			/*IEnumSpObjectTokens *pSpEnumTokens = NULL;
			if (SUCCEEDED(SpEnumTokens(SPCAT_VOICES, _T("vendor=microsoft;"), NULL, &pSpEnumTokens)))
			{
			ISpObjectToken *pSpToken = NULL;

			while (SUCCEEDED(pSpEnumTokens->Next(1, &pSpToken, NULL)) &&
			pSpToken != NULL)
			{
			pSpVoice->SetVoice(pSpToken);
			pSpVoice->Speak(L"Hello World!", SPF_DEFAULT, NULL);

			pSpToken->Release();
			}

			pSpEnumTokens->Release();
			}*/
			hResult = pSpVoice->Speak(strInputText, SPF_DEFAULT, NULL);
		}

		return hResult;
	}

	HRESULT SaveTextToWaveFile(const _TCHAR * pOutputFileName, const _TCHAR * pInputTextData)
	{
		HRESULT hResult = S_FALSE;
		CComPtr<ISpVoice> pSpVoice = NULL;
		CComPtr<ISpStream> pSpWavStream = NULL;
		CSpStreamFormat * pSpStreamStreamNew = NULL;
		CComPtr<ISpStreamFormat> pSpStreamStreamOld = NULL;
		
		hResult = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pSpVoice);
		if (SUCCEEDED(hResult))
		{
			pSpStreamStreamNew = new CSpStreamFormat();
			pSpVoice->GetOutputStream(&pSpStreamStreamOld);
			hResult = pSpStreamStreamNew->AssignFormat(pSpStreamStreamOld);
			hResult = SPBindToFile(pOutputFileName, SPFM_CREATE_ALWAYS, &pSpWavStream, &pSpStreamStreamNew->FormatId(), pSpStreamStreamNew->WaveFormatExPtr());
			if (SUCCEEDED(hResult))
			{ 
				hResult = pSpVoice->SetOutput(pSpWavStream, TRUE);
				hResult = pSpVoice->Speak(pInputTextData, SPF_IS_XML, NULL);
			} 
		}

		if (pSpStreamStreamNew)
		{
			delete pSpStreamStreamNew;
			pSpStreamStreamNew = NULL;
		}

		return hResult;
	}

	static CString ParseError(DWORD dwErrorCodes, HINSTANCE hInstance = NULL)
	{
		BOOL bResult = FALSE;
		CString strErrorText(_T(""));
		HLOCAL hLocal = NULL;

		bResult = ::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			hInstance,
			dwErrorCodes,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
			(LPTSTR)&hLocal,
			0,
			NULL);
		if (!bResult)
		{
			if (hInstance)
			{
				bResult = ::FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_HMODULE |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					hInstance,
					dwErrorCodes,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
					(LPTSTR)&hLocal,
					0,
					NULL);
				if (!bResult)
				{
					// failed
					// Unknown error code %08x (%d)
					strErrorText.Format(_T("Unknown error code 0x%08X"), dwErrorCodes);
				}
			}
		}

		if (bResult && hLocal)
		{
			// success
			LPTSTR pT = StrStrI((LPCTSTR)hLocal, _T("\r"));
			if (pT != NULL)
			{
				//lose CRLF
				*pT = _T('\0');
			}
			strErrorText = (LPCTSTR)hLocal;
		}

		if (hLocal)
		{
			::LocalFree(hLocal);
			hLocal = NULL;
		}

		return strErrorText;
	}

	//获取声音文件数据的函数，ptWaveFileName参数指向要打开的声音文件
	UINT GetWaveTimeLength(const _TCHAR * ptWaveFileName)
	{
		UINT nResult = 0;
		//定义HMMIO文件句柄
		HMMIO hMMIO = NULL;
		//文件类型
		ULONG nDataType = 0;
		//文件时长
		ULONG nByteSize = 0;
		//定义PCMWAVEFORMAT结构对象，用来判断WAVE文件格式
		PCMWAVEFORMAT format = { 0 };

		//GetFileVersionInfo()
		hMMIO = ::mmioOpen((_TCHAR *)ptWaveFileName, NULL, MMIO_READWRITE);//以读写模式打开所给的WAVE文件； 
		if (hMMIO == NULL)
		{
			goto __LEAVE_CLEAN__;
		}
		
		//定位到WAVE文件的类型位置
		::mmioSeek(hMMIO, 8, SEEK_SET); 
		::mmioRead(hMMIO, (HPSTR)&nDataType, sizeof(nDataType));
		
		//判断该文件是否为"WAVE"文件格式
		if (memcmp(&nDataType, "WAVE", sizeof(nDataType))) 
		{
			goto __LEAVE_CLEAN__;
		}

		::mmioSeek(hMMIO, 20, SEEK_SET);
		//对打开的文件进行定位，此时指向WAVE文件的PCMWAVEFORMAT结构的数据
		::mmioRead(hMMIO, (HPSTR)&format, sizeof(PCMWAVEFORMAT));

		//获取该结构的数据
		//获取WAVE文件的声音数据的大小
		::mmioSeek(hMMIO, 42, SEEK_SET);
		::mmioRead(hMMIO, (HPSTR)&nByteSize, sizeof(nByteSize));

		//计算文件时长
		nResult = nByteSize / format.wf.nAvgBytesPerSec;

		::mmioClose(hMMIO, 0);
		hMMIO = 0;

	__LEAVE_CLEAN__:

		return nResult;
	}

	HRESULT LaunchRecognition(HWND hWnd)
	{
		HRESULT hResult = S_FALSE;
		CSpEvent * pSpEvent = NULL;
		BOOL bEndStreamReached = FALSE;
		CComPtr<ISpStream> pSpStream = NULL;
		CSpStreamFormat * pSpStreamFormat = NULL;
		CComPtr<ISpRecognizer> pSpRecognizer = NULL;
		//CComPtr<ISpRecoContext> pSpRecoContext = NULL;
		CComPtr<ISpRecoGrammar> pSpRecoGrammar = NULL;
		ULONGLONG ullGramId = 0x00000001;
		const ULONGLONG ullInterest = SPFEI(SPEI_SOUND_START) | 
			SPFEI(SPEI_SOUND_END) |	SPFEI(SPEI_PHRASE_START) | 
			SPFEI(SPEI_RECOGNITION) | SPFEI(SPEI_FALSE_RECOGNITION) | 
			SPFEI(SPEI_HYPOTHESIS) | SPFEI(SPEI_INTERFERENCE) | 
			SPFEI(SPEI_RECO_OTHER_CONTEXT) | SPFEI(SPEI_REQUEST_UI) | 
			SPFEI(SPEI_RECO_STATE_CHANGE) | SPFEI(SPEI_PROPERTY_NUM_CHANGE) | 
			SPFEI(SPEI_PROPERTY_STRING_CHANGE);

		hResult = pSpRecognizer.CoCreateInstance(CLSID_SpSharedRecognizer);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		m_pSpRecoContext.Release();
		m_pSpRecoContext = NULL;
		hResult = pSpRecognizer->CreateRecoContext(&m_pSpRecoContext);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		hResult = m_pSpRecoContext->SetNotifyWindowMessage(hWnd, SP_EVENT_WM_NOTIFY, 0, 0);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		hResult = m_pSpRecoContext->SetInterest(ullInterest, ullInterest);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		hResult = m_pSpRecoContext->CreateGrammar(ullGramId, &pSpRecoGrammar);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		hResult = pSpRecoGrammar->LoadDictation(0, SPLO_STATIC);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		hResult = pSpRecoGrammar->SetDictationState(SPRS_ACTIVE);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

	__LEAVE_CLEAN__:

		return hResult;
	}

	void HandleEvent(UINT nID, CComPtr<ISpRecoContext> & pSpRecoContext)
	{
		HRESULT hResult = S_FALSE;
		CString strTextOld(_T(""));
		CString strTextNew(_T(""));
		CSpEvent * pSpEvent = NULL;
		
		pSpEvent = new CSpEvent();

		// Loop processing events while there are any in the queue
		while(SUCCEEDED(pSpEvent->GetFrom(pSpRecoContext)))
		{
			if (pSpEvent->eEventId == SPEI_UNDEFINED)
			{
				break;	
			}
			switch (pSpEvent->eEventId)
			{
			case SPEI_UNDEFINED:
				break;
			case SPEI_HYPOTHESIS:
				{
					strTextOld = _T("");
					GetDlgItemText(nID, strTextOld);
					hResult = ParseInput(strTextNew, pSpEvent);
					if (SUCCEEDED(hResult))
					{
						strTextOld.AppendFormat(_T("\r\n%s"), strTextNew);
						SetDlgItemText(nID, strTextOld);
					}
					else
					{
						ShowMsgs(_T("转换提示"), _T("转换出错！错误原因：%s"), ParseError(hResult));
					}
				}
				break;
			}
		}

		if (pSpEvent)
		{
			delete pSpEvent;
			pSpEvent = NULL;
		}
	}
	HRESULT ParseInput(CString & strRecognizeText, CSpEvent * pSpEvent)
	{
		LPWSTR lpwText = NULL;
		HRESULT hResult = S_FALSE;
		SPPHRASE * pSpPhrase = NULL;
		CComPtr<ISpRecoResult> pSpRecoResult = NULL;

		pSpRecoResult = pSpEvent->RecoResult();

		hResult = pSpRecoResult->GetPhrase(&pSpPhrase);

		if (SUCCEEDED(hResult))
		{
			// Get the phrase's entire text string, including replacements.
			hResult = pSpRecoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &lpwText, NULL);
			if (SUCCEEDED(hResult))
			{
				strRecognizeText.Append(lpwText);
			}
		}

		if (lpwText)
		{
			::CoTaskMemFree(lpwText);
			lpwText = NULL;
		}
		if (pSpPhrase)
		{
			::CoTaskMemFree(pSpPhrase);
			pSpPhrase = NULL;
		}
		return hResult;
	}

	HRESULT WaveFileToText(CString & strRecognizeText, const _TCHAR * ptWaveAudioFileName)
	{
		HRESULT hResult = S_FALSE;
		CSpEvent * pSpEvent = NULL;
		BOOL bEndStreamReached = FALSE;
		CComPtr<ISpStream> pSpStream = NULL;
		CSpStreamFormat * pSpStreamFormat = NULL;
		CComPtr<ISpRecognizer> pSpRecognizer = NULL;
		CComPtr<ISpRecoContext> pSpRecoContext = NULL;
		CComPtr<ISpRecoGrammar> pSpRecoGrammar = NULL;

#define MY_REASONABLE_TIMEOUT 3000

		// Create basic SAPI stream object
		// NOTE: The helper SpBindToFile can be used to perform the following operations
		hResult = pSpStream.CoCreateInstance(CLSID_SpStream);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		pSpStreamFormat = new CSpStreamFormat();
		// generate WaveFormatEx structure, assuming the wav format is 22kHz, 16-bit, Stereo
		hResult = pSpStreamFormat->AssignFormat(SPSF_22kHz16BitStereo);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// setup stream object with wav file MY_WAVE_AUDIO_FILENAME
		//   for read-only access, since it will only be access by the SR engine
		//const GUID guid = sInputFormat.FormatId();
		hResult = pSpStream->BindToFile(ptWaveAudioFileName, SPFM_OPEN_READONLY, &pSpStreamFormat->FormatId(), pSpStreamFormat->WaveFormatExPtr(), SPFEI_ALL_EVENTS);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// Create in-process speech recognition engine
		hResult = pSpRecognizer.CoCreateInstance(CLSID_SpInprocRecognizer);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// connect wav input to recognizer
		// SAPI will negotiate mismatched engine/input audio formats using system audio codecs, so second parameter is not important - use default of TRUE
		hResult = pSpRecognizer->SetInput(pSpStream, TRUE);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// Create recognition context to receive events
		hResult = pSpRecognizer->CreateRecoContext(&pSpRecoContext);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// Create grammar, and load dictation
		// ignore grammar ID for simplicity's sake
		// NOTE: Voice command apps would load CFG here
		hResult = pSpRecoContext->CreateGrammar(NULL, &pSpRecoGrammar);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		hResult = pSpRecoGrammar->LoadDictation(NULL, SPLO_STATIC);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// check for recognitions and end of stream event
		hResult = pSpRecoContext->SetInterest(SPFEI(SPEI_RECOGNITION) | SPFEI(SPEI_END_SR_STREAM), SPFEI(SPEI_RECOGNITION) | SPFEI(SPEI_END_SR_STREAM));
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// use Win32 events for command-line style application
		hResult = pSpRecoContext->SetNotifyWin32Event();
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// activate dictation, and begin recognition
		hResult = pSpRecoGrammar->SetDictationState(SPRS_ACTIVE);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// while events occur, continue processing
		// timeout should be greater than the audio stream length, or a reasonable amount of time expected to pass before no more recognitions are expected in an audio stream
		pSpEvent = new CSpEvent();
		while (!bEndStreamReached && SUCCEEDED(pSpRecoContext->WaitForNotifyEvent(GetWaveTimeLength(ptWaveAudioFileName) * 1000)))
		{
			// pull all queued events from the reco context's event queue
			while (!bEndStreamReached && SUCCEEDED(pSpEvent->GetFrom(pSpRecoContext)))
			{
				// Check event type
				switch (pSpEvent->eEventId)
				{
					// speech recognition engine recognized some audio
				case SPEI_RECOGNITION:
					// TODO: log/report recognized text
					ParseInput(strRecognizeText, pSpEvent);
					break;

					// end of the wav file was reached by the speech recognition engine
				case SPEI_END_SR_STREAM:
					bEndStreamReached = TRUE;
					break;
				}

				// clear any event data/object references
				pSpEvent->Clear();
			}// END event pulling loop - break on empty event queue OR end stream
		}// END event polling loop - break on event timeout OR end stream

		if (pSpEvent)
		{
			delete pSpEvent;
			pSpEvent = NULL;
		}

		if (pSpStreamFormat)
		{
			delete pSpStreamFormat;
			pSpStreamFormat = NULL;
		}

		// deactivate dictation
		hResult = pSpRecoGrammar->SetDictationState(SPRS_INACTIVE);
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// unload dictation topic
		hResult = pSpRecoGrammar->UnloadDictation();
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

		// close the input stream, since we're done with it
		// NOTE: smart pointer will call SpStream's destructor, and consequently ::Close, but code may want to check for errors on ::Close operation
		hResult = pSpStream->Close();
		if (FAILED(hResult))
		{
			goto __LEAVE_CLEAN__;
		}

	__LEAVE_CLEAN__:

		return hResult;
	}

private:
	CEditT<CWindow> m_InputTextCtrl;
	CTrackBarCtrlT<CWindow> m_VolumeCtrl;
	CComboBoxT<CWindow> m_Voice;

	VOICECONFIG m_VoiceConfig;

	CComPtr<ISpRecoContext> m_pSpRecoContext;
};
