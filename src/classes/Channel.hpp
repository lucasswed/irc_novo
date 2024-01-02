/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:15 by ralves-g          #+#    #+#             */
/*   Updated: 2024/01/02 15:41:18 by pcampos-         ###   ########.fr       */
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
		std::vector<Client>	_clients;
		std::vector<Client>	_operators;
		std::vector<Client>	_invited;
		std::map<std::string, bool>	_modes;
		std::string					_topic;

	public:
		Channel(std::string const name);
		~Channel();

		//getters
		std::string getName() const;
		std::string getKey() const;
		int getClientLimit() const;
		std::vector<Client> getMembers() const;
		std::vector<Client> getOperators() const;
		std::vector<Client> getInvited() const;
		int getMode(std::string mode) const;
		std::string getTopic() const;
		bool isMember(Client client);
		bool isInvited(Client Client);

		//setters
		void setName(std::string name);
		void setKey(std::string key);
		void setClientLimit(int limit);
		void addMembers(std::vector<Client> members);
		void addOperators(std::vector<Client> operators);
		void Invite(std::vector<Client> invited);
		void setModes(std::map<std::string, bool> mode, bool flags);
		void setTopic(std::string topic);

		void messageAll(std::string msg);
		void messageAll(std::string msg, int senderFd);
		void addClient(Client client);
		void kickClient(Client client);
		bool isOperator(Client client);
		void changeMode(std::string mode);
};

#endif