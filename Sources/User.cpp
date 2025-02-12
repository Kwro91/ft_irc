#include "../Includes/Define.hpp"

int User::_maxId = 0;

User::User(Server *serv): _serv(serv), _password(0)
{
	_maxId += 1;
	_id = User::_maxId;
	_isRegistered = false;
	_isWelcomed = false;
	_quit = false;
	_register.okPass = false;
	_register.okNick = false;
	_register.okUser = false;
	_isPingSent = false;
}

User::User(User &other){ *this = other;}

User &User::operator=(User &other)
{
	(void) other;
	return (*this);
}

User::~User(){}

//BUFFER
void	User::handleBufferCommands(const std::string& buffer)
{
	Message	msg;

	if ( buffer.find_first_of(OK_CHAR) == std::string::npos)
		return;

	char	*token = strtok(const_cast<char*>(buffer.c_str()), LAST_CHAR);

	if (token == NULL)
		return;
	while (token != NULL)
	{
		std::string command(token);
		std::cout << GPURPLE << "Commande recu :" << command << END << std::endl;
		msg = parseMsg(command);
		std::cout << YELLOW << "Message traité : " << END << std::endl;
		printMsg(msg);
		executeCommande(&msg);
		if (getQuit())
			return ;
		token = strtok(NULL, LAST_CHAR);
	}
}

void 	User::setBuffer(std::string buffer)
{
	_isPingSent = false;

	_buffer += buffer;

	if (_buffer.find(LAST_CHAR) != std::string::npos )
	{
		handleBufferCommands(_buffer);
		_buffer.clear();
	}
}

//CHANNEL

void	User::rmJoinedChannel(Channel *ch)
{
	std::vector<Channel*>::iterator it = _joinedChannels.begin();
	for ( ; it != _joinedChannels.end(); ++it)
	{
		if ((*it)->getName() == ch->getName())
		{
			_joinedChannels.erase(it);
			return;
		}
	}
}


std::ostream		&operator<<( std::ostream & flux, User const &rhs )
{
	flux << YELLOW << "Les infos de mon user" << END << std::endl;
	flux << "ID: " << rhs.getId() << std::endl;
	flux << "Nickname: " << rhs.getNickname() << std::endl;
	flux << "Username: " << rhs.getUsername() << std::endl;
	flux << "Hostname: " << rhs.getHostname() << std::endl;
	flux << "User Socket: " << rhs.getSocket() << std::endl;
	return flux;
}

//COMMANDES

void User::executeCommande(Message *msg)
{
    // Définir un tableau de paires (commande, pointeur de membre)
    struct Command
	{
        const std::string name;
        void (User::*handler)(Message *);
    };

    Command commands[NB_COMDS] =
	{
        {"NICK", &User::nick},
        {"USER", &User::user},
        {"PASS", &User::pass},
        {"JOIN", &User::join},
        {"KICK", &User::kick},
        {"PRIVMSG", &User::privMsg},
        {"INVITE", &User::invite},
        {"PING", &User::ping},
        {"PONG", &User::pong},
        {"MODE", &User::mode},
		{"TOPIC", &User::topic},
		{"PART", &User::leave}

    };

    // Vérifier les commandes autorisées pour les utilisateurs non enregistrés
    bool isUnregisteredCommand =
        msg->command == "CAP" || msg->command == "PASS" || 
        msg->command == "USER" || msg->command == "NICK";

    for (int i = 0; i < NB_COMDS; ++i)
    {
        if (msg->command == commands[i].name)
        {
            // Si l'utilisateur est enregistré, exécuter la commande
            if (_isRegistered)
            {
                (this->*commands[i].handler)(msg);
                return;
            }
            // Si la commande est autorisée pour les non-enregistrés, l'exécuter
            else if (isUnregisteredCommand)
            {
                (this->*commands[i].handler)(msg);
                return;
            }
            // Sinon, renvoyer une erreur
            else
            {
                ft_reply(451, msg->command, "Vous n'êtes pas enregistré", "");
                return;
            }
        }
    }
}

void	User::addCommandToRegister(std::string &command)
{
	if (command == "PASS")
	{
		_register.okPass = true;
		std::cout << "PASS a ete ajouter au registre de l'utilisateur n° " << _id << std::endl;
	}
	else if (command == "NICK")
	{
		_register.okNick = true;
		std::cout << "NICK a ete ajouter au registre de l'utilisateur n° " << _id << std::endl;
	}
	else if (command == "USER")
	{
		_register.okUser = true;
		std::cout << "USER a ete ajouter au registre de l'utilisateur n° " << _id << std::endl;
	}
	_isRegistered = ( _register.okUser == true && _register.okNick == true && _register.okPass == true);
	if (_isRegistered == true && _isWelcomed == false)
	{
		comeIn();
		_isWelcomed = true;
		std::cout << GGREEN << "l'utilisateur a bien ete enregistrer" << END << std::endl;
	}
}

/*GET*/

struct sockaddr_in	User::getAddress() const { return _address; }
std::string			User::getNickname() const { return _nickName; }
std::string			User::getUsername() const { return _userName; }
std::string			User::getHostname() const { return _hostName; }
std::string			User::getRealname() const { return _realName; }
std::string			User::getBuffer() const { return _buffer; }
std::string			User::getIp() const { return _ip; }
bool				User::getIsPingSent() const { return _isPingSent; }
bool                User::getQuit() { return (_quit); }
int					User::getId() const { return _id; }
int					User::getSocket() const { return _socket; }
int					User::getPass() const { return _password; }
int					User::getMaxId() const { return _maxId; }


/*SET*/

void 				User::setAddress(struct sockaddr_in address) { _address = address; }
void				User::setNickname(std::string nName) { _nickName = nName; }
void				User::setUsername(std::string uName) { _userName = uName; }
void				User::setHostname(std::string hName) { _hostName = hName; }
void				User::setIp(std::string ip) { _ip = ip; }
void 				User::setSocket(int socket) { _socket = socket; }
