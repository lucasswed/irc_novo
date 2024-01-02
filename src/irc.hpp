/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 19:08:52 by lucas-ma          #+#    #+#             */
/*   Updated: 2024/01/02 14:50:16 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#define MAX_PORT 65535

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

std::vector<std::string> ft_split(std::string const &str, std::string const &del);
bool isNickValid(std::string const &nick);
std::string toUP(std::string str);

#endif