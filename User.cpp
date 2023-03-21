#include "User.hpp"

User::User() : _nickname("Default"), _username("Default"), _realname("Default"), _fd(-1), _authorized(false) {
}

User::User(int fd) : _nickname("Default"), _username("Default"), _realname("Default"), _fd(fd), _authorized(false) {
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

std::vector<Channel*>	User::getChannels()
{
	return (_channels);
}

std::string	User::getIp() const
{
	return (_ip);
}

bool	User::isOperator()
{
	return (_operator);
}

std::string	User::getUserInfo() const 
{
	std::string info;

	info = this->getNickname() + "!" + this->getUsername() + "@" + this->getIp();
	return info;
}

bool	User::getAuth() const {
	return (this->_authorized);
}

void	User::setAuth(bool status) {
	this->_authorized = status;
}

bool	User::getPasswordSent() const{
	return (this->_passwordSent);
}
void	User::setPasswordSent(bool status) {
	this->_passwordSent = status;
}