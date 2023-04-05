#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Server.hpp"

class User;

class Channel
{
	private:
		std::string				_channelname;
		std::vector<int>		_users;
		std::vector<int>		_operators;
		std::string				_topic;
		std::string				_topicSetBy;
		std::time_t				_topicSetAt;
								Channel();

	public:
								Channel(std::string channelname);
								~Channel();
		std::string				getChannelName();
		void					addUser(int userfd);
		void					deleteUser(int userfd);
		void					setOperator(int operatorfd);
		std::vector<int>		getUsers() const ;
		std::vector<int>		getOperators() const ;
		bool					isOperator(int userfd);
		bool					isOnCchannel(int userfd);
		void					setTopic(std::string topic);
		std::string				getTopic() const;
		void					setTopicSetBy(std::string nick);
		std::string				getTopicSetBy() const;
		void					setTopicSetAt(std::time_t time);
		std::time_t				getTopicSetAt() const;
};

#endif