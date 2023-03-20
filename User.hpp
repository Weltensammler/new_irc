#ifndef USER_HPP
# define USER_HPP

#include "Server.hpp"

class Channel;

class User
{
	private:
		std::string				_username;
		std::string				_realname;
		std::string				_nickname;
		std::vector<Channel*>	_channels;
		bool					_operator;
		int						_fd;

	public:
		User();
		User(int);
		~User();
		User					&operator= (const User &other);
		int						getFd() const;
		std::string				getUsername() const;
		void					setUsername(std::string username);
		std::string				getRealname() const;
		void					setRealname(std::string realname);
		std::string				getNickname() const;
		void					setNickname(std::string nickname);
		void					setChannel(Channel * channel);
		std::vector<Channel*>	getChannels();
};

#endif