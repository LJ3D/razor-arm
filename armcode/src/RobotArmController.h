#ifndef BRACCIORAZORFIRMWARE_ROBOTARMCONTROLLER_H
#define BRACCIORAZORFIRMWARE_ROBOTARMCONTROLLER_H

#include <Arduino.h>
#include <CommandParser.h>

class RobotArmController;
typedef CommandParser<99, 6, 10, 15, 64> RobotCommandParser;

class RobotArmController
{
private:
    RobotCommandParser *parser;
    inline static RobotArmController *instance = nullptr;
    float angles[6];

public:
    static RobotArmController *Instance();
    bool Setup();
    void Loop();
    void Home();
    void LimpMode(bool isLimp);
    void Read();
    void SetCoordinates(RobotCommandParser::Argument *args, char *response);
    void SetServo(RobotCommandParser::Argument *args, char *response);
    void MoveServo(RobotCommandParser::Argument *args, char *response);
    void SetSpeed(RobotCommandParser::Argument *args, char *response);
};

#endif // BRACCIORAZORFIRMWARE_ROBOTARMCONTROLLER_H
