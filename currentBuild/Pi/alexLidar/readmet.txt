===================================================================================================
Alex GNUPLOT Remote Desktop Plotter
===================================================================================================
GNUPLOT Updater / Plotter for Remote Desktop

How to use:
update.exe - open a bash terminal, call update.exe when needed, to update the plots
plotter.exe - open in cmd terminal, use to access the 5 most recent plots


===================================================================================================
Setup Details: SCP for lidar_reading.dat for usage in GNUPLOT
===================================================================================================
README details how to setup the executable for automating SCP for lidar_reading.dat
allows for SSH access w/o entering RPI password

Steps to implement:
1. 	Create a SSH key, based on this link: 
	https://www.techrepublic.com/article/how-to-use-secure-copy-with-ssh-key-authentication/
	https://www.ssh.com/ssh/copy-id
	- navigate to .ssh folder in remote desktop
	>> ssh ~/.ssh
	
	- create a new RSA SSH key
	>> ssh-keygen -t rsa
	
	- name the new key:
	>> keyAlex
	
	- do not set a passphrase (press enter twice)
	>> 
	>> 
	
	The following output should be seen:
	
	Generating public/private rsa key pair.
	Enter file in which to save the key (/c/Users/Admin/.ssh/id_rsa): keyAlex
	Enter passphrase (empty for no passphrase):
	Enter same passphrase again:
	Your identification has been saved in keyAlex.
	Your public key has been saved in keyAlex.pub.
	The key fingerprint is:
	SHA256:<code> Admin@<deviceName>
	The key's randomart image is:
	+---[RSA 2048]----+
	|+.               |
	|+...             |
	|.o. + o          |
	| . o O o .       |
	|+ =.B * S        |
	|=*=* B + .       |
	|+*OE+.+          |
	|.o.Bo.           |
	|  o++..          |
	+----[SHA256]-----+
	
	- SSH key is now generated
	- Copy over SSH key to Alex
	>> ssh-copy-id pi@<ipAddress>

2. 	Copy over alexLidar folder to desktop

3.	Copy files
	- call SCP with SSH key
	>> scp -i ~/.ssh/id_rsa.pub USER@SERVER:/home/USER/FILENAME /home/USER/FILENAME
	
	In this case:
	>> scp -i ~/.ssh/keyAlex pi@<IP_ADDRESS>:~/Desktop/Alex/Pi/lidar_reading.dat ~/Desktop/alexLidar
	
	Connecting via SSH w/o password:
	>> ssh -i ~/.ssh/keyAlex pi@<IP_ADDRESS>

===================================================================================================