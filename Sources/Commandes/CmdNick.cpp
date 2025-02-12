#include "../../Includes/Define.hpp"

bool	validNickName(std::string nickName)
{
	if (nickName.length() > 20)
		return false;
	for (size_t i = 0; i < nickName.length(); i++)
	{
		if (!std::strchr(NICK_VALID_CHAR, nickName[i]))
			return false;
	}
	return true;
}

void	User::nick(Message *msg)
{
    std::cout << SGREEN << "~   Nick a été utilisé par l'utilisateur n°" << _id  << ", " << GGREEN << getNickname() << END << std::endl;
	std::string nick = "NICK";
	if (msg->param.empty())
		ft_reply(431, _nickName, "", "");
	else if (!validNickName(msg->param[0]))
		ft_reply(432, _nickName, "", "");
	else if (_serv->checkNickUsed(msg->param[0]))
		ft_reply(433, msg->param[0], "", "");
	else
	{
		std::string message;
		if ( _register.okNick )
		{
			message = formatUser();
			setNickname(msg->param[0]);
		}
		else
		{
			setNickname(msg->param[0]);
			message = formatUser();
		}
		message.append("NICK " + _nickName + LAST_CHAR);
		multiUserMsg(message);
		sendMsg(message);
		addCommandToRegister(nick);
	}
}