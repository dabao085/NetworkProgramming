/*
 * Author: Cheng xiang
 * Date: 2019-2-27
 * wrapper class of lock, condition variable
*/

#ifndef _LOCKER_H_
#define _LOCKER_H_

#include "network.h"
#include <exception>
#include <iostream>
class CLocker
{
public:
    CLocker()
    {
        if(pthread_mutex_init(&m_lock, NULL) != 0)
        {
            std::cout << "pthread_mutex_init error!" << std::endl;
            throw std::exception();
        }
    }
    ~CLocker()
    {
        if(pthread_mutex_destroy(&m_lock) != 0) //析构函数不抛出异常, Effective C++ item 8
        {
            std::cout << "pthread_mutex_destroy error!" << std::endl;
        }
    }
    
    bool lock()
    {
        return pthread_mutex_lock(&m_lock) == 0;
    }

    bool unlock()
    {
        return pthread_mutex_unlock(&m_lock) == 0;
    }
private:
    pthread_mutex_t m_lock;
};

class CCondition
{
public:
    CCondition()
    {
        if(pthread_mutex_init(&m_lock, NULL) != 0)
        {
            std::cout << "pthread_mutex_init error!" << std::endl;
            throw std::exception();
        }
        if(pthread_cond_init(m_cond, NULL) != 0)
        {
            std::cout <<"pthread_cond_init error!" << std::endl;
            pthread_mutex_destroy(&m_lock);
            throw std::exception();
        }
    }
    ~CCondition()//不抛出异常，同上，注意析构顺序
    {
        pthread_mutex_destroy(&m_lock);
        pthread_cond_destroy(&m_cond);
    }

    bool wait()
    {
        pthread_mutex_lock(&m_lock);
        int ret = pthread_cond_wait(&m_cond, &m_lock);
        pthread_mutex_unlock(&m_lock);
        return ret == 0;
    }

    bool signal()
    {
        return pthread_cond_signal(&m_cond) == 0;
    }
private:
    pthread_mutex_t m_lock;
    pthread_condition_t m_cond;
};
#endif