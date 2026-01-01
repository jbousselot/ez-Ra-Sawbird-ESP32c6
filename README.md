# Remote Switching for the SAWbird+ H1t LNA using an ESP32

September 2025, I was asked to help with a radio telescope project located at the San Diego Astronomy Association, in the mountains of San Diego county.  The team constructed a dish and feed horn mounted on telescope post, and pointed at the sky.  Many optical telescopes at this location have domes or retractable roof structures and a modest amount of space to host computer equipment.  Partnering with an optical telescope member at the site and using a small form factor PC with USB SDR, the team deployed ezRA, Easy Radio Astronomy. https://github.com/tedcline/ezRA 

I was given documentation about the ezRA software, Nooelec SAWbird+ H1t, and the switching challenges encountered during design and deployment and was tasked with the challenge, "how can we activate the LNA module 50ohm switch function remotely from the PC?"  This is function built into the ezRA code used to baseline background noise.

This task could easily be handled by an ESP32 microcontroller as the site has a wifi network through a Starlink AP gateway.  Connect the ESP32 to the IP network, install it near the SAWbird, and let the ezRA software issue control commands from the PC.  5v was available behind the dish and the AP is less then 50 feet away.  Prototype testing began with two ESP32 modules, an ESP32-C3 SuperMini and Seeed Studio ESP32-C6.  This deployment needed to fit inside the electric box behind the dish, powered by a 5v supply, and reliably perform two way telemetry to the telescope hut.

Both the C3 and C6 have an onboard ceramic antenna, but the C6 comes with an external antenna port, allowing physical separation of the 2.4GHz WiFi signal from the SAWbird.  Using a small piece of coax harvested from other WiFi equipment, we are able to place the radiating antenna element outside the electrical box.  During testing, the ceramic antenna on the C3 and C6 worked fine between the hut and the box, but separating the 2.4GHz RF away from the LNA was a desirable option we all thought would be helpful, so the C6 was selected.  A piece of coax with a U.FL connector and about 10" of U137 is what we used, things you can get by completely disassembling dead laptops to remove the WiFi antennas, or they can be purchased as pigtails from various online sources.  Amazon and AliExpress have them in multiple lengths and connectors for cheap.  Carefully cut about 31mm of shield from the coax and the exposed core conductor is now your antenna element.  

ESP32 boards use 3.3v signaling on the IO pins, requiring an additional circuit of a transistor and mosfet to cleanly switch the 5v control pin.

When the ESP32 boots, the default mode is pin D10 LOW, which places the SAWbird in normal operate mode.  When ezRA wants to baseline the background noise, it sends an HTTP request to the ESP32, instructing pin D10 to go HIGH, switching the transistor and mosfet to bring the Control pin on SAWbird high, and activating the RF switch to a 50ohm ground.  Samples are taken for a few seconds and the command is issued to return pin D10 to LOW.

The total cost for all parts is less than $25.  The C6 boards are commonly sold as singles or bundles of three. Wiring harnesses are inexpensive in quantity.  Individual eletronic components are inexpensive and can be ordered in small quantities from Mouser or Digikey but will have minimum shipping costs.  This circuit uses two resistors, a decoupling capacitor, and a common transistor and mosfet.   Ask a friend or shop smartly!


# How To Implement - how WE implemented it
The ezRA software program "ezCol.py" makes a call to hidusb-relay-cmd.exe, a program that that accepts two command line flags, ON/OFF and a number to signify which relay to operate on multi-relay boards.<br/>
[WINDOWS/OSX/LINUX](https://github.com/pavel-a/usb-relay-hid)<br/>  
[LINUX](https://github.com/darrylb123/usbrelay)<br/>

The python scripts here were complied into an exe and named the same to match what ezRA code expects.  We chose this path to simplify deployments and updates.  When a new github version of ezRA is released, we download and install, making sure to place the hidusb-relay-cmd.exe and supporting directory where the application expects to see the file.  
Compiling python into a windows EXE is very easy, but it takes a 722 byte python file and turns it into a 20MB bundle.  

Place the script into a new directory and name it usb-relay-cmd.py <br/>
pyinstaller usb-relay-cmd.py <br/>
{compiling work will be done} <br/>
A usb-relay-cmd.exe file will be in the dist directory, also a "_internal" directory.  Copy both of these into the directory where ezRA expects to find the exe file.<br/>
The ezRA code will send the command ON/OFF and relay number to the program, which makes an HTTP request to the ESP32 and modifies pin D10 accordingly.<br/>
