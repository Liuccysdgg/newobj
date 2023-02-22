#pragma

#include "basetype/string.h"
#include "util/lock.h"
class integer
{
public:
    integer(bool mutex = false)
    {
        m_mutex = nullptr;
        m_value = 0;
        if (mutex)
            m_mutex = new newobj::mutex;
    }
    ~integer()
    {
        if (m_mutex != nullptr)
            delete m_mutex;
    }
    inline operator int()
    {
        return m_value;
    }
    inline char operator[](int idx)
    {
        char ret = 0x00;
        ___lock___();
        ret = ((char*)&m_value)[idx];
        ___unlock___();
        return ret;
    }
    inline void operator=(int value)
    {
        ___lock___();
        m_value = value;
        ___unlock___();
    }
    inline bool operator==(int value)
    {
        return m_value == value;
    }
    inline bool operator>=(int value)
    {
        return m_value >= value;
    }
    inline bool operator<=(int value)
    {
        return m_value <= value;
    }
    inline bool operator!=(int value)
    {
        return m_value != value;
    }
    inline nstring to_string()
    {
        return nstring::from(m_value);
    }
private:
    void ___lock___()
    {
        if (m_mutex != nullptr)
            m_mutex->lock();
    }
    void ___unlock___()
    {
        if (m_mutex != nullptr)
            m_mutex->unlock();
    }
private:
    newobj::mutex* m_mutex;
    int m_value;
};
