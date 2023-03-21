#include "Channel.hpp"

Channel::Channel(std::string channelname) : _channelname(channelname) {}

std::string Channel::getChannelName(){ return this->_channelname;}

void		Channel::addUser(User &newuser)
{
	this->_users.insert(std::make_pair<std::string, User*>(newuser.getUsername(), &newuser));
}

void		Channel::setOperator(User &newoperator)
{
	this->_operators.insert(std::make_pair<std::string, User*>(newoperator.getUsername(), &newoperator));
}

std::map<std::string, User*>	Channel::getUsers() const
{
	return (_users);
}

std::map<std::string, User*>	Channel::getOperators()
{
	return (_operators);
}

bool	Channel::isOperator(std::string username)
{
	if (_operators[username])
		return (true);
	return (false);
}