#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include<stdlib.h>

//Defining the buffer size
#define SIZE 1024

//write_file function which will make the new file and 
//write content of the received file by the client
void write_file(int socket_fd)
{
    int n;
    FILE* fp;
    char* filename = "received.txt";
    char* buffer[1024] = {0};

    fp = fopen(filename,'w');
    while(1)
    {
        n = recv(socket_fd,buffer,SIZE,0);
        if(n<=0)    return;
        fprintf(fp,%s,buffer);
        bzero(buffer,SIZE);
    }

    return;
}
int main()
{
    //First we will declare the host and port number
    char * ip_ad = "172.0.0.1";
    int port = 6969;
    int var;

    //Now decalring some important variables
    int server_fd, new_socket;
    struct sockaddr_in server_address , new_address;
    socklen_t address_size;

    //creating buffer
    char buffer[SIZE] = {0};

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
    var = bind(server_fd,(struct sockaddr *) &server_address,&address_size);
    if(e<0)
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
    new_socket = accept(server_fd,(struct sockadddr *)&new_address,&address_size);

    //writing the data in the new file provided by the client
    write_file(new_socket);

    printf("FILE creation was successful\n");

    return 0;
}