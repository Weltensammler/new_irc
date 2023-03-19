#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "Server.hpp"
#include <vector>

enum commandEnum
{
	PASS,
	USER,
	NICK,
	PONG,
	PRIVMSG,
	NOTICE,
	JOIN,
	OPER,
	QUIT,
	KILL,
	KICK,
	PING,
	LIST,
	WHO,
	PART,
	MESSAGE
};

class Commands
{
	private:
		commandEnum	_type;
		std::vector<std::string> _message;

		void	passCommand(Server &server);
		void	userCommand();
		void	nickCommand();
		void	pongCommand();
		void	privmsgCommand();
		void	noticeCommand();
		void	joinCommand();
		void	operCommand();
		void	quitCommand();
		void	killCommand();
		void	kickCommand();
		void	pingCommand();
		void	listCommand();
		void	whoCommand();
		void	partCommand();
		void	sendMessage();
	public:
		Commands(std::vector<std::string> message);
		~Commands();
		void		determineCommand(Server &server, int clientfd);
		commandEnum	gettype();
};

#endif