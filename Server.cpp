#include "Server.hpp"
#include "Commands.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password), _servername(SERVERNAME)
{
	// if (_checkPassword(password) == false)
	// {
	// 	std::cerr << "Wrong Password" << std::endl;
	// 	return;
	// }

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

Server::~Server()
{
}

Server &Server::operator=(Server const &src) {
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
					User new_user(userSocket);
					this->_users.insert(std::make_pair(userSocket, &new_user));
				}
				else
				{
					close(userSocket);
				}
			}
			// TODO readinput
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
	//TODO everything from here on has to be moved to the else and in there to a parser
	else // data from client
	{
		// Display message
		//TODO delete this after testing
		std::cout << "Received: " << std::endl << std::string(buf, 0, bytesRecv) << std::endl;
		std::cout << "---------------------" << std::endl;
		parseMessage(std::string(buf, 0, bytesRecv), clientfd);
		// Commands command(parseMessage(std::string(buf, 0, bytesRecv)));
		// command.determineCommand();
	}

	// Send message
	std::string nick = "you";
	std::string user = "you";
	std::string channel = "#test";
	std::string message = buf;

	std::ostringstream cmd;
	cmd << ":ben JOIN :ch1\r\n"
		<< ":local 332 ben ch1 :Welcome to the channel!\r\n"
		<< ":local 353 ben = ch1 :ben\r\n";
		// << ":Bene PRIVMSG ben@local :Hello are you receiving this message ?\r\n";
		// << "NICK " << nick << "\r\n"
		// << "USER " << user << " 0 * :" << user << "\r\n"
		// << "JOIN " << channel << "\r\n"
		// << "PRIVMSG " << channel << " :" << message << "\r\n";
	std::string cmd_str = cmd.str();

	// char *join = ":ben@local JOIN :ch1\r\n";

	// char *welcome = ":local 332 ben #ch1 :Welcome to the channel!\r\n";

	// char *userlist = ":local 353 ben = #ch1 :ben\r\n";

	send(clientfd, cmd_str.c_str(), cmd_str.size(), 0);
	// send(clientfd, join, sizeof(join), 0);
	// send(clientfd, welcome, sizeof(welcome), 0);
	// send(clientfd, userlist, sizeof(user), 0);
	// send(clientfd, buf, bytesRecv + 1, 0);
	//std::cout << "Message: " << message << std::endl;
}

void Server::mainLoop()
{
	while (1)
	{
		switch (poll(this->_polls, MAX_USER, TIMEOUT))
		{
		case 0:
			std::cout << "Ping." << std::endl
			break;
		case -1:
			std::cout << "There is an error" << std::endl;
			break;
		default:
			// std::cout << "begin of the default switch" << std::endl;
			this->connectUser();
			break;
		}
	}
}

void Server::pingUsers() {
	// std::time_t now = std::time(NULL);
	// std::string msg;
	// for (size_t i = 0; i < this->_users.size(); i++) {
	// 	User &user = *(this->_users.at(i + 1));
	// 	msg = "PING " + std::to_string(now) + "\r\n";
	// 	write(user.getFd(), msg.c_str(), msg.length());
	// }
}

bool Server::checkPassword(std::string password)
{
	if (password != this->_password)
		return false;
	return true;
}

//* Vector[0] ist immer der Command, Vector[1 bis n - 1] optionale Angaben z.B.: Channelnamen,... Vector[n] ist immer Plane Text "Hi, wie geht es dir?"
void Server::parseMessage(std::string input, int clientfd)
{
	std::vector<std::string> vec;
	while (input.find('\n') != std::string::npos)
	{
		size_t index = input.find('\n');
		std::string substring = input.substr(0, index);
		input = input.substr(index + 1, input.length() - (index + 1));
		// std::cout << "SUbstring: " << substring << std::endl;
		// std::cout << "Neuer Input: " << input << std::endl;
		std::vector<std::string> sub_vec;
		std::stringstream stream(substring);
		std::string word;
		while (stream >> word) {
			sub_vec.push_back(word);
		}
		Commands command(sub_vec, *(this->_users.find(clientfd)->second));
		command.determineCommand(*this);
	}
	
	// std::string raw;
	// std::string Text;
	// std::vector<std::string> vec;
	// size_t txt = input.find(':');
	// std::cout << "Input: " << input << std::endl;
	// std::cout << "---------------------" << std::endl;
	// if (txt != -1)
	// {
	// 	std::string Text = input.substr(txt);
	// 	raw =input.substr(0,txt);
	// }
	// vec = _find_str(raw, " ");
	// vec.push_back(Text);
	// std::cout << "Vec[0]: " << vec[0] << std::endl;
	// std::cout << "---------------------" << std::endl;
	//return vec;

}

// std::vector<std::string> Server::_find_str(std::string s, std::string del)
// {
// 	std::vector<std::string> vec;
// 	int end = s.find(del); 
// 	while (end != -1)
// 	{
// 		vec.push_back(s.substr(0, end));
// 		s.erase(s.begin(), s.begin() + end + 1);
// 		end = s.find(del);
// 	}
// 	vec.push_back(s.substr(0, end));
// 	return vec;
// }

std::map<int, User*> Server::getUsers() const
{
	return (this->_users);
}

void		Server::setChannel(Channel *channel)
{
	this->_channel.insert(std::make_pair<std::string, Channel*>(channel->getChannelName(), channel));
}

std::string	Server::getServername()
{
	return (this->_servername);
}

Channel		*Server::findChannel(std::string channelname)
{
	return(this->_channel[channelname]);
}
