#include "RobotArmController.h"
#include <CommandParser.h>
#include <Braccio++.h>
#include <string.h>

lv_obj_t *statusLabel;
static float const HOME_POS[6] = {157.5, 157.5, 187.5, 57.5, 157.5, 90.0};

RobotArmController *RobotArmController::Instance()
{
    if (instance == nullptr)
    {
        instance = new RobotArmController();
    }
    return instance;
}

bool RobotArmController::Setup()
{
    parser = new RobotCommandParser();

    bool successful = parser->registerCommand("LIMP", "i", [](auto *args, char *response)
                                              { RobotArmController::instance->LimpMode((args[0].asInt64 > 0)); });

    successful = successful && parser->registerCommand("READ", "", [](auto *args, char *response)
                                                       { RobotArmController::instance->Read(); });

    successful = successful && parser->registerCommand("HOME", "", [](auto *args, char *response)
                                                       { RobotArmController::instance->Home(); });

    successful = successful && parser->registerCommand("SETX", "dddddd", [](auto *args, char *response)
                                                       { RobotArmController::instance->SetCoordinates(args, response); });

    successful = successful && parser->registerCommand("SET", "id", [](auto *args, char *response)
                                                       { RobotArmController::instance->SetServo(args, response); });

    successful = successful && parser->registerCommand("MOVE", "id", [](auto *args, char *response)
                                                       { RobotArmController::instance->MoveServo(args, response); });

    successful = successful && parser->registerCommand("SPEED", "d", [](auto *args, char *response)
                                                       { RobotArmController::instance->SetSpeed(args, response); });
    return successful;
}

void RobotArmController::Loop()
{
    if (Serial.available())
    {
        char line[128];
        size_t lineLength = Serial.readBytesUntil('\n', line, 127);
        line[lineLength] = '\0';
        
        char response[RobotCommandParser::MAX_RESPONSE_SIZE];
        parser->processCommand(line, response);
        Serial.println(response);
    }
}

void RobotArmController::LimpMode(bool isLimp)
{
    if (isLimp)
    {

        Braccio.disengage();
    }
    else
    {
        Braccio.engage();
    }
}

void RobotArmController::Read()
{
    Braccio.positions(angles);

    char buffer[1000];
    sprintf(buffer, "[%.2f,%.2f,%.2f,%.2f,%.2f,%.2f]", angles[0], angles[1], angles[2], angles[3], angles[4], angles[5]);
    Serial.println(buffer);
}

void RobotArmController::Home()
{
    angles[0] = HOME_POS[0];
    angles[1] = HOME_POS[1];
    angles[2] = HOME_POS[2];
    angles[3] = HOME_POS[3];
    angles[4] = HOME_POS[4];
    angles[5] = HOME_POS[5];

    Braccio.moveTo(angles[0], angles[1], angles[2], angles[3], angles[4], angles[5]);
    Serial.println("OK");
}

void RobotArmController::SetCoordinates(RobotCommandParser::Argument *args, char *response)
{
    angles[0] = (float)args[0].asDouble;
    angles[1] = (float)args[1].asDouble;
    angles[2] = (float)args[2].asDouble;
    angles[3] = (float)args[3].asDouble;
    angles[4] = (float)args[4].asDouble;
    angles[5] = (float)args[5].asDouble;

    Braccio.moveTo(angles[0], angles[1], angles[2], angles[3], angles[4], angles[5]);
    Serial.println("OK");
}

void RobotArmController::SetServo(RobotCommandParser::Argument *args, char *response)
{
    int servo = args[0].asInt64;
    float angle = (float)args[1].asDouble;

    Braccio.move(servo).to(angle);
    Serial.println("OK");
}

void RobotArmController::MoveServo(RobotCommandParser::Argument *args, char *response)
{
    auto servo = Braccio.get(args[0].asInt64);
    float angleIncrement = (float)args[1].asDouble;

    servo.to(servo.position() + angleIncrement);
    Braccio.move(servo).to(angleIncrement);
    Serial.println("OK");
}

void RobotArmController::SetSpeed(RobotCommandParser::Argument *args, char *response)
{
    double speed = args[0].asDouble;
    if(speed > 120 || speed < 1){
        Serial.println("Invalid speed");
    }else{
        Braccio.setAngularVelocity(speed);
        Serial.println("OK");
    }
}