#pragma once
#include "util/window.h"
namespace newobj
{
#ifdef _WIN32
	// ÏµÍ³ä¯ÀÀÆ÷ Windows=IE
	class browser
	{
	public:
		browser(); 
		~browser();
		void create(nstring url,int32 width,int32 height,ptr_win parent = 0); 
		ptr_win open();
		void refresh(const nstring& url);
	public:
		static nstring m_url;
		static int32 m_width;
		static int32 m_height;
		static void* m_iWebBrowser;
	private:

		void* m_browser;
		void* m_container;
		ptr_win m_handle;
		
		int32 m_parent;
		
	};
#endif
}