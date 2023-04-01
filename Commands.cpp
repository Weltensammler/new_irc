#include "Commands.hpp"

Commands::Commands(std::vector<std::string> message, int userfd, Server &server) : _userfd(userfd), _server(server) {
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

void	Commands::determineCommand()
{
	std::cout << "determine aufgerufen mit: " << this->gettype() << std::endl;
	switch (this->gettype())
	{
		case PASS:
			this->passCommand();
			break;
		case USER:
			this->userCommand();
			break;
		case NICK:
			this->nickCommand();
			break;
		case PRIVMSG:
			this->privmsgCommand();
			break;
		case NOTICE:
			this->noticeCommand();
			break;
		case JOIN:
			this->joinCommand();
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

void Commands::passCommand()
{
	std::cout << "Command PASS" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (_server.getPassword().empty())
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
	if (!_server.checkPassword(this->_message[1]))
	{
		std::cout << "PASS falsch!" << std::endl;
		sendError(ERR_PASSWDMISMATCH, "");
		return;
	}
	std::cout << "PASS richtig!" << std::endl;
	this->_server.findUserByFd(_userfd)->setAuth(true);
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
	this->_server.findUserByFd(_userfd)->setRealname(realname);
	this->_server.findUserByFd(_userfd)->setUsername(this->_message[1]);
}

void Commands::nickCommand() {
	std::cout << "Command NICK" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (this->_message.size() < 2) {
		std::cout << "Error: ERR_NONICKNAMEGIVEN" << std::endl;
		return sendError(ERR_NONICKNAMEGIVEN, "");
	}
	if (this->checkIfNicknameAlreadyUsed(this->_message[1]))
	{
		std::cout << "Error: ERR_NICKNAMEINUSE" << std::endl;
		return sendError(ERR_NICKNAMEINUSE, "");
	}
	if (this->_message[1].size() < 9 && _validateString(this->_message[1]))
	{
		// std::cout << "SetNickename "<< _message[1] << std::endl;
		// std::cout << "---------------------" << std::endl;
		// _server.printusers();
		this->_server.findUserByFd(_userfd)->setNickname(this->_message[1]);
		// std::cout << "userfd in nick Command: " << _userfd << " Nickname in nick Command: " << this->_message[1] << std::endl;
		// std::cout << "Nach set Nickname " << std::endl;
		// std::cout << "---------NICK------------" << std::endl;
		// _server.printusers();
	}
	else {
		std::cout << "Error: ERR_ERRONEUSNICKNAME" << std::endl;
		return sendError(ERR_ERRONEUSNICKNAME, "");
	}
}

void Commands::privmsgCommand() {
	std::cout << "Command PRIVMSG" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (this->_message.size() == 1) {
		return sendError(ERR_NORECIPIENT, "");
	}
	if (this->_message.size() < 3) {
		return sendError(ERR_NOTEXTTOSEND, "");
	}
	std::string	reciverNick = this->_message[1];
	if (reciverNick.at(0) == '#')
	{
		sendMessageToChannel(_server.findChannel(this->_message[1]), ":" + this->_server.findUserByFd(_userfd)->getNickname() + " " + "PRIVMSG" + " " + this->_message[1] + " :"+ this->_message[2] + "\r\n");
	}
	else
	{
		sendMessageToUser("PRIVMSG");
	}
}

void Commands::noticeCommand() {
	std::cout << "Command NOTICE" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::joinCommand()
{
	std::cout << "Command JOIN" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (this->_message.size() < 2) 
	{
		return sendError(ERR_NEEDMOREPARAMS, "");
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
		channel = _server.findChannel(channels[i]);
		if (!channel)
		{
			// try
			// {
				//channel does not exist
				channel = new Channel(channels[i]);
				_server.setChannel(channel);
				_server.findUserByFd(_userfd)->setChannel(channel);
				std::cout << "user join 207: "<< this->_server.findUserByFd(_userfd)->getUsername() << " user fd: " << _userfd << std::endl;
				channel->addUser(_userfd);
				channel->setOperator(_userfd);
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
			if (std::find(this->_server.findUserByFd(_userfd)->getChannels().begin(), this->_server.findUserByFd(_userfd)->getChannels().end(), channel) == _server.findUserByFd(_userfd)->getChannels().end())
			{
				this->_server.findUserByFd(_userfd)->setChannel(channel);
				channel->addUser(_userfd);
			}
		}
		sendMessageToChannel(channel, ":" + this->_server.findUserByFd(_userfd)->getUserInfo() + " " + "JOIN" + " :" + channel->getChannelName() + "\r\n");

		std::stringstream names;
		std::vector<int> users = channel->getUsers();
		std::vector<int> operators = channel->getOperators();
		names << ":" + _server.getServername() +" 353 " << _server.findUserByFd(_userfd)->getNickname() << " = " << channel->getChannelName() << " :";
		std::map<int, User*>::iterator it = this->_server.getItBegin();
		std::map<int, User*>::iterator end = this->_server.getItEnd();
		for (; it != end; it++)
		{
			if (channel->isOperator(it->first) || _server.findUserByFd(it->first)->isOperator()) {
				names << '@';
			}
			names << _server.findUserByFd(it->first)->getNickname();
			names << ' ';
		}
		names << "\r\n";
		std::string namesString = names.str();
		write(_userfd, namesString.c_str(), namesString.length());
		write(1, namesString.c_str(), namesString.length());

		std::stringstream endOfNamesList;
		endOfNamesList << ":" + _server.getServername() +" 366 " << _server.findUserByFd(_userfd)->getNickname() << " " << channel->getChannelName() << " :End of /NAMES list.\r\n";
		std::string endOfNamesListString = endOfNamesList.str();
		write(_userfd, endOfNamesListString.c_str(), endOfNamesListString.length());
		write(1, endOfNamesListString.c_str(), endOfNamesListString.length());
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

bool	Commands::checkIfNicknameAlreadyUsed(std::string nickname)
{
	std::map<int, User*>::iterator it;
	std::map<int, User*> users = _server.getUsers();
	for (it = users.begin(); it != users.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return (true);
	}
	return (false);
}

void Commands::deleteUser(User &user) {
	std::map<int, User*>::iterator it;
	std::map<int, User*> users = _server.getUsers();
	for (it = users.begin(); it != users.end(); ++it) {
		if ((*it->second).getFd() == user.getFd()) {
			// removeUserFromAllChannels(user, reason);
			users.erase(it);
			_server._polls[user.getFd()].fd = -1;
			break;
		}
	}
}

void Commands::sendError(int errorCode, std::string arg)
{
	std::string	msg = ":OurIRCServer ";
	std::stringstream	stream;
	std::string commandName = this->_message[0];
	stream << errorCode;
	msg += stream.str() + " " + _server.findUserByFd(_userfd)->getNickname();
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
	write(_userfd, msg.c_str(), msg.size());
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

void	Commands::sendMessageToChannel(Channel *channel, std::string string) {
	std::vector<int>	users = channel->getUsers();

	for (std::vector<int>::iterator it = users.begin(); it != users.end(); it++)
	{
		int fd = *it;
		write(fd, string.c_str(), string.length());
		std::cout << "fd = " << fd << std::endl;
		write(1, string.c_str(), string.length());
		std::cout <<std::endl;
	}
}

void Commands::sendMessageToUser(std::string reason)
{
	if (this->_message.size() == 1) {
		return sendError(ERR_NORECIPIENT, "");
	}
	if (this->_message.size() < 3) {
		return sendError(ERR_NOTEXTTOSEND, "");
	}

	std::stringstream	logStream;
	std::string			reciverNick = this->_message[1];
	std::string			message = this->_message[2];
	User				*reciver = 0;


	// if (toLowercase(reciverNick) == toLowercase(_botName)) {
	// 	handleBotMessage(command);
	// 	return;
	// }
	reciver = _server.findUserByNick(reciverNick);
	if (reciver) {
		std::stringstream toSend;
		toSend << ':' + reciver->getUserInfo() << " " <<
		reason << " " << reciverNick << " :" << message << "\r\n";
		std::string str = toSend.str();
		write(reciver->getFd(), str.c_str(), str.size());
		// logger.logUserMessage(str, *reciver, OUT);
	}
	else {
		this->sendError(ERR_NOSUCHNICK, "");
	}
}
