
void initComm()
{
	//Baud Rate = 9600 Bits per Second
	//*** Note: Valid On 20MHz Crystal ONLY ***
	//For other crystal freq calculate new values for SPBRG
	SPBRG=129;
	BRGH=1; //High Speed
	
	TXEN=1;//Transmit enabled

	SYNC=0;//Asynchronous Mode
	SPEN=1;//Serial Port Enabled


	RCIE=1;    // 0 = Disbles the EUSART receive interrupt
	TXIE=0;
	PEIE=1;

 	RX9=0;	   //9-bit reception is not desired,

	//RCSTA
	CREN=1;	//Enable Receiver (RX) when in asynchronous mode
}

char ptr=0;

void USARTWriteByte(char ch)
{
	//Wait for TXREG Buffer to become available
	while(!TXIF);

	//Write data
	TXREG=ch;
}

void USARTWriteString(const char *str)
{
/*
	while((*str)!='\0')
	{
		//Wait for TXREG Buffer to become available
		while(!TXIF);

		//Write data
		TXREG=(*str);

		//Next goto char
		str++;
	}
*/
}
void USARTWriteInt(int val,unsigned char field_length)
{
/*
	char str[5]={0,0,0,0,0};
	int i=4,j=0;

	if(val<0) 
	{
		USARTWriteByte('-');	//Write '-' sign for negative numbers.
		val=(val*(-1));				//Make it positive.
	}

	//Convert Number To String and pump over Tx Channel.
	while(val)
	{
		str[i]=val%10;
		val=val/10;
		i--;
	}
	if(field_length>5)
		while(str[j]==0) j++;
	else
		j=5-field_length;
	
	for(i=j;i<5;i++)
	{
		USARTWriteByte('0'+str[i]);
	}
*/
}
