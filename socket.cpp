#include "socket.h"
#include "packet.h"

extern int sock;                        /* Socket -- GLOBAL for signal handler */
extern struct sockaddr_in echoServAddr; /* Server address */
extern unsigned short echoServPort;     /* Server port */
extern struct sigaction handler;        /* Signal handling action definition */
extern volatile int packet_count;
extern char echoBuffer[];         /* Datagram buffer */

int start_socket()
{

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        printf("socket() failed");

    /* Set up the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        printf("bind() failed");

    /* Set signal handler for SIGIO */
    handler.sa_handler = SIGIOHandler;
    /* Create mask that mask all signals */
    if (sigfillset(&handler.sa_mask) < 0) 
        printf("sigfillset() failed");
    /* No flags */
    handler.sa_flags = 0;

    if (sigaction(SIGIO, &handler, 0) < 0)
        printf("sigaction() failed for SIGIO");

    /* We must own the socket to receive the SIGIO message */
    if (fcntl(sock, F_SETOWN, getpid()) < 0)
        printf("Unable to set process owner to us");

    /* Arrange for nonblocking I/O and SIGIO delivery */
    if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
        printf("Unable to put client sock into non-blocking/async mode");

  return 0;
}



void SIGIOHandler(int signalType)
{
    struct sockaddr_in echoClntAddr;  /* Address of datagram source */
    unsigned int clntLen;             /* Address length */
    int recvMsgSize;                  /* Size of datagram */
    
    do  /* As long as there is input... */
    {      
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,
               (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
        {
            /* Only acceptable error: recvfrom() would have blocked */
            if (errno != EWOULDBLOCK)  
                printf("recvfrom() failed");
        }
        else
	  {
	    packet_count++;
	    
	    if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) 
		       &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)
	      printf("sendto() failed");
	  }
    }  while (recvMsgSize >= 0);
    /* Nothing left to receive */
}
