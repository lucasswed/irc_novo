/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 19:46:22 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/11/21 16:59:57 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define PENDING 10
# define FD_MAX 10
# define MAX_READ 3500

# include <string>
# include <netdb.h>
# include <sys/socket.h>
# include <netinet/in.h>

class Server
{
	private:
		int	_port;
		std::string _pass;
		addrinfo	*setServerInfo();
	public:
		Server(int const& port, std::string const& password);
		~Server();
};

#endif