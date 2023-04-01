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
class Channel;

class Commands
{
	private:
		std::string					_servername;
		Server						&_server;
		commandEnum					_type;
		std::vector<std::string>	_message;
		int							_userfd;

		void						passCommand();
		void						userCommand();
		void						nickCommand();
		void						privmsgCommand();
		void						noticeCommand();
		void						joinCommand();
		void						operCommand();
		void						quitCommand();
		void						killCommand();
		void						kickCommand();
		void						listCommand();
		void						whoCommand();
		void						partCommand();
		void						sendMessage();
		std::vector<std::string>	splitArgs(int i);
		bool	checkIfNicknameAlreadyUsed(std::string nickname);
		bool	_allowedCharacter(char c);
		bool	_validateString(const std::string &string);
		void	deleteUser(User &user);
	public:
		Commands(std::vector<std::string> message, int userfd, Server &server);
		~Commands();
		void		determineCommand();
		commandEnum	gettype();
		void		sendMessageToChannel(Channel *channel, std::string string);
		void		sendMessageToUser(std::string reason);
		void 		sendError(int errorCode, std::string arg);
};

#endif