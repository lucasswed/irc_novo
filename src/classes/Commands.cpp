/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 14:50:19 by ralves-g          #+#    #+#             */
/*   Updated: 2024/01/02 16:04:59 by ralves-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Manager.hpp"
# include "messageCodes.hpp"

int	Manager::getFdByNick(std::string nickname) {
	std::vector<Client>::iterator itr;
	for (itr = _clients.begin(); itr != _clients.end(); itr++)
	{
		if (nickname == itr->getNickname())
			break;
	}
	return (itr != _clients.end() ? itr->getFd() : -1);
}

std::string Manager::getNickByFd(int fd) {
	std::vector<Client>::iterator itr;
	for (itr = _clients.begin(); itr != _clients.end(); itr++)
	{
		if (fd == itr->getFd())
			break;
	}
	return (itr != _clients.end() ? itr->getNickname() : "");
}

std::vector<Channel>::iterator Manager::getChnlByName(std::string name) {
	std::vector<Channel>::iterator itr;
	for (itr = _channels.begin(); itr != _channels.end(); itr++)
	{
		if (name == itr->getName())
			break;
	}
	return (itr);
}

void Manager::createChannel(std::string name) {
	_channels.push_back(Channel(name));
}

void Manager::joinChannel(std::string channel, std::string key, int client) {
 	std::vector<Channel>::iterator itr = getChnlByName(channel);
	if (itr == _channels.end())
	{
		createChannel(channel);
		return ;
	}
	if (itr->getMode("INVITE") == 1)
	{
		if (!itr->isInvited(client))
		{
			sendMessage(formatMessage(client, "Error joining [" + channel + "] client needs and invite to join this channel"), client);
			return ;
		}
	}
	if (itr->getMode("KEY") && key != itr->getKey())
	{
		sendMessage(formatMessage(client, "Error joining [" + channel + "] wrong key"), client);
		return ;
	}
	if (itr->getMode("LIMIT") && itr->getMembers().size() >= itr->getClientLimit())
		itr->addMember(client);
	else
	{
		sendMessage(formatMessage(client, "Error joining [" + channel + "] channel user limit reached"), client);
		return ;
	}
}

void Manager::joinCmd(Client client) {
	std::vector<std::string> channels = ft_split(client.getCmd()[1], ",");
	// std::vector<std::string> msg = client.getCmd();
	// std::vector<std::string> channels = ft_split(msg.substr(0, msg.find_first_of(" ") - 1), ",");
		
	if (channels.size() == 1 && channels[0] == "0")
	{
		for (int i, i2 = 0; i < _channels.size(); i++)
			_channels[i].kickClient(client.getFd());
		return ;
	}
	std::vector<std::string> keys = ft_split(client.getCmd()[2], ",");
	std::vector<Channel>::iterator c_itr;

	if (channels.size() < keys.size())
	{
		sendMessage(formatMessage(client, "Error: too many keys for the number of channels"), client.getFd());
		return ;
	}
	for (int i, i2 = 0; i < channels.size(); i++)
	{
		if (getChnlByName(channels[i])->getMode("KEY") && i2 == keys.size())
		{
			sendMessage(formatMessage(client, "Error: not enough keys for the number of key protected channels"), client.getFd());
			return ;
		}
		joinChannel(channels[i], keys[i], client.getFd());
		if (getChnlByName(channels[i])->getMode("KEY"))
			i2++;
	}
}

void Manager::quitCmd(Client client) {
	for (int i, i2 = 0; i < _channels.size(); i++)
		_channels[i].kickClient(client.getFd());
	sendMessage(formatMessage(client, "Quit: Gone to have lunch"), client.getFd());
}

void Manager::kickCmd(Client client) {
	// std::vector<std::string> tokens = ft_split(msg, " ");
	std::vector<std::string> tokens = client.getCmd();
	if (tokens.size() != 3 && tokens.size() != 4)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
		return ;
	}
	std::vector<std::string> channels = ft_split(tokens[0], ",");
	std::vector<std::string> clients = ft_split(tokens[1], ",");
	if (channels.size() != clients.size() && channels.size() != 1)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
		return ;
	}
	if (channels.size() == 1)
	{
		for (int i = 0; i < clients.size(); i++)
			getChnlByName(channels[0])->kickClient(getFdByNick(clients[i]));
			//send message 
		return ;
	}
	for (int i  = 0; i <  clients.size(); i++)
		getChnlByName(channels[i])->kickClient(getFdByNick(clients[i]));
		//send message
}

void Manager::inviteCmd(Client client) {
	if (client.getCmd().size() != 3)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
		return ;
	}
	if (getFdByNick(client.getCmd()[1]) == -1)
	{
		sendMessage(formatMessage(client, NOSUCHNICK) + " " + client.getCmd()[1] + " :No such nick/channel", client.getFd());
		return ;
	}
	if (getChnlByName(client.getCmd()[2]) == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " #" + client.getCmd()[2] + " :No such channel", client.getFd());
		return ;
	}
	if (getChnlByName(client.getCmd()[2])->isMember(getFdByNick(client.getCmd()[1])))
	{
		sendMessage(formatMessage(client, ERR_USERONCHANNEL) + " " + client.getCmd()[1] + " #" + client.getCmd()[2] + " :is already on channel", client.getFd());
		return ;
	}
	if (getChnlByName(client.getCmd()[2])->getMode("INVITED") && !(getChnlByName(client.getCmd()[2])->isOperator(client.getFd())))
	{
		sendMessage(formatMessage(client, INVITEONLYCHAN) + " #" + client.getCmd()[2] + " :Cannot join channel (+i)", client.getFd());
		return ;
	}
	getChnlByName(client.getCmd()[2])->Invite(getFdByNick(client.getCmd()[1]));
	sendMessage(formatMessage(client, INVITING) + " #" + client.getCmd()[2] + " " + client.getCmd()[1], client.getFd());
}

