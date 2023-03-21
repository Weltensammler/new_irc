#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "Server.hpp"
#include <vector>
#include "Answers.hpp"
// #include "Utils.hpp"

enum commandEnum
{
	PASS,
	USER,
	NICK,
	PRIVMSG,
	NOTICE,
	JOIN,
	OPER,
	QUIT,
	KILL,
	KICK,
	LIST,
	WHO,
	PART,
	MESSAGE
};

class User;

class Commands
{
	private:
		std::string					_servername;
		commandEnum					_type;
		std::vector<std::string>	_message;
		User						&_user;

		void						passCommand(Server &server);
		void						userCommand();
		void						nickCommand(Server &server);
		void						privmsgCommand();
		void						noticeCommand();
		void						joinCommand(Server &server);
		void						operCommand();
		void						quitCommand();
		void						killCommand();
		void						kickCommand();
		void						listCommand();
		void						whoCommand();
		void						partCommand();
		void						sendMessage();
		std::vector<std::string>	splitArgs(int i);
		bool	checkIfNicknameAlreadyUsed(std::string nickname, Server &server);
		bool	_allowedCharacter(char c);
		bool	_validateString(const std::string &string);
		void	deleteUser(User &user, Server &server);
	public:
		Commands(std::vector<std::string> message, User &user);
		~Commands();
		void		determineCommand(Server &server);
		commandEnum	gettype();
		void		sendMessageToChannel(const Channel &channel, std::string string);
		void 		sendError(int errorCode, std::string arg);
};

#endif