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
		std::vector<int>		_voice_state;
		std::vector<int>		_excludeUsers;
		std::string				_topic;
		bool					_topicoperator;
		bool					_isModerated;
		bool					_inviteonly;
		std::string				_topicSetBy;
		std::time_t				_topicSetAt;
		int						_limit;
		int						_currUserNumb;
								Channel();

	public:
								Channel(std::string channelname);
								~Channel();
		std::string				getChannelName();
		void					addUser(int userfd);
		void					deleteUser(int userfd);
		void					setOperator(int operatorfd);
		void					resetOperator(int operatorfd);
		void					setTopicOperator(bool topicoperator);
		bool					getTopicOperator();
		void					setInviteOnly(bool inviteonly);
		bool					getInviteOnly();
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
		bool					setlimit(int limit);
		int						getlimit() const;
		void					resetlimit();
		int						currUserNumbs() const;
		void					incrementUserNumb();
		void					decrementUserNumb();
		void					banUser(int userfd);
		bool					isUserBaned(int userfd) const;
		void					unBanUser(int userfd);
		bool					isModerated() const;
		void					setModerated(bool State);
		void					set_voice_state(int userfd);
		void					reset_voice_state(int userfd);
		bool					get_voice_state(int _userfd);
};

#endif