/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/01/05 05:02:32 by lucas-ma         ###   ########.fr       */
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
typedef void (*modeFunction)(Client &client);

class Channel;

class Manager
{
private:
	static std::vector<Client> _clients;
	static std::vector<Channel> _channels;
	static std::string _hostname;
	static std::string _servername;
	static std::map<std::string, cmdFunction> _cmdMap;
	static std::map<std::string, modeFunction> _modeMap;

	static void onCmd(std::string event, cmdFunction function);
	static void onMode(std::string event, modeFunction function);

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
	static void setChannOpps(Client &client);

	static bool isValidChannel(std::string const &channelName, Client &client);
	static void sendWhoMessage(std::vector<int> list, Client &send, std::string channelName);

	static void createChannel(std::string name);
	static void joinChannel(std::string channel, std::string key, Client client);
	static std::vector<Channel>::iterator getChnlByName(std::string name);
	static int getFdByNick(std::string nickname);
	static std::string getNickByFd(int fd);
	static std::map<std::string, cmdFunction> getCmdMap(void);
	static void privmsgClients(std::vector<std::string> const &recipients, std::string msg, Client &client);
	static void privmsgChannels(std::vector<std::string> const &recipients, std::string msg, Client &client);
	static void msgToChannel(std::string channelName, std::string msg, Client &client);
	static std::string checkMode(std::string cmd);

	// Cmd Functions
	static void joinCmd(Client &client);		// Falta ver as passes e com modes
	static void quitCmd(Client &client);		// Ta bom
	static void kickCmd(Client &client);		// To Do
	static void partCmd(Client &client);		// To Test
	static void modeCmd(Client &client);		// A ser feito
	static void topicCmd(Client &client);		// Falta ver com modes ativos
	static void inviteCmd(Client &client);	// Quem convida ta bem, o convite nao chega ao outro user e precisa de modes para testar
	static void privmsgCmd(Client &client); // To Test
	static void listCmd(Client &client);		// Ta bom
	static void lusersCmd(Client &client);	// Ta bom
	static void nickCmd(Client &client);		// Ta bom
	static void whoCmd(Client &client);			// Nem existe ainda

	// Mode Functions
	static void inviteMode(Client &client);
	static void topicMode(Client &client);
	static void keyMode(Client &client);
	static void operatorMode(Client &client);
	static void limitMode(Client &client);
};

#endif