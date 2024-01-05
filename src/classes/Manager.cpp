/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:13:17 by lucas-ma          #+#    #+#             */
/*   Updated: 2024/01/05 05:24:26 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Manager.hpp"

std::vector<Client> Manager::_clients;
std::vector<Channel> Manager::_channels;
std::map<std::string, cmdFunction> Manager::_cmdMap;
std::map<std::string, modeFunction> Manager::_modeMap;
std::string Manager::_hostname = "localhost";
std::string Manager::_servername = "irc.server.gg";

void Manager::onCmd(std::string event, cmdFunction function)
{
	_cmdMap.insert(std::pair<std::string, cmdFunction>(event, function));
}

void Manager::onMode(std::string event, modeFunction function)
{
	_modeMap.insert(std::pair<std::string, modeFunction>(event, function));
}

std::string Manager::formatMessage(const Client &client)
{
	return (":" + client.getNickname() + "!" + client.getUsername() + "@" + Manager::_hostname);
}

void Manager::fillMaps(void)
{
	// COMMANDS
	onCmd("WHO", &Manager::whoCmd);
	onCmd("JOIN", &Manager::joinCmd);
	onCmd("QUIT", &Manager::quitCmd);
	onCmd("KICK", &Manager::kickCmd);
	onCmd("PART", &Manager::partCmd);
	onCmd("MODE", &Manager::modeCmd);
	onCmd("TOPIC", &Manager::topicCmd);
	onCmd("INVITE", &Manager::inviteCmd);
	onCmd("PRIVMSG", &Manager::privmsgCmd);
	onCmd("LIST", &Manager::listCmd);
	onCmd("LUSERS", &Manager::lusersCmd);
	onCmd("NICK", &Manager::nickCmd);

	// MODES
	onMode("INVITE", &Manager::inviteMode);
	onMode("TOPIC", &Manager::topicMode);
	onMode("KEY", &Manager::keyMode);
	onMode("OPERATOR", &Manager::operatorMode);
	onMode("LIMIT", &Manager::limitMode);
}

void Manager::sendWhoMessage(std::vector<int> list, Client &sender, std::string channelName)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		Client &client = *Manager::getClientByFd(list[i]);
		std::string status;
		if (channelName != "*")

			if (Manager::getChnlByName(channelName)->isOperator(list[i]))
				status = Manager::getChnlByName(channelName)->isOperator(list[i]) ? "@" : "+";
		sendMessage(formatMessage(sender, RPL_WHOREPLY) + " " + channelName + " localhost irc.server.gg " + client.getNickname() + " H" + status + " :1" + client.getUsername(), sender.getFd());
	}
	sendMessage(formatMessage(sender, RPL_ENDOFWHO) + " " + channelName + " :End of WHO list", sender.getFd());
}

bool Manager::isValidChannel(std::string const &channelName, Client &client)
{
	if (channelName[0] != '#')
		return (false);

	std::vector<Channel>::iterator channelIt = getChnlByName(channelName);
	if (channelIt == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + channelName + " :No such channel", client.getFd());
		return (false);
	}

	return (true);
}

void Manager::whoCmd(Client &client)
{
	std::clog << LIGHTGREEN << "WHO CMD" << RESET << std::endl;
	if (client.getCmd().size() == 1)
	{
		std::vector<int> allClients;
		for (size_t i = 0; i < _clients.size(); i++)
			allClients.push_back(_clients[i].getFd());
		sendWhoMessage(allClients, client, "*");
		return;
	}
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() <= 3 && isValidChannel(cmd[1], client))
	{
		Channel &channel = *getChnlByName(cmd[1]);
		if (cmd.size() == 3 && cmd[2] == "o")
			sendWhoMessage(channel.getOperators(), client, channel.getName());
		else
			sendWhoMessage(channel.getMembers(), client, channel.getName());
	}
	else
		sendMessage(formatMessage(client, UNKNOWNCOMMAND) + " :USAGE: WHO [<channel> [<0>]]", client.getFd());
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

void Manager::setChannOpps(Client &client)
{
	Manager::sendMessage(Manager::formatMessage(client, WELCOME_MESSAGE) + " :Welcome to the Darjest Region of the Internet", client.getFd());
	Manager::sendMessage(Manager::formatMessage(client, RPL_YOURHOST) + " :Your host is " + _hostname + ", running version 1.0", client.getFd());
	Manager::sendMessage(Manager::formatMessage(client, RPL_CREATED) + " :This server was created sometime in 2023", client.getFd());
	Manager::sendMessage(Manager::formatMessage(client, CHANNEL_OPPS) + " :CHANTYPES=#", client.getFd());
	Manager::sendMessage(Manager::formatMessage(client, CHANNEL_OPPS) + " :CHANMODES=i,t,k,o,l", client.getFd());
	client.setRegistered(true);
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

std::vector<Channel>::iterator Manager::getChnlByName(std::string name)
{
	std::vector<Channel>::iterator itr;
	for (itr = _channels.begin(); itr != _channels.end(); itr++)
	{
		if (toUP(name) == toUP(itr->getName()))
			break;
	}
	return (itr);
}

int Manager::getFdByNick(std::string nickname)
{
	std::vector<Client>::iterator itr;
	for (itr = _clients.begin(); itr != _clients.end(); itr++)
	{
		if (nickname == itr->getNickname())
			break;
	}
	return (itr != _clients.end() ? itr->getFd() : -1);
}

std::string Manager::getNickByFd(int fd)
{
	std::vector<Client>::iterator itr;
	for (itr = _clients.begin(); itr != _clients.end(); itr++)
	{
		if (fd == itr->getFd())
			break;
	}
	return (itr != _clients.end() ? itr->getNickname() : "");
}

void Manager::createChannel(std::string name)
{
	_channels.push_back(Channel(name));
}

void Manager::topicCmd(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() < 2)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " TOPIC :Not enought paramaters", client.getFd());
		return;
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
			sendMessage(formatMessage(client, RPL_NOTOPIC) + " " + cmd[1] + " :No topic is set", client.getFd());
			return;
		}
		else
		{
			sendMessage(formatMessage(client, RPL_TOPIC) + " " + cmd[1] + " " + channelIt->getTopic(), client.getFd());
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
			channelIt->messageAll(formatMessage(client, RPL_TOPIC) + " " + cmd[1] + " " + channelIt->getTopic());
		}
	}
}

void Manager::joinChannel(std::string channel, std::string key, Client client)
{
	std::vector<Channel>::iterator itr = getChnlByName(channel);
	if (itr == _channels.end())
	{
		createChannel(channel);
		getChnlByName(channel)->addOperator(client.getFd());
		// sendMessage(formatMessage(client, "Successfully joined " + channel), client.getFd());
		sendMessage(formatMessage(client) + " JOIN " + channel, client.getFd());
		if (getChnlByName(channel)->getMode("TOPIC") == 1)
			sendMessage(formatMessage(client, RPL_TOPIC) + " " + channel + " :No topic is set", client.getFd());
		else
			sendMessage(formatMessage(client, RPL_NOTOPIC) + " " + channel + " " + getChnlByName(channel)->getTopic(), client.getFd());

		return;
	}
	if (itr->getMode("INVITE") == 1)
	{
		if (!itr->isInvited(client.getFd()))
		{
			sendMessage(formatMessage(client, INVITEONLYCHAN) + " " + channel + " :Cannot join channel (+i)", client.getFd());
			return;
		}
	}
	if (itr->getMode("KEY") && key != itr->getKey())
	{
		sendMessage(formatMessage(client, BADCHANNELKEY) + " " + channel + " :Cannot join channel (+k)", client.getFd());
		return;
	}

	if (itr->getMode("LIMIT") == 1 && itr->getMembers().size() >= itr->getClientLimit())
	{
		sendMessage(formatMessage(client, CHANNELISFULL) + " " + channel + " :Cannot join channel (+l)", client.getFd());
		return;
	}
	else
	{
		itr->addMember(client.getFd());
		// sendMessage(formatMessage(client, "Successfully joined " + channel), client.getFd());
		sendMessage(formatMessage(client) + " JOIN " + channel, client.getFd());
		if (getChnlByName(channel)->getMode("TOPIC") == 1)
			sendMessage(formatMessage(client, RPL_TOPIC) + " " + channel + " :No topic is set", client.getFd());
		else
			sendMessage(formatMessage(client, RPL_NOTOPIC) + " " + channel + " " + getChnlByName(channel)->getTopic(), client.getFd());
		return;
	}
}

void Manager::joinCmd(Client &client)
{
	std::vector<std::string> channelsToEnter = ft_split(client.getCmd()[1], ",");
	for (size_t i = 0; i < channelsToEnter.size(); i++)
	{
		std::clog << (channelsToEnter[i][0] != '#') << " <--- test" << std::endl;
		if (channelsToEnter[i].length() <= 1 || channelsToEnter[i][0] != '#')
		{
			sendMessage(formatMessage(client, NEEDMOREPARAMS) + "JOIN" + " :Not enought paramaters", client.getFd());
			return;
		}
	}
	if (channelsToEnter.size() == 1 && channelsToEnter[0] == "0")
	{
		for (size_t i = 0; i < _channels.size(); i++)
			_channels[i].kickClient(client.getFd());
		return;
	}

	std::vector<std::string> keys;
	if (client.getCmd().size() >= 3)
		keys = ft_split(client.getCmd()[2], ",");
	std::vector<Channel>::iterator c_itr;
	if (channelsToEnter.size() < keys.size())
	{
		sendMessage(formatMessage(client, "Error: too many keys for the number of channels"), client.getFd()); // to check
		return;
	}

	for (size_t i = 0, i2 = 0; i < channelsToEnter.size(); i++)
	{
		if (getChnlByName(channelsToEnter[i]) != _channels.end() && getChnlByName(channelsToEnter[i])->getMode("KEY") == 1 && i2 == keys.size())
		{
			sendMessage(formatMessage(client, "Error: not enough keys for the number of key protected channels"), client.getFd()); // to check
			return;
		}
		if (keys.size() == 0)
			joinChannel(channelsToEnter[i], "", client);
		else
			joinChannel(channelsToEnter[i], keys[i], client);
		if (getChnlByName(channelsToEnter[i])->getMode("KEY"))
			i2++;
	}
}

void Manager::quitCmd(Client &client)
{
	for (size_t i = 0; i < _channels.size(); i++)
		if (_channels[i].isInvited(client.getFd()) || _channels[i].isMember(client.getFd()))
			_channels[i].kickClient(client.getFd());
	sendMessage(formatMessage(client, "Quit: Gone to have lunch"), client.getFd());
}

void Manager::kickCmd(Client &client)
{
	std::vector<std::string> tokens = client.getCmd();
	if (tokens.size() < 4)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
		return;
	}
	std::vector<std::string> channels = ft_split(tokens[0], ",");
	std::vector<std::string> clients = ft_split(tokens[1], ",");
	if (channels.size() != clients.size() && channels.size() != 1)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
		return;
	}
	if (channels.size() == 1)
	{
		if (getChnlByName(tokens[0]) == _channels.end())
		{
			sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + tokens[0] + " :No such channel", client.getFd());
			return;
		}
		if (!getChnlByName(tokens[0])->isOperator(client.getFd()))
		{
			sendMessage(formatMessage(client, CHANOPRIVSNEEDED) + " " + tokens[0] + " :You're not channel operator", client.getFd());
			return;
		}
		for (size_t i = 0; i < clients.size(); i++)
		{
			if (getChnlByName(tokens[0])->isMember(client.getFd()))
				getChnlByName(channels[0])->kickClient(getFdByNick(clients[i]));
		}
		// send message
		return;
	}
	else
	{
		for (size_t i = 0; i < clients.size(); i++)
		{
			getChnlByName(channels[0])->kickClient(getFdByNick(clients[i]));
		}
	}
	getChnlByName(tokens[0])->kickClient(getFdByNick(tokens[1]));
}

void Manager::inviteCmd(Client &client)
{
	if (client.getCmd().size() != 3)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
		return;
	}
	if (getFdByNick(client.getCmd()[1]) == -1)
	{
		sendMessage(formatMessage(client, NOSUCHNICK) + " " + client.getCmd()[1] + " :No such nick/channel", client.getFd());
		return;
	}
	if (getChnlByName(client.getCmd()[2]) == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + client.getCmd()[2] + " :No such channel", client.getFd());
		return;
	}
	if (getChnlByName(client.getCmd()[2])->isMember(getFdByNick(client.getCmd()[1])))
	{
		sendMessage(formatMessage(client, ERR_USERONCHANNEL) + " " + client.getCmd()[1] + " " + client.getCmd()[2] + " :is already on channel", client.getFd());
		return;
	}
	if (getChnlByName(client.getCmd()[2])->getMode("INVITED") && !(getChnlByName(client.getCmd()[2])->isOperator(client.getFd())))
	{
		sendMessage(formatMessage(client, INVITEONLYCHAN) + " " + client.getCmd()[2] + " :Cannot join channel (+i)", client.getFd());
		return;
	}
	getChnlByName(client.getCmd()[2])->Invite(getFdByNick(client.getCmd()[1]));
	sendMessage(formatMessage(client, INVITING) + " " + client.getCmd()[2] + " " + client.getCmd()[1], client.getFd());
}

void Manager::partCmd(Client &client)
{
	if (client.getCmd().size() <= 2)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " " + client.getCmd()[0] + " :Not enough parameters", client.getFd());
		return;
	}
	std::vector<std::string> channels = ft_split(client.getCmd()[1], " ");
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (getChnlByName(channels[i]) == _channels.end())
			sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + channels[i] + " :No such channel", client.getFd());
		else if (!(getChnlByName(channels[i])->isMember(client.getFd())))
		{
			std::clog << "Tested with " + channels[i] << " and is " << !(getChnlByName(channels[i])->isMember(client.getFd())) << std::endl;
			sendMessage(formatMessage(client, USERNOTINCHANNEL) + " " + client.getNickname() + " " + channels[i] + " :They aren't on that channel", client.getFd());
		}
		else
		{
			getChnlByName(channels[i])->kickClient(client.getFd());
			sendMessage(formatMessage(client) + " PART " + channels[i], client.getFd());
			// msg
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
			sendMessage(formatMessage(client, RPL_LIST) + " " + channelIt->getName() + " " + to_string(channelIt->getMembers().size()) + " " + channelIt->getTopic(), client.getFd());
		sendMessage(formatMessage(client, RPL_LISTEND) + " :End of list", client.getFd());
		return;
	}
	std::vector<std::string> chans = ft_split(cmd[1], ",");
	for (std::vector<std::string>::iterator it = chans.begin(); it != chans.end(); it++)
	{
		std::vector<Channel>::iterator channelIt = getChnlByName(*it);
		if (channelIt == _channels.end())
		{
			sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + *it + " :No such channel", client.getFd());
			return;
		}
	}
	sendMessage(formatMessage(client, RPL_LISTSTART) + " :List of Channels", client.getFd());
	for (std::vector<std::string>::iterator it = chans.begin(); it != chans.end(); it++)
	{
		std::vector<Channel>::iterator channelIt = getChnlByName(*it);
		sendMessage(formatMessage(client, RPL_LIST) + " " + to_string(channelIt->getMembers().size()) + " " + channelIt->getTopic(), client.getFd());
	}
	sendMessage(formatMessage(client, RPL_LISTEND) + " :End of list", client.getFd());
}

bool Manager::inVector(std::vector<int> ops, int target)
{
	for (std::vector<int>::iterator it = ops.begin(); it != ops.end(); it++)
		if (*it == target)
			return (true);
	return (false);
}

int Manager::getAllOps()
{
	std::vector<int> ops;
	for (std::vector<Channel>::iterator channelIt = _channels.begin(); channelIt != _channels.end(); channelIt++)
	{
		for (size_t i = 0; i < channelIt->getOperators().size(); i++)
		{
			std::clog << channelIt->getOperators()[i] << std::endl;
			if (!inVector(ops, channelIt->getOperators()[i]))
				ops.push_back(channelIt->getOperators()[i]);
		}
	}
	return (ops.size());
}

void Manager::lusersCmd(Client &client)
{
	sendMessage(formatMessage(client, LUSERCLIENT) + " :There are " + to_string(_clients.size()) + " users on 1 server", client.getFd());
	int ops = getAllOps();
	if (ops > 0)
		sendMessage(formatMessage(client, LUSEROP) + " " + to_string(ops) + " :operator(s) online", client.getFd());
	if (_channels.size() > 0)
		sendMessage(formatMessage(client, LUSERCHANNELS) + " " + to_string(_channels.size()) + " :channels formed", client.getFd());
}

void Manager::nickCmd(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() < 2)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " COMMAND ERROR: Not enought paramaters", client.getFd());
		return;
	}
	if (!isNickValid(cmd[1]))
	{
		sendMessage(formatMessage(client, ERRONEUSNICKNAME) + " :Erroneus nickname", client.getFd());
		return;
	}
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getNickname() == cmd[1])
		{
			sendMessage(formatMessage(client, NICKNAMEINUSE) + " :Nickname is already in use", client.getFd());
			return;
		}
	}
	client.setNickname(cmd[1]);
}

void Manager::msgToChannel(std::string channelName, std::string msg, Client &client)
{
	(void)client;
	for (size_t i = 0; i < getChnlByName(channelName)->getMembers().size(); i++)
	{
		// if (getChnlByName(channelName)->getMembers()[i] != client.getFd())
		sendMessage(formatMessage(*getClientByFd(getChnlByName(channelName)->getMembers()[i])) + " PRIVMSG " + channelName + " " + msg, getChnlByName(channelName)->getMembers()[i]);
	}
}

void Manager::privmsgChannels(std::vector<std::string> const &recipients, std::string msg, Client &client)
{
	for (size_t i = 0; i < recipients.size(); i++)
	{
		if (_channels.empty() || getChnlByName(recipients[i]) == _channels.end())
		{
			sendMessage(formatMessage(client, NOSUCHNICK) + " " + recipients[i] + " :No such nick/channel", client.getFd());
			continue;
		}
		else if (!(getChnlByName(recipients[i])->isMember(client.getFd())))
		{
			sendMessage(formatMessage(client, NOTONCHANNEL) + " " + recipients[i] + " :You're not on that channel", client.getFd());
		}
		else
		{
			msgToChannel(recipients[i], msg, client);
		}
	}
}

void Manager::privmsgClients(std::vector<std::string> const &recipients, std::string msg, Client &client)
{
	for (size_t i = 0; i < recipients.size(); i++)
	{
		if (_clients.empty() || getFdByNick(recipients[i]) == -1)
		{
			sendMessage(formatMessage(client, NOSUCHNICK) + " " + recipients[i] + " :No such nick/channel", client.getFd());
			continue;
		}
		else
		{
			size_t i2;
			for (i2 = 0; i2 < _clients.size(); i2++)
				if (_clients[i2].getFd() == getFdByNick(recipients[i]))
					break;
			if (i2 == _clients.size())
			{
				sendMessage(formatMessage(client, NOSUCHNICK) + " " + recipients[i] + " :No such nick/channel", client.getFd());
				continue;
			}
			else
			{
				sendMessage(formatMessage(_clients[i2]) + "From \"" + client.getNickname() + "\" " + msg, getFdByNick(recipients[i]));
			}
		}
	}
}

void Manager::privmsgCmd(Client &client)
{
	std::clog << "Received -> " << std::endl;
	for (size_t i = 0; i < client.getCmd().size(); i++)
		std::clog << client.getCmd()[i] << std::endl;
	if (client.getCmd().size() < 2)
	{
		sendMessage(formatMessage(client, ERR_NORECIPIENT) + " :No recipient given" + toUP(client.getCmd()[0]), client.getFd());
	}
	if (client.getCmd().size() < 3)
	{
		sendMessage(formatMessage(client, ERR_NOTEXTTOSEND) + " :No text to send", client.getFd());
		return;
	}
	std::vector<std::string> recipients = ft_split(client.getCmd()[1], ",");
	std::string flag = recipients[0].size() > 1 && recipients[0][1] == '#' ? "channel" : "client";
	std::string msg;
	for (size_t i = 2; i < client.getCmd().size(); i++)
	{
		if (i != 2)
			msg += " ";
		msg += client.getCmd()[i];
	}
	for (size_t i = 0; i < recipients.size(); i++)
	{
		if ((flag == "channel" && !(recipients[i].size() > 1 && recipients[i][1] == '#')) ||
				(flag == "client" && !(recipients[i].size() > 0 && recipients[i][1] != '#')))
		{
			sendMessage(formatMessage(client) + " :Wrong parameters", client.getFd());
			return;
		}
	}
	if (flag == "channel")
		privmsgClients(recipients, msg, client);
	else
		privmsgChannels(recipients, msg, client);
}

std::string Manager::checkMode(std::string cmd)
{
	if (cmd.size() != 2 || (cmd[0] != '-' && cmd[0] != '+'))
		return ("");
	if (cmd[1] == 'i')
		return ("INVITE");
	if (cmd[1] == 't')
		return ("TOPIC");
	if (cmd[1] == 'k')
		return ("KEY");
	if (cmd[1] == 'l')
		return ("LIMIT");
	return ("");
}

void Manager::showModes(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	std::vector<Channel>::iterator target = getChnlByName(cmd[1]);
	std::string modes = "";
	if (target->getMode("INVITE"))
		modes += "+i ";
	if (target->getMode("TOPIC"))
		modes += "+t ";
	if (target->getMode("KEY"))
		modes += "+k ";
	if (target->getMode("LIMIT"))
		modes += "+l";
	sendMessage(formatMessage(client, CHANNELMODEIS) + target->getName() + " :" + modes, client.getFd());
}

void Manager::modeCmd(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() < 2)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " MODE :Not enought paramaters", client.getFd());
		return;
	}
	if (cmd.size() == 2 && cmd[1][0] == '#')
		return (showModes(client));
	std::string mode = checkMode(cmd[2]);
	if (cmd.size() == 4 && (cmd[3][0] == '-' || cmd[3][0] == '+') && cmd[3][1] == 'o')
		mode = "OPERATOR";
	if (mode.size() == 0)
	{
		sendMessage(formatMessage(client, UMODEUNKNOWNFLAG) + " " + cmd[2] + " :Unknown mode", client.getFd());
		return ;
	}
	if (cmd.size() < 3)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " MODE :Not enought paramaters", client.getFd());
		return ;
	}

	_modeMap[mode](client);
}

void Manager::inviteMode(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	std::vector<Channel>::iterator target = getChnlByName(cmd[1]);
	if (target == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + client.getCmd()[1] + " :No such channel", client.getFd());
		return;
	}
	if (!target->isMember(client.getFd()))
	{
		sendMessage(formatMessage(client, NOTONCHANNEL) + " " + target->getName() + " :You're not on that channel", client.getFd());
		return;
	}
	if (!target->isOperator(client.getFd()))
	{
		sendMessage(formatMessage(client, CHANOPRIVSNEEDED) + " " + target->getName() + " :You're not channel operator", client.getFd());
		return;
	}
	if (cmd[2][0] == '-')
	{
		target->setMode("INVITE", false);
		target->messageAll(formatMessage(client, CHANNELMODEIS) + " " + target->getName() + " :Channel Invite mode was unset by " + client.getNickname());
	}
	else
	{
		target->setMode("INVITE", true);
		target->messageAll(formatMessage(client, CHANNELMODEIS) + " " + target->getName() + " :Channel Invite mode was set by " + client.getNickname());
	}
}

void Manager::topicMode(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	std::vector<Channel>::iterator target = getChnlByName(cmd[1]);
	if (target == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + client.getCmd()[1] + " :No such channel", client.getFd());
		return;
	}
	if (!target->isMember(client.getFd()))
	{
		sendMessage(formatMessage(client, NOTONCHANNEL) + " " + target->getName() + " :You're not on that channel", client.getFd());
		return;
	}
	if (!target->isOperator(client.getFd()))
	{
		sendMessage(formatMessage(client, CHANOPRIVSNEEDED) + " " + target->getName() + " :You're not channel operator", client.getFd());
		return;
	}
	if (cmd[2][0] == '-')
	{
		target->setMode("TOPIC", false);
		target->messageAll(formatMessage(client, CHANNELMODEIS) + " " + target->getName() + " :Channel Topic mode was unset by " + client.getNickname());
	}
	else
	{
		target->setMode("TOPIC", true);
		target->messageAll(formatMessage(client, CHANNELMODEIS) + " " + target->getName() + " :Channel Topic mode was set by " + client.getNickname());
	}
}

void Manager::keyMode(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() < 4 && cmd[2][0] != '-')
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " MODE :Not enought paramaters", client.getFd());
		return;
	}
	std::vector<Channel>::iterator target = getChnlByName(cmd[1]);
	if (target == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + client.getCmd()[1] + " :No such channel", client.getFd());
		return;
	}
	if (!target->isMember(client.getFd()))
	{
		sendMessage(formatMessage(client, NOTONCHANNEL) + " " + target->getName() + " :You're not on that channel", client.getFd());
		return;
	}
	if (!target->isOperator(client.getFd()))
	{
		sendMessage(formatMessage(client, CHANOPRIVSNEEDED) + " " + target->getName() + " :You're not channel operator", client.getFd());
		return;
	}
	if (cmd[2][0] == '-')
	{
		target->setMode("KEY", false);
		target->messageAll(formatMessage(client, CHANNELMODEIS) + " " + target->getName() + " :Channel key was unset by " + client.getNickname());
	}
	else
	{
		if (target->getMode("KEY"))
		{
			sendMessage(formatMessage(client, ERR_KEYSET) + " " + target->getName() + " :Channel key already set", client.getFd());
			return;
		}
		target->setMode("KEY", true);
		target->setKey(cmd[3]);
		sendMessage(formatMessage(client, CHANNELMODEIS) + " " + target->getName() + " :Channel key was set to " + cmd[3], client.getFd());
		target->messageAll(formatMessage(client, CHANNELMODEIS) + " " + target->getName() + " :Channel key was set by " + client.getNickname());
	}
}

void Manager::limitMode(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() < 4 && cmd[2][0] != '-')
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " MODE :Not enought paramaters", client.getFd());
		return;
	}
	std::vector<Channel>::iterator target = getChnlByName(cmd[1]);
	if (target == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + client.getCmd()[1] + " :No such channel", client.getFd());
		return;
	}
	if (!target->isMember(client.getFd()))
	{
		sendMessage(formatMessage(client, NOTONCHANNEL) + " " + target->getName() + " :You're not on that channel", client.getFd());
		return;
	}
	if (!target->isOperator(client.getFd()))
	{
		sendMessage(formatMessage(client, CHANOPRIVSNEEDED) + " " + target->getName() + " :You're not channel operator", client.getFd());
		return;
	}
	if (cmd[2][0] == '-')
	{
		target->setMode("LIMIT", false);
		target->messageAll(formatMessage(client, CHANNELMODEIS) + " " + target->getName() + " :Channel limit mode was unset by " + client.getNickname());
	}
	else
	{
		target->setMode("LIMIT", true);
		target->setClientLimit(std::atoi(cmd[3].c_str()));
		target->messageAll(formatMessage(client, CHANNELMODEIS) + " " + target->getName() + " :Channel limit mode was set to " + cmd[3] + " by " + client.getNickname());
	}
}

void Manager::operatorMode(Client &client)
{
	std::vector<std::string> cmd = client.getCmd();
	if (cmd.size() < 4)
	{
		sendMessage(formatMessage(client, NEEDMOREPARAMS) + " MODE :Not enought paramaters", client.getFd());
		return ;
	}
	std::vector<Channel>::iterator target = getChnlByName(cmd[1]);
	if (target == _channels.end())
	{
		sendMessage(formatMessage(client, ERR_NOSUCHCHANNEL) + " " + client.getCmd()[1] + " :No such channel", client.getFd());
		return ;
	}
	std::clog << LIGHTPURPLE << getFdByNick(cmd[2])<< RESET << std::endl; 
	if (getFdByNick(cmd[2]) == -1)
	{
		sendMessage(formatMessage(client, NOSUCHNICK) + " " + cmd[2] + " :No such nick/channel", client.getFd());
		return ; 
	}
	if (!target->isMember(client.getFd()))
	{
		sendMessage(formatMessage(client, NOTONCHANNEL) + " " + target->getName() + " :You're not on that channel", client.getFd());
		return ;
	}
	if (!target->isMember(getFdByNick(cmd[2])))
	{
		sendMessage(formatMessage(client, NOTONCHANNEL) + " " + cmd[2] + " :User is not on that channel", client.getFd());
		return ;
	}
	if (!target->isOperator(client.getFd()))
	{
		sendMessage(formatMessage(client, CHANOPRIVSNEEDED) + " " + target->getName() + " :You're not channel operator", client.getFd());
		return ;
	}
	if (cmd[3][0] == '-')
	{
		target->removeOperator(getFdByNick(cmd[2]));
		target->messageAll(formatMessage(client, RPL_UMODEIS) + " " + cmd[2] + " : " + client.getNickname() + "removed " + cmd[2] + " operator status");
	}
	else
	{
		target->addOperator(getFdByNick(cmd[2]));
		target->messageAll(formatMessage(client, RPL_UMODEIS) + " " + cmd[2] + " : " + client.getNickname() + "gave " + cmd[2] + " operator status");
	}
}