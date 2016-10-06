# Srcotron - short instructions for use

## Assembly

1. Secure your Arduino/Iteaduino UNO in its place in the box
2. Attach Olimex ECG-EMG shield into the board, making sure the pin alignment is right
3. Close the box and connect the cables
4. Done!

## Minimal working conditions

* Assembled srcotron kit, a USB cable and ECG electrodes
* [Python 2.66](https://www.python.org/download/releases/2.6.6/) or any later 2.-- version + [pygame](http://www.pygame.org/download.shtml)
* Arduino IDE
* Arduino UNO or an equivalent board programmable via USB

## Uploading the code onto scrotron

Boils down to using Arduino IDE to load and upload the code file. Download the ekg_py.ino sketch from python/ekg_py and upload it to srcotron.
The sketch is set up to send readings via USB. Those readings will be caught and displayed by a python script test.py which you can also find
in the python map.

## Setting up the python scrpit

The only important setting to look out for is the name of your PC's COM port on which srcotron is connected. You can check that port via Arduino IDE
as it is also used to upload code to the board. The python script can be viewed and eddited by any primitive text edditing tool such as notepad. In the code
under "serial init", a setting called s.port must be changed to match the name of the COM port on which srcotron is connected.

## Using srcotron for measurement

Once the code is set up to run, all you need to do is run the python script and connect the electrodes properly! Signal, as well as pulse detection, are captured
and displayed in real time. Electrodes should be connected according to [Einthoven's triangle](http://www.medicine.mcgill.ca/physio/vlab/cardio/images/einthoven.jpg) for best performance.
Depending on your electrodes, markings and colours may differ. Use this simple algorithm which lets you set up the electrodes properly regardless of their markings:

0. If what you see is what you'd expect, great!
1. If the signal is inverted (displayed upside down), switch the left and right hand electrodes and -> 1.
2. If the signal shape is weird, switch the left leg electrode with any of the other two and -> 2.
