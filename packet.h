#ifndef PACKET_H
#define PACKET_H

#define ECHOMAX 255     /* Longest string to echo */

typedef struct packet
{
  float time_stamp,acc,ax,ay,az,gyro,gx,gy,gz,mag,mx,my,mz;
  
} packet_t;

void extract_packet(packet_t * packet, char * buffer);

void print_packet(packet_t * in);



#endif
