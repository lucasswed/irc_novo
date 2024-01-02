/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:20 by ralves-g          #+#    #+#             */
/*   Updated: 2024/01/02 12:16:30 by ralves-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

# define SUCCESS 0
# define FAILURE 1

Channel::Channel(std::string const name): _name(name) {
	;
}

Channel const& Channel::operator=(Channel const &ref){
	;
}

Channel::~Channel() {
	;
}

std::string Channel::getName() const
{
	return (this->_name);
}

std::string Channel::getKey() const
{
	return (this->_key);
}

int Channel::getClientLimit() const
{
	return (this->_clientLimit);
}

std::vector<Client> Channel::getMembers() const {
	return (this->_members);
}

std::vector<Client> Channel::getOperators() const {
	return (this->_operators);
}

std::vector<Client> Channel::getInvited() const
{
	return (this->_invited);
}

std::map<std::string, bool> Channel::getModes() const
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

void Channel::setMembers(std::vector<Client> members)
{
	this->_members = members;
}

void Channel::setOperators(std::vector<Client> operators)
{
	this->_operators = operators;
}

void Channel::setInvited(std::vector<Client> invited)
{
	this->_invited = invited;
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

/*
bool	Channel::addClient(Client &client) {
	if (_members.empty())
	{
		_members.push_back(newClient);
		return SUCCESS;
	}
	for (std::vector<Client>::iterator itr = _members.begin(); itr != _members.end(); itr++)
	{
		if (itr->getFd() == newClient.getFd() 
			|| itr->getUsername() == newClient.getUsername()
			|| itr->getNickname() == newClient.getNickname())
			return FAILURE;
	}
	_members.push_back(newClient);
	return SUCCESS;
}

int	Channel::addOperator(Client newOP) {
	this->addMember(newOP);
	if (_members.empty())
		return FAILURE;
	for (std::vector<Client>::iterator itr = _members.begin(); itr != _members.end(); itr++)
	{
		if (itr->getFd() == newOP.getFd())
		{
			_operators.push_back(newOP);
			if (!itr->isOperator())
			{
				itr->setOperator(true);
				return SUCCESS;
			}
			else
			{
				//msg user ja e um operator
				return FAILURE;
			}
		}
	}
	return FAILURE;
}

void Channel::setClientLimit(int limit ) {
	_clientLimit = limit;
}

bool Channel::isMember(Client client) {
	for (int i = 0; i < _members.size(); i++)
		if (client.getFd() == _members[i].getFd())
			return true;
	return false;
}

bool Channel::isOperator(Client client) {
	for (int i = 0; i < _operators.size(); i++)
		if (client.getFd() == _operators[i].getFd())
			return true;
	return false;
}

bool Channel::isInvited(Client client) {
	for (int i = 0; i < _invited.size(); i++)
		if (client.getFd() == _invited[i].getFd())
			return true;
	return false;
}
*/void Channel::addMember(Client &client) {
	if (!isMember(client))
		_members.push_back(client);
}

void Channel::addOperator(Client &client) {
	if (!isOperator(client))
		_operators.push_back(client);
}
void Channel::Invite(Client &client) {
	if (!isInvited(client))
		_invited.push_back(client);
}

void Channel::setTopic(std::string topic) {
	_topic = topic;
}

void Channel::setMode(std::string mode, bool flag) {
	if(_modes.find(mode) != _modes.end())
		_modes.find(mode)->second = flag;
}

void Channel::kickClient(Client client) {
	if (isOperator(client))
		for (std::vector<Client>::iterator itr = _operators.begin(); itr != _operators.end(); itr++)
			if (itr->getFd() == client.getFd())
				_operators.erase(itr);
	if(isInvited(client))
		for (std::vector<Client>::iterator itr = _invited.begin(); itr != _invited.end(); itr++)
			if (itr->getFd() == client.getFd())
				_invited.erase(itr);
	if (isMember(client))
		for (std::vector<Client>::iterator itr = _members.begin(); itr != _members.end(); itr++)
			if (itr->getFd() == client.getFd())
				_members.erase(itr);
}

