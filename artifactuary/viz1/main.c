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


static GLUSshaderprogram g_program;

static GLint g_viewProjectionMatrixLocation;
static GLint g_modelMatrixLocation;

static GLint g_vertexLocation;
static GLint g_colorLocation;
static GLint g_texCoordLocation;

static GLuint g_verticesVBO;
static GLuint g_colorsVBO;
static GLuint g_texCoordsVBO;
static GLuint g_indicesVBO;

//

static GLuint g_texture;

//

static GLuint g_numberIndices;


/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;
    
    glusLoadTextFile("light.vert.glsl", &vertexSource);
    glusLoadTextFile("light.frag.glsl", &fragmentSource);
    glusBuildProgramFromSource(&g_program, (const GLUSchar**)&vertexSource.text, (const GLUSchar**)&fragmentSource.text);
    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);
    
    g_viewProjectionMatrixLocation = glGetUniformLocation(g_program.program, "u_viewProjectionMatrix");
    g_modelMatrixLocation = glGetUniformLocation(g_program.program, "u_modelMatrix");
    
    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");
    g_colorLocation = glGetAttribLocation(g_program.program, "a_color");
    g_texCoordLocation = glGetAttribLocation(g_program.program, "a_texCoord");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClearDepthf(1.0f);
    
    GLfloat lightVertices[] = {
        -1.0f, -1.0f, -8.0f, 0.0f,
        -1.0f,  1.0f, -8.0f, 0.0f,
         1.0f,  1.0f, -8.0f, 0.0f,
         1.0f, -1.0f, -8.0f, 0.0f,
    };
    
    GLfloat lightColors[] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };
    
    GLfloat lightTexCoords[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };
    
    GLushort lightIndices[] = {0, 1, 2, 0, 2, 3};
    
    g_numberIndices = 6;
    
    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof (GLfloat), (GLfloat*)lightVertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &g_colorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof (GLfloat), (GLfloat*)lightColors, GL_STATIC_DRAW);
    
    glGenBuffers(1, &g_texCoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof (GLfloat), (GLfloat*)lightTexCoords, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &g_indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_numberIndices * sizeof (GLushort), (GLuint*)lightIndices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glUseProgram(g_program.program);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_colorsVBO);
    glVertexAttribPointer(g_colorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_colorLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsVBO);
    glVertexAttribPointer(g_texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_texCoordLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);

/*    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslate(0.0f, 0.0f, -8.0f);
*/
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
    GLfloat viewMatrix[16];
    GLfloat viewProjectionMatrix[16];
    
    // Set the viewport depending on the width and height of the window.
    glViewport(0, 0, width, height);
    
    glusLookAtf(viewMatrix,
        0.0f, 0.0f, 8.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    glusPerspectivef(viewProjectionMatrix, 45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
    glusMatrix4x4Multiplyf(viewProjectionMatrix, viewProjectionMatrix, viewMatrix);
    glUniformMatrix4fv(g_viewProjectionMatrixLocation, 1, GL_FALSE, viewProjectionMatrix);
}

/**
 * Function to render and display content. Swapping of the buffers is automatically done.
 *
 * @return true for continuing, false to exit the application
 */
GLUSboolean update(GLUSfloat time)
{
    GLfloat modelMatrix[16];
    
    glusMatrix4x4Identityf(modelMatrix);
    glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    
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
