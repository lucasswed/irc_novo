/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:20 by ralves-g          #+#    #+#             */
/*   Updated: 2023/12/20 15:28:42 by ralves-g         ###   ########.fr       */
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

std::vector<Client> Channel::getMembers() const {
	return (_members);
}

std::vector<Client> Channel::getOperators() const {
	return (_operators);
}

int	Channel::addMember(Client newClient) {
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


