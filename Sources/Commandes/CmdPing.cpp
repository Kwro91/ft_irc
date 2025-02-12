#include "../../Includes/Define.hpp"

void			User::ping(Message *msg)
{
	bool pongSend = false;
		if (msg->param.empty())
		return (ft_reply(409, "", "", ""));
	size_t nbParam = (msg->param.size() > 1 ? 2 : 1);
	for (size_t i = 0; i < nbParam; ++i)
	{
		if (msg->param[i].compare(_serv->getName()) == 0)
		{
			if (!pongSend)
			{
				std::string to_send =  "PONG " + msg->param[0] +  " :" + msg->param[0] + LAST_CHAR;
				sendMsg(to_send);
				pongSend = true;
			}
		}
		else
			ft_reply(402, msg->param[i], "", "");
	}
}
