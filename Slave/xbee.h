//XBEE firmware constants
#define COORDINATOR 0
#define ROUTER 1

//defining XBEE firmware
#define TYPE COORDINATOR

//defining XBEE self address
#define SELF {0x00, 0x13, 0xA2, 0x00, 0x40, 0xA9, 0xE2, 0xAA}

/*
Jadavpur University XBEE Protocol::

	length::		10byte

	structure::
		command:	1byte
		parameters:	9bytes

commands::

	0x00::		reset
		usage:	coordinator broadcast on coordiantor powerup
		param:	8byte self XBee address, 1byte self networkID 0
		prerq:	coordinator powerup
		subsq:	none
	note:		routers must join new network

	0x01::		discover
		usage:	router broadcast on powerup
		param:	8byte self XBee address, 1byte request networkID or 0
		prerq:	power on
		subsq:	wait for coordinator response until timeout

	0x02::		offer
		usage:	coordinator unicast to router on discovery after polling
		param:	8byte self XBee address, 1byte networkID offered
		prerq:	receive discovery request
		subsq:	wait for router response until timeout
	note:		coordinator must offer lowest available networkID

	0x03::		request
		usage:	router unicast to coordinator on receiving offer
		param:	8byte self XBee address, 1byte networkID accepted
		prerq:	receive networkID offer
		subsq:	wait for coordinator response until timeout

	0x04::		acknowledge
		usage:	coordinator unicast to router on request
		param:	8byte XBee address, 1byte networkID of each XBees on network
		prerq:	receive networkID request
		subsq:	issue up command to all routers
	note:		number of acknowledge commands unicast to joining router = XBees on network

	0x05::		join
		usage:	router unicast to coordinator on joining network
		param:	8byte self XBee address, 1byte self networkID
		prerq:	receive acknowledgement
		subsq:	none

	0x06::		up
		usage:	coordinator broadcast on router join
		param:	8byte router XBee address, 1byte networkID joined
		prerq:	acknowledgement of joining
		subsq:	none
	note:		routers must update network database

	0x07::		down
		usage:	coordinator broadcast on router leave
		param:	8byte router XBee address, 1byte networkID left
		prerq:	router in database does not respond to poll
		subsq:	none
	note:		routers must update network database

	0x08::		poll request
		usage:	coordinator broadcast on router discovery
		param:	9byte 0x00
		prerq:	receive of discovery request
		subsq:	wait until timeout
	note:		routers must respond

	0x09::		poll response
		usage:	router unicast to coordinator on poll request
		param:	8byte self XBee address, 1byte self networkID
		prerq:	receive of poll request
		subsq:	none

	0x10::		movement request
		usage:	router unicast to coordinator to request movement of other router
		param:	1byte self requesterID, 1byte requested networkID, 1byte direction, 1byte speed or 0, 1byte time or 0, 4byte 0
		prerq:	computation
		subsq:	none

	0x11::		movement command
		usage:	coordinator unicast to router for movement
		param:	1byte self requesterID, 1byte requested networkID, 1byte direction, 1byte speed or 0, 1byte time or 0, 4byte 0
		prerq:	computation or receive of movement request
		subsq:	wait for router response until timeout

	0x12::		movement acknowledge
		usage:	router unicast to coordinator to confirm movement
		param:	1byte self requesterID, 1byte requested networkID, 1byte direction, 1byte speed or 0, 1byte time or 0, 4byte 0
		prerq:	receive of movement command
		subsq:	movement

	0x13::		movement response
		usage:	coordinator unicast to router to confirm movement of other router
		param:	1byte self requesterID, 1byte requested networkID, 1byte direction, 1byte speed or 0, 1byte time or 0, 4byte 0
		prerq:	receive of movement acknowledgement
		subsq:	none

	0xFE::		abandon
		usage:	unicast on abandoning command or request
		param:	1byte command or request being abandoned, 1byte requester networkID, 1byte requested networkID, 6byte 0
		prerq:	processed mismatch or timeout
		subsq:	none

	0xFF::		abort
		usage:	unicast on aborting network configuration
		param:	8byte self address, 1byte offeredID
		prerq:	processed network configuration mismatch or timeout
		subsq:	none
*/

//JUXP IDs
#define JUXP_RESET 0x00
#define JUXP_DISCOVER 0x01
#define JUXP_OFFER 0x02
#define JUXP_REQUEST 0x03
#define JUXP_ACKNOWLEDGE 0x04
#define JUXP_JOIN 0x05
#define JUXP_UP 0x06
#define JUXP_DOWN 0x07
#define JUXP_POLL_REQUEST 0x08
#define JUXP_POLL_RESPONSE 0x09
#define JUXP_MOVEMENT_REQUEST 0x10
#define JUXP_MOVEMENT_COMMAND 0x11
#define JUXP_MOVEMENT_ACKNOWLEDGE 0x12
#define JUXP_MOVEMENT_RESPONSE 0x13
#define JUXP_REPORT_MAP 0x20
#define JUXP_REPORT_SELF 0x21
#define JUXP_ABANDON 0xFE
#define JUXP_ABORT 0xFF

//constant field sizes
#define JUXP_LENGTH 0x0A
#define PACKET_LENGTH 0x1C
#define ADDRESS_LENGTH 0x08
#define DATABASE_LENGTH 0x05

//constant indices
#define INDEX_START 0x00
#define INDEX_LENGTH 0x01
#define INDEX_API 0x03
#define INDEX_ID 0x04
#define INDEX_ADDRESS 0x05
#define INDEX_NETWORK 0x0D
#define INDEX_RADIUS 0x0F
#define INDEX_OPTION 0x10
#define INDEX_JUXP 0x11
#define INDEX_CHECKSUM 0x1B

//constant fields
#define START 0x7E
#define LENGTH_UP 0x00
#define LENGTH_DOWN 0x18
#define API 0x10
#define ID 0x00
#define NETWORK_UP 0xFF
#define NETWORK_DOWN 0xFE
#define RADIUS 0x00
#define OPTION 0x00

//field constants
#define INITIALIZE_CONSTANT 0x00
#define INVALIDATE_CONSTANT 0xFF

#define SEND_RPi_DELIMITER '^'
#define SEND_MASTER_DELIMITER '@'

typedef unsigned char BYTE;

#define DATA_BUFFER_SIZE 48

//JUXP packet
BYTE packet_out[PACKET_LENGTH];
//<seminar>
//BYTE packet_in[JUXP_LENGTH];
#define JUXP_DATA 0x30
#define DATA_LEVEL_MASTER 0x00
#define DATA_LEVEL_PI 0x01
BYTE packet_in[DATA_BUFFER_SIZE];
//</seminar>

//configuration variables
BYTE broadcast[ADDRESS_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF};
BYTE major[ADDRESS_LENGTH] = SELF;
BYTE minor[ADDRESS_LENGTH];
BYTE requestedID;
BYTE offeredID;
BYTE networkID;

//network database
BYTE database[DATABASE_LENGTH][ADDRESS_LENGTH];

#if TYPE == COORDINATOR

//network poll database
BYTE pollbase[DATABASE_LENGTH][ADDRESS_LENGTH];

#endif

//movement constants
#define MOVE_STOP 'x'
#define MOVE_FORWARD 'w'
#define MOVE_BACKWARD 's'
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'


void master_lcd_print_putchar(BYTE x) 
{
	queue_push(SEND_MASTER_DELIMITER);
	queue_push(2);
	queue_push(33);
	queue_push(x);
	queue_push(SEND_MASTER_DELIMITER);
}

void master_lcd_print_hex(BYTE tmp) 
{
	unsigned char dig=tmp>>4;
	if(dig<10)
 		master_lcd_print_putchar('0'+dig);
	else
		master_lcd_print_putchar('a'+dig-10);

	dig=tmp%16;
	if(dig<10)
 		master_lcd_print_putchar('0'+dig);
	else
		master_lcd_print_putchar('a'+dig-10);
}

void sendDataMaster(BYTE *string,int length)
{
//<seminar>
//	if(string[0]!=10)
//		return;
//</seminar>
	queue_push(SEND_MASTER_DELIMITER);
	queue_push(length);

	BYTE i=0;
	for(i=0;i<length;i++)
	{
			queue_push(string[i]);
	}
	queue_push(SEND_MASTER_DELIMITER);
}

void master_lcd_print(BYTE *string) {

	queue_push(SEND_MASTER_DELIMITER);
	int length=0;
	while(*(string+length) != '\0')
		length++;
	queue_push(length);
	queue_push(34);

	BYTE i=0;
	while(*(string+i) != '\0')
	{
		queue_push(string[i]);
		i++;
	}
	queue_push(SEND_MASTER_DELIMITER);
}

void sendDataRPi(BYTE *string, BYTE length) {
	queue_push(SEND_RPi_DELIMITER);
	queue_push(length);
	BYTE i;
	for(i = 0; i < length; ++i)
		queue_push(string[i]);
	queue_push(SEND_RPi_DELIMITER);
}


//timer variable
BYTE timer;

//timer state constants
#define TIMER_SET 0xFF
#define TIMER_UNSET 0x00
#define TIMER_OVERFLOW 0xAA

void init_timer()
{
	T1CON=0b00110000;
}

int timer_count_current;
int timer_count_max;
void START_TIMER(BYTE n)
{
	TMR1ON=1;	//Enable Timer
	TMR1IE=1;	//Enable Interrupt
	TMR1IF=0;	//Clear timer Flag
	PEIE=1;
	GIE=1;

	TMR1L=0;
	TMR1H=0;

	//Set Time
	timer_count_current=0;
	timer_count_max=n*2;
	timer=TIMER_SET;
}

void STOP_TIMER()
{
	TMR1ON=0;	//Stop Timer
	TMR1IE=0;	//Interrupt stop
	TMR1IF=0;	//Clear flag
	timer=TIMER_UNSET;
} 

/*
//microcontroller functions
void START_TIMER(BYTE);
void STOP_TIMER(void);
void USARTWriteByte(BYTE);
*/

//JUXN states
enum {
	NETWORKING,
	TIMEKILLING,
	DISCOVERING,
	POLLING,
	OFFERING,
	REQUESTING,
	ACKNOWLEDGING,
	MOVING
} state;

//initializes constant packet_out fields and JUXN state
void INITIALIZE_PACKET_OUT(void) {
	BYTE i;
	for(i = 0; i < PACKET_LENGTH; ++i)
		packet_out[i] = INITIALIZE_CONSTANT;
	packet_out[INDEX_START] = START;
	packet_out[INDEX_LENGTH] = LENGTH_UP;
	packet_out[INDEX_LENGTH+1] = LENGTH_DOWN;
	packet_out[INDEX_API] = API;
	packet_out[INDEX_ID] = ID;
	packet_out[INDEX_NETWORK] = NETWORK_UP;
	packet_out[INDEX_NETWORK+1] = NETWORK_DOWN;
	packet_out[INDEX_RADIUS] = RADIUS;
	packet_out[INDEX_OPTION] = OPTION;
}

void INITIALIZE_PACKET_IN(void) {
	BYTE i;
	for(i = 0; i < JUXP_LENGTH; ++i)
		packet_in[i] = INITIALIZE_CONSTANT;
}

//checksums JUXP packet
void CHECKSUM(void) {
	BYTE i, checksum = 0;
	for(i = INDEX_API; i < INDEX_CHECKSUM; ++i)
		checksum += packet_out[i];
	checksum = 0xFF - checksum;
	packet_out[INDEX_CHECKSUM] = checksum;
}

//sets destination
void ADDRESS(BYTE *address) {
	BYTE i;
	for(i = 0; i < ADDRESS_LENGTH; ++i)
		packet_out[INDEX_ADDRESS+i] = address[i];
}

//sends JUXP packet
void SEND(void) {
	BYTE i;
	for(i = 0; i < PACKET_LENGTH; ++i) {
		USARTWriteByte(packet_out[i]);
	}
}

#if TYPE == COORINATOR

//JUXP 0x00
void RESET(void) {
	ADDRESS(broadcast);
	packet_out[INDEX_JUXP] = JUXP_RESET;
	BYTE i;
	for(i = INDEX_JUXP+1; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = major[i-(INDEX_JUXP+1)];
	packet_out[INDEX_CHECKSUM-1] = networkID;
	CHECKSUM();
	SEND();
}

#endif

#if TYPE == ROUTER

//JUXP 0x01
void DISCOVER(void) {
	ADDRESS(broadcast);
	packet_out[INDEX_JUXP] = JUXP_DISCOVER;
	BYTE i;
	for(i = INDEX_JUXP+1; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = major[i-(INDEX_JUXP+1)];
	packet_out[INDEX_CHECKSUM-1] = requestedID;
	CHECKSUM();
	SEND();
}

#endif

#if TYPE == COORDIANTOR

//JUXP 0x02
void OFFER(void) {
	ADDRESS(minor);
	packet_out[INDEX_JUXP] = JUXP_OFFER;
	BYTE i;
	for(i = INDEX_JUXP+1; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = major[i-(INDEX_JUXP+1)];
	packet_out[INDEX_CHECKSUM-1] = offeredID;
	CHECKSUM();
	SEND();
}

#endif

#if TYPE == ROUTER

//JUXP 0x03
void REQUEST(void) {
	ADDRESS(minor);
	packet_out[INDEX_JUXP] = JUXP_REQUEST;
	BYTE i;
	for(i = INDEX_JUXP+1; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = major[i-(INDEX_JUXP+1)];
	packet_out[INDEX_CHECKSUM-1] = networkID;
	CHECKSUM();
	SEND();
}

#endif

#if TYPE == COORINATOR

//JUXP 0x04
void ACKNOWLEDGE(void) {
	ADDRESS(minor);
	packet_out[INDEX_JUXP] = JUXP_ACKNOWLEDGE;
	BYTE i, j;
	for(i = 0; i < DATABASE_LENGTH; ++i) {
		for(j = INDEX_JUXP+1; j+1 < INDEX_CHECKSUM; ++j)
			packet_out[j] = database[i][j-(INDEX_JUXP+1)];
		packet_out[INDEX_CHECKSUM-1] = i+1;
		CHECKSUM();
		SEND();
	}
}

#endif

#if TYPE == ROUTER

//JUXP 0x05
void JOIN(void) {
	ADDRESS(minor);
	packet_out[INDEX_JUXP] = JUXP_JOIN;
	BYTE i;
	for(i = INDEX_JUXP+1; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = major[i-(INDEX_JUXP+1)];
	packet_out[INDEX_CHECKSUM-1] = networkID;
	CHECKSUM();
	SEND();
}

#endif

#if TYPE == COORINATOR

//JUXP 0x06
void UP(void) {
	ADDRESS(broadcast);
	packet_out[INDEX_JUXP] = JUXP_UP;
	BYTE i;
	for(i = INDEX_JUXP+1; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = minor[i-(INDEX_JUXP+1)];
	packet_out[INDEX_CHECKSUM-1] = offeredID;
	CHECKSUM();
	SEND();
}

//JUXP 0x07
void DOWN(BYTE *address, BYTE netID) {
	ADDRESS(broadcast);
	packet_out[INDEX_JUXP] = JUXP_DOWN;
	BYTE i;
	for(i = INDEX_JUXP+1; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = address[i-(INDEX_JUXP+1)];
	packet_out[INDEX_CHECKSUM-1] = netID;
	CHECKSUM();
	SEND();
}

//JUXP 0x08
void POLL_REQUEST(void) {
	ADDRESS(broadcast);
	packet_out[INDEX_JUXP] = JUXP_POLL_REQUEST;
	BYTE i;
	for(i = INDEX_JUXP+1; i < INDEX_CHECKSUM; ++i)
		packet_out[i] = INITIALIZE_CONSTANT;
	CHECKSUM();
	SEND();
}

#endif

#if TYPE == ROUTER

//JUXP 0x09
void POLL_RESPONSE(void) {
	ADDRESS(minor);
	packet_out[INDEX_JUXP] = JUXP_POLL_RESPONSE;
	BYTE i;
	for(i = INDEX_JUXP+1; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = major[i-(INDEX_JUXP+1)];
	packet_out[INDEX_CHECKSUM-1] = networkID;
	CHECKSUM();
	SEND();
}

#endif
/*
#if TYPE == ROUTER

//JUXP 0x10
void MOVEMENT_REQUEST(BYTE requestedID, BYTE direction, BYTE speed, BYTE time) {
	ADDRESS(minor);
	packet_out[INDEX_JUXP] = JUXP_MOVEMENT_REQUEST;
	packet_out[INDEX_JUXP+1] = networkID;
	packet_out[INDEX_JUXP+2] = requestedID;
	packet_out[INDEX_JUXP+3] = direction;
	packet_out[INDEX_JUXP+4] = speed;
	packet_out[INDEX_JUXP+5] = time;
	BYTE i;
	for(i = INDEX_JUXP+6; i < INDEX_CHECKSUM; ++i)
		packet_out[i] = INITIALIZE_CONSTANT;
	CHECKSUM();
	SEND();
}

#endif

#if TYPE == COORDINATOR

//JUXP 0x11
void MOVEMENT_COMMAND(BYTE requesterID, BYTE requestedID, BYTE direction, BYTE speed, BYTE time) {
	ADDRESS(database[requestedID-1]);
	packet_out[INDEX_JUXP] = JUXP_MOVEMENT_COMMAND;
	packet_out[INDEX_JUXP+1] = requesterID;
	packet_out[INDEX_JUXP+2] = requestedID;
	packet_out[INDEX_JUXP+3] = direction;
	packet_out[INDEX_JUXP+4] = speed;
	packet_out[INDEX_JUXP+5] = time;
	BYTE i;
	for(i = INDEX_JUXP+6; i < INDEX_CHECKSUM; ++i)
		packet_out[i] = INITIALIZE_CONSTANT;
	CHECKSUM();
	SEND();
}

#endif

#if TYPE == ROUTER

//JUXP 0x12
void MOVEMENT_ACKNOWLEDGE(BYTE requesterID, BYTE direction, BYTE speed, BYTE time) {
	ADDRESS(minor);
	packet_out[INDEX_JUXP] = JUXP_MOVEMENT_ACKNOWLEDGE;
	packet_out[INDEX_JUXP+1] = requesterID;
	packet_out[INDEX_JUXP+2] = networkID;
	packet_out[INDEX_JUXP+3] = direction;
	packet_out[INDEX_JUXP+4] = speed;
	packet_out[INDEX_JUXP+5] = time;
	BYTE i;
	for(i = INDEX_JUXP+6; i < INDEX_CHECKSUM; ++i)
		packet_out[i] = INITIALIZE_CONSTANT;
	CHECKSUM();
	SEND();
}

#endif

#if TYPE == COORDINATOR

//JUXP 0x13
void MOVEMENT_RESPONSE(BYTE requesterID, BYTE requestedID, BYTE direction, BYTE speed, BYTE time) {
	ADDRESS(database[requesterID-1]);
	packet_out[INDEX_JUXP] = JUXP_MOVEMENT_RESPONSE;
	packet_out[INDEX_JUXP+1] = requesterID;
	packet_out[INDEX_JUXP+2] = requestedID;
	packet_out[INDEX_JUXP+3] = direction;
	packet_out[INDEX_JUXP+4] = speed;
	packet_out[INDEX_JUXP+5] = time;
	BYTE i;
	for(i = INDEX_JUXP+6; i < INDEX_CHECKSUM; ++i)
		packet_out[i] = INITIALIZE_CONSTANT;
	CHECKSUM();
	SEND();
}

#endif

//JUXP 0x20
void REPORT_MAP(BYTE pointx, BYTE pointy, BYTE pointz, BYTE weight, BYTE time) {
	ADDRESS(broadcast);
	packet_out[INDEX_JUXP] = JUXP_REPORT_MAP;
	packet_out[INDEX_JUXP+1] = networkID;
	packet_out[INDEX_JUXP+2] = pointx;
	packet_out[INDEX_JUXP+3] = pointy;
	packet_out[INDEX_JUXP+4] = pointz;
	packet_out[INDEX_JUXP+5] = weight;
	packet_out[INDEX_JUXP+6] = time;
	BYTE i;
	for(i = INDEX_JUXP+7; i < INDEX_CHECKSUM; ++i)
		packet_out[i] = INITIALIZE_CONSTANT;
	CHECKSUM();
	SEND();
}

//JUXP 0x21
void REPORT_SELF(BYTE locatex, BYTE locatey, BYTE locatez, BYTE orientx, BYTE orienty, BYTE orientz, BYTE velocity) {
	ADDRESS(broadcast);
	packet_out[INDEX_JUXP] = JUXP_REPORT_MAP;
	packet_out[INDEX_JUXP+1] = networkID;
	packet_out[INDEX_JUXP+2] = locatex;
	packet_out[INDEX_JUXP+3] = locatey;
	packet_out[INDEX_JUXP+4] = locatez;
	packet_out[INDEX_JUXP+5] = orientx;
	packet_out[INDEX_JUXP+6] = orienty;
	packet_out[INDEX_JUXP+7] = orientz;
	packet_out[INDEX_JUXP+8] = velocity;
	packet_out[INDEX_JUXP+9] = INITIALIZE_CONSTANT;
	CHECKSUM();
	SEND();
}
*/
//JUXP 0xFE
void ABANDON(BYTE destinationID, BYTE protocol, BYTE requesterID, BYTE requestedID) {
	ADDRESS(database[destinationID-1]);
	packet_out[INDEX_JUXP] = JUXP_ABANDON;
	packet_out[INDEX_JUXP+1] = protocol;
	packet_out[INDEX_JUXP+2] = requesterID;
	packet_out[INDEX_JUXP+3] = requestedID;
	BYTE i;
	for(i = INDEX_JUXP+4; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = INITIALIZE_CONSTANT;
	CHECKSUM();
	SEND();
}

//JUXP 0xFF
void ABORT(void) {
	ADDRESS(minor);
	packet_out[INDEX_JUXP] = JUXP_ABORT;
	BYTE i;
	for(i = INDEX_JUXP+1; i+1 < INDEX_CHECKSUM; ++i)
		packet_out[i] = major[i-(INDEX_JUXP+1)];
	packet_out[INDEX_CHECKSUM-1] = offeredID;
	CHECKSUM();
	SEND();
}

//forward declaration
void STATE(void);

//initializes network configuration
void INITIALIZE_NETWORK(void) {
	INITIALIZE_PACKET_IN();
	INITIALIZE_PACKET_OUT();
	state = NETWORKING;
	START_TIMER(1);
//	STATE();
}

/*
//prints network database
void PRINT_DATABASE(void) {
	int i, j, match;
	char string[2];
	string[1] = '\0';
	for(i = 0; i < DATABASE_LENGTH; ++i) {
		match = 0;
		for(j = 0; j < ADDRESS_LENGTH; ++j)
			match |= database[i][j];
		if(match) {
			string[0] = i + '0';
			master_lcd_print(string);
		}
	}
}
*/
#if TYPE == COORDINATOR

/*

Jadavpur University XBEE Network::

coordinator states::

	networking::
		usage:	when not registered on a network
		trggr:	state:	off
				actns:	powerup
		rspns:	state:	timekilling
				actns:	reset network

	timekilling::
		usage:	when not doing anything else
		rspns:	state:	discovering
				actns:	on router discovery
		rspns:	state:	moving
				actns:	on router move request
	note: central state

	discovering::
		usage:	when discovering router
		trggr:	state:	timekilling
				actns:	on router discovery
		rspns:	state:	polling
				actns:	poll network routers

	polling::
		usage:	when polling network routers
		trggr:	state:	discovering
				actns:	poll network routers
		rspns:	state:	polling
				actns:	receive poll
				state:	timekilling
				actns:	abort on polled network full
				state:	timekilling
				actns:	on router abort
				state:	offering
				actns:	offer router on timeout

	offering::
		usage:	when offering router networkID
		trggr:	state:	polling
				actns:	offer router on timeout
		rspns:	state:	timekilling
				actns:	on router abort
				state:	timekilling
				actns:	abort joining router on timeout
				state:	requesting
				actns:	joining router requests

	requesting::
		usage:	when requested by joining router
		trggr:	state:	offering
				actns:	joining router requests
		rspns:	state:	timekilling
				actns:	abort router on requesting networkID not offered
				state:	acknowledging
				actns:	transmitting acknowledgement

	acknowledging::
		usage:	when acknowledging router join
		trggr:	state:	requesting
				actns:	transmitting acknowledgement
		rspns:	state:	timekilling
				actns:	on router abort
				state:	timekilling
				actns:	abort joining router on timeout
				state:	timekilling
				actns:	add joined router to network

	moving::
		usage:	when router movement is requested
		trggr:	state:	timekilling
				actns:	on router movement request
		rspns:	state:	timekilling
				actns:	abandon on timeout
				state:	timekilling
				actns:	router movement response on router moovement acknowledgement

*/

//coordinator network event handling function for state networking
void NETWORKER(void) {
//	master_lcd_print(" <T", 3);
//	STOP_TIMER();
	BYTE i, j;
	for(i = 0; i < DATABASE_LENGTH; ++i)
		for(j = 0; j < ADDRESS_LENGTH; ++j)
			database[i][j] = INITIALIZE_CONSTANT;
	for(i = 0; i < ADDRESS_LENGTH; ++i)
		minor[i] = INITIALIZE_CONSTANT;
	networkID = INITIALIZE_CONSTANT;
	requestedID = INITIALIZE_CONSTANT;
	offeredID = INITIALIZE_CONSTANT;
	RESET();
	//master_lcd_print(" >R", 3);
	state = TIMEKILLING;
}

//coordinator network event handling function for state timekilling
void TIMEKILLER(void) {
	BYTE i, length;
	switch(packet_in[0]) {
		case JUXP_DISCOVER:
			//master_lcd_print(" <D", 3);
			for(i = 0; i < ADDRESS_LENGTH; ++i)
				minor[i] = packet_in[i+1];
			requestedID = packet_in[JUXP_LENGTH-1];
			offeredID = INITIALIZE_CONSTANT;
			state = DISCOVERING;
			START_TIMER(1);
//			STATE();
		break;/*
		case JUXP_MOVEMENT_REQUEST:
			//master_lcd_print(" <MQ", 4);
			offeredID = packet_in[1];
			requestedID = packet_in[2];
			if(requestedID) {
				MOVEMENT_COMMAND(requestedID, offeredID, packet_in[3], packet_in[4], packet_in[5]);
				//master_lcd_print(" >MC", 4);
				START_TIMER(10);
				state = MOVING;
			}	//handle coordinator movement requests later
		break; */
		case JUXP_DATA:
			length = packet_in[4];
			switch(packet_in[3]) {
				case DATA_LEVEL_MASTER:
					//5 is the starting of frame to send
					sendDataMaster(packet_in + 5, length);
				break;
				case DATA_LEVEL_PI:
					sendDataRPi(packet_in + 5, length);
				break;
			}
		break;
	}
}

//coordinator network event handling function for state discovering
void DISCOVERER(void) {
//	master_lcd_print(" <T", 3);
//	STOP_TIMER();
	BYTE i, j;
	for(i = 0; i < DATABASE_LENGTH; ++i)
		for(j = 0; j < ADDRESS_LENGTH; ++j)
			pollbase[i][j] = INITIALIZE_CONSTANT;
	POLL_REQUEST();
	//master_lcd_print(" >P", 3);
	START_TIMER(5);
	state = POLLING;
}

//coordinator network event handling function for state polling
void POLLER(void) {
	BYTE i, j, match, temp[ADDRESS_LENGTH];
	switch(timer) {
		case TIMER_UNSET:
			//master_lcd_print(" <T", 3);
			STOP_TIMER();
			for(i = 0; i < DATABASE_LENGTH; ++i) {
				match = 0;
				for(j = 0; j < ADDRESS_LENGTH; ++j)
					match |= pollbase[i][j];
				if(!match) {
					if((offeredID == INITIALIZE_CONSTANT) || (requestedID == i+1))
						offeredID = i+1;
					match = 0;
					for(j = 0; j < ADDRESS_LENGTH; ++j) {
						temp[j] = database[i][j];
						match |= database[i][j];
					}
					if(match) {
						DOWN(temp, i-1);
						for(j = 0; j < ADDRESS_LENGTH; ++j)
							database[i][j] = INITIALIZE_CONSTANT;
					}
				}
			}
			if(offeredID == INITIALIZE_CONSTANT) {
				ABORT();
				//master_lcd_print(" >X", 3);
				state = TIMEKILLING;
			}
			else {
				OFFER();
				//master_lcd_print(" >O", 3);
				START_TIMER(10);
				state = OFFERING;
			}
		break;
		default:
			switch(packet_in[0]) {
				case JUXP_POLL_RESPONSE:
					//master_lcd_print(" <P", 3);
					for(i = 0; i < ADDRESS_LENGTH; ++i)
						pollbase[packet_in[JUXP_LENGTH-1]][i] = packet_in[i+1];
				break;
				case JUXP_ABORT:
					//master_lcd_print(" <X", 3);
					match = 0;
					for(i = 0; i < ADDRESS_LENGTH; ++i)
						match |= minor[i] ^ packet_in[i+1];
					match |= offeredID ^ packet_in[JUXP_LENGTH-1];
					if(!match) {
						STOP_TIMER();
						state = TIMEKILLING;
					}
				break;
			}
		break;
	}
}

//coordinator network event handling function for state offering
void OFFERER(void) {
	BYTE i, match;
	switch(timer) {
		case TIMER_UNSET:
			//master_lcd_print(" <T", 3);
			STOP_TIMER();
			ABORT();
			//master_lcd_print(" >X", 3);
			state = TIMEKILLING;
		break;
		default:
			switch(packet_in[0]) {
				case JUXP_REQUEST:
					//master_lcd_print(" <R", 3);
					STOP_TIMER();
					state = REQUESTING;
					START_TIMER(1);
//					STATE();
				break;
				case JUXP_ABORT:
					//master_lcd_print(" <X", 3);
					match = 0;
					for(i = 0; i < ADDRESS_LENGTH; ++i)
						match |= minor[i] ^ packet_in[i+1];
					match |= offeredID ^ packet_in[JUXP_LENGTH-1];
					if(!match) {
						STOP_TIMER();
						state = TIMEKILLING;
					}
				break;
			}
		break;
	}
}

//coordinator network event handling function for state requesting
void REQUESTER(void) {
//	master_lcd_print(" <T", 3);
//	STOP_TIMER();
	BYTE i, match = 0;
	for(i = 0; i < ADDRESS_LENGTH; ++i)
		match |= minor[i] ^ packet_in[i+1];
	match |= offeredID ^ packet_in[JUXP_LENGTH-1];
	if(!match) {
		ACKNOWLEDGE();
		//master_lcd_print(" >A", 3);
		START_TIMER(10);
		state = ACKNOWLEDGING;
	}
	else {
		ABORT();
		//master_lcd_print(" >X", 3);
		state = TIMEKILLING;
	}
}

//coordinator network event handling function for state acknowledging
void ACKNOWLEDGER(void) {
	BYTE i, match;
	switch(timer) {
		case TIMER_UNSET:
			//master_lcd_print(" <T", 3);
			STOP_TIMER();
			ABORT();
			//master_lcd_print(" >X", 3);
			state = TIMEKILLING;
		break;
		default:
			switch(packet_in[0]) {
				case JUXP_JOIN:
					match = 0;
					for(i = 0; i < ADDRESS_LENGTH; ++i)
						match |= minor[i] ^ packet_in[i+1];
					match |= offeredID ^ packet_in[JUXP_LENGTH-1];
					if(!match) {
						STOP_TIMER();
						for(i = 0; i < ADDRESS_LENGTH; ++i)
							database[offeredID-1][i] = minor[i];
						UP();
						//master_lcd_print(" >U", 3);
						state = TIMEKILLING;
						master_lcd_print("Router joined");
//						PRINT_DATABASE();
					}
				break;
				case JUXP_ABORT:
					//master_lcd_print(" <X", 3);
					match = 0;
					for(i = 0; i < ADDRESS_LENGTH; ++i)
						match |= minor[i] ^ packet_in[i+1];
					match |= offeredID ^ packet_in[JUXP_LENGTH-1];
					if(!match) {
						STOP_TIMER();
						state = TIMEKILLING;
					}
				break;
			}
		break;
	}
}

//coordinator network event handling function for state moving
void MOVER(void) {
	BYTE i, j, match, temp[ADDRESS_LENGTH];
	switch(timer) {
		case TIMER_UNSET:
			//master_lcd_print(" <T", 3);
			STOP_TIMER();
			ABANDON(requestedID, JUXP_MOVEMENT_COMMAND, offeredID, requestedID);
			ABANDON(offeredID, JUXP_MOVEMENT_REQUEST, offeredID, requestedID);
			//master_lcd_print(" >X", 3);
			state = TIMEKILLING;
		break;
		default:
			switch(packet_in[0]) {/*
				case JUXP_MOVEMENT_ACKNOWLEDGE:
					master_lcd_print(" <MA", 4);
					match = 0;
					match |= offeredID ^ packet_in[1];
					match |= requestedID ^ packet_in[2];
					if(!match) {
						STOP_TIMER();
						if(offeredID) {
							MOVEMENT_RESPONSE(offeredID, requestedID, packet_in[3], packet_in[4], packet_in[5]);
							master_lcd_print(" >MP", 4);
						}	//handle coordiantor movement commands later
						state = TIMEKILLING;
					}
				break; */
				case JUXP_ABANDON:
					//master_lcd_print(" <X", 3);
					match = 0;
					match |= JUXP_MOVEMENT_COMMAND ^ packet_in[0];
					match |= offeredID ^ packet_in[1];
					match |= requestedID ^ packet_in[2];
					if(!match) {
						STOP_TIMER();
						ABANDON(offeredID, JUXP_MOVEMENT_REQUEST, offeredID, requestedID);
						//master_lcd_print(" >X", 3);
						state = TIMEKILLING;
					}
				break;
			}
		break;
	}
}

#elif TYPE == ROUTER

/*

Jadavpur University XBEE Network::

router states::

	networking::
		usage:	when not registered on a network
		trggr:	state:	off
				actns:	powerup
		rspns:	state:	discovering
				actns:	broadcast discovery

	discovering::
		usage:	when allowing discovery
		trggr:	state:	timekilling
				actns:	broadcast discovery
		rspns:	state:	networking
				actns:	abort on timeout
				state:	networking
				actns:	on network resets
				state:	networking
				actns:	on coordinator abort
				state:	offering
				actns:	on coordinator offer

	offering::
		usage:	when offered router networkID
		trggr:	state:	discovering
				actns:	on coordinator offer
		rspns:	state:	networking
				actns:	abort coordinator on receiving unsatisfactory offer
				state:	requesting
				actns:	request coordinator for networkID

	requesting::
		usage:	when requesting coordinator
		trggr:	state:	offering
				actns:	request coordiantor for networkID
		rspns:	state:	requesting
				actns:	on acknowledgements
				state:	networking
				actns:	on network reset
				state:	networking
				actns:	on coordinator abort
				state:	acknowledging
				actns:	on timeout

	acknowledging::
		usage:	when acknowledging network join
		trggr:	state:	requesting
				actns:	on timeout
		rspns:	state:	networking
				actns:	abort coordiantor on invalid acknowledgements
				state:	timekilling
				actns:	join network

	timekilling::
		usage:	when not doing anything else
		rspns:	state:	timekilling
				actns:	on up
				state:	timekilling
				actns:	on down
				state:	polling
				actns:	on poll request
	note: central state

	polling::
		usage:	when network is being polled
		trggr:	state:	timekilling
				actns:	on poll request
		rspns:	state:	timekilling
				actns:	poll self

	moving::
		usage:	when movement is requested
		trggr:	state:	timekilling
				actns:	on movemnet command
		rspns:	state:	timekilling
				actns:	acknowledge movement
				state:	timekilling
				actns:	abandon on parameter mismatch

*/

//router network event handling function for state networking
void NETWORKER(void) {
//	master_lcd_print(" <T", 3);
//	STOP_TIMER();
	BYTE i, j;
	for(i = 0; i < DATABASE_LENGTH; ++i)
		for(j = 0; j < ADDRESS_LENGTH; ++j)
			database[i][j] = INVALIDATE_CONSTANT;
	for(i = 0; i < ADDRESS_LENGTH; ++i)
		minor[i] = INITIALIZE_CONSTANT;
	networkID = INITIALIZE_CONSTANT;
	requestedID = INITIALIZE_CONSTANT;	//handle preferences later
	offeredID = INITIALIZE_CONSTANT;
//<mark1>
	DISCOVER();
	//master_lcd_print(" >D", 3);
	START_TIMER(10);
	state = DISCOVERING;
//</mark1>
//<seminar>
//	state = TIMEKILLING;
//</seminar>
}

//router network event handling function for state discovering
void DISCOVERER(void) {
	BYTE i;
	switch(timer) {
		case TIMER_UNSET:
			//master_lcd_print(" <T", 3);
			STOP_TIMER();
			ABORT();
			//master_lcd_print(" >X", 3);
			state = NETWORKING;
			START_TIMER(1);
//			STATE();
		break;
		default:
			switch(packet_in[0]) {
				case JUXP_OFFER:
					//master_lcd_print(" <O", 3);
					STOP_TIMER();
					for(i = 0; i < ADDRESS_LENGTH; ++i)
						minor[i] = packet_in[i+1];
					offeredID = packet_in[JUXP_LENGTH-1];
					state = OFFERING;
					START_TIMER(1);
//					STATE();
				break;
				case JUXP_ABORT:
					//master_lcd_print(" <X", 3);
					STOP_TIMER();
					state = NETWORKING;
					START_TIMER(1);
//					STATE();
				break;
				case JUXP_RESET:
					//master_lcd_print(" <R", 3);
					STOP_TIMER();
					state = NETWORKING;
					START_TIMER(1);
//					STATE();
				break;
			}
	}
}

//handle preferences later
//router network event handling function for state offering
void OFFERER(void) {
//	master_lcd_print(" <T", 3);
//	STOP_TIMER();
	networkID = offeredID;
	REQUEST();
	//master_lcd_print(" >R", 3);
	START_TIMER(10);
	state = REQUESTING;
}

//router network event handling function for state requesting
void REQUESTER(void) {
	BYTE i, match;
	switch(timer) {
		case TIMER_UNSET:
			//master_lcd_print(" <T", 3);
			STOP_TIMER();
			state = ACKNOWLEDGING;
			START_TIMER(1);
//			STATE();
		break;
		default:
			switch(packet_in[0]) {
				case JUXP_ACKNOWLEDGE:
					//master_lcd_print(" <A", 3);
					for(i = 0; i < ADDRESS_LENGTH; ++i)
						database[packet_in[JUXP_LENGTH-1]-1][i] = packet_in[i+1];
				break;
				case JUXP_ABORT:
					//master_lcd_print(" <X", 3);
					STOP_TIMER();
					state = NETWORKING;
					START_TIMER(1);
//					STATE();
				break;
				case JUXP_RESET:
					//master_lcd_print(" <R", 3);
					STOP_TIMER();
					state = NETWORKING;
					START_TIMER(1);
//					STATE();
				break;
			}
	}
}

//router network event handling function for state acknowledging
void ACKNOWLEDGER(void) {
//	master_lcd_print(" <T", 3);
//	STOP_TIMER();
	BYTE i, j, match;
	for(i = 0; i < DATABASE_LENGTH; ++i) {
		match = 0;
		for(j = 0; j < ADDRESS_LENGTH; ++j)
			match |= database[i][j] ^ INVALIDATE_CONSTANT;
		if(!match) {
			ABORT();
			//master_lcd_print(" >X", 3);
			state = NETWORKING;
			START_TIMER(1);
//			STATE();
			return;
		}
	}
	JOIN();
	master_lcd_print("I have joined");
	state = TIMEKILLING;
}

//router network event handling function for state timekilling
void TIMEKILLER(void) {
	BYTE i, length;
	switch(packet_in[0]) {
		case JUXP_POLL_REQUEST:
			//master_lcd_print(" <P", 3);
			state = POLLING;
			START_TIMER(1);
//			STATE();
		break;
		case JUXP_UP:
			//master_lcd_print(" <U", 3);
			for(i = 0; i < ADDRESS_LENGTH; ++i)
				database[packet_in[JUXP_LENGTH-1]-1][i] = packet_in[i+1];
		break;
		case JUXP_DOWN:
			//master_lcd_print(" <D", 3);
			for(i = 0; i < ADDRESS_LENGTH; ++i)
				database[packet_in[JUXP_LENGTH-1]-1][i] = INITIALIZE_CONSTANT;
		break;
/*		case JUXP_MOVEMENT_COMMAND:
			master_lcd_print(" <MC", 4);
			offeredID = packet_in[1];
			requestedID = packet_in[2];
			if(!(requestedID ^ networkID)) {
				MOVEMENT_ACKNOWLEDGE(offeredID, packet_in[3], packet_in[4], packet_in[5]);
				master_lcd_print(" >MC", 4);
				queue_push(MOVE_STOP);
				queue_push(packet_in[3]);
				for(i = 1; i < packet_in[4]; ++i)
					queue_push(packet_in[3]);
				if(packet_in[5])
					START_TIMER(packet_in[5]);
				state = MOVING;
			}
		break;
		case JUXP_REPORT_MAP:
			master_lcd_print(" <RM", 4);
			sendDataRPi(packet_in, 10);
			//send packet_in to RPi
		break;
*/
//<seminar>
		case JUXP_DATA:
			length = packet_in[4];
			switch(packet_in[3]) {
				case DATA_LEVEL_MASTER:
					//5 is the starting of frame to send
					sendDataMaster(packet_in + 5, length);
				break;
				case DATA_LEVEL_PI:
					sendDataRPi(packet_in + 5, length);
				break;
			}
		break;
//</seminar>
	}
}

//router network event handling function for state polling
void POLLER(void) {
	//master_lcd_print(" <T", 3);
	STOP_TIMER();
	POLL_RESPONSE();
	//master_lcd_print(" >P", 3);
	state = TIMEKILLING;
}

//router network event handling function for state moving
void MOVER(void) {
	BYTE i, j, match, temp[ADDRESS_LENGTH];
	switch(timer) {
		case TIMER_UNSET:
			//master_lcd_print(" <T", 3);
			STOP_TIMER();
			queue_push(MOVE_STOP);
			state = TIMEKILLING;
		break;
		default:
			switch(packet_in[0]) {
				case JUXP_ABANDON:
					//master_lcd_print(" <X", 3);
					match = 0;
					match |= JUXP_MOVEMENT_COMMAND ^ packet_in[0];
					match |= offeredID ^ packet_in[1];
					match |= requestedID ^ packet_in[2];
					if(!match) {
						STOP_TIMER();
						state = TIMEKILLING;
					}
				break;
/*				case JUXP_MOVEMENT_COMMAND:
					master_lcd_print(" <MC", 4);
					if(!(offeredID ^ packet_in[1]) && !(requestedID ^ packet_in[2])) {
						MOVEMENT_ACKNOWLEDGE(offeredID, packet_in[3], packet_in[4], packet_in[5]);
						master_lcd_print(" >MC", 4);
						queue_push(MOVE_STOP);
						queue_push(packet_in[3]);
						for(i = 1; i < packet_in[4]; ++i)
							queue_push(packet_in[3]);
						if(packet_in[5])
							START_TIMER(packet_in[5]);
					}
				break;	*/
			}
		break;
	}
}

#endif

//network state event handler
void STATE(void) {
	BYTE i;
//	for(i = 0; i < 10; ++i)
//		queue_push(packet_in[i]);
	switch(state) {
		case NETWORKING:
			//master_lcd_print("!N", 2);
			NETWORKER();
		break;
		case TIMEKILLING:
			//master_lcd_print("!T", 2);
			TIMEKILLER();
		break;
		case DISCOVERING:
			//master_lcd_print("!D", 2);
			DISCOVERER();
		break;
		case POLLING:
			//master_lcd_print("!P", 2);
			POLLER();
		break;
		case OFFERING:
			//master_lcd_print("!O", 2);
			OFFERER();
		break;
		case REQUESTING:
			//master_lcd_print("!R", 2);
			REQUESTER();
		break;
		case ACKNOWLEDGING:
			//master_lcd_print("!A", 2);
			ACKNOWLEDGER();
		break;
/*		case MOVING:
			//master_lcd_print("!M", 2);
			MOVER();
		break;	*/
		default:
			//master_lcd_print("!d", 2);
		break;
	}
}
