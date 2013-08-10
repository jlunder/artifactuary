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
    //printf("Vertex shader:\n%s\n--------\n", vertexSource.text);
    //printf("Fragment shader:\n%s\n--------\n", fragmentSource.text);
    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);
    
    g_viewProjectionMatrixLocation = glGetUniformLocation(g_program.program, "u_viewProjectionMatrix");
    g_modelMatrixLocation = glGetUniformLocation(g_program.program, "u_modelMatrix");
    
    //printf("uniforms: %d %d\n", g_viewProjectionMatrixLocation, g_modelMatrixLocation);
    
    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");
    g_colorLocation = glGetAttribLocation(g_program.program, "a_color");
    g_texCoordLocation = glGetAttribLocation(g_program.program, "a_texCoord");
    
    //printf("attributes: %d %d %d\n", g_vertexLocation, g_colorLocation, g_texCoordLocation);
    
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClearDepthf(1.0f);
    
    GLfloat lightVertices[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f,
    };
    
    GLfloat lightColors[] = {
        1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f,
    };
    
    GLfloat lightTexCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    
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
    
    /*
    glusLookAtf(viewMatrix,
        0.0f, 0.0f, 8.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    glusPerspectivef(viewProjectionMatrix, 45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
    glusMatrix4x4Multiplyf(viewProjectionMatrix, viewProjectionMatrix, viewMatrix);
    */
    
    glusMatrix4x4Identityf(viewProjectionMatrix);
    for(int j = 0; j < 4; ++j) {
        for(int i = 0; i < 4; ++i) {
            printf("%g ", viewProjectionMatrix[j * 4 + i]);
        }
        printf("\n");
    }
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
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glUseProgram(g_program.program);
    
    glusMatrix4x4Identityf(modelMatrix);
    glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_colorsVBO);
    glVertexAttribPointer(g_colorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_colorLocation);
    
    glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsVBO);
    glVertexAttribPointer(g_texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_texCoordLocation);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDisableVertexAttribArray(g_vertexLocation);
    glDisableVertexAttribArray(g_colorLocation);
    glDisableVertexAttribArray(g_texCoordLocation);
    
    
    glUseProgram(0);
    
    
    return GLUS_TRUE;
}

/**
 * Function to clean up things.
 */
GLUSvoid terminate(GLUSvoid)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &g_verticesVBO);
    glDeleteBuffers(1, &g_colorsVBO);
    glDeleteBuffers(1, &g_texCoordsVBO);
    
    glUseProgram(0);
    
    glusDestroyProgram(&g_program);
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
