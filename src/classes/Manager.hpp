/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 19:56:10 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/16 10:56:44 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "Client.hpp"
#include "../irc.hpp"
#include "../colors.hpp"
#include "../messageCodes.hpp"

#include <vector>
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>

class Manager
{
private:
  static std::vector<Client> _clients;
  static std::string _hostname;
  static std::string _servername;

public:
  static bool addClient(int fd);
  static void removeClient(int fd);
  static std::vector<Client>::iterator getClientByFd(int fd);
  static bool checkClientData(Client &client);
  static bool checkPassword(Client const &client, std::string const &password);
  static bool checkNick(Client const &client);
  static void sendMessage(std::string message, int clientFd);
  static std::string formatMessage(Client const &client, std::string const& code);
};

#endif