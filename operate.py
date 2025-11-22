#!/usr/bin/python3
import requests
import sys

if len(sys.argv) > 1:
    command = sys.argv[1].lower()
    if command == 'on':
       x = requests.get('http://ESP-IP-ADDR/sawbird/operate')
       print(x.text)
    elif command == 'off':
       x = requests.get('http://ESP-IP-ADDR/sawbird/50ohm')
       print(x.text)
    else:
        print("Usage: ",sys.argv[0], "[ON|OFF] 1")
        pass
else:
    print("Usage: ",sys.argv[0], "[ON|OFF] 1")
