#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "gl_debug.hpp"
#include "arduinoserialio/arduinoSerial.hpp"
#include <thread>
#include <vector>

#define DEBUG true

std::vector<double> getJointPositions(arduinoSerial& Serial){
    char response[256];
    Serial.flush();
    Serial.print("READ");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Serial.readBytes(response, 256);
    std::cout << response << std::endl;
    double numbers[6] = {0};
    if(std::sscanf(response, "READ\n[%lf,%lf,%lf,%lf,%lf,%lf]", &numbers[0], &numbers[1], &numbers[2], &numbers[3], &numbers[4], &numbers[5]) != 6){
        std::cout << "ERR: Failed to parse response\n";
    }
    std::vector<double> v(numbers, numbers + sizeof(numbers) / sizeof(numbers[0]));
    return v;
}

void setJointPositions(arduinoSerial& Serial, std::vector<double> positions){
    std::string cmd = "SETX";
    for(double& p : positions){
        cmd += " " + std::to_string(p);
    }
    Serial.print(cmd);
}

void adjustJointPos(arduinoSerial& Serial, int id, double adjustment){
    std::vector<double> currPos = getJointPositions(Serial);
    currPos[id] += adjustment;
    setJointPositions(Serial, currPos);
}

int main(){
    arduinoSerial Serial;
    Serial.openPort("/dev/ttyACM0");
    Serial.begin(B9600);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    auto window = glfwCreateWindow(512, 512, "Arm controls", nullptr, nullptr);
    if(!window){
        throw std::runtime_error("ERR: Failed to create window");
    }
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    });
    glfwMakeContextCurrent(window);
    if(!gladLoaderLoadGL()){
        throw std::runtime_error("ERR: Failed to load GLAD");
    }
    if(DEBUG){
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debug::messageCallback, nullptr);
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);


        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A)){
            Serial.print("HOME");
            char response[256] = {0};
            // Wait a bit for the response
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            Serial.readBytes(response, 256);
            std::cout << response << "\n";
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F)){
            std::vector<double> jointPositions = getJointPositions(Serial);
            for(double& j : jointPositions){
                std::cout << j << ", ";
            }
            std::cout << "\n";
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Z)){
            adjustJointPos(Serial, 2, 15.0);
        }


        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}