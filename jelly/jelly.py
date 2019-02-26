from scapy.all import *
from collections import namedtuple
import csv
import sys
import time

s=conf.L2socket(iface="wlan0")
prep=None
js=None
nextjs=None

bytelist=[random.randint(-128,127) for _ in range(1526)]

JamSetting=namedtuple("JamSetting", "timestamp channel power period  length")

def get_ip_address():
    ipaddr=None
    while(ipaddr==None):
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect(("8.8.8.8", 80))
            ipaddr=s.getsockname()[0]
        except:
            pass
    return ipaddr

def update(js):
    print js
    global prep

    rt=RadioTap(len=18, present='Flags+Rate+Channel+dBm_AntSignal+Antenna')
    rt.Rate=2
    ch=js.channel
    ip=int(get_ip_address().split(".")[-1])
    if(ch==0):
        ch=(ip%14)+1;
    rt.Channel=ch
    #rt.dBm_TX_Power=int(js.power)
    rt.dBm_AntSignal=-1*int(js.power)
    hdr=Dot11(addr1='ff:ff:ff:ff:ff:ff',addr2='00:11:22:33:44:55', addr3='00:11:22:33:44:55')

    l=int(js.length)
    sub=bytelist[0:l]
    buf = struct.pack('%sb' % l, *sub)
    pl=Raw(load=buf)

    doty=hdr/pl
    pkt=rt/doty
    prep=pkt.build()

start=None
with open(sys.argv[1],mode="r") as csvfile:
    reader=csv.reader(csvfile,quoting=csv.QUOTE_NONNUMERIC)
    start=time.time()
    while True:
        if nextjs==None:
            js=JamSetting(*next(reader))
            update(js)
        else:
            js=nextjs
        try:
            nextjs=JamSetting(*next(reader))
        except StopIteration:
            pass
            nextjs=None
        while True:
            if not (nextjs==None) and (time.time()>(start+(nextjs.timestamp/1e9))):
                update(nextjs)
                break
            if(js.power!=0):
                x=s.send(prep)
            time.sleep(js.period/1000.0)
