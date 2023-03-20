#include "Commands.hpp"
#include "Answers.hpp"

Commands::Commands(std::vector<std::string> message, User &user) : _user(user) {
	if (message[0] == "PASS") {this->_type = PASS;}
	else if (message[0] == "USER") {this->_type = USER;}
	else if (message[0] == "NICK") {this->_type = NICK;}
	else if (message[0] == "PONG") {this->_type = PONG;}
	else if (message[0] == "PRIVMSG") {this->_type = PRIVMSG;}
	else if (message[0] == "NOTICE") {this->_type = NOTICE;}
	else if (message[0] == "JOIN") {this->_type = JOIN;}
	else if (message[0] == "OPER") {this->_type = OPER;}
	else if (message[0] == "QUIT") {this->_type = QUIT;}
	else if (message[0] == "KILL") {this->_type = KILL;}
	else if (message[0] == "KICK") {this->_type = KICK;}
	else if (message[0] == "PING") {this->_type = PING;}
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
		case PONG:
			this->pongCommand();
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
		case PING:
			this->pingCommand();
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
	if (!server.checkPassword(this->_message[1]))
	{
		std::cout << "PASS falsch!" << std::endl;
		return;
	}
	std::cout << "PASS richtig!" << std::endl;
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
		//return sendError(command, ERR_NONICKNAMEGIVEN);
		std::cout << "Error: ERR_NONICKNAMEGIVEN" << std::endl;
		return;
	}

}

void Commands::pongCommand() {
	std::cout << "Command PONG" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::privmsgCommand() {
	std::cout << "Command PRIVMSG" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::noticeCommand() {
	std::cout << "Command NOTICE" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::joinCommand(Server &server) {
	std::cout << "Command JOIN" << std::endl;
	std::cout << "---------------------" << std::endl;
// 	if (this->_message.size() < 2) {
// 		std::cout << "No Channel given" << std::endl;
// 		// return sendError(command, ERR_NEEDMOREPARAMS);
// 	}
// 	std::vector<std::string> channels = splitArgs(1);
// 	for(int i = 0; i < channels.size(); i++)
// 	{
// 		// std::string const	&channelName = command.getArgument(0);
// 		//? Not needed?
// 		//? User				&user = command.getUser();
// 		Channel				*channel;

// 		if (channels[i].at(0) != '#') {
// 			//sendError(command, 403);
// 			std::cout << "Wrong channel name!" << std::endl;
// 			return;
// 		}
// 		channel = server.findChannel(channels[i]);
// 		if (!channel)
// 		{
// 			try
// 			{
// 				//channel does not exist
// 				channel = new Channel(channels[i]);
// 				server.setChannel(channel);
// 				_user.setChannel(channel);
// 				//TODO ab hier
// 				channel->addUser(user);
// 				channel->setOper(&user);
// 			}
// 			catch (FtException &e)
// 			{
// 				//sendError(command, ERR_NOSUCHCHANNEL);
// 				std::cout << "Wrong Channelname!" << std::endl;
// 			}
// 				return;
// 			}
// 		}
// 		else
// 		{
// 			if (std::find(user.getChannels().begin(), user.getChannels().end(), channel) == user.getChannels().end())
// 			{
// 				user.addChannel(channel);
// 				channel->addUser(user);
// 			}
// 		}
// 		sendMessageToChannel(*channel, ":" + user.getUserInfoString() + " " + "JOIN" + " :" + channel->getName() + "\r\n");

// 		std::stringstream names;
// 		std::vector<User *> users = channel->getUsers();
// 		std::vector<User *> operators = channel->getOperators();
// 		names << ":" + _serverName +" 353 " << user.getNick() << " = " << channel->getName() << " :";

// 		for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it) {
// 			if (channel->isOperator((*it)) || (*it)->isOper()) {
// 				names << '@';
// 			}
// 			names << (*it)->getNick();
// 			if ((it + 1) != users.end())
// 				names << ' ';
// 		}
// 		names << "\r\n";
// 		std::string namesString = names.str();
// 		write(user.getFd(), namesString.c_str(), namesString.length());
// 		logger.logUserMessage(namesString, user, OUT);

// 		std::stringstream endOfNamesList;
// 		endOfNamesList << ":" + _serverName +" 366 " << user.getNick() << " " << channel->getName() << " :End of /NAMES list.\r\n";
// 		std::string endOfNamesListString = endOfNamesList.str();
// 		write(user.getFd(), endOfNamesListString.c_str(), endOfNamesListString.length());
// 		logger.logUserMessage(endOfNamesListString, user, OUT);
// 	}
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

void Commands::pingCommand() {
	std::cout << "Command PING" << std::endl;
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

// void Commands::sendError(Command const &command, int errorCode)
// {
// 	std::string	msg = ":My_IRC ";
// 	std::stringstream	stream;
// 	User &user = command.getUser();
// 	std::string commandName = command.typeToString();

// 	stream << errorCode;
// 	msg += stream.str() + " " + user.getNick();
// 	switch (errorCode)
// 	{
// 		case ERR_NOSUCHNICK:
// 			msg += ' ' + command.getArgument(0) + " :No such nick/channel\n";
// 			break;
// 		case ERR_NOSUCHSERVER:
// 			msg += " :No such server\n";
// 			break;
// 		case ERR_NOSUCHCHANNEL:
// 			msg += ' ' + command.getArgument(0) + " :No such channel\n";
// 			break;
// 		case ERR_CANNOTSENDTOCHAN:
// 			msg += " :Cannot send to channel\n";
// 			break;
// 		case ERR_TOOMANYCHANNELS:
// 			msg += " :You have joined too many channels\n";
// 			break;
// 		case ERR_WASNOSUCHNICK:
// 			msg += " :There was no such nickname\n";
// 			break;
// 		case ERR_TOOMANYTARGETS:
// 			msg += " :Duplicate recipients delivered\n";
// 			break;
// 		case ERR_NOORIGIN:
// 			msg += " :No origin specified\n";
// 			break;
// 		case ERR_NORECIPIENT:
// 			msg += " :No recipient given (" + commandName + ")\n";
// 			break;
// 		case ERR_NOTEXTTOSEND:
// 			msg += " :No text to send\n";
// 			break;
// 		case ERR_NOTOPLEVEL:
// 			msg += " " + commandName + " :No toplevel domain specified\n";
// 			break;
// 		case ERR_WILDTOPLEVEL:
// 			msg += " " + commandName + " :Wildcard in toplevel domain\n";
// 			break;
// 		case ERR_UNKNOWNCOMMAND:
// 			msg += " " + commandName + " :Unknown command\n";
// 			break;
// 		case ERR_NOMOTD:
// 			msg += " :MOTD File is missing\n";
// 			break;
// 		case ERR_NOADMININFO:
// 			msg += " " + commandName + " :No administrative info available\n";
// 			break;
// 		case ERR_NONICKNAMEGIVEN:
// 			msg += " :No nickname given\n";
// 			break;
// 		case ERR_ERRONEUSNICKNAME:
// 			msg += " " + command.getArgument(0) + " :Erroneus nickname\n";
// 			break;
// 		case ERR_NICKNAMEINUSE:
// 			msg += " * :Nickname is already in use.\n";
// 			break;
// 		case ERR_NICKCOLLISION:
// 			msg += " " + commandName + " :Nickname collision KILL\n";
// 			break;
// 		case ERR_USERNOTINCHANNEL:
// 			msg += " " + commandName + " :They aren't on that channel\n";
// 			break;
// 		case ERR_NOTONCHANNEL:
// 			msg += " " + commandName + " :You're not on that channel\n";
// 			break;
// 		case ERR_USERONCHANNEL:
// 			msg += " " + commandName + " :is already on channel\n";
// 			break;
// 		case ERR_NOLOGIN:
// 			msg += " " + commandName + " :User not logged in\n";
// 			break;
// 		case ERR_SUMMONDISABLED:
// 			msg += " :SUMMON has been disabled\n";
// 			break;
// 		case ERR_USERSDISABLED:
// 			msg += " :USERS has been disabled\n";
// 			break;
// 		case ERR_NOTREGISTERED:
// 			msg += " :You have not registered\n";
// 			break;
// 		case ERR_NEEDMOREPARAMS:
// 			msg += " " + commandName + " :Not enough parameters\n";
// 			break;
// 		case ERR_ALREADYREGISTRED:
// 			msg += " :You may not reregister\n";
// 			break;
// 		case ERR_NOPERMFORHOST:
// 			msg += " :Your host isn't among the privileged\n";
// 			break;
// 		case ERR_PASSWDMISMATCH:
// 			msg += " :Password incorrect\n";
// 			break;
// 		case ERR_YOUREBANNEDCREEP:
// 			msg += " :You are banned from this server\n";
// 			break;
// 		case ERR_KEYSET:
// 			msg += " " + commandName + " :Channel key already set\n";
// 			break;
// 		case ERR_CHANNELISFULL:
// 			msg += " " + commandName + " :Cannot join channel (+l)\n";
// 			break;
// 		case ERR_UNKNOWNMODE:
// 			msg += " :is unknown mode char to me\n";
// 			break;
// 		case ERR_INVITEONLYCHAN:
// 			msg += " " + commandName + " :Cannot join channel (+i)\n";
// 			break;
// 		case ERR_BANNEDFROMCHAN:
// 			msg += " " + commandName + " :Cannot join channel (+b)\n";
// 			break;
// 		case ERR_BADCHANNELKEY:
// 			msg += " " + commandName + " :Cannot join channel (+k)\n";
// 			break;
// 		case ERR_NOPRIVILEGES:
// 			msg += " :Permission Denied- You do not have the correct IRC Operator privileges\n";
// 			break;
// 		case ERR_CHANOPRIVSNEEDED:
// 			msg += " " + commandName + " :You're not channel operator\n";
// 			break;
// 		case ERR_CANTKILLSERVER:
// 			msg += " :You cant kill a server!\n";
// 			break;
// 		case ERR_NOOPERHOST:
// 			msg += " :No O-lines for your host\n";
// 			break;
// 		case ERR_UMODEUNKNOWNFLAG:
// 			msg += " :Unknown MODE flag\n";
// 			break;
// 		case ERR_USERSDONTMATCH:
// 			msg += " :Cant change mode for other users\n";
// 			break;
// 		default:
// 			msg += "UNKNOWN ERROR\n";
// 			break;
// 	}
// 	write(user.getFd(), msg.c_str(), msg.size());
// }