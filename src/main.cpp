/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 19:22:33 by lucas-ma          #+#    #+#             */
/*   Updated: 2024/01/05 02:21:14 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "./classes/Server.hpp"
#include <signal.h>

bool strIsNumber(std::string const &str)
{
	size_t i = 0;

	if (str[0] == '-' || str[0] == '+')
		i++;
	for (; str[i]; i++)
	{
		if (!std::isdigit(str[i]))
			return (false);
	}
	return (true);
}

bool check_errors(char **av)
{
	if (!strIsNumber(av[1]))
	{
		std::cerr << "Port argument is not a number!" << std::endl;
		return (false);
	}

	int port = atoi(av[1]);
	if (port < 0 || port > MAX_PORT)
	{
		// IPPORT_USERRESERVED
		std::cerr << "Port outside of range!" << std::endl;
		std::cerr << "The range is [0 , " << MAX_PORT << "]" << std::endl;
		return (false);
	}

	std::string teste(av[2]);
	if (teste.empty())
	{
		std::cerr << "Empty password!" << std::endl;
		return (false);
	}

	for (size_t i = 0; teste[i]; i++)
	{
		if (std::isspace(teste[i]))
		{
			std::cerr << "The password must not contain spaces!" << std::endl;
			return (false);
		}
	}

	return (true);
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Wrong arguments number!" << std::endl;
		std::cerr << "Right usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	// signal(SIGINT, signalHandler);
	// signal(SIGQUIT, signalHandler);
	// signal(SIGTERM, signalHandler);

	if (!check_errors(av))
		return (1);
	Server server(av[1], av[2]);

	Manager::fillMaps();
	server.start();
	return (0);
}