/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 19:08:52 by lucas-ma          #+#    #+#             */
/*   Updated: 2023/12/13 10:40:45 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#define MAX_PORT 65535

#include <cstdlib>
#include <string>
#include <iostream>

std::vector<std::string> ft_split(std::string const &str, std::string const &del);
bool isNickValid(std::string const &nick);

#endif