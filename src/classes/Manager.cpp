/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:13:17 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/19 13:43:00 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Manager.hpp"

std::vector<Client> Manager::_clients;
std::string Manager::_hostname = "localhost";
std::string Manager::_servername = "irc.server.com";

void  Manager::on(std::string event, cmdFunction function) {
  cmdMap.insert(std::pair<std::string, cmdFunction>(event, function));
}

void  Manager::fillCmdMap(void) {
  // on("JOIN", &);
  // on("QUIT", &);
  // on("KICK", &);
  // on("PART", &);
  // on("MODE", &);
  // on("TOPIC", &);
  // on("INVITE", &);
  // on("PRIVMSG", &);
  // on("LIST", &);
  // on("NAMES", &);
  // on("WHO", &);
  // on("LUSERS", &);
  // on("NICK", &);
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
  for (size_t i = 0; i < client.cmd.size(); i++)
  {
    if (client.cmd[i] == "PASS")
      client.setPassword(client.cmd[i + 1]);
    else if (client.cmd[i] == "NICK")
      client.setNickname(client.cmd[i + 1]);
    else if (client.cmd[i] == "USER")
      client.setUsername(client.cmd[i + 1]);
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