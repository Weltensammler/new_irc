#include "User.hpp"

User::User() : _nickname("Default"), _username("Default"), _realname("Default"), _fd(-1) {
}

User::User(int fd) : _nickname("Default"), _username("Default"), _realname("Default"), _fd(fd) {
}

User::~User(){}

User		&User::operator=(const User &other) {
	this->_fd = other.getFd();
	this->_realname = other.getRealname();
	this->_username = other.getUsername();
	this->_nickname = other.getNickname();
	return (*this);
}

int			User::getFd() const {
	return (this->_fd);
}

std::string	User::getUsername() const {
	return (this->_username);
}

void		User::setUsername(std::string username) {
	this->_username = username;
}

std::string	User::getRealname() const {
	return (this->_realname);
}

void		User::setRealname(std::string realname) {
	this->_realname = realname;
}

std::string	User::getNickname() const {
	return (this->_nickname);
}

void		User::setNickname(std::string nickname) {
	this->_nickname = nickname;
}

void		User::setChannel(Channel * channel)
{
	_channels.push_back(channel);
}
