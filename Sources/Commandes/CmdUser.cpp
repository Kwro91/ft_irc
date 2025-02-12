#include "../../Includes/Define.hpp"

void    User::user(Message *msg)
{
    std::cout << SGREEN << "~   User a été utilisé par l'utilisateur n°" << _id  << ", " << GGREEN << getNickname() << END << std::endl;
	std::string user = "USER";

	if (_register.okUser)
		return (ft_reply(462, "", "", ""));
	if (msg->param.size() < 4)
		return (ft_reply(461, msg->command, "", ""));

	_userName = msg->param[0];

	(void) msg->param[1];
	_hostName = _ip;
	_hostName += ".chezMoi.fr";

	(void) msg->param[2];

	_realName = msg->param[3];
	addCommandToRegister(user);
}

void User::leaveChannel()
{
	std::vector<Channel *> channelsToLeave = _joinedChannels;

	for (std::vector<Channel *>::iterator it = channelsToLeave.begin(); it != channelsToLeave.end(); ++it)
	{
		_serv->rmUserFromChannel(this, (*it)->getName());
	}
}