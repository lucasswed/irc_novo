/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/01/03 11:03:54 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "../irc.hpp"
#include "../colors.hpp"
#include "../messageCodes.hpp"

#include <map>
#include <vector>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>

typedef void (*cmdFunction)(Client &client);
typedef void (*modeFunction)(Channel &channel, Client &client);

class Manager
{
private:
	static std::vector<Client> _clients;
	static std::vector<Channel> _channels;
	static std::string _hostname;
	static std::string _servername;
	static std::map<std::string, cmdFunction> _cmdMap;
	static std::map<std::string, modeFunction> _modeMap;

	static void on(std::string event, cmdFunction function);
	static void on(std::string event, modeFunction function);

public:
	static bool addClient(int fd);
	static void removeClient(int fd);
	static std::vector<Client>::iterator getClientByFd(int fd);
	static bool checkClientData(Client &client);
	static bool checkPassword(Client const &client, std::string const &password);
	static bool checkNick(Client const &client);
	static void sendMessage(std::string message, int clientFd);
	static std::string formatMessage(Client const &client, std::string const &code);
	static void fillMaps(void);

	void createChannel(std::string name);
	void joinChannel(std::string channel, std::string key, Client client);
	std::vector<Channel>::iterator getChnlByName(std::string name);
	int getFdByNick(std::string nickname);
	std::string getNickByFd(int fd);
	static std::map<std::string, cmdFunction> getCmdMap(void);

	//Cmd Functions
	void joinCmd(Client &client);
	void quitCmd(Client &client);
	void kickCmd(Client &client);
	void partCmd(Client &client);
	void modeCmd(Client &client);
	void topicCmd(Client &client);
	void inviteCmd(Client &client);
	void privmsgCmd(Client &client);
	void listCmd(Client &client);
	void whoCmd(Client &client);
	void lusersCmd(Client &client);
	void nickCmd(Client &client);
};

#endif