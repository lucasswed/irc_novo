/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:20 by ralves-g          #+#    #+#             */
/*   Updated: 2024/01/03 11:02:37 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

# define SUCCESS 0
# define FAILURE 1

Channel::Channel(std::string const name): _name(name) {
	_modes.insert(std::pair<std::string, bool>("INVITE", false));
	_modes.insert(std::pair<std::string, bool>("TOPIC", false));
	_modes.insert(std::pair<std::string, bool>("KEY", false));
	_modes.insert(std::pair<std::string, bool>("OPERATOR", false));
	_modes.insert(std::pair<std::string, bool>("LIMIT", false));
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

std::vector<int> Channel::getMembers() const {
	return (this->_members);
}

std::vector<int> Channel::getOperators() const {
	return (this->_operators);
}

std::vector<int> Channel::getInvited() const
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

int Channel::getMode(std::string mode) const {
	return (_modes.find("mode") != _modes.end() ? _modes.find("mode")->second : -1);
}

void Channel::setClientLimit(int limit ) {
	_clientLimit = limit;
}

bool Channel::isMember(int client) {
	for (int i = 0; i < _members.size(); i++)
		if (client == _members[i])
			return true;
	return false;
}

bool Channel::isOperator(int client) {
	for (int i = 0; i < _operators.size(); i++)
		if (client == _operators[i])
			return true;
	return false;
}

bool Channel::isInvited(int client) {
	for (int i = 0; i < _invited.size(); i++)
		if (client == _invited[i])
			return true;
	return false;
}

void Channel::addMember(int client) {
	if (!isMember(client))
		_members.push_back(client);
}

void Channel::addOperator(int client) {
	if (!isOperator(client))
		_operators.push_back(client);
}
void Channel::Invite(int client) {
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

void Channel::kickClient(int client) {
	if (isOperator(client))
		for (std::vector<int>::iterator itr = _operators.begin(); itr != _operators.end(); itr++)
			if (*itr == client)
				_operators.erase(itr);
	if(isInvited(client))
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