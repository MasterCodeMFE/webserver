/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manufern <manufern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:12:21 by manufern          #+#    #+#             */
/*   Updated: 2025/03/06 19:12:27 by manufern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void ){}

Request::Request( Request const &src ){ (void)src; }

Request::~Request( void ){}

Request &Request::operator=( Request const &src ){ (void)src; return ( *this );}


