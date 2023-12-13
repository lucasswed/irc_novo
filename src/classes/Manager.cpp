/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:13:17 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/13 10:57:17 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Manager.hpp"

std::vector<Client>   Manager::_clients;

bool Manager::addClient(int fd) {
  if (Manager::getClientByFd(fd) != _clients.end())
    return true;
  _clients.push_back(Client(fd));
  std::cout << GREEN << "Client added succesfully" << RESET << std::endl;
  return false;
}

void Manager::removeClient(int fd) {
  for (size_t i = 0; i < _clients.size(); i++) {
    if (_clients[i].getFd() == fd) {
      // remove the client from all channels
      _clients.erase(_clients.begin() + i);
      close(fd);
      std::cout << RED << "Client removed succesfully" << RESET << std::endl;
      return;
    }
  }
}

std::vector<Client>::iterator Manager::getClientByFd(int fd) {
  std::vector<Client>::iterator client = _clients.begin();
  for (; client != _clients.end(); client++)
    if (client->getFd() == fd)
      break;
  return client;
}

bool Manager::checkClientData(Client& client) {
  if (client.getLogged())
    return (true);
  for (size_t i = 0; i <  client.cmd.size(); i++) {
    if (client.cmd[i] == "PASS")
      client.setPassword(client.cmd[i + 1]);
    else if (client.cmd[i] == "NICK")
      client.setNickname(client.cmd[i + 1]);
    else if (client.cmd[i] == "USER")
      client.setUsername(client.cmd[i + 1]);
  }
  return (false);
}

bool Manager::checkPassword(Client const& client, std::string const& password) {
  if (client.getPassword() != password || client.getPassword().empty()) {
    std::cout << RED << "Wrong password or empty" << RESET << std::endl;
    // sendMessage() to the client with the error
    return (false);
  }
  return (true);
}

bool Manager::checkNick(Client const& client) {
  if (!isNickValid(client.getNickname())) {
    // sendMessage() to the client with the error nick not valid
    return (false);
  }
  for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
    if (it->getFd() == client.getFd())
      continue ;
    if (it->getNickname() == client.getNickname()) {
      // sendMessage() to the client with the nick in use error
      return (false);
    }
  }
  std::cout << GREEN << "Nickname checked" << RESET << std::endl;
  return (true);
}
