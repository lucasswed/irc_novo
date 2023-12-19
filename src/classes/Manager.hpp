/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 19:56:10 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/19 13:38:08 by lucas-ma         ###   ########.fr       */
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
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>

typedef void  (*cmdFunction)(Client& client);

class Manager
{
private:
  static std::vector<Client> _clients;
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
  static std::string formatMessage(Client const &client, std::string const& code);
  static void fillCmdMap(void);
};

#endif