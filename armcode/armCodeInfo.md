# General File Info
* hostcode/main.cpp -> Holds the code for the control from the computer /-/ Contains the Keyboard Input code
* armcode/src/main.cpp -> Defines arm initialisation functions
* armcode/src/RobotArmController.cpp -> Defines the serial commands, including the home position constant
* .pio/libdeps/nanorp2040connect/Arduino_Braccio_plusplus/src/braccio.h -> defines "Braccio" as type "BraccioClass", as well as defining "BraccioClass" and "Servo" classes


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

* "LIMP" -> Disengage all motors. Takes 1 integer, fails if it is < 1
* "READ" -> Retrieve the current positions of each motor
* "HOME" -> Return to natural position (157.5, 157.5, 157.5, 157.5, 257.5, 90.0)
* "SETX dddddd" -> Move all joints to a position. Takes 6 doubles, one for each joint
* "SET id" -> Move a joint to a certain position. Takes an int for the joint and a double for the angle
* "MOVE id" -> Move a joint by a certain amount. Takes an int for the joint and a double for the angle
* "SPEED d" -> Adjust the speed of the motors to "d" deg/sec. 


# Keyboard Inputs
* "H" -> Return the arm to its home position /-/ Calls homeArm(arduinoSerial& Serial)
* "P" -> Prints the current positions of all motors /-/ Calls getJointPositions(arduinoSerial& Serial)
* "R","F" -> Moves motor id 1 (gripper, known issues) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ "F" calls with "adj" argument inverted
* "Q","E" -> Moves motor id 2 (gripper rotation) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ "E" calls with "adj" argument inverted
* "A","D" -> Moves motor id 3 (top joint) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ "D" calls with "adj" argument inverted
* "W","S" -> Moves motor id 4 (middle joint) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ "S" calls with "adj" argument inverted
* U-Arw,D-Arw -> Moves motor id 5 (bottom joint) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ D-Arw calls with "adj" argument inverted
* R-Arw,L-Arw -> Moves motor id 6 (base rotation) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ L-Arw calls with "adj" argument inverted


# Keyboard-Called Commands
* homeArm(arduinoSerial& Serial) -> Sets all motors to their home position /-/ sends "HOME" over serial
* getJointPositions(arduinoSerial& Serial) -> Retrieves the positions of all motors as a vector /-/ sends "READ" over serial
* adjustJointPos(arduinoSerial& Serial, int idx, double adj) -> moves motor "idx" by "adj" degrees /-/ sends "MOVE [idx] [adj]" over serial
* setJointPositions(arduinoSerial& Serial, std::vector<double> positions) -> moves all motors to the corresponding position in "positions" /-/ currently uncalled
* setSpeed(arduinoSerial& Serial, int speed) -> Sets the movement speed of all motors to "speed" degrees per second /-/ Sends "SPEED [speed]" over serial /-/ "speed" is an int due to parsing issues with the double passed into "SPEED"