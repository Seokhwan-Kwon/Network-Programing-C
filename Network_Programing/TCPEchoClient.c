//
//  TCPEchoClient.c
//  Network_Programing
//
//  Created by Seokhwan Kwon on 2020/04/23.
//  Copyright © 2020 Seokhwan Kwon. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;
    char *servIP;
    char *echoString;
    char echoBuffer[RCVBUFSIZE];
    unsigned int echoStringLen;
    int bytesRcvd, totalBytesRcvd;
    
    if((argc <3) || (argc > 4))
    {
        fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port] \n", argv[0]);
        exit(1);
    }
    
    servIP = argv[1];
    echoString = argv[2];
    
    if (argc == 4)
        echoServPort = atoi(argv[3]);
    else
        echoServPort = 7;
    
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port = htons(echoServPort);
    
    if(connect(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) <0)
        DieWithError("connect() failed");
    
    echoStringLen = (unsigned int)strlen(echoString); // (unsigned int) 요고 내가 추가한 것
    
    if(send(sock, echoString, echoStringLen, 0) != echoStringLen)
        DieWithError("send() sent a differnet number of bytes than expected");
    
    totalBytesRcvd = 0;
    printf("Received : ");
    while(totalBytesRcvd < echoStringLen)
    {
        if((bytesRcvd = (int)recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)   // (int) 요거 내가 추가한 것
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;
        echoBuffer[bytesRcvd] = '\0';
        printf("%s", echoBuffer);   // "%s", 요거 내가 추가한 것
    }
    printf("\n");
    close(sock);
    exit(0);
}