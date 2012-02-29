// Grid_TimeStamp.cpp : Defines the entry point for the DLL application.
//

#define USE_GETTIMEDATE_API

#include <windows.h>
#include <string>

#include "ITCCustomFilter.h"

#ifndef ITCFILTER_SUCCESS
#define ITCFILTER_SUCCESS S_OK
#endif

static TCHAR* asciiTable[] = { 
	L"\x00",L"\x01",L"\x02",L"\x03",
	L"\x04",L"\x05",L"\x06",L"\x09",
	L"\x0A",L"\x0B",L"\x0C",L"\x0D",
	L"\x0E",L"\x0F",L"\x10",L"\x11",
	L"\x12",L"\x13",L"\x14",L"\x15",
	L"\x16",L"\x17",L"\x18",L"\x19",
	L"\x1A",L"\x1B",L"\x1C",L"\x1D",
	L"\x1E",L"\x1F",
};

void myReplace(std::wstring& str, const std::wstring& oldStr, const std::wstring& newStr)
{
  size_t pos = 0;
  while((pos = str.find(oldStr, pos)) != std::wstring::npos)
  {
     str.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
}

TCHAR * stringReplace(TCHAR *search, TCHAR *replace, TCHAR *string) {
	std::wstring str (string);
	std::wstring strReplace(replace);
	std::wstring strSearch(search);
	myReplace(str, strSearch, strReplace);
	
	//cut off, if string is to long
	if(str.length() > 63){
		std::wstring str2 = str.substr(0, 63);
		wsprintf(string, L"%s", str2.c_str());
	}
	else
		wsprintf(string, L"%s", str.c_str());
	return string;
}

// Persistent storage for the separator
static WCHAR s_szDateTimeSeparator[64] = {0};
// Persistent storage for the separator
static WCHAR s_szSeparator[64] = {0};
// Persistent storage for the postamble
static WCHAR s_szPostamble[64] = {0};

// Minimal DllMain
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    return TRUE;
}

///// COMPILE STEP.
// Compile is any setup that might be needed before filtering or editing.
// It is called once whenever the grid setting is changed with SSAPI or
// Intermec Settings.
//
// For this sample, the following grid expression can be compiled:
//   <CDEF>\cf.dll|FilterEdit|Compile|postamblestring
//     where 'postamblestring' can be any string.
//     Note that cf.dll must be deployed to the device's root folder.
//
// Compile a filter.
// Return values:
//  ITCFILTER_SUCCESS indicates success.
//  Any negative code indicates error.
//
ITCFILTER_API Compile( LPCWSTR szInput ){ 
	// The 3rd parameter in the configured filter / edit expression.
	// For this example, it's the separator and postamble that will be added to the reversed label.
    // If a separator and postamble was sent in, save it.
    // Also, protect ourselves from an incoming NULL pointer.
	TCHAR* szTemp = new TCHAR[64];
	memset(szTemp, 0, sizeof(TCHAR)*64);

	TCHAR* pszInput = new TCHAR[64];
	memset(pszInput, 0, sizeof(TCHAR)*64);
	
	wcsncpy(pszInput, szInput, 63);
	
	memset(s_szPostamble, 0, sizeof(TCHAR)*64);
	memset(s_szSeparator, 0, sizeof(TCHAR)*64);
	memset(s_szDateTimeSeparator, 0, sizeof(TCHAR)*64);

	szTemp = stringReplace(L"\\t", L"\t", pszInput);
	wcscpy(pszInput, szTemp);
	szTemp = stringReplace(L"\\r", L"\r", pszInput);
	wcscpy(pszInput, szTemp);
	szTemp = stringReplace(L"\\n", L"\n", pszInput);

	wcscpy(pszInput, szTemp);
	TCHAR* newInput = new TCHAR[64];
	wsprintf(newInput, pszInput);
	memset(szTemp, 0, 64 * sizeof(TCHAR));

	//split string at '@'
	int iTokenCount=0;
	TCHAR sTokens[3][64];// = new TCHAR[3][64]; //temporary storage
	TCHAR* wToken = wcstok(newInput, L"@");
	if(wToken!=NULL){
		iTokenCount++; //number of tokens
		wsprintf(sTokens[iTokenCount-1], L"%s", wToken);
		//wsprintf(s_szSeparator, wToken);
		wToken = wcstok(NULL, L"@");
		if(wToken!=NULL){
			iTokenCount++;
			wsprintf(sTokens[iTokenCount-1], L"%s", wToken);
			//wsprintf(s_szPostamble, wToken);
			wToken = wcstok(NULL, L"@");
			if(wToken!=NULL){
				iTokenCount++;
				wsprintf(sTokens[iTokenCount-1], L"%s", wToken);
				//wsprintf(s_szPostamble, wToken);
			}
		}
		wsprintf(s_szPostamble, L" ");
		wsprintf(s_szSeparator, L" ");
		wsprintf(s_szDateTimeSeparator, L" ");
		switch(iTokenCount){
			case 0:
				break;
			case 1:
				wsprintf(s_szPostamble, sTokens[0]);
				break;
			case 2:
				wsprintf(s_szPostamble, sTokens[0]);
				wsprintf(s_szSeparator, sTokens[1]);
				break;
			case 3:
				wsprintf(s_szPostamble, sTokens[0]);
				wsprintf(s_szSeparator, sTokens[1]);
				wsprintf(s_szDateTimeSeparator, sTokens[2]);
				break;
			default://more tokens?, only first 3 will be used
				wsprintf(s_szPostamble, sTokens[0]);
				wsprintf(s_szSeparator, sTokens[1]);
				wsprintf(s_szDateTimeSeparator, sTokens[2]);
				break;
		}
	}
	//CleanUp
	delete(szTemp);
	delete(pszInput);
	delete(newInput);
	return ITCFILTER_SUCCESS;
}

// Match data to a filter.
// Return values:
//  S_OK (zero) indicates success. szOutputBuffer contains the output.
//    If the function set *pnOutputChars to zero then there is no output.
//  ITCFILTER_MORE_OUTPUT indicates that szOutputBuffer contains the output.
//    Also causes this function to be called again with zero input to get
//    other output buffer.
//  ITCFILTER_TOO_BIG indicates that the output is larger than nOutputBufferChars.
//    The required output buffer characters is saved in *pnOutputChars.
//  Other negative code indicates error.
//   
//
ITCFILTER_API TimeStamp (
	LPCWSTR szInput,        // [in] input data
	DWORD   nInputChars,    // [in] number of input data characters
	LPWSTR  szOutputBuffer, // [out] output data
	PDWORD  pnOutputChars   // number of characters in [in] szOutputBuffer, [out] output data 
	)
	{
		HRESULT hr = S_OK;
		TCHAR* sTimeStamp = new TCHAR[18];
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);

		//format datetime to 'YYYYMMDD hh:mm:ss'
#ifdef USE_GETTIMEDATE_API
		TCHAR* pszDateFormat = NULL;
		TCHAR* pszTimeFormat = L"HH:mm:ss";
		TCHAR* pszDate = new TCHAR[64];
		TCHAR* pszTime = new TCHAR[64];

		LCID lc = LOCALE_USER_DEFAULT;
		int nLen = 64;
		int nLen2 = 64;
		nLen = GetDateFormatW(lc, // Use this thread's
			// locale.
			DATE_SHORTDATE, // Flags are passed into this function.
			NULL, // Use current system time.
			pszDateFormat, // format selected by user
			pszDate, // used for formatted date
			nLen); // used for date buffer
		if(nLen!=0)
			wsprintf(sTimeStamp, L"%s", pszDate);
		nLen2 = 64;
		nLen2 = GetTimeFormatW(lc, // Use this thread's
			// locale.
			TIME_FORCE24HOURFORMAT, // Flags are passed into this function.
			NULL, // Use current system time.
			pszTimeFormat, // format selected by user
			pszTime, // used for formatted date
			nLen2); // used for date buffer
		if(nLen2!=0)
			wsprintf(sTimeStamp, L"%s", pszTime);
		if(nLen>0 && nLen2>0)
			wsprintf(sTimeStamp, L"%s %s", pszDate, pszTime);
		else
			wsprintf(sTimeStamp, L"%04i%02i%02i%s%02i:%02i:%02i", 
				sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				s_szDateTimeSeparator,
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
#else
		wsprintf(sTimeStamp, L"%04i%02i%02i%s%02i:%02i:%02i", 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay,
			s_szDateTimeSeparator,
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
#endif
		DWORD nCharsNeeded = wcslen(sTimeStamp) + wcslen(s_szSeparator) + nInputChars + wcslen(s_szPostamble);
        // First check again to make sure we're provided enough space
        if( nCharsNeeded > *pnOutputChars )
        {
            hr = ITCFILTER_TOO_BIG;
        }
        // Enough space, so do the work
        else
        {
			memset(szOutputBuffer, 0, *pnOutputChars);
			wsprintf(szOutputBuffer, L"%s%s%s%s", sTimeStamp, s_szSeparator, szInput, s_szPostamble);
		}
		*pnOutputChars = nCharsNeeded;
#ifdef USE_GETTIMEDATE_API
		delete pszDate;
		delete pszTime;
#endif
		return hr;
	}