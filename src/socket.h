/*
 * Copyright (C) 2013  Daniel Carrera <dcarrera@miten.es>
 *
 * This file is part of rtstream.
 * 
 * rtstream is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rtstream is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rtstream.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <unistd.h>// for close

#include <arpa/inet.h> //inet_pton

enum socket_mode_list {
	SOCK_TCP_SERVER,
    SOCK_TCP_CLIENT,
    SOCK_UDP_TRANSMIT,
    SOCK_UDP_RECEIVE,
};

struct socket {
    int   mode;
    char *ip;
    int   port;
};


void socket4_init(struct socket *so);
char *socket4_accept_open();
void socket4_accept_close();
//UDP
int  socket4_sendto_list_add(char *ip, int port);
int  socket4_sendto_list_remove(char *ip);
void socket4_sendto_list(char *data, int len);
//TCP
void socket4_send(char *data);
int  socket4_recv(char *data, int len);
void socket4_session_send(char *data);
int  socket4_session_recv(char *data, int len);

/* For IPV6 */
void  socket_init(struct socket *so);
char *socket_accept_open();
void  socket_accept_close();
//UDP
int   socket_sendto_list_add(char *ip, int port);
int   socket_sendto_list_remove(char *ip);
void  socket_sendto_list(char *data, int len);
//TCP
void  socket_send(char *data);
int   socket_recv(char *data, int len);
void  socket_session_send(char *data);
int   socket_session_recv(char *data, int len);

#endif // SOCKET_H_INCLUDED
