/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/01/04 15:42:47 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "Client.hpp"
#include "../irc.hpp"
#include "../colors.hpp"
#include "../messageCodes.hpp"

#include <map>
#include <vector>
#include <cstdlib>
#include <iostream>

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>

typedef void (*cmdFunction)(Client &client);
// typedef void (*modeFunction)(Channel &channel, Client &client);

class Channel;

class Manager
{
private:
	static std::vector<Client> _clients;
	static std::vector<Channel> _channels;
	static std::string _hostname;
	static std::string _servername;
	static std::map<std::string, cmdFunction> _cmdMap;
	// static std::map<std::string, modeFunction> _modeMap;

	static void on(std::string event, cmdFunction function);
	// static void on(std::string event, modeFunction function);

public:
	static bool addClient(int fd);
	static void removeClient(int fd);
	static std::vector<Client>::iterator getClientByFd(int fd);
	static bool checkClientData(Client &client);
	static bool checkPassword(Client const &client, std::string const &password);
	static bool checkNick(Client const &client);
	static void sendMessage(std::string message, int clientFd);
	static std::string formatMessage(const Client &client);
	static std::string formatMessage(Client const &client, std::string const &code);
	static void fillMaps(void);

	static void createChannel(std::string name);
	static void joinChannel(std::string channel, std::string key, Client client);
	static std::vector<Channel>::iterator getChnlByName(std::string name);
	static int getFdByNick(std::string nickname);
	static std::string getNickByFd(int fd);
	static std::map<std::string, cmdFunction> getCmdMap(void);

	// Cmd Functions
	static void joinCmd(Client &client);
	static void quitCmd(Client &client);
	static void kickCmd(Client &client);
	static void partCmd(Client &client);
	static void modeCmd(Client &client);
	static void topicCmd(Client &client);
	static void inviteCmd(Client &client);
	static void privmsgCmd(Client &client);
	static void listCmd(Client &client);
	static void lusersCmd(Client &client);
	static void nickCmd(Client &client);

	static void whoCmd(Client &client);
};

#endif