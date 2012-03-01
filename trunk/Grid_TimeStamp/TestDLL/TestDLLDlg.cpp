// TestDLLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestDLL.h"
#include "TestDLLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef ITCFILTER_MORE_OUTPUT
#define ITCFILTER_MORE_OUTPUT  (long)(0x01690001)
#define ITCFILTER_TOO_BIG      (long)(0xC104001B)
#endif

// CTestDLLDlg dialog

CTestDLLDlg::CTestDLLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDLLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDLLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Data, CEDIT_Data);
	DDX_Control(pDX, IDC_EDIT_Separator, CEdit_SEP);
	DDX_Control(pDX, IDC_EDIT_Postamble, CEdit_POST);
	DDX_Control(pDX, IDC_EDIT_OUT, CEdit_Out);
	DDX_Control(pDX, IDC_EDIT_DATETIMESEP, CEdit_DateTimeSep);
	DDX_Control(pDX, IDC_EDIT_DATETIMESEP, CEdit_DateTimeSep);
}

BEGIN_MESSAGE_MAP(CTestDLLDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDLLDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestDLLDlg message handlers

BOOL CTestDLLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CEdit_Out.SetTabStops(50);
	CEDIT_Data.SetWindowTextW(L"daten");
	CEdit_SEP.SetWindowTextW(L"\\t123\\t");
	CEdit_POST.SetWindowTextW(L"\\t456\\n");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CTestDLLDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_TESTDLL_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_TESTDLL_DIALOG));
	}
}
#endif

typedef HRESULT (*PFN_ITCCompileFunc)( LPCWSTR szInput); // Compile
typedef HRESULT (*PFN_ITCFilterFunc)(					 // TimeStamp
	LPCWSTR szInput,        // [in] input data
	DWORD   nInputChars,    // [in] number of input data characters
	LPWSTR  szOutputBuffer, // [out] output data
	PDWORD  pnOutputChars   // number of characters in [in] szOutputBuffer, [out] output data 
    );

void CTestDLLDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//load lib
	CEdit_Out.SetWindowTextW(L"");
	HINSTANCE hLib = ::LoadLibrary(L"\\Grid_TimeStamp.dll");
	PFN_ITCCompileFunc CompileFunc = NULL;
	PFN_ITCFilterFunc FilterFunc=NULL;
	// TODO: Add your control notification handler code here
	if (hLib!=NULL){
		CompileFunc = (PFN_ITCCompileFunc) ::GetProcAddress(hLib, _T("Compile"));
		FilterFunc = (PFN_ITCFilterFunc) ::GetProcAddress(hLib, L"TimeStamp");
		if(CompileFunc!=NULL && FilterFunc!=NULL){
			TCHAR* szInput = new TCHAR[64];
			TCHAR* szSep = new TCHAR[64];
			TCHAR* szDateTimeSep = new TCHAR[64];
			TCHAR* szPost = new TCHAR[64];
			CEdit_SEP.GetWindowTextW(szSep, 64);
			CEdit_POST.GetWindowTextW(szPost, 64);
			CEdit_DateTimeSep.GetWindowTextW(szDateTimeSep, 64);
			if(wcslen(szDateTimeSep)>0 && wcslen(szSep)>0 && wcslen(szPost)>0)
				wsprintf(szInput, L"%s@%s@%s", szDateTimeSep, szSep, szPost);
			else if (wcslen(szDateTimeSep)==0 && wcslen(szSep)>0 && wcslen(szPost)>0)
				wsprintf(szInput, L"%s@%s", szSep, szPost);
			else if (wcslen(szDateTimeSep)==0 && wcslen(szSep)==0 && wcslen(szPost)>0)
				wsprintf(szInput, L"%s", szPost);
			else
				wsprintf(szInput, L"");

			HRESULT hRes = CompileFunc(szInput);
			if(hRes!=0){
				MessageBox(L"CompileFunc FAILED");
				return;
			}
			if(hRes==0){
				CEDIT_Data.GetWindowTextW(szInput, 64);
				
				TCHAR* szOut=new TCHAR[128];
				DWORD dwIn = wcslen(szInput)+sizeof(TCHAR);
				DWORD dwOut = 128;
				hRes = FilterFunc(szInput, dwIn, szOut, &dwOut);
				if(hRes!=0){
					if(hRes==ITCFILTER_TOO_BIG)
						MessageBox(L"FilterFunc returned: Filter to big");
					else if(hRes==ITCFILTER_MORE_OUTPUT)
						MessageBox(L"FilterFunc returned: ITCFILTER_MORE_OUTPUT");
					else 
						MessageBox(L"FilterFunc Failed");
					return;
				}
				else
				{
					TCHAR* szTemp= new TCHAR[dwOut+2];
					wsprintf(szTemp, L"'%s'", szOut);
					MessageBox(szTemp, L"FilterFunc OK");
					CEdit_Out.SetWindowTextW(szOut);
				}
			}
		}// if compile
		else
			MessageBox(L"Could not load functions of \\Grid_TimeStamp.dll", L"Missing Functions");
		FreeLibrary(hLib);
	}//if hLib
	else{
		MessageBox(L"Could not load \\Grid_TimeStamp.dll", L"Missing DLL");
	}
}
