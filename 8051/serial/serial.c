/**********************************
Two way serial communication
Wait for key then respond
***********************************/


#include <at89x051.h>

unsigned char received_character;
int received_flag;


/** 
Initialize the RS232 serial port
We need to turn off interrupt first since 
we're using single line for RX/TX
**/

void serial_init()
{
	SCON = 0x50;		// 8-bit data
	TMOD = 0x20;		// timer for auto-reload
	TH1 = 0xF4;		// setup for 2400 baud
	TR1 = 1;		// turn on timer 1
	TI = 1;			// indicate ready to transmit
	EA = 1;			// enable interrupts
//	ES = 1;			// enable serial interrupts
}

/* send a null terminated string to the RS232 port */
void send_serial(unsigned char *s) __critical
{
	// Disable the interrupt
	while(*s != 0x0)
	{
		SBUF = *s;
		while(! TI)	// wait until sent
		{
		}
		TI = 0;
		s++;		// get the next character
	}
	// enable interrupt
}

/* send a single character to the serial port */
void send_1_char(unsigned char c) __critical
{
	// disable interrupt
	SBUF = c;
	while (!TI)		// wait till transmitted
	{
	}
	TI = 0;
}

/** serial port interrupt service routine. Program jumps to this
routine when a character is transmitted or received */
void serial() __interrupt 4
{
	if(RI)			// if a character received
	{
		received_character = SBUF;
		RI = 0;
		received_flag = 1;
	}
}


/** main routine **/
main()
{
	received_flag = 0;
	serial_init();		// initialize serial port
	
	for(;;)
	{
		REN = 0;
		ES = 0;
		send_serial("Enter a character:");
		REN = 1;
		ES = 1;
		while(received_flag == 0)
		{
		}
		REN = 0;
		ES = 0;
		received_flag = 0;
		send_1_char(received_character);	//echo
		send_serial("The next character is:");
		received_character++;
		send_1_char(received_character);
		send_1_char(0x0D);			// send carriage return
		send_1_char(0x0A);			// send line feed
	}
}
