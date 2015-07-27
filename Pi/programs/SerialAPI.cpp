//RPi message format: PiDelimiter Length BYTE[Length] PiDelimiter
//BYTE[Length] format: CommandID CommandParameters[Length-1]

#include <wiringSerial.h>

#define PiDelimiter '^'
#define BYTE unsigned char

int fd;

//Motor functions
void setMotors(BYTE motors[8]) {
	serialPutchar(fd, (BYTE)PiDelimiter);
	serialPutchar(fd, 9);
	serialPutchar(fd, 10);
	BYTE i;
	for(i = 0; i < 8; ++i)
		serialPutchar(fd, motors[i]);
	serialPutchar(fd, PiDelimiter);
}
BYTE *getMotor(BYTE motorID) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)2);
	serialPutchar(fd, (BYTE)11);
	serialPutchar(fd, motorID);
	serialPutchar(fd, PiDelimiter);
	BYTE *array = new BYTE[2];
	array[0] = serialGetchar(fd);
	array[1] = serialGetchar(fd);
	return array;
}
void setMotor(BYTE motorID, BYTE direction, BYTE speed) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)4);
	serialPutchar(fd, (BYTE)12);
	serialPutchar(fd, motorID);
	serialPutchar(fd, direction);
	serialPutchar(fd, speed);
	serialPutchar(fd, PiDelimiter);
}

//Network functions
void getNetwork(void) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)1);
	serialPutchar(fd, (BYTE)20);
	serialPutchar(fd, PiDelimiter);
}

void networkMessage(BYTE destination, BYTE level, BYTE length, BYTE *message) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)(length + 3));
	serialPutchar(fd, destination);
	serialPutchar(fd, level);
	serialPutchar(fd, length);
	BYTE i;
	for(i = 0; i < length; ++i)
		serialPutchar(fd, message[i]);
	serialPutchar(fd, PiDelimiter);
}

//LCD functions
void LCD_clear(void) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)1);
	serialPutchar(fd, (BYTE)31);
	serialPutchar(fd, PiDelimiter);
}
void LCD_setCursor(BYTE row, BYTE column) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)3);
	serialPutchar(fd, (BYTE)32);
	serialPutchar(fd, row);
	serialPutchar(fd, column);
	serialPutchar(fd, PiDelimiter);
}
void LCD_putchar(BYTE character) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)2);
	serialPutchar(fd, (BYTE)33);
	serialPutchar(fd, character);
	serialPutchar(fd, PiDelimiter);
}
void LCD_print(BYTE *string) {
	BYTE i, length;
	for(i = length = 0; string[i] != '\0'; ++i)
		length++;
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)(length + 1));
	serialPutchar(fd, (BYTE)34);
	for(i = 0; i < length; ++i)
		serialPutchar(fd, string[i]);
	serialPutchar(fd, PiDelimiter);
}

//Camera functions
void getPan(void) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)1);
	serialPutchar(fd, (BYTE)41);
	serialPutchar(fd, PiDelimiter);
}
void getTilt(void) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)1);
	serialPutchar(fd, (BYTE)42);
	serialPutchar(fd, PiDelimiter);
}
void setPan(BYTE pan) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)2);
	serialPutchar(fd, (BYTE)43);
	serialPutchar(fd, pan);
	serialPutchar(fd, PiDelimiter);
}
void setTilt(BYTE tilt) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)2);
	serialPutchar(fd, (BYTE)44);
	serialPutchar(fd, tilt);
	serialPutchar(fd, PiDelimiter);
}

//Ultrasound functions
void ping(void) {
	serialPutchar(fd, PiDelimiter);
	serialPutchar(fd, (BYTE)1);
	serialPutchar(fd, (BYTE)50);
	serialPutchar(fd, PiDelimiter);
}

int initializeSerialAPI(char *port = "/dev/ttyAMA0", int baud = 115200) {
	return (fd = serialOpen(port, baud));
}
