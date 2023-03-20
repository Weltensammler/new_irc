#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Server.hpp"

class Channel
{
	private:
		std::string	_channelname;
		Channel();

	public:
		Channel(std::string channelname);
		~Channel();
};

#endif