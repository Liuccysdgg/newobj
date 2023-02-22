#include "browser.h"
#ifdef _WIN32
#include <Windows.h>
#include <atlbase.h>  
CComModule _Module;
#include <atlwin.h>
#include <windows.h>
#include "codec.h"
#include "forstring.h"
DEFINE_GUID(CGID_DocHostCommandHandler, 0xf38bc242, 0xb950, 0x11d1, 0x89, 0x18, 0x00, 0xc0, 0x4f, 0xc2, 0xc8, 0x36);

nstring newobj::browser::m_url = "";
int32 newobj::browser::m_height = 0;
int32 newobj::browser::m_width = 0;
void* newobj::browser::m_iWebBrowser = nullptr;
newobj::browser::browser()
{
	m_browser = nullptr;
	m_container = nullptr;
	m_handle = 0;
	m_width = 0;
	m_height = 0;
	m_parent = 0;
}

newobj::browser::~browser()
{
}

void newobj::browser::create(nstring url, int32 width, int32 height, ptr_win parent)
{
	m_url = url;
	m_width = width;
	m_height = height;
	m_parent = parent;
}

LRESULT WINAPI Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	
	VARIANT varMyURL;
	static CAxWindow WinContainer;
	LPOLESTR pszName = (LPOLESTR)OLESTR("shell.Explorer.2");
	//GetClientRect(hWnd, &rc);
	rc.left = 0;
	rc.top = 0;
	rc.bottom = newobj::browser::m_height;
	rc.right = newobj::browser::m_width;
	switch (message)
	{
	case WM_CREATE:
	{
		WinContainer.Create(hWnd, rc, 0, WS_CHILD | WS_VISIBLE);
		WinContainer.CreateControl(pszName);

		WinContainer.QueryControl(__uuidof(IWebBrowser2), (void**)&newobj::browser::m_iWebBrowser);
		VariantInit(&varMyURL);
		varMyURL.vt = VT_BSTR;
		((IWebBrowser2*)newobj::browser::m_iWebBrowser)->put_Silent(TRUE);


		//const wchar_t* arr = forstring::to_wstring("file:///Z:/kws/Index.htm").c_str();

		//varMyURL.bstrVal = SysAllocString(forstring::to_wstring(newobj::).c_str());
		varMyURL.bstrVal = SysAllocString((OLECHAR*)forstring::to_wstring(newobj::browser::m_url).c_str());
		((IWebBrowser2*)newobj::browser::m_iWebBrowser)->Navigate2(&varMyURL, 0, 0, 0, 0);
		VariantClear(&varMyURL);
		((IWebBrowser2*)newobj::browser::m_iWebBrowser)->Release();
		
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return (int)DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
ptr_win newobj::browser::open()
{

	static TCHAR szAppName[] = TEXT("WebBrowser");
	static TCHAR szClassName[] = TEXT("WebBrowser");
	HWND hWnd;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = Callback;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	/*wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);*/
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szClassName;
	if (!RegisterClass(&wndclass))
		return 0;
	hWnd = CreateWindowA(szClassName, szAppName, WS_OVERLAPPEDWINDOW, 0, 0, m_width, m_height, (HWND)m_parent, NULL, 0, NULL);
	m_handle = (ptr_win)hWnd;
	ShowWindow(hWnd, 0);
	UpdateWindow(hWnd);
	return (ptr_win)hWnd;
}
void newobj::browser::refresh(const nstring& url)
{
	VARIANT varMyURL;
	VariantInit(&varMyURL);
	varMyURL.vt = VT_BSTR;
	varMyURL.bstrVal = SysAllocString((OLECHAR*)forstring::to_wstring(url).c_str());
	((IWebBrowser2*)newobj::browser::m_iWebBrowser)->Navigate2(&varMyURL, 0, 0, 0, 0);
}
#endif
