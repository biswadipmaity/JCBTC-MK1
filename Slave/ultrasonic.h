#define Echo RB0
#define Trigger RB3

char edge =0;
char capture =0;
unsigned tOld;
unsigned tNew;
unsigned tWord;
unsigned distance;

void ultra_init()
{
	CCP1CON=0x05;	//Capture mode: every rising edge

	T1CON=0b00100001;
	TMR1IE=0;

	GIE=1;
	PEIE=1;
	CCP1IE=1;
	CCP1IF=0;
}

void calculate()
{
	if(capture)
	{
		CCP1IE=0;
		capture=0;
		tWord= ~tOld + tNew + 1;
		CCP1CON=0x05;

		distance=tWord;

	//	__delay_us(60);

		CCP1IF=0;
		CCP1IE=1;
	}
}


void sendPulse(void) 
{
    Trigger = 1;
    __delay_us(10);
    Trigger = 0;
}

void sendUnsignedInt(unsigned int val)
{
	char str[5]={0,0,0,0,0};
	int i=4;

	while(val)
	{
		str[i]=val%10;
		val=val/10;
		i--;
	}

	RCIE=0;
	queue_push('%');	
	for(i=0;i<5;i++)
	{
		queue_push('0'+str[i]);
	}
	RCIE=1;
}

int last_state=0;
int present_state=0;
void ultrasonic_check()
{
	sendPulse();
 	while(capture==0);
	calculate();


	if(distance<500)
		present_state=1;
	else if(distance<1000)
		present_state=2;
	else if(distance<2000)
		present_state=3;
	else if(distance<3000)
		present_state=4;
	else if(distance<4000)
		present_state=5;
	else
		present_state=6;
	
	if(last_state!=present_state)
	{
		RCIE=0;
		queue_push('%');
		queue_push('A'+present_state);
		RCIE=1;
		last_state=present_state;
	}

	//sendUnsignedInt(distance);
	//__delay_ms(300);
}