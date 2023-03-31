#include "Channel.hpp"

Channel::Channel()
{
	_channelname = "default";
}

Channel::Channel(std::string channelname) : _channelname(channelname) {}

std::string Channel::getChannelName(){ return this->_channelname;}

// void		Channel::addUser(User &newuser)
// {
// 	// std::pair<std::string, User&> pair = make_pair(newuser.getUsername(), newuser);
// 	std::cout << "warum" << std::endl;
// 	std::cout << newuser.getUsername() << std::endl;
// 	_users[newuser.getUsername()] = newuser;
// 	std::cout << "wieso" << std::endl;
// 	std::cout << "add user called:" << this->_users.find(newuser.getUsername())->second.getUsername() << std::endl;
// }

void	Channel::addUser(int userfd)
{
	for (std::vector<int>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (*it == userfd)
			return ;
	}
	this->_users.push_back(userfd);
}

// void		Channel::setOperator(User &newoperator)
// {
// 	// std::pair<std::string, User> pair = make_pair(newoperator.getUsername(), newoperator);
// 	// this->_operators.insert(pair);
// 	_users[newoperator.getUsername()] = newoperator;
// }

void	Channel::setOperator(int operatorfd)
{
	for (std::vector<int>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == operatorfd)
			return ;
	}
	this->_users.push_back(operatorfd);
}

// std::map<std::string, User&>	Channel::getUsers() const
// {
// 	std::cout << "get users" << std::endl;
// 	std::cout << _users.size() << std::endl;
// 	std::cout << "get users2" << std::endl;
// 	return (_users);
// }

std::vector<int>	Channel::getUsers() const
{
	return (_users);
}

// std::map<std::string, User&>	Channel::getOperators()
// {
// 	return (_operators);
// }

std::vector<int>	Channel::getOperators() const
{
	return (_operators);
}

// bool	Channel::isOperator(std::string username)
// {
// 	if (_operators.find(username) != _operators.end())
// 		return (true);
// 	return (false);
// }

bool	Channel::isOperator(int userfd)
{
	for (std::vector<int>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == userfd)
			return (true);
	}
	return (false);
}