#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
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

class User;
class Channel;

class Server
{
	private:
		std::map<int, User*>				_users;
		int									_port;
		std::string							_password;
		std::map<std::string, Channel *>	_channel;
		// std::vector<std::string>	_find_str(std::string s, std::string del);

	public:
		struct pollfd			_polls[MAX_USER];
		Server();
		Server(int port, std::string password);
		Server(Server const &src);
		~Server();
		Server &operator=(Server const &src);

		int			createServer();
		int			connectUser();
		void		readinput(int clientfd);
		void		parseMessage(std::string message, int clientfd);
		void		mainLoop();
		bool		checkPassword(std::string password);
		void		setChannel(Channel * channel);
		Channel		*findChannel(std::string channelname);
		std::map<int, User*> getUsers() const;
		
};

#endif