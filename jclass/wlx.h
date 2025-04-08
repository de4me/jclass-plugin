#pragma once


#include <Windows.h>


#ifdef JCLASS_EXPORTS
#define JCLASS_API __declspec(dllexport)
#else
#define JCLASS_API __declspec(dllimport)
#endif


enum LISTPLUGIN {

	LISTPLUGIN_OK = 0,
	LISTPLUGIN_ERROR = 1,
};


enum LCPFLAG {

	LCP_WRAPTEXT = 1,
	LCP_FITTOWINDOW = 2,
	LCP_ANSI = 4,
	LCP_ASCII = 8,
	LCP_VARIABLE = 12,
	LCP_FORCESHOW = 16,
	LCP_FITLARGERONLY = 32,
	LCP_CENTER = 64,
	LCP_DARKMODE = 128,
	LCP_DARKMODENATIVE = 256,
};


enum LCFLAG {

	LC_COPY = 1,
	LC_NEWPARAMS = 2,
	LC_SELECTALL = 3,
	LC_SETPERCENT = 4,
};


enum LCSFLAG {

	LCS_FINDFIRST = 1,
	LCS_MATCHCASE = 2,
	LCS_WHOLEWORDS = 4,
	LCS_BACKWARDS = 8,
};


enum ITMCOMMAND {

	ITM_PERCENT = 0XFFFE,
	ITM_FONTSTYLE = 0XFFFD,
	ITM_WRAP = 0XFFFC,
	ITM_FIT = 0XFFFB,
	ITM_NEXT = 0XFFFA,
};


typedef struct {

	int size;
	DWORD PluginInterfaceVersionLow;
	DWORD PluginInterfaceVersionHi;
	char DefaultIniName[MAX_PATH];
} ListDefaultParamStruct;


extern "C" {

	JCLASS_API void WINAPI ListGetDetectString(char* DetectString, int maxlen);

	JCLASS_API HWND WINAPI ListLoad(HWND ParentWin, char* FileToLoad, int ShowFlags);
	JCLASS_API int WINAPI ListLoadNext(HWND ParentWin, HWND ListWin, char* FileToLoad, int ShowFlags);

	JCLASS_API void WINAPI ListCloseWindow(HWND ListWin);

	//JCLASS_API int WINAPI ListSendCommand(HWND ListWin, int Command, int Parameter);
	//JCLASS_API int WINAPI ListNotificationReceived(HWND ListWin, int Message, WPARAM wParam, LPARAM lParam);

	//JCLASS_API int WINAPI ListSearchText(HWND ListWin, char* SearchString, int SearchParameter);
	//JCLASS_API int WINAPI ListPrint(HWND ListWin, char* FileToPrint, char* DefPrinter, int PrintFlags, RECT* Margins);
	
	//JCLASS_API void WINAPI ListSetDefaultParams(ListDefaultParamStruct* dps);
	//JCLASS_API HBITMAP WINAPI ListGetPreviewBitmap(char* FileToLoad, int width, int height, char* contentbuf, int contentbuflen);
}