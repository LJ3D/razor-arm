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
    Serial.print("READ\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Serial.readBytes(response, 256);
    std::cout << "getJointPositions(): Read response: " << response << std::endl;
    double numbers[6] = {157.5,157.5,187.5,57.5,157.5,90};
    if(std::sscanf(response, "READ\n[%lf,%lf,%lf,%lf,%lf,%lf]", &numbers[0], &numbers[1], &numbers[2], &numbers[3], &numbers[4], &numbers[5]) != 6){
        std::cout << "ERR: Failed to parse response\n";
    }else{
        std::cout << "getJointPositions(): Parsed response: " << numbers[0] << ", " << numbers[1] << ", " << numbers[2] << ", " << numbers[3] << ", " << numbers[4] << ", " << numbers[5] << "\n";
    }
    std::vector<double> v(numbers, numbers + sizeof(numbers) / sizeof(numbers[0]));
    return v;
}

void setJointPositions(arduinoSerial& Serial, std::vector<double> positions){
    std::string cmd = "SETX";
    for(double& p : positions){
        cmd += " " + std::to_string(p);
    }
    std::cout << "setJointPositions(): Sending command: " << cmd << "\n";
    Serial.print(cmd+"\n");
}

void adjustJointPos(arduinoSerial& Serial, int id, double adjustment){
    std::vector<double> currPos = getJointPositions(Serial);
    currPos[id] += adjustment;
    setJointPositions(Serial, currPos);
}

int main(){
    // Initialise serial communication
    arduinoSerial Serial;
    Serial.openPort("/dev/ttyACM0");
    Serial.begin(B9600);

    // GLFW + OpenGL boilerplate
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
    // End GLFW + OpenGL boilerplate

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents(); // Check for events (keyboard, mouse, etc)

        /*
            Home the arm
        */
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_L)){
            Serial.print("HOME\n");
            char response[256] = {0};
            // Wait a bit for the response
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            Serial.readBytes(response, 256);
            std::cout << response << "\n";
        }

        /*
            Print the current joint positions (for debugging, etc)
        */
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_P)){
            std::vector<double> jointPositions = getJointPositions(Serial);
            for(double& j : jointPositions){
                std::cout << j << ", ";
            }
            std::cout << "\n";
        }

        /*
            A horribly in-ergonomic way of controlling the arm :)
        */
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_R)){
            adjustJointPos(Serial, 0, 15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F)){
            adjustJointPos(Serial, 0, -15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Q)){
            adjustJointPos(Serial, 1, 15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_E)){
            adjustJointPos(Serial, 1, -15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A)){
            adjustJointPos(Serial, 2, 15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D)){
            adjustJointPos(Serial, 2, -15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W)){
            adjustJointPos(Serial, 3, 15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S)){
            adjustJointPos(Serial, 3, -15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)){
            adjustJointPos(Serial, 4, 15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)){
            adjustJointPos(Serial, 4, -15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)){
            adjustJointPos(Serial, 5, 15.0);
        }
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)){
            adjustJointPos(Serial, 5, -15.0);
        }

        glfwSwapBuffers(window); // Render the current frame
    }

    // Cleanup
    glfwTerminate();
    return 0;
}