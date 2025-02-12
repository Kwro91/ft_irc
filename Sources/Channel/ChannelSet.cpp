#include "../../Includes/Channel.hpp"

void	Channel::setMaxUser(size_t maxUser){
	_maxUser = maxUser;
}

void	Channel::setTopic(std::string topic){
	_topic = topic;
}

void	Channel::setLogMsg(std::string logMsg){
	_logMsg = logMsg;
}

void	Channel::setTopicOperator(int value){
	_topicOperator = value;
}

void Channel::setNbUser(size_t nb){
	_nbUser = nb;
}

// Invite

void	Channel::setInvite(bool value){
	_invite = value;
}

// Private
void	Channel::setPrivate(bool value){
	_private = value;
}

void	Channel::setPassword(std::string password) {
	_password = password;
}