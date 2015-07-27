#define QUEUE_SIZE 32
char queue[QUEUE_SIZE];

int queue_index_start;
int queue_index_end;
int queue_size;

void init_queue()
{
	queue_size=0;
	queue_index_end=-1;
	queue_index_start=0;
}

void queue_push(char c)
{

	if(queue_size==QUEUE_SIZE)
		return;
	queue_index_end++;
	queue_index_end%=QUEUE_SIZE;
		queue[queue_index_end]=c;

	queue_size++;
//	for(c=0;c<QUEUE_SIZE;c++)
//	{		
//		USARTWriteInt(queue[c],3);
//		USARTWriteString(" ");
//	}
//	USARTWriteString("\n\t");
}
char queue_pop()
{
	char c;
	if(queue_size==0)
		return '$';

	c=queue[queue_index_start++];
	queue_index_start%=QUEUE_SIZE;
	queue_size--;
	return c;
}

void push_hex(unsigned char tmp)
{
	unsigned char dig=tmp>>4;
	if(dig<10)
 		queue_push('0'+dig);
	else
		queue_push('a'+dig-10);

	dig=tmp%16;
	if(dig<10)
 		queue_push('0'+dig);
	else
		queue_push('a'+dig-10);

}

void print_packet(unsigned char *tmp,int length)
{
	int i;
	for(i=0;i<length;i++)
		push_hex(*(tmp+i));
}

void print_string(const char *str)
{
	while((*str)!='\0')
	{
//		queue_push(*str);
		//Next goto char
		str++;
	}
}