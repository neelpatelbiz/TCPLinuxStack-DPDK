#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define PORTSTRLEN 5
//#define SERVER_PORT "4545"
int main(int argc, char **argv)
{
	char *host = NULL, *host_port = "5201";
	/*At least assign port in case node is left NULL*/
	
	struct addrinfo hints, *res;
	//struct sockaddr_storage clientAddress;
	//socklen_t clientAddressLength;
	int  status,server_socket/*, client_socket, numMessages*/;

	if(argc != 3){
		fprintf(stderr, "Usage: Bounce IP_ADDR PORTNUM\n");
		exit(1);
	}

	host = argv[1];	/*host gets passed as arg*/
	host_port = argv[2];/*host_port gets passed as arg*/
	if(strcmp(host, "unspec") == 0)
	{
		host = NULL;
		hints.ai_flags = AI_PASSIVE;
	}
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	if( (status = getaddrinfo(host, 
		host_port, &hints, &res)) !=0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", 
				gai_strerror(status));
		exit(1);
	}

	struct addrinfo *it;
	int bufSize = INET6_ADDRSTRLEN;//at least as long as IPv6
	char hostAddrView[bufSize];
	in_port_t hostPortView = 0;
	printf("Listing all available addreses and Ports... \n");
	for(it = res; it!=NULL; it=it->ai_next){
		if(it->ai_family == AF_INET)
		{
			inet_ntop(it->ai_family, 
				&(((struct sockaddr_in *)(res->ai_addr))->sin_addr), 
				hostAddrView, bufSize);
			hostPortView = ((struct sockaddr_in *)(res->ai_addr))->sin_port;
			
		}
		else if(it->ai_family == AF_INET6)
		{
			inet_ntop(it->ai_family, 
				&(((struct sockaddr_in6 *)(res->ai_addr))->sin6_addr),
				hostAddrView, bufSize);
			hostPortView = ((struct sockaddr_in6 *)(res->ai_addr))->sin6_port;
			
		}
		fprintf(stdout, "Host IP: %s\nPort: %d\n\n", hostAddrView, ntohs(hostPortView));
	}

	if( (server_socket = socket(res->ai_family, 
		res->ai_socktype, res->ai_protocol)) ==-1 )
	{
		fprintf(stderr, "socket: %s\n", strerror(errno));
		exit(1);
	}
	
	if ( bind(server_socket, res->ai_addr, res->ai_addrlen) == -1 )
	{
		fprintf(stderr, "bind: %s\n", strerror(errno));
		exit(1);
	}
	/*

	
	if ( listen(server_socket, 1) == -1)
	{
		fprintf(stderr, "listen: %s\n", strerror(errno));
		return 4;
	}
	if ( (client_socket = accept(server_socket, (struct sockaddr*)&clientAddress, &clientAddressLength)) == -1)	
	{
		fprintf(stderr, "accept: %s\n", strerror(errno));
		return 5;
	}
	if (recv(client_socket, &numMessages, sizeof(numMessages), 0) < 4)
	{
		fprintf(stderr, "accept: %s\n", strerror(errno));
		return 6;
	}

	for (int i = 0; i<numMessages; i++)
	{
		if ( (client_socket = accept(server_socket, (struct sockaddr*)&clientAddress, &clientAddressLength)) == -1)	
		{
			fprintf(stderr, "accept: %s\n", strerror(errno));
			return 7;
		}

		if ( send(client_socket, &packet, sizeof(packet), 0) == -1)
		{
			fprintf(stderr, "send: %s\n", strerror(errno));
			return 8;
		}
	}
	
	*/
	freeaddrinfo(res);
	//close(server_socket);
	return 0;
}
