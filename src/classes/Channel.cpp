/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:20 by ralves-g          #+#    #+#             */
/*   Updated: 2024/01/02 12:09:37 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

# define SUCCESS 0
# define FAILURE 1

Channel::Channel(std::string name): _name(name) {
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

void	Channel::rmMember(Client member) {
	if (_members.empty())
		return ;
	for (std::vector<Client>::iterator itr = _members.begin(); itr != _members.end(); itr++)
	{
		if (itr->getFd() == member.getFd())
			_members.erase(itr);
	}
}

int	Channel::isOperator(Client check) {
	if (_operators.empty())
		return false;
	for (std::vector<Client>::iterator itr = _operators.begin(); itr != _operators.end(); itr++)
	{
		if (itr->getFd() == check.getFd())
					return true;
	}
	return false;
}
*/
