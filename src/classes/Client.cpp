/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 19:41:52 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/12 19:44:48 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int id) : _id(id) {}

Client::~Client(void) {}

void Client::init_client(std::string nickname, std::string username) {
  this->_nickname = nickname;
  this->_username = username;
}