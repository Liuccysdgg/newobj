#include "process.h"
#include "forstring.h"
#include "util/system.h"
#ifdef WIN32
#include <Windows.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <tlhelp32.h>
#include <locale>
#include "psapi.h"
#include <ShellAPI.h>
#include <process.h>
#include <stdio.h>
#include <winternl.h>
#endif
#ifdef WIN32

newobj::process::guard::guard()
{
	::newobj::ithread::start();
}

newobj::process::guard::~guard()
{
}

bool newobj::process::guard::add(const nstring& path, void* extra, std::function<bool(const process_info& proc, void* extra)> callback)
{
	process_info info;
	info.path = path;
	info.pid = 0;
	info.extra = extra;
	return false;
}

void* newobj::process::guard::remove(const nstring& path)
{
	return nullptr;
}

bool newobj::process::guard::run()
{

	
	return false;
}

int32 newobj::process::exec(const nstring& path,const nstring& rundir,bool wait, bool show)
{
	if (wait == true)
	{
		STARTUPINFO si;
		
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = show==true?SW_SHOW:SW_HIDE;
		ZeroMemory(&pi, sizeof(pi));
		// Start the child process. 
		if (!CreateProcessA(
			NULL,   // No module name (use command line)
			(LPSTR)path.c_str(),        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			rundir.length() == 0 ? NULL : rundir.c_str(),           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			return GetLastError();
		}
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return 0;
	}
	else
	{
		STARTUPINFOA info = { sizeof(STARTUPINFOA) };
		info.dwFlags = STARTF_USESHOWWINDOW;
		info.wShowWindow = SW_SHOW;
		PROCESS_INFORMATION proc;
		if (CreateProcessA(0, (LPSTR)path.c_str(), 0, 0, FALSE, 0, 0, rundir.length() == 0 ? NULL : rundir.c_str(), &info, &proc) == false)
		{
			DWORD d = GetLastError();
			d++;
			return false;
		}
		CloseHandle(proc.hProcess);
		CloseHandle(proc.hThread);
		return true;
	}
}
void  newobj::process::list(std::list<ProcData>& value)
{
#ifdef _WIN32
	/*
		DWORD dwSize;//结构大小
		DWORD cntUsage;//此进程的引用计数
		DWORD th32ProcessID;//进程ID
		DWORD th32DefaultHeapID;//进程默认堆ID
		DWORD th32ModuleID;//进程模块ID
		DWORD cntThreads;//此进程开启的线程计数
		DWORD th32ParentProcessID;//父进程ID
		LONG pcPriClassBase;//线程优先权
		DWORD dwFlags;//保留
		char szExeFile[MAX_PATH];//进程名
	*/
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//printf("[list] 2\n");
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot Failed, GetLastError()=%d\n", GetLastError());
		return;
	}
	BOOL bMore = Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		ProcData data;
		data.id = pe32.th32ProcessID;
		data.name = pe32.szExeFile;
		data.parent = pe32.th32ParentProcessID;
		value.push_back(data);
		bMore = Process32Next(hProcessSnap, &pe32);
	}
	//printf("[list] 5\n");
	CloseHandle(hProcessSnap);
	//printf("[list] 6\n");
	return;
#else
	return;
#endif
}
//nstring  newobj::process::getpath(uint32 pid)
//{
//#ifdef _WIN32
//	HANDLE hProcess = OpenProcess(0x1F0FFF, FALSE, pid);
//	if (!hProcess)
//		return "";
//	char filePath[MAX_PATH];
//	memset(filePath, 0, MAX_PATH);
//	DWORD ret = GetModuleFileNameExA(hProcess, NULL, filePath, MAX_PATH);
//	nstring file = nstring(filePath, strlen(filePath) > MAX_PATH ? 0 : strlen(filePath));
//	CloseHandle(hProcess);
//	return ret == 0 ? "" : file;
//#else
//	return "";
//#endif
//}
nstring  newobj::process::getpath(uint32 pid)
{
	t_ret_var(pid == 0, "");
	auto hProc = OpenProcess(PROCESS_QUERY_INFORMATION, 0, pid);
	if (hProc == 0)
		return "";
	char wzFile[MAX_PATH];
	char szDevName[MAX_PATH];
	memset(wzFile, 0, MAX_PATH);
	memset(szDevName, 0, MAX_PATH);

	GetProcessImageFileNameA(hProc, wzFile, MAX_PATH);
	CloseHandle(hProc);

	char szDrive[3] = { 64, 58, 0 };
	uint32 nSize = 0;
	for (uint32 i = 0; i < 26; i++)
	{
		szDrive[0] = szDrive[0] + 1;
		nSize = QueryDosDeviceA((LPCSTR)szDrive, szDevName, MAX_PATH);
		if (nSize != 0 && (nstring(wzFile).find(nstring(szDevName)+"\\") != -1 || nstring(szDevName).find(wzFile) != -1))
		{
			nstring path = nstring(wzFile).replace(0, strlen(szDevName), nstring((char*)szDrive));
			return path;
		}
	}
	return "";
}
bool  newobj::process::exist(const nstring& procpath)
{
	std::list<ProcData> list;
	process::list(list);
	for_iter(iter, list)
	{
		if (forstring::en_case(iter->path(), false) == forstring::en_case(procpath, false))
			return true;
	}
	return false;
}
uint32  newobj::process::getpid(const nstring& procpath)
{
	std::list<ProcData> list;
	process::list(list);
	for_iter(iter, list)
	{
		if (forstring::en_case(iter->path(), false) == forstring::en_case(procpath, false))
			return iter->id;
	}
	return 0;
}
bool  newobj::process::kill(uint32 pid)
{
#ifdef _WIN32
	/*杀死进程*/
	auto handle = OpenProcess(1, 0, pid);
	t_ret_f(handle == 0);
	f_ret_f(TerminateProcess(handle, 0));
	return CloseHandle(handle);
#else
	return false;
#endif
}
uint32  newobj::process::current_pid()
{
	return ::getpid();
}
bool  newobj::process::more_open(const nstring& name)
{
	//初始化临界区全局原子变量
	HANDLE MutexHandle = CreateMutexA(NULL, FALSE, name.c_str());  //创建互斥体. 信号量为0. 有信号的状态.wait可以等待
	DWORD ErrorCode = 0;
	ErrorCode = GetLastError();
	if (ERROR_ALREADY_EXISTS == ErrorCode)
	{
		CloseHandle(MutexHandle);
		return true;
	}
	return false;
}
bool  newobj::process::exist_mutex(const nstring& name)
{
	//初始化临界区全局原子变量
	HANDLE MutexHandle = CreateMutexA(NULL, FALSE, name.c_str());  //创建互斥体. 信号量为0. 有信号的状态.wait可以等待
	DWORD ErrorCode = 0;
	ErrorCode = GetLastError();
	CloseHandle(MutexHandle);
	return ERROR_ALREADY_EXISTS == ErrorCode;
}
typedef LONG(WINAPI* PROCNTQSIP)(HANDLE, UINT, PVOID, ULONG, PULONG);
typedef struct
{
	DWORD ExitStatus;
	DWORD PebBaseAddress;
	DWORD AffinityMask;
	DWORD BasePriority;
	ULONG UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
}   ___PROCESS_BASIC_INFORMATION;
typedef NTSTATUS(CALLBACK* PFN_NTQUERYINFORMATIONPROCESS)(
	HANDLE ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID ProcessInformation,
	ULONG ProcessInformationLength,
	PULONG ReturnLength OPTIONAL
	);
NTSTATUS __NtQueryInformationProcess(
	HANDLE hProcess,
	PROCESSINFOCLASS pic,
	PVOID pPI,
	ULONG cbSize,
	PULONG pLength
) {

	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
	if (hNtDll == NULL) {
		return(-1);
	}

	NTSTATUS lStatus = -1;  // error by default.

	// Note that function name is not UNICODE
	PFN_NTQUERYINFORMATIONPROCESS pfnNtQIP =
		(PFN_NTQUERYINFORMATIONPROCESS)GetProcAddress(
			hNtDll, "NtQueryInformationProcess");
	if (pfnNtQIP != NULL) {
		lStatus = pfnNtQIP(hProcess, pic, pPI, cbSize, pLength);
	}

	FreeLibrary(hNtDll);
	return(lStatus);
}
uint32  newobj::process::parent_pid(uint32 pid)
{
	LONG                      status;
	DWORD                     dwParentPID = (DWORD)-1;
	HANDLE                    hProcess;
	___PROCESS_BASIC_INFORMATION pbi;

	// Get process handle
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (!hProcess)
		return 0;
	// Retrieve information
	status = __NtQueryInformationProcess(hProcess,
		ProcessBasicInformation,
		(PVOID)&pbi,
		sizeof(___PROCESS_BASIC_INFORMATION),
		NULL
	);
	// Copy parent Id on success
	if (!status)
		dwParentPID = pbi.InheritedFromUniqueProcessId;
	CloseHandle(hProcess);
	return dwParentPID;
}
bool  newobj::process::effective(uint32 __pid__)
{
	DWORD *pid = new DWORD[1024];
	DWORD cbNeeded = 0;
	bool existence = false;
	if (EnumProcesses(pid, 1024 * 2, &cbNeeded))
	{
		for (uint32 i = 0; i < cbNeeded / 4; i++)
		{
			if (pid[i] == __pid__)
			{
				existence = true;
				break;
			}
		}
	}
	return existence;
}
#endif
