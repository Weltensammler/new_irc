#ifndef USER_HPP
# define USER_HPP

#include "Server.hpp"

class User
{
	private:
		std::string	_nick;
		int			_fd;

	public:
		User();
		User(int);
		~User();
		User &operator= (const User &other);
		int	getFd() const;
		std::string getNick() const;
		void setNick(std::string nick);
};

#endif