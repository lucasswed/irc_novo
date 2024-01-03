/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 19:46:22 by lucas-ma          #+#    #+#             */
/*   Updated: 2024/01/03 11:00:41 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define PENDING 10
#define FD_MAX 10
#define MAX_READ 3500

#include "../colors.hpp"
#include "Manager.hpp"
#include "../irc.hpp"

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
private:
	int _serverFd;
	int _maxFd;
	fd_set _masterFd; //  variable is used to store the set of socket file descriptors for which you want to check readability.
	std::string _port;
	std::string _pass;
	addrinfo *setServerInfo();
	int bindSocket(addrinfo *server);
	void connectionHandler(void);
	void acceptConnection(void);
	void checkActivity(fd_set readFd);
	void messageHandler(int socket, int read, char *buffer);
	bool signUpClient(Client &client);
	void runCmd(Client &client);

public:
	Server(std::string const &port, std::string const &password);
	~Server();

	void start(void);
};

#endif