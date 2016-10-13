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
    
    std::unique_ptr<lithosphere> ground = std::make_unique<lithosphere>(seed, width, length,seaLevel,0.01,8000000, 3.00,10,terrainNoiseRoughness );

    ground->update();
    return 0;
}

