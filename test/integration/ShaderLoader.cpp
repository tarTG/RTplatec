/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShaderLoader.cpp
 * Author: tartg
 * 
 * Created on 18. Oktober 2016, 12:39
 */

#include "ShaderLoader.h"

GLuint ShaderLoader::generateProgram(const std::string& pathName) 
{
    GLuint program = glCreateProgram();

    
    ShaderLoader::attachShader(program,pathName + ".vs.glsl",GL_VERTEX_SHADER);

    ShaderLoader::attachShader(program, pathName + ".tcs.glsl", GL_TESS_CONTROL_SHADER);

    ShaderLoader::attachShader(program, pathName + ".tes.glsl",GL_TESS_EVALUATION_SHADER);

    ShaderLoader::attachShader(program, pathName + ".gs.glsl", GL_GEOMETRY_SHADER);
    
    ShaderLoader::attachShader(program,pathName + ".fs.glsl",GL_FRAGMENT_SHADER);
    
    ShaderLoader::attachShader(program, pathName + ".cs.glsl",GL_COMPUTE_SHADER);
    
    
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (!status)
    {
        char buffer[4096];
        glGetProgramInfoLog(program, 4096, NULL, buffer);
        std::cout << "Error Program link in " << buffer << std::endl;
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

void ShaderLoader::attachShader(GLuint& program, std::string filename, GLenum shader_type) 
{
    GLuint ret =  loadShaders( filename,shader_type);
    if(ret != 0)
        glAttachShader(program, ret);
}



GLuint ShaderLoader::loadShaders(const std::string filename, GLenum shader_type)
{
    GLuint result = 0;
    FILE * fp;
    size_t filesize;
    char * data;
    
    fp = fopen(filename.c_str(), "rb");
    
    if (!fp)
        return result;
    
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    data =  new char [filesize + 1];    
    
    if (!data)
        return result;

    fread(data, 1, filesize, fp);
    data[filesize] = 0;
    fclose(fp);

    result = glCreateShader(shader_type);

    if (!result)
        return result;

    glShaderSource(result, 1, &data, NULL);


    glCompileShader(result);

    GLint status = 0;
    glGetShaderiv(result, GL_COMPILE_STATUS, &status);

    if (!status)
    {
        char buffer[4096] ;
        glGetShaderInfoLog(result, 4096, NULL, buffer);

        std::cout << "Fail Shader " << filename  << " in " << buffer << std::endl;
        glDeleteShader(result);
    }
    
    delete[] data;
    
    return result;

}