//122 146 147

#define I2C_ADDR    0xa0
#define RX_Buffer_Len 32
#define TX_Buffer_Len 16

unsigned char RX_Buffer[RX_Buffer_Len];
unsigned char RX_Index;

unsigned char TX_Buffer[TX_Buffer_Len];
unsigned char TX_Index;

unsigned char receive_flag=0;
void i2c_slave_init()
{
	SSPCON=0b00110110;
	//Enables the serial port and configures the SDA and SCL pins as serial port pins
	//I2C Slave mode, 7-bit address
	
	SSPADD=I2C_ADDR;

	SSPIE=1;
	PEIE=1;
	GIE=1;
}
unsigned char read_data;
void interrupt ISR()
{
	unsigned char tmp;
	if((TMR1IF)&&(TMR1IE))
	{
		if(timer_count_current==timer_count_max)
		{
			timer=TIMER_OVERFLOW;
			timer_count_current=0;
		}
		else
			timer_count_current++;
		TMR1IF=0;
		
	}
	if((CCP1IE)&&(CCP1IF))
	{
		if(edge==0)
		{
			CCP1CON=0x04;	//Capture mode: every falling edge
			tOld=256*CCPR1H + CCPR1L;
			edge=1;
		}
		else
		{
			tNew=256*CCPR1H + CCPR1L;
			capture=1;
			edge=0;
		}
		CCP1IF=0;
		return;
	}
	else if((RCIF)&&(RCIE))
	{
		GIE=0;
		read_data=RCREG;
		receive_flag=1;
//sendDataMaster_hex(read_data);
//		queue_push(read_data);
		RCIF=0;
		GIE=1;
	}
	else if(SSPIF)
	{
		tmp=SSPSTAT & 0b00101101;
		GIE=0;
//	USARTWriteString("y0 ");
		if(tmp==0b00001001)			// Write operation, last byte was an address, buffer is full.
		{
//			USARTWriteString("y1\r");
			/*
				The master device on the bus has begun a new write
				operation by initiating a Start or Restart condition on the
				bus, then sending the slave I2C address byte. The LSb
				of the address byte is ‘0’ to indicate that the master
				wishes to write data to the slave.

				At this time, the SSP buffer is full and holds the previously
				sent address byte. The SSPBUF register must be
				read at this time to clear the BF bit, even if the address
				byte is to be discarded. If the SSPBUF is not read, the
				subsequent byte sent by the master will cause an SSP
				overflow to occur and the SSP module will NACK the
				byte.
			*/
			RX_Index=0;
			read_data=SSPBUF;	//Dummy Read
		}
		else if(tmp==0b00101001)	// Write operation, last byte was an data, buffer is full.
		{
//			USARTWriteString("y2\r");
			/*
				After the address byte is sent for an I2C write operation
				(State 1), the master may write one or more data bytes
				to the slave device. If SSPBUF was not full prior to the
				write, the slave node SSP module will generate an ACK
				pulse on the 9th clock edge. Otherwise, the SSPOV bit
				will be set and the SSP module will NACK the byte.
			*/

			RX_Buffer[RX_Index]=SSPBUF;
			RX_Index++;
			RX_Index=RX_Index%RX_Buffer_Len;
		}
		else if((tmp&0b00101100)==0b00001100)	// Read operation, last byte was an address,
		{										//Mask BF bit in SSPSTAT (check not required)
//			USARTWriteString("y3\r");
			/*
				The master device on the bus has begun a new read
				operation by initiating a Start or a Restart condition on
				the bus, then sending the slave I2C address byte. The
				LSb of the address byte is ‘1’ to indicate that the master
				wishes to read data from the slave.

				At this time, the SSP buffer is ready to be loaded with
				data to be sent to the master. The CKP bit is also
				cleared to hold the SCL line low. The slave data is sent
				to the master by loading SSPBUF and then setting the
				CKP bit to release the SCL line.
			*/

			TX_Index=0;
			read_data=queue_pop();
			while(BF);
	
			do
			{
				WCOL=0;
				//SSPBUF='a';
				SSPBUF=read_data;
//				USARTWriteByte(read_data);
			}
			while(WCOL);
			//USARTWriteByte(read_data);
			CKP=1;
		}
		else if(tmp==0b00101100)	// Read operation, last byte was an data, buffer is empty
		{
//			USARTWriteString("y4\r");
			/*
				This occurs each time the master has previously
				read a byte of data from the slave and wishes to read
				another data byte.
			*/
			TX_Index++;
			TX_Index=TX_Index%TX_Buffer_Len;
			
			read_data=queue_pop();
			while(BF);
			do
			{
				WCOL=0;
				//SSPBUF='b';
				SSPBUF=read_data;
//				if(read_data!='$')
//					USARTWriteByte(read_data);
			}
			while(WCOL);
			CKP=1;
		}
		else if((tmp&0b00101000)==0b00101000)	//NACK received when sending data to the master
		{										//Mask RW bit in SSPSTAT.
//			USARTWriteString("y5\r");
			/*
				This occurs when the master has sent a NACK in
				response to data that has been received from the slave
				device. This action indicates that the master does not
				wish to read further bytes from the slave. The NACK
				signals the end of the I2C message and has the effect
				of resetting the slave I2C logic.
			
				The NACK event is identified because the CKP bit
				remains set. Specifically, the status bits indicate that a
				data byte has been received from the master and the
				buffer is empty.
			*/
		}
		else	//Something went wrong! 
		{
			USARTWriteString("y6\r");
		}
		SSPIF=0;
		GIE=1;
	}

}