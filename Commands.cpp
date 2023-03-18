#include "Commands.hpp"

Commands::Commands(std::vector<std::string> message) {
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
}

Commands::~Commands() {}

void	Commands::determineCommand(std::vector<std::string> message)
{
	switch (this->gettype())
	{
		case PASS:
			this->passCommand();
		case USER:
			this->userCommand();
		case NICK:
			this->nickCommand();
		case PONG:
			this->pongCommand();
		case PRIVMSG:
			this->privmsgCommand();
		case NOTICE:
			this->noticeCommand();
		case JOIN:
			this->joinCommand();
		case OPER:
			this->operCommand();
		case QUIT:
			this->quitCommand();
		case KILL:
			this->killCommand();
		case KICK:
			this->kickCommand();
		case PING:
			this->pingCommand();
		case LIST:
			this->listCommand();
		case WHO:
			this->whoCommand();
		case PART:
			this->partCommand();
		default:
			this->sendMessage();
	};
	
}