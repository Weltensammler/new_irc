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
		User						&_currUser;
		//*std::map<int, User*>		&_users;

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
		//*User	&_getUser(std::string UserNickname);//* neue Funktion um zu prüfen, ob eingegebener User existiert
		//*bool	_compare(User const &user1, User const &user2) const; //* um zwei User-Klassen miteinander vergleichen zu können
	public:
		Commands(std::vector<std::string> message, User &user);
		//*Commands(std::vector<std::string> message, std::map<int, User *> users, User &currUser);
		~Commands();
		void		determineCommand(Server &server);
		commandEnum	gettype();
		void		sendMessageToChannel(const Channel &channel, std::string string);
		//*void		sendPrivatMessage(const User &user, std::string string); //* Neue Funktion für private Nachrichten
		void		sendError(int errorCode, std::string arg);
};

#endif