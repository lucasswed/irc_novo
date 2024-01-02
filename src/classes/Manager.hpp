/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/01/02 12:12:45 by ralves-g         ###   ########.fr       */
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

typedef void  (*cmdFunction)(std::string& msg, Client& client);

class Manager
{
private:
  static std::vector<Client> _clients;
  static std::vector<Channel> _channels;
  static std::string _hostname;
  static std::string _servername;
  static std::map<std::string, cmdFunction> cmdMap;

	static void on(std::string event, cmdFunction function);

public:
	static bool addClient(int fd);
	static void removeClient(int fd);
	static std::vector<Client>::iterator getClientByFd(int fd);
	static bool checkClientData(Client &client);
	static bool checkPassword(Client const &client, std::string const &password);
	static bool checkNick(Client const &client);
	static void sendMessage(std::string message, int clientFd);
	static std::string formatMessage(Client const &client, std::string const &code);
	static void fillCmdMap(void);

  void createChannel(std::string name);
  void joinChannel(std::string channel, std::string key, Client& client);
  std::vector<Channel>::iterator getChnlByName(std::string name);
  int	getFdByNick(std::string nickname);
  std::string getNickByFd(int fd);
  void joinCmd(std::string msg, Client& client);
  void quitCmd(std::string msg, Client& client);
  void kickCmd(std::string msg, Client& client);
	static std::map<std::string, cmdFunction> getCmdMap(void);
};

#endif