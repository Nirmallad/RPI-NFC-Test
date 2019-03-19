#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>

#include "wiringPi.h"
#include "wiringSerial.h"


#define DEBUG_MODE	0

typedef enum{
	KEY_1 	= 0x00001E00,
	KEY_2 	= 0x00001F00,
	KEY_3 	= 0x00002000,
	KEY_4 	= 0x00002100,
	KEY_5 	= 0x00002200,
	KEY_6 	= 0x00002300,
	KEY_7 	= 0x00002400,
	KEY_8 	= 0x00002500,
	KEY_9 	= 0x00002600,
	KEY_0 	= 0x00002700,
	KEY_ENT = 0x00002800,
	KEY_A 	= 0x02000400,
	KEY_B 	= 0x02000500,
	KEY_C 	= 0x02000600,
	KEY_D 	= 0x02000700,
	KEY_E 	= 0x02000800,
	KEY_F 	= 0x02000900
} KEY_VALUE;


typedef union{		
	uint8_t extra_padding[16];
	struct{
		uint32_t key_value1;
		uint32_t key_value2;
	};
}HID_DATA;

/*typedef struct{
	//uint16_t pre_extra_padding;
	//uint16_t key_value;
	uint8_t extra_padding[8];
}HID_DATA;*/

bool send_data = false;
uint8_t g_serial_tx_buf[128], g_serial_tx_index;
int fd ;

FILE *fp;

inline uint32_t Reverse32(uint32_t value) 
{
    return (((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) <<  8) |
            ((value & 0x00FF0000) >>  8) |
            ((value & 0xFF000000) >> 24));
}


/* Signal Handler for SIGINT */
void sigintHandler(int sig_num) 
{ 
    /* Reset handler to catch SIGINT next time. 
       Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, sigintHandler); 
    printf("%s","Terminating the program.\r\n"); 
    fflush(stdout); 
    serialClose(fd);  	
    fclose(fp);
    exit(0);
} 

void Get_Key(KEY_VALUE value){

#if DEBUG_MODE
	switch(value){
		case KEY_0: 	printf("Key 0\n");	break;
		case KEY_1: 	printf("Key 1\n"); 	break;
		case KEY_2: 	printf("Key 2\n"); 	break;
		case KEY_3: 	printf("Key 3\n"); 	break;
		case KEY_4: 	printf("Key 4\n"); 	break;
		case KEY_5: 	printf("Key 5\n"); 	break;
		case KEY_6: 	printf("Key 6\n"); 	break;
		case KEY_7: 	printf("Key 7\n"); 	break;
		case KEY_8: 	printf("Key 8\n"); 	break;
		case KEY_9: 	printf("Key 9\n"); 	break;

		
		case KEY_A: 	printf("Key A\n"); 	break;
		case KEY_B: 	printf("Key B\n"); 	break;
		case KEY_C: 	printf("Key C\n"); 	break;
		case KEY_D: 	printf("Key D\n"); 	break;
		case KEY_E: 	printf("Key E\n"); 	break;
		case KEY_F: 	printf("Key F\n"); 	break;
		default: 		printf("ERROR\n"); 	break;
		
		case KEY_ENT: 	printf("Key Enter\n");	break;
	}	
#endif

	switch(value){
		case KEY_0: 	printf("0"); 		g_serial_tx_buf[g_serial_tx_index++] = '0';		break;
		case KEY_1: 	printf("1"); 		g_serial_tx_buf[g_serial_tx_index++] = '1';		break;
		case KEY_2: 	printf("2"); 		g_serial_tx_buf[g_serial_tx_index++] = '2';		break;
		case KEY_3: 	printf("3"); 		g_serial_tx_buf[g_serial_tx_index++] = '3';		break;
		case KEY_4: 	printf("4"); 		g_serial_tx_buf[g_serial_tx_index++] = '4';		break;
		case KEY_5: 	printf("5"); 		g_serial_tx_buf[g_serial_tx_index++] = '5';		break;
		case KEY_6: 	printf("6"); 		g_serial_tx_buf[g_serial_tx_index++] = '6';		break;
		case KEY_7: 	printf("7"); 		g_serial_tx_buf[g_serial_tx_index++] = '7';		break;
		case KEY_8: 	printf("8"); 		g_serial_tx_buf[g_serial_tx_index++] = '8';		break;
		case KEY_9: 	printf("9"); 		g_serial_tx_buf[g_serial_tx_index++] = '9';		break;

		
		case KEY_A: 	printf("A"); 		g_serial_tx_buf[g_serial_tx_index++] = 'A';		break;
		case KEY_B: 	printf("B"); 		g_serial_tx_buf[g_serial_tx_index++] = 'B';		break;
		case KEY_C: 	printf("C"); 		g_serial_tx_buf[g_serial_tx_index++] = 'C';		break;
		case KEY_D: 	printf("D"); 		g_serial_tx_buf[g_serial_tx_index++] = 'D';		break;
		case KEY_E: 	printf("E"); 		g_serial_tx_buf[g_serial_tx_index++] = 'E';		break;
		case KEY_F: 	printf("F"); 		g_serial_tx_buf[g_serial_tx_index++] = 'F';		break;
		default: 		printf("ERROR\n"); 		break;
		
		case KEY_ENT: 	printf("\n"); 		g_serial_tx_buf[g_serial_tx_index++] = '\n';	send_data = true;	break;
	}
}	

int main( int argc, char *argv[] )  {

	HID_DATA hid_data[50];
	uint8_t loop_i;
	
	unsigned char filename[50];
	
	if( argc == 2 ) {
		printf("The argument supplied is %s\n", argv[1]);
	}
	else if( argc > 2 ) {
		printf("Too many arguments supplied.\n");
		exit(EXIT_FAILURE);
	}
	else {
		printf("One argument expected.\n");
		exit(EXIT_FAILURE);
	}
   
	/* Set the SIGINT (Ctrl-C) signal handler to sigintHandler     Refer http://en.cppreference.com/w/c/program/signal */	
    signal(SIGINT, sigintHandler); 
   
	sprintf (filename, "/dev/%s", argv[1]);
	fp = fopen(filename, "rb");

	if(fp == NULL){
	   printf("Unable to open %s\n", filename);
	   exit(EXIT_FAILURE);
	}
	
	if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0)
	{
		//fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		printf ("Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	
	if (wiringPiSetup () == -1)
	{
		//fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		printf ("Unable to start wiringPi: %s\n", strerror (errno)) ;
		return 1 ;
	}	
	printf("%s", "wiringPi Initailized\r\n");	
	printf("Serial NFC Transmit Started\r\n");
	
	
	memset(&g_serial_tx_buf, 0, sizeof(g_serial_tx_buf));
	g_serial_tx_index = 0;	
	while(1){	
		loop_i = 0;
		
		while(loop_i < 9){
			
			fread(&hid_data[loop_i], sizeof( HID_DATA), 1, fp); 
			/*printf("%02X\t%02X\t%02X\t%02X\n", 	hid_data[9-loop_i].key_value,
												hid_data[9-loop_i].extra_padding[0],
												hid_data[9-loop_i].extra_padding[1],
												hid_data[9-loop_i].extra_padding[2] );*/
	#if DEBUG_MODE
			printf("%02X\t%02X\t%02X\t%02X\t%02X\t%02X\t%02X\t%02X\t",
												hid_data[loop_i].extra_padding[0],hid_data[loop_i].extra_padding[1],
												hid_data[loop_i].extra_padding[2],hid_data[loop_i].extra_padding[3],
												hid_data[loop_i].extra_padding[4],hid_data[loop_i].extra_padding[5],
												hid_data[loop_i].extra_padding[6],hid_data[loop_i].extra_padding[7]);
			printf(": %08X\t %08X\t", hid_data[loop_i].key_value1, Reverse32(hid_data[loop_i].key_value1));
	#endif
			Get_Key(Reverse32(hid_data[loop_i].key_value1));
			loop_i ++;		
		}
		if(send_data == true){
	#if DEBUG_MODE
			printf("Serial Buffer: %d\t%s\n", g_serial_tx_index, g_serial_tx_buf);
	#endif		
			send_data = false;
			g_serial_tx_index = 0;
			serialPuts(fd, g_serial_tx_buf);

			
		}
	}
	fclose(fp);
   
}





