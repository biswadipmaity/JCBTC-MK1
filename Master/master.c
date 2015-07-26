#include<pic18.h>
#include<htc.h>

#define XTAL_FREQ 20MHZ
#define _XTAL_FREQ 20e6
#pragma inline delay

#pragma warning disable 1385
#pragma warning disable 359

unsigned char speed_motor1;
unsigned char speed_motor2;
unsigned char speed_motor3;
unsigned char speed_motor4;

void hold(int x)
{
	unsigned int z;
	while(x)
	{
		for(z=0;z<(10000);z++);
		x--;
	}
}

#include "rpi.h"
#include "lcd.h"
#include "i2c_master.h"
#include "wheels.h"
#include "delay.h"
#include "servo.h"
#include "usart.h"
#include "backlit.h"

#define steps 50

#define i2c_RPi_DELIMITER '^'
#define i2c_Master_DELIMITER '@'

int length=0;
enum 
{
	ULTRASONIC,
	PI,
	i2c_MASTER,
	SLEEP
}receiver_i2c_state;

enum
{
	RPI_SLEEP,
	RPI_START,
	M1_DIRECTION,
	M1_SPEED,
	M2_DIRECTION,
	M2_SPEED,
	M3_DIRECTION,
	M3_SPEED,
	M4_DIRECTION,
	M4_SPEED,
	RPI_STOP
}receiver_pi_state;

#include "rpistates.h"

unsigned char c;

void main()
{
	int i;
	hold(10);
	PORTA=0x00;
	PORTB=0x00;
	PORTC=0x00;
	PORTD=0x00;
	PORTE=0x00;
	
	USBEN=0;
	UTRDIS=1;
	ADCON1=0x0F;
	CMCON=0x07;
	T0SE=1;
	T0CS=0;
	
	TRISA=0x00;		
	TRISB=0b00000011;
	TRISC=0b11000000;
	TRISD=0x00;
	TRISE=0x00;
	
	wheel_state=off;
	update_wheel();
	
	BACKLIT=0;
	lcd_init();

	hold(100);

	lcd_command(0x80);
	lcd_writeString(">");

	init_i2c();
	i2c_start();
	idle_i2c();

	i2c_write(0xa1);
	idle_i2c();

	i2c_start_read();
	idle_i2c();

	BACKLIT=1;
	initBacklit();
	backlit_dim=dim_in;
	
	USARTInit();

	PEIE=1;
	GIE=1;

	servo_reset();
	receiver_i2c_state=SLEEP;
	receiver_pi_state=RPI_SLEEP;

	if(i2c_test_write())
		while(1)
		{
			c=i2c_read();
			idle_i2c();
			i2c_read_ack();
			idle_i2c();
 			if(c!='$')
			{
lcd_data(c);
			}
			while(!rpi_buffer_empty())
			{
				char c=rpi_buffer_pop();
				take_action(c);
			}
			hold(2);
		}
/* OLD CODE:
	current_state=action_sleep;

	if(i2c_test_write())
		while(1)
		{
			c=i2c_read();
			idle_i2c();

			i2c_read_ack();
			idle_i2c();

 			if(c!='$')
			{
//				lcd_data_hex(c);
				switch(receiver_i2c_state)
				{
					case ULTRASONIC:
							if(c=='$')
							{
								receiver_i2c_state=SLEEP;
							}
							else
							{
								lcd_data(c);
								USARTWriteByte(c);
							}
						break;

					case PI:
						if (length == -1)
						{
							commandFlag = 1;
							length = c;
							int i;
							for (i = 0; i < COMMAND_ARRAY_LENGTH; i++)
								commandArray[i] = 0;
						}
						else if (commandFlag == 1)
						{
							commandFlag = 0;
							receiver_pi_function = c;
							length--;
						}
						else if (length>0)
						{
							//USARTWriteByte(c);
							commandArray[length - 1] = c;
							length--;
						}
						else if (c == i2c_RPi_DELIMITER)
						{
							switch (receiver_pi_function)
							{
								case pi_sleep:											break;
								case pi_setMotors:		setMotors(commandArray);		break;
								case pi_setMotor:		setMotor(commandArray);			break;
								case pi_getMotor:		getMotor(commandArray);			break;
								case pi_clearLCD:		clearLCD(commandArray);			break;
								case pi_LCDSetCursor:	LCDSetCursor(commandArray);		break;
								case pi_LCDPutChar:		LCDPutChar(commandArray);		break;
								case pi_LCDPrint:		LCDPrint(commandArray);			break;
								case pi_getNetwork:		getNetwork(commandArray);		break;
								case pi_networkMessage:	networkMessage(commandArray);	break;
								case pi_getTilt:		getTilt(commandArray);			break;
								case pi_getPan:			getPan(commandArray);			break;
								case pi_setTilt:		setTilt(commandArray);			break;
								case pi_setPan:			setPan(commandArray);			break;
								case pi_ping:			ping(commandArray);				break;
								default:												break;
							}
							receiver_i2c_state = SLEEP;
						}
						else //error
						{
							receiver_i2c_state = SLEEP;
						}
						break;

					case i2c_MASTER:
							if(length==-1)
							{
								length=c;
							}
							else if(length>0)
							{
								take_action(c);
								length--;
							}
							else if(c==i2c_Master_DELIMITER)
							{
								current_state=action_sleep;
								receiver_i2c_state=SLEEP;
							}
							else //Error
							{
								receiver_i2c_state=SLEEP;

							}
						break;

					case SLEEP:
							if(c==i2c_RPi_DELIMITER)
							{
								length=-1;
								receiver_i2c_state=PI;
							}
							else if(c==i2c_Master_DELIMITER)
							{
								length=-1;
								receiver_i2c_state=i2c_MASTER;
							}
							else if(c=='%')
							{
								//length=-1;
								receiver_i2c_state=ULTRASONIC;
							}
							else 
							{
								//length=-1;
								receiver_i2c_state=SLEEP;
							}
						break;
				}
			}
			if(current_state!=action_lcd_print_string)
				hold(2);
			if(current_state!=action_sleep)
				continue;

// NOTE: COMMENTED APPREHENDING CONFLICT WITH STATE MACHINE ABOVE, HOWEVER, BUFFER MUST BE FLUSHED
			while(!rpi_buffer_empty())
			{
				char c=rpi_buffer_pop();
				take_action(c);
				while((current_state!=action_sleep)&&(rpi_buffer_empty()));
			}

			hold(2);

		 	//move();
		}
	lcd_command(0xa8);	//Next Line={128+40}H
	lcd_writeString("JCBTC");

*/
}
