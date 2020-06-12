#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<sys/select.h>
#include<arpa/inet.h>

#define MAX_FD 256

int main(int argc, char** argv){
	if(argc != 2){
		printf("must have a port\n");
		exit(0);
	}
	int sockfd = 0 ;
	/*
	struct addrinfo hint;
        struct addrinfo	*res;
	
	memset(&hint, 0, sizeof(hint));

	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;
	//char *host ;

	getaddrinfo( "127.0.0.1", "3490" , &hint , &res);
	
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
       	bind(sockfd, res->ai_addr, res->ai_addrlen);
	listen(sockfd, 5);
	*/
	

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in info, client_info ;

	info.sin_family = AF_INET;
	info.sin_port = htons(atoi(argv[1]));
	info.sin_addr.s_addr = INADDR_ANY ;
	
	bind(sockfd, (struct sockaddr*)&info , sizeof(info));
	listen(sockfd, 10);
	
	fd_set read_set;
	fd_set working_set;
	FD_ZERO(&read_set);
	FD_SET(sockfd, &read_set);

	char IP_buffer[MAX_FD][50];
	int Port_buffer[MAX_FD];

	
	while(1){
		memcpy(&working_set, &read_set, sizeof(fd_set));
		int retval = select(MAX_FD, &working_set, NULL, NULL, NULL);
		for(int fd = 0 ; fd < MAX_FD ; fd ++ ){
			if(!FD_ISSET(fd, &working_set)){
				continue;
			}
			if(fd == sockfd){ // new client
								
				int len = sizeof(client_info);
				//char buffer[256] ;

				int clientfd = accept(sockfd, (struct sockaddr*)&client_info, &len );

				//printf("port = %u\n",client_info.sin_port);
				char *buffer = inet_ntoa(client_info.sin_addr);
				//printf("IP = %s\n", buffer);
				strcpy(IP_buffer[clientfd],buffer);
				Port_buffer[clientfd] = client_info.sin_port;
				//printf("recv a client\n");
				FD_SET(clientfd,&read_set);
			}
			else{
								
				char get_char[50] ;
				char send_char[10] = "G";
				
				int sz = recv(fd, get_char, sizeof(get_char), 0);
				if(sz == 0){ // disconnect
					FD_CLR(fd,&read_set);
				}
				else{
					printf("recv from %s:%d\n",IP_buffer[fd], Port_buffer[fd]);
					send(fd, send_char, sizeof(send_char), 0);
				}
			}
		}
	
	}
	
	printf("END!");
//	printf("%d\n",get_number);

	//printf("%d\n",hint.ai_flags);


	//sock = socket(AF_INET, SOCK_STREAM, 0);
	//struct sock_addr_in info;

//	freeaddrinfo(res);	
}
