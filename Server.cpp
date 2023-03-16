#include "Server.hpp"

Server::Server()
{
	for (int i = 0; i < 1024; i++)
	{
		this->polls[i].fd = -1;
		this->polls[i].events = 0;
		this->polls[i].revents = 0;
	}
}


/* Create a server
	-difference AF(adress family) and PF(protocol family) PF was intended to 
		handle several protocols this never happened so AF is what you should use
	-difference INET(v4) and INET6(v6) is the size of their address space
		INET6 improved features:
			-better routing and security
			-eliminates the need for address translation, which is necessary in INET
	-SOCK_STREAM type provides sequenced, reliable, two-way connection based byte streams
		SOCK_DGRAM socket supports datagrams (connectionless, unreliable messages of a fixed
		(typically small) maximum length)
	-SOCK_RAW sockets provide access to internal network protocols and interfaces (only super-user)*/
int	Server::createServer()
{
	this->serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverfd == -1)
	{
		std::cerr << "Can't create a socket!" << std::endl;
		return (-1);
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_addr.s_addr = htonl(INADDR_ANY);
	hint.sin_port = htons(54000);
	if (bind(this->serverfd, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Can't bind to IP/Port!" << std::endl;
		return (-2);
	}
	if (listen(this->serverfd, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen!" << std::endl;
		return (-3);
	}
	this->polls[0].fd = this->serverfd;
	this->polls[0].events = POLLIN;
}

int Server::connectUser()
{

}
