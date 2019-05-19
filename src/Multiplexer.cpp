#include "Multiplexer.h"

Multiplexer::Multiplexer(SimpleQueue<std::string>& queue,
	std::vector<address> addressesIn, 
	address addressOut) :
queue(queue), addrsIn(addressesIn), addrOut(addressOut) {}


void Multiplexer::start(){
	this->running = true;

	// Start listening on each sream
	for(auto addr : addrsIn){
		threads.push_back(std::thread(&Multiplexer::listen, this, addr));
	}

	// Start posting to out stream
	threads.push_back(std::thread(&Multiplexer::send, this, addrOut));
}


void Multiplexer::stop(){
	this->running = false;
	for(int i = 0; i < threads.size(); i++){
		threads[i].join();
	}
}

void Multiplexer::listen(address addr){
	for(int i = 0; i < 5; i++){
		this->queue.push("mystring");
	}
}

void Multiplexer::send(address addr){
	while(this->running){
		if(!this->queue.empty()){
			std::cout << this->queue.front() << std::endl;
			this->queue.pop();
		}
	}
}