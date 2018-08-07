/*
    Port scanner code in c
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc , char **argv)
{
    struct hostent *host;
    int err, i , sock ,start , end;
    char hostname[50];
    struct sockaddr_in sa;

    //Get the hostname to scan
    printf("Enter hostname or IP : ");
    fgets(hostname,50,stdin);
    hostname[strcspn(hostname, "\n\r")] = '\0'; 

    //Get start port number
    printf("\nEnter start port number : ");
    scanf("%d" , &start);

    //Get end port number
    printf("Enter end port number : ");
    scanf("%d" , &end);

    //Initialise the sockaddr_in structure
    strncpy((char*)&sa , "" , sizeof sa);
    sa.sin_family = AF_INET;

    //direct ip address, use it
    if(isdigit(hostname[0]))
    {
        printf("Doing inet_addr...");
        sa.sin_addr.s_addr = inet_addr(hostname);
        printf("Done\n");
    }
    //Resolve hostname to ip address
    else if( (host = gethostbyname(hostname)) != 0)
    {
        printf("Doing gethostbyname...\n");
        printf("Host name: %s\n", host->h_name);
        printf("IP address: %s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));
        sa.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)host->h_addr)));
        printf("Done\n");
    }
    else
    {
        herror(hostname);
        exit(2);
    }

    //Start the port scan loop
    printf("Starting the portscan loop : \n");
    for( i = start ; i <= end ; i++)
    {
        //Fill in the port number
        sa.sin_port = htons(i);
        //Create a socket of type internet
        sock = socket(AF_INET , SOCK_STREAM , 0);

        //Check whether socket created fine or not
        if(sock < 0)
        {
            perror("\nSocket");
            exit(1);
        }
        //Connect using that socket and sockaddr structure
        err = connect(sock , (struct sockaddr*)&sa , sizeof sa);

        //not connected
        if( err < 0 )
        {
            //printf("%s %-5d %s\r" , hostname , i, strerror(errno));
            fflush(stdout);
        }
        //connected
        else
        {
            printf("%-5d open\n",  i);
        }
        close(sock);
    }

    printf("\r");
    fflush(stdout);
    return(0);
}
