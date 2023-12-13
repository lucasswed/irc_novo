/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 16:39:01 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/13 09:55:48 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::checkPassword(Client& client) {
	if (client.getLogged())
		return (true);
	std::cout << BLUE << "Running password check..." << RESET << std::endl;
	// check pass
	if (!Manager::checkPassword(client, this->_pass))
		return (false);
	if (client.getNickname().empty() || client.getUsername().empty()) {
		std::cout << BLUE << "Waiting for username and nickname..." << RESET << std::endl;
		return (false);
	}
	// if ()
	// verify username and nick
	client.setLogged(true);
	std::cout << GREEN << "Client correctly connected" << std::endl;
	return (true);
}

void Server::messageHandler(int socket, int read, char *buffer)
{
	buffer[read] = 0;
	std::string msg(buffer);
	if (FD_ISSET(socket, &this->_masterFd))
	{
		std::vector<Client>::iterator iter = Manager::getClientByFd(socket);
		Client& client = *iter;
		if (msg.find('\n') == std::string::npos) {
			client.temp += msg;
			return ;
		}
		client.temp += msg;
		client.cmd = ft_split(client.temp, "\r\n\t ");
		// if (!Manager::checkClientData(client))
			// this->checkPassword(client);
		std::cout << GREEN << "Message received: " << client.temp << WHITE << std::endl;
		client.temp.clear();
	}
}

void Server::acceptConnection(void)
{
	int newSocket;
	struct sockaddr_in clientAddress;
	socklen_t clientSize = sizeof(clientAddress);

	if ((newSocket = accept(this->_serverFd, (struct sockaddr *)&clientAddress, &clientSize)) == -1)
	{
		std::cerr << "Error accepting the client!" << std::endl;
		std::exit(1);
	}
	FD_SET(newSocket, &this->_masterFd);
	if (newSocket > this->_serverFd)
		this->_maxFd = newSocket + 1;
	Manager::addClient(newSocket);
}

void Server::checkActivity(fd_set readFd)
{
	int readBytes;
	char buffer[MAX_READ + 1];

	for (int i = 0; i <= this->_maxFd; i++)
	{
		bzero(buffer, MAX_READ + 1);
		if (FD_ISSET(i, &readFd))
		{
			if (i == this->_serverFd)
				this->acceptConnection();
			else
			{
				if ((readBytes = recv(i, buffer, sizeof(buffer), 0)) <= 0)
				{
					Manager::removeClient(i);
					FD_CLR(i, &this->_masterFd);
				}
				else
					this->messageHandler(i, readBytes, buffer);
			}
		}
	}
}

void Server::connectionHandler(void)
{
	fd_set readFd; // Temporary set of sockets used to check for activity
	this->_maxFd = this->_serverFd;

	FD_ZERO(&this->_masterFd);
	FD_SET(this->_serverFd, &this->_masterFd);
	while (true)
	{
		readFd = this->_masterFd; // Copy the master socket to the temporary set thats going to read the activity
		// wait until either socket has data ready to be recv()
		if (select(this->_maxFd + 1, &readFd, NULL, NULL, NULL) == -1)
		{
			std::cerr << "Select function failed!" << std::endl;
			std::exit(1);
		}
		this->checkActivity(readFd);
	}
}

void Server::start(void)
{
	if (this->_serverFd == -1)
	{
		std::cerr << "Socket not inicialized!" << std::endl;
		std::exit(1);
	}
	if (listen(this->_serverFd, PENDING) == -1)
	{
		std::cerr << "Listen function failed!" << std::endl;
		std::exit(1);
	}
	std::cout << "Server listening on port " << this->_port << std::endl;
	this->connectionHandler();
}

struct addrinfo *Server::setServerInfo(void)
{
	int status = 0;
	struct addrinfo temp;
	struct addrinfo *ret;

	bzero(&temp, sizeof(temp));
	temp.ai_family = AF_UNSPEC;			// Compatible with IP4 and IP6
	temp.ai_socktype = SOCK_STREAM; // Receive and send type of connection
	temp.ai_flags = AI_PASSIVE;			// Automatically fills in ip
	if ((status = getaddrinfo(NULL, this->_port.c_str(), &temp, &ret)) != 0)
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
	return (ret);
}

int Server::bindSocket(struct addrinfo *server)
{
	int need = 1;

	for (struct addrinfo *temp = server; temp; temp = temp->ai_next)
	{
		int socket_fd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
		if (socket_fd == -1 || temp->ai_family != AF_INET6)
			continue;
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &need, sizeof(need)) == -1)
			close(socket_fd);
		if (bind(socket_fd, temp->ai_addr, temp->ai_addrlen) == -1)
			close(socket_fd);
		else
			return (socket_fd);
	}
	return (-1);
}

Server::Server(std::string const &port, std::string const &password) : _port(port), _pass(password)
{
	struct addrinfo *server;

	server = setServerInfo();
	_serverFd = this->bindSocket(server);
	freeaddrinfo(server);
}

Server::~Server(void) {}