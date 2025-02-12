#ifndef USER_HPP
#define USER_HPP

#include "Define.hpp"

class Server;
class Channel;

typedef struct s_fullRegistered
{
	bool okPass;
	bool okNick;
	bool okUser;
}	fullRegistered;

typedef struct s_message
{
	std::string prefix;
	std::string command;
	std::vector<std::string> param;
}	Message;

Message		parseMsg(const std::string &input);
void		printMsg(const Message& msg);
void		printInAscii(const std::string& str);

class User 
{
	private:
		Server					*_serv;
		std::string 			_nickName;
		std::string 			_userName;
		std::string 			_hostName;
		std::string 			_realName;
		std::string				_ip;
		std::string				_buffer;
		static int				_maxId;
		int						_password;
		int 					_id;
		int 					_socket;
		bool					_isRegistered;
		bool					_isWelcomed;
		bool					_quit;
		bool					_isPingSent;
		struct sockaddr_in		_address;
		std::vector<Channel *>	_joinedChannels;
		fullRegistered			_register;
		bool					_topicOperatorOnly;

	public:

		User(Server *server);
		User(User &other);
		User &operator=(User &other);
		~User();


//BUFFER
	void	handleBufferCommands(const std::string& buffer);

//CHANNEL
	bool	checkEqual(const User* other) const;
	void	rmJoinedChannel(Channel *ch);
	void	addJoinedChannel(Channel *ch);


//COMMANDES
	void    executeCommande(Message *msg);
	void	addCommandToRegister(std::string &command);
	void	comeIn();

	void	pass(Message *msg);
	void	join(Message *msg);
	void	makeJoin(std::string chan, Channel *channel);
	void	serverMsg(int socket, std::string msg);
	void	userMsg(std::string msg, User *target);
	void	nick(Message *msg);
	void    user(Message *msg);
	void	invite(Message *msg);
	void	leave(Message *msg);
	void	leaveChannel();
	std::string	formatUser();
	void	channelMsg(std::string msg, Channel *target);
	void	multiUserMsg(std::string msg);
	void	topic(Message *msg);
	void	mode(Message *msg);
	void	modeTiret(Message *msg);
	void	cmdPvtMsg(User *u, std::string name, std::string msg);


void	ping(Message *msg);
void	pong(Message *msg);

void	userModes(Message *msg);

	void	privMsg(Message *msg);
	void	kick(Message *msg);




//MESSAGES
	void	sendMsg(std::string msg);
	void	ft_reply(const int code, std::string arg1, std::string arg2, std::string arg3);
	std::string	formatReply(const int code)const ; 
	std::string	irssiCode(const int code, std::string arg1, std::string arg2, std::string arg3);

/*GET*/
	struct sockaddr_in	getAddress() const;
	std::string			getNickname() const;
	std::string			getUsername() const;
	std::string			getHostname() const;
	std::string			getRealname() const;
	std::string			getBuffer() const;
	std::string			getIp() const;
	bool				getIsPingSent() const;
	bool				getQuit();
	int					getId() const;
	int					getSocket() const;
	int					getMaxId() const;
	int					getPass() const;


/*SET*/
	void 				setAddress(struct sockaddr_in address);
	void				setNickname(std::string nName);
	void				setUsername(std::string uName);
	void				setHostname(std::string hName);
	void				setBuffer(std::string buffer);
	void				setIp(std::string ip);
	void				setIsPingSent(bool isPingSentStatus);
	void 				setSocket(int socket);

};

std::ostream	&operator<<( std::ostream &flux, User const &rhs);

#endif
