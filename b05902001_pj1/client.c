#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<poll.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netdb.h>

int number = 0 ;
int timeout = 1000;

int is_IP(char *str){
	int a[4]  ;
	int value = 1 ;
	if(4 == sscanf(str, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3])){
		for(int i=0 ; i<4 ; i++){
			if( 0 <= a[i] && a[i] <= 255){
				continue;
			}
			else{
				value = 0 ;
			}
		}
	}
	else{
		value = 0 ;
	}

	return value ;
}

void* child(void* address){
	
	char IP_address[200] ;
	char item[50] ;
	char send_item[50];
	char host[12] ;
	int host_number = 0 ;
	const char cut[2] = ":" ;
	char *pointer ;
	strcpy(item,address);
	strcpy(send_item,address);

	//*************************//
	
//	struct addrinfo hints;
//	struct addrinfo *res;
	
//	memset(&hints, 0, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_flags = AI_PASSIVE;
	
//	getaddrinfo(NULL, "3490", &hints, &res);
	
//	printf("%s\n",res->ai_addr->sa_data);

	//*************************//

	pointer = strtok(item,cut);
	strcpy(IP_address,pointer);
	pointer = strtok(NULL,cut);
	strcpy(host,pointer);
	host_number = atoi(host);
	
	char IP_test[200] ;
	strcpy(IP_test, IP_address);	
	if(!is_IP(IP_test)){
		struct hostent *get_host ;
		get_host = gethostbyname(IP_test);
		struct in_addr **list ;
		list = (struct in_addr **) get_host->h_addr_list ;
		for(int i=0 ; list[i] != NULL ; i++ ){

			strcpy(IP_address, inet_ntoa(*list[i]));
			break;
		}

	}

	//printf("IP_address = %s host = %d\n",IP_address, host_number);
			//
			
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in connect_info ;
	connect_info.sin_family = PF_INET ;

	connect_info.sin_addr.s_addr = inet_addr(IP_address);

	connect_info.sin_port = htons(host_number);
	int tip = connect(sockfd, (struct sockaddr *)&connect_info, sizeof(connect_info));
	
	if(tip == -1){
		printf("fail to connect");
	}

	int send_number = 1 ;
	int count = 0;
			//struct timeval timeout_time ;
			//timeout_time.tv_sec = 0;
			//timeout_time.tv_usec = time * 1000 ;
			//setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_time, sizeof timeout_time);
			
				
			
	while(1){
		//send(sockfd, send_number, sizeof(sed_number));
		int receive_number ;
		
		struct pollfd fd;
		fd.fd = sockfd;
		fd.events = POLLIN ;
		//ret = poll(&fd, 1, time);
		char send_char[10] = "A";	
		char receive_char[20] ;

		send(sockfd, send_item, sizeof(send_item), 0);
		int ret = poll(&fd, 1, timeout);
		clock_t t1 = clock(); 	
		//printf("send message\n");
		switch(ret){
			case 0: // timeout
				printf("timeout when connect to %s\n", IP_address);
				break;
			default:
				
				recv(sockfd, &receive_char, sizeof(receive_char), 0);
				clock_t t2 = clock();
				printf("recv from %s, RTT = %ld msec\n", IP_address, t2-t1);
				break;
			
		}
			//recv(sockfd, &receive_number, sizeof(receive_number));
		send_number++;

		count++;
		if(count == number){
			break;
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char** argv){

//	int number = 0 ;
//	int time = 1000;
	pthread_t t[256];
	int pthread_count = 0 ;

	//printf("%s\n",argv[1]);
	
	for(int i = 1 ; i < argc ; i++){
		if( !strcmp( argv[i], "-n" ) ){
			number = atoi(argv[i+1]);
			i++ ;
			//printf("number = %d\n",number);
		}
		else if( !strcmp( argv[i], "-t" ) ){
			timeout = atoi(argv[i+1]);
			i++ ;
			//printf("time = %d\n",time);
		}
		else{
			pthread_create(&t[pthread_count], NULL, child, argv[i]);
			pthread_count++;
			//printf("IP and host = %s\n",argv[i]);
			
			//int sockfd = 0;
			//sockfd = socket(AF_INET, SOCK_STREAM, 0);
			//struct sockaddr_in connect_info ;
			//connect_info.sin_family = PF_INET ;

			//connect_info.sin_addr.s_addr = inet_addr("127.0.0.1");

			//connect_info.sin_port = htons(1328);

			//connect(sockfd, (struct sockaddr *)&connect_info, sizeof(connect_info));
			


		}
	}
	
	for(int i=0 ; i<pthread_count ; i++ ){
		pthread_join(t[i], NULL);
	}
	//char message[20] = "HEllo"; 
	//send(sockfd, message, sizeof(message), 0);

}
