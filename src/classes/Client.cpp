/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:24:38 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/13 09:53:14 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _logged(false)
{
}

Client::~Client() {}

void Client::init_client(std::string nickname, std::string username)
{
  this->_nickname = nickname;
  this->_username = username;
}

int Client::getFd(void) const
{
  return this->_fd;
}

bool Client::getLogged(void) const
{
  return (this->_logged);
}

std::string Client::getPassword(void) const
{
  return (this->_password);
}

std::string Client::getUsername(void) const {
  return (this->_username);
}

std::string Client::getNickname(void) const {
  return (this->_nickname);
}

void Client::setLogged(bool value)
{
  this->_logged = value;
}

void Client::setNickname(std::string nick)
{
  this->_nickname = nick;
}

void Client::setUsername(std::string username)
{
  this->_username = username;
}

void Client::setPassword(std::string password)
{
  this->_password = password;
}