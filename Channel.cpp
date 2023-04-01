#include "Channel.hpp"

Channel::Channel()
{
	_channelname = "default";
}

Channel::Channel(std::string channelname) : _channelname(channelname) {}

std::string Channel::getChannelName(){ return this->_channelname;}

void	Channel::addUser(int userfd)
{
	std::cout << "AddUser FD:" << userfd << std::endl;
	for (std::vector<int>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (*it == userfd)
			return ;
	}
	std::cout <<"User wird hinzugefügt fd:" << userfd << std::endl;
	this->_users.push_back(userfd);
}

void	Channel::setOperator(int operatorfd)
{
	for (std::vector<int>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == operatorfd)
			return ;
	}
	this->_users.push_back(operatorfd);
}

std::vector<int>	Channel::getUsers() const
{
	return (_users);
}

std::vector<int>	Channel::getOperators() const
{
	return (_operators);
}

bool	Channel::isOperator(int userfd)
{
	for (std::vector<int>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == userfd)
			return (true);
	}
	return (false);
}