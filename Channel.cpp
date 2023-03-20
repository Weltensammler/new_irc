#include "Channel.hpp"

Channel::Channel(std::string channelname) : _channelname(channelname) {}

std::string Channel::getChannelName(){ return this->_channelname;}

void		Channel::addUser(User* newuser)
{
	this->_user.insert(std::make_pair<std::string, User*>(newuser->getUsername(), newuser));
}
