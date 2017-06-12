// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <vector>
#include<array>
#include <math.h>


#define GLEW_STATIC
#include <GL/glew.h>
#include "GL/freeglut.h"
#include <GL/gl.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/ext.hpp"

#include "MeshLoader.h"
#include "GeometryBuffer.h"
#include "TransformFeedback.h"
#include "ShaderTools.h"
#include "MeshInstance.h";
#include "Camera.h";
#include "UniformTools.h";



// TODO: reference additional headers your program requires here
