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

//Some Global Variables for main. Is ugly, but a lot easier to implement, for 
//test purposes.
std::unique_ptr<lithosphere> ground;  //Global refrenence to lithosphere
float seed;                           //current used random seed
u_int32_t textureWidth = 600, textureLength = 800; // start texture Dimensions
float seaLevel = 0.5;                 //start sea level
float terrainNoiseRoughness = 0.7;    //start terrain noise roughness
    
/**
 * AntTweak callback to make new plates
 * @param clientData - is void, but pointer to lithosphere* is used
 */
void TW_CALL CBnewTectonic(void *clientData)
{ 
    static_cast<lithosphere*>(clientData)->restart();
}

/**
 * AntTweak callback to restart the world with the same random seed
 * @param clientData - is void, but pointer to TwBar* is used
 */
void TW_CALL CBrestart(void *clientData)
{ 
    //generate new lithosphere. Old lithosphere should be destructed through unique_ptr
    ground = std::make_unique<lithosphere>(seed,textureWidth, textureLength, seaLevel, ground->folding_ratio,ground->aggr_overlap_abs, ground->aggr_overlap_rel,  ground->max_plates,terrainNoiseRoughness );
    //get gui bar
    TwBar* guiBar = static_cast<TwBar*>(clientData);
    
    //remove the old references
    TwRemoveVar(guiBar,"max Plates");
    TwRemoveVar(guiBar,"Folding Ratio");
    TwRemoveVar(guiBar,"Aggr Overlap Rel");
    TwRemoveVar(guiBar,"Aggr Overlap Abs");
    TwRemoveVar(guiBar,"New Tectonic");
    
    //set new Gui references of the new lithosphre
   TwAddVarRW(guiBar,"max Plates",TW_TYPE_UINT32,&ground->max_plates,"group='Tectonic' min=1.0 max = 10.0 help='Max Plate count for next start.'" );
   TwAddVarRW(guiBar,"Folding Ratio",TW_TYPE_FLOAT,&ground->folding_ratio,"group='Tectonic' max=1.0 min=0.0 step='0.01' help='Percent of overlapping crust that is folded.' " );   
   TwAddVarRW(guiBar,"Aggr Overlap Rel",TW_TYPE_FLOAT,&ground->aggr_overlap_rel,"group='Tectonic' max=1.0 min=0.0 step='0.03' help='% of overlapping area -> aggregation.' " );   
   TwAddVarRW(guiBar,"Aggr Overlap Abs",TW_TYPE_UINT32,&ground->aggr_overlap_abs,"group='Tectonic' max=2000000 min=0.0 step='10000' help='# of overlapping pixels -> aggregation.' " );   
   TwAddButton(guiBar,"New Tectonic",CBnewTectonic,ground.get(),"group='Tectonic' help='Generate new Plates.'" );

}

/**
 * AntTweak callback to generate a new world
 * @param clientData - is void, but pointer to TwBar* is used
 */
void TW_CALL CBnewWorld(void *clientData)
{ 
    seed = rand(); //generate new seed
    //generate new lithosphere. Old lithosphere should be destructed through unique_ptr
    ground = std::make_unique<lithosphere>(seed,textureWidth, textureLength, seaLevel, ground->folding_ratio,ground->aggr_overlap_abs, ground->aggr_overlap_rel,  ground->max_plates,terrainNoiseRoughness );
    //get gui bar
    TwBar* guiBar = static_cast<TwBar*>(clientData);
    
    //remove the old references
    TwRemoveVar(guiBar,"max Plates");
    TwRemoveVar(guiBar,"Folding Ratio");
    TwRemoveVar(guiBar,"Aggr Overlap Rel");
    TwRemoveVar(guiBar,"Aggr Overlap Abs");
    TwRemoveVar(guiBar,"New Tectonic");
    
    //set new Gui references of the new lithosphre
   TwAddVarRW(guiBar,"max Plates",TW_TYPE_UINT32,&ground->max_plates,"group='Tectonic' min=1.0 max = 10.0 help='Max Plate count for next start.'" );
   TwAddVarRW(guiBar,"Folding Ratio",TW_TYPE_FLOAT,&ground->folding_ratio,"group='Tectonic' max=1.0 min=0.0 step='0.01' help='Percent of overlapping crust that is folded.' " );   
   TwAddVarRW(guiBar,"Aggr Overlap Rel",TW_TYPE_FLOAT,&ground->aggr_overlap_rel,"group='Tectonic' max=1.0 min=0.0 step='0.03' help='% of overlapping area -> aggregation.' " );   
   TwAddVarRW(guiBar,"Aggr Overlap Abs",TW_TYPE_UINT32,&ground->aggr_overlap_abs,"group='Tectonic' max=2000000 min=0.0 step='10000' help='# of overlapping pixels -> aggregation.' " );   
   TwAddButton(guiBar,"New Tectonic",CBnewTectonic,ground.get(),"group='Tectonic' help='Generate new Plates.'" );

}

/**
 * update a texture on the graphic card
 * @param texID - ID of the texture
 * @param pixelBufferID - ID of the pixel buffer
 * @param data - pointer to the data
 */
void setTexture(GLuint texID, GLuint pixelBufferID, const float* data);

/*
 * 
 */
int main(int argc, char** argv) 
{
    
    float frameTime; //current frame time
    bool enable_tectonic = true; //bool for enabling/disabling tectonic
    bool renderAgeMap = false; //bool for enabling/disabling age map rendering
    int windowLength = 800, windowHeight= 600; //start values for window dimensions
    
    srand(time(NULL)); //initialize random number generator
    seed = rand();     //generate a random number

    //generate a new lithosphere.
    ground = std::make_unique<lithosphere>(seed, textureWidth, textureLength,seaLevel,0.01,8000000, 3.00,10,terrainNoiseRoughness );

    //initalize rendering
    std::unique_ptr<simpleRender> render = std::make_unique<simpleRender>(windowLength,windowHeight,"RTplatec");
    
    if(render->init() != 0) //if something faild
    {
        std::cout << "Failed to init window!";
        return 0; //close program
    }
    //initalize input handler
    std::unique_ptr<inputHandler> input = std::make_unique<inputHandler>(render->getWindow());
    
    //generate a GUI bar and set values
    TwBar* lithoParameter = input->createNewBar("Parameter"); 
    TwDefine("Parameter position='8 8' size='200 400' refresh=0.015"); 
    TwAddVarRO(lithoParameter,"Frames",TW_TYPE_FLOAT,&frameTime," help='Current Frames per Seconds' " );   
    TwAddVarRW(lithoParameter,"Noise Roughness",TW_TYPE_FLOAT,&terrainNoiseRoughness,"group='Tectonic' max=4 min = 0.1 step=0.1 help='set roughness of terrain' " ); 
    TwAddButton(lithoParameter,"Restart",CBrestart,lithoParameter,"group='Tectonic' help='Restart world.'" );    
    TwAddButton(lithoParameter,"New World",CBnewWorld,lithoParameter,"group='Tectonic' help='Generate a new world.'" );    
    TwAddVarRW(lithoParameter,"SeaLevel",TW_TYPE_FLOAT,&seaLevel,"group='Tectonic' max=1.0 min = 0.0 step=0.02 help='set sea Level for next terrain' " );   
    TwAddVarRW(lithoParameter,"enable/disable",TW_TYPE_BOOLCPP,&enable_tectonic,"group='Tectonic' help='Enable/Disable tectonic update.' " );
    TwAddVarRW(lithoParameter,"Render age map",TW_TYPE_BOOLCPP,&renderAgeMap,"group='Tectonic' help='Render Age Map.' " );
    TwAddVarRW(lithoParameter,"max Plates",TW_TYPE_UINT32,&ground->max_plates,"group='Tectonic' min=1.0 max = 10.0 help='Max Plate count for next start.'" );
    TwAddVarRW(lithoParameter,"Folding Ratio",TW_TYPE_FLOAT,&ground->folding_ratio,"group='Tectonic' max=1.0 min=0.0 step='0.01' help='% of overlapping crust that iss folded.' " );   
    TwAddVarRW(lithoParameter,"Aggr Overlap Rel",TW_TYPE_FLOAT,&ground->aggr_overlap_rel,"group='Tectonic' max=1.0 min=0.0 step='0.03' help='% of overlapping area -> aggregation.' " );   
    TwAddVarRW(lithoParameter,"Aggr Overlap Abs",TW_TYPE_UINT32,&ground->aggr_overlap_abs,"group='Tectonic' max=2000000 min=0.0 step='10000' help='# of overlapping pixels -> aggregation.' " );   
    TwAddButton(lithoParameter,"New Tectonic",CBnewTectonic,ground.get(),"group='Tectonic' help='Generate new Plates.'" );

    //initialize time counter
    auto timeBeforeLoop =  std::chrono::high_resolution_clock::now();        
    glViewport(0, 0, windowLength, windowHeight); //set Viewport
    
    /** Init OpenGL stuff*/
    GLuint vao; //generate Vertex Buffer Object
    glGenVertexArrays(1, &vao); 
    glBindVertexArray(vao);
    //load Shader
    GLuint shaderID = ShaderLoader::generateProgram(std::string(SHADER_PATH )+ std::string("simpleDisplay"));
     GLuint textureID;
     GLuint pixelBuffer;
    glGenBuffers(1,&pixelBuffer); //generate pixel Buffer
    glGenTextures(1,&textureID);  //generate Texture ID

    glBindTexture(GL_TEXTURE_2D, textureID); //bind Texture

     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //setup Opengl Texture
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER); //setup Opengl Texture
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);    
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, textureWidth, textureLength); //resvere Memory on openGL

    glBindTexture(GL_TEXTURE_2D, 0);

    
    
     //0x891A = GL_CLAMP_VERTEX_COLOR, 0x891B = GL_CLAMP_FRAGMENT_COLOR
    //nessesarry to allow values greater than 1.0 on textures
    glClampColor(0x891A, GL_FALSE);
    glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
    glClampColor(0x891B, GL_FALSE);
     while( !glfwGetKey(render->getWindow(),GLFW_KEY_ESCAPE)) //as long as "ESCAPE" isn't pressed
    {
         
        timeBeforeLoop =  std::chrono::high_resolution_clock::now(); //get timestamp
        //get current window size
        glfwGetWindowSize(render->getWindow(),&windowLength, &windowHeight);
        
        if(enable_tectonic) //if tectonic enabled
        {
           ground->update(); //update lithosphere
        }
        if(!renderAgeMap) //if render heigth map
        {
           setTexture(textureID,pixelBuffer,ground->getTopography()); //set texture
        }
        else //if render age map
        {
            //ugly workaround to replace int values to float value and normalize them
            uint32_t A = ground->getHeight() * ground->getWidth(); //get size
            std::vector<uint32_t> amap = std::vector<uint32_t>(ground->getAgemap(),ground->getAgemap() + A); //get Age map
            float lowest = *std::min_element(amap.begin(), amap.end()); //find min value
            float highest = *std::max_element(amap.begin(), amap.end()); //find max value
            std::vector<float> amapFloat = std::vector<float>(); //make a float vector
            amapFloat.reserve(A);
            //normalize age map and write into float vector, maybe this can happen on GPU
            std::transform(amap.begin(), amap.end(),amapFloat.begin(),[&](auto a){return (a - lowest) / (float)(highest - lowest);});
            //finially set texture
            setTexture(textureID,pixelBuffer,amapFloat.data());
        }

        render->clearWindow(); //clear window
        glBindVertexArray(vao); //bind vertex array object
        glUseProgram(shaderID); //set shader program
        glActiveTexture(GL_TEXTURE0 ); // set texture
        glBindTexture(GL_TEXTURE_2D, textureID);
        //set uniform values
        glUniform1f(glGetUniformLocation(shaderID, "windowLength"),windowLength);
         glUniform1f(glGetUniformLocation(shaderID, "windowHeigth"),windowHeight); 
        glUniform1i(glGetUniformLocation(shaderID, "renderAgeMap"),renderAgeMap);
         
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //draw image
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
         input->update(); //handle input
        render->render(); //swap buffers and render

        //calculate frame time
        frameTime = 1.f/(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timeBeforeLoop).count()/1000.f);        // the difference
  
    }
    
            //0891A = GL_CLAMP_VERTEX_COLOR, 0x891B = GL_CLAMP_FRAGMENT_COLOR
    glClampColor(0x891A, GL_TRUE);
    glClampColor(GL_CLAMP_READ_COLOR, GL_TRUE);
    glClampColor(0x891B, GL_TRUE);
    
    
        //delete all opengl stuff
    glDeleteProgram(shaderID);
    glDeleteVertexArrays(1,&vao);
    glDeleteTextures(1,&textureID);
    glDeleteBuffers(1,&pixelBuffer);
    
    
    input->exit(); //close input stuff
    render->exit(); //close opengl Context
    

    //quit
    return 0;
}


void setTexture(GLuint texID, GLuint pixelBufferID, const float* data)
{
    //set and bind texture
        glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);  
    //bind pixel buffer
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,pixelBufferID);        
    //buffer data for upload
    glBufferData(GL_PIXEL_UNPACK_BUFFER,textureLength* textureWidth * sizeof(float),0,GL_STREAM_DRAW);
    GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER,GL_WRITE_ONLY); //get pointer to unpack buffer
    if(ptr ) //if pointer is available
    {
        memcpy(ptr,data,textureLength* textureWidth * sizeof(float)); //copy data from texture to buffer
    }        

    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER); //close buffer
    //load buffer to graphic card
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,  textureWidth, textureLength,  GL_RED, GL_FLOAT,0); 

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0); //unbind everything
    glBindTexture(GL_TEXTURE_2D,0);
}