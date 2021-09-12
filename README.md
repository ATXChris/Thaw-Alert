# Thaw Alert

Set-up ubuntu VM <br />
	will need to install gcc perl package on guest additions step
	https://brb.nci.nih.gov/seqtools/installUbuntu.html
pull ESP8266_RTOS_SDK
sudo apt-get install linux-source
sudo apt install libncurses-dev
sudo apt install ncurses-dev kernel -package
sudo apt install flex
sudo apt install bison
sudo apt install gperf
sudo apt install python3-pip
Install virtualbox USB driver manually.
	goto folder C:\Program Files\Oracle\VirtualBox\drivers\USB\filter
	right click VboxUSBMon.inf
	install
	reboot

Use terminal to navigate to Thaw_Alert directory

export PATH="$HOME/esp/xtensa-lx106-elf/bin:$PATH"
export IDF_PATH=~/esp/ESP8266_RTOS_SDK
make all

