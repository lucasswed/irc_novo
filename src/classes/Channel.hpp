/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:15 by ralves-g          #+#    #+#             */
/*   Updated: 2024/01/02 16:14:44 by ralves-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <vector>
# include <string>
# include <map>
# include "Manager.hpp"

class Client;

class Channel {

	private:
		Channel();
		Channel(Channel const& ref);
		Channel const& operator=(Channel const& ref);
		
		std::string					_name;
		std::string					_key;
		int							_clientLimit;

		//change users to FDs!!!
		std::vector<int>	_members;
		std::vector<int>	_operators;
		std::vector<int>	_invited;
		std::map<std::string, bool>	_modes;
		std::string					_topic;

	public:
		Channel(std::string const name);
		~Channel();

		//getters
		std::string getName() const;
		std::string getKey() const;
		int getClientLimit() const;
		std::vector<int> getMembers() const;
		std::vector<int> getOperators() const;
		std::vector<int> getInvited() const;
		int getMode(std::string mode) const;
		std::string getTopic() const;
		bool isMember(int client);
		bool isOperator(int Client);
		bool isInvited(int Client);

		//setters
		void setName(std::string name);
		void setKey(std::string key);
		void setClientLimit(int limit);
		void addMembers(std::vector<int> members);
		void addOperators(std::vector<int> operators);
		void Invite(std::vector<int> invited);
		void setModes(std::map<std::string, bool> mode, bool flags);
		void setTopic(std::string topic);

		void addClient(Client &client);
		void kickClient(Client &client);
		bool isOperator(Client &client);
		void changeMode(std::string mode);

		void addMember(int client);
		void Channel::addMember(int client);
		void addOperator(int client);
		void Invite(int client);
		void setTopic(std::string topic);
		void setMode(std::string mode, bool flag);
		void kickClient(int client);
		bool isOperator(int client);
		void changeMode(std::string mode);
		
		void messageAll(std::string msg);
		void messageAll(std::string msg, int senderFd);
		void addClient(Client client);
		void kickClient(Client client);
		bool isOperator(Client client);
		void changeMode(std::string mode);
};

#endif