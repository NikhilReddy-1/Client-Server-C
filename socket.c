#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Port number not provided. Program stopped\n");
		exit(1);
	}
	
	int sockfd, newsockfd, portno, n;
	char buff[255];
	
	struct sockaddr_in server_addr, cli_addr;
	socklen_t clilen;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error openening socket!\n");
	}
	
	bzero((char *)&server_addr ,sizeof(server_addr));
	portno = atoi(argv[1]);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portno);
	
	if(bind(sockfd, (struct sockaddr*) &server_addr ,sizeof(server_addr)) < 0){
		error("Binding fail!\n");
	}
	
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	
	if(newsockfd < 0){
		error("Error at accept!\n");
	}
	
	
	while(1)
	{
		bzero(buff, 255);
		n = read(newsockfd, buff, 255);
		if(n < 0){
			error("Read failed!\n");
		}
		printf("Client : %s\n",buff);
		bzero(buff, 255);
		fgets(buff, 255, stdin);
		
		n = write(newsockfd, buff, strlen(buff));
		if(n < 0)
		{
			error("Error on writing!\n");
		}
		
		int i = strncmp("bye", buff, 3);
		if(i == 0)
			break;
	}
	
	close(newsockfd);
	close(sockfd);
	return 0;
	
	
	
}
