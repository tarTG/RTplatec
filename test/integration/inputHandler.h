/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   inputHandler.h
 * Author: tartg
 *
 * Created on 18. Oktober 2016, 10:35
 */

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>
#include <AntTweakBar.h>



class inputHandler {
public:
    inputHandler(GLFWwindow* window);
    
    TwBar* createNewBar(const std::string& description);
    
    void update();
    
    void exit();

private:
    GLFWwindow* window;
};

#endif /* INPUTHANDLER_H */

