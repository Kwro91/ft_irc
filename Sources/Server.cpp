#include "../Includes/Server.hpp"

/*CONSTRUCTEUR*/

Server::Server(): _name("FT_irc.net"){}

Server::Server(Server &other)
{*this = other;}

Server &Server::operator=(Server &other)
{
	this->_name = other._name;
	this->_password = other._password;
	this->_port = other._port;
	this->_serverSocket = other._serverSocket;
	this->_serverAddr = other._serverAddr;
	this->_actualSocket = other._actualSocket;
	this->_okSocket = other._okSocket;
	this->_userList = other._userList;
	this->_buffer = other._buffer;
	this->_serverInfo = other._serverInfo;

	return (*this);
}

/*DESTRUCTEUR*/
Server::~Server() {}

/*CONFIG SERVER*/

bool Server::execServer()
{
	if (setupSocket())
		return(1);
	if(run())
		return(1);
	return(0);
}

bool Server::checkArguments(int ac, char **av)
{
	if (ac != 3 )
		throw (MyError(ERR_INPUT));
	_port = atoi(av[1]);
	if (_port < 6665 || _port > 6669)
		throw (MyError(ERR_PORT));
	_password = av[2];
	return(0);
}

int Server::setupSocket()
{
	// Création du socket
	if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return (MyError(ERR_CREATE), 1);
	// Configuration de l'adresse du serveur
	std::memset(&_serverAddr.sin_zero, 0, sizeof(_serverAddr));
	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_serverAddr.sin_port = htons(this->_port);
	//Reutiliser l'adresse
	int optval = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		close (_serverSocket);
		throw MyError(ERR_OPT);
	}
	// Liaison du socket
	if (bind(_serverSocket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1)
	{
		close (_serverSocket);
		throw MyError(ERR_BIND);
	}
	// Mise en écoute
	if (listen(_serverSocket, ALLOWED_CONNECTIONS) == -1)
	{
		close (_serverSocket);
		throw MyError(ERR_LISTEN);
	}
	//Mettre _serverSocket en non bloquant
	fcntl(_serverSocket, F_SETFL, O_NONBLOCK);
	//Set le groupe de fd check par select
	FD_ZERO(&_actualSocket);
	FD_SET(this->_serverSocket, &_actualSocket);

	std::cout << *this << std::endl;
	return(0);
}

bool Server::run()
{
	std::cout << GREEN << "Serveur IRC correctement initialisé, bienvenue !" << std::endl << std::endl;
	_serverState = true;
	int retour;
	while (Server::checkRunning())
	{
		memset(&_okSocket, 0, sizeof(_okSocket));
		_okSocket = _actualSocket;
		retour = select(FD_SETSIZE, &_okSocket, NULL, NULL, &_tv);
		if (retour == -1)
		{		
			std::cout << retour << std::endl;
			throw (MyError(ERR_SELECT));
		}
		else if (retour != 0)
			socketActivity();
		checkWrongPasswords();
		checkNobodyConnected(_serverState);
	}
	finish();
	return (0);
}

bool	Server::finish()
{
	std::cout << "\nExtinction du server\n";
	while (!_userList.empty())
    {
        std::map<int, User*>::iterator it = _userList.begin();
        rmUserFromServer(it->second, "");
    }
	close(getServerSocket());
	return (true);
}


//SOCKET
int isSocketClose(int socket_fd)
{
	char buffer[1]; // Tampon pour tester si des données sont disponibles
	int res = recv(socket_fd, buffer, 1, MSG_PEEK);

	if (res == 0)
	{
		return 1;
	} else if (res < 0)
	{
		// Gestion des erreurs spécifiques
		if (errno == EAGAIN || errno == EINTR)
			return 0;
		else
		{
			std::cerr << "Fatal error in recv(): " << strerror(errno) << std::endl;
			return 1;
		}
	}
	// La socket est toujours ouverte
	return 0;
}

void	Server::socketActivity()
{
	for (int nbSocket = 0; nbSocket < FD_SETSIZE; nbSocket++)
	{
		if (FD_ISSET(nbSocket, &_okSocket))
		{
			if (nbSocket == _serverSocket)
				addNewUser();
			else
			{
				if (isSocketClose(nbSocket) == true)
					rmUserFromServer(_userList[nbSocket], " a été deconnecté");
				else
				{
					message(nbSocket);
					if (_userList[nbSocket]->getQuit())
						rmUserFromServer(_userList[nbSocket], " quitte le server");
				}
			}
		}
	}
}


/*CHECK*/

void	Server::checkWrongPasswords()
{
	if (!_userList.empty())
	{
		std::vector<User *>				toDeleteList;
		std::map<int , User *>::iterator	it = _userList.begin();
		std::map<int , User *>::iterator	itEnd = _userList.end();

		while( it != itEnd )
		{
			if (it->second->getPass() == 1)
				toDeleteList.push_back(it->second);
			++it;
		}
		for (size_t i = 0; i < toDeleteList.size(); i++)
			rmUserFromServer(toDeleteList.at(i), "Mot de pass refusé");
	}
}

void	Server::checkNobodyConnected(bool &isRunning)
{
	time_t		t;
	static bool	waitingForUsers = false;

	time(&t);

	if ( _userList.size() >= 1 )
	{
		_innactive = t;
		waitingForUsers = false;
	}
	else if ( waitingForUsers == false )
	{
		time(&_innactive);
		waitingForUsers = true;
	}
	if ( std::difftime(t, _innactive) > T_SHUTDOWN )
	{
		std::cout << "Le server va s'eteindre car personne n'est connecter depuis " << std::difftime(t, _innactive) << " seconds" <<  std::endl;
		isRunning = false;
	}
}

/*USER SERVE*/

void	Server::rmUserFromServer(User	*user, std::string cause)
{
	std::cout << YELLOW << "Utilisateur n°" << user->getId() << " " << cause << END << std::endl;

	int userId = user->getId();
	int sock = user->getSocket();
	user->leaveChannel();
	close(sock);
	delete _userList.at(sock);
	FD_CLR(sock, &_actualSocket);
	_userList.erase(sock);
	std::cout << RED << "Utilisateur n°" << END << userId << RED << " a été supprimé\n" << END << std::endl;
}

void	Server::message(int userSocketNumber)
{
	char bufferLocal[MAX_BUFFER + 1];
	ssize_t  bytesReceived;

	memset(bufferLocal, 0, MAX_BUFFER + 1);;
	bytesReceived = recv( userSocketNumber, bufferLocal, MAX_BUFFER, 0);
	if (bytesReceived < 0)
		throw (MyError(ERR_RECV));
	else
	{
		_buffer.append(bufferLocal);
		_userList[userSocketNumber]->setBuffer(_buffer);
	}
	_buffer.erase();
}

/*USER*/

void	Server::addNewUser()
{
	int addrSize = sizeof(struct sockaddr_in);
	int userSocket;
	struct sockaddr_in userAddr;
	User *newU = new (std::nothrow) User(this);
	if (!newU)
		throw (MyError(ERR_U_MEMORY));	
	if ((userSocket = accept(this->_serverSocket, (struct sockaddr*)&userAddr, (socklen_t*)&addrSize)) < 0)
	{
		delete newU;
		close (_serverSocket);
		throw MyError("Failed to accept connection");
	}
	fcntl(userSocket, F_SETFL, O_NONBLOCK);
	newU->setSocket(userSocket);
	newU->setAddress(userAddr);
	newU->setIp(inet_ntoa(userAddr.sin_addr));
	_userList.insert(std::pair<int , User *>(newU->getSocket(), newU));
	FD_SET(userSocket, &_actualSocket);
	std::cout << GGREEN << "Nouvel utilisateur" << END <<  ", n°" << newU->getId() << " ajouté\n" << std::endl;
}


/*USER CHANNEL*/

void	Server::addUserToChannel(User* user, std::string nameChannel)
{
	Channel* channel = getChannel(nameChannel);
	if (channel == NULL)
	{
		std::cout << "Le channel :" << nameChannel << " n'existe pas" << std::endl;
		return;
	}
	if (!channel->checkUser(user))
	{
		channel->addUser(user);
		user->addJoinedChannel(channel);
	}
}

void	Server::rmUserFromChannel(User* user, std::string nameChannel)
{
	Channel* channel = getChannel(nameChannel);
	if (channel == NULL)
	{
		std::cout << "Le channel : " << nameChannel << " n'existe pas" << std::endl;
		return;
	}
	channel->removeUser(user->getId());
	user->rmJoinedChannel(channel);
	if (channel->checkEmpty())
		rmChannel(nameChannel);
}

/*CHANNEL*/


void	Server::createChannel(User* admin, std::string nameChannel)
{
	if (channelExists(nameChannel))
	{
		std::cout << "Le channel " << nameChannel << " existe deja " << std::endl;
		return;
	}
	Channel* new_channel = new (std::nothrow) Channel(*admin, nameChannel);
	if (new_channel == NULL)
		return;
	_channelList.push_back(new_channel);
	std::cout << "Le channel " << nameChannel << " a ete cree" << std::endl;
}

bool	Server::channelExists(std::string nameChannel)
{
	for (size_t i = 0; i < _channelList.size(); i++)
	{
		if (_channelList[i]->getName() == nameChannel)
		{
			return true;
		}
	}
	return false;
}

void	User::addJoinedChannel(Channel *ch)
{
	_joinedChannels.push_back(ch);
}

Channel	*Server::getChannel(std::string nameChannel)
{
	for (size_t i = 0; i < _channelList.size(); i++)
	{
		if (_channelList[i]->getName() == nameChannel)
		{
			return _channelList[i];
		}
	}
	return NULL;
}

void	Server::rmChannel(std::string nameChannel)
{
	std::vector<Channel *>::iterator it = _channelList.begin();
	for ( ; it != _channelList.end(); ++it)
	{
		if ((*it)->getName() == nameChannel)
		{
			delete *it;
			_channelList.erase(it);
			return;
		}
	}
}

/*COMMANDE*/

bool	Server::checkNickUsed(std::string nickName)
{
	std::map<int , User *>::iterator itBegin = _userList.begin();

	while (itBegin != _userList.end())
	{
		if (itBegin->second->getNickname() == nickName)
			return true;
		itBegin++;
	}
	return false;
}


/*GET*/
bool Server::checkRunning() { return _serverState; }
std::string		Server::getName() const {return _name;}
std::string		Server::getPassword() const {return _password;}
std::string		Server::getServerInfo() const{ return _serverInfo;}
int				Server::getPort() const {return _port;}
int				Server::getServerSocket() const {return _serverSocket;}
size_t			Server::getUserName() const { return _userList.size(); }
User*	Server::getUser(std::string nickName)
{
	std::map<int, User*>::iterator start = _userList.begin();
	while(start != _userList.end())
	{
		if (start->second->getNickname() == nickName)
			return(start->second);
		start++;
	}
	return (NULL);
}

/*FLUX*/
std::ostream		&operator<<( std::ostream &flux, Server const & rhs )
{
	flux << std::endl << YELLOW << "Les infos de mon IRC :" << END << std::endl;
	flux << "Nom: " << rhs.getName() << std::endl;
	flux << "Mot de passe: " << rhs.getPassword() << std::endl;
	flux << "Port: " << rhs.getPort() << std::endl;
	flux << "Server Socket: " << rhs.getServerSocket() << std::endl;
	flux << "Nombre d'utilisateur: " << rhs.getUserName() << std::endl;
	time_t current_time;
    time(&current_time);
    flux << "Cree le: " << ctime(&current_time) << std::endl; 
	return flux;
}