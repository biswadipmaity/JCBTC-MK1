
enum dim{
	dim_rainbow,
	dim_out,
	dim_in,
	dim_fixed
}backlit_dim;

int IntCount;
int backlit_duty;
int dim_flag;

int speed_motor;

void initBacklit()
{
	TMR0ON=0;//Timer0 = off;

	PCFG0=1;//All digital
	PCFG1=1;
	PCFG2=1;
	PCFG3=1;
	

T0PS2=1;
T0PS1=1;
T0PS0=1;

	TMR0IE=1; //Enable Timer 0 overflow interrupt
	TMR0IP=0;//Low proirity

	T08BIT=1; //8bit timer
	T0CS=0;//Use clock
	PSA=1; //No prescaler

	IntCount=0;
	TMR0ON=1;
}


void interrupt low_priority ISR_Low(void)
{

	if(TMR0IF==1)
	{
		TMR0=250;

		IntCount++;
		if(IntCount==255)
		{
				BACKLIT=1;
				update_wheel();
				IntCount=0;

				switch(backlit_dim)
				{
					case dim_rainbow: 
						if(dim_flag==0)
						{
							if(backlit_duty==128)
								dim_flag=1;
							else
								backlit_duty++;
						}
						else
						{
							if(backlit_duty==0)
								dim_flag=0;
							else
								backlit_duty--;
						}
						break;

					case dim_out:
							if(backlit_duty>100)
								backlit_duty--;
						break;	

					case dim_in:if(backlit_duty<255)
							backlit_duty++;
						break;

					case dim_fixed:	break;
				}
		}
		
		if((IntCount-backlit_duty)>=0)		
		{
			BACKLIT=0;
		}


/*
		if((IntCount-speed_motor)>=0)
		{
			M1A=1;
			M1B=1;
		
			M2A=1;
			M2B=1;
		
			M3A=1;
			M3B=1;
		
			M4A=1;
			M4B=1;
		}
*/

		if((IntCount-speed_motor1)>=0)
		{
			M1A=0;
			M1B=0;
		}

		if((IntCount-speed_motor2)>=0)
		{
			M2A=0;
			M2B=0;
		}

		if((IntCount-speed_motor3)>=0)
		{
			M3A=0;
			M3B=0;
		}

		if((IntCount-speed_motor4)>=0)
		{
			M4A=0;
			M4B=0;
		}

		TMR0IF=0;
	}
}
