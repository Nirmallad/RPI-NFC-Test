#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "wiringPi.h"
#include "wiringSerial.h"

volatile int p_run = 0;
FILE *fptr;
int fd ;



/* Signal Handler for SIGINT */
void sigintHandler(int sig_num) 
{ 
    /* Reset handler to catch SIGINT next time. 
       Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, sigintHandler); 
    fprintf(fptr, "%s","Terminating the program.\r\n"); 
    fflush(stdout); 
    serialClose(fd);  	
    fclose(fptr);
    p_run = 0; exit(0);
} 

int main ()
{
	
	char byte = 0;
	

	/* Set the SIGINT (Ctrl-C) signal handler to sigintHandler     Refer http://en.cppreference.com/w/c/program/signal */
	
    signal(SIGINT, sigintHandler); 
    
	fptr = fopen("program_log.txt", "w");
	if(fptr == NULL)
	{
	  printf("Error!");
	  exit(1);
	}   

    
	if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0)
	{
		//fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		fprintf (fptr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	fprintf(fptr,"%s", "Serial Port Open\r\n");
	
	if (wiringPiSetup () == -1)
	{
		//fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		fprintf (fptr, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return 1 ;
	}	
	fprintf(fptr,"%s", "Wirin Pi Initailized\r\n");
	//serialPutchar(fd, '$');
	printf("Serial NFC Transmit Started\r\n");
	
	p_run = 1;
	fprintf(fptr,"%s", "Serial NFC Transmit Started\r\n");
	
	while(1) 
	{
		byte =  getchar();
		serialPutchar(fd, byte);		
		putchar(byte);
		fputc(byte, fptr);
		//serialGetchar (fd);
	}
	serialClose(fd);  	
	printf("Serial NFC Transmit Closed\r\n");
	fprintf(fptr,"%s","Serial NFC Transmit Closed\r\n");
	fclose(fptr);
}
