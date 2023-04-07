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
		std::string				_ip;
		std::vector<Channel*>	_channels;
		bool					_operator;
		bool					_voicestate;
		bool					_authorized;
		bool					_passwordSent;
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
		std::string				getIp() const;
		void					setNickname(std::string nickname);
		void					setChannel(Channel * channel);
		void					resetChannel(Channel *channel);
		std::vector<Channel*>	getChannels();
		bool					isOperator();
		void					setOperator(bool Status);
		std::string 			getUserInfo() const;
		bool					getAuth() const;
		void					setAuth(bool status);
		bool					getPasswordSent() const;
		void					setPasswordSent(bool state);
		bool					getVoiceState() const;
		void					setVoiceState(bool state);
};

#endif