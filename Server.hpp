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
#include "User.hpp"

class Server
{
	private:
		struct pollfd			polls[1024];
		int						serverfd;
		std::map<int, User*>	users;

	public:
		Server();
		~Server();

		int		createServer();
		int		connectUser();

};

#endif