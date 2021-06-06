

# PPG - Procedural Puzzle Generator

My abandoned bachelor's project experimenting with procedurally generating puzzles for video games.

## Disclaimer:

You won't find _good_ C++ code here. At the time I wrote this, I kind of only knew Java and C, so this is the result of my naive C++ implementation. Only later I found out about all the good principles that come with modern (and actual) C++ which I did not use here.

__This is the core repository. For extending projects please refer to these links:__

[ppg-yaml; YAML extension for the use with ppg-core](https://github.com/dAmihl/ppg-yaml)

[gdppg; A Godot module for ppg-yaml](https://github.com/dAmihl/gdppg)

## Examples:

The best example I can give is within my ["paper"](/doc/Prozedurale_Generation_von_Raetseln_EggerM.pdf)

I have also created a small game in Godot Engine using this system which can be found here: [simplePPG](https://github.com/dAmihl/simplePPG)

## Build:

Use cmakebuild.sh to run CMAKE and create build/ directory

Use cmake.exe --build build/ --target INSTALL to install library and include paths

or cmake.exe --build build/ --target INSTALL --config Release
