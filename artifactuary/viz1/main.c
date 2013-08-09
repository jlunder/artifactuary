/**
 * OpenGL ES 2.0 - Example 01
 *
 * @author	Norbert Nopper norbert@nopper.tv
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

#include <stdio.h>

#include "GLES2/glus.h"

/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslate(0.0f, 0.0f, -8.0f);

    return GLUS_TRUE;
}

/**
 * Function is called before first update and every time when the window is resized.
 *
 * @param w	width of the window
 * @param h	height of the window
 */
GLUSvoid reshape(GLUSint width, GLUSint height)
{
    // Set the viewport depending on the width and height of the window.
    glViewport(0, 0, width, height);
}

/**
 * Function to render and display content. Swapping of the buffers is automatically done.
 *
 * @return true for continuing, false to exit the application
 */
GLUSboolean update(GLUSfloat time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    return GLUS_TRUE;
}

/**
 * Function to clean up things.
 */
GLUSvoid terminate(GLUSvoid)
{
}

/**
 * Main entry point.
 */
int main(int argc, char* argv[])
{
	EGLint eglAttributes[] = {
	        EGL_RED_SIZE, 8,
	        EGL_GREEN_SIZE, 8,
	        EGL_BLUE_SIZE, 8,
	        EGL_DEPTH_SIZE, 16,
	        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	        EGL_NONE
	};

    glusInitFunc(init);

    glusReshapeFunc(reshape);

    glusUpdateFunc(update);

    glusTerminateFunc(terminate);

    if (!glusCreateWindow("GLUS Example Window", 640, 480, eglAttributes, GLUS_FALSE))
    {
        printf("Could not create window!\n");
        return -1;
    }

    glusRun();

    return 0;
}
