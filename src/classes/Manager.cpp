/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:13:17 by lucas-ma          #+#    #+#             */
/*   Updated: 2024/01/03 16:01:33 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Manager.hpp"

std::vector<Client> Manager::_clients;
std::string Manager::_hostname = "localhost";
std::string Manager::_servername = "irc.server.com";

void Manager::on(std::string event, cmdFunction function)
{
	_cmdMap.insert(std::pair<std::string, cmdFunction>(event, function));
}

void Manager::on(std::string event, modeFunction function)
{
	_modeMap.insert(std::pair<std::string, modeFunction>(event, function));
}

void Manager::fillMaps(void)
{
	//COMMANDS
	// on("JOIN", &);
	// on("QUIT", &);
	// on("KICK", &);
	// on("PART", &);
	// on("MODE", &);
	// on("TOPIC", &Manager::topicCmd);
	// on("INVITE", &);
	// on("PRIVMSG", &);
	// on("LIST", &);
	// on("NAMES", &);
	// on("LUSERS", &);
	// on("NICK", &);
	//MODES
	// on("INVITE", &);
	// on("TOPIC", &);
	// on("KEY", &);
	// on("OPERATOR", &);
	// on("LIMIT", &);

}

bool Manager::addClient(int fd)
{
	if (Manager::getClientByFd(fd) != _clients.end())
		return true;
	_clients.push_back(Client(fd));
	std::cout << GREEN << "Client added succesfully" << RESET << std::endl;
	return false;
}

void Manager::removeClient(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
		{
			// remove the client from all channels
			_clients.erase(_clients.begin() + i);
			close(fd);
			std::cout << RED << "Client removed succesfully" << RESET << std::endl;
			return;
		}
	}
}

std::vector<Client>::iterator Manager::getClientByFd(int fd)
{
	std::vector<Client>::iterator client = _clients.begin();
	for (; client != _clients.end(); client++)
		if (client->getFd() == fd)
			break;
	return client;
}

bool Manager::checkClientData(Client &client)
{
	if (client.getLogged())
		return (true);
	for (size_t i = 0; i < client.getCmd().size(); i++)
	{
		if (client.getCmd()[i] == "PASS")
			client.setPassword(client.getCmd()[i + 1]);
		else if (client.getCmd()[i] == "NICK")
			client.setNickname(client.getCmd()[i + 1]);
		else if (client.getCmd()[i] == "USER")
			client.setUsername(client.getCmd()[i + 1]);
	}
	return (false);
}

bool Manager::checkPassword(Client const &client, std::string const &password)
{
	if (client.getPassword() != password || client.getPassword().empty())
	{
		std::cout << RED << "Wrong password or empty" << RESET << std::endl;
		// sendMessage() to the client with the error
		sendMessage(formatMessage(client, PASSWDMISMATCH) + " :Password required", client.getFd());
		return (false);
	}
	return (true);
}

bool Manager::checkNick(Client const &client)
{
	if (!isNickValid(client.getNickname()))
	{
		std::cout << RED << "Nickname wrong formatted" << RESET << std::endl;
		// sendMessage() to the client with the error nick not valid
		sendMessage(formatMessage(client, ERRONEUSNICKNAME) + " :Erroneus nickname", client.getFd());
		return (false);
	}
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getFd() == client.getFd())
			continue;
		if (it->getNickname() == client.getNickname())
		{
			std::cout << RED << "Nickname already in use" << RESET << std::endl;
			// sendMessage() to the client with the nick in use error
			sendMessage(formatMessage(client, NICKNAMEINUSE) + " :Nickname is already in use", client.getFd());
			return (false);
		}
	}
	std::cout << GREEN << "Nickname checked" << RESET << std::endl;
	return (true);
}

void Manager::sendMessage(std::string message, int clientFd)
{
	message += "\r\n";
	std::cout << PURPLE << "sending message: " << message << RESET << std::endl;
	if (send(clientFd, message.c_str(), message.length(), 0) == -1)
	{
		std::cerr << LIGHTRED << "Send function failed" << RESET << std::endl;
		exit(1);
	}
}

std::string Manager::formatMessage(Client const &client, std::string const &code)
{
	if (!client.getNickname().empty())
		return (":" + _hostname + " " + code + " " + client.getNickname());
	else
		return (":" + _hostname + " " + code + " " + "teste");
}

std::map<std::string, cmdFunction> Manager::getCmdMap(void)
{
	return (_cmdMap);
}

std::vector<Channel>::iterator Manager::getChnlByName(std::string name) {
	std::vector<Channel>::iterator itr;
	for (itr = _channels.begin(); itr != _channels.end(); itr++)
	{
		if (toUP(name) == toUP(itr->getName()))
			break;
	}
	return (itr);
}

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

void Manager::topicCmd(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() < 2)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR: Not enought paramaters", client.getFd());
		return ;
	}
	std::vector<Channel>::iterator channelIt = getChnlByName(cmd[1]);
	if (channelIt == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + cmd[1] + " :No such channel", client.getFd());
		return;
	}
	if (cmd.size() < 3)
	{
		if (channelIt->getTopic().empty())
		{
			sendMessage(formatMessage(client, "331") + " " + cmd[1] + " :No topic is set", client.getFd());
			return;
		}
		else
		{
			sendMessage(formatMessage(client, RPL_TOPIC) + " " + cmd[1] + " :" + channelIt->getTopic(), client.getFd());
			return;
		}
	}
	else
	{
		if (channelIt->getMode("TOPIC") && !channelIt->isOperator(client.getFd()))
		{
			sendMessage(formatMessage(client, CHANOPRIVSNEEDED) + " " + cmd[1] + " :You're not channel operator", client.getFd());
			return;
		}
		else
		{
			channelIt->setTopic(cmd[2]);
			channelIt->messageAll(formatMessage(client, RPL_TOPIC) + " " + cmd[1] + " :" + channelIt->getTopic());
		}
	}
}

void Manager::joinChannel(std::string channel, std::string key, Client client) {
 	std::vector<Channel>::iterator itr = getChnlByName(channel);
	if (itr == _channels.end())
	{
		createChannel(channel);
		return ;
	}
	if (itr->getMode("INVITE") == 1)
	{
		if (!itr->isInvited(client.getFd()))
		{
			sendMessage(formatMessage(client, INVITEONLYCHAN) + " " + channel + " :Cannot join channel (+i)", client.getFd());
			return ;
		}
	}
	if (itr->getMode("KEY") && key != itr->getKey())
	{
		sendMessage(formatMessage(client, BADCHANNELKEY) + " " + channel + " :Cannot join channel (+k)", client.getFd());
		return ;
	}
	if (itr->getMode("LIMIT") && itr->getMembers().size() >= itr->getClientLimit())
	{
		itr->addMember(client.getFd());
		sendMessage(formatMessage(client, "Successfully joined " + channel), client.getFd());
		if (getChnlByName(channel)->getMode("t") == 1)
			sendMessage(formatMessage(client, RPL_TOPIC) + " " + channel + " :No topic is set", client.getFd());
		else
			sendMessage(formatMessage(client, RPL_NOTOPIC) + " " + channel + " :" + getChnlByName(channel)->getTopic(), client.getFd());
	}
	else
	{
		sendMessage(formatMessage(client, CHANNELISFULL) + " " + channel + " :Cannot join channel (+l)", client.getFd());
		return ;
	}
}

void Manager::joinCmd(Client &client) {
	std::vector<std::string> channels = ft_split(client.getCmd()[1], ",");

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
		sendMessage(formatMessage(client, "Error: too many keys for the number of channels"), client.getFd()); //to check
		return ;
	}
	for (int i, i2 = 0; i < channels.size(); i++)
	{
		if (getChnlByName(channels[i])->getMode("KEY") && i2 == keys.size())
		{
			sendMessage(formatMessage(client, "Error: not enough keys for the number of key protected channels"), client.getFd()); //to check
			return ;
		}
		joinChannel(channels[i], keys[i], client);
		if (getChnlByName(channels[i])->getMode("KEY"))
			i2++;
	}
}

void Manager::quitCmd(Client &client) {
	for (int i, i2 = 0; i < _channels.size(); i++)
		_channels[i].kickClient(client.getFd());
	sendMessage(formatMessage(client, "Quit: Gone to have lunch"), client.getFd());
}

void Manager::kickCmd(Client &client) {
	std::vector<std::string> tokens = client.getCmd();
	if (tokens.size() != 3 && tokens.size() != 4)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
		return ;
	}
	if (getChnlByName(tokens[0]) == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + tokens[0] + " :No such channel", client.getFd());
		return ;
	}
	if (!getChnlByName(tokens[0])->isInvited(client.getFd()))
	{
		sendMessage(formatMessage(client, CHANOPRIVSNEEDED) + " " + tokens[0] + " :You're not channel operator", client.getFd());
		return ;
	}
	//multiple channels and users
	// std::vector<std::string> channels = ft_split(tokens[0], ",");
	// std::vector<std::string> clients = ft_split(tokens[1], ",");
	// if (channels.size() != clients.size() && channels.size() != 1)
	// {
	// 	sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
	// 	return ;
	// }
	// if (channels.size() == 1)
	// {
	// 	for (int i = 0; i < clients.size(); i++)
	// 		getChnlByName(channels[0])->kickClient(getFdByNick(clients[i]));
	// 		//send message 
	// 	return ;
	// }
	getChnlByName(tokens[0])->kickClient(getFdByNick(tokens[1]));
}

void Manager::inviteCmd(Client &client) {
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
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + client.getCmd()[2] + " :No such channel", client.getFd());
		return ;
	}
	if (getChnlByName(client.getCmd()[2])->isMember(getFdByNick(client.getCmd()[1])))
	{
		sendMessage(formatMessage(client, ERR_USERONCHANNEL) + " " + client.getCmd()[1] + " " + client.getCmd()[2] + " :is already on channel", client.getFd());
		return ;
	}
	if (getChnlByName(client.getCmd()[2])->getMode("INVITED") && !(getChnlByName(client.getCmd()[2])->isOperator(client.getFd())))
	{
		sendMessage(formatMessage(client, INVITEONLYCHAN) + " " + client.getCmd()[2] + " :Cannot join channel (+i)", client.getFd());
		return ;
	}
	getChnlByName(client.getCmd()[2])->Invite(getFdByNick(client.getCmd()[1]));
	sendMessage(formatMessage(client, INVITING) + " " + client.getCmd()[2] + " " + client.getCmd()[1], client.getFd());
}

void Manager::partCmd(Client &client) {
	if (client.getCmd().size() != 2)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
		return ;
	}
	std::vector<std::string> channels = ft_split(client.getCmd()[1], " ");
	for (int i = 0; i < channels.size(); i++)
	{
		if (getChnlByName(channels[i]) == _channels.end())
			sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + channels[i] + " :No such channel", client.getFd());
		else if (!getChnlByName(channels[i])->isMember(client.getFd()))
			sendMessage(formatMessage(client, USERNOTINCHANNEL) + " " + client.getNickname() + " " + channels[i] + " :They aren't on that channel", client.getFd());
		else
		{
			getChnlByName(channels[i])->kickClient(client.getFd());
			//msg
		}
	}
}

void Manager::listCmd(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() == 1)
	{
		sendMessage(formatMessage(client, RPL_LISTSTART) + " :List of Channels", client.getFd());
		for (std::vector<Channel>::iterator channelIt = _channels.begin(); channelIt != _channels.end(); channelIt++)
			sendMessage(formatMessage(client, RPL_LIST) + " " +  std::to_string(channelIt->getMembers().size()) + ": " + channelIt->getTopic(), client.getFd());
		sendMessage(formatMessage(client, RPL_LISTEND) + " :End of list", client.getFd());
		return ;
	}
	std::vector<std::string> chans = ft_split(cmd[1], ",");
	for(std::vector<std::string>::iterator it = chans.begin(); it != chans.end(); it++)
	{
		std::vector<Channel>::iterator channelIt = getChnlByName(*it);
		if (channelIt == _channels.end())
		{
			sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + *it + " :No such channel", client.getFd());
			return;
		}
	}
	sendMessage(formatMessage(client, RPL_LISTSTART) + " :List of Channels", client.getFd());
	for(std::vector<std::string>::iterator it = chans.begin(); it != chans.end(); it++)
	{
		std::vector<Channel>::iterator channelIt = getChnlByName(*it);
		sendMessage(formatMessage(client, RPL_LIST) + " " +  std::to_string(channelIt->getMembers().size()) + ": " + channelIt->getTopic(), client.getFd());
	}
	sendMessage(formatMessage(client, RPL_LISTEND) + " :End of list", client.getFd());
}

void Manager::lusersCmd(Client &client)
{
	sendMessage(formatMessage(client, LUSERCLIENT) + " :There are " + std::to_string(_clients.size()) + " users on 1 server", client.getFd());
	int ops;
	for (std::vector<Channel>::iterator channelIt = _channels.begin(); channelIt != _channels.end(); channelIt++)
		ops += channelIt->getOperators().size();
	if (ops > 0)
		sendMessage(formatMessage(client, LUSEROP) + std::to_string(ops) + " :operator(s) online", client.getFd());
	if (_channels.size() > 0)
		sendMessage(formatMessage(client, LUSERCHANNELS) + std::to_string(_channels.size()) + " :channels formed", client.getFd());
}

void Manager::nickCmd(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() < 2)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR: Not enought paramaters", client.getFd());
		return ;
	}
	if (!isNickValid(cmd[1]))
	{
		sendMessage(formatMessage(client, ERRONEUSNICKNAME) + " :Erroneus nickname", client.getFd());
		return ;
	}
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getNickname() == cmd[1])
		{
			sendMessage(formatMessage(client, NICKNAMEINUSE) + " :Nickname is already in use", client.getFd());
			return ;
		}
	}
	client.setNickname(cmd[1]);
}

void Manager::privmsgCmd(Client& client) {
	if (client.getCmd().size() < 2)
	{
		
	}
	if (client.getCmd().size() < 3)
	{
		
	}
}
