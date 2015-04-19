#define servo1 RA0
#define servo2 RC1

unsigned int angle; 			//0'-180': angle will vary for different servo motor
unsigned int pulse;			//pulse width

int servo_up_down=0;
int servo_left_right=0;

void set_servo_1(int angle1)
{
	pulse = (angle1*3) + 200;
GIE=0;
	servo1 = 1;
		while(pulse)
		{
			__delay_us(1);
			pulse--;
		}
	servo1 = 0;
GIE=1;
	return;
}

#define min 450
#define max 2100

void set_servo_2(int angle2)
{
	pulse = (angle2*3) + 200;
GIE=0;
	servo2 = 1;
		while(pulse)
		{
			__delay_us(1);
			pulse--;
		}
	servo2 = 0;
GIE=1;
	return;
}

void up()
{
	if(servo_up_down<255)
		servo_up_down+=15;
	set_servo_1(servo_up_down);
	
}
void down()
{
	if(servo_up_down>0)
		servo_up_down-=15;
	set_servo_1(servo_up_down);
}

void left()
{
	if(servo_left_right<255)
		servo_left_right+=15;

	set_servo_2(servo_left_right);

}
void right()
{
	if(servo_left_right>0)
		servo_left_right-=15;

	set_servo_2(servo_left_right);
}

void servo_reset()
{
	int k=0;
	servo_left_right=120;
	for(k=0;k<5;k++)
	{
		set_servo_2(servo_left_right);
		hold(1);
	}

	servo_up_down=15;
	for(k=0;k<5;k++)
	{
		set_servo_1(servo_up_down);
		hold(1);
	}
}