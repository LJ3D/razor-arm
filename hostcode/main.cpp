#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "gl_debug.hpp"
#include "arduinoserialio/arduinoSerial.hpp"
#include <thread>

#define DEBUG true

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
            std::cout << response << std::endl;
        }


        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}