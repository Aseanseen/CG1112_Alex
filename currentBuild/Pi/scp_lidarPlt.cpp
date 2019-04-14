#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
 
int main() {
	//std::cout << "SCP lidar files\n";
	std::system("scp -i ~/.ssh/keyAlex pi@192.168.43.197:~/Desktop/CG1112_Alex_Team-2-02-2/currentBuild/Pi/lidar_reading.dat ~/Desktop/alexLidar");
	//std::system("scp -i ~/.ssh/keyAlex pi@192.168.43.197:~/Desktop/lidar.txt ~/Desktop");
	return 0;
}
