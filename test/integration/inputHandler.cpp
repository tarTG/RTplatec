/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   inputHandler.cpp
 * Author: tartg
 * 
 * Created on 18. Oktober 2016, 10:35
 */

#include "inputHandler.h"


// Callback function called by GLFW when window size changes
void  WindowSizeCB(int width, int height)
{
    glViewport(0, 0, width, height);
    // Send the new window size to AntTweakBar
    TwWindowSize(width, height);
}
inputHandler::inputHandler(GLFWwindow* window) : window(window)
{
    // Initialize AntTweakBar
    TwInit(TW_OPENGL, nullptr);
    // Set GLFW event callbacks
    // - Redirect window size changes to the callback function WindowSizeCB
    glfwSetWindowSizeCallback(window,(GLFWwindowsizefun)WindowSizeCB);
    // - Directly redirect GLFW mouse button events to AntTweakBar
    glfwSetMouseButtonCallback(window,(GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    // - Directly redirect GLFW mouse position events to AntTweakBar
    glfwSetCursorPosCallback(window,(GLFWcursorposfun)TwEventMousePosGLFW);
    // - Directly redirect GLFW mouse wheel events to AntTweakBar
    glfwSetScrollCallback(window,(GLFWscrollfun)TwEventMouseWheelGLFW);
    // - Directly redirect GLFW key events to AntTweakBar
    glfwSetKeyCallback(window,(GLFWkeyfun)TwEventKeyGLFW);
    // - Directly redirect GLFW char events to AntTweakBar
    glfwSetCharCallback(window,(GLFWcharfun)TwEventCharGLFW);
}

TwBar* inputHandler::createNewBar(const std::string& description) 
{
    return TwNewBar(description.c_str());
}


void inputHandler::update() 
{
    // Draw tweak bars
    TwDraw();

}


void inputHandler::exit() 
{
    TwTerminate();
}



