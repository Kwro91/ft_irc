#include "../Includes/Define.hpp"


void	User::ft_reply(const int code, std::string arg1, std::string arg2, std::string arg3)
{
	std::string reply;

	reply = formatReply(code);
	reply += irssiCode(code, arg1, arg2, arg3);
	reply += LAST_CHAR;

	sendMsg(reply);
}


void	User::serverMsg(int socket, std::string msg)
{
	std::cout << YELLOW << "Message du server envoyé a Irssi :\n" << END << msg << std::endl;
	if (send(socket, msg.c_str(), msg.length(), 0) < 0)
		throw (Server::MyError(ERR_SEND));
}
void	User::sendMsg(std::string msg)
{
	serverMsg(getSocket(), msg);
}

void	User::userMsg(std::string msg, User *target)
{
	serverMsg(target->getSocket(), msg);
}


void	User::channelMsg(std::string msg, Channel *target)
{
	std::vector<User *> members = target->getUsers();
	std::vector<User *>::iterator it = members.begin();
	for ( ; it != members.end(); it++)
	{
		if ((*it)->getNickname() != _nickName)
			userMsg(msg, (*it));
	}
}

void	User::multiUserMsg(std::string msg)
{
	std::set<int> users;
	if (_joinedChannels.empty())
		return ;
	std::vector<Channel *>::iterator channel = _joinedChannels.begin();
	for ( ; channel != _joinedChannels.end(); channel++)
	{
		size_t old_size = users.size();
		std::vector<User *> members = (*channel)->getUsers();
		std::vector<User *>::iterator cl = members.begin();
		for ( ; cl != members.end(); cl++)
		{
			if ((*cl)->getId() != _id)
			{
				users.insert((*cl)->getId());
				if (users.size() > old_size)
				{
					userMsg(msg, (*cl));
					++old_size;
				}
			}
		}
	}
}


std::string	User::formatUser()
{
	std::string		msg;

	msg.append(":");
	msg.append(_nickName);
	msg.append("!");
	msg.append(_userName);
	msg.append("@");
	msg.append(getHostname());
	msg.append(" ");
	return (msg);
}

std::string	User::formatReply(const int code) const
{
	std::string	rep;

	rep.append(":");
	rep.append(_serv->getName());
	rep.append(" ");
	if (code < 10)
		rep.append("00");
	else if (code < 100)
		rep.append("0");
	rep.append(to_string(code));	//:hostname 025
	rep.append(" ");
	if (getNickname() == "")
		rep.append("*"); 			//:hostname 025 *
	else
		rep.append(_nickName);	//:hostname 025 nick 
	rep.append(" ");
	return (rep);
}


std::string	User::irssiCode(const int code, std::string arg1, std::string arg2, std::string arg3)
{
	switch(code)
	{
		case 1:
			return (RPL_WELCOME(arg1, arg2, arg3));
		case 2:
			return (RPL_YOURHOST(arg1, arg2));
		// nick
		case 353:
			return (RPL_NAMREPLY(arg1, arg2));
		case 366:
			return (RPL_ENDOFNAMES(arg1));
		case 401:
			return (ERR_NOSUCHNICK(arg1));
		// part
		case 403:
			return (ERR_NOSUCHCHANNEL(arg1));
		case 442:
			return (ERR_NOTONCHANNEL(arg1));
		case 431:
			return (ERR_NONICKNAMEGIVEN);
		case 432:
			return (ERR_ERRONEUSNICKNAME(arg1));
		case 433:
			return (ERR_NICKNAMEINUSE(arg1));
		case 461:
			return (ERR_NEEDMOREPARAMS(arg1));
		case 462:
			return (ERR_ALREADYREGISTERED);
		case 464:
			return (ERR_PASSWDMISMATCH);
		//kick
		case 482:
			return (ERR_CHANOPRIVSNEEDED(arg1));
		case 441:
			return (ERR_USERNOTINCHANNEL(arg1, arg2));
		case 411:
			return (ERR_NORECIPIENT(arg1));
		case 412:
			return (ERR_NOTEXTTOSEND);
		//ping & pong
		case 402:
			return (ERR_NOSUCHSERVER(arg1));
		case 409:
			return (ERR_NOORIGIN);
		//invite
		case 443:
			return (ERR_USERONCHANNEL(arg1, arg2));
		case 341:
			return (RPL_INVITING(arg1, arg2));
		case 473:
			return (ERR_INVITEONLYCHAN(arg1));
		//limitUser
		case 405:
			return (ERR_TOOMANYCHANNELS(arg1));
		case 471:
			return (ERR_CHANNELISFULL(arg1));
		case 332:
			return (RPL_TOPIC(arg1, arg2));
		case 331:
			return (RPL_NOTOPIC(arg1));
		case 472:
			return (ERR_UNKNOWNMODE(arg1));
		case 475:
            return (ERR_BADCHANNELKEY(arg1));
		default:
			return ("BAD REPLY");
	}
	return ("");
}

