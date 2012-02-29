// TestDLLDlg.h : header file
//

#pragma once
#include "afxwin.h"

// CTestDLLDlg dialog
class CTestDLLDlg : public CDialog
{
// Construction
public:
	CTestDLLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTDLL_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit CEDIT_Data;
	CEdit CEdit_SEP;
	CEdit CEdit_POST;
	CEdit CEdit_Out;
	CEdit CEdit_DateTimeSep;
};
