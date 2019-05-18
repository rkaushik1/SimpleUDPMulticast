#pragma once

#include <queue>
#include <mutex>

template <class T>
class SimpleQueue {

private:
	std::queue<T> sub_queue;
	std::mutex queue_mutex;

public:
	void push(T const & data);
	bool empty();
	T front();
	void pop();
};

#include "SimpleQueue.cpp"