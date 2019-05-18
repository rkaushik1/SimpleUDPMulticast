#pragma once

template <class T>
void SimpleQueue<T>::push(T const& data){
	std::lock_guard<std::mutex> lock(this->queue_mutex);
	this->sub_queue.push(data);
}

template <class T>
bool SimpleQueue<T>::empty() {
	std::lock_guard<std::mutex> lock(this->queue_mutex);
	return this->sub_queue.empty();
}

template <class T>
T SimpleQueue<T>::front() {
	std::lock_guard<std::mutex> lock(this->queue_mutex);
	return this->sub_queue.front();
}

template <class T>
void SimpleQueue<T>::pop(){
	std::lock_guard<std::mutex> lock(this->queue_mutex);
	this->sub_queue.pop();
}