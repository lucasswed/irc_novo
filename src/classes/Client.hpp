/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 18:47:46 by lucas-ma          #+#    #+#             */
/*   Updated: 2024/01/04 16:49:34 by lucas-ma         ###   ########.fr       */
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
	std::vector<std::string> _cmd;
	bool _registered;
	bool _logged;
	bool _passChecked;

public:
	std::string temp;

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
	std::vector<std::string> getCmd() const;
	bool getRegistered(void) const;

	// setters
	void setLogged(bool value);
	void setPassChecked(bool value);
	void setNickname(std::string nick);
	void setUsername(std::string username);
	void setPassword(std::string password);
	void setCmd(std::vector<std::string> cmd);
	void setRegistered(bool value);
};

#endif