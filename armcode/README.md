# General File Info
* armcode/src/main.cpp -> Defines arm initialisation functions
* armcode/src/RobotArmController.cpp -> Defines the serial commands, including the home position constant
* .pio/libdeps/nanorp2040connect/Arduino_Braccio_plusplus/src/braccio.h -> Defines "Braccio" as type "BraccioClass", as well as defining "BraccioClass" and "Servo" classes


# RobotArmController
* Setup() -> Checks validity of other commands for this class
* LimpMode(bool isLimp) -> Disengage all motors 
* Read() -> Grab current angles and print to screen
* Home() -> Return to natural position (157.5, 157.5, 187.5, 57.5, 157.5, 90.0)
* SetCoordinates(RobotCommandParser::Argument *args, char *response) -> Takes args as list containing angles to be moved to /-/ Called when joints need to be moved, moves all simultaneously /-/ Calls _servo.setPosition(uint8_t const id, float const angle_deg)
* SetServo(RobotCommandParser::Argument *args, char *response) -> Moves a specific motor to a position
* MoveServo(RobotCommandParser::Argument *args, char *response) -> Moves a specific motor by an angle
* SetSpeed(RobotCommandParser::Argument *args, char *response) -> needs testing, presumably sets speed in deg/sec


# SmartServoClass:
* setPosition(uint8_t const id, float const angle_deg) -> Moves a joint to angle_deg /-/ called by RobotArmController::SetCoordinates()


# Serial Commands
!! All Arguments must be separated by a space !!

* "LIMP" -> Disengage all motors. Takes 1 integer, will re-engage all motors if it is < 1 (or more accurately to the code, if it is not > 0)
* "READ" -> Retrieve the current positions of each motor
* "HOME" -> Return to natural position (157.5, 157.5, 157.5, 157.5, 257.5, 90.0)
* "SETX dddddd" -> Move all joints to a position. Takes 6 doubles, one for each joint
* "SET id" -> Move a joint to a certain position. Takes an int for the joint and a double for the angle
* "MOVE id" -> Move a joint by a certain amount. Takes an int for the joint and a double for the angle
* "SPEED d" -> Adjust the speed of the motors to "d" deg/sec
