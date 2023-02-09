#include "thread.h"
#include <string.h>
#include "util/forstring.h"
#include "system.h"
#include "hpsocket/hpsocket.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <thread>
#endif

#ifdef LIB_QT
#ifdef QT4
#include "Qt/qdatetime.h"
#include "Qt/qcoreapplication.h"
#else
#include "QtCore/qglobal.h"
#include "QtCore/qdatetime.h"
#include "QtCore/qcoreapplication.h"
#endif
#endif
struct newobj_thread_pool_param
{
	newobj::thread_pool* pool;
	void* param;
};
namespace newobj
{
	void newobj_thread_thread(void* lpParam)
	{
		auto t = (newobj::ithread*)lpParam;
		t->__thread_handle(lpParam);
	}
	ithread::ithread()
	{
		m_state = 2;
		m_thread = false;
	}
	ithread::~ithread()
	{

	}
	bool ithread::start()
	{
		if (this->m_state == 0)
			return false;
		else if (this->m_state == 1)
			return false;
		//struct NewobjThreadParam
		//{

		//};
		/*启动线程*/
		m_thread = true;
		newobj::thread::create(newobj_thread_thread,this);
		
		this->m_state = 0;
		return true;
	}
	bool ithread::state()
	{
		if (this->m_state == 2)
			return false;
		return true;
	}
	void ithread::wait()
	{
		//timestamp start_time = sys::GetNowMSec();
		while (m_thread == true)
		{
			system::sleep_msec(100);
		}
		this->m_state = 2;
		system::sleep_msec(100);
	}
	bool ithread::stop()
	{
		this->m_state = 1;
		return true;
	}
	void* ithread::__thread_handle(void* param)
	{
		newobj::ithread* t = (newobj::ithread*)param;
		t->m_thread = true;
		while (true)
		{
			if (t->m_state == 1)
			{
				t->m_state = 2;
				break;
			}
			else
			{
				if (t->run() == false)
				{
					t->m_state = 2;
					break;
				}
			}
		}
		t->m_thread = false;
		return NULL;
	}
	
	thread_pool::thread_pool()
	{
		m_hp_thread =  (void*)HP_Create_ThreadPool();
	}

	thread_pool::~thread_pool()
	{
		HP_Destroy_ThreadPool((IHPThreadPool*)m_hp_thread);
	}

	bool thread_pool::start(uint32 thread_count, uint32 max_queue_size)
	{
		return ((IHPThreadPool*)m_hp_thread)->Start(thread_count, max_queue_size, TRP_CALL_FAIL);
	}
	VOID __HP_CALL TaskProc_function_thread_pool(PVOID pvArg) {
		newobj_thread_pool_param* p = (newobj_thread_pool_param*)pvArg;
		p->pool->__thread_handle(p);
		delete p;
	}
	bool thread_pool::submit(void* param)
	{
		newobj_thread_pool_param* p = new newobj_thread_pool_param;
		p->param = param;
		p->pool = this;
		if (((IHPThreadPool*)m_hp_thread)->Submit(TaskProc_function_thread_pool, (PVOID)p) == false)
		{
			delete p;
			return false;
		}
		return true;
	}
	void thread_pool::__thread_handle(void* param)
	{
		((newobj_thread_pool_param*)param)->pool->run_pool(((newobj_thread_pool_param*)param)->param);
	}

}

namespace newobj
{
	namespace thread
	{
		struct NewobjThreadCreateParam
		{
			std::function<void(void*)> func;
			void* param;
		};
#ifdef WIN32
		DWORD WINAPI newobj_thread_create_thread(LPVOID lpParam)
#else
		void newobj_thread_create_thread(void* lpParam)
#endif
		{
			auto info = (NewobjThreadCreateParam*)lpParam;
			info->func(info->param);
			delete info;
#ifdef WIN32
			return 0;
#endif
		}

		bool create(std::function<void(void*)> func, void* param, bool wait)
		{
			auto info = new NewobjThreadCreateParam;
			info->func = func;
			info->param = param;
#ifdef WIN32
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)newobj_thread_create_thread, (LPVOID)info, 0, 0);
			if (hThread == NULL)
			{
				delete info;
				return false;
			}
			if (wait)
				WaitForSingleObject(hThread, 0xFFFFFFFF); // 等待，直到线程被激发
			CloseHandle(hThread);
			return true;
#else
			std::thread t(newobj_thread_create_thread, info);
			if (wait)
			{
				t.join();
			}
			else
			{
				t.detach();
			}
			return true;
#endif

		}

		bool create(std::function<void(void*)> func, void* param,std::function<bool/*true=继续 false=退出等待*/()> wait)
		{
			auto info = new NewobjThreadCreateParam;
			info->func = func;
			info->param = param;
#ifdef WIN32
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)newobj_thread_create_thread, (LPVOID)info, 0, 0);
			if (hThread == NULL)
			{
				delete info;
				return false;
			}
			if (wait())
			{
				WaitForSingleObject(hThread, 0xFFFFFFFF);
			}
			//// Qt Wait
			//{

			//	QTime time;
			//	time.start();
			//	uint32 loop = 0;
			//	while (time.elapsed() < (int)timeout_msec)
			//	{
			//		loop++;
			//		if (loop >= 500)
			//		{
			//			auto rs = WaitForSingleObject(hThread, 1);
			//			if (rs == WAIT_OBJECT_0)
			//				break;
			//			else if (rs == WAIT_TIMEOUT)
			//				continue;
			//			else if (rs == WAIT_FAILED)
			//				continue;
			//			loop = 0;
			//		}
			//		
			//		QCoreApplication::processEvents();
			//	}
			//}
			CloseHandle(hThread);
			return true;
#else
			return false;
#endif
		}
	}
}
