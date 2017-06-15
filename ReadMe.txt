=============================================================================
    CONSOLE APPLICATION : ExplodingTeapot_2.0 Project Overview : Sean Halnon
=============================================================================

Controls:
Left click:  create explosion at click point.
Right click: reset mesh to original state


Description. 
Project uses transform feedback 2 process physics of triangles. When mesh is loaded into an openGL buffer, each vertex is assigned an integer vertex attribute that is
an offset into the a feedback buffer. There is one index for every triangle. After the physics is processed via transform feedback, the output buffer is bound as a 
texture buffer object, and the render vertex shader uses the index attribute for a texelfetch call to retrieve an offset from the feedback buffer. 


Shader:
There are 4 shaders. 
1 vertex and fragment shader for rendering
3 vertex shaders for transform feedback. 
   -An update shader that simply applies the velocity and gravity to the offset. The w component of the InitialLocation attribute is used as a sort of indicator whether
    the triangle shoudl interact wtih physics or not. 
   -An Explosion shader. This shader contains a uniform for a the explosion location, and adds velocity to the triangles in range of the explosion. If a triangle is
    in range, the w component of InitialLocation is set to 1 to indicate that physics should be processed for that triangle by the update shader. 
   -A reset shader. This Resets the offset and velocity attributes to 0 vectors. And it sets the w component of InitialLocation to -1 to indicate that gravity
    and physics processing should not be processed in the update shader. 


Model Information:
The model that is loaded by default has 4 teapot meshes, totaling 354,816 triangles. 

Performance information:
My system runs the software at about 1480 fps. 
  My system is using an Nvidia GTX680, an Intel i5 2500k, and has 8gigs of memory. 


Libraries used:
freeglut
GLM
GLEW