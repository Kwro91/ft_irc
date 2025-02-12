#include "../../Includes/Define.hpp"

void			User::pong(Message *msg)
{
    std::cout << SGREEN << "~   Pong a été utilisé par l'utilisateur n°" << _id  << ", " << GGREEN << getNickname() << END << std::endl;
	if (msg->param.size() < 1)
		return (ft_reply(461, msg->command, "", ""));
}