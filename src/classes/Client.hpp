/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 18:47:46 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/12 23:07:16 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
private:
  int _fd;
  std::string _username;
  std::string _nickname;

public:
  Client(int fd);
  ~Client(void);

  void init_client(std::string nickname, std::string username);
  int getFd(void) const;
};

#endif