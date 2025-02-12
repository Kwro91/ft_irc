#include "../../Includes/Channel.hpp"

Channel::Channel(std::string name, int id) : _name(name), _id(id), _nbUser(0), _invite(false), _private(false){
	std::cout << BLUE << "Channel " << _name << " successfully created" << END << std::endl;
}

Channel::~Channel(){

}

Channel &Channel::operator=(Channel &other)
{
	if (this != &other)
	{
		_userList = other.getUsers();
		_operatorList = other._operatorList;
		_whiteList = other._whiteList;
		_maxUser = other._maxUser;
	}
	return *this;
}

Channel::Channel(Channel &other) 
{
	*this = other;
}

std::ostream &operator<<(std::ostream &out, const Channel &c){
	out << c.getName() <<"channel, " << c.getNbUser() << " user";
	return (out);
}

Channel::Channel(User &admin, std::string name) : _name(name), _invite(0), _private(0)
{
	_nbUser = 0;
	_maxUser = 10;
	_topicOperator = 1;
	_operatorList.push_back(admin.getId());
}