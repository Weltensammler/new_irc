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
			this->nickCommand();
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
	std::cout << "Username: " << this->_message[1] << std::endl;
	std::cout << "Realname: " << this->_message[4] << std::endl;
}

void Commands::nickCommand() {
	std::cout << "Command NICK" << std::endl;
	std::cout << "---------------------" << std::endl;
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

void Commands::joinCommand() {
	std::cout << "Command JOIN" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (this->_message.size() < 2) {
		std::cout << "No Channel given" << std::cout;
		// return sendError(command, ERR_NEEDMOREPARAMS);
	}
	std::string const	&channelName = command.getArgument(0);
	//? Not needed?
	//? User				&user = command.getUser();
	Channel				*channel;

	if (channelName.at(0) != '#') {
		sendError(command, 403);
		return;
	}
	channel = findChannel(channelName);
	if (!channel) {
		try {
			//channel does not exist
			channel = new Channel(channelName);
			_channels.push_back(channel);
			user.addChannel(channel);
			channel->addUser(user);
			channel->setOper(&user);
		}
		catch (FtException &e) {
			sendError(command, ERR_NOSUCHCHANNEL);
			return;
		}
	}
	else {
		if (std::find(user.getChannels().begin(), user.getChannels().end(), channel) == user.getChannels().end()) {
			user.addChannel(channel);
			channel->addUser(user);
		}
	}
	sendMessageToChannel(*channel, ":" + user.getUserInfoString() + " " + "JOIN" + " :" + channel->getName() + "\r\n");

	std::stringstream names;
	std::vector<User *> users = channel->getUsers();
	std::vector<User *> operators = channel->getOperators();
	names << ":" + _serverName +" 353 " << user.getNick() << " = " << channel->getName() << " :";

	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); ++it) {
		if (channel->isOperator((*it)) || (*it)->isOper()) {
			names << '@';
		}
		names << (*it)->getNick();
		if ((it + 1) != users.end())
			names << ' ';
	}
	names << "\r\n";
	std::string namesString = names.str();
	write(user.getFd(), namesString.c_str(), namesString.length());
	logger.logUserMessage(namesString, user, OUT);

	std::stringstream endOfNamesList;
	endOfNamesList << ":" + _serverName +" 366 " << user.getNick() << " " << channel->getName() << " :End of /NAMES list.\r\n";
	std::string endOfNamesListString = endOfNamesList.str();
	write(user.getFd(), endOfNamesListString.c_str(), endOfNamesListString.length());
	logger.logUserMessage(endOfNamesListString, user, OUT);
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