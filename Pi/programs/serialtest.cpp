#include "SerialAPI.cpp"
#include <stdio.h>

int main(void)
{
	initializeSerialAPI();
	char c=0;
	BYTE state[8]={0, 0, 0, 0, 0, 0, 0, 0};
	BYTE length, i, *array = new BYTE[20], *response;
	while(c!=27)
	{
		scanf("%c", &c);
		switch(c)
		{
/*			case 'n':
				getPan();
			break;
			case 'N':
				setPan(200);
			break;
			case 't':
				getTilt();
			break;
			case 'T':
				setTilt(200);
			break;
			case 'c':
				LCD_clear();
			break;
			case 'S':
				LCD_setCursor(2, 1);
			break;
			case 'h':
				LCD_putchar(65);
			break;
*/			case 'v':
				array[0] = 'a';
				array[1] = 'b';
				array[2] = '1';
				array[3] = '2';
				array[4] = ' ';
				array[5] = '!';
				array[6] = '\0';
/*				serialPutchar(fd, PiDelimiter);
				serialPutchar(fd, (BYTE)(length + 1));
				serialPutchar(fd, (BYTE)34);
				for(i = 0; i < length; ++i)
					serialPutchar(fd, string[i]);
				serialPutchar(fd, PiDelimiter);
*/				LCD_print(array);
			break;
			case 'r':
				response = getMotor(1);
				printf("motor state: %d %d\n", response[0], reponse[1]);
				delete response;
			break;
			case 'u':
				response = getMotor(2);
				printf("motor state: %d %d\n", response[0], reponse[1]);
				delete response;
			break;
			case 'f':
				response = getMotor(3);
				printf("motor state: %d %d\n", response[0], reponse[1]);
				delete response;
			break;
			case 'j':
				response = getMotor(4);
				printf("motor state: %d %d\n", response[0], reponse[1]);
				delete response;
			break;
			case 'q':
				state[0]=0;
				state[1]=200;
				setMotors(state);
			break;
			case 'w':
				state[0]=0;
				state[1]=0;
				setMotors(state);
			break;
			case 'e':
				state[0]=1;
				state[1]=200;
				setMotors(state);
			break;
			case 'i':
				state[2]=0;
				state[3]=200;
				setMotors(state);
			break;
			case 'o':
				state[2]=0;
				state[3]=0;
				setMotors(state);
			break;
			case 'p':
				state[2]=1;
				state[3]=200;
				setMotors(state);
			break;
			case 'a':
				state[4]=0;
				state[5]=200;
				setMotors(state);
			break;
			case 's':
				state[4]=0;
				state[5]=0;
				setMotors(state);
			break;
			case 'd':
				state[4]=1;
				state[5]=200;
				setMotors(state);
			break;
			case 'k':
				state[6]=0;
				state[7]=200;
				setMotors(state);
			break;
			case 'l':
				state[6]=0;
				state[7]=0;
				setMotors(state);
			break;
			case ';':
				state[6]=1;
				state[7]=200;
				setMotors(state);
			break;
/*			case 'z':
				ping();
			break;
*/		}
	}
	return 0;
}
