/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FractalGenerator.h
 * Author: tg
 *
 * Created on 18. Dezember 2015, 16:14
 */

#ifndef FRACTALGENERATOR_H
#define FRACTALGENERATOR_H

#include <stdlib.h>


class FractalGenerator {
public:
//   static int sqrdmd(float* map, int size, float rgh);
   static int noiseTurbulence(float* map, int heigth, int length,int seed, double persistence, int turbRoughness);
   static void generateNoise(float* map, int heigth, int length, int seed, double persistence);
private:


};

#endif /* FRACTALGENERATOR_H */

