#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Define.hpp"
#include "User.hpp"

class Channel{
	private:
		const std::string 					_name; //le nom du channel
		std::string							_topic; //le theme du channel
		bool								_topicOperator; // true si topic utilisable uniquement par l'operateur
		std::string							_logMsg;
		size_t								_maxUser;
		int									_id;
		size_t								_nbUser;
		std::vector<User*>					_userList;
		std::vector<int>					_operatorList; //Les id des personnes operator
		bool								_invite;
		std::vector<int>					_whiteList; //les id des personnes autorisees
		bool								_private;
		std::string							_password;

	public:
	 										Channel(User &admin, std::string name);
											Channel(std::string name, int id);
											~Channel();
											Channel(Channel &other);
											Channel &operator=(Channel &other);

		const size_t&						getNbUser() const;
		const int&							getId() const;
		int									getTopicOperator() const; //pour la cmd topic
		std::string							getUserNicks();
		const std::string&					getName() const;
		const std::string&					getLogMsg() const;
		const std::string&					getTopic() const;
		const std::vector<User*>&			getUsers() const;
		size_t								getMaxUser() const;
		
		void								setMaxUser(size_t maxUser);
		void								setTopic(std::string maxUser);
		void								setTopicOperator(int value);
		void								setLogMsg(std::string maxUser);
		void								setNbUser(size_t nb);


		void								addUser(User *u);
		void								removeUser(int id);
		void								printChannelUsers();
		int									findInUserList(User *u) const; //savoir si un User fait partie de la userList
		bool								checkUser(User *user) const;
		bool								checkOperator(User *u) const;
		void								addOperator(int id);
		void								removeOperator(int id);
		int									findInOperatorList(int id) const; //savoir si un nom fait partie de la operatorList
		bool								checkEmpty() const;
		// Invite
		const std::vector<int>&				getWhiteList() const;
		int									getInviteStatus() const;
		
		void								setInvite(bool value);
		
		void								addWhitelist(int id);
		void								addAllUsersToWhiteList();
		void								removeWhitelist(int id);
		void								printWhiteList();
		void								eraseWhiteList();
		int									findInWhiteList(int id) const; //savoir si un nom fait partie de la whitelist

		//Private
		int									getPrivateStatus() const;
		
		void								setPrivate(bool value);
		void								setPassword(std::string password);
		
		int									comparePassword(std::string password) const; //comparer un mdp avec le mdp prive secret

		void								modeOperator(); //mode -o
};

std::ostream &operator<<(std::ostream &out, const Channel &c);

#endif