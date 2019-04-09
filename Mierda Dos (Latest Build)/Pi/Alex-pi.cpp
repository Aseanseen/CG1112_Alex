#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include "packet.h"
#include "serial.h"
#include "serialize.h"
#include "constants.h"

//take note that PORT_NAME may change, but it should either be ttyACM0 or ACM1
#define PORT_NAME			"/dev/ttyACM0"
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

//handles 6 params
void handleStatus(TPacket *packet) {
	printf("[PI] fetched ARDUINO status report\n");
	printf("\n ------- ARDUINO STATUS REPORT ------- \n\n");
	printf("[AR]: Left Forward Count:\t\t%d\n", packet->params[0]);
	printf("[AR]: Right Forward Count:\t\t%d\n", packet->params[1]);
	printf("[AR]: Left Revolutions:\t\t%d\n", packet->params[2]);
	printf("[AR]: Right Revolutions:\t\t%d\n", packet->params[3]);
	printf("[AR]: Forward Dist:\t%d\n", packet->params[4]);
	printf("[AR]: Reverse Dist:\t%d\n", packet->params[5]);
	//printf("Left Reverse Ticks Turns:\t%d\n", packet->params[6]);
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
	printf("[AR] msg: %s\n", packet->data);
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
			printf("==================================================\n");
			printf("[PI] displaying options (case insensitive)\n");
			printf("==================================================\n");
			printf("Movement Commands:\n");
			printf("F - Forward, B - Reverse, R - Right, L - Left\n");
			printf("S - Stop, A - Approach, D - Get dist\n");
			printf("--------------------------------------------------\n");
			printf("Status Commands:\n");
			printf("G - Get status, C - Clear status\n");
			printf("==================================================\n");
			break;
		
		case 'a': //approach obj
		case 'A':
			printf("[PI] approach command\n");
			flushInput();
			commandPacket.command = COMMAND_APPROACH;
			sendPacket(&commandPacket);
			break;
			
		case 'd':
		case 'D':
			printf("[PI] get dist command\n");
			flushInput();
			commandPacket.command = COMMAND_GETDIST;
			sendPacket(&commandPacket);
			break;
			
		case 'f': //forward
		case 'F':
			printf("[PI] forward command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_FORWARD;
			sendPacket(&commandPacket);
			break;

		case 'b': //reverse
		case 'B':
			printf("[PI] reverse command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_REVERSE;
			sendPacket(&commandPacket);
			break;

		case 'l': //left
		case 'L':
			printf("[PI] left command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_TURN_LEFT;
			sendPacket(&commandPacket);
			break;

		case 'r': //right
		case 'R':
			printf("[PI] right command\n");
			getParams(&commandPacket);
			commandPacket.command = COMMAND_TURN_RIGHT;
			sendPacket(&commandPacket);
			break;

		case 's': //stop
		case 'S':
			printf("[PI] stop command\n");
			commandPacket.command = COMMAND_STOP;
			sendPacket(&commandPacket);
			break;

		case 'c': //clear status
		case 'C':
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

		case 'q': //exit
		case 'Q':
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
