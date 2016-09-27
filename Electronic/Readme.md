# NoFollow - Electronics

<div style="align=middle"><img src ="http://i.giphy.com/EJ3tXBzfkZKDu.gif" /></div>


## CircuitMakerProject link <a href="https://workspace.circuitmaker.com/Projects/Details/Marcelo-Farias/NoFollowBoard">here</a>

## General Stuff

This repository keeps all the information about the electronics for the line following robot ‘NoFollow’.

THE MOST IMPORTANT ADVICE YOU CAN EVER GET ABOUT USING CIRCUITMAKER IS:
DONT. TRUST. DATABASE. COMPONENTS.

We did. It was not good.
Many of them are wrong - be it wrong sizes, wrong pins in the wrong places, wrong names, whatever! Don’t EVER 
trust them! Never! If possible, make them yourself! We had TONS of troubles because of this!

Anyway, proceeding:

To control the robot, we chose the Atmega 32U4 microcontroler (the same one that arduino Leonardo uses). 

However, this choice proved inadequate, since it’s memory was too small and couldn’t fit the entire intended 

program. We had to scrap the IMU because it’s library was too heavy and the LCD library had to suffer some
modifications to fit.

Here are the project sheets:

## Project Sheets

### `Main Sheet`:

![Main Sheet](https://s3.postimg.org/jhgaha7dv/Main.jpg)

  This sheet just links all the other sheets together. The idea is that the 'Core' part does all the processing while
the other sheets worry about interfacing or power-related stuff.

### `Core Sheet`: 

![Core Sheet](https://s3.postimg.org/gtqu6xecj/Core.jpg)

  In the core sheet we have the microcontroller and the necessary components to make it work properly - like a 
crystal oscillator and some capacitors. It also contains the usb header and its basic circuit.
The reset button is also included.

### `Power Sheet`:

![Power Sheet](https://s3.postimg.org/cbojetyar/Power.jpg)

  This is the power sheet, and is filled with (duh) power related stuff.
This robot was designed to work with a battery of 7.4V which was connected to a voltage regulator LM1117 to 
supply 5V continuous. It has a contact to keep the circuit safe when turning on and off.

### `Sensor Sheet`:

![Sensor Sheet](https://s3.postimg.org/jqdv77k6b/Sensors.jpg)

  We tried to put all the input/output devices in this sheet.
To detect the line on the ground we chose two analog arrays of eight infrared sensors, one overlapping the
other. Since the ATmega32U4 doesn't have 16 analog I/O’s, we had to use a mutiplex to access all 16 inputs
through only one pin.
  However, due in part to the small size of the Atmega’s memory and due in part to last-minute assembly, we only 
used one sensor. The robot worked anyway, but we don’t know if it would have worked better with two of them.
  Note about the MUX: we incorrectly used a big package, but there are smaller ones (and easier to find!) if you 
want to. We would have used them, but again: don’t trust database components.
  The capacitors are necessary for filtering the IMU (whatever that means).
  The LED_ON gate is connected directly to the ATMEGA, so as to turn off the LEDs when not in use and preserve 
battery. In hindsight, this was mostly useless.
  The buzzer was never used due to the memory shortage problem already cited.
  The button worked okay like this BUT it was placed wrongly on the circuit board. It was placed in a 45º angle, 
but the button works with a 90º axis so, for example, UP became UP-RIGHT and LEFT became UP-LEFT, and so forth. 
Like a compass turned 45 degrees to the right, you get it?

### `Comunications Sheet`:

![Comunications Sheet](https://s3.postimg.org/cw3khirj7/Communication.jpg)

  This sheet was supposed to be for all communications - which eventually turned out to be just a bluetooth 
module.
  HC05 is a simple bluetooth module that worked fine. The small circuit on its left is a thingy that steps the 
voltage up and down so the HC05 doesn’t burn the Atmega and vice-versa. It’s very important.

### `Motor Sheet`:
![Motor Sheet](https://s3.postimg.org/usiy5ne8z/Motors.jpg)

  This is the sheet where all motors and things-that-go-spinning went.
There is an H-bridge for motor control, complete with all the stuff it needs to work, and the motors all have 
diodes to prevent dangerous reverse currents. 
  There are also LED’s that indicate if the motor is going forwards or backwards. These were just for debugging, 
so they aren’t necessary, but they honestly look pretty cool. I recommend them.
IMPORTANT: while routing, we used small copper lines to connect the motors to the H-bridge. We later fixed it, 
which was very good, because or else we would have the first ever line-burning robot.
  However, the H-bridge has a big metal plate underneath it to use as ground/heatsink. So don’t repeat our
mistake and don’t put routes underneath the H-bridge. We had to cut out some isolant tape and glue it on the 
H-bridge, which worked, but you can do better than that.

## Routed Board
![Routed Board](https://s3.postimg.org/6b29t1ivn/pcb.jpg)

This is the final routed board. Notice the huge ground lines - these are important, since the motors are going 
to draw a LOT of current. 
  IMPORTANT: the LCD display is routed incorrectly. This was a CircuitMaker bug which (apparently) didn’t save a 
fix we made to the LCD part and ended up carrying a wrongly-pinned LCD to the routing process and, basically, 
gave us trouble.

## About soldering:
Please, PLEASE check if your PCB manufacturing company can make metallized vias. To our surprise, ours didn’t, 
which meant we had to solder 200+ metal bits to make them work, and it was generally unpleasant and caused us a 
lot of headache.
