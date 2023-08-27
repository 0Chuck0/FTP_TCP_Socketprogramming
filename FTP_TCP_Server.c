#include<unistd.h>
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>

//Defining the buffer size
#define SIZE 1024

//write_file function which will make the new file and 
//write content of the received file by the client

void write_file(int socket_fd,char* filename )
{
    int n;
    FILE* fp;
    char buffer[SIZE];

    fp = fopen(filename,"w");
    while(1)
    {
        n = recv(socket_fd,buffer,SIZE,0);
        if(n<=0)    return ;
        fprintf(fp,"%s",buffer);
        bzero(buffer,SIZE);
    }

    return ;
}

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

int main()
{
    //First we will declare the host and port number
    char * ip_ad = "127.0.0.1";
    int port = 8080;
    int var;

    //Now decalring some important variables
    int server_fd, new_socket;
    struct sockaddr_in server_address , new_address;
    socklen_t address_size;

    //creating buffer
    char buffer[SIZE];

    //Creating the socket
    server_fd = socket(AF_INET, SOCK_STREAM,0);
    //AF_INET --> IPv4 Address
    //SOCK_STREAM --> TCP
    //0 --> IP Protocol Number

    if(server_fd < 0)
    {
        perror("Socket creation failed !");
        exit(1);
    }
    printf("Socket creation is successful\n");

    //defining some members of the server_address structure
    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    server_address.sin_addr.s_addr = inet_addr(ip_ad);  //inet_addr converts string to IPv4 Address
    address_size = sizeof(server_address);

    //bindin the socket with IP and Port number
    var = bind(server_fd,(struct sockaddr *) &server_address,address_size);
    if(var<0)
    {
        perror("Socket binding failed !");
        exit(1); 
    }
    printf("Socket Binding is successful\n");

    //listening for the clients
    var = listen(server_fd,10);
    if(var==0)
    {
        printf("Listening....\n");
    }
    else{
        perror("Error in listening");
        exit(1);
    }

    //accepting the connection at new_socket
    address_size = sizeof(new_address);
    new_socket = accept(server_fd,(struct sockaddr *)&new_address,&address_size);

    //writing the data in the new file provided by the client
    char* filename = "received.txt";
    write_file(new_socket,filename);
    printf("FILE creation was successful\n");
/*
    //So now i want to make a feature where the user can ask the server to resend the same file
    //reading the string of the user
    recv(new_socket,buffer,SIZE,0);
    if(buffer == "yes")
    {
    	FILE *fp = fopen(filename,"r");
    	if(fp == NULL)
	{
		perror("Unable to read the file");
		exit(1);
	}
    	send_file(fp,new_socket);
    	printf("********\nFIle was sent successfully\n*********\n");
    	
    }
    //so here i am waiting for users confirmation
    */
    return 0;
}
