#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Server.hpp"

class User;

class Channel
{
	private:
		std::string						_channelname;
		std::map<std::string, User*>	_users;
		std::map<std::string, User*>	_operators;
		Channel();

	public:
		Channel(std::string channelname);
		~Channel();
		std::string						getChannelName();
		void							addUser(User &newuser);
		void							setOperator(User &newoperator);
		std::map<std::string, User*>	getUsers();
		std::map<std::string, User*>	getOperators();
};

#endif