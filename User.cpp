#include "User.hpp"

User::User() : _nick("Default"), _fd(-1) {
}

User::User(int fd) : _nick("na"), _fd(fd) {
}

User::~User(){}

User &User::operator=(const User &other) {
	this->_fd = other.getFd();
	this->_nick = other.getNick();
	return (*this);
}

int User::getFd() const {
	return (this->_fd);
}

std::string User::getNick() const {
	return (this->_nick);
}

void User::setNick(std::string nick) {
	this->_nick = nick;
}