/*
#define M1A RA1
#define M1B RA2

#define M2A	RA3
#define M2B	RA4

#define M3A	RA5
#define M3B	RE0

#define M4A	RE2
#define M4B	RE1
*/


#define M1A RE1
#define M1B RE2

#define M2A	RA3
#define M2B	RA4

#define M3A	RA5
#define M3B	RE0

#define M4A	RA2
#define M4B	RA1

bit M1A_mem;
bit M1B_mem; 
bit M2A_mem;
bit M2B_mem;
bit M3A_mem;
bit M3B_mem;
bit M4A_mem;
bit M4B_mem;

enum wheel{
	forward,
	backward,
	left_rotate,
	right_rotate,
wheel_test,
	off
}wheel_state;


void update_wheel()
{
	M1A=M1A_mem;
	M1B=M1B_mem;
	
	M2A=M2A_mem;
	M2B=M2B_mem;
	
	M3A=M3A_mem;
	M3B=M3B_mem;
	
	M4A=M4A_mem;
	M4B=M4B_mem;
/*
	switch(wheel_state)
	{
		case forward:
			M1A=1;
			M1B=0;
		
			M2A=1;
			M2B=0;
		
			M3A=1;
			M3B=0;
		
			M4A=1;
			M4B=0;
		break;

case wheel_test:
			M1A=1;
			M1B=0;
		
			M2B=1;
			M2A=0;
		
			M3A=1;
			M3B=0;
		
			M4A=0;
			M4B=1;
break;

		case backward:
			M1B=1;
			M1A=0;
		
			M2B=1;
			M2A=0;
		
			M3A=0;
			M3B=1;
		
			M4A=0;
			M4B=1;
		break;

		case left_rotate:	
			M1A=1;
			M1B=0;
	
			M2B=1;
			M2A=0;
	
			M3A=1;
			M3B=0;
		
			M4A=0;
			M4B=1;
		break;

		case right_rotate:
			M1A=0;
			M1B=1;
		
			M2A=1;
			M2B=0;
		
			M3A=0;
			M3B=1;
		
			M4A=1;
			M4B=0;
		break;

		case off:
			M1A=1;
			M1B=1;
		
			M2A=1;
			M2B=1;
		
			M3A=1;
			M3B=1;
		
			M4A=1;
			M4B=1;
		break;
	}
*/
}

void complement()
{
	M1A=!M1A;
	M1B=!M1B;

	M2B=!M2B;
	M2A=!M2A;

	M3A=!M3A;
	M3B=!M3B;

	M4A=!M4A;
	M4B=!M4B;	
}
