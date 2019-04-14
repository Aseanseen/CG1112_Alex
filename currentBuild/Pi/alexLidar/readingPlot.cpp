#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int exitFlag = 0;
int plotNum = 0;
char ch = ' ';

void flushInput() {
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}

int main() {
	while (!exitFlag) {
		printf("Enter Command:\n");
		scanf("%c", &ch);
		flushInput();
		
		switch(ch) {
			case 'h': //help
			case 'H':
				printf("===========================================================\n");
				printf("HELP MENU\n");
				printf("===========================================================\n");
				printf("P - Plot >> N (Nth most recent plot)\n");
				printf("U - Update existing plots\n");
				printf("===========================================================\n");
				break;
				
			case 'p':
			case 'P':
				printf("Select which of the 5 most recent plots: ");
				scanf("%d", &plotNum);
				flushInput();
				switch(plotNum) {
					case 1: std::system("gnuplot liplot1.plt --persist"); break;
					case 2: std::system("gnuplot liplot2.plt --persist"); break;
					case 3: std::system("gnuplot liplot3.plt --persist"); break;
					case 4: std::system("gnuplot liplot4.plt --persist"); break;
					case 5: std::system("gnuplot liplot5.plt --persist"); break;
					default: printf("Invalid\n");
				}
				break;
			
			//broken unless you can SCP and MV in terminal
			case 'u':
			case 'U':
				printf("Fetching new plot\n");
				std::system("scp -i ~/.ssh/keyAlex pi@192.168.43.197:~/Desktop/Alex/Pi/lidar_reading.dat ~/Desktop/alexLidar");
				printf("Updating plots\n");
				std::system("mv ~/Desktop/alexLidar/lidar_reading4.dat ~/Desktop/alexLidar/lidar_reading5.dat");
				std::system("mv ~/Desktop/alexLidar/lidar_reading3.dat ~/Desktop/alexLidar/lidar_reading4.dat");
				std::system("mv ~/Desktop/alexLidar/lidar_reading2.dat ~/Desktop/alexLidar/lidar_reading3.dat");
				std::system("mv ~/Desktop/alexLidar/lidar_reading1.dat ~/Desktop/alexLidar/lidar_reading2.dat");
				std::system("mv ~/Desktop/alexLidar/lidar_reading.dat ~/Desktop/alexLidar/lidar_reading1.dat");
				printf("Updated\n");
				break;
				
			default: printf("Invalid command\n");
		}
	}
	return 0;
}