#include "../../Includes/Define.hpp"

void	User::join(Message *msg)
{
    std::cout << SGREEN << "~   Join a été utilisé par l'utilisateur n°" << _id  << ", " << GGREEN << getNickname() << END << std::endl;
	if (msg->param.empty())
		return (ft_reply(461, msg->command, "", ""));
	std::stringstream ss(msg->param[0]);
	std::string chan;
	while (std::getline(ss, chan, ','))
	{
		if (_joinedChannels.size() >= 5)
		{
				ft_reply(405, chan, "", "");
				break;
		}
		if ((chan[0] != '#' && chan[0] != '+' && chan[0] != '!') || chan.length() > 50)
		{
			ft_reply(403, chan, "", "");
			continue;
		}
		_serv->createChannel(this, chan);
		Channel *channel = _serv->getChannel(chan);
		if (!channel)
		{
			std::cerr << RED << "Channel :" << chan << "was not created (not enough memory)" << END << std::endl;
			return ;
		}
		if (channel->checkUser(this))
			return (ft_reply(443, _nickName, chan, ""));

		bool cInvite = 0;
		bool cPrivate = 0;

		if (channel->getInviteStatus() == 0)
			cInvite = 1;
		if (channel->getPrivateStatus() == 0)
			cPrivate = 1;
		if (channel->getNbUser() == channel->getMaxUser()) // check si le chan est full
			return (ft_reply(471, chan, "", "")); //Channel FULL
		
		if (cInvite == 0) // check le mode INVITATION
		{
			if (channel->findInWhiteList(this->getId()) != -1)
				cInvite = 1;
			else
				return (ft_reply(473, chan, "", "")); //In Invite ONLY
		}

		if (cPrivate == 0) // check le mode PRIVE
		{
			if (msg->param.size() == 2)
			{
				if (channel->comparePassword(msg->param[1]) == 1)
					cPrivate = 1;
				else
					return (ft_reply(475, chan, "", "")); //Bad PWD
			}
			else
				return (ft_reply(475, chan, "", "")); //Bad PWD
		}
		if (cPrivate == 1 && cInvite == 1) //JOIN SI TOUTE LES CONDITIONS REMPLIES
		{
			makeJoin(chan, channel);
		}
		else
			return (ft_reply(471, chan, "", "")); //Channel FULL
	}
}

void	User::makeJoin(std::string chan, Channel *channel){
		_serv->addUserToChannel(this, chan);
		std::string msg = formatUser();
		msg.append("JOIN " + chan + LAST_CHAR);
		sendMsg(msg);
		std::string topic = channel->getTopic();
		if (!(topic.empty()))
			ft_reply(332, chan, topic, "");
		ft_reply(353, chan, channel->getUserNicks(), "");
		ft_reply(366, chan, "", "");
}
