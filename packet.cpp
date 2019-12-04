#include "packet.h"
#include <stdio.h>

void extract_packet(packet_t * packet, char * buffer)
{

  float acc,ax,ay,az,gyro,gx,gy,gz,mag,mx,my,mz;
  double time_stamp;
  sscanf(buffer,"%lf,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", 
  	 &time_stamp,&acc,&ax,&ay,&az,&gyro,&gx,&gy,&gz,&mag,&mx,&my,&mz); 

  packet->time_stamp = time_stamp;
  packet->gyro = gyro;
  packet->acc = acc;
  packet->mag = mag;
  packet->ax = ax;
  packet->ay = ay;
  packet->az = az;
  packet->gx = gx;
  packet->gy = gy;
  packet->gz = gz;
  packet->mx = mx;
  packet->my = my;
  packet->mz = mz;
	   
}

void print_packet(packet_t * in)
{
  printf("%lf, %f,%f,%f, %f,%f,%f, %f,%f,%f\n",in->time_stamp,in->ax,in->ay,
	 in->az,in->gx,in->gy,in->gz,in->mx,in->my,in->mz);
}
