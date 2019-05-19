#pragma once

#include <iostream>
#include <sstream> 
#include <thread>
#include <vector>
#include <sstream>
#include <string>
#include <atomic>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#include "SimpleQueue.h"

struct address {
	std::string addr;
	std::string port;
};

class Multiplexer {
private:
	SimpleQueue<std::string>& queue;
	std::vector<address> addrsIn;
	std::vector<std::thread> threads;
	std::atomic<bool> running;
	address addrOut;

	void receive(address addr);
	void send(address addr);


public:
	Multiplexer(SimpleQueue<std::string>& queue,
		std::vector<address> addresses, 
		address addressOut);
	void start();
	void stop();
};