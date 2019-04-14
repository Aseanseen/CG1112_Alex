#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include "packet.h"
#include "serial.h"
#include "serialize.h"
#include "constants.h"
#include <stdlib.h>


//take note that PORT_NAME may change, but it should either be ttyACM0 or ACM1
#define PORT_NAME			"/dev/ttyACM1"
#define BAUD_RATE			B57600

int exitFlag=0;
sem_t _xmitSema;

void handleError(TResult error) {
	switch(error) {
		case PACKET_BAD:
			printf("[PI] received Bad Magic Number\n");
			break;

		case PACKET_CHECKSUM_BAD:
			printf("[PI] Bad checksum\n");
			break;

		default:
			printf("[PI] UNKNOWN ERROR\n");
	}
}
void scan(){
		printf("plotting...\n");

			std::system("g++ w8s1.cpp Lib/librplidar_sdk.a -lpthread -lm");

			std::system("./a.out /dev/ttyUSB0");

			
}	

void plot(){
	
	std::system("gnuplot liplot.plt --persist");
}	



//handles 6 params
void handleStatus(TPacket *packet) {
	printf("[PI] fetched ARDUINO status report\n");
	printf("\n ------- ARDUINO STATUS REPORT ------- \n\n");
	printf("[AR]: Left Encoder Count:  	\t\t%d\n", packet->params[0]);
	printf("[AR]: Right Encoder Count: 	\t\t%d\n", packet->params[1]);
	printf("[AR]: Left Angle:    		\t\t%d\n", packet->params[2]);
	printf("[AR]: Right Angle:			\t\t%d\n", packet->params[3]);
	printf("[AR]: Forward Dist:			\t%d\n", packet->params[4]);
	printf("[AR]: Reverse Dist:			\t%d\n", packet->params[5]);
	printf("[AR]: Net angle movement: 	\t%d\n", packet->params[6]);
	//printf("Right Reverse Ticks Turns:\t%d\n", packet->params[7]);
	//printf("Forward Distance:\t\t%d\n", packet->params[8]);
	//printf("Reverse Distance:\t\t%d\n", packet->params[9]);
	printf("\n---------------------------------------\n\n");
}

void handleResponse(TPacket *packet) { // The response code is stored in command
	switch(packet->command) {
		case RESP_OK:
			printf("[AR] Command OK\n");
		break;

		case RESP_STATUS:
			handleStatus(packet);
		break;
		
		case RESP_TOO_CLOSE:
			printf("[AR] Too close to an object in front\n");
		break;

		default:
			printf("[PI] No comprehendo.\n");
	}
}

void handleErrorResponse(TPacket *packet) { //The error code is returned in command
	switch(packet->command) {
		case RESP_BAD_PACKET:
			printf("[AR] received bad magic number\n");
		break;

		case RESP_BAD_CHECKSUM:
			printf("[AR] received bad checksum\n");
		break;

		case RESP_BAD_COMMAND:
			printf("[AR] received bad command\n");
		break;

		case RESP_BAD_RESPONSE:
			printf("[AR] received unexpected response\n");
		break;

		default:
			printf("[AR] received unknown error\n");
	}
}

void handleMessage(TPacket *packet) {
	printf("[AR] >> MSG: %s\n", packet->data);
}

void handlePacket(TPacket *packet) {
	switch(packet->packetType) {
		case PACKET_TYPE_COMMAND: //Only we send command packets, so ignore
			break;

		case PACKET_TYPE_RESPONSE:
				handleResponse(packet);
			break;

		case PACKET_TYPE_ERROR:
				handleErrorResponse(packet);
			break;

		case PACKET_TYPE_MESSAGE:
				handleMessage(packet);
			break;
	}
}

void sendPacket(TPacket *packet) {
	char buffer[PACKET_SIZE];
	int len = serialize(buffer, packet, sizeof(TPacket));

	serialWrite(buffer, len);
}

void *receiveThread(void *p) {
	char buffer[PACKET_SIZE];
	int len;
	TPacket packet;
	TResult result;
	int counter=0;

	while(1) {
		len = serialRead(buffer);
		counter+=len;
		if(len > 0) {
			result = deserialize(buffer, len, &packet);

			if(result == PACKET_OK) {
				counter=0;
				handlePacket(&packet);
			}
			else if(result != PACKET_INCOMPLETE) {
				printf("[PI] RECEIVED PACKET ERROR\n");
				handleError(result);
			}
		}
	}
}

void flushInput() {
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

void getParams(TPacket *commandPacket) {
	printf("[PI] input parameters:\n");
	printf("[PI] enter magnitude / speed\n");
	scanf("%d %d", &commandPacket->params[0], &commandPacket->params[1]);
	flushInput();
}

void sendCommand(char command) {
	TPacket commandPacket;

	commandPacket.packetType = PACKET_TYPE_COMMAND;

	switch(command) {
		case 'h':
		case 'H':
			printf("===========================================================\n");
			printf("[PI] displaying options (case insensitive)\n");
			printf("===========================================================\n");
			printf("Movement Commands:\n");
			printf("|Q - Approach | W - Forward | E - Fwd COFF | R - get Dst\n");
			printf("|A - Left     | S - Reverse | D - Right    | F - get RGB\n");
			printf("|Z - left (S) | C - Stop    | C - Rt (SW)  | V - Plot \n");
			printf("-----------------------------------------------------------\n");
			printf("Status Commands:\n");
			printf("T - Clear status, G - Get status\n");
			printf("-----------------------------------------------------------\n");
			printf("DEBUG COMMANDS:\n");
			printf("P - Calibrate Light Sensor\n");
			printf("O - Exit\n");
			printf("===========================================================\n");
			break;
		
		case 'q': //approach obj
		case 'Q':
			printf("[PI] approach command\n");
			commandPacket.command = COMMAND_APPROACH;
			sendPacket(&commandPacket);
			break;
			
		case 'e':
		case 'E':
			printf("[PI] forward, collision disabled\n");
			printf("[PI] WARNING COLLISION DETECTION OFF\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_FWD_NO_STOP;
			sendPacket(&commandPacket);
			break;
			
		case 'r':
		case 'R':
			printf("[PI] get dist command\n");
			commandPacket.command = COMMAND_GETDIST;
			sendPacket(&commandPacket);
			break;
			
		case 'w': //forward
		case 'W':
			printf("[PI] forward command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_FORWARD;
			sendPacket(&commandPacket);
			scan();
			break;

		case 's': //reverse
		case 'S':
			printf("[PI] reverse command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_REVERSE;
			sendPacket(&commandPacket);
			
			scan();
			break;

		case 'a': //left
		case 'A':
			printf("[PI] left command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_TURN_LEFT;
			sendPacket(&commandPacket);
			scan();
			
			break;

		case 'd': //right
		case 'D':
			printf("[PI] right command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_TURN_RIGHT;
			sendPacket(&commandPacket);
			scan();
			break;
			
		
		case 'z': //swing left
		case 'Z':
			printf("[PI] swing left command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_SWING_LEFT;
			sendPacket(&commandPacket);
			scan();
			
			break;

		case 'c': //swing right
		case 'C':
			printf("[PI] swing right command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_SWING_RIGHT;
			sendPacket(&commandPacket);
			scan();
			break;

		case 'x': //stop
		case 'X':
			printf("[PI] stop command\n");
			commandPacket.command = COMMAND_STOP;
			sendPacket(&commandPacket);
			break;

		case 't': //clear status
		case 'T':
			printf("[PI] clear status command\n");
			commandPacket.command = COMMAND_CLEAR_STATS;
			commandPacket.params[0] = 0;
			sendPacket(&commandPacket);
			break;

		case 'g': //fetch status
		case 'G':
			printf("[PI] fetch status command\n");
			commandPacket.command = COMMAND_GET_STATS;
			sendPacket(&commandPacket);
			
			break;
			
		case 'p':
		case 'P':
			printf("[PI] calibrating ARDUINO light sensor\n");
			commandPacket.command = COMMAND_CALIBRATELS;
			sendPacket(&commandPacket);
			break;
			
		case 'f':
		case 'F':
			printf("[PI] fetching RGB values\n");
			commandPacket.command = COMMAND_GETRGB;
			sendPacket(&commandPacket);
			break;
			
		case 'v': //plot
		case 'V':
			printf("[PI] plotting\n");
			plot();
			break;

		case 'o': //exit
		case 'O':
			printf("[PI] EXIT CALLED\n");
			exitFlag=1;
			break;
		
		default:
			printf("Bad command\n");

	}
}

int main() {
	//Connect to the Arduino
	startSerial(PORT_NAME, BAUD_RATE, 8, 'N', 1, 5);

	//Sleep for two seconds
	printf("[PI] WAITING TWO SECONDS FOR ARDUINO TO REBOOT\n");
	sleep(2);
	printf("[PI] DONE\n");

	//Spawn receiver thread
	pthread_t recv;

	pthread_create(&recv, NULL, receiveThread, NULL);
	
	
	
	//Send a hello packet
	TPacket helloPacket;
	helloPacket.packetType = PACKET_TYPE_HELLO;
	sendPacket(&helloPacket);

	

	while(!exitFlag) {
		char ch;
		printf("[PI] Enter Command:\n");
		scanf("%c", &ch);

		//Purge extraneous characters from input stream
		flushInput();

		sendCommand(ch);
	}

	printf("[PI] Closing connection to Arduino.\n");
	endSerial();
}
