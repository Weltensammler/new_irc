#include "Commands.hpp"

Commands::Commands(std::vector<std::string> message, int userfd, Server &server) : _userfd(userfd), _server(server) {
	if (message[0] == "PASS") {this->_type = PASS;}
	else if (message[0] == "USER") {this->_type = USER;}
	else if (message[0] == "NICK") {this->_type = NICK;}
	else if (message[0] == "PRIVMSG") {this->_type = PRIVMSG;}
	else if (message[0] == "NOTICE") {this->_type = NOTICE;}
	else if (message[0] == "JOIN") {this->_type = JOIN;}
	else if (message[0] == "INVITE") {this->_type = INVITE;}
	else if (message[0] == "TOPIC") {this->_type = TOPIC;}
	else if (message[0] == "KILL") {this->_type = KILL;}
	else if (message[0] == "KICK") {this->_type = KICK;}
	else if (message[0] == "MODE") {this->_type = MODE;}
	else {this->_type = MESSAGE;}
	this->_message = message;
	std::cout << "Message is: " << (this->_message[0]) << std::endl;
	std::cout << "---------------------" << std::endl;
}

Commands::~Commands() {}

void	Commands::determineCommand()
{
	std::cout << "determine aufgerufen mit: " << this->gettype() << std::endl;
	//TODO check if operator maybe check in the command (because here we don't really know the channel)
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
		case INVITE:
			this->inviteCommand();
			break;
		case TOPIC:
			this->topicCommand();
			break;
		case KILL:
			this->killCommand();
			break;
		case KICK:
			this->kickCommand();
			break;
		case MODE:
			this->modeCommand();
			break;
		default:
			this->sendMessage();
			break;
	};
}

commandEnum	Commands::gettype()
{
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
		// return sendError(command, ERR_NEEDMOREPARAMS);
		return ;
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

void Commands::privmsgCommand()
{
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
		std::cout << "PRIVMSG#########" << std::endl;
		//_server.printchannels();
		Channel *channel = _server.findChannel(this->_message[1]);
		User *user =  this->_server.findUserByFd(_userfd);
		if (channel->isModerated() == true && !user->getVoiceState() && !user->isOperator())
		{
			sendError(ERR_CHANOPRIVSNEEDED, "");
			return;
		}
		sendMessageToChannel(channel, ":" +user->getNickname() + " " + "PRIVMSG" + " " + this->_message[1] + " :"+ this->_message[2] + "\r\n", false);
	}
	else
	{
		sendMessageToUser("PRIVMSG");
	}
}

void Commands::noticeCommand()
{
	std::cout << "Command NOTICE" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (this->_message.size() == 1) {
		return sendError(ERR_NORECIPIENT, "");
	}
	if (this->_message.size() < 3) {
		return sendError(ERR_NOTEXTTOSEND, "");
	}
	std::string	reciverNick = this->_message[1];
	if (reciverNick.at(0) == '#')
		sendMessageToChannel(_server.findChannel(this->_message[1]), ":" + this->_server.findUserByFd(_userfd)->getNickname() + " " + "NOTICE" + " " + this->_message[1] + " :"+ this->_message[2] + "\r\n", false);
	else
		sendMessageToUser("NOTICE");
}

void Commands::joinCommand()
{
	std::cout << "Command JOIN" << std::endl;
	std::cout << "---------------------" << std::endl;
	if (this->_message.size() < 2) 
	{
		return sendError(ERR_NEEDMOREPARAMS, "");
	}
	std::vector<std::string> newchannels = splitArgs(1);
	for(int i = 0; i < newchannels.size(); i++)
	{
		// std::string const	&channelName = command.getArgument(0);
		//? Not needed?
		//? User				&user = command.getUser();
		Channel				*channel;
		if (newchannels[i][0] != '#')
		{
			//sendError(command, 403);
			std::cout << "Wrong channel name!" << std::endl;
			return;
		}
		channel = _server.findChannel(newchannels[i]);
		if (!channel)
		{
			// try
			// {
				//channel does not exist
				channel = new Channel(newchannels[i]);
				_server.setChannel(channel);
				_server.findUserByFd(_userfd)->setChannel(channel);
				std::cout << "user join 207: "<< this->_server.findUserByFd(_userfd)->getUsername() << " user fd: " << _userfd << std::endl;
				std::cout << "Adduser to Channel Join 1 Command: " << _userfd << std::endl;
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
			//std::cout <<" Ich bin hier HHHHHHHHHHHHHHHHHHHHHHHHHH"<< std::endl;
			std::vector<Channel*> ch = this->_server.findUserByFd(_userfd)->getChannels();
			std::vector<Channel*>::iterator begin = ch.begin();
			std::vector<Channel*>::iterator end = ch.end();
			if (std::find(begin, end, channel) == end)
			{
				//*Check if User is Baned from the Channel
				if (channel->isUserBaned(_userfd) == true)
					return sendError(ERR_BANNEDFROMCHAN, "");

				//*Check if channel limit is reached or is unset
				if (channel->getlimit() == -1 || channel->getlimit() > channel->currUserNumbs())
				{
					this->_server.findUserByFd(_userfd)->setChannel(channel);
					std::cout << "Add User to Channnel Join 2 Command: " << _userfd << std::endl;
					channel->addUser(_userfd);
					channel->incrementUserNumb();
				}
				else
				{
					return sendError(ERR_CHANNELISFULL, "");
				}
			}
		}
		std::cout << "JOIN" << std::endl;
		//_server.printchannels();
		sendMessageToChannel(channel, ":" + this->_server.findUserByFd(_userfd)->getUserInfo() + " " + "JOIN" + " :" + channel->getChannelName() + "\r\n", true);

		std::stringstream names;
		std::vector<int> users = channel->getUsers();
		std::vector<int> operators = channel->getOperators();
		names << ":" + _server.getServername() +" 353 " << _server.findUserByFd(_userfd)->getNickname() << " = " << channel->getChannelName() << " :";
		std::vector<int>::iterator it = users.begin();
		std::vector<int>::iterator end = users.end();
		for (; it != end; it++)
		{
			if (channel->isOperator(*it) || _server.findUserByFd(*it)->isOperator())
			{
				names << '@';
			}
			names << _server.findUserByFd(*it)->getNickname();
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

void Commands::inviteCommand()
{
	std::cout << "Command INVITE" << std::endl;
	std::cout << "---------------------" << std::endl;
	//TODO checkt inviteonly variable in INVITE command
}

void Commands::topicCommand()
{
	std::cout << "Command TOPIC" << std::endl;
	std::cout << "---------------------" << std::endl;
	//ERR_NEEDMOREPARAMS		done
	//ERR_NOSUCHCHANNEL			done
	//ERR_NOTONCHANNEL			done
	//ERR_CHANOPRIVSNEEDED		done
	//RPL_NOTOPIC				done
	//RPL_TOPIC					done
	//RPL_NOTOPICWHOTIME		done
	if (this->_message.size() < 2 || ((int)this->_message[1][0] == 13 && (int)this->_message[1][1] == 10))
		return sendError(ERR_NEEDMOREPARAMS, "");
	if (this->_message[1][0] != '#' || !_server.findChannel(this->_message[1]))
		return sendError(ERR_NOSUCHCHANNEL, "");
	if (!checkIfUserOnChannel(this->_message[1], this->_userfd))
		return sendError(ERR_NOTONCHANNEL, "");
	if (this->_message.size() == 2)
	{
		Channel *channel = this->_server.findChannel(this->_message[1]);
		std::string topic = channel->getTopic();
		if (topic.empty())
			return sendReplyToUser(RPL_NOTOPIC, "");
		else {
			std::time_t setAt = channel->getTopicSetAt();
			std::string setBy = channel->getTopicSetBy();
			sendReplyToUser(RPL_TOPIC, topic);
			return sendReplyToUser(RPL_TOPICWHOTIME, setBy + " " + std::to_string(setAt));
		}
	}
	else if (this->_message.size() == 3 && this->_message[2][0] == ':' && ((int)this->_message[2][1] == 13 && (int)this->_message[2][2] == 10))
	{
		if (this->_server.findChannel(this->_message[1])->getTopicOperator())
		{
			if (!checkIfOperator(this->_message[1], _userfd))
				return sendError(ERR_CHANOPRIVSNEEDED, "");
		}
		Channel *channel = this->_server.findChannel(this->_message[1]);
		channel->setTopic("");
		channel->setTopicSetBy(_server.findUserByFd(_userfd)->getNickname());
		channel->setTopicSetAt(std::time(nullptr));
		//SEND TO ALL FEHLT
		sendReplyToUser(RPL_TOPIC, "");
		return ;
	} else {
		if (this->_server.findChannel(this->_message[1])->getTopicOperator())
		{
			if (!checkIfOperator(this->_message[1], _userfd))
				return sendError(ERR_CHANOPRIVSNEEDED, "");
		}
		Channel *channel = this->_server.findChannel(this->_message[1]);
		channel->setTopic(this->_message[2]);
		channel->setTopicSetBy(_server.findUserByFd(_userfd)->getNickname());
		channel->setTopicSetAt(std::time(nullptr));
		//SEND TO ALL FEHLT
		sendReplyToUser(RPL_TOPIC, channel->getTopic());
		return ;
	}
}

void Commands::killCommand()
{
	std::cout << "Command KILL" << std::endl;
	std::cout << "---------------------" << std::endl;
	deleteUser();
}

void Commands::kickCommand()
{
	std::cout << "Command KICK" << std::endl;
	std::cout << "---------------------" << std::endl;
	//numeric replies:
	//ERR_NEEDMOREPARAMS	done
	//ERR_NOSUCHCHANNEL		done
	//ERR_CHANOPRIVSNEEDED	done
	//ERR_USERNOTINCHANNEL	done
	//ERR_NOTONCHANNEL		done
	//TODO tests funktionieren noch nicht so ganz check if operator sieht gut aus aber beim Rest hakt es noch
	std::cout << "message 1: " << this->_message[1] << "\nmessage 2: " << this->_message[2] << std::endl;

//!Original wird ersetzt durch
	// if (this->_message.size() < 3)
	// {
	// 	std::cout << "Kick zu wenig" << std::endl;
	// 	return sendError(ERR_NEEDMOREPARAMS, "");
	// }
	// if (!_server.findChannel(this->_message[1]))
	// {
	// 	std::cout << "no channel" << std::endl;
	// 	return sendError(ERR_NOSUCHCHANNEL, "");
	// }
	// if (!checkIfOperator(this->_message[1], _userfd))
	// {
	// 	std::cout << "no operator" << std::endl;
	// 	return sendError(ERR_CHANOPRIVSNEEDED, "");
	// }
//!Neue Funktion macht das selbe wie der alte Code, wenn kein MaxMsgSize vorhanden ist wird standard -1 gesetzt
	if (checkMain(3) == false)
		return;
	if (_server.findUserByNick(this->_message[2]))
	{
		if (!checkIfUserOnChannel(this->_message[1], _server.findUserByNick(this->_message[2])->getFd()))
		{
			std::cout << "user not in channel" << std::endl;
			return sendError(ERR_USERNOTINCHANNEL, "");
		}
	}
	else
		return sendError(ERR_USERNOTINCHANNEL, "");
	if (!checkIfUserOnChannel(this->_message[1], _userfd))
	{
		std::cout << "you are not on channel" << std::endl;
		return sendError(ERR_NOTONCHANNEL, "");
	}
	//TODO wenn User von Channel gekickt wird channel->decremetUserNumb();
}

bool	Commands::checkIfOperator(std::string channel, int userfd)
{
	if (!_server.findChannel(channel)->isOperator(_userfd))
		return (false);
	return (true);
}

bool	Commands::checkIfUserOnChannel(std::string channel, int userfd)
{
	if (!_server.findChannel(channel)->isOnCchannel(_userfd))
		return (false);
	return (true);
}

void Commands::modeCommand()
{
	std::cout << "Command MDOE" << std::endl;
	std::cout << "---------------------" << std::endl;
	//*check if channel exists, is operator and check msg size
	if (checkMain(3,4) == false)
		return;
	//*check for add or remove option, is nothing given -> undefined behavior -> add
	std::string m_char;
	bool add;
	if (this->_message[2].size() > 2)
		return sendError(ERR_UNKNOWNMODE,"");
	if (this->_message[2][0] == '+')
	{
		m_char = this->_message[2].substr(1);
		add = true;
	}
	else if (this->_message[2][0] == '-')
	{
		m_char = this->_message[2].substr(1);
		add = false;
	}
	else
	{
		m_char = this->_message[1];
		add = true;
	}

	if (m_char == "t")
	{
		//TODO write function at the beginning of mode, that verifies messsage[1] is a Channel
		//! checked in CheckMain --Tom--
		if (add == true)
			_server.findChannel(this->_message[1])->setTopicOperator(true);
		else
			_server.findChannel(this->_message[1])->setTopicOperator(false);
		// t (only operators can change topic
	}
	else if (m_char == "i")
	{
		// i (sets channel status to invite only, only operators can invite users)
		//TODO checkt inviteonly variable in INVITE command
		if (add == true)
			_server.findChannel(this->_message[1])->setInviteOnly(true);
		else
			_server.findChannel(this->_message[1])->setInviteOnly(false);
	}
	else if (m_char == "m")
	{
		// m (sets channel status to moderated only operators and users with voice status can send messages)
		if (add == true)
			_server.findChannel(this->_message[1])->setModerated(true);
		else
			_server.findChannel(this->_message[1])->setModerated(false);
	}
	else if (m_char == "l")
	{
		// l (set client limit to channel)
		if (add == true)
		{
			if (this->_message.size() != 4)
				return sendError(ERR_NEEDMOREPARAMS, "");
			if (_server.findChannel(this->_message[1])->setlimit(std::atoi(this->_message[3].c_str())) == false)
				return sendError(ERR_TOOMANYUSERS, "");
		}
		else
		{
			_server.findChannel(this->_message[1])->resetlimit();
		}
	}
	//* from here on user modes
	else if (m_char == "o")
	{
		// o (makes a user operator)
		if (!checkIfUserOnChannel(this->_message[1], _userfd))
		{
			std::cout << "you are not on channel" << std::endl;
			return sendError(ERR_NOTONCHANNEL, "");
		}
		if (_server.findUserByNick(this->_message[3]))
		{
			if (!checkIfUserOnChannel(this->_message[1], _server.findUserByNick(this->_message[3])->getFd()))
			{
				std::cout << "user not in channel" << std::endl;
				return sendError(ERR_USERNOTINCHANNEL, "");
			}
		}
		else
			return sendError(ERR_NOSUCHNICK, "");
		if (add == true)
		{
			_server.findUserByNick(this->_message[3])->setOperator(true);
		}
		else
		{
			_server.findUserByNick(this->_message[3])->setOperator(false);
		}
	}
	else if (m_char == "v")
	{
		//* v (gives a user voice status, can talk even if channel is in moderation mode)
		if (!checkIfUserOnChannel(this->_message[1], _userfd))
		{
			std::cout << "you are not on channel" << std::endl;
			return sendError(ERR_NOTONCHANNEL, "");
		}
		if (_server.findUserByNick(this->_message[3]))
		{
			if (!checkIfUserOnChannel(this->_message[1], _server.findUserByNick(this->_message[3])->getFd()))
			{
				std::cout << "user not in channel" << std::endl;
				return sendError(ERR_USERNOTINCHANNEL, "");
			}
		}
		else
			return sendError(ERR_NOSUCHNICK, "");
		if (add == true)
		{
			_server.findUserByNick(this->_message[3])->setVoiceState(true);
		}
		else
		{
			_server.findUserByNick(this->_message[3])->setVoiceState(false);
		}
	}
	else if (m_char == "b")
	{
		// b (specified user can't access the channel)
		if (!checkIfUserOnChannel(this->_message[1], _userfd))
		{
			std::cout << "you are not on channel" << std::endl;
			return sendError(ERR_NOTONCHANNEL, "");
		}
		if (_server.findUserByNick(this->_message[3]))
		{
			if (!checkIfUserOnChannel(this->_message[1], _server.findUserByNick(this->_message[3])->getFd()))
			{
				std::cout << "user not in channel" << std::endl;
				return sendError(ERR_USERNOTINCHANNEL, "");
			}
		}
		else
			return sendError(ERR_NOSUCHNICK, "");
		
		Channel	*channel = _server.findChannel(_message[1]);
		User*user = _server.findUserByNick(this->_message[3]);

		if (add == true)
		{
			channel->banUser(user->getFd());
			user->resetChannel(channel);
		}
		else
		{
			channel->unBanUser(user->getFd());
			user->setChannel(channel);
		}
	}
}

void Commands::sendMessage()
{
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

void Commands::deleteUser()
{
	std::map<int, User*>::iterator it;
	std::map<int, User*> users = _server.getUsers();
	for (it = users.begin(); it != users.end(); ++it)
	{
		if ((*it->second).getFd() == this->_userfd)
		{
			this->removeUserFromChannels(this->_userfd);
			users.erase(it);
			_server._polls[this->_userfd].fd = -1;
			break;
		}
	}
}

void Commands::removeUserFromChannels(int userfd)
{
	User *user = this->_server.findUserByFd(_userfd);
	std::vector<Channel*> channels = user->getChannels();
	std::vector<Channel*>::iterator it;
	for (it = channels.begin(); it != channels.end(); it++)
	{
		(*it)->deleteUser(user->getFd());
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

void	Commands::sendReplyToUser(int replyCode, std::string arg)
{
	std::string	msg = ":OurIRCServer ";
	std::stringstream	stream;
	std::string commandName = this->_message[0];
	stream << replyCode;
	msg += stream.str() + " " + _server.findUserByFd(_userfd)->getNickname();

	switch (replyCode)
	{
		case RPL_NOWAWAY:
			msg += " :You have been marked as being away\n";
			break;
		case RPL_WHOISOPERATOR:
			msg +=  + " :is an IRC operator\n";
			break;
		case RPL_ENDOFWHOWAS:
			msg +=  + " :End of WHOWAS\n";
			break;
		case RPL_LISTSTART:
			msg += "Channel :Users  Name\n";
			break;
		case RPL_NOTOPIC:
			msg +=  + " :No topic is set\n";
			break;
		case RPL_TOPIC:
			msg +=  + " :" + arg + "\n";
			break;
		case RPL_TOPICWHOTIME:
			msg +=  + " " + arg + "\n";
			break;
		case RPL_SUMMONING:
			msg +=  + " :Summoning user to IRC\n";
			break;
		case RPL_ENDOFNAMES:
			msg +=  + " :End of /NAMES list\n";
			break;
		case RPL_ENDOFLINKS:
			msg +=  + " :End of /LINKS list\n";
			break;
		case RPL_ENDOFBANLIST:
			msg +=  + " :End of channel ban list\n";
			break;
		case RPL_ENDOFINFO:
			msg += " :End of /INFO list\n";
			break;
		case RPL_MOTDSTART:
			msg += " :- Message of the day - \n";
			break;
		case RPL_ENDOFMOTD:
			msg += " :End of /MOTD command\n";
			break;
		case RPL_YOUREOPER:
			msg += " " + commandName + " :You are now an IRC operator\n";
			break;
		case RPL_REHASHING:
			msg +=  + " :Rehashing\n";
			break;
		case RPL_USERSSTART:
			msg += " :UserID   Terminal  Host\n";
			break;
		case RPL_USERS:
			msg += " :%-8s %-9s %-8s\n";
			break;
		case RPL_ENDOFUSERS:
			msg += " :End of users\n";
			break;
		case RPL_NOUSERS:
			msg += " :Nobody logged in\n";
			break;
		case RPL_ENDOFSTATS:
			msg +=  + " :End of /STATS report\n";
			break;
		case RPL_STATSUPTIME:
			msg += " :Server Up %d days %d:%02d:%02d\n";
			break;
		case RPL_UMODEIS:
			msg +=  + "\n";
			break;
		case RPL_LUSEROP:
			msg +=  + " :operator(s) online\n";
			break;
		case RPL_LUSERUNKNOWN:
			msg +=  + " :unknown connection(s)\n";
			break;
		case RPL_LUSERCHANNELS:
			msg +=  + " :channels formed\n";
			break;
		case RPL_LUSERME:
			msg += " :I have clients and servers\n";
			break;
		case RPL_ADMINME:
			msg +=  + " :Administrative info\n";
			break;
		default:
			msg += "UNKNOWN REPLY\n";
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

void	Commands::sendMessageToChannel(Channel *channel, std::string string, bool self)
{
	std::vector<int>	users = channel->getUsers();

	for (std::vector<int>::iterator it = users.begin(); it != users.end(); ++it)
	{
		std::cout << "it == " << *it << " | self == " << self << std::endl;
		if (*it != _userfd || self == true)
		{
			int fd = *it;
			write(fd, string.c_str(), string.length());
			// std::cout << "fd = " << fd << std::endl << "vector size users = " << users.size() << std::endl << "vector size channel users = " << channel->getUsers().size() << std::endl;
			write(1, string.c_str(), string.length());
			std::cout <<std::endl;
		}
		else
			std::cout << "WIRD NICHT GESENDET User fd == " << *it << std::endl;
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
	if (reciver)
	{
		std::stringstream toSend;
		toSend << ':' + /*reciver->getUserInfo()*/ _server.findUserByFd(_userfd)->getUserInfo() << " " <<
		reason << " " << reciverNick << " :" << message << "\r\n";
		std::string str = toSend.str();
		write(reciver->getFd(), str.c_str(), str.size());
		// logger.logUserMessage(str, *reciver, OUT);
	}
	else
	{
		this->sendError(ERR_NOSUCHNICK, "");
	}
}

bool Commands::checkMain(int MinMsgSize, int MaxMsgSize = -1)
{
	if (this->_message.size() < MinMsgSize)
	{
		std::cout << "Parameter zu wenig" << std::endl;
		sendError(ERR_NEEDMOREPARAMS, "");
		return false;
	}
	if (MaxMsgSize != -1)
	{
		if (this->_message.size() > MaxMsgSize)
		{
			std::cout << "Parameter zu viele" << std::endl;
			sendError(ERR_TOOMANYPARAMS, "");
			return false;
		}
	}
	if (!_server.findChannel(this->_message[1]))
	{
		std::cout << "no channel" << std::endl;
		sendError(ERR_NOSUCHCHANNEL, "");
		return false;
	}
	if (!checkIfOperator(this->_message[1], _userfd))
	{
		std::cout << "no operator" << std::endl;
		sendError(ERR_CHANOPRIVSNEEDED, "");
		return false;
	}
	return true;
}



