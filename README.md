# NoFollow
The Software of NoBox's Line Follow robot

# Parts of the puzzle

* `FollowFirmware_v0.1`: Initializes all the classes (modules).

* `commander`: Handles data received via radio communication.

* `controller`: Controls the robots motor power, threads callbacks and PID constants.

* `motors`: Initiates the "pinMODEs", sets power and directions of the motors.

* `pid`: Configures PID control on the angular
velocity of the robot.

* `robot`: Configurates the correct pinMODEs and beeps, defines the current robot's state.

* `system`: Initializes and verifies the robot's activities, such as Serial communication, LEDs and battery level.

* `sensors`: Read the outside sensors and check the conditions.

# Contributors
* [Gustavo Silva de Paula](https://github.com/Gustavo7337)
* [Ivan Seidel](http://github.com/ivanseidel)
* [João Pedro Poloni Ponce](https://github.com/JoaoPedroPP)
* [Marcelo Farias](https://github.com/MarceloFariaz)
* [Yves-Medhard Tibe-Bi](https://github.com/yvesmedhard)
