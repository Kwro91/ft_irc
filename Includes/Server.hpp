#ifndef SERVER_HPP
#define SERVER_HPP

#include "Define.hpp"

class User;
class Channel;

class Server
{
	private :
		std::string				_name;
		std::string				_password;
		std::string				_buffer;
		std::string				_serverInfo;
		bool					_serverState;
		int 					_port;
		int						_serverSocket;
		fd_set					_actualSocket;
		fd_set					_okSocket;
		struct sockaddr_in		_serverAddr;
		time_t					_innactive;
		struct timeval			_tv;
		std::map<int , User *>	_userList;
		std::vector<Channel *>	_channelList;


	public :
		Server();
		Server(Server &other);
		Server &operator=(Server &other);
		~Server();

		bool 	execServer();
		bool	checkArguments(int ac, char **av);
		int		setupSocket();
		bool	run();
		void	socketActivity();
		bool	finish();

		bool	checkRunning();
		void	addNewUser();
		void	checkWrongPasswords();
	void	checkNobodyConnected(bool &isRunning);


//CHANNEL
		 void	createChannel(User* admin, std::string nameChannel);
		 bool	channelExists(std::string nameChannel);
		void	rmChannel(std::string nameChannel);

//USER
		 void	addUserToChannel(User* user, std::string nameChannel);
		void	rmUserFromServer(User* user, std::string cause);
		void	rmUserFromChannel(User* user, std::string nameChannel);

//MSG
		void	message(int userSocketNumber);

//COMMANDE
		bool	checkNickUsed(std::string nickName);

//SET
		void	setRunning(bool state);
		void	setServerSocket(int socket);


//GET
	time_t					const *getStartT() const;
	std::string				getName() const;
	std::string				getPassword() const;
	std::string				getServerState() const;
	std::string				getServerInfo() const;
	int						getPort() const;
	int						getServerSocket() const;
	size_t					getUserName() const;
	User*					getUserNick(std::string nickName);
	 Channel*				getChannel(std::string nameChannel);
	std::vector<Channel *> *getChannelList();
	User*	getUser(std::string nickName);



	class MyError : public std::exception
	{
		private :
			std::string error;

		public :
			MyError(const std::string &message);
			const char *what() const throw();
			virtual ~MyError() throw();
	};
};

std::ostream	&operator<<( std::ostream & flux, Server const & rhs );

#endif
