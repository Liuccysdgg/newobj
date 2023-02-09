#include "lock.h"
#include "public/exception.h"
#define HAVE_STRUCT_TIMESPEC

#ifdef _WIN32
#include <Windows.h>
#ifndef MSVC_2010
#include <mutex>
#include <shared_mutex>
#endif
#else
#include <pthread.h>
#endif
newobj::mutex::mutex()
{
	m_locked = false;
#ifdef _WIN32
	m_lock = (void*)CreateMutexA(NULL, FALSE, NULL);
#else
	m_lock = new pthread_mutex_t;
	pthread_mutex_init((pthread_mutex_t*)m_lock, NULL);
#endif
} 

newobj::mutex::~mutex()
{
		unlock();
#ifdef _WIN32
		try
		{
			unlock();
			CloseHandle((HANDLE)m_lock);
		}
		catch (const std::exception&)
		{

		}
#else
		pthread_mutex_destroy((pthread_mutex_t*)m_lock);
		delete (pthread_mutex_t*)m_lock;
#endif
		
}

void newobj::mutex::lock()
{
	m_locked = true;
#ifdef _WIN32
	WaitForSingleObject((HANDLE)m_lock, INFINITE);
#else
	pthread_mutex_lock((pthread_mutex_t*)m_lock);
#endif
}

void newobj::mutex::unlock()
{
	m_locked = false;
#ifdef _WIN32
	ReleaseMutex((HANDLE)m_lock);
#else
	pthread_mutex_unlock((pthread_mutex_t*)m_lock);
#endif
}

bool newobj::mutex::try_lock()
{
#ifdef WIN32
	DWORD dwWaitResult;

	dwWaitResult = WaitForSingleObject((HANDLE)m_lock, 0);
	if (dwWaitResult != WAIT_TIMEOUT && dwWaitResult != WAIT_FAILED)
	{
		m_locked = true;
		return true;
	}
	return false;
#else
	return pthread_mutex_trylock((pthread_mutex_t*)m_lock) == 0;
#endif
}

newobj::alock::alock(newobj::mutex& mutex, bool lock)
{
	m_mutex_newobj = nullptr;
	m_lock = false;

	this->m_mutex_newobj = &mutex;
	m_lock = lock;
	if(m_lock)
		this->m_mutex_newobj->lock();
}
newobj::alock::~alock()
{
	if (this->m_mutex_newobj != nullptr)
	{
		if (m_lock)
			this->m_mutex_newobj->unlock();
	}
}

void newobj::alock::lock()
{
	if (this->m_mutex_newobj != nullptr)
	{
		this->m_mutex_newobj->lock();
		this->m_lock = true;
	}
	
}


void newobj::alock::unlock()
{
	if (this->m_mutex_newobj != nullptr)
	{
		this->m_mutex_newobj->unlock();
		this->m_lock = false;
	}	
}

newobj::rwlock::rwlock()
{
#ifdef _WIN32
#ifdef MSVC_2010
	m_lock = (void*)CreateMutexA(NULL, FALSE, NULL);
#else
	m_lock = (void*)new std::shared_mutex;
#endif
#else
	m_lock = new pthread_rwlock_t;
    pthread_rwlock_init((pthread_rwlock_t*)m_lock, NULL);
#endif
}

newobj::rwlock::~rwlock()
{
#ifdef _WIN32
	try
	{
		unlock();
#ifdef MSVC_2010
		CloseHandle((HANDLE)m_lock);
#else
		delete (std::shared_mutex*)m_lock;
#endif
	}
	catch (const std::exception&)
	{

	}
#else
    pthread_rwlock_destroy((pthread_rwlock_t*)m_lock);
	delete (pthread_rwlock_t*)m_lock;
#endif
}

void newobj::rwlock::lock_shared()
{
#ifdef _WIN32
#ifdef MSVC_2010
	WaitForSingleObject((HANDLE)m_lock, INFINITE);
#else
	((std::shared_mutex*)m_lock)->lock_shared();
#endif
#else
	pthread_rwlock_rdlock((pthread_rwlock_t*)m_lock);
#endif
}

void newobj::rwlock::lock()
{
#ifdef _WIN32
#ifdef MSVC_2010
	WaitForSingleObject((HANDLE)m_lock, INFINITE);
#else
	((std::shared_mutex*)m_lock)->lock();
#endif
#else
	pthread_rwlock_wrlock((pthread_rwlock_t*)m_lock);
#endif


}
void newobj::rwlock::unlock()
{
#ifdef _WIN32
	
#ifdef MSVC_2010
	ReleaseMutex((HANDLE)m_lock);
#else
	((std::shared_mutex*)m_lock)->unlock();
#endif
#else
    pthread_rwlock_unlock((pthread_rwlock_t*)m_lock);
#endif
}

void newobj::rwlock::unlock_shared()
{
#ifdef _WIN32
#ifdef MSVC_2010
	ReleaseMutex((HANDLE)m_lock);
#else
	((std::shared_mutex*)m_lock)->unlock_shared();
#endif
#else
	pthread_rwlock_unlock((pthread_rwlock_t*)m_lock);
#endif
}
