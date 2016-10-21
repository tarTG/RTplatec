# RTplatec
[![Build Status](https://travis-ci.org/tarTG/RTplatec.svg?branch=master)](https://travis-ci.org/tarTG/RTplatec)

A fork of platec (https://github.com/Mindwerks/plate-tectonics) to fit the needs of RTWG (https://github.com/tarTG/RTWG)


## Features

- Better start generation throught libnoise ( http://libnoise.sourceforge.net/ )
- Adjustment of parameters during runtime
- Option, to load a heightmap texture as a start terrain
- Apply your own errosion and write back into platec via litosphere->setTopography
- Example Programm uses Shader, which enables resizeable window
- small performance opitmisation


![](screenshots/RTplatecExample.png?raw=true)

## Reqirements

- For the integration test: a OpenGL 4.+ context
- a C++14 compiler
- currently only tested under Debian/Ubuntu-based Linux

## Installation

### Just the library 
- sudo apt-get install libnoise-dev
- go to the RTplatec folder
- cmake ..
- make all
- library can be found in build/src/libRTplatec.a

### Library and integration test
- go to the RTplatec folder
- sudo scipts/installDependencies.sh
    - will install glfw3, gl3w and anttweakbar
- cmake -DINTEGRATION_TEST=ON ..
- make all
- binary can be found in build/test/integration/main

## Further plans
- move platec code to modern C++ for performance improvements
- extend integration test (texture export, different shaders, more options)

## Credits
Credits go to the original platec developer Lauri Viitanen ( http://sourceforge.net/projects/platec/ )
and to @ftomassetti and his plate-tectonics project ( https://github.com/Mindwerks/plate-tectonics ) 
for massive improvement of platec.



 