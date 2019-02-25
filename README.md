# JamLab-NG

JamLab-NG is a tool for evaluating the performance of low-power wireless protocols in noisy environments in a repeatable and fully-automated way.
This tool has been used to set-up the EWSN 2019 dependability competition.
* [EWSN 2019 Dependability Competition (Beijing, China)](http://ewsn2019.thss.tsinghua.edu.cn/competition-scenario.html)

A scientific paper about D-Cube was published at the 16th International Conference on Embedded Wireless Systems and Networks (EWSN), and will available soon.

## Details

Currently this repositiory contains the parts required to build configure and can be used with [Scapy](https://scapy.net/) as is. Jelly, our wrapper for parsing the CSV files will be added soon.

## Instructions

JamLab-NG relies on [nexmon](https://github.com/seemoo-lab/nexmon) to work on the Raspberry Pi 3 Model B. To get started follow the [guide](https://github.com/seemoo-lab/nexmon#build-patches-for-bcm43430a1-on-the-rpi3zero-w-or-bcm434355c0-on-the-rpi3-using-raspbian-recommended).

### Kernel Module with Confiture Scheduler

Clone this repository into ```patches/bcm43430a1/7_45_41_46/``` (as jamlabng next to the existing nexmon folder)  
Run ```make install-firmware``` and you now have a working version of JamlabNG installed on the RPI3.

### Confiture App

The version included does not rely on libnexutil but instead simply calls the existing nexutil binary in ```/usr/bin/```. Use the included makefile to build the executable.

### Jelly Tool

Jelly is build around Scapy and will be released soon!

### Gotchas
Keep in mind that libisl needs to be build first.  
Do not forget to first build the build tools, ucode, flashpatches etc by running make in the root of nexmon.  
When sourcing setup_env.sh as user, keep in mind that later operations requiring root will need to be run with ```sudo -E make xxx```. To avoid this you can source as root user (keep in mind, this is not very safe).  
Do not forget to install nexutil from utilities/nexutil/!

