//$ ip address
// gcc raw_out.cpp -lm
//./a.out 5555 

// This example was modified from TCP/IP Sockets in C: Practical Guide
// for Programmers by Michael J Donahoo

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind, and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() and getpid() */
#include <fcntl.h>      /* for fcntl() */
#include <sys/file.h>   /* for O_NONBLOCK and FASYNC */
#include <signal.h>     /* for signal() and SIGALRM */
#include <errno.h>      /* for errno */
#include <math.h>
#include "socket.h"

//--------------

#include "cv.h"
#include "highgui.h"
#include "cvplot.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int sock;                        /* Socket -- GLOBAL for signal handler */
struct sockaddr_in echoServAddr; /* Server address */
unsigned short echoServPort;     /* Server port */
struct sigaction handler;        /* Signal handling action definition */
char echoBuffer[ECHOMAX];         /* Datagram buffer */

// volatile keyword is a qualifier that is applied to a variable when it is declared.
// It tells the compiler that the value of the variable may change at any time--without a
// ny action being taken by the code the compiler finds nearby.
volatile int packet_count;

  
int main(int argc, char *argv[])
{

    int previous_packet = 0;
    packet_count = 0;
    float acc,ax,ay,az,gyro,gx,gy,gz,mag,mx,my,mz;
    double time_stamp;
    
    // Test for correct number of parameters 
    if (argc != 2)
    {
        fprintf(stderr,"Usage:  %s <SERVER PORT>\n", argv[0]);
        exit(1);
    }

    // get the port
    echoServPort = atoi(argv[1]);
    start_socket();
     
    //    namedWindow( "ORIENTATION", WINDOW_AUTOSIZE );
    
    // do a continous loop and process the samples 
    // the packets are read in the background 
    for (;;)
      {
	//if we received a packet, than process it
	if( (packet_count > 0) && (packet_count > previous_packet))
	  {
	    previous_packet = packet_count;
	    printf("We receive a packet #%i \n", packet_count);

	    //handle the packet
	    sscanf(echoBuffer,"%lf,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", 
		   &time_stamp,&acc,&ax,&ay,&az,&gyro,&gx,&gy,&gz,&mag,&mx,&my,&mz); 
	    
	   printf("%lf, %f,%f,%f, %f,%f,%f, %f,%f,%f\n",time_stamp,ax,ay,az,gx,gy,gz,mx,my,mz);

	    
	  }
	
      } //-----------------------------------------------------


} // main



