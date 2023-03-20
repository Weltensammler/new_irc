#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Server.hpp"

class User;

class Channel
{
	private:
		std::string						_channelname;
		std::map<std::string, User*>	_user;
		std::map<std::string, User*>	_operators;
		Channel();

	public:
		Channel(std::string channelname);
		~Channel();
		std::string	getChannelName();
		void		addUser(User &newuser);
		void		setOperator(User &newoperator);
};

#endif