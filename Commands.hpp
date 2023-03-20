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
		commandEnum					_type;
		std::vector<std::string>	_message;
		User						&_user;

		void						passCommand(Server &server);
		void						userCommand();
		void						nickCommand(Server &server);
		void						pongCommand();
		void						privmsgCommand();
		void						noticeCommand();
		void						joinCommand(Server &server);
		void						operCommand();
		void						quitCommand();
		void						killCommand();
		void						kickCommand();
		void						pingCommand();
		void						listCommand();
		void						whoCommand();
		void						partCommand();
		void						sendMessage();
		std::vector<std::string>	splitArgs(int i);
		bool	checkIfNicknameAlreadyUsed(std::string nickname, Server &server);
	public:
		Commands(std::vector<std::string> message, User &user);
		~Commands();
		void		determineCommand(Server &server);
		commandEnum	gettype();
		
};

#endif