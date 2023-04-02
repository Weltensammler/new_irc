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
	INVITE,
	TOPIC,
	KILL,
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
		void						killCommand();
		void						kickCommand();
		void						modeCommand();
		void						sendMessage();
		std::vector<std::string>	splitArgs(int i);
		bool						checkIfNicknameAlreadyUsed(std::string nickname);
		bool						_allowedCharacter(char c);
		bool						_validateString(const std::string &string);
		void						deleteUser();
		void						removeUserFromChannels(int userfd);

	public:
									Commands(std::vector<std::string> message, int userfd, Server &server);
									~Commands();
		void						determineCommand();
		commandEnum					gettype();
		void						sendMessageToChannel(Channel *channel, std::string string, bool self);
		void						sendMessageToUser(std::string reason);
		void 						sendError(int errorCode, std::string arg);
};

#endif