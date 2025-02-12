#include "../../Includes/Define.hpp"

void			User::kick(Message *msg)
{
    std::cout << SGREEN << "~   Kick a été utilisé par l'utilisateur n°" << _id  << ", " << GGREEN << getNickname() << END << std::endl;
	if (msg->param.size() < 2)
		return (ft_reply(461, msg->command, "", ""));
	Channel		*channel = _serv->getChannel((msg->param[0]));
	if (channel == NULL)
		return (ft_reply(403, msg->param[0], "", ""));
	
	std::string ch_name = channel->getName();
	User		*to_kick = _serv->getUser((msg->param[1]));
	
	if (to_kick == NULL)
		return (ft_reply(401, msg->param[1], "", ""));
	if (!channel->checkUser(this))
		return (ft_reply(442, ch_name, "", ""));
	else if (!channel->checkUser(to_kick))
		return (ft_reply(441, msg->param[1], ch_name, ""));
	else if (!channel->checkOperator(this))
		return (ft_reply(482, ch_name, "", ""));
	else
	{
		std::string part_msg;
		if (msg->param.size() == 2)
			part_msg = "has been kicked";
		else
			part_msg = msg->param[2];
		std::string message = formatUser();
		message.append("KICK " + ch_name + " " + msg->param[1] + " :" + part_msg + LAST_CHAR);
		channelMsg(message, channel);
		sendMsg(message);
		_serv->rmUserFromChannel(to_kick, ch_name);
	}
}