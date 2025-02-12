#include "../../Includes/Define.hpp"

void    User::topic(Message *msg)
{
    std::cout << SGREEN << "~   Topic a été utilisé par l'utilisateur n°" << _id  << ", " << GGREEN << getNickname() << END << std::endl;
    if (msg->param.empty())
        return (ft_reply(461, msg->command, "", ""));
    Channel        *this_channel = _serv->getChannel((msg->param[0]));
    if (this_channel == NULL)
        return (ft_reply(403, msg->param[0], "", ""));
    std::string ch_name = this_channel->getName();
    if (!this_channel->checkUser(this))
        return (ft_reply(442, ch_name, "", ""));
    if (msg->param.size() == 1) 
    {
        std::string topic = this_channel->getTopic();
        std::cout << PURPLE << topic << std::endl;
        if (topic.empty())
            return (ft_reply(331, ch_name, "", ""));
        else
            return (ft_reply(332, ch_name, topic, ""));
        return;
    }
    if (!this_channel->checkOperator(this) && this_channel->getTopicOperator() == true)
        return (ft_reply(482, ch_name, "", ""));
    else if (this_channel->getTopicOperator() == false || this_channel->checkOperator(this))
    {
        if (msg->param.size() == 2)
        {
            if (msg->param[1].empty())
                this_channel->setTopic("");
            else 
			    this_channel->setTopic(msg->param[1]);
		
			// Set the new topic
			std::cout << GGREEN << msg->param[1] << END << std::endl;
			std::cout << GPURPLE << this_channel->getTopic()<< END << std::endl;
			std::string message = formatUser();
			message.append("TOPIC " + msg->param[0] + " :" + msg->param[1] + LAST_CHAR);
            channelMsg(message, this_channel);
            sendMsg(message);
        }
    }
    else
    {
        channelMsg("Not allowed\n", this_channel);
    }
}
