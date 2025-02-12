#include "../../Includes/Channel.hpp"

const std::string&	Channel::getName() const {return (_name);}

const std::vector<User*>&	Channel::getUsers() const {return (_userList);}

const size_t&	Channel::getNbUser() const {return (_nbUser);}

const int&	Channel::getId() const {return (_id);}

size_t	Channel::getMaxUser() const {return (_maxUser);}

const std::string& Channel::getTopic() const {return (_topic);}

int Channel::getTopicOperator() const {return (_topicOperator);}

const std::string& Channel::getLogMsg() const {return (_logMsg);}

std::string	Channel::getUserNicks(){
	std::ostringstream oss;
	for (std::vector<User*>::iterator it = _userList.begin(); it != _userList.end(); ++it)
	{
		if (it != _userList.begin()) // Ajouter un espace seulement entre les pseudonymes
			oss << " ";
		if (checkOperator(*it))
			oss << "@";
		oss << (*it)->getNickname();
	}
	return oss.str();
}

// Invite

int	Channel::getInviteStatus() const {return (_invite);}


const std::vector<int>&	Channel::getWhiteList() const {return (_whiteList);}

// Private

int Channel::getPrivateStatus() const {return (_private);}
