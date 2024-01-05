/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas-ma <lucas-ma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 23:44:14 by lucas-ma          #+#    #+#             */
/*   Updated: 2024/01/05 02:27:09 by lucas-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

std::vector<std::string> ft_split(std::string const &str, std::string const &del)
{
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = str.find_first_of(del);

	while (end != std::string::npos)
	{
		std::string token = str.substr(start, end - start);
		if (!token.empty())
			tokens.push_back(token);
		start = end + 1;
		end = str.find_first_of(del, start);
	}

	std::string last = str.substr(start);
	if (!last.empty())
		tokens.push_back(last);
	return (tokens);
}

bool isNickValid(std::string const &nick)
{
	return (nick.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_") == std::string::npos);
}

std::string toUP(std::string str)
{
	std::string ret = str;
	std::transform(ret.begin(), ret.end(), ret.begin(), toupper);
	return (ret);
}

std::string to_string(size_t const &size)
{
	std::stringstream ss;
	ss << size;
	return (ss.str());
}

// void signalHandler(int sig)
// {
// 	(void)sig;
// 	std::cout << "signal received" << std::endl;
// 	exit(0);
// }