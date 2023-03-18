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


#define MAX_USER		1024
#define MAX_BUFFER		4096
#define SERVER_PASSWORD	"1234"
#define TIMEOUT			42000

class Server
{
	private:
		std::map<int, User*>	_users;
		int						_port;
		std::string				_password;
		

		Server();
		Server(Server const &src);
		Server &operator=(Server const &src);

		bool						_checkPassword(std::string password);
		std::vector<std::string>	_find_str(std::string s, std::string del);

	public:
		struct pollfd			_polls[MAX_USER];
		Server(int port, std::string password);
		~Server();

		int							createServer();
		int							connectUser();
		void						readinput(int clientfd);
		std::vector<std::string>	parseMessage(std::string message);
		void						mainLoop();

		
};

#endif