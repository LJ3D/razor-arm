# General File Info
* hostcode/main.cpp -> Holds the code for the control from the computer /-/ Contains the Keyboard Input codes


# Keyboard Inputs
* "H" -> Return the arm to its home position /-/ Calls homeArm(arduinoSerial& Serial)
* "'" -> Return the arm to its home and resets its speed /-/ Calls homeArm(Serial) and setSpeed(Serial)
* "P" -> Prints the current positions of all motors /-/ Calls getJointPositions(arduinoSerial& Serial)
* "R","F" -> Moves motor id 1 (gripper, known issues) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ "F" calls with "adj" argument inverted
* "Q","E" -> Moves motor id 2 (gripper rotation) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ "E" calls with "adj" argument inverted
* "A","D" -> Moves motor id 3 (top joint) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ "D" calls with "adj" argument inverted
* "W","S" -> Moves motor id 4 (middle joint) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ "S" calls with "adj" argument inverted
* U-Arw,D-Arw -> Moves motor id 5 (bottom joint) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ D-Arw calls with "adj" argument inverted
* R-Arw,L-Arw -> Moves motor id 6 (base rotation) /-/ Calls adjustJointPos(arduinoSerial& Serial, int idx, double adj) /-/ L-Arw calls with "adj" argument inverted
* SPC -> Triggers the wave animation /-/ calls wave(Serial)
* ENT -> Triggers the pick up animation /-/ Calls pickUp(Serial)
* "," -> Triggers the worm animation /-/ Calls worm(serial)
* "c" -> Triggers random movements /-/ calls chaos(Serial, false)
* L-Sh + "c" -> Triggers random movement at maximum speed /-/ Calls chaos(Serial, true)
* "[" -> Decreases the speed of all motors /-/ Calls setSpeed(Serial, curr_speed - SPEED_ADJUSTMENT)
* "]" -> Increases the speed of all motors /-/ -> Calls setSpeed(Serial, curr_speed + SPEED_ADJUSTMENT)
* "-" -> Decrease the angle adjustment by 1 /-/ Sets pos_adjustment to itself - 1
* "=" -> Increase the angle adjustment by 1 /-/ Sets pos_adjustment to itself + 1
* "L" Toggles limp mode /-/ Sends either "LIMP 1" or "LIMP 0" over the serial


# Keyboard-Called Commands
* homeArm(arduinoSerial& Serial) -> Sets all motors to their home position /-/ sends "HOME" over serial
* getJointPositions(arduinoSerial& Serial) -> Retrieves the positions of all motors as a vector /-/ sends "READ" over serial
* adjustJointPos(arduinoSerial& Serial, int idx, double adj) -> Moves motor "idx" by "adj" degrees /-/ sends "MOVE [idx] [adj]" over serial
* setJointPositions(arduinoSerial& Serial, std::vector<double> positions) -> Moves all motors to the corresponding position in "positions" /-/ currently uncalled /-/ Sends "SETX [positions]" over serial
* setSpeed(arduinoSerial& Serial, int speed, bool noset = false) -> Sets the movement speed of all motors to "speed" degrees per second /-/ Will not store the new speed in "noset" is true /-/ Values of "speed" greater than 120 will set it to 120, as will values less than 10 to 10 /-/ Sends "SPEED [speed]" over serial /-/ "speed" is an int due to parsing issues with the double passed into "SPEED"
* wave(arduinoSerial& Serial), pickUp(arduinoSerial& Serial), worm(arduinoSerial& Serial) -> Pre-coded animations /-/ Each call setJointPositions() for each position in the animation
* chaos(arduinoSerial& Serial, bool death = false) -> Sequence of 20 randomly generated positions /-/ If "death" is true, will call setSpeed(Serial, 120, true) and then setSpeed(Serial, curr_speed) /-/ Calls setJointPositions() for each position
