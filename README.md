# Thaw Alert

Set-up ubuntu VM <br />
	will need to install gcc perl package on guest additions step
	https://brb.nci.nih.gov/seqtools/installUbuntu.html <br />
pull ESP8266_RTOS_SDK <br />
sudo apt-get install linux-source <br />
sudo apt install libncurses-dev <br />
sudo apt install ncurses-dev kernel -package <br />
sudo apt install flex <br />
sudo apt install bison <br />
sudo apt install gperf <br />
sudo apt install python3-pip <br />
Install virtualbox USB driver manually. <br />
- goto folder C:\Program Files\Oracle\VirtualBox\drivers\USB\filter <br />
- right click VboxUSBMon.inf <br />
- install <br />
- reboot <br />

Use terminal to navigate to Thaw_Alert directory <br />

export PATH="$HOME/esp/xtensa-lx106-elf/bin:$PATH" <br />
export IDF_PATH=~/esp/ESP8266_RTOS_SDK <br />
make all <br />

