#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <algorithm>
#include <ctime>
#include <string>
#include <sstream>
#include <poll.h>
#include <map>
#include <vector>
#include <fcntl.h>
#include "User.hpp"
#include "Channel.hpp"


#define MAX_USER		1024
#define MAX_BUFFER		4096
#define TIMEOUT			42000
#define SERVERNAME		"OurIRCServer"

class User;
class Channel;

class Server
{
	private:
		std::string							_servername;
		std::map<int, User*>				_users;
		int									_port;
		std::string							_password;
		std::map<std::string, Channel *>	_channel;
		std::map<int, std::string>			_storedmsg;

	public:
		struct pollfd						_polls[MAX_USER];
											Server();
											Server(int port, std::string password);
											Server(Server const &src);
											~Server();
		Server								&operator=(Server const &src);

		int									createServer();
		int									connectUser();
		void								readinput(int clientfd);
		void								parseMessage(std::string message, int clientfd);
		void								mainLoop();
		bool								checkPassword(std::string password);
		std::string							getPassword() const;
		void								setChannel(Channel *channel);
		void								pingUsers();
		std::string							getServername();
		Channel								*findChannel(std::string channelname);
		std::map<int, User*>				getUsers() const;
		User								*findUserByNick(std::string nickname);
		User								*findUserByFd(int userfd);
		void								printusers();
		std::map<int, User*>::iterator		getItBegin();
		std::map<int, User*>::iterator		getItEnd();
};

#endif