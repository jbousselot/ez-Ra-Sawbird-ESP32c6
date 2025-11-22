A project for Radio Telescope remote operations.  
Nooelec Sawbird antenna switch has a feature to ground the RF signal to a 50ohm load, which is used to determine what is background noise.
The ezRa software calls a very specific command to implement the change on the sawbird, passing two arguments to the exe file.  
This was writtien in python on linux, and accepts two command args, but ignores the second.

Original ezRa software link here...

Receiving hardware is an ESP32c6 from Seeed Studio, with U.FL antenna port and external PCB dual band antenna for 2.4 and 5.8 ghz.  Can make a simple wire antenna with 31mm of coax exposed or simply use the onboard ceramic antenna.  
