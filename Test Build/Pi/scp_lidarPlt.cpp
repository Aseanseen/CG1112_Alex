#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
 
int main() {
	std::cout << "SCP lidar files\n";
	std::system("scp -i ~/.ssh/keyAlex pi@192.168.43.197:~/Desktop/lidar.txt ~/Desktop");
	return 0;
}