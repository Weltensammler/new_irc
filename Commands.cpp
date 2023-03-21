#include "Commands.hpp"

Commands::Commands(std::vector<std::string> message, User &user) : _user(user) {
	if (message[0] == "PASS") {this->_type = PASS;}
	else if (message[0] == "USER") {this->_type = USER;}
	else if (message[0] == "NICK") {this->_type = NICK;}
	else if (message[0] == "PRIVMSG") {this->_type = PRIVMSG;}
	else if (message[0] == "NOTICE") {this->_type = NOTICE;}
	else if (message[0] == "JOIN") {this->_type = JOIN;}
	else if (message[0] == "OPER") {this->_type = OPER;}
	else if (message[0] == "QUIT") {this->_type = QUIT;}
	else if (message[0] == "KILL") {this->_type = KILL;}
	else if (message[0] == "KICK") {this->_type = KICK;}
	else if (message[0] == "LIST") {this->_type = LIST;}
	else if (message[0] == "WHO") {this->_type = WHO;}
	else if (message[0] == "PART") {this->_type = PART;}
	else {this->_type = MESSAGE;}
	this->_message = message;
	std::cout << "Message is: " << (this->_message[0]) << std::endl;
	std::cout << "---------------------" << std::endl;
}

Commands::~Commands() {}

void	Commands::determineCommand(Server &server)
{
	std::cout << "determine aufgerufen mit: " << this->gettype() << std::endl;
	switch (this->gettype())
	{
		case PASS:
			this->passCommand(server);
			break;
		case USER:
			this->userCommand();
			break;
		case NICK:
			this->nickCommand(server);
			break;
		case PRIVMSG:
			this->privmsgCommand();
			break;
		case NOTICE:
			this->noticeCommand();
			break;
		case JOIN:
			this->joinCommand(server);
			break;
		case OPER:
			this->operCommand();
			break;
		case QUIT:
			this->quitCommand();
			break;
		case KILL:
			this->killCommand();
			break;
		case KICK:
			this->kickCommand();
			break;
		case LIST:
			this->listCommand();
			break;
		case WHO:
			this->whoCommand();
			break;
		case PART:
			this->partCommand();
			break;
		default:
			this->sendMessage();
			break;
	};
}

commandEnum	Commands::gettype() {
	return (this->_type);
}

void Commands::passCommand(Server &server) {
	std::cout << "Command PASS" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (server.getPassword().empty())
	{
		std::cout << "PASS leer!" << std::endl;
		return;
	}
	if (this->_message[1].empty())
	{
		std::cout << "PASS zu wenig!" << std::endl;
		sendError(ERR_NEEDMOREPARAMS, "");
		return;
	}
	if (!server.checkPassword(this->_message[1]))
	{
		std::cout << "PASS falsch!" << std::endl;
		sendError(ERR_PASSWDMISMATCH, "");
		return;
	}
	std::cout << "PASS richtig!" << std::endl;
	std::map<int, User*> users = server.getUsers();
	User &user = *(users.find(this->_user.getFd())->second);
	user.setAuth(true);
	return;
}

void Commands::userCommand() {
	std::cout << "Command USER" << std::endl;
	std::cout << "---------------------" << std::endl;
	std::size_t messageSize = this->_message.size();
	if (messageSize < 5) {
		//return sendError(command, ERR_NEEDMOREPARAMS);
		std::cout << "Error: ERR_NEEDMOREPARAMS" << std::endl;
		return;
	}
	std::string realname = "";
	int i = 0;
	while (messageSize > 4)
	{
		realname += this->_message[4 + i++];
		messageSize--;
	}
	this->_user.setRealname(realname);
	this->_user.setUsername(this->_message[1]);
}

void Commands::nickCommand(Server &server) {
	std::cout << "Command NICK" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (this->_message.size() < 2) {
		std::cout << "Error: ERR_NONICKNAMEGIVEN" << std::endl;
		return sendError(ERR_NONICKNAMEGIVEN, "");
	}
	if (this->checkIfNicknameAlreadyUsed(this->_message[1], server))
	{
		std::cout << "Error: ERR_NICKNAMEINUSE" << std::endl;
		return sendError(ERR_NICKNAMEINUSE, "");
	}
	if (this->_message[1].size() < 9 && _validateString(this->_message[1]))
		this->_user.setNickname(this->_message[1]);
	else {
		std::cout << "Error: ERR_ERRONEUSNICKNAME" << std::endl;
		return sendError(ERR_ERRONEUSNICKNAME, "");
	}
}

void Commands::privmsgCommand() {
	std::cout << "Command PRIVMSG" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::noticeCommand() {
	std::cout << "Command NOTICE" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::joinCommand(Server &server)
{
	std::cout << "Command JOIN" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (this->_message.size() < 2) 
	{
		std::cout << "No Channel given" << std::endl;
		// return sendError(command, ERR_NEEDMOREPARAMS);
	}
	std::vector<std::string> channels = splitArgs(1);
	for(int i = 0; i < channels.size(); i++)
	{
		// std::string const	&channelName = command.getArgument(0);
		//? Not needed?
		//? User				&user = command.getUser();
		Channel				*channel;
		if (channels[i][0] != '#')
		{
			//sendError(command, 403);
			std::cout << "Wrong channel name!" << std::endl;
			return;
		}
		channel = server.findChannel(channels[i]);
		if (!channel)
		{
			// try
			// {
				//channel does not exist
				channel = new Channel(channels[i]);
				server.setChannel(channel);
				_user.setChannel(channel);
				channel->addUser(_user);
				channel->setOperator(_user);
			// }
			//TODO exception
			// catch (FtException &e)
			// {
			// 	std::cout << "Wrong Channelname!" << std::endl;
			// 	return sendError(ERR_NOSUCHCHANNEL, NULL);
			// }
			// return;
		}
		else
		{
			if (std::find(_user.getChannels().begin(), _user.getChannels().end(), channel) == _user.getChannels().end())
			{
				_user.setChannel(channel);
				channel->addUser(_user);
			}
		}
		//TODO send message function
		sendMessageToChannel(*channel, ":" + _user.getUserInfo() + " " + "JOIN" + " :" + channel->getChannelName() + "\r\n");

		std::stringstream names;
		std::map<std::string, User *> users = channel->getUsers();
		std::map<std::string, User *> operators = channel->getOperators();
		names << ":" + server.getServername() +" 353 " << _user.getNickname() << " = " << channel->getChannelName() << " :";

		for (std::map<std::string, User *>::iterator it = users.begin(); it != users.end(); ++it) {
			if (channel->isOperator(it->first) || it->second->isOperator()) {
				names << '@';
			}
			names << it->second->getNickname();
			//TODO find out why this is needed
			// if ((it + 1) != users.end())
			// 	names << ' ';
		}
		names << "\r\n";
		std::string namesString = names.str();
		write(_user.getFd(), namesString.c_str(), namesString.length());

		std::stringstream endOfNamesList;
		endOfNamesList << ":" + server.getServername() +" 366 " << _user.getNickname() << " " << channel->getChannelName() << " :End of /NAMES list.\r\n";
		std::string endOfNamesListString = endOfNamesList.str();
		write(_user.getFd(), endOfNamesListString.c_str(), endOfNamesListString.length());
	}
}

void Commands::operCommand() {
	std::cout << "Command OPER" << std::endl;
	std::cout << "---------------------" << std::endl;

}

void Commands::quitCommand() {
	std::cout << "Command QUIT" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::killCommand() {
	std::cout << "Command KILL" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::kickCommand() {
	std::cout << "Command KICK" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::listCommand() {
	std::cout << "Command LIST" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::whoCommand() {
	std::cout << "Command WHO" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::partCommand() {
	std::cout << "Command PART" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::sendMessage() {
	std::cout << "Command SEND MESSAGE" << std::endl;
	std::cout << "---------------------" << std::endl;
}

std::vector<std::string>	Commands::splitArgs(int i)
{
	std::vector<std::string>	singleArgs;
	std::string					args = _message[i];

	while (args.find(',') != std::string::npos)
	{
		size_t index = args.find(',');
		std::string substring = args.substr(0, index);
		args = args.substr(index + 1, args.length());
		singleArgs.push_back(substring);
	}
	singleArgs.push_back(args);
	return (singleArgs);
}

bool	Commands::checkIfNicknameAlreadyUsed(std::string nickname, Server &server)
{
	std::map<int, User*>::iterator it;
	std::map<int, User*> users = server.getUsers();
	for (it = users.begin(); it != users.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (true);
	}
	return (false);
}

void Commands::deleteUser(User &user, Server &server) {
	std::map<int, User*>::iterator it;
	std::map<int, User*> users = server.getUsers();
	for (it = users.begin(); it != users.end(); ++it) {
		if ((*it->second).getFd() == user.getFd()) {
			// removeUserFromAllChannels(user, reason);
			users.erase(it);
			server._polls[user.getFd()].fd = -1;
			break;
		}
	}
}

void Commands::sendError(int errorCode, std::string arg)
{
	std::string	msg = ":My_IRC ";
	std::stringstream	stream;
	std::string commandName = this->_message[0];
	stream << errorCode;
	msg += stream.str() + " " + _user.getNickname();
	switch (errorCode)
	{
		case ERR_NOSUCHNICK:
			msg += ' ' + arg + " :No such nick/channel\n";
			break;
		case ERR_NOSUCHSERVER:
			msg += " :No such server\n";
			break;
		case ERR_NOSUCHCHANNEL:
			msg += ' ' + arg + " :No such channel\n";
			break;
		case ERR_CANNOTSENDTOCHAN:
			msg += " :Cannot send to channel\n";
			break;
		case ERR_TOOMANYCHANNELS:
			msg += " :You have joined too many channels\n";
			break;
		case ERR_WASNOSUCHNICK:
			msg += " :There was no such nickname\n";
			break;
		case ERR_TOOMANYTARGETS:
			msg += " :Duplicate recipients delivered\n";
			break;
		case ERR_NOORIGIN:
			msg += " :No origin specified\n";
			break;
		case ERR_NORECIPIENT:
			msg += " :No recipient given (" + commandName + ")\n";
			break;
		case ERR_NOTEXTTOSEND:
			msg += " :No text to send\n";
			break;
		case ERR_NOTOPLEVEL:
			msg += " " + commandName + " :No toplevel domain specified\n";
			break;
		case ERR_WILDTOPLEVEL:
			msg += " " + commandName + " :Wildcard in toplevel domain\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg += " " + commandName + " :Unknown command\n";
			break;
		case ERR_NOMOTD:
			msg += " :MOTD File is missing\n";
			break;
		case ERR_NOADMININFO:
			msg += " " + commandName + " :No administrative info available\n";
			break;
		case ERR_NONICKNAMEGIVEN:
			msg += " :No nickname given\n";
			break;
		case ERR_ERRONEUSNICKNAME:
			msg += " " + arg + " :Erroneus nickname\n";
			break;
		case ERR_NICKNAMEINUSE:
			msg += " * :Nickname is already in use.\n";
			break;
		case ERR_NICKCOLLISION:
			msg += " " + commandName + " :Nickname collision KILL\n";
			break;
		case ERR_USERNOTINCHANNEL:
			msg += " " + commandName + " :They aren't on that channel\n";
			break;
		case ERR_NOTONCHANNEL:
			msg += " " + commandName + " :You're not on that channel\n";
			break;
		case ERR_USERONCHANNEL:
			msg += " " + commandName + " :is already on channel\n";
			break;
		case ERR_NOLOGIN:
			msg += " " + commandName + " :User not logged in\n";
			break;
		case ERR_SUMMONDISABLED:
			msg += " :SUMMON has been disabled\n";
			break;
		case ERR_USERSDISABLED:
			msg += " :USERS has been disabled\n";
			break;
		case ERR_NOTREGISTERED:
			msg += " :You have not registered\n";
			break;
		case ERR_NEEDMOREPARAMS:
			msg += " " + commandName + " :Not enough parameters\n";
			break;
		case ERR_ALREADYREGISTRED:
			msg += " :You may not reregister\n";
			break;
		case ERR_NOPERMFORHOST:
			msg += " :Your host isn't among the privileged\n";
			break;
		case ERR_PASSWDMISMATCH:
			msg += " :Password incorrect\n";
			break;
		case ERR_YOUREBANNEDCREEP:
			msg += " :You are banned from this server\n";
			break;
		case ERR_KEYSET:
			msg += " " + commandName + " :Channel key already set\n";
			break;
		case ERR_CHANNELISFULL:
			msg += " " + commandName + " :Cannot join channel (+l)\n";
			break;
		case ERR_UNKNOWNMODE:
			msg += " :is unknown mode char to me\n";
			break;
		case ERR_INVITEONLYCHAN:
			msg += " " + commandName + " :Cannot join channel (+i)\n";
			break;
		case ERR_BANNEDFROMCHAN:
			msg += " " + commandName + " :Cannot join channel (+b)\n";
			break;
		case ERR_BADCHANNELKEY:
			msg += " " + commandName + " :Cannot join channel (+k)\n";
			break;
		case ERR_NOPRIVILEGES:
			msg += " :Permission Denied- You do not have the correct IRC Operator privileges\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			msg += " " + commandName + " :You're not channel operator\n";
			break;
		case ERR_CANTKILLSERVER:
			msg += " :You cant kill a server!\n";
			break;
		case ERR_NOOPERHOST:
			msg += " :No O-lines for your host\n";
			break;
		case ERR_UMODEUNKNOWNFLAG:
			msg += " :Unknown MODE flag\n";
			break;
		case ERR_USERSDONTMATCH:
			msg += " :Cant change mode for other users\n";
			break;
		default:
			msg += "UNKNOWN ERROR\n";
			break;
	}
	write(_user.getFd(), msg.c_str(), msg.size());
}

bool	Commands::_allowedCharacter(char c)
{
	if ((c >= 65 && c <= 90) /*'A'-'Z'*/ || (c >= 97 && c <= 123) /*'a'-'z'*/ || (c >= 48 && c <= 57) /*'0'-'9'*/ || (c == '-' || c == '_'))
		return true;
	return false;
}

bool	Commands::_validateString(const std::string &string)
{
	for (std::vector<const char>::iterator it = string.begin(); it != string.end(); ++it)
		if (!_allowedCharacter(*it))
			return false;
	return true;
}

void	Commands::sendMessageToChannel(const Channel &channel, std::string string) {
	std::map<std::string, User *>	users = channel.getUsers();

	for (std::map<std::string, User *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		int fd = it->second->getFd();
		write(fd, string.c_str(), string.length());
	}
}