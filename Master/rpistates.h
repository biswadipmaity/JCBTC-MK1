#define COMMAND_ARRAY_LENGTH 20

int action_length = -1;
int commandFlag = -1;
BYTE commandArray[COMMAND_ARRAY_LENGTH];

enum
{
	pi_sleep = 0,
	pi_setMotors = 10,
	pi_getMotor = 11,
	pi_setMotor = 12,
	pi_getNetwork = 20,
	pi_networkMessage = 21,
	pi_clearLCD = 31,
	pi_LCDSetCursor = 32,
	pi_LCDPutChar = 33,
	pi_LCDPrint = 34,
	pi_getPan = 41,
	pi_getTilt = 42,
	pi_setPan = 43,
	pi_setTilt = 44,
	pi_ping = 50
}receiver_pi_function;

void setMotors(BYTE *array) {
	int i;

	for(i=0;i<8;i++)
	{
		lcd_data_hex(array[i]);
		lcd_data(' ');
	}

	speed_motor4=array[0];
	speed_motor3=array[2];
	speed_motor2=array[4];
	speed_motor1=array[6];

	if(array[1]==0)//Forward
	{
		M4A_mem=1;
		M4B_mem=0;
	}
	else
	{
		M4A_mem=0;
		M4B_mem=1;
	}


	if(array[3]==0)//Forward
	{
		M3A_mem=1;
		M3B_mem=0;
	}
	else
	{
		M3B_mem=1;
		M3A_mem=0;
	}

	if(array[5]==0)//Forward
	{
		M2A_mem=1;
		M2B_mem=0;
	}
	else
	{
		M2A_mem=0;
		M2B_mem=1;
	}


	if(array[7]==0)//Forward
	{
		M1A_mem=1;
		M1B_mem=0;
	}
	else
	{
		M1A_mem=0;
		M1B_mem=1;
	}

	
lcd_data('M');
lcd_data('S');
}
void getMotor(BYTE *array) {
	switch(array[0])
	{
		case 1:
			USARTWriteByte(M1B_mem);
			USARTWriteByte(speed_motor1);
		break;
		case 2:
			USARTWriteByte(M2B_mem);
			USARTWriteByte(speed_motor1);
		break;
		case 3:
			USARTWriteByte(M3B_mem);
			USARTWriteByte(speed_motor1);
		break;
		case 4:
			USARTWriteByte(M4B_mem);
			USARTWriteByte(speed_motor1);
		break;
	}
} 
void setMotor(BYTE *array)
{
/*
	if(array[2]=1) //Front Left
	{
		if(array[1]=0) //Forward
		{
			M1A_mem=1;
			M1B_mem=0;
		}
		else if(array[1]=1) //Backward
		{
			M1A_mem=0;
			M1B_mem=1;
		}	
		speed_motor1=array[0];	
	}	
	else if(array[2]=2) //Front Right
	{
		if(array[1]=0) //Forward
		{
			M2A_mem=1;
			M2B_mem=0;
		}
		else if(array[1]=1) //Backward
		{
			M2A_mem=0;
			M2B_mem=1;
		}	
		speed_motor2=array[0];
	}
	else if(array[2]=3) // Back Left
	{
		if(array[1]=0) //Forward
		{
			M3A_mem=1;
			M3B_mem=0;
		}
		else if(array[1]=1) //Backward
		{
			M3A_mem=0;
			M3B_mem=1;
		}	
		speed_motor3=array[0];
	}
	else if(array[2]=4) //Back Right
	{
		if(array[1]=0) //Forward
		{
			M4A_mem=1;
			M4B_mem=0;
		}
		else if(array[1]=1) //Backward
		{
			M4A_mem=0;
			M4B_mem=1;
		}	
		speed_motor4=array[0];
	}	
*/
} 

void getNetwork(BYTE *array) 
{
	
} 
void networkMessage(BYTE *array) {
} 

void clearLCD(BYTE *array) 
{
	lcd_command(0x01);
} 
void LCDSetCursor(BYTE *array) 
{
	
} 
void LCDPutChar(BYTE *array) 
{

}
 
void LCDPrint(BYTE *array) 
{   int count=0;
	while((*(array+count))!='\0')
		count++;
	while(count>0)
	{
		lcd_data(array[count-1]);
		ms_delay();
		//Next goto char
		count--;
	}
} 

void getPan(BYTE *array) 
{
	
} 
void getTilt(BYTE *array)
{

} 
void setPan(BYTE *array) 
{
	set_servo_1(array[0]);
} 
void setTilt(BYTE *array) 
{
	set_servo_2(array[0]);
} 

void ping(BYTE *array) {
} 

void take_action(char c)
{
	if(commandFlag == -1)
	{
		commandFlag = 0;
		int i;
		for (i = 0; i < COMMAND_ARRAY_LENGTH; i++)
			commandArray[i] = 0;
	}
	else if(commandFlag == 0)
	{
		commandFlag = 1;
		action_length = c;
	}
	else if(commandFlag == 1)
	{
		commandFlag = 2;
		receiver_pi_function = c;
		action_length--;
	}
	else if (action_length>0)
	{
		commandArray[action_length - 1] = c;
		action_length--;
	}
	else if (c == i2c_RPi_DELIMITER)
	{
		switch(receiver_pi_function)
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
		action_length = -1;
		commandFlag = -1;
	}
	else //error
	{
		action_length = -1;
		commandFlag = -1;
	}
}