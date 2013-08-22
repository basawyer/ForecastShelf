import urllib2
import serial
import os
import time
import re

def doRead(ser,term):
    matcher = re.compile(term)    #gives you the ability to search for anything
    buff    = ""
    tic     = time.clock()
    buff   += ser.read(1)
    # you can use if not ('\n' in buff) too if you don't like re
    while ((time.clock() - tic) < 5):
       buff += ser.read(1)
       print buff
    if buff == "1\n":
       return 1
    else:
       return 0

# Connect to Xbee
response = urllib2.urlopen("http://forecastshelf.blakesawyer.net/getshelfdata.php?zip=24060").read()
ser = serial.Serial(port='COM3', baudrate=9600, timeout=1)

# Send data from URL Get request
#print 'Sending data first'
#ser.write(urllib2.urlopen("http://forecastshelf.blakesawyer.net/getshelfdata.php?zip=24060").read())
#print 'Waiting for response'
#response = doRead(ser,term='\n')
#while(1):
	# Send data from URL Get request
	#print 'Waiting for response'
ser.write(response)
ser.close()
	#response = doRead(ser,term='\n')
f = open('C:\ForecastShelf\sends.txt','a')
f.write(time.strftime("%Y-%m-%d %H:%M:%S")+" " +response+'\n')
f.close()
	#time.sleep(30)



