
void USARTInit()
{
	//Baud Rate = 115200 Bits per Second
	//*** Note: Valid On 20MHz Crystal ONLY ***
	//For other crystal freq calculate new values for SPBRG
	SPBRG=42;

	SYNC=0;//Asynchronous Mode
	SPEN=1;//Serial Port Enabled

	TXEN=1;		//Transmit enabled	
	TXIE=0;

    RCIE=1;     // 1 = Enables the EUSART receive interrupt
	RCIP = 1;   // High Priority interrupt
	IPEN=1;     // Enable interrupt Priority
    GIEL = 1;	//enable interrupt peripheral
    GIEH = 1;   //enable interrupt global 

 	RX9=0;//9-bit reception is not desired,

	BRGH=1; //High Speed
		
	//BAUDCON
	BRG16=1;//16-bit Baud Rate Generator


	//RCSTA
	CREN=1;	//Enable Receiver (RX) when in asynchronous mode
}

void lcd_command_ISR(int x)
{
	PORTD=x;	//			;move command to LCD port
	RS=0;
	E=1;
	ms_delay();			//delay for write
	E=0;
}

void lcd_data_ISR(char c)
{
	lcd_column++;
	if(lcd_column==16)
	{
		lcd_column=0;
		if(lcd_row==0)
		{
			lcd_row=1;
			lcd_command_ISR(0xa8);
		}
		else if(lcd_row==1)
		{
			lcd_row=0;
			lcd_command_ISR(0x01);
			lcd_command_ISR(0x80);
		}
	}
	PORTD=c;				//;move data to LCD port
	RS=1;
	E=1;
	ms_delay();			//delay for write
	E=0;
}

void lcd_data_hex_ISR(unsigned char c)
{
	unsigned char tmp=c/16;

	if(tmp<10)
		lcd_data_hex('0'+tmp);
	else
		lcd_data_hex('a'+tmp-10);

	tmp=c%16;
	if(tmp<10)
		lcd_data_hex('0'+tmp);
	else
		lcd_data_hex('a'+tmp-10);

}

void interrupt ISR_High(void)
{
	unsigned char read_data;
	if(RCIF)
	{
		read_data=RCREG;
		rpi_buffer_push(read_data);
		RCIF=0;
	}
}

void USARTWriteByte(char ch)
{
	//Wait for TXREG Buffer to become available
	while(!TXIF);

	//Write data
	TXREG=ch;
}