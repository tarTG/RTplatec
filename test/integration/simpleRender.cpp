/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   simpleRender.cpp
 * Author: tartg
 * 
 * Created on 18. Oktober 2016, 10:09
 */

#include "simpleRender.h"




simpleRender::simpleRender(u_int32_t windowHeight, uint32_t windowLength, std::string windowTitle) :
windowHeight(windowHeight),windowLength(windowLength), windowTitle(windowTitle)
{
}

u_int32_t simpleRender::init() 
{
    if (!glfwInit())
          return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(windowLength, windowHeight, windowTitle.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
        
    if(gl3wInit() !=  0)   
    {
        glfwTerminate();
        return 1;
    }
    return 0;
}

void simpleRender::clearWindow() 
{
    glClearColor(1.f, 1.f,1.f, 1.f);
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );    
}


void simpleRender::render()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(window);


    /* Poll for and process events */
     glfwPollEvents();
}



void simpleRender::exit()
{
    glfwDestroyWindow(window);
    
    glfwTerminate();
}

GLFWwindow* simpleRender::getWindow() const 
{
    return window;
}

