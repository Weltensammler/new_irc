#include "Server.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password)
{
	if (_checkPassword(password) == false)
	{
		std::cerr << "Wrong Password" << std::endl;
		return;
	}
		
	for (int i = 0; i < 1024; i++)
	{
		this->_polls[i].fd = -1;
		this->_polls[i].events = 0;
		this->_polls[i].revents = 0;
	}
}

Server::~Server() {

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
	this->_polls[0].fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_polls[0].fd == -1)
	{
		std::cerr << "Can't create a socket!" << std::endl;
		return (-1);
	}
	if (fcntl(this->_polls[0].fd, F_SETFL, O_NONBLOCK))
	{
		std::cerr << "Can't set socket to nonblocking!" << std::endl;
		exit(-1);
	}
	int opt = 1;
	setsockopt(this->_polls[0].fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)); //ermöglicht reuse of ip Adressen die mit bind gebunden wurden
	sockaddr_in hostin;
	hostin.sin_family = AF_INET;													//Mögliche IP-Adressen (Aufbau der IP-Adresse) (Adress-family)
	hostin.sin_addr.s_addr = htonl(INADDR_ANY);									//Eigene IP-Adresse/ Da wo der Server läuft
	hostin.sin_port = htons(this->_port);												//Port, wodrüber der Server erreichbar ist
	if (bind(this->_polls[0].fd, (sockaddr *)&hostin, sizeof(hostin)) == -1)
	{
		std::cerr << "Can't bind to IP/Port!" << std::endl;
		return (-2);
	}
	if (listen(this->_polls[0].fd, 5) == -1)   //The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow.
	{
		std::cerr << "Can't listen!" << std::endl;
		return (-3);
	}
	this->_polls[0].events = POLLIN;
	return (this->_polls[0].fd);
}

int Server::connectUser()
{
	for (int i = 0; i < MAX_USER; i++)
	{
		if ((this->_polls[i].revents & POLLIN)) // fd is ready fo reading
		{
			if (this->_polls[i].fd == this->_polls[0].fd) // request for new connection
			{
				sockaddr_in user;
				socklen_t userSize = sizeof(user);
				char host[NI_MAXHOST];
				char service[NI_MAXSERV];
				memset(host, 0, NI_MAXHOST);
				memset(service, 0, NI_MAXSERV);
				int userSocket = accept(this->_polls[0].fd, (sockaddr *)&user, &userSize);
				if (userSocket == -1)
				{
					std::cerr << "Problem with client connecting!" << std::endl;
					return (-1);
				}
				// TODO the next 10 lines are not needed, just there for testing purpose
				int result = getnameinfo((sockaddr *)&user, userSize, host, NI_MAXHOST, service, NI_MAXSERV, 0);
				if (result)
				{
					std::cout << host << " connected on " << service << std::endl;
				}
				else
				{
					inet_ntop(AF_INET, &user.sin_addr, host, NI_MAXHOST);
					std::cout << host << " connected on " << ntohs(user.sin_port) << std::endl;
				}
				int j = 0;
				for (; j < MAX_USER; j++)
					if (this->_polls[j].fd == -1)
						break;
				if (j < MAX_USER)
				{
					this->_polls[j].fd = userSocket;
					this->_polls[j].events = POLLIN; //? do we need this line
					this->_polls[j].revents = 0;
				}
				else
				{
					close(userSocket);
				}
			}
			//TODO readinput
			else // data from an existing connection, recieve it
			{
				readinput(this->_polls[i].fd);
			}
		}
	}
	return (0);
}

void Server::readinput(int clientfd)
{
	// While receiving display message, echo message
	char buf[MAX_BUFFER];
	// Clear the buffer
	memset(buf, 0, MAX_BUFFER);
	// Wait for a message
	int bytesRecv = recv(clientfd, buf, MAX_BUFFER, 0);
	if (bytesRecv == -1)
	{
		std::cerr << "There was a connection issue!" << std::endl;
	}
	else if (bytesRecv == 0)
	{
		if (close(clientfd) == -1)
			std::cerr << "close!" << std::endl;
		std::cerr << "The Client disconnected!" << std::endl;
		clientfd *= -1; // it will be ignored in future
	}
	else // data from client
	{
		// Display message
		std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
	}

	// Send message
	std::string nick = "you";
	std::string user = "you";
	std::string channel = "#test";
	std::string message = buf;

	std::ostringstream cmd;
	cmd << "NICK " << nick << "\r\n"
	<< "USER " << user << " 0 * :" << user << "\r\n"
	<< "JOIN " << channel << "\r\n"
	<< "PRIVMSG " << channel << " :" << message << "\r\n";
	std::string cmd_str = cmd.str();

	send(clientfd, cmd_str.c_str(), cmd_str.size(), 0);
	// send(clientfd, buf, bytesRecv + 1, 0);
	std::cout << "Message: " << message << std::endl;
}


bool Server::_checkPassword(std::string password)
{
	if (password.length() != std::string(SERVER_PASSWORD).length())
		return false;
	if (password != std::string(SERVER_PASSWORD))
		return false;
	return true;
}