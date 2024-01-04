/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messageCodes.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcampos- <pcampos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 10:27:02 by lucas-ma          #+#    #+#             */
/*   Updated: 2024/01/04 21:24:07 by pcampos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGECODES_HPP
#define MESSAGECODES_HPP

#define WELCOME_MESSAGE "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define CHANNEL_OPPS "005"
#define LUSERCLIENT "251"
#define LUSEROP "252"
#define LUSERUNKNOWN "253"
#define LUSERCHANNELS "254"
#define RPL_LISTSTART "321"
#define RPL_LIST "322"
#define RPL_LISTEND "323"
#define CHANNELMODEIS "324"
#define RPL_NOTOPIC "331"
#define RPL_TOPIC "332"
#define INVITING "341"
#define RPL_WHOREPLY "352"
#define NAMREPLY "353"
#define ENDOFNAMES "366"
#define ENDOFBANLIST "368"
#define NOSUCHNICK "401"
#define NOSUCHSERVER "402"
#define ERR_NOSUCHCHANNEL "403"
#define CANNOTSENDTOCHAN "404"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define UNKNOWNCOMMAND "421"
#define NONICKNAMEGIVEN "431"
#define ERRONEUSNICKNAME "432"
#define NICKNAMEINUSE "433"
#define USERNOTINCHANNEL "441"
#define NOTONCHANNEL "442"
#define ERR_USERONCHANNEL "443"
#define NEEDMOREPARAMS "461"
#define PASSWDMISMATCH "464"
#define CHANNELISFULL "471"
#define INVITEONLYCHAN "473"
#define BANNEDFROMCHAN "474"
#define BADCHANNELKEY "475"
#define BADCHANNELNAME "476"
#define NOPRIVILEGES "481"
#define CHANOPRIVSNEEDED "482"
#define UMODEUNKNOWNFLAG "501"
#define RPL_ENDOFWHO "315"
#define ERR_KEYSET "467"

#endif