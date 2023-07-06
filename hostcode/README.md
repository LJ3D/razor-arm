# General File Info
* hostcode/main.cpp -> Holds the code for the control from the computer /-/ Contains the Keyboard Input codes


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
* adjustJointPos(arduinoSerial& Serial, int idx, double adj) -> Moves motor "idx" by "adj" degrees /-/ sends "MOVE [idx] [adj]" over serial
* setJointPositions(arduinoSerial& Serial, std::vector<double> positions) -> Moves all motors to the corresponding position in "positions" /-/ currently uncalled /-/ Sends "SETX [positions]" over serial
* setSpeed(arduinoSerial& Serial, int speed) -> Sets the movement speed of all motors to "speed" degrees per second /-/ Sends "SPEED [speed]" over serial /-/ "speed" is an int due to parsing issues with the double passed into "SPEED"
