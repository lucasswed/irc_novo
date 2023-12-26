/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralves-g <ralves-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:52:15 by ralves-g          #+#    #+#             */
/*   Updated: 2023/12/20 17:58:11 by ralves-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <vector>
# include <string>

class Client;

class Channel {
	private:
		Channel();
		Channel(Channel const& ref);
		Channel const& operator=(Channel const& ref);
		std::string			_name;
		std::vector<Client>	_members;
		std::vector<Client>	_operators;
	public:
		Channel(std::string name);
		~Channel();
		std::vector<Client> getMembers() const;
		std::vector<Client> getOperators() const;
		int	addMember(Client newClient);
		int	addOperator(Client newOP);
		void	rmMember(Client member);
		int	isOperator(Client check);

};

#endif