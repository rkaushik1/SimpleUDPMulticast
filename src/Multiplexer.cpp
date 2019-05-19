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
	struct sockaddr_in serv_addr, cli_addr;
	unsigned int addrlen = sizeof(cli_addr);
	int sock, recvlen;
	unsigned char buf[256];

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("cannot create socket\n");
		return;
	}

	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(addr.addr.c_str());
	serv_addr.sin_port = htons(stoi(addr.port));

	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind failed");
		return;
	}

	while(this->running) {
		std::stringstream ss;
		printf("waiting on port %d\n", stoi(addr.port));
		recvlen = recvfrom(sock, buf, 256, 0, 
			(struct sockaddr *) &cli_addr, &addrlen);
		printf("received %d bytes\n", recvlen);
		if (recvlen > 0) {
			buf[recvlen] = 0;
			ss << buf;
			this->queue.push(ss.str());
			printf("received message: \"%s\"\n", buf);
			std::cout << this->queue.front() << std::endl;
		}
		else {
			exit(1);
		}
	}
	std::cout << "Closing port: " << addr.port << std::endl;

}

void Multiplexer::send(address addr){
	int seq = 0;
	struct sockaddr_in serv_addr, cli_addr;
	unsigned int addrlen = sizeof(cli_addr);
	int sock, recvlen;
	char buf[256];

	int slen = sizeof(serv_addr);

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("socket created\n");
	}

	memset((char *)&cli_addr, 0, sizeof(cli_addr));

	/* pick any port number not used */
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	cli_addr.sin_port = htons(0);

	if (bind(sock, (struct sockaddr *)&cli_addr, sizeof(cli_addr)) < 0) {
		perror("bind failed");
		return;
	} 

	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(stoi(addr.port));
	serv_addr.sin_addr.s_addr = inet_addr(addr.addr.c_str());

	while(this->running){
		if(!this->queue.empty()){

			strcpy(buf, (this->queue.front() + ":" 
				+ std::to_string(seq)).c_str());
			seq++;
			this->queue.pop();

			if (sendto(sock, buf, std::strlen(buf), 0, 
				(struct sockaddr *)&serv_addr, slen)==-1){
				perror("sendto");
			}
		}	
	}
	close(sock);
}