#!/usr/bin/python3
# Python code to capture weater statistics from the ESP32 and BME280 module
# When this python3 code is run, it will create a new sqlite3 database for each month and insert weather stats as often as you collect them.  A new file is created each month.
# use the collected weather statistics from the Radio Telescope location to correlate data anomalies and influences.

import sqlite3
import requests
import re
import datetime

url = 'http://192.168.1.13/esp32/WEATHER-API'
db_name_prefix = 'boyce_astro' 
response = requests.get(url)
response.raise_for_status() 
text_content = response.text
numbers = re.findall(r"-?\d+\.?\d*", text_content)

database_name = db_name_prefix + "_" + datetime.datetime.now().strftime('%m_%Y') + ".sqlite3"
if len(numbers) >= 3:
    # Convert the first 3 captured string matches to actual floats
    temp_c, pressure_mb, humidity = [float(num) for num in numbers[:3]]

else:
    print("Failed to capture data from ESP32 ")

conn = sqlite3.connect(database_name)
cursor = conn.cursor()

cursor.execute('''
    CREATE TABLE IF NOT EXISTS boyceastro (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        temp_c REAL,
        pressure_mb REAL,
        humidity REAL,
        time_utc TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    )
''')

cursor.execute('''
    INSERT INTO boyceastro (temp_c, pressure_mb, humidity, time_utc)
    VALUES (?, ?, ?, CURRENT_TIMESTAMP)
''', (temp_c, pressure_mb, humidity))

conn.commit()
conn.close()

print("Data inserted successfully!")

