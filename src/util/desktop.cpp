#include "util/desktop.h"
#ifdef _WIN32
#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef LIB_QT
#include "QtCore/qprocess.h"
#ifdef QT5
#include "QtGui/qguiapplication.h"
#include "QtGui/qscreen.h"
#include "QtGui/qpixmap.h"
#endif
#endif
#include "util/process.h"
#include "util/window.h"


newobj::desktop::desktop(const nstring& name, bool create_new, bool destory)
{

    m_desktop = 0;
    m_destory = true;

	m_name = name;
    m_destory = destory;
    m_desktop = (ptr)OpenDesktopA(name.c_str(), 0, true, GENERIC_ALL);
    if(m_desktop == 0 && create_new)
	    m_desktop = (uint64)CreateDesktopA(name.c_str(), 0, 0, 0, GENERIC_ALL, 0);
}

newobj::desktop::desktop(ptr handle,bool destory)
{
    m_destory = destory;
    m_desktop = handle;
}

newobj::desktop::~desktop()
{
    if (m_desktop != 0 && m_destory == true)
    {
        CloseDesktop((HDESK)m_desktop);
    }
}

bool newobj::desktop::show()
{
    t_ret_f(m_desktop == 0);
	current();
	return SwitchDesktop((HDESK)m_desktop);
}

bool newobj::desktop::exec(const nstring& filepath,const nstring& path)
{ 
    t_ret_f(m_desktop == 0);
    STARTUPINFOA info = { sizeof(STARTUPINFOA) };
    info.dwFlags = STARTF_USESHOWWINDOW;
    info.wShowWindow = SW_SHOW;
    info.lpDesktop = (LPSTR)m_name.c_str();
    PROCESS_INFORMATION proc;
    f_ret_f(CreateProcessA(
        0, 
        (LPSTR)filepath.c_str(),
        0,
        0,
        FALSE,
        0,
        0,
        path.length()==0?NULL:path.c_str(),
        &info,
        &proc)
    );
    CloseHandle(proc.hProcess);
    CloseHandle(proc.hThread);
    m_exec_hds.push_back(proc.dwProcessId);
    return true;
}

void newobj::desktop::current()
{
    t_ret(m_desktop == 0);
	bool result = SetThreadDesktop((HDESK)m_desktop);
    return;
}

void newobj::desktop::destoryAllProcesses(bool subproc)
{
    std::list<process::ProcData> proc_list;
    process::list(proc_list);
    for (size_t i = 0; i < m_exec_hds.size(); i++)
    {
        for_iter(iter, proc_list)
        {
            if (m_exec_hds[i] == iter->id)
            {
                if (subproc)
                {
                    desktorySubProcesses(proc_list, iter->id);
                    process::kill(iter->id);
                }
            }
        }
        
    }
    m_exec_hds.clear();
}

ptr newobj::desktop::handle()
{
    return m_desktop;
}
#if defined(LIB_QT) && defined(QT5)
QPixmap newobj::desktop::screenshot()
{
    return QGuiApplication::primaryScreen()->grabWindow(winhandle());
}
#endif
ptr newobj::desktop::winhandle()
{
    auto list = window::find(0,"#32769");
    if (list.size() > 1 || list.size() == 0)
        return 0;
    return list[0];
}


void newobj::desktop::desktorySubProcesses(const std::list<newobj::process::ProcData>& list, ptr pid)
{
    for_iter(iter,list)
    {
        if (iter->parent == pid)
        {
            desktorySubProcesses(list, iter->id);
            newobj::process::kill(pid);
        }
    }
}

BOOL CALLBACK DeskEnum(LPSTR desk, LPARAM lParam)
{
    std::vector<desktop::DesktopInfo> *infos = (std::vector<desktop::DesktopInfo>*)lParam;
    HDESK hDesk = OpenDesktopA(desk, 0, FALSE, DESKTOP_READOBJECTS | DESKTOP_ENUMERATE);
    if (hDesk)
    {
        HDESK hCurrentDesk = GetThreadDesktop(GetCurrentThreadId());
        newobj::desktop::DesktopInfo info;
        info.name = desk;
        info.handle = (ptr)hDesk;
        infos->push_back(info);
    }
    return TRUE;
}


std::vector<desktop::DesktopInfo> newobj::desktop::all(const std::string& winsta)
{
    std::vector<desktop::DesktopInfo> infos;
    // Winsta0
    //HWINSTA current = GetProcessWindowStation();
    HWINSTA result = 0;
    if(winsta == "")
        result = OpenWindowStationA("Winsta0", FALSE, WINSTA_ENUMDESKTOPS);
    else
        result = OpenWindowStationA(winsta.c_str(), FALSE, WINSTA_ENUMDESKTOPS);
    if (result != 0)
    {
        //SetProcessWindowStation(hWinsta);
        EnumDesktopsA(result, &DeskEnum, (LPARAM)&infos);
        //SetProcessWindowStation(current);
        //CloseWindowStation(hWinsta);
    }
    return infos;
}

ptr newobj::desktop::get_current()
{
    return (ptr)GetThreadDesktop(GetCurrentThreadId());
}

void newobj::desktop::show(ptr handle)
{
    SetThreadDesktop((HDESK)handle);
    SwitchDesktop((HDESK)handle);
}

ptr newobj::desktop::get_desktop(const nstring& name)
{
    return (ptr)OpenDesktopA(name.c_str(), 0, true, GENERIC_ALL);
}

bool newobj::desktop::close(ptr desktop)
{
    return CloseDesktop((HDESK)desktop);
}
#endif