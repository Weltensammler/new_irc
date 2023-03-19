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
	this->_message = message;
	std::cout << "Message is: " << (this->_message[0]) << std::endl;
	std::cout << "---------------------" << std::endl;
}

Commands::~Commands() {}

void	Commands::determineCommand()
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

commandEnum	Commands::gettype() {
	return (this->_type);
}

void Commands::passCommand() {
	std::cout << "Command PASS" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void Commands::userCommand() {
	std::cout << "Command USER" << std::endl;
	std::cout << "---------------------" << std::endl;
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