#include "Channel.hpp"

Channel::Channel()
{
	_channelname = "default";
	_topicoperator = true;
	_inviteonly = false;
	_limit = -1;
}

Channel::Channel(std::string channelname) :	_channelname(channelname),
											_topicoperator(true), _inviteonly(false),
											_topic(std::string()), _topicSetAt(NULL),
											_topicSetBy(std::string()), _limit(-1), _currUserNumb(1), _isModerated(false) {}

std::string Channel::getChannelName(){ return this->_channelname;}

void	Channel::addUser(int userfd)
{
	//std::cout << "AddUser FD:" << userfd << std::endl;
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

bool	Channel::setlimit(int limit)
{
	if (limit > 0)
	{
		this->_limit = limit;
		return true;
	}
	else
		return false;
}

int		Channel::getlimit() const
{
	return (this->_limit);
}

void	Channel::resetlimit()
{
	this->_limit = -1;
}

void	Channel::setTopicOperator(bool topicoperator)
{
	_topicoperator = topicoperator;
}

bool	Channel::getTopicOperator()
{
	return (_topicoperator);
}

void	Channel::setInviteOnly(bool inviteonly)
{
	_inviteonly = inviteonly;
}

bool	Channel::getInviteOnly()
{
	return (_inviteonly);
}

int	Channel::currUserNumbs() const
{
	return _currUserNumb;
}

void	Channel::incrementUserNumb()
{
	_currUserNumb++;
}

void	Channel::decrementUserNumb()
{
	if (_currUserNumb > 0)
		_currUserNumb--;
}

void Channel::banUser(int userfd)
{
	std::vector<int>::iterator it = _excludeUsers.begin();
	for(;it != _excludeUsers.end(); it++)
	{
		if (*it == userfd)
			return;
	}
	_excludeUsers.push_back(userfd);
}

bool	Channel::isUserBaned(int userfd) const
{
	std::vector<int>::const_iterator it = _excludeUsers.begin();
	for(;it != _excludeUsers.end(); it++)
	{
		if (*it == userfd)
			return true;
	}
	return false;
}

void	Channel::unBanUser(int userfd)
{
	std::vector<int>::iterator it = _excludeUsers.begin();
	for(;it != _excludeUsers.end(); it++)
	{
		if (*it == userfd)
		{
			_excludeUsers.erase(it);
			return;
		}
	}
}

bool	Channel::isModerated() const
{
	return _isModerated;
}

void	Channel::setModerated(bool State)
{
	_isModerated = State;
}