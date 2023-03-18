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
	PART
};

class Commands
{
	private:
		commandEnum	_type;

		void	passCommand();
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
		void		determineCommand(std::vector<std::string> message);
		commandEnum	gettype();
};

#endif