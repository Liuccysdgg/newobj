#pragma once
#include "public/define.h"
#include <functional>
namespace newobj
{
	class NEWOBJ_API mutex
	{
	public:
		mutex();
		~mutex();
		void lock();
		void unlock();
		bool try_lock();
		inline void exec(std::function<void()> callback)
		{
			lock();
			callback();
			unlock();
		}
	private:
		void *m_lock;
		bool m_locked;
	};
	class NEWOBJ_API alock {
	public:

		alock(newobj::mutex& mutex,bool lock = true);
		~alock();
		void lock();
		void unlock();
	private:
		newobj::mutex* m_mutex_newobj;
		bool m_lock;
	};
    class rwlock
    {
    public:
        rwlock();
        ~rwlock();
        void lock_shared();
        void lock();

        void unlock();
		void unlock_shared();
    private:
		void* m_lock;
    };
}
