#include "../../Includes/Define.hpp"

void            User::leave(Message *msg)
{
    std::cout << SGREEN << "~   Leave a été utilisé par l'utilisateur n°" << _id  << ", " << GGREEN << getNickname() << END << std::endl;
    if (msg->param.empty())
        return (ft_reply(461, msg->command, "", ""));

    std::string part_msg = "";
    if (msg->param.size() == 1)
        part_msg = "Au revoir";
    else
        part_msg = msg->param[1];

    std::stringstream ss(msg->param[0]);
    std::string channel;
    while (std::getline(ss, channel, ','))
    {
        Channel *target = _serv->getChannel((channel));
        if (target == NULL)
            ft_reply(403, channel, "", "");
        else if (!target->checkUser(this))
            ft_reply(442, target->getName(), "", "");
        else
        {
            std::string message = formatUser();
            message.append("PART " + channel + " :" + part_msg + LAST_CHAR);
            channelMsg(message, target);
            sendMsg(message);
            _serv->rmUserFromChannel(this, channel);
        }
    }
}