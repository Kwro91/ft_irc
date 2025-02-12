#include "../../Includes/Define.hpp"

void	User::comeIn()
{
	ft_reply(1, getNickname(), getUsername(), getHostname());
	ft_reply(2, _serv->getName(), "1.0", "");
}

void	User::pass(Message *msg)
{
    std::cout << SGREEN << "~   Pass a été utilisé par l'utilisateur n°" << _id  << ", " << GGREEN << getNickname() << END << std::endl;
	std::string pass = "PASS";

	if (_register.okPass)
		return (ft_reply(462, "", "", ""));
	_hostName = _ip;
	_hostName += ".chezMoi.fr";
	if (msg->param.empty())
		return (ft_reply(461, msg->command, "", ""));
	else if (!msg->param[0].compare(_serv->getPassword()))
		addCommandToRegister(pass);
	else
	{
		_password = 1;
		std::cout << BLUE << "Mot de passe correcte : "<< _serv->getPassword().c_str() << END << std::endl;
		return (ft_reply(464, "", "", ""));
	}
}
