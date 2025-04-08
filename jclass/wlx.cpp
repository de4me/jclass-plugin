#include "wlx.h"


#include <fstream>
#include <format>


#include "cWlxWindow.h"


const char detect_string[] = "EXT=\"CLASS\" & [0]=202 & [1]=254 & [2]=186 & [3]=190";


const TCHAR* class_string = _T("TCWLX-jclass");


LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {

    case DLL_PROCESS_ATTACH:
    {
        module.setHandle(hModule);
        const WNDCLASS wndclass = { 0, WndProc, 0, sizeof(PVOID), hModule, nullptr, nullptr, nullptr, nullptr, class_string};
        return RegisterClass(&wndclass) != 0;
        break;
    }

    case DLL_PROCESS_DETACH:
        UnregisterClass(class_string, hModule);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}


void WINAPI ListGetDetectString(char* DetectString, int maxlen) {

    if (maxlen < sizeof(detect_string)) {
        return;
    }
    strcpy_s(DetectString, maxlen, detect_string);
}


HWND WINAPI ListLoad(HWND ParentWin, char* FileToLoad, int ShowFlags) {

    auto handle = CreateWindow(class_string, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, 400, 300, ParentWin, nullptr, module.handle(), nullptr);
    if (handle == nullptr) {
        return nullptr;
    }
    
    auto wlx_window = new cWlxWindow(handle);
    if (wlx_window == nullptr) {
        DestroyWindow(handle);
        return nullptr;
    }

    if (wlx_window->handle() == nullptr) {
        delete wlx_window;
        DestroyWindow(handle);
        return nullptr;
    }

    SetWindowLongPtr(handle, 0, reinterpret_cast<LONG_PTR>(wlx_window));

    if (ListLoadNext(ParentWin, handle, FileToLoad, ShowFlags) == LISTPLUGIN_OK) {
        return handle;
    }

    DestroyWindow(wlx_window->handle());
    DestroyWindow(handle);
    delete wlx_window;
    return nullptr;
}


int WINAPI ListLoadNext(HWND ParentWin, HWND ListWin, char* FileToLoad, int ShowFlags) {

    try {
        cWlxWindow* wlx_window = reinterpret_cast<cWlxWindow*>(GetWindowLongPtr(ListWin, 0));
        if (wlx_window == nullptr) {
            return LISTPLUGIN_ERROR;
        }

        wlx_window->setFilePath(FileToLoad);
        wlx_window->setParentFocus();

        ifstream stream;

        stream.open(FileToLoad, ios::in | ios::binary | ios::ate);
        size_t file_size = static_cast<size_t>(stream.tellg());
        stream.seekg(0, ios::beg);
        wlx_window->read(stream, file_size);
        wlx_window->parse();
        return LISTPLUGIN_OK;
    }
    catch (...) {
        return LISTPLUGIN_ERROR;
    }

}


void WINAPI ListCloseWindow(HWND ListWin) {
    
    cWlxWindow* wlx_window = reinterpret_cast<cWlxWindow*>(GetWindowLongPtr(ListWin, 0));
    if (wlx_window != nullptr) {
        DestroyWindow(wlx_window->handle());
        delete wlx_window;
    }
    DestroyWindow(ListWin);
}


LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

#if _DEBUG
    auto message_text = format("{} {}\n", message, wParam);
    OutputDebugStringA(message_text.c_str());
#endif

    auto window = reinterpret_cast<cWlxWindow*>(GetWindowLongPtr(hWnd, 0));
    if (window == nullptr) {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

	switch (message)
	{

	case WM_KEYDOWN:
	{
		switch (wParam) {

		case VK_TAB:
		case VK_RIGHT:
			window->pageNext();
			return 0;

		case VK_LEFT:
			window->pagePrev();
			return 0;

        default:
            return 0;
		}
	}

	case WM_SIZE:
	{
		SIZE size = { LOWORD(lParam), HIWORD(lParam) };
		window->setSize(size);
		return 0;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}