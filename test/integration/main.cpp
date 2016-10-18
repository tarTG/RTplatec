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
#include "simpleRender.h"
#include "inputHandler.h"
#include "ShaderLoader.h"
#include <GLFW/glfw3.h>
#include <chrono>


std::unique_ptr<lithosphere> ground;
float seed;
u_int32_t width = 600, length = 800;
float seaLevel = 0.5;
float terrainNoiseRoughness = 0.7;
    
void TW_CALL CBnewTectonic(void *clientData)
{ 
    static_cast<lithosphere*>(clientData)->restart();
}

void TW_CALL CBrestart(void *clientData)
{ 
    seed = rand();
    ground = std::make_unique<lithosphere>(seed,width, length, seaLevel, ground->folding_ratio,ground->aggr_overlap_abs, ground->aggr_overlap_rel,  ground->max_plates,terrainNoiseRoughness );

    TwBar* guiBar = static_cast<TwBar*>(clientData);
    
    TwRemoveVar(guiBar,"max Plates");
    TwRemoveVar(guiBar,"Folding Ratio");
    TwRemoveVar(guiBar,"Aggr Overlap Rel");
    TwRemoveVar(guiBar,"Aggr Overlap Abs");
    TwRemoveVar(guiBar,"New Tectonic");
    
   TwAddVarRW(guiBar,"max Plates",TW_TYPE_UINT32,&ground->max_plates,"group='Tectonic' min=1.0 max = 10.0 help='Max Plate count for next start.'" );
   TwAddVarRW(guiBar,"Folding Ratio",TW_TYPE_FLOAT,&ground->folding_ratio,"group='Tectonic' max=1.0 min=0.0 step='0.01' help='Percent of overlapping crust that's folded.' " );   
   TwAddVarRW(guiBar,"Aggr Overlap Rel",TW_TYPE_FLOAT,&ground->aggr_overlap_rel,"group='Tectonic' max=1.0 min=0.0 step='0.03' help='% of overlapping area -> aggregation.' " );   
   TwAddVarRW(guiBar,"Aggr Overlap Abs",TW_TYPE_UINT32,&ground->aggr_overlap_abs,"group='Tectonic' max=2000000 min=0.0 step='10000' help='# of overlapping pixels -> aggregation.' " );   

   TwAddButton(guiBar,"New Tectonic",CBnewTectonic,ground.get(),"group='Tectonic' help='Generate new Plates.'" );

}

void setTexture(GLuint texID, GLuint pixelBufferID, const float* data);

/*
 * 
 */
int main(int argc, char** argv) 
{
    
    float frameTime;
    bool enable_tectonic = true;
    u_int32_t windowLength = 800, windowHeight= 600;
    
    srand(time(NULL));
    seed = rand();

    ground = std::make_unique<lithosphere>(seed, width, length,seaLevel,0.01,8000000, 3.00,10,terrainNoiseRoughness );

    std::unique_ptr<simpleRender> render = std::make_unique<simpleRender>(windowLength,windowHeight,"RTplatec");
    std::unique_ptr<inputHandler> input = std::make_unique<inputHandler>(render->getWindow());
    
    TwBar* lithoParameter = input->createNewBar("Parameter for platec");
    TwAddVarRW(lithoParameter,"Noise Roughness",TW_TYPE_FLOAT,&terrainNoiseRoughness,"group='Tectonic' max=4 min = 0.1 step=0.1 help='set roughness of terrain' " );   
    TwAddVarRW(lithoParameter,"SeaLevel",TW_TYPE_FLOAT,&seaLevel,"group='Tectonic' max=1.0 min = 0.0 step=0.02 help='set sea Level for next terrain' " );   
    TwAddVarRW(lithoParameter,"enable/disable",TW_TYPE_BOOL32,&enable_tectonic,"group='Tectonic' help='Enable/Disable tectonic update.' " );
    TwAddVarRW(lithoParameter,"max Plates",TW_TYPE_UINT32,&ground->max_plates,"group='Tectonic' min=1.0 max = 10.0 help='Max Plate count for next start.'" );
    TwAddVarRW(lithoParameter,"Folding Ratio",TW_TYPE_FLOAT,&ground->folding_ratio,"group='Tectonic' max=1.0 min=0.0 step='0.01' help='% of overlapping crust that iss folded.' " );   
    TwAddVarRW(lithoParameter,"Aggr Overlap Rel",TW_TYPE_FLOAT,&ground->aggr_overlap_rel,"group='Tectonic' max=1.0 min=0.0 step='0.03' help='% of overlapping area -> aggregation.' " );   
    TwAddVarRW(lithoParameter,"Aggr Overlap Abs",TW_TYPE_UINT32,&ground->aggr_overlap_abs,"group='Tectonic' max=2000000 min=0.0 step='10000' help='# of overlapping pixels -> aggregation.' " );   
    TwAddButton(lithoParameter,"New Tectonic",CBnewTectonic,ground.get(),"group='Tectonic' help='Generate new Plates.'" );
    TwAddButton(lithoParameter,"Restart",CBrestart,&lithoParameter,"group='Tectonic' help='Restart world.'" );

    TwAddVarRO(lithoParameter,"Frames",TW_TYPE_FLOAT,&frameTime," help='Current Frames per Seconds' " );   

    
    if(render->init() != 0)
    {
        std::cout << "Failed to init window!";
        return 0;
    }

    auto timeBeforeLoop =  std::chrono::high_resolution_clock::now();        
    glViewport(0, 0, windowLength, windowHeight);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint shaderID = ShaderLoader::generateProgram(std::string(SHADER_PATH )+ std::string("simpleDisplay"));
     GLuint textureID;
     GLuint pixelBuffer;
    glGenBuffers(1,&pixelBuffer);
    glGenTextures(1,&textureID);  //generate Texture ID

    glBindTexture(GL_TEXTURE_2D, textureID);

     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //setup Opengl Texture
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER); //setup Opengl Texture
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);    
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, width, length);

    glBindTexture(GL_TEXTURE_2D, 0);

    
    
            //0x891A = GL_CLAMP_VERTEX_COLOR, 0x891B = GL_CLAMP_FRAGMENT_COLOR
    glClampColor(0x891A, GL_FALSE);
    glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
    glClampColor(0x891B, GL_FALSE);
     while( !glfwGetKey(render->getWindow(),GLFW_KEY_ESCAPE))
    {
        timeBeforeLoop =  std::chrono::high_resolution_clock::now();
        
        if(enable_tectonic)
        {
           ground->update();
           setTexture(textureID,pixelBuffer,ground->getTopography());
        }

        render->clearWindow();
        glBindVertexArray(vao);
        glUseProgram(shaderID);
        glActiveTexture(GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1f(glGetUniformLocation(shaderID, "windowLength"),windowLength);
         glUniform1f(glGetUniformLocation(shaderID, "windowHeigth"),windowHeight);      
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
        render->render();
         input->update();
        frameTime = 1.f/(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timeBeforeLoop).count()/1000.f);        // the difference
  
    }
    
            //0891A = GL_CLAMP_VERTEX_COLOR, 0x891B = GL_CLAMP_FRAGMENT_COLOR
    glClampColor(0x891A, GL_TRUE);
    glClampColor(GL_CLAMP_READ_COLOR, GL_TRUE);
    glClampColor(0x891B, GL_TRUE);
    
    input->exit();
    render->exit();
    
    glDeleteProgram(shaderID);
    glDeleteVertexArrays(1,&vao);
    glDeleteTextures(1,&textureID);
    glDeleteBuffers(1,&pixelBuffer);

    return 0;
}


void setTexture(GLuint texID, GLuint pixelBufferID, const float* data)
{
        glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);  
    
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,pixelBufferID);        

    glBufferData(GL_PIXEL_UNPACK_BUFFER,length* width * sizeof(float),0,GL_STREAM_DRAW);
    GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER,GL_WRITE_ONLY);
    if(ptr )
    {
        memcpy(ptr,data,length* width * sizeof(float));
    }        

    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    //load texture to graphic card
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,  width, length,  GL_RED, GL_FLOAT,0); 

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
    glBindTexture(GL_TEXTURE_2D,0);
}