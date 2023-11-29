/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 16:39:01 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/11/21 17:00:31 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

struct addrinfo	*Server::setServerInfo(void) {
	
}

Server::Server(int const& port, std::string const& password) : _port(port), _pass(password) {
	struct addrinfo *server;

	server = setServerInfo();
}