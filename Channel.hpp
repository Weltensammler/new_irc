#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Server.hpp"

class User;

class Channel
{
	private:
		std::string				_channelname;
		std::vector<int>		_users;
		std::vector<int>		_operators;
								Channel();

	public:
								Channel(std::string channelname);
								~Channel();
		std::string				getChannelName();
		void					addUser(int userfd);
		void					deleteUser(int userfd);
		void					setOperator(int operatorfd);
		std::vector<int>		getUsers() const ;
		std::vector<int>		getOperators() const ;
		bool					isOperator(int userfd);
};

#endif