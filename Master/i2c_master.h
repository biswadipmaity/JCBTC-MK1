# define SDA RB0
# define SCL RB1
/*
//common i2c routines
void i2c_start(void);
void i2c_stop(void);
void i2c_high_sda(void);
void i2c_low_sda(void);
void i2c_high_scl(void);
void i2c_low_scl(void);
void i2c_nack(void);
void i2c_ack(void);
unsigned char i2c_in_byte(void);
void i2c_out_byte(unsigned char o_byte);
void delay_10us();
void delay_ms(long t);

void i2c_stop(void)
{
	i2c_low_scl();
	i2c_low_sda();
	i2c_high_scl();
	i2c_high_sda(); // bring SDA high while SCL is high
	// idle is SDA high and SCL high
}

void i2c_start(void)
{
	i2c_low_scl();
	i2c_high_sda();
	i2c_high_scl(); // bring SDA low while SCL is high
	i2c_low_sda();
	i2c_low_scl();
}


void i2c_low_scl(void)
{
	SCL = 0;
	 delay_10us(5);
}
void i2c_high_sda(void)
{
	// bring SDA to high impedance
	SDA = 1;
	delay_10us(5);
}

void i2c_high_scl(void)
{
	SCL = 1; // high impedance
	 delay_10us(5);
}
void i2c_low_sda(void)
{
	SDA= 0;// output a hard logic zero
	delay_10us(5);
}

void i2c_nack(void)
{
	i2c_high_sda(); // data at one
	i2c_high_scl(); // clock pulse
	i2c_low_scl();
}
void i2c_ack(void)
{
	i2c_low_sda(); // bring data low and clock
	i2c_high_scl();
	i2c_low_scl();
	i2c_high_sda();
}

void i2c_out_byte(unsigned char o_byte)
{
	unsigned char n;
	for(n=0; n<8; n++)
	{
		if(o_byte&0x80)
		{
			i2c_high_sda();
		}
		else
		{
			i2c_low_sda();
		}
		i2c_high_scl();
		i2c_low_scl();
		o_byte = o_byte << 1;
	}
	i2c_high_sda();
}
unsigned char i2c_in_byte(void)
{
	unsigned char i_byte,n;
	i2c_high_sda();
	for (n=0; n<8; n++)
	{
		i2c_high_scl();
		TRISB|=(0b00000001);
		if (SDA)
		{
			i_byte = (i_byte << 1) | 0x01; // msbit first
		}
		else
		{
			i_byte = i_byte << 1;
		}
		i2c_low_scl();
	}
	TRISB&=(0b11111110);
	return(i_byte);
}

void delay_10us()
{
	#asm
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	#endasm
}
void delay_ms(long t) // delays t millisecs
{
	do
	{
		delay_10us();
	} while(--t);
}
*/

#define I2CClock D’100000’ ; define I2C bite rate
#define ClockValue (((FOSC/I2CClock)/4) -1) ;

void init_i2c()
{
	SSPCON1=0b00101000;
	SSPADD=49;	//100kHz at 20MHz XTAL
	SSPEN=1;
	SSPSTAT=0b11000000;
}

void i2c_start()
{
	SEN=1; //Start Condition Enable
}

void i2c_write(unsigned char o_byte)
{
	SSPBUF=o_byte;
}

int i2c_test_write()
{
	if(ACKSTAT==1)//Not acknowledged
	{
		PEN=1;
		return -1;
	}
	return 1;
}

void i2c_start_read()
{
	RCEN=1;
	return;
}

unsigned char i2c_read()
{
	unsigned char tmp=SSPBUF;
	return tmp;
}

void i2c_read_ack()
{
	ACKDT=0;
	ACKEN=1;
	while(ACKEN);
	RCEN=1;
	return;
}

void i2c_read_nack()
{
	ACKDT=1;
	ACKEN=1;
	while(ACKEN);
	return;
}

void i2c_read_stop()
{
	SSPIE=0;
	PEN=1;
}
void idle_i2c()
{
	while(SSPSTAT & 0b00000100);
	while(SSPCON2 & 0x1F );
	return;
}