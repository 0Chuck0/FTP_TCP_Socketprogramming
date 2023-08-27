#include<unistd.h>
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>

#define SIZE 1024

void send_file(FILE* fp,int socket_fd)
{
	int n;
	char data[SIZE]={0};
	int flag;
	//sending the data until the file gets empty
	while(fgets(data,SIZE,fp) != NULL)
	{
		flag = send(socket_fd,data,SIZE,0);
		if(flag==-1)
		{
			perror("Error in sending the file");
			exit(1);
		}
		bzero(data,SIZE);
	}
	
}

void write_file(int socket_fd,char* filename)
{
    int n;
    FILE* fp;
    char buffer[SIZE];

    fp = fopen(filename,"w");
    while(1)
    {
        n = recv(socket_fd,buffer,SIZE,0);
        if(n<=0)    return;
        fprintf(fp,"%s",buffer);
        bzero(buffer,SIZE);
    }

    return;
}

int main()
{
	//Declare IP and Port number
	char* ip_ad = "127.0.0.1";
	int port = 8080;
	
	//Declare some inmportant variables
	struct sockaddr_in sock_address;
	int socket_fd;
	
	//Create socket
	socket_fd = socket(AF_INET,SOCK_STREAM,0);
	
	if(socket_fd<0)
	{
		perror("Unable to create the socket");
		exit(1);
	}
	printf("Socket Creation Successful\n");
	
	
	//set some values of the socket address structure
	sock_address.sin_family = AF_INET;
	sock_address.sin_port = port;
	sock_address.sin_addr.s_addr = inet_addr(ip_ad);
	
	
	//connect socket with the server
	int var ;
	var =  connect(socket_fd,(struct sockaddr *) &sock_address, sizeof(sock_address));
	if(var == -1)
	{
		perror("Could not connect to the server!");
		exit(1);
	}
	printf("connection with the server was successfully established\n");
	
	//send the file to the server after establishment of the connection
	char *filename = "send.txt";
	FILE *fp = fopen(filename,"r");
	
	if(fp == NULL)
	{
		perror("Unable to read the file");
		exit(1);
	}
	
	//Send the file to the server
	send_file(fp,socket_fd);
	
	sleep(5);
	printf("File was sent successfully.\n");
	/*
	//now I will take the input from the client that if he wants the file back
	//printf("Do you want server to send the file back?\n type yes or no\n");
	char *answer = "yes";
	//scanf("%s",answer);
	printf("%s",answer);
	
	
	send(socket_fd,answer,sizeof(answer),0);
	//after sending the string to the server user will wait for the file
	if(answer == "yes")
	{
		char* filename = "received.txt";
    		write_file(socket_fd,filename);
    		printf("********\nFile Creation was successful\n*********\n");
	}
		
		*/
	printf("Closing the connection\n");
	sleep(5);
  	close(socket_fd);
	
	return 0;
}
