#include "Multiplexer.h"

Multiplexer::Multiplexer(SimpleQueue<std::string>& queue,
	std::vector<address> addressesIn, 
	address addressOut) :
queue(queue), addrsIn(addressesIn), addrOut(addressOut) {}


void Multiplexer::start(){
	this->running = true;

	// Start listening on each sream
	for(auto addr : addrsIn){
		threads.push_back(std::thread(&Multiplexer::receive, this, addr));
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

void Multiplexer::receive(address addr){
	int sock;
	unsigned int clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0){
    	std::cout << "ERROR opening socket\n";
	}
	memset(&serv_addr, 0, sizeof(serv_addr)); 
    memset(&cli_addr, 0, sizeof(cli_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(addr.addr.c_str());
	serv_addr.sin_port = (unsigned short) stoi(addr.port);

	if(bind(sock, (struct sockaddr *) &serv_addr, 
		sizeof(serv_addr)) < 0) {
		std::cout << "ERROR on binding\n";
	}

    int n; 
    unsigned int len;
    std::cout << "calling recvfrom\n";
    // n = recvfrom(sock, (char *)buffer, 256,  
    // 	0, ( struct sockaddr *) &cli_addr, 
    // 	&len); 
    sendto(sock, (const char *)"hello", strlen("hello"), 
        0, (const struct sockaddr *) &serv_addr,  
            sizeof(serv_addr)); 
    std::cout << "called recvfrom\n";
    buffer[n] = '\0'; 
    std::cout << "Here is the message: " << buffer << std::endl;

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