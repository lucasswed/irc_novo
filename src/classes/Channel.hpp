/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:15 by ralves-g          #+#    #+#             */
/*   Updated: 2023/12/26 17:10:47 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <vector>
# include <string>
# include <map>

class Client;

class Channel {

	private:
		Channel();
		Channel(Channel const& ref);
		Channel const& operator=(Channel const& ref);
		
		std::string			_name;
		std::string			_key;
		int							_clientLimit;
		std::vector<Client>	_members;
		std::vector<Client>	_operators;
		std::vector<Client>	_invited;
		std::map<std::string, bool>	_modes;
		std::string			_topic;

	public:
		Channel(std::string name);
		~Channel();

		//getters
		std::string getName() const;
		std::string getKey() const;
		int getClientLimit() const;
		std::vector<Client> getMembers() const;
		std::vector<Client> getOperators() const;
		std::vector<Client> getInvited() const;
		std::map<std::string, bool> getModes() const;
		std::string getTopic() const;

		//setters
		void setName(std::string name);
		void setKey(std::string key);
		void setClientLimit(int limit);
		void setMember(Client &client);
		void setOperator(Client &client);
		void setInvited(Client &client);
		void setMode(std::string mode);
		void setTopic(std::string topic);

		void kickClient(Client Client);
		bool isOperator(Client Client);
};

#endif