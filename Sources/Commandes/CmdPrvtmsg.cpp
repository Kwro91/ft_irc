#include "../../Includes/Define.hpp"

void User::privMsg(Message *msg)
{
    std::cout << SGREEN << "~   PrvtMsg a été utilisé par l'utilisateur n°" << _id  << ", " << GGREEN << getNickname() << END << std::endl;
    
    if (msg->param.empty()) {
        ft_reply(411, msg->command, "", "");
        return;
    }
    
    if (msg->param.size() == 1) {
        ft_reply(412, "", "", "");
        return;
    }
    
    const std::string &targetName = msg->param[0];
    const std::string &msgContent = msg->param[1];
    std::string message = formatUser() + "PRIVMSG " + targetName + " :" + msgContent + LAST_CHAR;
    
    Channel *channel = _serv->getChannel(targetName);
    if (channel) {
        channelMsg(message, channel);
        return;
    }
    
    User *target = _serv->getUser(targetName);
    if (!target) {
        ft_reply(401, targetName, "", "");
        return;
    }
    
    userMsg(message, target);
}

