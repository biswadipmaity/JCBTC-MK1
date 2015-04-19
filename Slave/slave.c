#include <htc.h>
#pragma warning disable 359
#pragma warning disable 1090


#define _XTAL_FREQ 20e6
#pragma inline delay

#include "uart.h"
#include "queue.h"
#include "ultrasonic.h"
#include "xbee.h"
#include "i2c_slave.h"


void hold(int x)
{
	unsigned int z;
	while(x)
	{
		for(z=0;z<(10000);z++);
		x--;
	}
}

#if TYPE == COORDINATOR
	#define STRAY_LENGTH 18
#endif

#if TYPE == ROUTER
	#define STRAY_LENGTH 12
#endif

unsigned long receive_length;
enum receive_state
{
	frame_start,
	length_high,
	length_low,
	stray,
	data,
	checksum
}receiver;


void main() {
    OSCCON =0b01111100;

	hold(5);
    GIE=0;
    ANSEL =0x00;
    CMCON =0x07;

    PORTA=0x00;
    TRISA=0x00;
    PORTB=0x00;
    TRISB=0b00110111; 

	initComm();
	hold(5);

//	ultra_init();
//	USARTWriteString("Hello\r");
	init_queue();

//	queue_push('T');
//	queue_push('E');
//	queue_push('S');
 	i2c_slave_init();

	init_timer();

	INITIALIZE_NETWORK();

	//sendDataMaster_putchar('e');
	int current_cursor=0;
	while(1)
	{
		if(OERR)
		{
			CREN=0;
			CREN=1;
		}
		if(SSPOV)
			SSPOV=0;
	
		if(receive_flag==1)
		{
			switch(receiver)
			{
				case frame_start:
					if(read_data==0x7E)
						receiver=length_high;
				break;

				case length_high:
					receive_length=read_data;
					receive_length<<8;
					receiver=length_low;
				break;

				case length_low:
					receive_length+=read_data;

//sendDataMaster_putchar('#');
//sendDataMaster_hex(receive_length);

					receive_length-=STRAY_LENGTH;
					receiver=stray;


					current_cursor=0;
					//queue_push('*');
				break;

				case stray:
					if(current_cursor==0)
					{
						if(read_data==0x8a)
						{
							receiver=frame_start;
						}
					}
					current_cursor++;
					if(current_cursor==STRAY_LENGTH)
					{
						current_cursor=0;
						receiver=data;
					}
				break;
					
				case data:
					if(current_cursor<receive_length)
						packet_in[current_cursor]=read_data;
					current_cursor++;

//if((current_cursor==1)||(current_cursor==receive_length))
//	sendDataMaster_hex(read_data);


					//queue_push(read_data);
					if(current_cursor==receive_length)
						receiver=checksum;
				break;

				case checksum:
					STATE();
//sendDataMaster_putchar('&');
					receiver=frame_start;
					//print_packet(packet_in,JUXP_LENGTH);
/*
					switch(packet_in[1])
					{
						case 'w':
						case 'a':
						case 's':
						case 'd':
						case 'i':
						case 'j':
						case 'k':
						case 'l':
						case 'x':
							queue_push(packet_in[1]);
						break;
						default:
						break;
					}
*/
				break;

				default:
						receiver=frame_start;
				break;

			}
			receive_flag=0;
		}

		if(timer==TIMER_OVERFLOW)
		{
			STOP_TIMER();
			STATE();
		}
	

		//if(queue_size>0)	
		//	USARTWriteByte(queue_pop());

	//	ultrasonic_check();
	//	hold(2);
	}
}