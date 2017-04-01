# NoFollow
The Software of NoBox's Line Follow robot of 2016
<div style="align=middle"><img src ="https://github.com/Project-Neon/NoFollow/blob/master/NoFollow.png" /></div>

<p align="center";> <b>NoFollow Final Version</b> </p>

# Parts of the puzzle

* `Firmware`: Is the main firmware that controls all the functions of the robot.
* `Eletric`: This keeps all the information about the electronics for the line following robot ‘NoFollow’.
* `Mechanic`: Contains the STL files of the robot design.

# Parts of the `Firmware` puzzle

* `activities`: Controls the robots motor power, threads callbacks and PID constants.

* `interface`: Show in the LED interface the time to stop the robot and read the comands by a joystick.

* `lineReader`: Read the outside sensors and check the conditions.

* `motors`: Initiates the "pinMODEs", sets power and directions of the motors.

* `pid`: Configures PID control on the angular
velocity of the robot.

* `robot`: Configurates the correct pinMODEs and beeps, defines the current robot's state.

* `runner`: Is the "menu" of the robot. In the interface receive the comands by the joystick to do tests actuators, calibrate steering and follow line.


# Contributors
* [Gustavo Silva de Paula](https://github.com/Gustavo-dPaula)
* [Ivan Seidel](http://github.com/ivanseidel)
* [João Pedro Poloni Ponce](https://github.com/JoaoPedroPP)
* [Lucas Andrade Cioffi](https://github.com/lacioffi)
* [Marcelo Farias](https://github.com/MarceloFariaz)
* [Yves-Medhard Tibe-Bi](https://github.com/yvesmedhard)
