/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:20 by ralves-g          #+#    #+#             */
/*   Updated: 2024/01/04 16:02:15 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

#define SUCCESS 0
#define FAILURE 1

Channel::Channel(std::string const name) : _name(name)
{
	_modes.insert(std::pair<std::string, int>("INVITE", 0));
	_modes.insert(std::pair<std::string, int>("TOPIC", 0));
	_modes.insert(std::pair<std::string, int>("KEY", 0));
	_modes.insert(std::pair<std::string, int>("OPERATOR", 0));
	_modes.insert(std::pair<std::string, int>("LIMIT", 0));
}

Channel::~Channel()
{
	return;
}

std::string Channel::getName() const
{
	return (this->_name);
}

std::string Channel::getKey() const
{
	return (this->_key);
}

size_t Channel::getClientLimit() const
{
	return (this->_clientLimit);
}

std::vector<int> Channel::getMembers() const
{
	return (this->_members);
}

std::vector<int> Channel::getOperators() const
{
	return (this->_operators);
}

std::vector<int> Channel::getInvited() const
{
	return (this->_invited);
}

std::map<std::string, int> Channel::getModes() const
{
	return (this->_modes);
}

std::string Channel::getTopic() const
{
	return (this->_topic);
}

void Channel::setName(std::string name)
{
	this->_name = name;
}

void Channel::setKey(std::string key)
{
	this->_key = key;
}

void Channel::setClientLimit(int limit)
{
	this->_clientLimit = limit;
}

void Channel::addMembers(std::vector<int> members)
{
	this->_members = members;
}

void Channel::addOperators(std::vector<int> operators)
{
	this->_operators = operators;
}

void Channel::Invites(std::vector<int> invited)
{
	this->_invited = invited;
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

int Channel::getMode(std::string mode) const
{
	std::clog << "getMode: " << mode << std::endl;
	for (std::map<std::string, int>::const_iterator it = _modes.begin(); it != _modes.end(); it++)
		std::clog << it->first << " " << it->second << std::endl;
	// for (std::map<std::string, bool>::const_iterator it = _modes.begin(); it != _modes.end(); it++)
	// 	if (it->first == mode)
	// 		return (it->second);
	return (-1);
}

bool Channel::isMember(int client)
{
	for (size_t i = 0; i < _members.size(); i++)
		if (client == _members[i])
			return true;
	return false;
}

bool Channel::isOperator(int client)
{
	for (size_t i = 0; i < _operators.size(); i++)
		if (client == _operators[i])
			return true;
	return false;
}

bool Channel::isInvited(int client)
{
	for (size_t i = 0; i < _invited.size(); i++)
		if (client == _invited[i])
			return true;
	return false;
}

void Channel::addMember(int client)
{
	if (!isMember(client))
		_members.push_back(client);
}

void Channel::addOperator(int client)
{
	if (!isOperator(client))
		_operators.push_back(client);
}
void Channel::Invite(int client)
{
	if (!isInvited(client))
		_invited.push_back(client);
}

void Channel::setMode(std::string mode, bool flag)
{
	if (_modes.find(mode) != _modes.end())
		_modes.find(mode)->second = flag;
}

void Channel::kickClient(int client)
{
	if (isOperator(client))
		for (std::vector<int>::iterator itr = _operators.begin(); itr != _operators.end(); itr++)
			if (*itr == client)
				_operators.erase(itr);
	if (isInvited(client))
		for (std::vector<int>::iterator itr = _invited.begin(); itr != _invited.end(); itr++)
			if (*itr == client)
				_invited.erase(itr);
	if (isMember(client))
		for (std::vector<int>::iterator itr = _members.begin(); itr != _members.end(); itr++)
			if (*itr == client)
				_members.erase(itr);
}

void Channel::messageAll(std::string msg)
{
	for (std::vector<int>::iterator it = _members.begin(); it != _members.end(); it++)
		Manager::sendMessage(msg, *it);
}

void Channel::messageAll(std::string msg, int senderFd)
{
	for (std::vector<int>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (*it == senderFd)
			continue;
		Manager::sendMessage(msg, *it);
	}
}