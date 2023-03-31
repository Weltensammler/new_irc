#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Server.hpp"

class User;

class Channel
{
	private:
		std::string						_channelname;
		std::vector<int>				_users;
		// std::map<std::string, User&>	_users;
		// std::map<std::string, User&>	_operators;
		std::vector<int>				_operators;
		Channel();

	public:
		Channel(std::string channelname);
		~Channel();
		std::string						getChannelName();
		// void							addUser(User &newuser);
		void							addUser(int userfd);
		// void							setOperator(User &newoperator);
		void							setOperator(int operatorfd);
		// std::map<std::string, User&>	getUsers() const ;
		std::vector<int>				getUsers() const ;
		// std::map<std::string, User&>	getOperators();
		std::vector<int>				getOperators() const ;
		// bool							isOperator(std::string username);
		bool							isOperator(int userfd);
};

#endif