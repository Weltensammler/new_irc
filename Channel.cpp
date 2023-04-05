#include "Channel.hpp"

Channel::Channel()
{
	_channelname = "default";
}

Channel::Channel(std::string channelname) : _channelname(channelname), _topic(std::string()), _topicSetAt(NULL), _topicSetBy(std::string()) {}

std::string Channel::getChannelName(){ return this->_channelname;}

void	Channel::addUser(int userfd)
{
	//std::cout << "########################################AddUser FD:" << userfd << std::endl;
	for (std::vector<int>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (*it == userfd)
		{
			//std::cout << "User schon vorhanden: " << *it<< std::endl;
			return ;
		}
	}
	//std::cout << "vector size: " << _users.size() << std::endl;
	this->_users.push_back(userfd);
}

void	Channel::deleteUser(int userfd)
{
	for (std::vector<int>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (*it == userfd)
		{
			this->_users.erase(it);
			return ;
		}
	}
}

void	Channel::setOperator(int operatorfd)
{
	for (std::vector<int>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == operatorfd)
			return ;
	}
	this->_operators.push_back(operatorfd);
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

bool	Channel::isOnCchannel(int userfd)
{
	for (std::vector<int>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (*it == userfd)
			return (true);
	}
	return (false);
}

void	Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}


std::string	Channel::getTopic() const {
	return (this->_topic);
}

void	Channel::setTopicSetBy(std::string nick)
{
	this->_topicSetBy = nick;
}

std::string		Channel::getTopicSetBy() const
{
	return (this->_topicSetBy);
}

void	Channel::setTopicSetAt(std::time_t time)
{
	this->_topicSetAt = time;
}

std::time_t		Channel::getTopicSetAt() const
{
	return (this->_topicSetAt);
}