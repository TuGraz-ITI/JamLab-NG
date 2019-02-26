# JamLab-NG

JamLab-NG is a tool for evaluating the performance of low-power wireless networking protocols in noisy RF environments. It enables the generation of repeatable and reproducible Wi-Fi interference using off-the-shelf Raspberry Pi3 devices.

JamLab-NG is the result of a cooperation between TU Graz and TU Darmstadt, and has been published at EWSN 2019 ([PDF](http://www.carloalbertoboano.com/documents/schuss19jamlab-ng.pdf)). It was also used in conjunction with D-Cube ([PDF](http://www.carloalbertoboano.com/documents/boano17competition.pdf) and [PDF](http://www.carloalbertoboano.com/documents/schuss18benchmark.pdf)) to set-up the EWSN 2019 dependability competition ([link](https://iti-testbed.tugraz.at/blog/tag/ewsn2019])).

This repository currently contains the parts required to build confiture and can be used with Scapy as is. Jelly is tested against the Rasperry Pi with JamLab-NG's confiture to support sending without CCA, using arbitrary channels, and varying transmission power.

## Preliminary Instructions

JamLab-NG's confiture relies on [nexmon](https://github.com/seemoo-lab/nexmon) to work on the Raspberry Pi. To get started follow the [guide](https://github.com/seemoo-lab/nexmon#build-patches-for-bcm43430a1-on-the-rpi3zero-w-or-bcm434355c0-on-the-rpi3-using-raspbian-recommended).

### Kernel Module with Confiture Scheduler

Clone this repository into ```patches/bcm43430a1/7_45_41_46/``` (next to the existing nexmon folder)  
After running ```make install-firmware``` from this repository, you should have a working version of JamlabNG installed on the RPI3 (you can use ```dmesg``` to check).

### Confiture App

The version included does not rely on libnexutil but instead simply calls the existing nexutil binary in ```/usr/bin/```. Use the included makefile to build the executable.

### Jelly Tool

Jelly is built around Scapy, so it needs to be installed first. Jelly is written for python2, please make sure you are not using python3. To install scapy, simply run ```pip install scapy```! To get started with Scapy as is, follow the guide to put your Wi-Fi card into [monitor mode](https://github.com/seemoo-lab/nexmon/tree/master/#using-the-monitor-mode-patch-1).
To put the wlan0 directly into monitor mode, run ```sudo nexutil -m1```. Jelly assumes that wlan0 is the interface in monitor mode.


### Gotchas
Keep in mind, that libisl needs to be build first.  
Do not forget to first build the build tools and extract parts (ucode, flashpatches) from the current firmware by running ```make``` in the root of nexmon.  
When sourcing setup_env.sh as user, keep in mind that later operations requiring root will need to be run with ```sudo -E make xxx```. To avoid this you can source as root user (keep in mind, this is not very safe).  
Do not forget to install nexutil from utilities/nexutil/!
For Scapy/Jelly the card needs to be but into monitor mode first by running ```sudo nexutil -m1```.  


Additional information will be added soon!
