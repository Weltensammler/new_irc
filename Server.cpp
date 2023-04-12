#include "Server.hpp"
#include "Commands.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password), _servername(SERVERNAME)
{
	for (int i = 0; i < 1024; i++)
	{
		this->_polls[i].fd = -1;
		this->_polls[i].events = 0;
		this->_polls[i].revents = 0;
	}
}

Server::Server()
{
	this->_password = "1234";
	this->_port = 54000;
}

Server::~Server() {}

Server &Server::operator=(Server const &src)
{
	this->_password = src._password;
	this->_port = src._port;
	for (int i = 0; i < 1024; i++)
	{
		this->_polls[i].fd = src._polls[i].fd;
		this->_polls[i].events = src._polls[i].events;
		this->_polls[i].revents = src._polls[i].revents;
	}
	return (*this);
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
int Server::createServer()
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
	setsockopt(this->_polls[0].fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // ermöglicht reuse of ip Adressen die mit bind gebunden wurden
	sockaddr_in hostin;
	hostin.sin_family = AF_INET;				// Mögliche IP-Adressen (Aufbau der IP-Adresse) (Adress-family)
	hostin.sin_addr.s_addr = htonl(INADDR_ANY); // Eigene IP-Adresse/ Da wo der Server läuft
	hostin.sin_port = htons(this->_port);		// Port, wodrüber der Server erreichbar ist
	if (bind(this->_polls[0].fd, (sockaddr *)&hostin, sizeof(hostin)) == -1)
	{
		std::cerr << "Can't bind to IP/Port!" << std::endl;
		return (-2);
	}
	if (listen(this->_polls[0].fd, 5) == -1) // The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow.
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
				std::cout << "user ip " << inet_ntoa(user.sin_addr) << std::endl;
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
					this->_polls[j].events = POLLIN;
					this->_polls[j].revents = 0;
					User* new_user = new User(userSocket);
					this->_users.insert(std::make_pair(userSocket, new_user));
				}
				else
				{
					close(userSocket);
				}
			}
			else // data from an existing connection, recieve it
				readinput(this->_polls[i].fd);
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
		if (_storedmsg.find(clientfd) != _storedmsg.end())
			_storedmsg.erase(_storedmsg.find(clientfd));
		if (close(clientfd) == -1)
			std::cerr << "close!" << std::endl;
		std::cerr << "The Client disconnected!" << std::endl;
		clientfd *= -1; // it will be ignored in future
	}
	else // data from client
	{
		std::cout << "Received: " << std::endl << std::string(buf, 0, bytesRecv) << "clientfd: " << clientfd << std::endl;
		std::cout << "---------------------" << std::endl;
		if (_storedmsg.find(clientfd) == _storedmsg.end() && std::string(buf, 0, bytesRecv).back() == '\n')
			parseMessage(std::string(buf, 0, bytesRecv), clientfd);
		else
		{
			_storedmsg[clientfd] = _storedmsg[clientfd] + std::string(buf, 0, bytesRecv);
			std::cout << "stored message: " << _storedmsg[clientfd] << std::endl;
			if (_storedmsg[clientfd].back() == '\n')
			{
				parseMessage(_storedmsg[clientfd], clientfd);
				_storedmsg.erase(clientfd);
			}
		}
	}
}

void Server::mainLoop()
{
	while (1)
	{
		switch (poll(this->_polls, MAX_USER, TIMEOUT))
		{
		case 0:
			pingUsers();
			break;
		case -1:
			std::cout << "There is an error" << std::endl;
			break;
		default:
			this->connectUser();
			break;
		}
	}
}

void Server::pingUsers() {
	std::time_t now = std::time(NULL);
	std::string msg;
	std::map<int, User*>::iterator it;
	for (it = this->_users.begin(); it != this->_users.end(); it++)
	{
		msg = "PING " + std::to_string(now) + "\r\n";
		write(it->first, msg.c_str(), msg.length());
	}
}

bool Server::checkPassword(std::string password)
{
	if (password != this->_password)
		return false;
	return true;
}

void Server::parseMessage(std::string input, int clientfd)
{
	std::vector<std::string> com_vec;
	std::string copyInput = input;

	while (copyInput.find("\n") != std::string::npos)
	{
		std::string sub = copyInput.substr(0, copyInput.find("\n"));
		com_vec.push_back(sub);
		copyInput = copyInput.substr(copyInput.find("\n") + 1);
	}

	std::vector<std::string>::iterator it = com_vec.begin();
	for (;it != com_vec.end(); it++)
	{
		std::vector<std::string> sub_vec;
		if ((*it).find(":") != std::string::npos)
		{
			std::string first = (*it).substr(0, (*it).find(":"));
			std::string second = (*it).substr((*it).find(":") + 1);
			std::stringstream stream(first);
			std::string word;
			while (stream >> word)
				sub_vec.push_back(word);
			sub_vec.push_back(second);
		}
		else
		{
			std::stringstream stream((*it));
			std::string word;
			while (stream >> word)
				sub_vec.push_back(word);
		}
		Commands* command = new Commands(sub_vec, clientfd, *this);
		command->determineCommand();
		delete command;
	}
}

std::map<int, User*> Server::getUsers() const
{
	return (this->_users);
}

void		Server::setChannel(Channel *channel)
{
	std::pair<std::string, Channel*> pair = make_pair(channel->getChannelName(), channel);
	this->_channel.insert(pair);
	std::cout << "set channel channelname: " << channel->getChannelName() << std::endl;
}

std::string	Server::getServername()
{
	return (this->_servername);
}

Channel		*Server::findChannel(std::string channelname)
{
	Channel	*foundchannel;
	std::map<std::string, Channel*>::iterator it;

	for (it = _channel.begin(); it != _channel.end(); it++)
	{
		if (it->second->getChannelName() == channelname)
			return (foundchannel = it->second);
	}
	return (0);
}

std::string	Server::getPassword() const
{
	return (this->_password);
}

User	*Server::findUserByNick(std::string nickname)
{
	User	*founduser;
	std::map<int, User*>::iterator it;

	for (it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (founduser = it->second);
	}
	return (0);
}

User	*Server::findUserByFd(int userfd)
{
	User	*founduser = _users.find(userfd)->second;
	return (founduser);
}

void		Server::printusers()
{
	std::map<int, User *>::iterator it = _users.begin();
	std::cout << "Es folgen alle User---------------------------" << std::endl;
	for (; it != _users.end(); it++)
	{
		std::cout << "FD: " << it->first << "\nUsername: " << it->second->getUsername() << "\nNickname: " << it->second->getNickname() << std::endl;
	}
	std::cout << "----------------------------------------------" << std::endl;
}

std::map<int, User*>::iterator	Server::getItBegin()
{
	return _users.begin();
}

std::map<int, User*>::iterator	Server::getItEnd()
{
	return _users.end();
}
