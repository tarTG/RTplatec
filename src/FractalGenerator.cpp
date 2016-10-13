/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FractalGenerator.cpp
 * Author: tg
 * 
 * Created on 18. Dezember 2015, 16:14
 */

#include "FractalGenerator.h"
#include <time.h>
#include <libnoise/noise.h>
#include <stdio.h>
#include "../3rdParty/libNoiseUtils/noiseutils.h"

using namespace noise;


int FractalGenerator::noiseTurbulence(float* map, int heigth, int length,int seed, double persistence, int turbRoughness)
{
    module::Perlin perModule;
    perModule.SetPersistence(persistence);
    perModule.SetSeed(seed);
    module::Turbulence turb;
    turb.SetSourceModule(0,perModule);
    turb.SetRoughness(turbRoughness);
    turb.SetSeed(seed);
    
    utils::NoiseMap heigthMap;
    utils::NoiseMapBuilderPlane mapBuilder;
    mapBuilder.SetSourceModule(turb);
    mapBuilder.SetDestNoiseMap(heigthMap);
    mapBuilder.SetDestSize(heigth,length);
    mapBuilder.SetBounds(0.0,4.0,0.0,3.0);
        mapBuilder.EnableSeamless();
    mapBuilder.Build();
    
    memcpy(map,heigthMap.GetConstSlabPtr(),heigth*length*sizeof(float));
}


void FractalGenerator::generateNoise(float* map, int heigth, int length, int seed, double persistence)
{
    module::Perlin perModule;
    perModule.SetSeed(seed);
    perModule.SetPersistence(persistence);
    utils::NoiseMap heigthMap;
    utils::NoiseMapBuilderPlane mapBuilder;
    mapBuilder.SetSourceModule(perModule);
    mapBuilder.SetDestNoiseMap(heigthMap);
    mapBuilder.SetDestSize(heigth,length);
    mapBuilder.SetBounds(0.0,4.0,0.0,5.0);
    mapBuilder.EnableSeamless();
    mapBuilder.Build();
    
    memcpy(map,heigthMap.GetConstSlabPtr(),heigth*length*sizeof(float));    
}