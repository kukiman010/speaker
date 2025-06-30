#pragma once
#ifndef SAVED_CONTAINER_H
#define SAVED_CONTAINER_H

#include <queue>
#include <list>
#include <mutex>

template <typename T>
class SQueue // saved queue
{
    std::queue<T>   _que;
    std::mutex      _mutex;


public:
    SQueue(){}

    void            push(const T& value);
    T               pop();
    bool            isEmpty(){return _que.empty();}
    unsigned int    size() {_mutex.lock(); return _que.size(); _mutex.unlock();}
};



template <typename T>
void SQueue<T>::push(const T& value)
{
    _mutex.lock();
    _que.push(value);
    _mutex.unlock();
}

template <typename T>
T SQueue<T>::pop()
{
    _mutex.lock();
    T value = _que.front();
    _que.pop();
    _mutex.unlock();

    return value;
}



template <typename T>
class Slist // saved queue
{
    std::list<T>    _list;
    std::mutex      _mutex;


public:
    Slist(){}

    void            push_back(const T& value);
    void            pop_back();
    bool            isEmpty(){return _list.empty();}
    unsigned int    size() {_mutex.lock(); return _list.size(); _mutex.unlock();}
    
//    https://en.cppreference.com/w/cpp/container/list
//    push>front
//    pop_front
//    merge
//    front
//    clear
//    erase
    
    
};



template <typename T>
void Slist<T>::push_back(const T& value)
{
    _mutex.lock();
    _list.push_back(value);
    _mutex.unlock();
}

template <typename T>
void Slist<T>::pop_back()
{
    _mutex.lock();
    _list.pop_back();
    _mutex.unlock();
}









#endif // SAVED_CONTAINER_H
