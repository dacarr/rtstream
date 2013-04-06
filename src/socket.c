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
#include "socket.h"

const int max_clients = 10;
int sockfd, sockfdac;
int send_len;
int recv_len;

//struct ip_mreq r_mreq; //for multicast

// structs to append multiple user connections
struct sockaddr_in6 *clients6;
struct sockaddr_in *clients4;



int socket_ip_ismulticast(char *address) {

    if (strncmp(address,"FF",2) == 0 || strncmp(address,"fF",2) == 0 ||
        strncmp(address,"Ff",2) == 0 || strncmp(address,"ff",2) == 0) return 1;
    else if (strncmp(address,"224",3) == 0) return 1;
    else if (strncmp(address,"225",3) == 0) return 1;
    else if (strncmp(address,"232",3) == 0) return 1;
    else if (strncmp(address,"234",3) == 0) return 1;
    else if (strncmp(address,"235",3) == 0) return 1;
    else if (strncmp(address,"239",3) == 0) return 1;

    return 0;
}


void socket4_init(struct socket *so) {

    if(so->mode == SOCK_TCP_SERVER) {
		
        struct sockaddr_in ip4addr;
        ip4addr.sin_family = AF_INET;
        ip4addr.sin_port = htons(so->port);

		if(so->ip != 0) {
			ip4addr.sin_addr.s_addr = inet_addr(so->ip);
		}
		else {
    		ip4addr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
        //inet_pton(AF_INET, so.ip, &ip4addr.sin_addr);

        sockfd = socket(PF_INET, SOCK_STREAM, 0);

        //int reuseaddr = 1;
        //setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
        bind(sockfd, (struct sockaddr*) &ip4addr, sizeof ip4addr);
        listen(sockfd, SOMAXCONN);
    }

    else if(so->mode == SOCK_UDP_TRANSMIT) {
        clients4 = malloc(max_clients * sizeof(*clients4));
        sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

	else if(so->mode == SOCK_UDP_RECEIVE) {

		struct sockaddr_in ip4addr;
        ip4addr.sin_family = AF_INET;
        ip4addr.sin_port = htons(so->port);
        ip4addr.sin_addr.s_addr = htonl(INADDR_ANY);

        sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

        bind(sockfd, (struct sockaddr*) &ip4addr, sizeof ip4addr);

		/* for MULTICAST *//*
		if (host != NULL){
		    // use setsockopt() to request that the kernel join a multicast group
		    r_mreq.imr_multiaddr.s_addr = inet_addr(host);
		    r_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &r_mreq, sizeof(r_mreq)) < 0) {
		        perror("setsockopt");
		        exit(1);
		    }
		}*/
    }
}


int socket4_sendto_list_add(char *ip, int port) {

    int i;
    for(i=0; i<=max_clients; i++)
        if(clients4[i].sin_port == 0) break;

    if(i == max_clients) return -1;

	clients4[i].sin_family = AF_INET;
	clients4[i].sin_port = htons(port);
	
	if(inet_pton(AF_INET, ip, &clients4[i].sin_addr.s_addr) != 1) {
		fprintf(stderr, "Error parsing ipv4 address\n");
		exit(3);
	}

	return 0;
}


int socket4_sendto_list_remove(char *ip) {
	// TODO multiple users on off
	/*
    int i;
    for(i=0; i<=max_clients; i++)
        if(strcmp(clients4[i].sin_addr.s_addr, ip) == 0){
            clients4[i].sin_addr.s_addr = 0;
            clients4[i].sin_port = 0;
            break;
        }
*/
    return 0;
}


void socket4_sendto_list(char *data, int len) {

    int i;
    for(i=0; i<=max_clients; i++) {

        if(clients4[i].sin_port == 0) break;
		
        sendto(sockfd, data, len, 0, (struct sockaddr*) &clients4[i], sizeof clients4[i]);
    }
	
}

void socket4_send(char *data) {
    send_len = send(sockfd, data, strlen(data), 0);
}

int socket4_recv(char *data, int len) {
    recv_len = recv(sockfd, data, len, 0);
    return recv_len; //len of received data
}

char *socket4_accept_open() {

    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    //TODO: modify to ACL
    if ((sockfdac = accept(sockfd, (struct sockaddr *) &client, &len)) == -1) {
         printf("Socket: error in accept()\n");
         exit(-1);
      }

      return inet_ntoa(client.sin_addr);
}


void socket4_session_send(char *data) {
    send_len = send(sockfdac, data, strlen(data), 0);
}


int socket4_session_recv(char *data, int len) {
    recv_len = recv(sockfdac, data, len, 0);
    return recv_len; //len of received data
}


void socket4_accept_close() {
    close(sockfdac);
}


/*********************************************************
 * IPV6 sockets
 *********************************************************/
void socket_init(struct socket *so) {

    struct sockaddr_in6 ipaddr;
	
    if(so->mode == SOCK_TCP_SERVER) {
		
        ipaddr.sin6_family = AF_INET6;
        ipaddr.sin6_port = htons(so->port);
		ipaddr.sin6_addr = in6addr_any;

        sockfd = socket(AF_INET6, SOCK_STREAM, 0);
        bind(sockfd, (struct sockaddr*) &ipaddr, sizeof ipaddr);
        listen(sockfd, SOMAXCONN);
    }

    else if(so->mode == SOCK_UDP_TRANSMIT) {
		
        clients6 = malloc(max_clients * sizeof(*clients6));
        sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    }

	else if(so->mode == SOCK_UDP_RECEIVE) {
		
        ipaddr.sin6_family = AF_INET6;
        ipaddr.sin6_port = htons(so->port);
        ipaddr.sin6_addr = in6addr_any;

        sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

        bind(sockfd, (struct sockaddr*) &ipaddr, sizeof ipaddr);

		/* for MULTICAST *//*
		if (host != NULL){
		    // use setsockopt() to request that the kernel join a multicast group
		    r_mreq.imr_multiaddr.s_addr = inet_addr(host);
		    r_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &r_mreq, sizeof(r_mreq)) < 0) {
		        perror("setsockopt");
		        exit(1);
		    }
		}*/
    }
}


int socket_sendto_list_add(char *ip, int port) {
	
    int i;
    for(i=0; i<=max_clients; i++)
        if(clients6[i].sin6_port == 0) break;

    if(i == max_clients) return -1;

	clients6[i].sin6_family = AF_INET6;
	clients6[i].sin6_port = htons(port);
	
	if(inet_pton(AF_INET6, ip, &clients6[i].sin6_addr.s6_addr) != 1) {
		fprintf(stderr, "Error parsing ipv6 address\n");
		exit(3);
	}
	
    return 0;
}


int socket_sendto_list_remove(char *ip) {
	// TODO multiple users on off
	/*
    int i;
    for(i=0; i<=max_clients; i++)
        if(strcmp(clients[i].ip, ip) == 0){
            clients[i].ip = 0;
            clients[i].port = 0;
            break;
        }
*/
    return 0;
}


void socket_sendto_list(char *data, int len) {
	
    int i;
    for(i=0; i<=max_clients; i++) {
        if(clients6[i].sin6_port == 0) break;
		
        sendto(sockfd, data, len, 0, (struct sockaddr*) &clients6[i], sizeof clients6[i]);
    }
}

void socket_send(char *data) {
    send_len = send(sockfd, data, strlen(data), 0);
}

int socket_recv(char *data, int len) {
    recv_len = recv(sockfd, data, len, 0);
    return recv_len; //len of received data
}

char *socket_accept_open() {

    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    //TODO: modify to ACL
    if ((sockfdac = accept(sockfd, (struct sockaddr *) &client, &len)) == -1) {
         printf("Socket: error in accept()\n");
         exit(-1);
    }

	return inet_ntoa(client.sin_addr);
}


void socket_session_send(char *data) {
    send_len = send(sockfdac, data, strlen(data), 0);
}


int socket_session_recv(char *data, int len) {
    recv_len = recv(sockfdac, data, len, 0);
    return recv_len; //len of received data
}


void socket_accept_close() {
    close(sockfdac);
}
