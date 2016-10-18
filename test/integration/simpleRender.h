/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   simpleRender.h
 * Author: tartg
 *
 * Created on 18. Oktober 2016, 10:08
 */

#ifndef SIMPLERENDER_H
#define SIMPLERENDER_H

#include "gl3w.h"
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <string>

class simpleRender 
{
public:
    simpleRender(u_int32_t windowHeight, uint32_t windowLength,std::string windowTitle);
    
    u_int32_t init();
    
    void clearWindow();
    
    void render();
    
    void exit();
    
    GLFWwindow* getWindow() const;
    
    u_int32_t windowHeight,windowLength;


private:
    std::string windowTitle;
    GLFWwindow* window;
};

#endif /* SIMPLERENDER_H */

