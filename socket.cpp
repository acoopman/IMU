#include "socket.h"

extern int sock;                        /* Socket -- GLOBAL for signal handler */
extern struct sockaddr_in echoServAddr; /* Server address */
extern unsigned short echoServPort;     /* Server port */
extern struct sigaction handler;        /* Signal handling action definition */
extern volatile int packet_count;
extern char echoBuffer[ECHOMAX];         /* Datagram buffer */

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
    float acc,ax,ay,az,gyro,gx,gy,gz,mag,mx,my,mz;
    double time_stamp;

    /*
    if(first_time == 1)
      {
	for(int i = 0; i < 256; i++)
	  samples[i] = 0;

	first_time = 0;
      }
    */
    
    //make a plotting window
#ifdef PLOT_DATA
    static int count = 10;
#endif
    
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

	  //AIDAN HERE
	  //*******************************************************************************
	  //*******************************************************************************
	  //*******************************************************************************
	  
	  //            printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
	  // printf("sensor=%s \n", echoBuffer);

	  //	  Timestamp [sec], sensorid, x, y, z, sensorid, x, y, z, sensorid, x, y, z
	  sscanf(echoBuffer,"%lf,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", 
		 &time_stamp,&acc,&ax,&ay,&az,&gyro,&gx,&gy,&gz,&mag,&mx,&my,&mz); 
	  	 
	  // printf("%lf, %f,%f,%f, %f,%f,%f, %f,%f,%f\n",time_stamp,ax,ay,az,gx,gy,gz,mx,my,mz);

	  float mag = sqrtf(ax*ax + ay*ay + az*az);

	  //subtract gravity out
	  mag -= 9.81f;

	  if( mag > 0.5)
	    printf("TAP mag = %f \n", mag);

	  /*

	  //put the most recent sample in the array
	  //shift the array over 1
	  for(int i = 0; i < 255; i++)
	    {
	      samples[i] = samples[i+1];
	    }

	  //put the sample in the end
	  samples[255] = (int) (10* fabs(mag)) ; //(int)5;

	  
	  //Plot the input
	  //-------------------------------------------------
#ifdef PLOT_DATA
	  CvPlot::clear("Magnitude");
	  CvPlot::plot("Magnitude", samples, 256);
#endif
	  if((ax<11)&&(ax>8))
	    {
	      printf("ax posative ----------\n");
	      imshow("ORIENTATION", side_face);
	    }
	  if((ay<11)&&(ay>8))
	    {
	      printf("ay posative----------\n");
	      imshow("ORIENTATION", verticle_up);   
	    }
	  if((az<11)&&(az>8))
	    {
	      printf("az posative----------\n");
	      imshow("ORIENTATION", screen_up);   
	    }
	  if((ax>-11)&&(ax<-8))
	    {
	      printf("ax Negative ----------\n");
	      imshow("ORIENTATION", other_side_face);   
	    }
	  if((ay>-11)&&(ay<-8))
	    {
	      printf("ay Negative ----------\n");
	      imshow("ORIENTATION", upside_down);   
	    }
	  if((az>-11)&&(az<-8))
	    {
	      printf("az Negative ----------\n");
	      imshow("ORIENTATION", screen_down);   
	    }
	  //Tell me what side is up on phone???
	  */
	  
	  //*******************************************************************************
	  //*******************************************************************************
	  //*******************************************************************************
	  packet_count++;
	  //--------------------------------------------------
	  
	  if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) 
		     &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)
	    printf("sendto() failed");
        }
    }  while (recvMsgSize >= 0);
    /* Nothing left to receive */
}
