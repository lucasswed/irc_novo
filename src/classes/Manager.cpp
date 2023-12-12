/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:13:17 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/12 23:11:54 by lucas-ma         ###   ########.fr       */
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
