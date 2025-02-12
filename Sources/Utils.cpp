#include "../Includes/Define.hpp"

const char *Server::MyError::what() const throw()
{
	return (error.c_str());
}
Server::MyError::MyError(const std::string &message) : error(message) {}
Server::MyError::~MyError() throw() {}

Message parseMsg(const std::string &input)
{
	Message msg;

	if (input.empty())
		return msg;

	size_t start = input.find_first_not_of(" \r\n\v\t\f");
	if (start == std::string::npos)
		return msg;

	// Préfixe
	if (input[start] == ':')
	{
		size_t prefixEnd = input.find(' ', start);
		if (prefixEnd != std::string::npos)
		{
			msg.prefix.assign(input, start + 1, prefixEnd - start - 1);
			start = prefixEnd + 1;
		}
		else
			return msg;
	}

	start = input.find_first_not_of(' ', start);
	if (start == std::string::npos)
		return msg;

	// Commande
	size_t commandEnd = input.find(' ', start);
	if (commandEnd != std::string::npos)
	{
		msg.command.assign(input, start, commandEnd - start);
		start = commandEnd + 1;
	}
	else
	{
		msg.command.assign(input, start, input.length() - start);
		return msg;
	}

	start = input.find_first_not_of(' ', start);

	// Paramètres
	while (start < input.length())
	{
		if (input[start] == ':')
		{
			msg.param.push_back(input.substr(start + 1));
			break;
		}
		else
		{
			size_t paramEnd = input.find(' ', start);
			if (paramEnd != std::string::npos)
			{
				msg.param.push_back(input.substr(start, paramEnd - start));
				start = input.find_first_not_of(' ', paramEnd);
			}
			else
			{
				msg.param.push_back(input.substr(start));
				break;
			}
		}
	}

	return msg;
}

void printMsg(const Message& msg)
{
	if (!msg.prefix.empty())
		std::cout << ":" << msg.prefix << " ";

	std::cout << "Commande = " << msg.command << std::endl;

	for (size_t i = 0; i < msg.param.size(); ++i)
		std::cout << "Paramètre = " << msg.param[i] << std::endl;

	std::cout << std::endl;
}

void printInAscii(const std::string& str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		std::cout << str[i] << " " << int(str[i]) << std::endl;
	}
}


size_t stringToSizeT(const std::string &value)
{
	std::stringstream ss(value);
	size_t result;
	ss >> result;
	if (ss.fail() || !ss.eof())
	{
		throw std::invalid_argument("Failed to convert to size_t");
	}
	return result;
}
