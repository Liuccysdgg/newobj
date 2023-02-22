#include "window.h"
#include "forstring.h"
#include "basetype/string.h"
#include "public/buffer.h"
#include <memory>
#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#include "util/map.hpp"
#include "util/process.h"
ptr_win newobj::window::parent(ptr_win handle)
{
	return (ptr_win)GetParent((HWND)handle);
}
nstring newobj::window::class_name(ptr_win handle)
{

	char name[255];
	memset(name, 0, 255);
	GetClassNameA((HWND)handle,name,255);
	return name;
}

nstring newobj::window::title_name(ptr_win handle)
{
	//wchar_t name[2048];
	//memset(name, 0, sizeof(name));
	//GetWindowTextW((HWND)handle,name, GetWindowTextLengthW((HWND)handle)*2+1);
	//std::wstring ret = name;
	//return str::WStringToStr(ret);
	char name[2048];
	memset(name, 0, sizeof(name));
	GetWindowTextA((HWND)handle, name, GetWindowTextLengthA((HWND)handle)+1);
	return name;
}
bool newobj::window::title_name(ptr_win handle, const nstring& name)
{
	return SetWindowTextA((HWND)handle,name.c_str());
}
std::vector<ptr_win> newobj::window::find(ptr_win parent, const nstring& class_name, const nstring& title_name)
{
	std::vector<ptr_win> list;
	HWND win_h = 0;
	if (parent == 0)
		parent = (ptr_win)GetDesktopWindow();
	win_h = GetWindow((HWND)parent,5);
	bool ok;
	while (win_h != 0)
	{
		ok = true;
		if (class_name.length() == 0 && title_name.length() == 0)
			list.push_back((ptr_win)win_h);
		else
		{
			if (class_name.length() != 0)
			{
				auto curr_cn = ::window::class_name((ptr_win)win_h);
				if (curr_cn.find(class_name) != -1)
				{
					if (title_name.length() != 0)
					{
						auto curr_tn = ::window::title_name((ptr_win)win_h);
						if (curr_tn.find(title_name) == -1)
							goto _CONTINUE;
						else
							list.push_back((ptr_win)win_h);
					}
					else
						list.push_back((ptr_win)win_h);
				}
			}
			else
			{
				if (title_name.length() != 0)
				{
					auto curr_tn = newobj::window::title_name((ptr_win)win_h);
					if (curr_tn.find(title_name) == -1)
						goto _CONTINUE;
					else
						list.push_back((ptr_win)win_h);
				}
			}
		}
_CONTINUE:
		win_h = GetWindow(win_h, 2);
	}
	return list;
}
ptr_win newobj::window::find(const nstring& class_name, const nstring& title_name)
{
	return (ptr_win)FindWindowA(class_name.c_str(),title_name.c_str());
}

ptr_win newobj::window::find(const nstring& path, const nstring& class_name, const nstring& title_name)
{
	auto win =  find(0,class_name.c_str(), title_name.c_str());
	if (win.size() == 0)
		return 0;
	for (uint32 i = 0; i < win.size(); i++)
	{
		if (forstring::en_case(newobj::window::getpath(win[i]), false) == forstring::en_case(path, false))
			return win[i];
	}
	return 0;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	struct Ewps
	{
		int loop;
		nstring class_name;
		nstring title_name;
		newobj::map<ptr_win,bool> list;
		bool child;
	};
	Ewps* ewps = (Ewps*)lParam;
	if (hwnd == 0)
		ewps->loop++;
	if (ewps->loop > 30000)
		return FALSE;

	auto class_name = newobj::window::class_name((ptr_win)hwnd);
	auto title_name = newobj::window::title_name((ptr_win)hwnd);
	
	if (ewps->class_name == "" || class_name == ewps->class_name)
	{
		if (ewps->title_name == "" || title_name == ewps->title_name)
			ewps->list.add((ptr)hwnd, false);
	}
		
	if(ewps->child)
		EnumChildWindows(hwnd, EnumWindowsProc, (LPARAM)ewps);

	return TRUE;
}


std::vector<ptr_win> newobj::window::childs(ptr_win parent, const nstring& class_name, const nstring& title_name, bool c)
{
	struct Ewps
	{
		Ewps()
		{
			loop = 0;
			child = false;
		}
		int loop;
		nstring class_name;
		nstring title_name;
		newobj::map<ptr_win,bool> list;
		bool child;
	};
	std::shared_ptr<Ewps> ewps(new Ewps);
	ewps->loop = 0;
	ewps->class_name = class_name;
	ewps->title_name = title_name;
	ewps->child = c;
	EnumChildWindows((HWND)parent, EnumWindowsProc, (LPARAM)ewps.get());
	std::vector<ptr_win> result;
	auto stdmap = ewps->list.to_stl();
	for_iter(iter,stdmap)
	{
		result.push_back(iter->first);
	}
	return result;
}
ptr_win newobj::window::childs_signal(ptr_win parent, const nstring& class_name, const nstring& title_name, bool childs)
{
	auto list = newobj::window::childs(parent,class_name,title_name, childs);
	t_ret_var(list.size() != 1, 0);
	return list[0];
}
nstring newobj::window::getpath(ptr_win handle)
{
	nstring filepath;
#if 0
	DWORD proc_id;
	GetWindowThreadProcessId((HWND)handle,&proc_id);
	if (proc_id > 0)
	{
		auto proc_handle = OpenProcess(1024|16,0,proc_id);
		if (proc_handle > 0)
		{
			HMODULE m_array[255];
			DWORD need_mem;
			if (EnumProcessModulesEx(proc_handle, m_array, 256, &need_mem,LIST_MODULES_ALL))
			{
				char mod_filepath[512];
				memset(mod_filepath, 0, 512);
				GetModuleFileNameExA(proc_handle,m_array[0],mod_filepath,256);
				String tstr = mod_filepath;
				if (!tstr.ToStdString().find("\\??\\") != -1)
					filepath = tstr.Remove("\\??\\").ToStdString();
				else
					filepath = tstr.ToStdString();
			}
			
		}
	}
	return filepath;
#else


	HWND hWnd = (HWND)handle;
	DWORD idProcess = NULL;
	GetWindowThreadProcessId(hWnd, &idProcess);
	return process::getpath(idProcess);
	//// 打开进程句柄
	//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, idProcess);
	//if (NULL != hProcess)
	//{
	//	char fp[MAX_PATH];
	//	GetModuleFileNameEx(hProcess, 0, fp, MAX_PATH);
	//	filepath = fp;
	//	CloseHandle(hProcess);
	//}
	//return filepath;
#endif
}

bool newobj::window::button::click(ptr_win handle)
{
	f_ret_f(PostMessageA((HWND)handle, 513, 1, 2 + 2 * 65536));
	f_ret_f(PostMessageA((HWND)handle, 514, 0, 2 + 2 * 65536));
	return true;
}
bool newobj::window::button::click(ptr_win parent, const nstring& class_name, const nstring& title_name)
{
	auto list = childs(parent, class_name, title_name);
	t_ret_f(list.size() != 1);
	button::click(list[0]);
	return true;
}
uint32 newobj::window::getpid(ptr_win handle)
{
	DWORD dwPid = 0;
	GetWindowThreadProcessId((HWND)handle, &dwPid);
	return dwPid;
	//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
}
Size newobj::window::size(ptr_win handle)
{
	Size result;
	RECT info;
	if (GetClientRect((HWND)handle, &info) == false)
		return result;
	result.width = info.right - info.left;
	result.height = info.bottom - info.top;
	return result;
}
Rect newobj::window::rect(ptr_win handle)
{
	Rect result;
	RECT info;
	if (GetWindowRect((HWND)handle, &info) == false)
		return result;
	result.right = info.right;
	result.bottom = info.bottom;
	result.left = info.left;
	result.top = info.top;
	return result;
}
void newobj::window::close(ptr_win handle)
{
	SendMessageA((HWND)handle, WM_CLOSE, 0, 0);
	//PostMessageA((HWND)handle, 16, 0, 0);
}
nstring newobj::window::content(ptr_win handle)
{
	uint32 count = SendMessageA((HWND)handle,14,0,0)+1;
	newobj::buffer data;
	if (count > 50000)
	{
		uint32 tempSize = 50000;
		data.setsize(50000);
		SendMessageA((HWND)handle, 13, tempSize + 1,(LPARAM)data.data());
		if (data[49999] < 0)
			data = data.left(data.length() - 1);
	}
	else
	{
		data.setsize(count);
		SendMessageA((HWND)handle, 13, count, (LPARAM)data.data());
	}
	return data.to_string();
}
void newobj::window::content(ptr_win handle, const nstring& content)
{
	SendMessageA((HWND)handle,12,0,(LPARAM)content.c_str());
}
bool newobj::window::content(ptr_win parent, const nstring& content, const nstring& class_name, const nstring& title_name)
{
	auto list = childs(parent, class_name, title_name);
	t_ret_f(list.size() != 1);
	newobj::window::content(list[0],content);
	return true;
}
bool newobj::window::top(ptr_win handle)
{
	return SetWindowPos((HWND)handle,(HWND)-1, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | 0);
}
void newobj::window::rect_sort(std::vector<ptr_win>& list, uint32 type,bool ps)
{
#define SORT_RECT(VALUE)                                                                                                \
    uint32 temp = 0;                                                                                                            \
    for (uint32 i = 0; i < list.size(); i++)                                                                                   \
    {                                                                                                                                       \
        for (uint32 x = i; x < list.size(); x++)                                                                                \
        {                                                                                                                                       \
			auto rect_i = newobj::window::rect(list[i]);                                                                    \
            auto rect_x = newobj::window::rect(list[x]);                                                                \
            if (ps == true ? rect_i.VALUE > rect_x.VALUE:rect_i.VALUE < rect_x.VALUE)                   \
            {                                                                                                                                   \
                temp = list[i];                                                                                                         \
                list[i] = list[x];                                                                                                                  \
                list[x] = temp;                                                                                                     \
            }                                                                                                                           \
        }                                                                                                                                   \
    }                                                                                                                                           
	if (type == 0)
	{
		SORT_RECT(top);
	}
	else if (type == 1)
	{
		SORT_RECT(right);
	}
	else if (type == 2)
	{
		SORT_RECT(left);
	}
	else if (type == 3)
	{
		SORT_RECT(bottom);
	}
}


bool newobj::window::menu::click(ptr_win handle, ptr_menu menu_child_handle, uint32 menu_item_idx)
{
	auto item = GetMenuItemID((HMENU)menu_child_handle, menu_item_idx);
	t_ret_f(item == 0);
	return PostMessageA((HWND)handle, 273, item, 0);
}

ptr_menu newobj::window::menu::get(ptr_win handle)
{
	t_ret_var(handle == 0, 0);
	return (ptr_menu)GetMenu((HWND)handle);
}

ptr_menu newobj::window::menu::child(ptr_menu menu_handle, uint32 menu_childs_idx)
{
	t_ret_var(menu_handle == 0, 0);
	return (ptr_menu)GetSubMenu((HMENU)menu_handle, menu_childs_idx);
}
bool newobj::window::checkbox::checked(ptr_win handle)
{
	return !(SendMessageA((HWND)handle, 240, 0, 0) == 0);
}
bool newobj::window::checkbox::checked(ptr_win handle, bool checked)
{
	return SendMessageA((HWND)handle, 241, checked==true?1:0, 0) == 0;
}

void newobj::window::tab::current(ptr_win handle, uint32 idx)
{
	SendMessageA((HWND)handle, 4912, idx, 0);
}
bool newobj::window::enable(ptr win_handle, bool enable)
{
	return EnableWindow((HWND)win_handle, enable);
}

void newobj::window::list::insert(ptr_win handle, uint32 index, const nstring& value)
{
	SendMessageA((HWND)handle, 385, index, (LPARAM)value.c_str());
}
#endif