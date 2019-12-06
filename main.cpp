//$ ip address
// gcc raw_out.cpp -lm
//./a.out 5555 

// This example was modified from TCP/IP Sockets in C: Practical Guide
// for Programmers by Michael J Donahoo

#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <math.h>
#include "socket.h"
#include "cv.h"
#include "highgui.h"
#include "cvplot.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "packet.h"

using namespace cv;
using namespace std;

int sock;                        /* Socket -- GLOBAL for signal handler */
struct sockaddr_in echoServAddr; /* Server address */
unsigned short echoServPort;     /* Server port */
struct sigaction handler;        /* Signal handling action definition */

#define ECHOMAX 255     /* Longest string to echo */
char echoBuffer[ECHOMAX];         /* Datagram buffer */


// volatile keyword is a qualifier that is applied to a variable when it is declared.
// It tells the compiler that the value of the variable may change at any time--without a
// ny action being taken by the code the compiler finds nearby.
volatile int packet_count;


int main(int argc, char *argv[])
{
  const int N = 400;
  float mag_array[N];
  unsigned char samples[N];
  packet_t packets[N];
  
  int previous_packet = 0;
  packet_count = 0;

  // Test for correct number of parameters 
  if (argc != 2)
    {
      fprintf(stderr,"Usage:  %s <SERVER PORT>\n", argv[0]);
      exit(1);
    }
  
  // get the port
  echoServPort = atoi(argv[1]);
  start_socket();

  //initialize it to 0
  for(int i = 0; i<N; i++)
    {
      mag_array[i]=0;
      samples[i] = 0;
    }
  
  //    namedWindow( "ORIENTATION", WINDOW_AUTOSIZE );
  
  // do a continous loop and process the samples 
  // the packets are read in the background 
  for (;;)
    {
      //if we received a packet, than process it
      if( (packet_count > 0) && (packet_count > previous_packet))
	{
	  int packet_idx = packet_count%N;
	  previous_packet = packet_count;

	  packet_t* curr_ptr =  &packets[packet_idx];
	  printf("We receive a packet #%i \n", packet_count);
	  
	  extract_packet(curr_ptr, echoBuffer);
	  
	  print_packet(curr_ptr);
	  float mag = sqrt((curr_ptr->ax*curr_ptr->ax)+(curr_ptr->ay*curr_ptr->ay)+(curr_ptr->az*curr_ptr->az));

	  //subtract gravity out
	  mag -= 9.81f;
	  
	  //shift over the array to the left to allow new magnitude value
	  for(int i =0; i < N-1; i++)
	    mag_array[i] = mag_array[i+1];

	  //put new magnitude value in the array at the end
	  mag_array[N-1] = mag;


	  // PLOT THE OUTPUT
	  for(int i = 0; i < N-1; i++)
	    samples[i] = samples[i+1];
	  samples[N-1] = (int) (10* fabs(mag)) ; //(int)5;
	  CvPlot::clear("Magnitude");
	  CvPlot::plot("Magnitude", samples, N);
	}
      
    } //-----------------------------------------------------
  
  
} // main



