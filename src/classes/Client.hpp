/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 18:47:46 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/26 17:11:21 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

class Client
{
private:
  int _fd;
  std::string _username;
  std::string _nickname;
  std::string _password;
  bool _logged;
  bool _passChecked;

public:
  std::string temp;
  std::vector<std::string> cmd;

  Client(int fd);
  ~Client(void);

  void init_client(std::string nickname, std::string username);

  // getters
  int getFd(void) const;
  bool getLogged(void) const;
  bool getPassChecked(void) const;
  std::string getPassword(void) const;
  std::string getUsername(void) const;
  std::string getNickname(void) const;

  // setters
  void setLogged(bool value);
  void setPassChecked(bool value);
  void setNickname(std::string nick);
  void setUsername(std::string username);
  void setPassword(std::string password);
};

#endif