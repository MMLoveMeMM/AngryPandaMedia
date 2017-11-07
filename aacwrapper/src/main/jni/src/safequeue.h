#ifndef __H_SAFE_QUEUE_H__
#define __H_SAFE_QUEUE_H__

#include<queue>
#include<iostream>
using namespace std;
#include "pthread.h"

namespace panda
{
/*
* 一个用于产生数据和消费数据之间的一个传递数据队列,安全pop和push
*/
template <typename T>
class safequeue
{
public:
	safequeue();
	~safequeue();
public:
	T Pop();
	void Push(T&);
	queue<T> popAll();
	void clrAll();
private:
	std::queue<T> mQueue;
	pthread_mutex_t mutexsum;

};
}

#endif //__H_SAFE_QUEUE_H__