#pragma once

#include <queue>
#include <mutex>

template <class T>
class SimpleQueue {

private:
	std::queue<T> sub_queue;
	std::mutex queue_mutex;

public:
	void push(T const & data){
		std::lock_guard<std::mutex> lock(this->queue_mutex);
		this->sub_queue.push(data);
	}

	bool empty(){
		std::lock_guard<std::mutex> lock(this->queue_mutex);
		return this->sub_queue.empty();
	}

	T front(){
		std::lock_guard<std::mutex> lock(this->queue_mutex);
		return this->sub_queue.front();
	}

	void pop(){
		std::lock_guard<std::mutex> lock(this->queue_mutex);
		this->sub_queue.pop();
	}
};
