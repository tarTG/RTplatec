/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShaderLoader.h
 * Author: tartg
 *
 * Created on 18. Oktober 2016, 12:39
 */

#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include "../3rdParty/GL3W/gl3w.h"
#include "../3rdParty/GL3W/glcorearb.h"
#include <string>
#include <iostream>

class ShaderLoader {
public:
    static GLuint generateProgram(const std::string& pathName);
private:
    static GLuint loadShaders(const std::string filename, GLenum shader_type);
    static void attachShader(GLuint& program, std::string filename,GLenum shader_type);



 

};

#endif /* SHADERLOADER_H */

