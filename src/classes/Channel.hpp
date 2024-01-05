/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:15 by ralves-g          #+#    #+#             */
/*   Updated: 2024/01/05 05:05:16 by ralves-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <vector>
#include <string>
#include <map>
#include "Manager.hpp"

class Client;

class Channel
{

private:
	std::string _name;
	std::string _key;
	int _clientLimit;

	// change users to FDs!!!
	std::vector<int> _members;
	std::vector<int> _operators;
	std::vector<int> _invited;
	std::map<std::string, int> _modes;
	std::string _topic;

public:
	Channel(std::string const name);
	~Channel();

	// getters
	std::string getName() const;
	std::string getKey() const;
	size_t getClientLimit() const;
	std::vector<int> getMembers() const;
	std::vector<int> getOperators() const;
	std::vector<int> getInvited() const;
	int getMode(std::string mode) const;
	std::map<std::string, int> getModes() const;
	std::string getTopic() const;
	bool isMember(int client);
	bool isOperator(int Client);
	bool isInvited(int Client);

	// setters
	void setName(std::string name);
	void setKey(std::string key);
	void setClientLimit(int limit);
	void addMember(int client);
	void addMembers(std::vector<int> members);
	void addOperator(int client);
	void addOperators(std::vector<int> operators);
	void Invite(int client);
	void Invites(std::vector<int> invited);
	void changeMode(std::string mode);
	void setModes(std::map<std::string, bool> mode, bool flags);
	void setTopic(std::string topic);
	void setMode(std::string mode, bool flag);
	void kickClient(int client);
	void partClient(int client);

	// msgs
	void messageAll(std::string msg);
	void messageAll(std::string msg, int senderFd);
};

#endif