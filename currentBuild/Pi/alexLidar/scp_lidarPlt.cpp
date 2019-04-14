/**
 * Alex SCP lidar_reading.dat
 * creates an executable to be called in terminal
 * SCPs lidar_reading.dat into alexLidar folder in desktop (templated)
 * rmb to change based on your ssh key that's authenticated w. Alex
 */
#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
int main() {
	//std::cout << "SCP lidar files\n";
	printf("Fetching new plot\n");
	std::system("scp -i ~/.ssh/keyAlex pi@192.168.43.197:~/Desktop/Alex/Pi/lidar_reading.dat ~/Desktop/alexLidar");

	printf("Updating plots\n");
	std::system("rm lidar_reading5.dat");
	std::rename("lidar_reading4.dat", "lidar_reading5.dat");
	sleep(1);
	std::rename("lidar_reading3.dat", "lidar_reading4.dat");
	sleep(1);
	std::rename("lidar_reading2.dat", "lidar_reading3.dat");
	sleep(1);
	std::rename("lidar_reading1.dat", "lidar_reading2.dat");
	sleep(1);
	std::rename("lidar_reading.dat", "lidar_reading1.dat");

	printf("Updated\n");
	return 0;
}
