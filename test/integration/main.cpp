/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: tartg
 *
 * Created on 13. Oktober 2016, 10:57
 */

#include <cstdlib>
#include <memory>
#include "lithosphere.hpp"
#include "gl3w.h"
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{
    
    float seed = 0.1;
    u_int32_t width = 600, length = 800;
    float seaLevel = 0.5;
    float terrainNoiseRoughness = 0.7;
    u_int32_t windowLenght =800,windowHeigth =600;
    std::unique_ptr<lithosphere> ground = std::make_unique<lithosphere>(seed, width, length,seaLevel,0.01,8000000, 3.00,10,terrainNoiseRoughness );

    if (!glfwInit())
          return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(windowLenght, windowHeigth, "RTplatec", NULL, NULL);
    if (!window)
      {
          glfwTerminate();
          return 0;
      }
    
        glfwMakeContextCurrent(window);
        
         if(gl3wInit() !=  0)   
         {
           glfwTerminate();
            return 0;
         }
    // Set GLFW event callbacks
    // - Redirect window size changes to the callback function WindowSizeCB
  //  glfwSetWindowSizeCallback(window,WindowSizeCB);
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
    
      while( !glfwGetKey(window,GLFW_KEY_ESCAPE))
    {
            ground->update();
            /* Swap front and back buffers */
            glfwSwapBuffers(window);


            /* Poll for and process events */
             glfwPollEvents();
    
      }
        glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}

