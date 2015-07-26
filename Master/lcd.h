#define RS RB4
#define E RB2
#define BACKLIT RB3

void lcd_command(int x);
void lcd_data(unsigned char);
void lcd_init();
void ms_delay();

bit lcd_row=0;
unsigned char lcd_column=0;

void lcd_command(int x)
{
	PORTD=x;	//			;move command to LCD port
	RS=0;
	E=1;
	ms_delay();			//delay for write
	E=0;
}

void lcd_data(char c)
{
	lcd_column++;
	if(lcd_column==16)
	{
		lcd_column=0;
		if(lcd_row==0)
		{
			lcd_row=1;
			lcd_command(0xa8);
		}
		else if(lcd_row==1)
		{
			lcd_row=0;
			lcd_command(0x01);
			lcd_command(0x80);
		}
	}
	PORTD=c;				//;move data to LCD port
	RS=1;
	E=1;
	ms_delay();			//delay for write
	E=0;
}

void ms_delay()
{
	int h;
	for(h=1;h<500;h++);
}

void lcd_init()
{
	lcd_command(0x38);	//8-bit 2 line display 
	lcd_command(0x01);	//LCD Clear
	lcd_command(0x06);	//Auto Increment address
	lcd_command(0x0C);	//Display On Cursor Off
}

void lcd_writeString(const char *str)
{
	while((*str)!='\0')
	{
		lcd_data(*str);
		ms_delay();
		//Next goto char
		str++;
	}
}

void lcd_WriteInt(int val,unsigned char field_length)
{
	char str[5]={0,0,0,0,0};
	int i=4,j=0;

	if(val<0) 
	{
		lcd_data('-');	//Write '-' sign for negative numbers.
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
		lcd_data('0'+str[i]);
	}
}

void lcd_data_hex(unsigned char c)
{
	unsigned char tmp=c/16;

	if(tmp<10)
		lcd_data('0'+tmp);
	else
		lcd_data('a'+tmp-10);

	tmp=c%16;
	if(tmp<10)
		lcd_data('0'+tmp);
	else
		lcd_data('a'+tmp-10);

}
