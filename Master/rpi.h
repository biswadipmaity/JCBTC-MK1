#define rpi_buffer_length 16

int rpi_buffer_start=0;
int rpi_buffer_end=0;
int rpi_buffer[rpi_buffer_length];

void rpi_buffer_push(char c)
{
	rpi_buffer[rpi_buffer_end]=c;
	rpi_buffer_end++;
	rpi_buffer_end%=rpi_buffer_length;
}

unsigned char rpi_buffer_empty()
{
	if(rpi_buffer_start==rpi_buffer_end)
		return 1;

	else
		return 0;
}

unsigned char rpi_buffer_pop()
{
	unsigned char tmp=rpi_buffer[rpi_buffer_start];

	if(rpi_buffer_start!=rpi_buffer_end)
	{
		rpi_buffer_start++;
		rpi_buffer_start%=rpi_buffer_length;
	}

	return tmp;
}
