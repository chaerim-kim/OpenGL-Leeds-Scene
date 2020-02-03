# OpenGL-Leeds-Scene
The Leeds application renders a visual scene of Leeds Millennium square together with Leeds University’s renowned Bacon statue. It aims to create an interactive animated scene using OpenGL and C++, to provide festive yet realistic model of the scene.  

It provides a certain level of user interaction through slider which manipulates the camera angle and the horse speed. 

![1-1](https://user-images.githubusercontent.com/33334078/73649135-0492bf00-4677-11ea-988b-c04fcb3c51b0.png)

## Compilation
$ module add qt  
$ qmake  
$ make. 
$ ./Leeds  


## Objects in the Program
1. Leeds city council - texture mapping, light and material, made up of convex objects
2. Bacon statue - rotating, material
3. Christmas trees - instancing, material
4. Merry go round - spinning, material, motion, complex rotation
5. Moving horse - user interaction with horse leg, rotating, hierarchical modelling

## What I learned
* A hands-on learning of OpenGL and modular file structure.
* Various OpenGL methods including texture mapping, light and material setup, rotation, scaling and transitions;
* Matrix calculation and instancing of convex coordinates and vertex-normal calculation.
