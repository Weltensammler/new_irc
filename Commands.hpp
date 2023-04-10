#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "Server.hpp"
#include <vector>
#include "Answers.hpp"

enum commandEnum
{
	PASS,
	USER,
	NICK,
	PRIVMSG,
	NOTICE,
	JOIN,
	INVITE,
	TOPIC,
	KICK,
	MODE,
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
		void						inviteCommand();
		void						topicCommand();
		void						kickCommand();
		void						modeCommand();
		void						sendMessage();
		std::vector<std::string>	splitArgs(int i);
		bool						checkIfNicknameAlreadyUsed(std::string nickname);
		bool						_allowedCharacter(char c);
		bool						_validateString(const std::string &string);
		bool						checkIfOperator(std::string channel, int userfd);
		bool						checkIfUserOnChannel(std::string channel, int userfd);
		bool						checkMain(int MinMsgSize, int MaxMsgSize = -1);
	public:
									Commands(std::vector<std::string> message, int userfd, Server &server);
									~Commands();
		void						determineCommand();
		commandEnum					gettype();
		void						sendMessageToChannel(Channel *channel, std::string string, bool self);
		void						sendMessageToUser(std::string reason);
		void						sendReplyToUser(int errorCode, std::string arg, int userfd);
		void 						sendError(int errorCode, std::string arg);
		void						sendToAllUsers(std::map<int, User*> users, Channel *channel) const;
};

#endif