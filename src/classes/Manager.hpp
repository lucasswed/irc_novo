/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 19:56:10 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/12 23:09:44 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "../colors.hpp"
#include "Client.hpp"

#include <vector>
#include <iostream>

#include <unistd.h>

class Manager
{
private:
  static std::vector<Client> _clients;
public:
  static bool addClient(int fd);
  static void removeClient(int fd);
  static std::vector<Client>::iterator getClientByFd(int fd);
};

#endif