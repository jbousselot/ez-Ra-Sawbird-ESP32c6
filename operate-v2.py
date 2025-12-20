#!/usr/bin/python3
# updated version of the python code to flip the state on the ESP32 connected Sawbird.
# this includes a shorter timeout and will return the HTTP code to help with debugging
import argparse
import requests
from requests.exceptions import Timeout

def make_http_ON(url):
    try:
        response = requests.get(url, timeout=3)
        print(response.text,response.status_code)
        #print('HTTP Status Code:', response.status_code)
        #print('Response Content:\n', response.text)
    except Timeout:
        print("The request timed out")

def make_http_OFF(url):
    try:
        response = requests.get(url, timeout=3)
        print(response.text,response.status_code)
        #print('HTTP Status Code:', response.status_code)
        #print('Response Content:\n', response.text)
    except Timeout:
        print("The request timed out")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Switch Nooelec Sawbird modes')
    parser.add_argument('option', choices=['ON', 'OFF'], type=str.upper, help="Choose either 'ON' or 'OFF'") # case independent
    parser.add_argument('switchnum', choices=['1', '0'], help="One or Zero") # not used but kept for syntax compatibility
    args = parser.parse_args()

    if args.option == "ON":
        make_http_ON("http://ESP-IP-ADDR/sawbird/operate")
    elif args.option == "OFF":
        make_http_OFF("http://ESP-IP-ADDR/sawbird/50ohm")
